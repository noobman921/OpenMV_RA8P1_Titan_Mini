/*
 * Embedded Haar cascade data declarations.
 */
#ifndef __CASCADE_DATA_H__
#define __CASCADE_DATA_H__

#include <stdint.h>

// Frontal face detection cascade (24x24 window, 15 stages, ~20KB)
extern const uint8_t cascade_frontalface[];
extern const uint32_t cascade_frontalface_len;

#endif // __CASCADE_DATA_H__
