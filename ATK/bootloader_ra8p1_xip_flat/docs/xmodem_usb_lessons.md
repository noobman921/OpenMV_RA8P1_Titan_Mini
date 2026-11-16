# RA8P1 裸机 USB CDC + XMODEM 移植经验

工程：`ws8/bootloader_ra8p1_xip_flat`
目标：上电后通过 USB CDC 收 XMODEM，把镜像写进外部 OSPI（OSPI0 CS0，0x80000000），校验后跳转。

结论分级：
- **【已验证】** 本轮通过修改后实际跑通、或由日志/寄存器/源码直接证明
- **【推断】** 与现象一致但未单独做对照实验

---

## 1. 裸机 FSP 的 USB 必须轮询 `R_USB_EventGet()`【已验证】

**现象**：USB 设备能枚举（Windows 出现未知设备），但 `usbfs_interrupt_handler` 一次都不进；读取寄存器发现 NVIC/ICU/INTENB 全部正确。

**根因**：`BSP_CFG_RTOS == 0` 时，FSP 的 USB 状态机由 `R_USB_EventGet()` 内部调用的
`usb_cstd_usb_task()` 推进；应用回调在裸机下**根本不会被调用**
（`R_USB_Callback()` 的注释明确写着 "RTOS only"，驱动里 `g_usb_apl_callback[]` 只赋值、无调用点）。

**证据**：
- `ra/fsp/src/r_usb_basic/r_usb_basic.c:315` `R_USB_EventGet()` 的 `#if (BSP_CFG_RTOS == 0)` 分支
- 例程 `ws8/38_usb_fs_pcdc/BSP/USB_UART/usb_uart.c` 的 `usb_uart_task()` 就是逐事件轮询
- 改成本方案后 COM 口正常出现

**做法**：主循环里反复调用一个 `usb_poll()`，处理 CONFIGURED / READ_COMPLETE / REQUEST /
WRITE_COMPLETE / DETACH / SUSPEND / RESUME 七类事件；所有等待循环（发送、接收）内部都要继续轮询。

---

## 2. USB 不能挂 DMA/DTC 传输，否则数据阶段写失败【已验证】

**现象**：设备配置成功（`R_USB_InfoGet` 返回 `device_status=3` = CONFIGURED），
但 `R_USB_Write()` 一直返回 **1506**（`FSP_ERR_USB_FAILED`），Tera Term 收不到任何数据。

**根因**：工程给 USB 挂了两个 `transfer_on_dmac`（TX/RX），
`usb_data_write()` 走 DMA 路径时 `usb_pstd_transfer_start()` 失败，被外层映射成 1506；
控制传输（枚举、类请求）走 CPU，所以看起来"设备正常、只是发不出数据"。

**证据**：
- 配置对比：ATK 裸机例程 `38_usb_fs_pcdc` 的 USB **没有**挂 transfer（0 处）；
  我们的工程挂了两条（`g_transfer0`/`g_transfer1`，触发源 `usbfs.fifo.0/1`）
- 去掉后立刻正常：Tera Term 收到 `XMODEM ready, waiting for image...`

**做法**：USB Basic 的 Transfer(TX)/Transfer(RX) 全部设为 **None**，与例程一致。

---

## 3. 1506 的定位方法【已验证】

`R_USB_Write()` 返回 `FSP_ERR_USB_FAILED` 有两种来源：

1. `R_USB_InfoGet()` 认为状态不是 CONFIGURED
2. `usb_data_write()` 内部失败

用一行诊断即可区分：

```c
usb_info_t info;
R_USB_InfoGet(&g_basic0_ctrl, &info, 0U);
printf("usb: write err=%d ds=%d spd=%d\r\n", err, info.device_status, info.speed);
```

`usb_status_t` 数值：`POWERED=0, DEFAULT=1, ADDRESS=2, **CONFIGURED=3**, SUSPEND=4, RESUME=5,
DETACH=6, REQUEST=7, REQUEST_COMPLETE=8, READ_COMPLETE=9, WRITE_COMPLETE=10`。

日志里 `ds=3` 就说明状态没问题，问题在数据通路（本次即 DMA）。

**注意**：不要只看 `ds` 的直觉值——我们一度把 3 误当成 ADDRESS，浪费了一轮。

---

## 4. UFSR `0x100` 是 UNALIGNED，不是 STKOF【已验证】

**现象**：一开始传输镜像就进入 UsageFault，UFSR = 0x100。

**根因**：`R_OSPI_B_Write()` 内部按 `uint64_t` 访问 xSPI 映射窗口，
源指针必须 8 字节对齐。我们传的是 `&frame[3]`（栈数组偏移 3），必然不对齐。

**证据**：
- CMSIS 位定义：UFSR bit0=UNDEFINSTR、bit1=INVSTATE、**bit8=UNALIGNED(0x100)**、bit16=STKOF(0x10000)
- 例程 `primary_cpu0_ra8p1/src/xmodem/xmodem.c` 正是用
  `uint8_t aligned_buf[128] BSP_ALIGN_VARIABLE(8); memcpy(...)` 规避的
- 改成对齐缓冲后传输正常

**做法**：写 OSPI 前把数据 `memcpy` 到 `BSP_ALIGN_VARIABLE(8)` 的缓冲；目标地址本身也要 8 字节对齐。

---

## 5. OSPI 相关的其它约束

| 项 | 结论 | 证据等级 |
|---|---|---|
| 引脚必须配全：CS + SCLK + SIO0..SIO3，且驱动能力 High | 只配 CS 时传输卡在 `FSP_HARDWARE_REGISTER_WAIT(CDCTL0_b.TRREQ, 0)` | 【推断】配置对比确认差异，修复后未复测该工程 |
| 初始化顺序：`R_OSPI_B_Open` → `LIOCFGCS_b[0].SDRSMPMD=1` → `0x66/0x99` → 延时 → `0x06` 写使能 | 与 ATK `10_norflash` 例程一致 | 【已验证】ID 能读到 `0x1768` |
| `address_msb_mask` 必须为 0xF0（3 字节地址时屏蔽 CS 基址高 4 位） | 为 0x00 时映射读地址偏移 8MB，读回全 FF | 【推断】按例程对齐后映射读恢复 |
| xSPI 窗口在复位状态下不可访问，只有程序 `R_OSPI_B_Open` 之后才可读 | J-Link 复位后 `mem 0x80000000/0x90000000` 均 "Could not read memory"；运行中可读 | 【已验证】 |
| 镜像写入外部 flash 只能靠程序自己擦写 | J-Link 8.12 对 CS0 报 `Algo reported a flash size of 0 bytes`；ws7 的镜像实际由 bootloader 经 USB CDC XMODEM 写入 | 【已验证】 |

---

## 6. 调试方法（本次最有效的手段）

1. **先找工作正常的例程，照着它的调用顺序和事件模型移植**，不要自创方案。
2. **读寄存器定位**：J-Link 直读 `NVIC->ISER/ISPR`、`R_ICU->IELSR[n]`、USB 的 `INTSTS0/DVSTCTR0/INTENB0`，
   一眼就能判断"中断没使能 / 没链接 / 外设没状态"。
3. **在关键分支加最小诊断**：事件号、返回码、`device_status`，比猜快得多。
4. **对比配置时用脚本逐项 diff**（去掉注释/空白），比肉眼可靠。

---

## 7. 本次改动的文件

`ws8/bootloader_ra8p1_xip_flat/src/`

| 文件 | 说明 |
|---|---|
| `flash_download.c/.h` | USB CDC 轮询层 + XMODEM 接收 + OSPI 擦写 + 入口 |
| `r_usb_pcdc_descriptor.c` | 原样复制自 ws7 例程 |
| `hal_entry.c` | 打开 USB → 等镜像 → 收完重开 OSPI → 跳转 |

关键约束回顾：
- 接收环形缓冲 1KB，`usb_poll()` 每毫秒轮询多次
- 每 128 字节块：跨 4KB 边界先擦扇区 → 分两次 64 字节写（各 8 字节对齐）→ 等 WIP
- 写完回 EOT 后关掉重开 OSPI，再跳转
