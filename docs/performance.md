# 性能对比

对比平台：RA8P1 Titan Mini、OpenMV Cam H7 Plus、OpenMV Cam RT1062、OpenMV N6、OpenMV AE3。

此文档参考RTThread的版本进行测评对比，测量脚本不一致。如果涉及一些芯片或硬件平台的性能测试，会直接采用RTThread的版本。以下本项目的名称定为 RA8P1 OMV。

## 平台配置

| 项目 | RA8P1 Titan Mini | OpenMV H7 Plus | OpenMV RT1062 |
|---|---|---|---|
| CPU | Cortex-M85，1 GHz | Cortex-M7，480 MHz | Cortex-M7，600 MHz |
| CPU 计算扩展 | DSP、FPU、Helium MVE | DSP、FPU | DSP、FPU |
| NPU | Ethos-U55-256，500 MHz | 无 | 无 |
| NPU INT8 理论峰值 | 256 GOPS | — | — |
| 外部 SDRAM | 32 MiB，16-bit | 32 MB，32-bit，100 MHz | 32 MB，16-bit，160 MHz |
| 相机接口 | OV5640，MIPI-CSI / VIN | OV5640，并行 DCMI | OV5640，并行 CSI |
| JPEG 编码 | 软件 | 硬件 | 软件 |
| USB | HS，480 Mb/s | FS，12 Mb/s | HS，480 Mb/s |

规格来源：[Renesas RA8P1](https://www.renesas.com/en/products/ra8p1)、[H7 Plus](https://openmv.io/products/openmv-cam-h7-plus)、[RT1062](https://openmv.io/products/openmv-cam-rt)。

## CPU 跑分

| 平台 | CPU 主频 | 单核 CoreMark | 数据来源 |
|---|---:|---:|---|
| RA8P1 Cortex-M85 | 1 GHz | 6362 | [EEMBC 提交结果](https://www.eembc.org/viewer/?benchmark_seq=13714) |
| OpenMV H7 Plus | 480 MHz | 2400 | OpenMV 官方规格 |
| OpenMV RT1062 | 600 MHz | 3020 | OpenMV 官方规格 |

## 图像处理耗时

分辨率：**QVGA（320×240）**。单位：**ms / 次，平均值，越小越快**。

RA8P1：CPU 1 GHz，I/D cache 开启；每项运行 10 次，3 轮各采样 100 次。分别使用 SDRAM、SRAM 工作图，计时不含采集、输入恢复和预览。

| 测试项目 | 格式 / 参数 | H7 Plus（官方） | RT1062（官方） | RA8P1 SDRAM | RA8P1 SRAM | RA8P1 OMV |
|---|---|---:|---:|---:|---:|---:|
| 反色 `invert()` | 灰度 | 0.80 | 0.74 | 0.937 | 0.085 | 0.213 |
| 帧差 `difference()` | 灰度 | 0.87 | 1.13 | 2.065 | 1.218 | 0.886 |
| 腐蚀 / 膨胀 | 灰度，3×3 | 4.27（官方共列） | 3.88（官方共列） | 2.735 / 2.745 | 2.134 / 2.138 | 2.439 / 2.440 |
| 均值滤波 `mean()` | 灰度，3×3 | 4.36 | 4.03 | 4.103 | 3.968 | 3.820 |
| 卷积 `morph()` | 灰度，3×3 | 11.87 | 10.05 | 2.915 | 2.762 | 2.931 |
| JPEG 编码 | 灰度，quality=90 | 1.25 | 5.54 | 4.140 | 3.479 | 5.215 |
| JPEG 编码 | RGB565，quality=90 | 4.94 | 16.44 | 10.517 | 9.575 | 15.480 |
| 灰度转换 `to_grayscale()` | RGB565 | 4.60 | 4.69 | 3.863 | 2.348 | 2.642 |

RA8P1 使用本地采集图；帧差参考为输入的反色图，卷积使用 3×3 锐化核。两块 OpenMV 的数值取自官方公开测试。此部分数据延用RTThread

RA8P1 OMV依照RTThread的方式测量。

数据来源：[OpenMV 官方基准](https://docs.google.com/spreadsheets/d/1-FNVKCEr8-6UYs8MUm6wgsOt2c8ihJ2mg9QXKkG91os/edit?usp=sharing)及 2026-09-17 的 RA8P1 板端实测。

## 神经网络示例模型推理耗时

测试范围：[神经网络示例](../examples/02_机器学习/00_神经网络)，23 个脚本对应 20 个 NPU 模型和 1 个 CPU 回归模型。单位 **ms / 次**。

RA8P1 测试固件：HEX SHA-256 前缀 `5aace4b1`。使用示例默认模型，固定输入，预热 5 次、3 轮各采样 30 次；计时包含输入复制、推理和输出回调，不含相机采集、图像预处理、检测后处理与绘图。

目前测试的方法为，如果sram存的下，则测sram性能，存不下，则测sdram。即每个选项为目前固件的最优性能。因此会出现某个模型性能下降很多的情况。

| 模型 | 输入 | RA8P1 / ms | OpenMV N6 / ms | OpenMV AE3 / ms | RA8P1 OMV / ms|
|---|---|---:|---:|---:|---:|
| FOMO 人脸中心检测 | 96×96 | 1.086 | — | — | 2.018 |
| BlazeFace（FLOAT32 接口） | 128×128 | 14.988 | — | — | 10.757 |
| BlazeFace（Renesas INT8） | 128×128 | 13.061 | — | — | 6.668 |
| BlazePalm 手掌检测 | 192×192 | 57.683 | — | — | 41.117 |
| Face Landmarks 468 点 | 192×192 | 16.839 | — | — | 19.957 |
| Hand Landmarks 21 点 | 224×224 | 31.586 | — | — | 18.469 |
| MoveNet 单人姿态 | 192×192 | 22.918 | — | — | 120.673 |
| YOLO LC 行人检测 | 192×192 | 5.941 | — | — | 5.269 |
| YOLOv8n person（示例 Vela 4） | 192×192 | 36.685 | 24.15 | 51.47 | 23.327 |
| YOLOv8n COCO80 | 192×192 | 29.787 | — | — | 29.304 |
| YOLOv8n Pose | 192×192 | 28.294 | — | — | 27.996 |
| YOLOv5n6-xiao VOC20 | 192×192 | 6.849 | — | — | 14.255 |
| YOLO-Fastest 灰度人脸 | 192×192 | 5.789 | — | — | 6.758 |
| Swift-YOLO Gesture | 192×192 | 8.223 | — | — | 10.060 |
| Swift-YOLO Pet | 192×192 | 7.916 | — | — | 9.492 |
| Swift-YOLO Nano Person | 192×192 | 14.940 | — | — | 13.844 |
| U-Net MobileNetV2 分割 | 128×128 | 46.091 | — | — | 130.081 |
| VWW 人体存在分类 | 96×96 | 0.609 | — | — | 1.798 |
| ResNet8 CIFAR10 | 32×32 | 2.261 | — | — | 2.397 |
| MobileNet V1 0.25 | 224×224 | 5.370 | — | — | 6.407 |
| 数值回归（CPU） | 1×36 | 0.051 | — | — | — |

单脸/多脸共用 BlazeFace 与 Face Landmarks；单手/多手共用 BlazePalm 与 Hand Landmarks。表中为各模型独立调用耗时。YOLOv8n person 使用示例自带的 Vela 4 CPU Softmax 版本。

“—”表示未检索到对应模型的 N6 / AE3 公开耗时。YOLOv8n 数据来自 [OpenMV 官方测试表](https://docs.google.com/spreadsheets/d/1-FNVKCEr8-6UYs8MUm6wgsOt2c8ihJ2mg9QXKkG91os/edit#gid=1692885012)。RA8P1 数据延用RTThread结果。

