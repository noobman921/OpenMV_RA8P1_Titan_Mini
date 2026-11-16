/*
 * RA8P1 time-module extension (have a look at how MicroPython ports do this).
 *
 * This file is NOT compiled on its own. It is textually included by
 * extmod/modtime.c through MICROPY_PY_TIME_INCLUDEFILE, exactly the mechanism
 * MicroPython's own ports use (ports/esp32, ports/rp2, ports/samd,
 * ports/renesas-ra, ...). Keeping the extension here means extmod/modtime.c
 * stays byte-identical to upstream and the whole patch is visible in the port.
 *
 * Adds time.clock(), implemented in src/OpenMV/modules/py_clock.c.
 *
 * MICROPY_PY_TIME_EXTRA_GLOBALS is consumed by the mp_module_time_globals_table
 * near the end of extmod/modtime.c, i.e. after this include. That is why the
 * clock entry can be appended there without touching the shared file.
 */
#include "py/runtime.h"
#include "py_clock.h"

#define MICROPY_PY_TIME_EXTRA_GLOBALS \
    { MP_ROM_QSTR(MP_QSTR_clock), MP_ROM_PTR(&py_clock_type) },
