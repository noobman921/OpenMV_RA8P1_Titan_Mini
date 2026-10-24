#ifndef MICROPYTHON_PORTS_RENESAS_RA_MOCONFIGPORT_H_
#define MICROPYTHON_PORTS_RENESAS_RA_MOCONFIGPORT_H_

#include <stdint.h>
//#include "hal_data.h"
#include "mpy_thread.h"
#include "mpconfigboard.h"

// options to control how MicroPython is built

// Use the minimal starting configuration (disables all optional features).
//#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)
// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER     (1)

// 核心功能
#define MICROPY_ENABLE_GC                 (1) //垃圾回收
#define MICROPY_HELPER_REPL               (1) //REPL
#define MICROPY_GCREGS_SETJMP             (1)

#define MICROPY_QSTR_EXTRA_POOL           mp_qstr_frozen_const_pool
#define MICROPY_MODULE_FROZEN_MPY         (1)
//#define MICROPY_ENABLE_EXTERNAL_IMPORT    (1)

#define MICROPY_ALLOC_PATH_MAX            (256)

// Use the minimum headroom in the chunk allocator for parse nodes.
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

// scheduler
#define MICROPY_SCHEDULER_DEPTH     (8)
#define MICROPY_SCHEDULER_STATIC_NODES (1)

// long int
#define MICROPY_LONGINT_IMPL MICROPY_LONGINT_IMPL_MPZ

// type definitions for the specific machine
#define MP_SSIZE_MAX (0x7fffffff)
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

// 启用 readline 历史记录
#define MICROPY_ENABLE_READLINE_HISTORY     (1)
#define MICROPY_READLINE_HISTORY_SIZE       (8)   // 历史记录条数

// 函数声明
void mp_uart_repl_init(void); //REPL初始化 uart_core.c

#define MP_STATE_PORT MP_STATE_VM

// GPIO

// tinyusb
#define MICROPY_HW_ENABLE_USBDEV    (1)
#define MICROPY_HW_TINYUSB_STACK    (1)
#define MICROPY_HW_USB_CDC          (1)

/* 本工程有一个专用 USB 任务（omvmain.c 的 ra8_svc_task）独家泵 tud_task_ext()。
 * 置 1 会让 shared/tinyusb/mp_usbd.c 的 mp_usbd_task() 变成空操作 —— 否则
 * MP 的 stdio 路径（mp_hal_stdin_rx_chr → mp_usbd_cdc_poll_interfaces →
 * mp_usbd_task）会从 VM 线程再泵一次，与专用任务并发进入不可重入的
 * tud_task_ext()。详见 mp_usbd.c 里的说明。 */
#define MICROPY_HW_USB_DEDICATED_TASK   (1)
#define CFG_TUD_ENDPPOINT_MAX       (6)  // CDC(3) + MSC(2) + reserve(1)
#define CFG_TUSB_MCU                OPT_MCU_RAXXX
#define MICROPY_WRAP_TUD_CDC_RX_CB(name)         __mp_##name
#define MICROPY_WRAP_TUD_CDC_LINE_STATE_CB(name)  __mp_##name
#define MICROPY_WRAP_TUD_EVENT_HOOK_CB(name)     __mp_##name
// hsusb —— 切回 FS 只需换成下一行（P413 多路开关会跟着自动切，见 omvmain.c）
//   #define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE)
#define CFG_TUSB_RHPORT1_MODE   (OPT_MODE_DEVICE | OPT_MODE_HIGH_SPEED)
// 端点尺寸故意不写死：tinyusb 会按 TUD_EPSIZE_BULK_MAX 自动取
// （tusb_types.h:102，FS=64 / HS=512）。写死 512 会让 FS 下
// tu_edpt_stream 的 mps 与实际端点不符，read_xfer 里 `available >= mps`
// 的前置检查就永远不成立，端点不会被武装。
#define CFG_TUD_CDC_TX_BUFSIZE  8192
#define CFG_TUD_CDC_RX_BUFSIZE  2048
#define CFG_TUD_ENDPOINT0_BUFSIZE   64

/* ---- 以下三项对齐 Titan_Mini_Openmv/platform/usb/tusb_config.h ----
 *
 * ① CFG_TUD_TASK_QUEUE_SZ：USBD 事件队列深度。默认 16，Titan 用 32。
 *    queue_event() 里是 `TU_ASSERT(osal_queue_send(...))`，而该队列的 tu_fifo
 *    是**非 overwritable** 的 —— 队列满时写入失败，TU_ASSERT 在 ISR 里**静默丢弃**
 *    这个 USB 事件。丢掉一个 OUT 端点的 XFER_COMPLETE，端点就不会被重新武装，
 *    主机的后续字节永远送不进来（实测症状：脚本包只到 776/1173 字节）。
 *    深度翻倍 = 突发时多一倍余量。
 *
 * ② CFG_TUD_TASK_EVENTS_PER_RUN：tud_task_ext() **单次调用**最多处理多少个事件
 *    （usbd.c:680-686）。默认 16，我们每 1ms 才调用一次 → 上限只有 16 事件/ms，
 *    是 HS 下的吞吐瓶颈。Titan 没改这个值，但它用专用线程**紧循环连续泵**；
 *    我们的 svc 任务也改成排空后再让出（见 omvmain.c），两边等价。 */
#define CFG_TUD_TASK_QUEUE_SZ        32
#define CFG_TUD_TASK_EVENTS_PER_RUN  64

/* ③ TX FIFO 在未连接（DTR=0）时不得被覆盖。默认 1（可覆盖），Titan 显式设 0。
 *    原注释：「An armed HWFIFO transfer owns its queued bytes even while DTR is low.」
 *    我们能跑不代表没问题 —— CDC 的 IN 方向正在被硬件 FIFO 持有，
 *    此时覆盖 FIFO 会让已排队的字节和端点状态不一致。 */
#define CFG_TUD_CDC_TX_OVERWRITABLE_IF_NOT_CONNECTED  0
//#define MICROPY_HW_USB_VID          (0x1209)
//#define MICROPY_HW_USB_PID          (0xABD1)

#define MICROPY_HW_USB_VID          (0x37C5)
#define MICROPY_HW_USB_PID          (0x1204)
// OpenMV
// 必须用单精度：ulab 的 FLOAT_TYPECODE 由本宏推导（ndarray.h:31），
// 而 py_image.c 的 to_ndarray('f') 硬编码 4 字节；用 DOUBLE 会让两者
// dtype 不一致，ML 预处理时抛 "cannot cast output with casting rule"。
#define MICROPY_FLOAT_IMPL (MICROPY_FLOAT_IMPL_FLOAT)
#define CMSIS_MCU_H  "../../../../ra/fsp/src/bsp/cmsis/Device/RENESAS/Include/R7KA8P1KF_core0.h"

/* 传输通道由平台补丁源码提供（Titan 模式）：
 *   src/OpenMV/ports/ra8/ra8_usb_channel.c          → omv_usb_channel
 *   src/OpenMV/ports/ra8/ra8_protocol_channel_stdio.c → omv_stdout/stdin_channel
 *
 * **故意不定义 OMV_USB_STACK_TINYUSB**：上游
 *   protocol/omv_protocol_channel_tinyusb.c
 * 整份包在 `#if OMV_USB_STACK_TINYUSB` 里，不定义就编成空文件 ——
 * 这样 src/OpenMV/protocol/ 可以保持与 openmv-master 逐字相同，
 * 将来整体升级上游时不会产生冲突。（Titan 用的是同一套做法。）
 *
 * stdio 通道没有这种闸门，所以在 .cproject 的 excluding 串里把上游那份排除，
 * 由我们的 ra8_protocol_channel_stdio.c 替代（Titan 的 SConscript:48 同样排除它）。 */
#define OMV_USB_CHANNEL_RA8     (1)

/* AprilTag 库（src/OpenMV/lib/apriltag/）需要这个宏才走用户配置 ——
 * 定义后 common/config.h 会 #include "apriltag_config.h"（在 lib/imlib/），
 * 把分配器重定向到 uma、断言置空、并关掉全部特性
 * （PTHREADS / IMAGE_IO / DEBUG / PROFILE = 0）。
 *
 * ⚠️ 为什么必须放在这里而不是编译选项里：这是 e2studio 工程，
 * makefile/subdir.mk 是【自动生成】的，手工加的 -D 会被下一次重新生成冲掉
 * （已踩过一次）。本文件被每个编译单元 -include 强制包含，所以放这里是持久的。
 *
 * 未定义时的后果：apriltag 用默认配置（全部特性=1）→ 拉进 pthread_*、
 * utime_now、fopen/fprintf → 链接期一堆 undefined reference。 */
#define APRILTAG_HAVE_CONFIG    (1)

/* newlib 系统调用桩（src/OpenMV/common/nosys_stubs.c）。
 *
 * 为什么必须定义：对齐上游 imlib 后引入 apriltag 库，它 include <stdio.h>
 * 并使用 FILE 相关接口，于是 newlib 会拉进 closer.c / fstatr.c / isattyr.c /
 * lseekr.c / writer.c / readr.c —— 分别需要 _close / _fstat / _isatty /
 * _lseek / _write / _read，否则链接期 undefined reference。
 *
 * 为什么放这里：e2studio 的 makefile/subdir.mk 是自动生成的，手工加的 -D 会被
 * 重新生成冲掉（已踩过两次）。本文件被每个编译单元 -include 强制包含。
 *
 * ⚠️ nosys_stubs.c 里的 _write/_read 是 ENOSYS 版本，取代了 TinyUSB 示例 BSP
 * （hw/bsp/board.c，已删除）那套转发到 board_uart_write/read 的实现 ——
 * 那对接口在本移植上没有定义（本板 USB 走 rusb2 DCD，不是 UART 桥）。 */
#define OMV_NOSYS_STUBS_ENABLE  (1)
// OpenMV 协议模块。上游在 Makefile 里传 -DMICROPY_PY_PROTOCOL=1，本工程走
// e2studio 构建没有那条链路，必须在此定义：否则 framebuffer.c 的帧就绪通知
// (#if MICROPY_PY_PROTOCOL) 被静默编掉，IDE 收不到"新帧可用"，退化成 ~1Hz
// 轮询要帧，实测帧率卡在 1fps（USB 本身只忙 2%）。
// 改本文件后必须重跑两处：
//   python src/MicroPython/gen_qstr.py   (重生 qstr/moduledefs/root_pointers)
//   bash build_tflm_backend.sh           (它 -include 本文件，ABI 必须一致)
#define MICROPY_PY_PROTOCOL     (1)

// Ethos-U55 NPU。上游在 Makefile 里传 -DETHOS_U（openmv/ports/alif/alif.mk:68），
// 本工程走 e2studio 构建没有那条链路，必须在此定义：否则 tflm_backend.cc:132 的
// resolver->AddEthosU() 被静默编掉，Vela 编译过的模型里那个 "ethos-u" 自定义算子
// 就找不到注册项，加载即失败。注意光定义本宏还不够——链接的归档也必须换成
// libtflm-cortex-m55-u55-release.a，否则注册到的是个空壳算子。
// 改本文件后同样要重跑 gen_qstr.py 与 build_tflm_backend.sh。
#define ETHOS_U                 (1)

// soft_timer 的时间基【故意不在这里定义】。
// 定义 MICROPY_SOFT_TIMER_TICKS_MS 会让 softtimer.c 把它当变量用
// （shared/runtime/softtimer.c:36），我们就得维护一个别人持续递增的 uwTick ——
// 它已经两次因驱动任务被饿死而冻结，导致所有软定时器停摆。
// 改为由 ports/renesas-ra/systick.c 提供 soft_timer_get_ms() /
// soft_timer_schedule_at_ms()，时间基直接取 FreeRTOS tick，永不冻结。
// （对应 Titan_Mini_Openmv/platform/micropython/mphal.c:90-91）
#define MICROPY_ENABLE_VM_ABORT (1)
#define MICROPY_PY_CSI          (1) // 摄像头 (旧名, 注册为 sensor)
#define MICROPY_PY_CSI_NG       (1) // 新 API, 注册为 csi (OpenMV 5 起)

// time 模块的端口扩展（现在只用来挂 time.clock()）。
// micropython 各端口统一用这个钩子，extmod/modtime.c 不需要改。
#define MICROPY_PY_TIME_INCLUDEFILE "ports/renesas-ra/modtime.c"

#if MICROPY_PY_THREAD
#define MICROPY_EVENT_POLL_HOOK \
    do {

    } while (0);

#define MICROPY_THREAD_YIELD() pyb_thread_yield()
#else
// REPL 的等待循环（uart_core.c 的 mp_hal_stdin_rx_chr）会一路调这个宏。
// 它是**不阻塞**的：mp_handle_pending() 与 __WFI() 都不会把 CPU 让给更低
// 优先级的任务（__WFI 只是睡到下一个中断，醒来后调度器仍挑最高优先级的就绪
// 任务，也就是 MPY Thread 自己）。于是任何优先级 < 3 的任务在 REPL 等输入期间
// 永远轮不到 —— 这正是 soft_timer 服务任务长期不执行的原因（见 omvmain.c）。
//
// MSC 的介质 worker 优先级 2，必须靠这里主动让出才有机会跑：否则 Windows 发来的
// U 盘命令永远等不到完成，约 10s 后主机复位端口，连 CDC 会话一起打掉。
// 只在确实有待处理作业时让出，平常路径不变。
// （对应 Titan_Mini_Openmv/platform/micropython/mphal.c 的 rt_thread_mdelay(1)）
//
// ⚠️ 2026-10-19：曾尝试按 Titan 的写法在这里追加 ra8_hal_poll()，并定义
// MICROPY_INTERNAL_EVENT_HOOK ra8_hal_poll()（让 imlib 的 ~50 处保活点直接跑
// 协议轮询）。**实测帧率与稳定性同时下降**，已回退。
// 原因推断：imlib_poll_events 的调用点分布在每帧要执行几十次的热循环里，
// 每次进去都做一遍完整的 siphon + 状态机，开销远超收益；
// 而 MICROPY_EVENT_POLL_HOOK 本身在 REPL 忙循环里每秒也被调用上千次。
// 协议轮询维持在 10ms 调度节拍（ra8_vm_poll_task）这一条路径上。
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        extern void ra8_vm_yield_if_needed(void); \
        mp_handle_pending(true); \
        ra8_vm_yield_if_needed(); \
        __WFI(); \
    } while (0);
#endif

#endif
