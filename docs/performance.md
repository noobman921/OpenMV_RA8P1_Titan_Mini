# 性能测试


## 平台配置

| 项目 | RA8P1 Titan Mini |
|---|---|
| CPU | Cortex-M85，1 GHz |
| CPU 计算扩展 | DSP、FPU、Helium MVE |
| NPU | Ethos-U55-256，500 MHz |
| NPU INT8 理论峰值 | 256 GOPS |
| 外部 SDRAM | 32 MiB，16-bit |
| 相机接口 | OV5640，MIPI-CSI / VIN |
| JPEG 编码 | 软件 |
| USB | HS，480 Mb/s |

规格来源：[Renesas RA8P1](https://www.renesas.com/en/products/ra8p1) 

**OpenMV固件:5.0.1**    
**OpenMV IDE:5.1.1**  

## CPU 跑分

| 平台 | CPU 主频 | 单核 CoreMark | 数据来源 |
|---|---:|---:|---|
| RA8P1 Cortex-M85 | 1 GHz | 6362 | [EEMBC 提交结果](https://www.eembc.org/viewer/?benchmark_seq=13714) |

## 图像处理耗时

分辨率：**QVGA（320×240）**。单位：**ms / 次，平均值，越小越快**。

RA8P1：CPU 1 GHz，I/D cache 开启；每项运行 10 次，3 轮各采样 100 次。工作图均使用SRAM，计时不含采集、输入恢复和预览。

| 测试项目 | 格式 / 参数 | RA8P1 OMV |
|---|---|---:|
| 反色 `invert()` | 灰度 | 0.213 |
| 帧差 `difference()` | 灰度 | 0.886 |
| 腐蚀 / 膨胀 | 灰度，3×3 | 2.439 / 2.440 |
| 均值滤波 `mean()` | 灰度，3×3 | 3.820 |
| 卷积 `morph()` | 灰度，3×3 | 2.931 |
| JPEG 编码 | 灰度，quality=90 | 5.215 |
| JPEG 编码 | RGB565，quality=90 | 15.480 |
| 灰度转换 `to_grayscale()` | RGB565 | 2.642 |

RA8P1 使用本地采集图；帧差参考为输入的反色图，卷积使用 3×3 锐化核。


## 神经网络示例模型推理耗时

使用示例默认模型，固定输入，预热 5 次、3 轮各采样 30 次；计时包含输入复制、推理和输出回调，不含相机采集、图像预处理、检测后处理与绘图。

测试的模型为[models](/py_script/models/)下提供的模型

目前固件的张量分配方法为，如果sram存的下，则测sram性能，存不下，则测sdram。即每个选项为目前固件的最优性能。因此会出现某个模型性能下降很多的情况。目前SRAM分配给张量的空间为1408KB(0x00160000)，以下模型均在SRAM中。  

| 模型 | 输入 | RA8P1 OMV / ms|
|---|---|---:|
| FOMO 人脸中心检测 | 96×96 | 1.937 |
| BlazeFace（FLOAT32 接口） | 128×128 | 11.521 |
| BlazeFace（Renesas INT8） | 128×128 | 6.268 |
| BlazePalm 手掌检测 | 192×192 | 40.767 |
| Face Landmarks 468 点 | 192×192 | 19.829 |
| Hand Landmarks 21 点 | 224×224 | 17.961 |
| MoveNet 单人姿态 | 192×192 | 24.219 |
| YOLO LC 行人检测 | 192×192 | 5.001 |
| YOLOv8n person（示例 Vela 4） | 192×192 | 22.840 |
| YOLOv8n COCO80 | 192×192 | 30.404 |
| YOLOv8n Pose | 192×192 |31.166 |
| YOLOv5n6-xiao VOC20 | 192×192 | 14.127 |
| YOLO-Fastest 灰度人脸 | 192×192 | 6.448 |
| Swift-YOLO Gesture | 192×192 | 9.964 |
| Swift-YOLO Pet | 192×192 | 9.571 |
| Swift-YOLO Nano Person | 192×192 | 13.638 |
| U-Net MobileNetV2 分割 | 128×128 | 47.345 |
| VWW 人体存在分类 | 96×96 | 1.681 |
| ResNet8 CIFAR10 | 32×32 | 1.939 |
| MobileNet V1 0.25 | 224×224 | 6.184 |
