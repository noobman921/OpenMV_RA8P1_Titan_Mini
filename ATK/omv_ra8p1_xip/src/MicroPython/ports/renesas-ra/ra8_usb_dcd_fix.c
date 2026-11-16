/*
 * RA8P1 USBHS PIPEBUF 分配修复 —— tinyusb 上游缺口 1（仅 HS 生效，FS 下是空操作）
 *
 * 问题：上游 src/portable/renesas/rusb2/dcd_rusb2.c 在 HS 下把 PIPEBUF
 * 写死为 0x7C08（BUFNMB[7:0]=8 → 起始块 8；BUFSIZE[14:10]=31 → 每 bank
 * 2048B）。每个 pipe 起始地址完全相同，CDC 的 IN/OUT 两个 HS bulk pipe
 * 互相踩。上游自带 FIXME："a real per-pipe allocation scheme is needed"。
 * FS 下 mps 只有 64 不出问题，只有切 HS 才暴露。
 *
 * 做法（照 Titan_Mini_Openmv/platform/drivers/ra8_usb_dcd.c 的手法）：
 * 不复制上游文件。把上游 dcd_rusb2.c 整个 include 进本编译单元，只把要
 * 替换的两个函数改名，再在本文件里定义同名包装 —— 包装调用上游原实现，
 * 之后按 pipe 分配互不重叠的 FIFO 区域。
 *
 * 启用前提（缺一不可）：
 *   1. lib/tinyusb 已换成 0.21（0.19 的 HS 路径缺 tu_hwfifo 等，不适用）；
 *   2. .cproject 的 src 排除串里加上
 *      MicroPython/lib/tinyusb/src/portable/renesas/rusb2/dcd_rusb2.c
 *      否则 dcd_edpt_open 符号重复定义，链接失败；
 *   3. 本文件位于 src/ 下（由 e2studio 目录收集自动编译）。
 *
 * 替代方案：链接器 --wrap（-Wl,--wrap=dcd_edpt_open 等）可免去本文件与
 * 第 2 步的排除串。见 docs/TINYUSB_MASTER_INTEGRATION.md §7.5.4。
 *
 * ---------------------------------------------------------------------------
 * ⚠️ 2026-09-21 记录：曾经在这里加过一个「屏蔽空闲 IN pipe 的 BRDY」的补丁，
 * 判据是 `remaining==0 && !zlp_pending`，**判据是错的，已删除**。两个错误：
 *   a) 读错了寄存器 —— 0x4035104A 是 BEMPSTS，不是 BRDYSTS（BRDYSTS 在 0x46，
 *      实测全程 0x0000），当时据以推出「BRDY 卡住／误报完成」的证据不成立；
 *   b) 记错了 queued 语义 —— 上游 pipe_xfer_complete() 里**有**
 *      `pipe->queued = false`，所以 Titan 的 `if (!pipe->queued) return;`
 *      在上游同样成立，不该另造判据。
 * 后果：`remaining==0 && queued==true` 恰好是「已武装、最后一包已写入 FIFO、
 * 等主机取走」的在途状态；把它当空闲、在 ISR 里屏蔽掉 BRDY 使能位，IN 传输的
 * 完成事件就永远收不到，CDC IN / MSC IN 全停 —— 表现为「设备管理器里有 COM 口，
 * 但 IDE 连不上、U 盘读不到」。
 * 要动 IN 完成路径，请先读 Titan 的 ra8_usb_process_pipe_bemp()（缺口 2），
 * 不要在这条路上猜。
 * ---------------------------------------------------------------------------
 */

#include "tusb_option.h"

#if CFG_TUD_ENABLED && defined(TUP_USBIP_RUSB2)

#include <stdint.h>
#include "device/dcd.h"

// 上游原实现改名，避免与本文件的同名定义冲突
#define dcd_edpt_open       ra8_upstream_dcd_edpt_open
#ifdef TUP_DCD_EDPT_ISO_ALLOC
#define dcd_edpt_iso_alloc  ra8_upstream_dcd_edpt_iso_alloc
#endif
/* 这两个也要改名：本文件在 fork 之上提供自己的版本，目的是补上
 * 「唤醒时把停车的 OUT 数据取走」这一步（见下面 ra8_resume_parked）。
 * 与 Titan_Mini_Openmv/platform/drivers/ra8_usb_dcd.c:17-21 同一手法。 */
#define dcd_edpt_xfer       ra8_edpt_xfer_unlocked
#define dcd_edpt_xfer_fifo  ra8_edpt_xfer_fifo_unlocked

// 前置声明：改名后的函数在上游头里没有原型，不声明会触发 -Wmissing-declarations
bool dcd_edpt_open(uint8_t rhport, tusb_desc_endpoint_t const *ep_desc);
#ifdef TUP_DCD_EDPT_ISO_ALLOC
bool dcd_edpt_iso_alloc(uint8_t rhport, uint8_t ep_addr, uint16_t largest_packet_size);
#endif

// 纳入我们 fork 的 DCD（上游原文 + OUT 路径「停车—唤醒」移植）。
// 上游原件 lib/tinyusb/.../rusb2/dcd_rusb2.c 在 .cproject 的排除串里，
// 由本文件以 #include 方式编入同一个编译单元。
#define RA8_DCD_FORK_INCLUDED
#include "ra8_usb_dcd_ra8.c"

#undef dcd_edpt_open
#ifdef TUP_DCD_EDPT_ISO_ALLOC
#undef dcd_edpt_iso_alloc
#endif
#undef dcd_edpt_xfer
#undef dcd_edpt_xfer_fifo

// RUSB2 FIFO RAM 按 64 字节分块。0..3 归 DCP，4..7 是固定的中断 pipe 6..9。
// 数据 pipe 各占一段互不重叠的区域：1/2 可能承载 1024B ISO，3..5 至多 512B。
// 取值与 Titan_Mini_Openmv/platform/port/ra8_usb_fifo.h 一致。
static uint16_t ra8_usb_fifo_value(unsigned pipe, unsigned packet_size)
{
    static const uint8_t first_block[6] = {0, 8, 40, 72, 88, 104};
    unsigned limit = pipe <= 2 ? 1024u : 512u;
    if (!pipe || pipe > 5 || !packet_size || packet_size > limit) {
        return UINT16_MAX;
    }
    unsigned blocks = (packet_size + 63u) / 64u;
    return (uint16_t)(((blocks - 1u) << 10) | first_block[pipe]);
}

// PIPEBUF 是 PIPESEL 窗口寄存器，必须先把窗口指到目标 pipe 再写。
// 不从上一次 upstream 调用遗留的 PIPESEL 反推：两次调用之间 ISR 可能改过它
// （get_pipectr() 就会写 PIPESEL）。pipe 号改从 _dcd.ep[][] 查表取得。
static void ra8_pipebuf_assign(rusb2_reg_t *rusb, unsigned pipe, unsigned mps)
{
    uint16_t v = ra8_usb_fifo_value(pipe, mps);
    if (v == UINT16_MAX) {
        return;
    }
    rusb->PIPESEL = (uint16_t)pipe;
    rusb->PIPEBUF = v;
}

// upstream 在返回前已 dcd_int_enable，改写 PIPEBUF 要自己进临界区
static unsigned ra8_pipe_of(uint8_t ep_addr)
{
    return _dcd.ep[tu_edpt_dir(ep_addr)][tu_edpt_number(ep_addr)];
}

bool dcd_edpt_open(uint8_t rhport, tusb_desc_endpoint_t const *ep_desc)
{
    bool ok = ra8_upstream_dcd_edpt_open(rhport, ep_desc);
    if (ok && rusb2_is_highspeed_rhport(rhport)) {
        const uint8_t ep_addr = ep_desc->bEndpointAddress;
        dcd_int_disable(rhport);
        ra8_pipebuf_assign(RUSB2_REG(rhport), ra8_pipe_of(ep_addr),
                           (unsigned)tu_edpt_packet_size(ep_desc));
        dcd_int_enable(rhport);
    }
    return ok;
}

// 只有 0.21 的 DCD 才提供 dcd_edpt_iso_alloc（0.19 的 RUSB2 走 CLOSE_API，没有该 API）。
// 不加守卫的话，0.19 下会留下一个未定义引用 ra8_upstream_dcd_edpt_iso_alloc，链接期才报错。
#ifdef TUP_DCD_EDPT_ISO_ALLOC
bool dcd_edpt_iso_alloc(uint8_t rhport, uint8_t ep_addr, uint16_t largest_packet_size)
{
    bool ok = ra8_upstream_dcd_edpt_iso_alloc(rhport, ep_addr, largest_packet_size);
    if (ok && rusb2_is_highspeed_rhport(rhport)) {
        dcd_int_disable(rhport);
        ra8_pipebuf_assign(RUSB2_REG(rhport), ra8_pipe_of(ep_addr),
                           (unsigned)largest_packet_size);
        dcd_int_enable(rhport);
    }
    return ok;
}
#endif

#endif

/* ============================================================================
 * 「停车—唤醒」的另一半：唤醒时把停掉的那包数据取走
 * ============================================================================
 * 移植自 Titan_Mini_Openmv/platform/drivers/ra8_usb_dcd.c:24-59。
 *
 * 背景（本工程此前只移植了「停车」这一半）：
 *   pipe_xfer_out() 发现管道未武装（pipe->queued == false）时会把管道停下：
 *     ra8_usb_rx_parked |= TU_BIT(num);
 *     BRDYENB &= ~TU_BIT(num);   PID = NAK;
 *   此时数据**仍在硬件 FIFO 里**，而且触发它的那个 BRDYSTS 已经被 ISR 清掉了。
 *
 *   随后协议层调用 tud_cdc_read() → dcd_edpt_xfer() → process_pipe_xfer() 重新
 *   武装（PID=BUF、BRDYENB 置位）。**但对 FIFO 里早已存在的数据不会再有新的
 *   BRDY 边沿** —— 主机会以为那包已经送达（它收到了 ACK），不会重发。
 *   结果：数据永远卡在设备 FIFO 里，CDC OUT 从此单向失效。
 *
 *   Titan 的原注释："The unarmed IRQ already acknowledged BRDYSTS. No new edge
 *   is guaranteed for its unread bank, so deliver it on this submission."
 *
 * 症状与实测吻合：IDE 收得到设备发出的事件（IN 通），但它发来的 SYNC 永远
 * 得不到回应（OUT 死）；设备侧毫发无伤（SOF 仍 1000/s、协议错误全 0）；
 * 是否触发取决于「主机发包」与「武装传输」的时序竞争 → 表现为偶发。
 *
 * 为什么必须整体包在临界区里：ra8_edpt_xfer_unlocked() 返回时已重新打开 USB
 * 中断，若在它与 ra8_resume_parked() 之间来了 BRDY，会与这里的直接读取打架。
 * 用 PRIMASK 而不是 taskENTER_CRITICAL：本函数也可能从 USB 中断上下文
 * （is_isr=true）被调用。 */
static void ra8_resume_parked(uint8_t port, uint8_t ep, bool is_isr)
{
    const unsigned num = _dcd.ep[tu_edpt_dir(ep)][tu_edpt_number(ep)];
    if (!tu_edpt_dir(ep) && num && (ra8_usb_rx_parked & TU_BIT(num))) {
        ra8_usb_rx_parked &= (uint16_t) ~TU_BIT(num);
        if (pipe_xfer_out(RUSB2_REG(port), num)) {
            pipe_xfer_complete(port, num, is_isr);
        }
    }
}

bool dcd_edpt_xfer(uint8_t port, uint8_t ep, uint8_t *buffer, uint16_t size, bool is_isr)
{
    const uint32_t primask = __get_PRIMASK();
    __disable_irq();
    bool result = ra8_edpt_xfer_unlocked(port, ep, buffer, size, is_isr);
    if (result) { ra8_resume_parked(port, ep, is_isr); }
    __set_PRIMASK(primask);
    return result;
}

bool dcd_edpt_xfer_fifo(uint8_t port, uint8_t ep, tu_fifo_t *fifo, uint16_t size, bool is_isr)
{
    const uint32_t primask = __get_PRIMASK();
    __disable_irq();
    bool result = ra8_edpt_xfer_fifo_unlocked(port, ep, fifo, size, is_isr);
    if (result) { ra8_resume_parked(port, ep, is_isr); }
    __set_PRIMASK(primask);
    return result;
}


