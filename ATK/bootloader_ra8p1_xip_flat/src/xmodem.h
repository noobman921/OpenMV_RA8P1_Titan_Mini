#ifndef XMODEM_H
#define XMODEM_H

#include <stdbool.h>
#include <stdint.h>

/* 通过 USB CDC 接收 XMODEM-128 镜像并写入外部 OSPI。
 * 收到完整镜像（EOT）并全部写入成功返回 true，传输出错返回 false。 */
bool xmodem_download(uint32_t flash_address);

#endif /* XMODEM_H */
