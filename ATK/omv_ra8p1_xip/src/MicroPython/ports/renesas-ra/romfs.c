/*
 * romfs.c
 *
 *  Created on: 2026年8月19日
 *      Author: qq292
 */

#if MICROPY_VFS_ROM

#include "py/mperrno.h"
#include "py/objarray.h"
#include "extmod/vfs.h"

// MRAM 地址是编译期常量，直接用宏，不需要 linker symbol
static const MP_DEFINE_MEMORYVIEW_OBJ(romfs_obj, 'B', 0,
    MICROPY_HW_ROMFS_PART0_LENGTH,
    (void *)MICROPY_HW_ROMFS_PART0_ORIGIN);

mp_obj_t mp_vfs_rom_ioctl(size_t n_args, const mp_obj_t *args) {
    switch (mp_obj_get_int(args[0])) {
        case MP_VFS_ROM_IOCTL_GET_NUMBER_OF_SEGMENTS:
            return MP_OBJ_NEW_SMALL_INT(1);

        case MP_VFS_ROM_IOCTL_GET_SEGMENT:
            return MP_OBJ_FROM_PTR(&romfs_obj);

        default:
            return MP_OBJ_NEW_SMALL_INT(-MP_EINVAL);
    }
}

#endif // MICROPY_VFS_ROM
