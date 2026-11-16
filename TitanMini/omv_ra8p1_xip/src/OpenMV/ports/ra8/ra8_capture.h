#ifndef RA8_CAPTURE_H
#define RA8_CAPTURE_H
#include <stdbool.h>

// 采集流水线开关。enable < 0 只查询。关闭时 ra_csi_snapshot 返回前 abort,
// VIN 不在后台连续采集, 下次 snapshot 会重新启动。
// (对应 Titan_Mini_Openmv/platform/include/ra8_capture.h 的 ra8_capture_pipeline)
bool ra8_capture_pipeline(int enable);

#endif
