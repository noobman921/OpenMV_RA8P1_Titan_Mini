/* SPDX-License-Identifier: Apache-2.0 */
/* board 模块 — 对应 Titan_Mini_Openmv/platform/openmv/board_module.c。
 * 只保留本端口有对应实现的成员: LED / usb_msc 依赖 Titan 的 led.h 与
 * titan_storage, 这里没有等价物, 未包含。 */
#include "py/runtime.h"
#include "py/mphal.h"
#include "framebuffer.h"
#include "ra8_capture.h"

// (SystemCoreClock, I-cache, D-cache, __ARM_FEATURE_MVE, MICROPY_HW_BOARD_NAME)
static mp_obj_t board_info(void) {
    mp_obj_t tuple[] = {
        mp_obj_new_int_from_uint(SystemCoreClock),
        mp_obj_new_bool((SCB->CCR & SCB_CCR_IC_Msk) != 0),
        mp_obj_new_bool((SCB->CCR & SCB_CCR_DC_Msk) != 0),
        mp_obj_new_int(__ARM_FEATURE_MVE),
        mp_obj_new_str(MICROPY_HW_BOARD_NAME, sizeof(MICROPY_HW_BOARD_NAME) - 1)
    };
    return mp_obj_new_tuple(5, tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_0(board_info_obj, board_info);

static mp_obj_t board_preview(size_t n_args, const mp_obj_t *args) {
    framebuffer_t *fb = framebuffer_get(FB_STREAM_ID);
    bool previous = fb->enabled;
    if (n_args) { framebuffer_set_enabled(fb, mp_obj_is_true(args[0])); }
    return mp_obj_new_bool(previous);
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(board_preview_obj, 0, 1, board_preview);

static mp_obj_t board_pipeline(size_t n_args, const mp_obj_t *args) {
    return mp_obj_new_bool(ra8_capture_pipeline(n_args ? mp_obj_is_true(args[0]) : -1));
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(board_pipeline_obj, 0, 1, board_pipeline);

static const mp_rom_map_elem_t board_dict[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_board) },
    { MP_ROM_QSTR(MP_QSTR_info),     MP_ROM_PTR(&board_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_preview),  MP_ROM_PTR(&board_preview_obj) },
    { MP_ROM_QSTR(MP_QSTR_pipeline), MP_ROM_PTR(&board_pipeline_obj) },
};
static MP_DEFINE_CONST_DICT(board_globals, board_dict);
const mp_obj_module_t board_module = {{&mp_type_module}, (mp_obj_dict_t *)&board_globals};
MP_REGISTER_MODULE(MP_QSTR_board, board_module);
