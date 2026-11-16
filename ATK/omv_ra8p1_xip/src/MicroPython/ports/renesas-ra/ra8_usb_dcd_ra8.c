/*
 * SPDX-FileCopyrightText: Copyright (c) 2020 Koji Kitayama
 * SPDX-FileCopyrightText: Portions copyrighted (c) 2021 Roland Winistoerfer
 * SPDX-FileCopyrightText: Copyright (c) 2020 Ha Thach (tinyusb.org)
 * SPDX-License-Identifier: MIT
 *
 * This file is part of the TinyUSB stack.
 */

/* ===========================================================================
 * 本文件 = 上游 lib/tinyusb/.../rusb2/dcd_rusb2.c 的 fork，含 OUT 路径
 * 「停车—唤醒」移植（详见下面 ra8_usb_rx_parked 的说明）。
 *
 * ⚠️ 它**不应被单独编译** —— 只由 ra8_usb_dcd_fix.c 以 #include 方式纳入
 * 同一个编译单元（上游原件在 .cproject 的排除串里，就是为这个留的位置）。
 * e2studio 是目录递归收集源文件，所以用这个宏挡住直接编译；
 * 直接编到本文件时不会产出任何符号，不会与 glue 里的定义冲突。
 * ========================================================================= */
#ifndef RA8_DCD_FORK_INCLUDED
#else

#include "tusb_option.h"

#if CFG_TUD_ENABLED && defined(TUP_USBIP_RUSB2)

#include "device/dcd.h"
#include "portable/renesas/rusb2/rusb2_common.h"

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM
//--------------------------------------------------------------------+
enum {
  PIPE_COUNT = 10,
};

typedef struct {
  void      *buf;      /* the start address of a transfer data buffer */
  uint16_t  length;    /* the number of bytes in the buffer */
  uint16_t  remaining; /* the number of bytes remaining in the buffer */

  uint8_t ep; /* an assigned endpoint address */
  uint8_t ff; /* `buf` is TU_FUFO or POD */
  bool queued;      /* a transfer is submitted and not yet completed (independent of `buf`, which is
                       NULL for a zero-length read) -- used to decide clear-stall re-arm */
  bool zlp_pending; /* a zero-length IN packet couldn't be queued at submit (FIFO full); retry on BRDY */
} pipe_state_t;

typedef struct
{
  pipe_state_t pipe[PIPE_COUNT];
  uint8_t ep[2][16];   /* a lookup table for a pipe index from an endpoint address */
  // Track whether sof has been manually enabled
  bool sof_enabled;
} dcd_data_t;

static dcd_data_t _dcd;

/* ---------------------------------------------------------------------------
 * 哪些 OUT 管道被「停车」了（BRDY 关闭 + PID=NAK）
 *
 * 移植自 Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:3。
 *
 * 上游的 OUT 完成路径只看 BRDY，收到短包后**不关闭 BRDY、不停管道**；而 PIPECFG
 * 开着 SHTNAK（短包后硬件自动停管道）与 DBLB，于是「已提交的 bank」和「下一次
 * BRDY」的时序完全交给硬件 —— 软件侧的目的地（tu_fifo）还没准备好，硬件就已经
 * 把下一包推进来了。
 *
 * Titan 的模型是「停车—唤醒」：
 *   · pipe_xfer_out 完成时：BRDYENB 清位 + PID=NAK（停车）
 *   · process_pipe_xfer 提交时：清 BEMPENB/BEMPSTS/BRDYSTS + BRDYENB 置位 + PID=BUF（唤醒）
 * Titan 的注释（同文件）:
 *   *"Keep a following BRDY latched until its destination is ready."*
 *
 * 为什么只有 HS 暴露：FS 下 mps=64、包小节奏慢，软件总是来得及；HS 下 mps=512、
 * 包来得快，这个时序窗口就被拉开 —— 实测表现为第一个 512 字节包正常到达
 * （used 0→512），之后再无完整包。对照实验：同一份代码切 FS 能跑完长脚本。
 * ------------------------------------------------------------------------- */
static volatile uint16_t ra8_usb_rx_parked;

/* VBUS 存在的边沿跟踪（定义在此，dcd_init 与 dcd_int_handler 都要用）。
 * 见下面 ra8_usb_vbus_change() 的说明。 */
static bool ra8_usb_vbus_present;


//--------------------------------------------------------------------+
// INTERNAL OBJECT & FUNCTION DECLARATION
//--------------------------------------------------------------------+


// Transfer conditions specifiable for each pipe for most MCUs
// - Pipe 0: Control transfer with 64-byte single buffer
// - Pipes 1 and 2: Bulk or ISO
// - Pipes 3 to 5: Bulk
// - Pipes 6 to 9: Interrupt
//
// Note: for small mcu such as
// - RA2A1: only pipe 4-7 are available, and no support for ISO
static unsigned find_pipe(unsigned xfer_type) {
  #if defined(BSP_MCU_GROUP_RA2A1)
  const uint8_t pipe_idx_arr[4][2] = {
      { 0, 0 }, // Control
      { 0, 0 }, // Isochronous not supported
      { 4, 5 }, // Bulk
      { 6, 7 }, // Interrupt
  };
  #else
  const uint8_t pipe_idx_arr[4][2] = {
      { 0, 0 }, // Control
      { 1, 2 }, // Isochronous
      { 1, 5 }, // Bulk
      { 6, 9 }, // Interrupt
  };
  #endif

  // find backward since only pipe 1, 2 support ISO
  const uint8_t idx_first = pipe_idx_arr[xfer_type][0];
  const uint8_t idx_last  = pipe_idx_arr[xfer_type][1];

  for (int i = idx_last; i >= idx_first; i--) {
    if (0 == _dcd.pipe[i].ep) {
      return (unsigned)i;
    }
  }

  return 0;
}

static volatile uint16_t* get_pipectr(rusb2_reg_t *rusb, unsigned num) {
  if (num) {
    return (volatile uint16_t*)&(rusb->PIPE_CTR[num - 1]);
  } else {
    return (volatile uint16_t*)&(rusb->DCPCTR);
  }
}

static volatile reg_pipetre_t* get_pipetre(rusb2_reg_t *rusb, unsigned num) {
  volatile reg_pipetre_t* tre = NULL;
  if ((1 <= num) && (num <= 5)) {
    tre = (volatile reg_pipetre_t*)&(rusb->PIPE_TR[num - 1].E);
  }
  return tre;
}

static volatile uint16_t* ep_addr_to_pipectr(uint8_t rhport, unsigned ep_addr) {
  rusb2_reg_t *rusb = RUSB2_REG(rhport);
  const unsigned epn = tu_edpt_number((uint8_t)ep_addr);

  if (epn) {
    const unsigned dir = tu_edpt_dir((uint8_t)ep_addr);
    const unsigned num = _dcd.ep[dir][epn];
    return get_pipectr(rusb, num);
  } else {
    return get_pipectr(rusb, 0);
  }
}

static uint16_t edpt0_max_packet_size(rusb2_reg_t* rusb) {
  return (uint16_t)rusb->DCPMAXP_b.MXPS;
}

static uint16_t edpt_max_packet_size(rusb2_reg_t *rusb, unsigned num) {
  rusb->PIPESEL = (uint16_t)num;
  return rusb->PIPEMAXP;
}

// Select the D0FIFO for `num` and wait until its buffer is ready for CPU access. Both flags
// normally settle within a few cycles (the pipe was just armed, or a BRDY freed a plane). But an
// IN pipe whose double buffer is already full stalls FRDY until the host drains it, and a
// no-handshake iso IN endpoint the host has stopped polling never drains at all — so FRDY would
// hang forever. This runs with the USB IRQ masked, so a naked spin freezes the whole stack; bound
// it and let the caller abort the FIFO access. Returns false on timeout.
#define RUSB2_FIFO_READY_SPIN 100000u
static inline bool pipe_wait_for_ready(rusb2_reg_t *rusb, unsigned num) {
  uint32_t spin = RUSB2_FIFO_READY_SPIN;
  while ( rusb->D0FIFOSEL_b.CURPIPE != num ) { if (!spin--) return false; }
  spin = RUSB2_FIFO_READY_SPIN;
  while ( !rusb->D0FIFOCTR_b.FRDY ) { if (!spin--) return false; }
  return true;
}

/* EP0（DCP）的 FIFO 就绪等待，有界。
 * DCP 是单缓冲，主机没取走上一包时 CFIFOCTR 的 FRDY 不会置起。同样地在 USB
 * 中断上下文里无界自旋会冻死整机，所以给上限；超时由调用方把 DCP 置 NAK 并
 * 放弃本次装载（保留 remaining 供诊断），而不是继续往没就绪的 FIFO 写。
 * 移植自 Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:170-176。 */
static bool ra8_usb_ep0_wait_ready(rusb2_reg_t *rusb) {
  uint32_t spin = RUSB2_FIFO_READY_SPIN;
  while (!(rusb->CFIFOCTR & RUSB2_CFIFOCTR_FRDY_Msk)) {
    if (!spin--) return false;
  }
  return true;
}

/* 「硬件上有一个新 SETUP 待处理」。
 *
 * Titan 的等价守卫写的是 `ra8_usb_setup_pending() || (INTSTS0 & VALID)`
 * （见 Titan dcd_rusb2_ra8.c:477、:512、:517、:1088）。前半段
 * `ra8_usb_setup_pending()` 属于**设备栈** —— Titan 自己 fork 的 usbd_ra8.c
 * 用 ra8_usb_setup.inc 维护 `_usbd_queued_setup`，覆盖「SETUP 已入队但设备栈
 * 还没取走」这个窗口。
 *
 * 本工程编的是**上游未打补丁的** usbd.c：它的 `_usbd_queued_setup`
 * （src/MicroPython/lib/tinyusb/src/device/usbd.c:137）是 **static**，DCD 够不到；
 * 只改本文件的前提下无法拿到那个计数。
 *
 * 这里只保留 DCD 自己能可靠判定的那一半 —— INTSTS0.VALID 表示 USB 请求有效，
 * 即硬件上确有一个 SETUP 待处理。方向与 Titan 的意图一致（「旧传输的提交不能
 * 继续」），只是覆盖窗口略窄。要补全前半段必须动设备栈，见
 * docs/USB_DISCONNECT_VS_TITAN_SOLUTION.md 的 D3-b。 */
#define RA8_EP0_NEW_SETUP_PENDING(rusb)  ((rusb)->INTSTS0 & RUSB2_INTSTS0_VALID_Msk)

//--------------------------------------------------------------------+
// Pipe Transfer
//--------------------------------------------------------------------+
static int pipe0_xfer_in(rusb2_reg_t *rusb) {
  pipe_state_t  *pipe = &_dcd.pipe[0];
  const unsigned rem  = pipe->remaining;

  if (!rem) {
    pipe->buf = NULL;
    return true;
  }

  const uint16_t mps = edpt0_max_packet_size(rusb);
  const uint16_t len = tu_min16(mps, rem);
  void          *buf = pipe->buf;

  if (len) {
    /* 先确认 DCP 缓冲真的可写。不确认就往 CFIFO 写，等于把数据塞进一个还
     * 归主机所有的缓冲 —— 上一个控制传输的残留会与新阶段的数据混在一起。
     * 失败时置 NAK 并返回 -1，由 process_pipe0_bemp 上报 FAILED；
     * process_pipe0_xfer 见到负数就不再武装 DCPCTR。
     * （Titan dcd_rusb2_ra8.c:194-199） */
    if (!ra8_usb_ep0_wait_ready(rusb)) {
      rusb->DCPCTR = RUSB2_PIPE_CTR_PID_NAK;
      pipe->buf = NULL; // failed, with remaining bytes retained for diagnostics
      return -1;
    }
    // uint16_t           fifo_sel = RUSB2_CFIFOSEL_ISEL_WRITE | FIFOSEL_BIGEND;
    tu_hwfifo_access_t access_mode;
    access_mode.param = (uintptr_t)rusb;
    //
    if (rusb2_is_highspeed_reg(rusb)) {
      //   fifo_sel |= RUSB2_FIFOSEL_MBW_32BIT;
      access_mode.data_stride = 4u;
    } else {
      //   fifo_sel |= RUSB2_FIFOSEL_MBW_16BIT;
      access_mode.data_stride = 2u;
    }
    // rusb->CFIFOSEL = fifo_sel;
    // while (0 == (rusb->CFIFOSEL & RUSB2_CFIFOSEL_ISEL_WRITE)) {}

    if (pipe->ff) {
      tu_hwfifo_write_from_fifo(&rusb->CFIFO, (tu_fifo_t *)buf, len, &access_mode);
    } else {
      tu_hwfifo_write(&rusb->CFIFO, buf, len, &access_mode);
      pipe->buf = (uint8_t *)buf + len;
    }
  }

  if (len < mps) {
    rusb->CFIFOCTR = RUSB2_CFIFOCTR_BVAL_Msk;
  }

  pipe->remaining = rem - len;
  return false;
}

static bool pipe0_xfer_out(rusb2_reg_t *rusb) {
  pipe_state_t  *pipe = &_dcd.pipe[0];
  const unsigned rem  = pipe->remaining;

  // BRDY with no armed transfer: a back-to-back data-stage packet beat the PID=NAK below (the
  // host has already ACKed it). Park it in the DCP buffer — an unread buffer NAKs further OUTs —
  // and let process_pipe0_xfer deliver it when usbd arms the next chunk. BCLR here would silently
  // drop the packet and shift every later chunk by one (usbtest ctrl_out corruption at ra4m1).
  if (pipe->buf == NULL && rem == 0) {
    rusb->DCPCTR = RUSB2_PIPE_CTR_PID_NAK;
    return false;
  }

  const uint16_t mps = edpt0_max_packet_size(rusb);
  const uint16_t vld = rusb->CFIFOCTR_b.DTLN;
  const uint16_t len = tu_min16(tu_min16(rem, mps), vld);
  void          *buf = pipe->buf;

  if (len) {
    tu_hwfifo_access_t access_mode = {.data_stride = (rusb2_is_highspeed_reg(rusb) ? 4u : 2u),
                                      .param       = (uintptr_t)rusb};

    if (pipe->ff) {
      tu_hwfifo_read_to_fifo(&rusb->CFIFO, (tu_fifo_t *)buf, len, &access_mode);
    } else {
      tu_hwfifo_read(&rusb->CFIFO, buf, len, &access_mode);
      pipe->buf = (uint8_t *)buf + len;
    }
  }

  if (len < mps) {
    rusb->CFIFOCTR = RUSB2_CFIFOCTR_BCLR_Msk;
  }

  pipe->remaining = rem - len;
  if ((len < mps) || (rem == len)) {
    pipe->buf = NULL;
    // Flow-control the single-buffer control pipe: NAK further OUT until usbd arms the next
    // data-stage chunk. usbd receives a multi-packet control-OUT one CFG_TUD_ENDPOINT0_SIZE
    // packet per submit; without this the DCP auto-accepts the next back-to-back packet into the
    // just-emptied buffer and the following BRDY (remaining==0) BCLR-discards it, dropping 64
    // bytes mid-transfer (e.g. usbtest ctrl_out 512B). RA4M1 UM R01UH0887 DCPCTR.PID.
    rusb->DCPCTR = RUSB2_PIPE_CTR_PID_NAK;
    return true;
  }

  return false;
}

/* IN 传输的最后一包已经写进 FIFO —— 但 pipe 1..5 是 DBLB（双缓冲），USB 侧
 * 可能还押着 1~2 个 bank 没被主机取走。此时关掉本条 BRDY、打开 BEMP，把
 * 「真正发完」的判定权交给 BEMP 处理器（ra8_usb_process_pipe_bemp）。
 * 注意**不要**在这里清 BEMPSTS：主机可能在 BVAL/最后一次 FIFO 写之后、但在
 * 使能 BEMP 中断之前就已经取走了最后一包，那一拍被清掉就永远收不到完成。
 * 移植自 Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:279-286。 */
static void ra8_usb_in_wait_drain(rusb2_reg_t *rusb, unsigned num) {
  if (num <= 5) {
    rusb->BRDYENB &= (uint16_t)~TU_BIT(num);
    rusb->BEMPENB |= TU_BIT(num);
  }
}

static bool pipe_xfer_in(rusb2_reg_t* rusb, unsigned num)
{
  pipe_state_t  *pipe = &_dcd.pipe[num];
  const unsigned rem  = pipe->remaining;

  if (!rem) {
    /* 软件侧计数归零 **不等于** 主机已经收完。pipe 1..5 可能还押着 USB 侧的
     * bank，只有 BEMP 处理器能把它退役 —— 所以这里【不】上报完成。
     * Titan 原注释（dcd_rusb2_ra8.c:293-296）：
     *   "Pipes 1..5 may still own one or two committed USB-side banks.
     *    Only their BEMP handler can retire the transfer."
     * 这正是上游真正缺的那一环：上游在 remaining==0 就 return true 报完成，
     * 而 bulk 管道无条件开着 DBLB（见下面 dcd_edpt_open 的 PIPECFG），于是
     * MSC 的 CSW 之类可能在数据真正发完之前就被上报 → 主机看到的 BOT 阶段
     * 顺序错乱 → BOT reset（= 总线复位）→ 把 CDC 的 IDE 会话一起打掉。 */
    if (num <= 5) { return false; }
    pipe->buf = NULL;
    return true;
  }

  const uint16_t fifo_sel     = num | FIFOSEL_BIGEND;
  const bool     is_highspeed = rusb2_is_highspeed_reg(rusb);
  if (is_highspeed) {
    rusb->D0FIFOSEL = fifo_sel | RUSB2_FIFOSEL_MBW_32BIT;
  } else {
    rusb->D0FIFOSEL = fifo_sel | RUSB2_FIFOSEL_MBW_16BIT;
  }

  const uint16_t mps = edpt_max_packet_size(rusb, num);
  if (!pipe_wait_for_ready(rusb, num)) {
    // Buffer never came ready (double-buffered IN pipe full, host not draining). Drop this load;
    // the transfer stays pending and is retried when a BRDY frees a plane or the pipe is re-armed.
    rusb->D0FIFOSEL = 0;
    return false;
  }
  uint16_t len = tu_min16(rem, mps);
  void    *buf = pipe->buf;

  /* 目的地（tu_fifo）必须真的装得下这一包，否则根本不读 —— 数据留在硬件缓冲里，
   * 等 tu_fifo 有空间的下一次 BRDY 再取。不检查就写，tu_fifo 满了会静默丢字节：
   * CDC OUT 上表现为协议帧残缺 / 脚本字节丢失，而且掩盖其它排查结论。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:319-325） */
  if (pipe->ff) {
    const uint16_t available = tu_fifo_count((tu_fifo_t *)buf);
    if (available < len) {
      rusb->D0FIFOSEL = 0;
      return false;
    }
  }

  if (len) {
    tu_hwfifo_access_t access_mode = {.data_stride = (rusb2_is_highspeed_reg(rusb) ? 4u : 2u),
                                      .param       = (uintptr_t)rusb};
    if (pipe->ff) {
      tu_hwfifo_write_from_fifo(&rusb->D0FIFO, (tu_fifo_t *)buf, len, &access_mode);
    } else {
      tu_hwfifo_write(&rusb->D0FIFO, buf, len, &access_mode);
      pipe->buf = (uint8_t *)buf + len;
    }
  }

  if (len < mps) {
    rusb->D0FIFOCTR = RUSB2_CFIFOCTR_BVAL_Msk;
  }

  rusb->D0FIFOSEL = 0;
  while (rusb->D0FIFOSEL_b.CURPIPE) {} /* if CURPIPE bits changes, check written value */

  pipe->remaining = rem - len;
  /* 最后一包已写入 → 转入「等 BEMP 确认真正发完」状态。 */
  if (!pipe->remaining) { ra8_usb_in_wait_drain(rusb, num); }

  return false;
}

static bool pipe_xfer_out(rusb2_reg_t* rusb, unsigned num)
{
  pipe_state_t  *pipe = &_dcd.pipe[num];
  const uint16_t rem  = pipe->remaining;
  // queued 用来区分「合法的零长度 OUT 传输」与「未武装的管道」。
  // 未武装时停车（关 BRDY + PID=NAK）并记住，别把硬件推进来的数据当有效负载。
  if (!pipe->queued) {
    ra8_usb_rx_parked |= TU_BIT(num);
    rusb->BRDYENB &= (uint16_t)~TU_BIT(num);
    *get_pipectr(rusb, num) = RUSB2_PIPE_CTR_PID_NAK;
    return false;
  }

  uint16_t fifo_sel = num | FIFOSEL_BIGEND;
  if (rusb2_is_highspeed_reg(rusb)) {
    fifo_sel |= RUSB2_FIFOSEL_MBW_32BIT;
  } else {
    fifo_sel |= RUSB2_FIFOSEL_MBW_16BIT;
  }
  rusb->D0FIFOSEL = fifo_sel;

  const uint16_t mps = edpt_max_packet_size(rusb, num);
  if (!pipe_wait_for_ready(rusb, num)) {
    rusb->D0FIFOSEL = 0;
    return false; // FIFO not ready; leave the receive pending (BRDY re-enters when data arrives)
  }

  const uint16_t vld  = (uint16_t)rusb->D0FIFOCTR_b.DTLN;
  const uint16_t len  = tu_min16(tu_min16(rem, mps), vld);
  void          *buf  = pipe->buf;

  /* 本包结束（短包或已收满）→ 立刻停车：关掉本条 BRDY、把管道置 NAK。
   * 让后续 BRDY 一直挂在状态位里，直到 process_pipe_xfer 提交新传输时
   * 重新武装（清状态位 + BRDYENB 置位 + PID=BUF）—— 这样目的地一定已就绪。
   * Titan 原注释："Keep a following BRDY latched until its destination is ready." */
  if ((len < mps) || (rem == len)) {
    rusb->BRDYENB &= (uint16_t)~TU_BIT(num);
    *get_pipectr(rusb, num) = RUSB2_PIPE_CTR_PID_NAK;
  }

  if (len) {
    tu_hwfifo_access_t access_mode = {.data_stride = (rusb2_is_highspeed_reg(rusb) ? 4u : 2u),
                                      .param       = (uintptr_t)rusb};
    if (pipe->ff) {
      tu_hwfifo_read_to_fifo(&rusb->D0FIFO, (tu_fifo_t *)buf, len, &access_mode);
    } else {
      tu_hwfifo_read(&rusb->D0FIFO, buf, len, &access_mode);
      pipe->buf = (uint8_t *)buf + len;
    }
  }

  if (len < mps) {
    rusb->D0FIFOCTR = RUSB2_CFIFOCTR_BCLR_Msk;
  }

  rusb->D0FIFOSEL = 0;
  while (rusb->D0FIFOSEL_b.CURPIPE) {} /* if CURPIPE bits changes, check written value */

  pipe->remaining = rem - len;
  if ((len < mps) || (rem == len)) {
    pipe->buf = NULL;
    return true;
  }

  return false;
}

static void process_setup_packet(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  if (0 == (rusb->INTSTS0 & RUSB2_INTSTS0_VALID_Msk)) return;

  /* 新 SETUP 到达 = 上一轮控制传输作废。把 EP0 的软件账（pipe[0] 的
   * buf/remaining/queued）连同它的两个状态位一起清掉，否则新一轮的
   * process_pipe0_xfer/process_pipe0_bemp 会拿旧账继续跑。
   * BEMPSTS/BRDYSTS 是「写 0 清除、写 1 保留」的写清寄存器，所以
   * `~(... & 1U)` 只对 bit0 写 0、对其余位写 1 —— 保住其它 pipe 的中断。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:417-420） */
  tu_varclr(&_dcd.pipe[0]);
  rusb->BEMPSTS = (uint16_t)~(rusb->BEMPSTS & 1U);
  rusb->BRDYSTS = (uint16_t)~(rusb->BRDYSTS & 1U);
  rusb->CFIFOCTR = RUSB2_CFIFOCTR_BCLR_Msk;
  uint16_t setup_packet[4] = {
      tu_htole16(rusb->USBREQ),
      tu_htole16(rusb->USBVAL),
      tu_htole16(rusb->USBINDX),
      tu_htole16(rusb->USBLENG)
  };

  rusb->INTSTS0 = ~((uint16_t) RUSB2_INTSTS0_VALID_Msk);
  dcd_event_setup_received(rhport, (const uint8_t*)&setup_packet[0], true);
}

static void process_status_completion(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  uint8_t ep_addr;
  /* Check the data stage direction */
  if (rusb->CFIFOSEL & RUSB2_CFIFOSEL_ISEL_WRITE) {
    /* IN transfer. */
    ep_addr = tu_edpt_addr(0, TUSB_DIR_IN);
  } else {
    /* OUT transfer. */
    ep_addr = tu_edpt_addr(0, TUSB_DIR_OUT);
  }

  dcd_event_xfer_complete(rhport, ep_addr, 0, XFER_RESULT_SUCCESS, true);
}

// Report a completed transfer on `num` and reset its bookkeeping. Single completion path for the
// BRDY handler and the EP0 parked-packet drain, so they can't diverge (e.g. on clearing `queued`).
static void pipe_xfer_complete(uint8_t rhport, unsigned num, bool in_isr) {
  pipe_state_t *pipe = &_dcd.pipe[num];
  pipe->queued = false;
  dcd_event_xfer_complete(rhport, pipe->ep, pipe->length - pipe->remaining,
                          XFER_RESULT_SUCCESS, in_isr);
}

/* IN 方向 pipe 1..5 的完成判定。
 *
 * 上游只走 BRDY，而 BRDY 表示「软件侧认为发完了」，不是「主机收完了」：bulk
 * 管道无条件开着 DBLB，USB 侧可能还押着 1~2 个 bank。只有 BEMP 能确认它们
 * 真的被主机取走 —— 这是上游缺的那一半，也是「MSC 的 CSW 提前上报 → 主机把
 * BOT 阶段判错 → BOT reset（总线复位）→ 顺带打掉 CDC 会话」的机制。
 *
 * 调用时机：dcd_int_handler 在 BEMP 中断里对 s & 0x3e（pipe 1..5）逐个调用。
 * 移植自 Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:458-473。 */
static void ra8_usb_process_pipe_bemp(uint8_t rhport, unsigned num) {
  rusb2_reg_t *rusb = RUSB2_REG(rhport);
  pipe_state_t *pipe = &_dcd.pipe[num];
  /* 只处理「已武装的 IN 管道」。别的（未武装 / OUT 管道）直接关掉本条 BEMP
   * 使能，免得它反复进来。 */
  if (!pipe->queued || !tu_edpt_dir(pipe->ep)) {
    rusb->BEMPENB &= (uint16_t)~TU_BIT(num);
    return;
  }
  /* 确认 USB 侧真的排空了才报完成：
   *   remaining / zlp_pending —— 软件侧还有东西要发；
   *   INBUFM                  —— 硬件缓冲里还押着数据；
   *   PID >= STALL            —— 管道不在正常收发态。
   * 任一成立就再等下一个 BEMP，绝不提前上报。 */
  uint16_t ctr = *get_pipectr(rusb, num);
  if (pipe->remaining || pipe->zlp_pending ||
      (ctr & RUSB2_PIPE_CTR_INBUFM_Msk) || ((ctr & 3U) >= RUSB2_PIPE_CTR_PID_STALL)) {
    return;
  }
  rusb->BEMPENB &= (uint16_t)~TU_BIT(num);
  pipe->buf = NULL;
  pipe_xfer_complete(rhport, num, true);
}

static bool process_pipe0_xfer(uint8_t rhport, rusb2_reg_t *rusb, int buffer_type, uint8_t ep_addr, void *buffer,
                               uint16_t total_bytes) {
  /* 硬件上已经有一个新的 SETUP 待处理 → 本次提交属于**上一轮**控制传输，
   * 不能带着它的缓冲/长度去动已经被新 SETUP 重置过的 DCP。
   * 见 RA8_EP0_NEW_SETUP_PENDING 的说明（Titan 同位置还查设备栈的
   * ra8_usb_setup_pending()，本工程够不到）。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:477） */
  if (RA8_EP0_NEW_SETUP_PENDING(rusb)) return false;
  uint16_t fifo_sel =
    (rusb2_is_highspeed_reg(rusb) ? RUSB2_FIFOSEL_MBW_32BIT : RUSB2_FIFOSEL_MBW_16BIT) | FIFOSEL_BIGEND;

  /* configure fifo direction and access unit settings */
  if (ep_addr != 0) {
    // Control IN
    fifo_sel |= RUSB2_CFIFOSEL_ISEL_WRITE;
  }
  rusb->CFIFOSEL = fifo_sel;
  while ((rusb->CFIFOSEL & RUSB2_CFIFOSEL_ISEL_WRITE) != (fifo_sel & RUSB2_CFIFOSEL_ISEL_WRITE)) {
    // wait until ISEL_WRITE take effect
  }

  pipe_state_t *pipe = &_dcd.pipe[0];
  pipe->ff           = buffer_type;
  pipe->length       = total_bytes;
  pipe->remaining    = total_bytes;

  if (total_bytes) {
    pipe->buf = buffer;
    if (ep_addr) {
      /* IN */
      TU_ASSERT(rusb->DCPCTR_b.BSTS && (rusb->USBREQ & 0x80));
      /* pipe0_xfer_in 返回 -1 = DCP 缓冲没就绪（已在里面置 NAK 并保留
       * remaining）。此时【不要】再武装 DCPCTR，否则等于拿一个失败的状态
       * 去骗设备栈。 */
      if (pipe0_xfer_in(rusb) < 0) return false;
    } else if (rusb->CFIFOCTR_b.DTLN > 0) {
      /* OUT: a back-to-back packet parked by pipe0_xfer_out already sits in the DCP buffer (its
         BRDY has fired and been cleared) — deliver it into this chunk now; no new BRDY will come
         for it. Runs with the USB IRQ masked (dcd_edpt_xfer). Detected via the hardware DTLN
         rather than a driver flag: the BCLR at SETUP/bus-reset then self-heals any parked state. */
      if (pipe0_xfer_out(rusb)) {
        pipe_xfer_complete(rhport, 0, false);
        return true; // PID stays NAK (set by pipe0_xfer_out) until the next chunk is armed
      }
    }
    if (RA8_EP0_NEW_SETUP_PENDING(rusb)) return false;
    rusb->DCPCTR = RUSB2_PIPE_CTR_PID_BUF;
  } else {
    /* ZLP */
    pipe->buf = NULL;
    if (RA8_EP0_NEW_SETUP_PENDING(rusb)) return false;
    rusb->DCPCTR = RUSB2_DCPCTR_CCPL_Msk | RUSB2_PIPE_CTR_PID_BUF;
  }

  return true;
}

// Queue a zero-length IN packet. Returns false if the FIFO buffer wasn't free (double-buffered pipe
// full, host not draining) so BVAL couldn't be written -- the caller retries on the next BRDY.
static bool pipe_zlp_in(rusb2_reg_t *rusb, unsigned num) {
  rusb->D0FIFOSEL = (uint16_t) num;
  const bool ready = pipe_wait_for_ready(rusb, num);
  if (ready) {
    rusb->D0FIFOCTR = RUSB2_CFIFOCTR_BVAL_Msk;
    ra8_usb_in_wait_drain(rusb, num);
  }
  rusb->D0FIFOSEL = 0;
  // deselect completes within a few bus cycles (not host-dependent), but bound it anyway: this
  // runs with the USB IRQ masked, where any stuck spin freezes the whole stack
  uint32_t spin = RUSB2_FIFO_READY_SPIN;
  while (rusb->D0FIFOSEL_b.CURPIPE) {
    if (!spin--) { break; }
  }
  return ready;
}

static bool process_pipe_xfer(rusb2_reg_t* rusb, int buffer_type, uint8_t ep_addr, void* buffer, uint16_t total_bytes)
{
  const unsigned epn = tu_edpt_number(ep_addr);
  const unsigned dir = tu_edpt_dir(ep_addr);
  const unsigned num = _dcd.ep[dir][epn];

  TU_ASSERT(num);

  pipe_state_t *pipe  = &_dcd.pipe[num];
  pipe->ff          = buffer_type;
  pipe->buf         = buffer;
  pipe->length      = total_bytes;
  pipe->remaining   = total_bytes;
  pipe->queued      = true;
  pipe->zlp_pending = false;

  /* IN 且是 DBLB 管道（1..5）：提交新传输 = 「唤醒」。清掉上一次的 BEMP 账、
   * 清两个状态位、重新打开 BRDY —— 与 pipe_xfer_in 收尾时的
   * ra8_usb_in_wait_drain（停车）成对，构成完整的「停车—唤醒」循环。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:559-564） */
  if (dir && num <= 5) {
    rusb->BEMPENB &= (uint16_t)~TU_BIT(num);
    rusb->BEMPSTS = (uint16_t)~(rusb->BEMPSTS & TU_BIT(num));
    rusb->BRDYSTS = (uint16_t)~(rusb->BRDYSTS & TU_BIT(num));
    rusb->BRDYENB |= TU_BIT(num);
  }

  if (dir) {
    /* IN */
    if (total_bytes) {
      pipe_xfer_in(rusb, num);
    } else {
      /* ZLP: if the FIFO buffer isn't free yet, defer the queue to the next BRDY (see process_pipe_brdy) */
      pipe->zlp_pending = !pipe_zlp_in(rusb, num);
    }
  } else {
    // OUT
    volatile reg_pipetre_t *pt = get_pipetre(rusb, num);

    volatile uint16_t *ctr = get_pipectr(rusb, num);
    if (*ctr & 0x3) *ctr = RUSB2_PIPE_CTR_PID_NAK;
    if (pt) {
      const uint16_t mps = edpt_max_packet_size(rusb, num);
      pt->TRE   = TU_BIT(8);
      pt->TRN   = (total_bytes + mps - 1) / mps;
      pt->TRENB = 1;
    }
    /* 唤醒：把管道从 NAK 恢复成 BUF。**必须在 if (pt) 之外** —— PIPETRE
     * （事务计数器）是没有也能收数据的可选特性，但唤醒一旦漏掉，这条 OUT
     * 管道就永久 NAK（pipe_xfer_out 在收包结束时停的车再没人来开）。
     * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:578-587） */
    *ctr = RUSB2_PIPE_CTR_PID_BUF;
    /* 唤醒：pipe_xfer_out 在上一包结束时把 BRDY 关了（停车），这里重新打开，
     * 与上面 PID=BUF 一起构成完整的「停车—唤醒」循环。 */
    rusb->BRDYENB |= TU_BIT(num);
  }

  //  TU_LOG2("X %x %d %d\r\n", ep_addr, total_bytes, buffer_type);
  return true;
}

static bool process_edpt_xfer(uint8_t rhport, rusb2_reg_t* rusb, int buffer_type, uint8_t ep_addr, void* buffer, uint16_t total_bytes)
{
  const unsigned epn = tu_edpt_number(ep_addr);
  if (0 == epn) {
    return process_pipe0_xfer(rhport, rusb, buffer_type, ep_addr, buffer, total_bytes);
  } else {
    return process_pipe_xfer(rusb, buffer_type, ep_addr, buffer, total_bytes);
  }
}

static void process_pipe0_bemp(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  pipe_state_t *pipe = &_dcd.pipe[0];
  /* 上一次 pipe0_xfer_in 因为 DCP 缓冲没就绪而失败（已在里面置 NAK、清 buf、
   * 保留 remaining）。这个迟到的 BEMP 是那次失败的余波，不能拿它编一个
   * 「成功完成」出来。 */
  if (!pipe->buf && pipe->remaining) return;
  const int completed = pipe0_xfer_in(rusb);
  if (completed) {
    dcd_event_xfer_complete(rhport, tu_edpt_addr(0, TUSB_DIR_IN),
                            completed > 0 ? pipe->length : pipe->length - pipe->remaining,
                            completed > 0 ? XFER_RESULT_SUCCESS : XFER_RESULT_FAILED, true);
  }
}

static void process_pipe_brdy(uint8_t rhport, unsigned num)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  pipe_state_t  *pipe = &_dcd.pipe[num];
  const unsigned dir  = tu_edpt_dir(pipe->ep);
  bool completed;

  if (dir) {
    /* IN */
    /* 未武装的 pipe 上出现 BRDY 是噪声（复位前残留、或已被停车）。
     * 直接丢弃，别让它去动 pipe->buf / remaining。 */
    if (!pipe->queued) { return; }
    if (pipe->zlp_pending) {
      // The submit-time ZLP couldn't be queued (FIFO full); a freed buffer plane lets us queue it
      // now. Don't report completion until the ZLP is actually queued (and then sent, next BRDY),
      // otherwise a spurious BRDY would complete a zero-length IN the host never received.
      pipe->zlp_pending = !pipe_zlp_in(rusb, num);
      completed = false;
    } else {
      completed = pipe_xfer_in(rusb, num);
    }
  } else {
    // OUT
    if (num) {
      completed = pipe_xfer_out(rusb, num);
    } else {
      completed = pipe0_xfer_out(rusb);
    }
  }
  if (completed) {
    pipe_xfer_complete(rhport, num, true);
    //  TU_LOG1("C %d %d\r\n", num, pipe->length - pipe->remaining);
  }
}

static void process_bus_reset(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);

  /* 复位后所有管道都会被重新配置，停车标记必须一起清掉，
   * 否则被标记的 OUT 管道会因为「以为还在停车」而不再唤起 BRDY。
   * Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:685 同。 */
  ra8_usb_rx_parked = 0;

  rusb->BEMPENB = 1;
  rusb->BRDYENB = 1;
  rusb->CFIFOCTR = RUSB2_CFIFOCTR_BCLR_Msk;

  rusb->D0FIFOSEL = 0;
  while (rusb->D0FIFOSEL_b.CURPIPE) {} /* if CURPIPE bits changes, check written value */

  rusb->D1FIFOSEL = 0;
  while (rusb->D1FIFOSEL_b.CURPIPE) {} /* if CURPIPE bits changes, check written value */

  volatile uint16_t *ctr = (volatile uint16_t*)((uintptr_t) (&rusb->PIPE_CTR[0]));
  volatile uint16_t *tre = (volatile uint16_t*)((uintptr_t) (&rusb->PIPE_TR[0].E));

  for (uint16_t i = 1; i <= 5; ++i) {
    rusb->PIPESEL = i;
    rusb->PIPECFG = 0;
    *ctr = RUSB2_PIPE_CTR_ACLRM_Msk;
    *ctr = 0;
    ++ctr;
    *tre = TU_BIT(8);
    tre += 2;
  }

  for (uint16_t i = 6; i <= 9; ++i) {
    rusb->PIPESEL = i;
    rusb->PIPECFG = 0;
    *ctr = RUSB2_PIPE_CTR_ACLRM_Msk;
    *ctr = 0;
    ++ctr;
  }
  tu_varclr(&_dcd);

  /* 清掉复位前残留的 BRDY/BEMP 状态位。
   * 这些是「写 0 清除、写 1 保留」的写清寄存器（所以 = ~STS 只对已置位的位写 0，
   * 与 ISR 里 `BRDYSTS = ~s` 同一手法 —— 手册要求不要对已清的位写 0，否则会
   * 误清刚被硬件置起来的位）。
   * 上面循环已把 pipe1..9 的 PIPECFG 清 0、tu_varclr(&_dcd) 也把软件侧账清了，
   * 但**状态位还挂在寄存器里**：紧接着的重新枚举会让 ISR 立刻处理一个属于
   * 【复位前那些管道】的陈旧事件，作用在刚重配好的 pipe 上 → 端点状态错乱 →
   * 主机重连失败（症状：断连后必须断电才能恢复）。
   * Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:686-687 同。 */
  rusb->BEMPSTS = (uint16_t)~rusb->BEMPSTS;
  rusb->BRDYSTS = (uint16_t)~rusb->BRDYSTS;

  TU_LOG3("Bus reset, RHST = %u\r\n", rusb->DVSTCTR0_b.RHST);
  tusb_speed_t speed;
  switch(rusb->DVSTCTR0 & RUSB2_DVSTCTR0_RHST_Msk) {
    case RUSB2_DVSTCTR0_RHST_LS:
      speed = TUSB_SPEED_LOW;
      break;

    case RUSB2_DVSTCTR0_RHST_FS:
      speed = TUSB_SPEED_FULL;
      break;

    case RUSB2_DVSTCTR0_RHST_HS:
      speed = TUSB_SPEED_HIGH;
      break;

    default:
      TU_ASSERT(false, );
  }

  dcd_event_bus_reset(rhport, speed, true);
}

static bool process_set_address(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  const uint16_t addr = (uint16_t)rusb->USBADDR_b.USBADDR;
  if (!addr) {
    return false;
  }
  /* SET_ADDRESS 同样是一次控制传输的边界：EP0 的软件账与两个状态位都要清，
   * 否则下面这个**合成**出来的 SETUP 事件会撞上真实的旧账。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:718-720） */
  tu_varclr(&_dcd.pipe[0]);
  rusb->BEMPSTS = (uint16_t)~(rusb->BEMPSTS & 1U);
  rusb->BRDYSTS = (uint16_t)~(rusb->BRDYSTS & 1U);

  const tusb_control_request_t setup_packet = {
#if defined(__CCRX__)
      .bmRequestType = { 0 },  /* Note: CCRX needs the braces over this struct member */
  #else
    .bmRequestType = 0,
  #endif
    .bRequest = TUSB_REQ_SET_ADDRESS,
    .wValue   = addr,
    .wIndex   = 0,
    .wLength  = 0,
  };

  dcd_event_setup_received(rhport, (const uint8_t *) &setup_packet, true);
  return true;
}

/*------------------------------------------------------------------*/
/* Device API
 *------------------------------------------------------------------*/

#if 0 // previously present in the rx driver before generalization
static uint32_t disable_interrupt(void)
{
  uint32_t pswi;
#if defined(__CCRX__)
  pswi = get_psw() & 0x010000;
  clrpsw_i();
#else
  pswi = __builtin_rx_mvfc(0) & 0x010000;
  __builtin_rx_clrpsw('I');
#endif
  return pswi;
}

static void enable_interrupt(uint32_t pswi)
{
#if defined(__CCRX__)
  set_psw(get_psw() | pswi);
#else
  __builtin_rx_mvtc(0, __builtin_rx_mvfc(0) | pswi);
#endif
}
#endif

bool dcd_init(uint8_t rhport, const tusb_rhport_init_t* rh_init) {
  (void) rh_init;
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  rusb2_module_start(rhport, true);

  // We disable SOF for now until needed later on.
  // Since TinyUSB doesn't use SOF for now, and this interrupt often (1ms interval)
  _dcd.sof_enabled = false;

#ifdef RUSB2_SUPPORT_HIGHSPEED
  if ( rusb2_is_highspeed_rhport(rhport) ) {
    rusb->SYSCFG_b.HSE = TUD_OPT_HIGH_SPEED ? 1 : 0; // FS-only build: no HS chirp
    rusb2_utmi_phy_powerup(rusb);

    rusb->SYSCFG_b.DRPD = 0;
    rusb->SYSCFG_b.USBE = 1;

    // Set CPU bus wait time (fine tunne later)
    // rusb2->BUSWAIT |= 0x0F00U;

    rusb->PHYSET_b.REPSEL = 1;
  } else
#endif
  {
    rusb->SYSCFG_b.SCKE = 1;
    while (!rusb->SYSCFG_b.SCKE) {}
    rusb->SYSCFG_b.DRPD = 0;
    rusb->SYSCFG_b.DCFM = 0;
    rusb->SYSCFG_b.USBE = 1;

    // MCU specific PHY init
    rusb2_phy_init();

    rusb->PHYSLEW = 0x5;
    rusb->DPUSR0R_FS_b.FIXPHY0 = 0u; /* USB_BASE Transceiver Output fixed */
  }

  /* Setup default control pipe */
  rusb->DCPMAXP_b.MXPS = 64;

  rusb->INTSTS0 = 0;
  rusb->INTENB0 = RUSB2_INTSTS0_VBINT_Msk | RUSB2_INTSTS0_BRDY_Msk | RUSB2_INTSTS0_BEMP_Msk |
                  RUSB2_INTSTS0_DVST_Msk | RUSB2_INTSTS0_CTRT_Msk | (_dcd.sof_enabled ? RUSB2_INTSTS0_SOFR_Msk : 0) |
                  RUSB2_INTSTS0_RESM_Msk;
  rusb->BEMPENB = 1;
  rusb->BRDYENB = 1;

  // If VBUS (detect) pin is not used, application need to call tud_connect() manually after tud_init()
  /* 记住上电瞬间的 VBUS 状态 —— 后续 VBINT 处理要靠它做边沿检测。 */
  ra8_usb_vbus_present = rusb->INTSTS0_b.VBSTS != 0;
  if (ra8_usb_vbus_present) {
    dcd_connect(rhport);
  }

  return true;
}

void dcd_int_enable(uint8_t rhport) {
  rusb2_int_enable(rhport);
}

void dcd_int_disable(uint8_t rhport) {
  rusb2_int_disable(rhport);
}

void dcd_set_address(uint8_t rhport, uint8_t dev_addr) {
  (void) rhport;
  (void) dev_addr;
}

void dcd_remote_wakeup(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  rusb->DVSTCTR0_b.WKUP = 1;
}

/* VBUS 变化处理，带**边沿**检测。
 * Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:148-162。
 *
 * 为什么需要它：本板原来的 VBINT 处理只看 VBSTS 电平就直接 connect/disconnect，
 * 既没有边沿检测，也**从不发出 DCD_EVENT_UNPLUGGED** —— 于是
 * omv_protocol_channel_tinyusb.c:163-164 里那个
 * `eventid == DCD_EVENT_UNPLUGGED` 分支在全工程都是**死代码**：
 * 拔线 / VBUS 丢失时 usb_channel_active 不会清零、ra8_msc_reset() 不会被调用、
 * HS 的单端接收器也一直开着。 */
static void ra8_usb_vbus_change(uint8_t rhport, bool present) {
  const bool changed = present != ra8_usb_vbus_present;
  ra8_usb_vbus_present = present;
  if (present) {
    dcd_connect(rhport);
  } else {
    dcd_disconnect(rhport);
    /* 自供电的板子在拔线后仍然活着，必须主动通知协议栈作废 CDC 会话与在途
     * MSC 作业。changed 门限不可省 —— Titan 原注释：
     *   "Repeated low-VBUS interrupts must not cancel a later session twice." */
    if (changed) dcd_event_bus_signal(rhport, DCD_EVENT_UNPLUGGED, true);
  }
}

void dcd_connect(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);

  if ( rusb2_is_highspeed_rhport(rhport)) {
    rusb->SYSCFG_b.CNEN = 1;
  }
  rusb->SYSCFG_b.DPRPU = 1;
}

void dcd_disconnect(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  rusb->SYSCFG_b.DPRPU = 0;
  /* RA8P1 UM 38.2.1：单端接收器（CNEN）只能在 VBUS **真的移除之后** 关。
   * 软件主动 detach（VBUS 仍在）时保留 CNEN，否则高速 PHY 会被留在一个
   * 半死状态里，下一次连接起不来。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:851-859） */
  if (rusb2_is_highspeed_rhport(rhport) && !rusb->INTSTS0_b.VBSTS) {
    rusb->SYSCFG_b.CNEN = 0;
  }
}

void dcd_sof_enable(uint8_t rhport, bool en)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);
  _dcd.sof_enabled = en;
  rusb->INTENB0_b.SOFE = en ? 1: 0;
}

//--------------------------------------------------------------------+
// Endpoint API
//--------------------------------------------------------------------+
bool dcd_edpt_open(uint8_t rhport, tusb_desc_endpoint_t const * ep_desc)
{
  (void)rhport;

  rusb2_reg_t * rusb = RUSB2_REG(rhport);
  const uint8_t  ep_addr = ep_desc->bEndpointAddress;
  const unsigned epn     = tu_edpt_number(ep_addr);
  const unsigned dir     = tu_edpt_dir(ep_addr);
  const unsigned xfer    = ep_desc->bmAttributes.xfer;

  const unsigned mps = tu_edpt_packet_size(ep_desc);

  if (xfer == TUSB_XFER_ISOCHRONOUS) {
    // Fullspeed ISO is limit to 256 bytes
    if ( !rusb2_is_highspeed_rhport(rhport) && mps > 256) {
      return false;
    }
  } else if (xfer == TUSB_XFER_INTERRUPT) {
    // Interrupt pipes (6-9) have a fixed 64-byte buffer even in high speed (RA6M5 UM 29.1);
    // a larger PIPEMAXP would enumerate, then silently truncate every transfer
    TU_ASSERT(mps <= 64);
  }

  // Re-opening an endpoint must reuse its pipe: usbd_edpt_close() is a no-op on ISO_ALLOC ports,
  // so a class's close/open across SET_INTERFACE (e.g. video's notification endpoint) would
  // otherwise allocate a second pipe with the same EPNUM and leak pipes until exhaustion.
  unsigned num = _dcd.ep[dir][epn];
  if (num == 0) {
    num = find_pipe(xfer);
    TU_ASSERT(num);
  }

  _dcd.pipe[num].ep = ep_addr;
  _dcd.ep[dir][epn] = num;

  /* setup pipe */
  dcd_int_disable(rhport);

  rusb->PIPESEL = num;
  if ( rusb2_is_highspeed_rhport(rhport) ) {
    // PIPEBUF is PIPESEL-windowed (RA6M5 UM 29.2.35): write it after selecting the pipe.
    // FIXME BUFNMB is a fixed 0x08 for every pipe; a real per-pipe allocation scheme is needed.
    rusb->PIPEBUF = 0x7C08;
  }
  rusb->PIPEMAXP = mps;
  volatile uint16_t *ctr = get_pipectr(rusb, num);
  *ctr = RUSB2_PIPE_CTR_ACLRM_Msk | RUSB2_PIPE_CTR_SQCLR_Msk;
  *ctr = 0;
  unsigned cfg = (dir << 4) | epn;

  if (xfer == TUSB_XFER_BULK) {
    cfg |= (RUSB2_PIPECFG_TYPE_BULK | RUSB2_PIPECFG_SHTNAK_Msk | RUSB2_PIPECFG_DBLB_Msk);
  } else if (xfer == TUSB_XFER_INTERRUPT) {
    cfg |= RUSB2_PIPECFG_TYPE_INT;
  } else {
    cfg |= (RUSB2_PIPECFG_TYPE_ISO | RUSB2_PIPECFG_DBLB_Msk);
  }

  rusb->PIPECFG = cfg;
  rusb->BRDYSTS = 0x3FFu ^ TU_BIT(num);
  rusb->BRDYENB |= TU_BIT(num);

  /* 新开的管道一定是「未停车、不挂 BEMP」的干净状态。
   * Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c 的 dcd_edpt_open 同。 */
  rusb->BEMPENB &= (uint16_t)~TU_BIT(num);
  rusb->BEMPSTS = (uint16_t)~(rusb->BEMPSTS & TU_BIT(num));
  ra8_usb_rx_parked &= (uint16_t)~TU_BIT(num);

  if (dir || (xfer != TUSB_XFER_BULK)) {
    *ctr = RUSB2_PIPE_CTR_PID_BUF;
  }

  // TU_LOG1("O %d %x %x\r\n", rusb->PIPESEL, rusb->PIPECFG, rusb->PIPEMAXP);
  dcd_int_enable(rhport);

  return true;
}

static void edpt_close(uint8_t rhport, uint8_t ep_addr);

void dcd_edpt_close_all(uint8_t rhport)
{
  unsigned i = TU_ARRAY_SIZE(_dcd.pipe);
  dcd_int_disable(rhport);
  while (--i) { /* Close all pipes except 0 */
    const unsigned ep_addr = _dcd.pipe[i].ep;
    if (!ep_addr) {
      continue;
    }
    edpt_close(rhport, (uint8_t)ep_addr);
  }
  dcd_int_enable(rhport);
}

// Internal helper: on this (ISO_ALLOC) IP the stack no longer calls dcd_edpt_close(); only
// dcd_edpt_close_all() uses it to tear down each pipe.
static void edpt_close(uint8_t rhport, uint8_t ep_addr)
{
  rusb2_reg_t * rusb = RUSB2_REG(rhport);
  const unsigned epn = tu_edpt_number(ep_addr);
  const unsigned dir = tu_edpt_dir(ep_addr);
  const unsigned num = _dcd.ep[dir][epn];

  rusb->BRDYENB &= (uint16_t)~TU_BIT(num);
  /* 关掉本条 BEMP 及其状态位、清掉停车标记 —— 这条 pipe 马上就要被回收，
   * 留下的账会挂到下一条复用它的人身上。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:916-917、:922） */
  rusb->BEMPENB &= (uint16_t)~TU_BIT(num);
  rusb->BEMPSTS = (uint16_t)~(rusb->BEMPSTS & TU_BIT(num));
  volatile uint16_t *ctr = get_pipectr(rusb, num);
  *ctr = 0;
  rusb->PIPESEL = (uint16_t)num;
  rusb->PIPECFG = 0;
  _dcd.pipe[num].ep          = 0;
  ra8_usb_rx_parked &= (uint16_t)~TU_BIT(num);
  _dcd.pipe[num].queued      = false;
  _dcd.pipe[num].zlp_pending = false;
  _dcd.ep[dir][epn] = 0;
}

bool dcd_edpt_iso_alloc(uint8_t rhport, uint8_t ep_addr, uint16_t largest_packet_size) {
  rusb2_reg_t * rusb = RUSB2_REG(rhport);
  const unsigned epn = tu_edpt_number(ep_addr);
  const unsigned dir = tu_edpt_dir(ep_addr);

  // Fullspeed ISO is limited to 256 bytes
  if (!rusb2_is_highspeed_rhport(rhport) && largest_packet_size > 256) {
    return false;
  }

  // Reserve an ISO-capable pipe (1 or 2) once; it persists across altsetting changes so
  // dcd_edpt_iso_activate() only has to re-arm it in place (no pipe free/realloc, which on this
  // shared-register IP would churn PIPESEL/PIPECFG and disturb the other pipes).
  const unsigned num = find_pipe(TUSB_XFER_ISOCHRONOUS);
  TU_ASSERT(num);
  _dcd.pipe[num].ep = ep_addr;
  _dcd.ep[dir][epn] = num;

  dcd_int_disable(rhport);
  rusb->PIPESEL = (uint16_t) num;
  if (rusb2_is_highspeed_rhport(rhport)) {
    // PIPEBUF is PIPESEL-windowed (RA6M5 UM 29.2.35): write it after selecting the pipe.
    // FIXME (as in dcd_edpt_open): BUFNMB is a fixed 0x08 for every pipe; a real allocator is needed.
    rusb->PIPEBUF = 0x7C08;
  }
  rusb->PIPEMAXP = largest_packet_size;
  volatile uint16_t *ctr = get_pipectr(rusb, num);
  *ctr = RUSB2_PIPE_CTR_ACLRM_Msk | RUSB2_PIPE_CTR_SQCLR_Msk;
  *ctr = 0; // leave the pipe NAKing until activated
  rusb->PIPECFG = (uint16_t) ((dir << 4) | epn | RUSB2_PIPECFG_TYPE_ISO | RUSB2_PIPECFG_DBLB_Msk);
  rusb->BRDYSTS = (uint16_t) (0x3FFu ^ TU_BIT(num));
  rusb->BRDYENB |= TU_BIT(num);
  dcd_int_enable(rhport);
  return true;
}

bool dcd_edpt_iso_activate(uint8_t rhport, const tusb_desc_endpoint_t *desc_ep) {
  rusb2_reg_t * rusb = RUSB2_REG(rhport);
  const uint8_t ep_addr = desc_ep->bEndpointAddress;
  const unsigned epn = tu_edpt_number(ep_addr);
  const unsigned dir = tu_edpt_dir(ep_addr);
  const unsigned num = _dcd.ep[dir][epn];
  TU_ASSERT(num); // must have been iso-alloc'd

  dcd_int_disable(rhport);
  rusb->PIPESEL = (uint16_t) num;
  rusb->PIPEMAXP = tu_edpt_packet_size(desc_ep);
  volatile uint16_t *ctr = get_pipectr(rusb, num);
  *ctr = RUSB2_PIPE_CTR_ACLRM_Msk | RUSB2_PIPE_CTR_SQCLR_Msk; // abort in-flight + reset data toggle
  *ctr = 0;
  // a transfer armed before SET_INTERFACE survives to here (no dcd close on this port): drop the
  // stale bookkeeping so a BRDY firing before the class re-arms can't replay it
  pipe_state_t *pipe = &_dcd.pipe[num];
  pipe->buf = NULL;
  pipe->remaining = 0;
  pipe->queued = false;
  pipe->zlp_pending = false;
  /* 同 edpt_close：激活前把这条 pipe 的 BEMP 账清干净，否则 SET_INTERFACE
   * 之前的残留会在类驱动重新武装之前被当成一次完成上报。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:1044-1047） */
  rusb->BEMPENB &= (uint16_t)~TU_BIT(num);
  rusb->BEMPSTS = (uint16_t)~(rusb->BEMPSTS & TU_BIT(num));
  rusb->BRDYENB |= TU_BIT(num);
  ra8_usb_rx_parked &= (uint16_t)~TU_BIT(num);
  *ctr = RUSB2_PIPE_CTR_PID_BUF; // enable
  dcd_int_enable(rhport);
  return true;
}

bool dcd_edpt_xfer(uint8_t rhport, uint8_t ep_addr, uint8_t * buffer, uint16_t total_bytes, bool is_isr)
{
  (void) is_isr;
  rusb2_reg_t* rusb = RUSB2_REG(rhport);

  dcd_int_disable(rhport);
  bool r = process_edpt_xfer(rhport, rusb, 0, ep_addr, buffer, total_bytes);
  dcd_int_enable(rhport);

  return r;
}

bool dcd_edpt_xfer_fifo(uint8_t rhport, uint8_t ep_addr, tu_fifo_t * ff, uint16_t total_bytes, bool is_isr)
{
  (void) is_isr;
  // USB buffers always work in bytes so to avoid unnecessary divisions we demand item_size = 1
  rusb2_reg_t* rusb = RUSB2_REG(rhport);

  dcd_int_disable(rhport);
  bool r = process_edpt_xfer(rhport, rusb, 1, ep_addr, ff, total_bytes);
  dcd_int_enable(rhport);

  return r;
}

void dcd_edpt_stall(uint8_t rhport, uint8_t ep_addr)
{
  volatile uint16_t *ctr = ep_addr_to_pipectr(rhport, ep_addr);
  if (!ctr) {
    return;
  }
  dcd_int_disable(rhport);
  /* EP0 上如果已经有一个新的 SETUP 在等，stall 这次旧请求会连带把新请求
   * 也一起打掉（主机看到的是 STALL，于是放弃或复位端口）。交给设备栈按
   * 新 SETUP 的节奏走。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:1085-1090） */
  if (tu_edpt_number(ep_addr) == 0 &&
      RA8_EP0_NEW_SETUP_PENDING(RUSB2_REG(rhport))) {
    dcd_int_enable(rhport);
    return;
  }
  const uint32_t pid = *ctr & 0x3;
  *ctr = pid | RUSB2_PIPE_CTR_PID_STALL;
  *ctr = RUSB2_PIPE_CTR_PID_STALL;
  dcd_int_enable(rhport);
}

void dcd_edpt_clear_stall(uint8_t rhport, uint8_t ep_addr)
{
  rusb2_reg_t * rusb = RUSB2_REG(rhport);
  volatile uint16_t *ctr = ep_addr_to_pipectr(rhport, ep_addr);
  if (!ctr) {
    return;
  }

  dcd_int_disable(rhport);
  *ctr = RUSB2_PIPE_CTR_SQCLR_Msk;

  if (tu_edpt_dir(ep_addr)) { /* IN */
    *ctr = RUSB2_PIPE_CTR_PID_BUF;
  } else {
    const unsigned num = _dcd.ep[0][tu_edpt_number(ep_addr)];
    rusb->PIPESEL = (uint16_t)num;
    // Drop any packet parked in the buffer while halted: a data-OUT packet the host sent before
    // aborting its transfer would otherwise be delivered into the next read after recovery
    // (BOT reset + clear-halt re-arms a 31-byte CBW read which then receives stale WRITE data,
    // "SCSI CBW is not valid" -> stall -> reset loop; ra6m5 msc write wedge).
    *ctr = RUSB2_PIPE_CTR_ACLRM_Msk;
    *ctr = 0;
    // Non-bulk OUT re-enables straight away. Bulk OUT is normally armed together with its transaction
    // counter (TRE) by process_pipe_xfer(), so we don't blindly re-enable it here — but if a receive
    // was already armed (still queued), SQCLR above just left it NAKing. Re-assert BUF so it keeps
    // receiving; the class driver still considers that read submitted and never re-arms it, so
    // otherwise the endpoint NAKs forever (usbtest toggle test 29 clears the halt on an armed pipe).
    // `queued` (not `buf`) is the armed test: a zero-length OUT read has buf==NULL yet is armed.
    if (rusb->PIPECFG_b.TYPE != 1 || _dcd.pipe[num].queued) {
      *ctr = RUSB2_PIPE_CTR_PID_BUF;
    }
  }
  dcd_int_enable(rhport);
}

//--------------------------------------------------------------------+
// ISR
//--------------------------------------------------------------------+

#if defined(__CCRX__)
TU_ATTR_ALWAYS_INLINE static inline unsigned __builtin_ctz(unsigned int value) {
  unsigned int count = 0;
  while ((value & 1) == 0) {
    value >>= 1;
    count++;
  }
  return count;
}
#endif

void dcd_int_handler(uint8_t rhport)
{
  rusb2_reg_t* rusb = RUSB2_REG(rhport);

  uint16_t is0 = rusb->INTSTS0;
  /* 本次中断里刚处理过一次总线复位 / 刚收到一个 SETUP。
   * 两者都会让 EP0 的硬件状态在**本次 ISR 的中途**被改写，后面的
   * status / BEMP / BRDY 分发若继续按旧状态跑，就会跟新状态打架 ——
   * 尤其是 BRDY 里那个属于上一个控制传输的 bit0。
   * （Titan_Mini_Openmv/platform/port/dcd_rusb2_ra8.c:1149-1150） */
  bool setup_seen = false;
  bool reset_seen = false;

  /* clear active bits except VALID (don't write 0 to already cleared bits according to the HW manual) */
  rusb->INTSTS0 = ~((RUSB2_INTSTS0_CTRT_Msk | RUSB2_INTSTS0_DVST_Msk | RUSB2_INTSTS0_SOFR_Msk |
                     RUSB2_INTSTS0_RESM_Msk | RUSB2_INTSTS0_VBINT_Msk) & is0) | RUSB2_INTSTS0_VALID_Msk;

  // VBUS changes
  if ( is0 & RUSB2_INTSTS0_VBINT_Msk ) {
    /* 走带边沿检测的版本 —— 它还会在 VBUS 真的消失时发 DCD_EVENT_UNPLUGGED
     * （本板原来只看电平，那个事件在全工程从没被发出过）。 */
    ra8_usb_vbus_change(rhport, rusb->INTSTS0_b.VBSTS != 0);
  }

  // Resumed
  if ( is0 & RUSB2_INTSTS0_RESM_Msk ) {
    dcd_event_bus_signal(rhport, DCD_EVENT_RESUME, true);
    if (!_dcd.sof_enabled) {
      rusb->INTENB0_b.SOFE = 0;
    }
  }

  // SOF received
  if ( (is0 & RUSB2_INTSTS0_SOFR_Msk) && rusb->INTENB0_b.SOFE ) {
    // USBD will exit suspended mode when SOF event is received
    const uint32_t frame = rusb->FRMNUM_b.FRNM;
    dcd_event_sof(rhport, frame, true);
    if (!_dcd.sof_enabled) {
      rusb->INTENB0_b.SOFE = 0;
    }
  }

  // Device state changes
  if ( is0 & RUSB2_INTSTS0_DVST_Msk ) {
    switch (is0 & RUSB2_INTSTS0_DVSQ_Msk) {
      case RUSB2_INTSTS0_DVSQ_STATE_DEF:
        process_bus_reset(rhport);
        reset_seen = true;
        break;

      case RUSB2_INTSTS0_DVSQ_STATE_ADDR:
        setup_seen = process_set_address(rhport);
        break;

      case RUSB2_INTSTS0_DVSQ_STATE_SUSP0:
      case RUSB2_INTSTS0_DVSQ_STATE_SUSP1:
      case RUSB2_INTSTS0_DVSQ_STATE_SUSP2:
      case RUSB2_INTSTS0_DVSQ_STATE_SUSP3:
        dcd_event_bus_signal(rhport, DCD_EVENT_SUSPEND, true);
        if (!_dcd.sof_enabled) {
          rusb->INTENB0_b.SOFE = 1;
        }

      default: break;
    }
  }

//  if ( is0 & RUSB2_INTSTS0_NRDY_Msk ) {
//    rusb->NRDYSTS = 0;
//  }

  // Control transfer stage changes
  if ( is0 & RUSB2_INTSTS0_CTRT_Msk ) {
    if ( is0 & RUSB2_INTSTS0_CTSQ_CTRL_RDATA ) {
      /* A setup packet has been received. */
      setup_seen |= (rusb->INTSTS0 & RUSB2_INTSTS0_VALID_Msk) != 0;
      process_setup_packet(rhport);
    } else if (!reset_seen && (0 == (is0 & RUSB2_INTSTS0_CTSQ_Msk))) {
      /* A ZLP has been sent/received. */
      process_status_completion(rhport);
    }
  }

  // Buffer empty
  if (!reset_seen && (is0 & RUSB2_INTSTS0_BEMP_Msk)) {
    /* 只认「使能了且置位了」的位：BEMPSTS 里可能挂着没被使能的 pipe 的残留，
     * 写 ~s 清掉它们会顺手把别的东西也清了。 */
    const uint16_t s = rusb->BEMPSTS & rusb->BEMPENB;
    rusb->BEMPSTS = (uint16_t)~s;
    /* EP0 刚收到新 SETUP 时，别用旧账去上报一次完成。 */
    if (!setup_seen && (s & 1)) {
      process_pipe0_bemp(rhport);
    }
    /* pipe 1..5 = 支持双缓冲的数据 pipe，它们的 IN 完成由 BEMP 判定
     * （见 ra8_usb_process_pipe_bemp）。上游从来不服务这些位。 */
    unsigned pending = s & 0x3eU;
    while (pending) {
      unsigned num = __builtin_ctz(pending);
      ra8_usb_process_pipe_bemp(rhport, num);
      pending &= ~TU_BIT(num);
    }
  }

  // Buffer ready
  if (!reset_seen && (is0 & RUSB2_INTSTS0_BRDY_Msk)) {
    const unsigned m = rusb->BRDYENB;
    unsigned s = rusb->BRDYSTS & m;
    /* 刚处理过 SETUP → 丢掉 EP0 的 BRDY：那一个是上一个控制传输的余波。 */
    if (setup_seen) s &= ~1U;
    /* clear active bits (don't write 0 to already cleared bits according to the HW manual) */
    rusb->BRDYSTS = ~s;
    while (s) {
      const unsigned num = __builtin_ctz(s);
      process_pipe_brdy(rhport, num);
      s &= ~TU_BIT(num);
    }
  }
}
#endif

#endif /* RA8_DCD_FORK_INCLUDED */
