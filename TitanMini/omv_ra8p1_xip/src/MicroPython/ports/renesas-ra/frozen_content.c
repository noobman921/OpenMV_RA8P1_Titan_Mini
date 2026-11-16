//
// Content for MICROPY_MODULE_FROZEN_STR
//
#include <stdint.h>
#define MP_FROZEN_STR_NAMES \

const uint32_t mp_frozen_str_sizes[] = { 0 };
const char mp_frozen_str_content[] = {
"\0"
};

//
// Content for MICROPY_MODULE_FROZEN_MPY
//
#include "py/mpconfig.h"
#include "py/objint.h"
#include "py/objstr.h"
#include "py/emitglue.h"
#include "py/nativeglue.h"

#if MICROPY_LONGINT_IMPL != 2
#error "incompatible MICROPY_LONGINT_IMPL"
#endif

#if MPZ_DIG_SIZE != 16
#error "incompatible MPZ_DIG_SIZE"
#endif

#if MICROPY_PY_BUILTINS_FLOAT
typedef struct _mp_obj_float_t {
    mp_obj_base_t base;
    mp_float_t value;
} mp_obj_float_t;
#endif

#if MICROPY_PY_BUILTINS_COMPLEX
typedef struct _mp_obj_complex_t {
    mp_obj_base_t base;
    mp_float_t real;
    mp_float_t imag;
} mp_obj_complex_t;
#endif

enum {
    MP_QSTR__percent_s_space__percent__dot_2f = MP_QSTRnumber_of,
    MP_QSTR__dot_,
    MP_QSTR__dot_txt,
    MP_QSTR__slash_rom_slash_micro_speech_dot_tflite,
    MP_QSTR_BlazeFace,
    MP_QSTR_BlazePalm,
    MP_QSTR_Expected_space_an_space_image_space_input,
    MP_QSTR_Expected_space_batches_space_to_space_be_space_1,
    MP_QSTR_FaceLandmarks,
    MP_QSTR_Fomo,
    MP_QSTR_HandLandmarks,
    MP_QSTR_Invalid_space_ROI_space_dimensions_bang_,
    MP_QSTR_MicroSpeech,
    MP_QSTR_MoveNet,
    MP_QSTR_NMS,
    MP_QSTR_No,
    MP_QSTR_Normalization,
    MP_QSTR_Yes,
    MP_QSTR_YoloLC,
    MP_QSTR_YoloPro,
    MP_QSTR_YoloV2,
    MP_QSTR_YoloV5,
    MP_QSTR_YoloV8,
    MP_QSTR__image,
    MP_QSTR_add_bounding_box,
    MP_QSTR_anchor_grid,
    MP_QSTR_anchors,
    MP_QSTR_anchors_len,
    MP_QSTR_audio,
    MP_QSTR_audio_buffer,
    MP_QSTR_audio_callback,
    MP_QSTR_audio_started,
    MP_QSTR_box1,
    MP_QSTR_box2,
    MP_QSTR_boxes,
    MP_QSTR_buf,
    MP_QSTR_channels,
    MP_QSTR_colors,
    MP_QSTR_cords,
    MP_QSTR_cords_idx,
    MP_QSTR_dequantize,
    MP_QSTR_detection_post_process,
    MP_QSTR_draw_predictions,
    MP_QSTR_draw_skeleton,
    MP_QSTR_f,
    MP_QSTR_find_max,
    MP_QSTR_find_max_axis,
    MP_QSTR_font_height,
    MP_QSTR_font_width,
    MP_QSTR_frequency,
    MP_QSTR_get_bounding_boxes,
    MP_QSTR_grayscale,
    MP_QSTR_h_scale,
    MP_QSTR_ih,
    MP_QSTR_inputs,
    MP_QSTR_iou,
    MP_QSTR_iw,
    MP_QSTR_keypoints,
    MP_QSTR_kp_color,
    MP_QSTR_kp_fill,
    MP_QSTR_kp_radius,
    MP_QSTR_kp_thickness,
    MP_QSTR_label_index,
    MP_QSTR_labels,
    MP_QSTR_line_color,
    MP_QSTR_line_thickness,
    MP_QSTR_lines,
    MP_QSTR_listen,
    MP_QSTR_logit,
    MP_QSTR_mediapipe_detection_postprocess,
    MP_QSTR_micro_speech,
    MP_QSTR_ml_dot_preprocessing,
    MP_QSTR_ml_dot_utils,
    MP_QSTR_ml_slash___init___dot_py,
    MP_QSTR_ml_slash_apps_dot_py,
    MP_QSTR_ml_slash_model_dot_py,
    MP_QSTR_ml_slash_postprocessing_slash_darknet_dot_py,
    MP_QSTR_ml_slash_postprocessing_slash_edgeimpulse_dot_py,
    MP_QSTR_ml_slash_postprocessing_slash_mediapipe_dot_py,
    MP_QSTR_ml_slash_postprocessing_slash_ultralytics_dot_py,
    MP_QSTR_ml_slash_preprocessing_dot_py,
    MP_QSTR_ml_slash_utils_dot_py,
    MP_QSTR_model,
    MP_QSTR_nms,
    MP_QSTR_nms_sigma,
    MP_QSTR_nms_threshold,
    MP_QSTR_np,
    MP_QSTR_outputs,
    MP_QSTR_pascal_voc,
    MP_QSTR_point,
    MP_QSTR_pred_history,
    MP_QSTR_preprocessor,
    MP_QSTR_quantize,
    MP_QSTR_radius,
    MP_QSTR_samples,
    MP_QSTR_score,
    MP_QSTR_score_idx,
    MP_QSTR_scores,
    MP_QSTR_sigma,
    MP_QSTR_sigmoid,
    MP_QSTR_softmax,
    MP_QSTR_start_audio_streaming,
    MP_QSTR_start_streaming,
    MP_QSTR_stop_audio_streaming,
    MP_QSTR_stop_streaming,
    MP_QSTR_t,
    MP_QSTR_text_color,
    MP_QSTR_timeout,
    MP_QSTR_uml,
    MP_QSTR_w_scale,
    MP_QSTR_window_h,
    MP_QSTR_window_w,
    MP_QSTR_xmax,
    MP_QSTR_xmin,
    MP_QSTR_ymax,
    MP_QSTR_ymin,
};

const qstr_hash_t mp_qstr_frozen_const_hashes[] = {
    55692,
    46475,
    31379,
    21552,
    36916,
    1093,
    17221,
    64924,
    38053,
    44942,
    27207,
    56130,
    26711,
    6891,
    54165,
    29476,
    7640,
    6218,
    59423,
    31389,
    58804,
    58803,
    58814,
    4829,
    2691,
    12603,
    21839,
    45175,
    4691,
    59916,
    805,
    38189,
    15745,
    15746,
    63494,
    18804,
    46485,
    57771,
    21484,
    58214,
    22029,
    22809,
    40188,
    7899,
    46531,
    43307,
    27095,
    19446,
    7599,
    52897,
    19234,
    6928,
    41066,
    28452,
    43168,
    21494,
    28475,
    30413,
    40060,
    61102,
    14553,
    33207,
    50402,
    53200,
    23433,
    2754,
    46680,
    3788,
    34908,
    81,
    22824,
    60294,
    40765,
    37718,
    59518,
    17891,
    18483,
    7976,
    7864,
    21462,
    56224,
    10171,
    50314,
    22645,
    21435,
    20445,
    28507,
    40105,
    51948,
    6633,
    31899,
    44948,
    5388,
    4093,
    52240,
    30765,
    5927,
    32190,
    41492,
    35831,
    48095,
    54927,
    33606,
    28567,
    62046,
    46545,
    3034,
    21566,
    26417,
    30261,
    54238,
    54209,
    489,
    759,
    32072,
    32342,
};

const qstr_len_t mp_qstr_frozen_const_lengths[] = {
    7,
    1,
    4,
    24,
    9,
    9,
    23,
    24,
    13,
    4,
    13,
    23,
    11,
    7,
    3,
    2,
    13,
    3,
    6,
    7,
    6,
    6,
    6,
    6,
    16,
    11,
    7,
    11,
    5,
    12,
    14,
    13,
    4,
    4,
    5,
    3,
    8,
    6,
    5,
    9,
    10,
    22,
    16,
    13,
    1,
    8,
    13,
    11,
    10,
    9,
    18,
    9,
    7,
    2,
    6,
    3,
    2,
    9,
    8,
    7,
    9,
    12,
    11,
    6,
    10,
    14,
    5,
    6,
    5,
    31,
    12,
    16,
    8,
    14,
    10,
    11,
    28,
    32,
    30,
    32,
    19,
    11,
    5,
    3,
    9,
    13,
    2,
    7,
    10,
    5,
    12,
    12,
    8,
    6,
    7,
    5,
    9,
    6,
    5,
    7,
    7,
    21,
    15,
    20,
    14,
    1,
    10,
    7,
    3,
    7,
    8,
    8,
    4,
    4,
    4,
    4,
};

extern const qstr_pool_t mp_qstr_const_pool;
const qstr_pool_t mp_qstr_frozen_const_pool = {
    &mp_qstr_const_pool, // previous pool
    MP_QSTRnumber_of, // previous pool size
    true, // is_sorted
    10, // allocated entries
    116, // used entries
    (qstr_hash_t *)mp_qstr_frozen_const_hashes,
    (qstr_len_t *)mp_qstr_frozen_const_lengths,
    {
        "%s %.2f",
        ".",
        ".txt",
        "/rom/micro_speech.tflite",
        "BlazeFace",
        "BlazePalm",
        "Expected an image input",
        "Expected batches to be 1",
        "FaceLandmarks",
        "Fomo",
        "HandLandmarks",
        "Invalid ROI dimensions!",
        "MicroSpeech",
        "MoveNet",
        "NMS",
        "No",
        "Normalization",
        "Yes",
        "YoloLC",
        "YoloPro",
        "YoloV2",
        "YoloV5",
        "YoloV8",
        "_image",
        "add_bounding_box",
        "anchor_grid",
        "anchors",
        "anchors_len",
        "audio",
        "audio_buffer",
        "audio_callback",
        "audio_started",
        "box1",
        "box2",
        "boxes",
        "buf",
        "channels",
        "colors",
        "cords",
        "cords_idx",
        "dequantize",
        "detection_post_process",
        "draw_predictions",
        "draw_skeleton",
        "f",
        "find_max",
        "find_max_axis",
        "font_height",
        "font_width",
        "frequency",
        "get_bounding_boxes",
        "grayscale",
        "h_scale",
        "ih",
        "inputs",
        "iou",
        "iw",
        "keypoints",
        "kp_color",
        "kp_fill",
        "kp_radius",
        "kp_thickness",
        "label_index",
        "labels",
        "line_color",
        "line_thickness",
        "lines",
        "listen",
        "logit",
        "mediapipe_detection_postprocess",
        "micro_speech",
        "ml.preprocessing",
        "ml.utils",
        "ml/__init__.py",
        "ml/apps.py",
        "ml/model.py",
        "ml/postprocessing/darknet.py",
        "ml/postprocessing/edgeimpulse.py",
        "ml/postprocessing/mediapipe.py",
        "ml/postprocessing/ultralytics.py",
        "ml/preprocessing.py",
        "ml/utils.py",
        "model",
        "nms",
        "nms_sigma",
        "nms_threshold",
        "np",
        "outputs",
        "pascal_voc",
        "point",
        "pred_history",
        "preprocessor",
        "quantize",
        "radius",
        "samples",
        "score",
        "score_idx",
        "scores",
        "sigma",
        "sigmoid",
        "softmax",
        "start_audio_streaming",
        "start_streaming",
        "stop_audio_streaming",
        "stop_streaming",
        "t",
        "text_color",
        "timeout",
        "uml",
        "w_scale",
        "window_h",
        "window_w",
        "xmax",
        "xmin",
        "ymax",
        "ymin",
    },
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_apps
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\apps.mpy
// - frozen file name: ml/apps.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/apps.py, scope ml_apps__lt_module_gt_
static const byte fun_data_ml_apps__lt_module_gt_[90] = {
    0x34,0x16, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x26,0x2c,0x2c,0x4c,0x22,0x29,0x2a,0x63, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'time'
    0x16,0x02, // STORE_NAME 'time'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x03, // LOAD_CONST_STRING 'Model'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x04, // IMPORT_NAME 'ml'
    0x1c,0x03, // IMPORT_FROM 'Model'
    0x16,0x03, // STORE_NAME 'Model'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x05, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x06, // IMPORT_NAME 'micropython'
    0x1c,0x05, // IMPORT_FROM 'const'
    0x16,0x05, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x07, // LOAD_CONST_STRING 'numpy'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x08, // IMPORT_NAME 'ulab'
    0x1c,0x07, // IMPORT_FROM 'numpy'
    0x16,0x2c, // STORE_NAME 'np'
    0x59, // POP_TOP
    0x48,0x08, // SETUP_EXCEPT 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x09, // IMPORT_NAME 'audio'
    0x16,0x09, // STORE_NAME 'audio'
    0x4a,0x0e, // POP_EXCEPT_JUMP 14
    0x57, // DUP_TOP
    0x11,0x2d, // LOAD_NAME 'ImportError'
    0x11,0x2e, // LOAD_NAME 'AttributeError'
    0x2a,0x02, // BUILD_TUPLE 2
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x59, // POP_TOP
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x0a, // LOAD_CONST_STRING 'MicroSpeech'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0a, // STORE_NAME 'MicroSpeech'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_apps__lt_module_gt_
// frozen bytecode for file ml/apps.py, scope ml_apps_MicroSpeech
static const byte fun_data_ml_apps_MicroSpeech[64] = {
    0x20,0x1a, // prelude
    0x0a, // names: MicroSpeech
    0x88,0x28,0x80,0x08,0x8a,0x0f,0x84,0x0d,0x84,0x07,0x64,0x20, // code info
    0x11,0x2f, // LOAD_NAME '__name__'
    0x16,0x30, // STORE_NAME '__module__'
    0x10,0x0a, // LOAD_CONST_STRING 'MicroSpeech'
    0x16,0x31, // STORE_NAME '__qualname__'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0d, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x1e, // STORE_NAME 'audio_callback'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x23, // STORE_NAME 'start_audio_streaming'
    0x32,0x03, // MAKE_FUNCTION 3
    0x16,0x25, // STORE_NAME 'stop_audio_streaming'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x10,0x0b, // LOAD_CONST_STRING 'Yes'
    0x10,0x0c, // LOAD_CONST_STRING 'No'
    0x2b,0x02, // BUILD_LIST 2
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x27, // STORE_NAME 'listen'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_apps_MicroSpeech
// frozen bytecode for file ml/apps.py, scope ml_apps_MicroSpeech___init__
static const byte fun_data_ml_apps_MicroSpeech___init__[159] = {
    0xf8,0x85,0x41,0x24, // prelude
    0x0d,0x32,0x0e,0x10,0x0f, // names: __init__, self, preprocessor, micro_speech, labels
    0x80,0x31,0x24,0x25,0x29,0x2c,0x25,0x29,0x48,0x32,0x32,0x32,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0e, // STORE_ATTR 'preprocessor'
    0xb1, // LOAD_FAST 1
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x03, // LOAD_GLOBAL 'Model'
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x0e, // STORE_ATTR 'preprocessor'
    0xb3, // LOAD_FAST 3
    0xb2, // LOAD_FAST 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'labels'
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'micro_speech'
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x12,0x03, // LOAD_GLOBAL 'Model'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'micro_speech'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'micro_speech'
    0x13,0x0f, // LOAD_ATTR 'labels'
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'labels'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x11, // LOAD_METHOD 'zeros'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x10,0x12, // LOAD_CONST_STRING 'dtype'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x13,0x13, // LOAD_ATTR 'int16'
    0x36,0x82,0x01, // CALL_METHOD 257
    0xb0, // LOAD_FAST 0
    0x18,0x14, // STORE_ATTR 'audio_buffer'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x11, // LOAD_METHOD 'zeros'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x10,0x12, // LOAD_CONST_STRING 'dtype'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x13,0x15, // LOAD_ATTR 'int8'
    0x36,0x82,0x01, // CALL_METHOD 257
    0xb0, // LOAD_FAST 0
    0x18,0x16, // STORE_ATTR 'spectrogram'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x11, // LOAD_METHOD 'zeros'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x10,0x12, // LOAD_CONST_STRING 'dtype'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x13,0x17, // LOAD_ATTR 'float'
    0x36,0x82,0x01, // CALL_METHOD 257
    0xb0, // LOAD_FAST 0
    0x18,0x18, // STORE_ATTR 'pred_history'
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x19, // STORE_ATTR 'audio_started'
    0x12,0x09, // LOAD_GLOBAL 'audio'
    0x14,0x1a, // LOAD_METHOD 'init'
    0x10,0x1b, // LOAD_CONST_STRING 'channels'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x1c, // LOAD_CONST_STRING 'frequency'
    0x22,0x80,0xfd,0x00, // LOAD_CONST_SMALL_INT 16000
    0x10,0x1d, // LOAD_CONST_STRING 'samples'
    0x22,0x82,0x40, // LOAD_CONST_SMALL_INT 320
    0x53, // LOAD_NULL
    0xb4, // LOAD_FAST 4
    0x80, // LOAD_CONST_SMALL_INT 0
    0x37,0x88,0x00, // CALL_METHOD_VAR_KW 1024
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_apps_MicroSpeech___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_apps_MicroSpeech___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 159,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 16,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_ml_apps_MicroSpeech___init__ + 9,
        .line_info_top = fun_data_ml_apps_MicroSpeech___init__ + 22,
        .opcodes = fun_data_ml_apps_MicroSpeech___init__ + 22,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_apps_MicroSpeech___init__ fun_data_ml_apps_MicroSpeech___init__[0]
#endif

// child of ml_apps_MicroSpeech
// frozen bytecode for file ml/apps.py, scope ml_apps_MicroSpeech_audio_callback
static const byte fun_data_ml_apps_MicroSpeech_audio_callback[138] = {
    0x3a,0x14, // prelude
    0x1e,0x32,0x33, // names: audio_callback, self, buf
    0x80,0x41,0x33,0x7b,0x32,0x78,0x31, // code info
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x1f, // LOAD_METHOD 'roll'
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'audio_buffer'
    0x22,0xfd,0x40, // LOAD_CONST_SMALL_INT -320
    0x10,0x20, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x02, // CALL_METHOD 258
    0xb0, // LOAD_FAST 0
    0x18,0x14, // STORE_ATTR 'audio_buffer'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x21, // LOAD_METHOD 'frombuffer'
    0xb1, // LOAD_FAST 1
    0x10,0x12, // LOAD_CONST_STRING 'dtype'
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x13,0x13, // LOAD_ATTR 'int16'
    0x36,0x82,0x01, // CALL_METHOD 257
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'audio_buffer'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x22,0x81,0x20, // LOAD_CONST_SMALL_INT 160
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x1f, // LOAD_METHOD 'roll'
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'spectrogram'
    0x22,0x58, // LOAD_CONST_SMALL_INT -40
    0x10,0x20, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x02, // CALL_METHOD 258
    0xb0, // LOAD_FAST 0
    0x18,0x16, // STORE_ATTR 'spectrogram'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR 'preprocessor'
    0x14,0x22, // LOAD_METHOD 'predict'
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'audio_buffer'
    0x2b,0x01, // BUILD_LIST 1
    0x36,0x01, // CALL_METHOD 1
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'spectrogram'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x22,0x58, // LOAD_CONST_SMALL_INT -40
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x1f, // LOAD_METHOD 'roll'
    0xb0, // LOAD_FAST 0
    0x13,0x18, // LOAD_ATTR 'pred_history'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x10,0x20, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0xb0, // LOAD_FAST 0
    0x18,0x18, // STORE_ATTR 'pred_history'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'micro_speech'
    0x14,0x22, // LOAD_METHOD 'predict'
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'spectrogram'
    0x2b,0x01, // BUILD_LIST 1
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x18, // LOAD_ATTR 'pred_history'
    0x7f, // LOAD_CONST_SMALL_INT -1
    0x56, // STORE_SUBSCR
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_apps_MicroSpeech_audio_callback = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_apps_MicroSpeech_audio_callback,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 138,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 30,
        .line_info = fun_data_ml_apps_MicroSpeech_audio_callback + 5,
        .line_info_top = fun_data_ml_apps_MicroSpeech_audio_callback + 12,
        .opcodes = fun_data_ml_apps_MicroSpeech_audio_callback + 12,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_apps_MicroSpeech_audio_callback fun_data_ml_apps_MicroSpeech_audio_callback[0]
#endif

// child of ml_apps_MicroSpeech
// frozen bytecode for file ml/apps.py, scope ml_apps_MicroSpeech_start_audio_streaming
static const byte fun_data_ml_apps_MicroSpeech_start_audio_streaming[51] = {
    0x21,0x10, // prelude
    0x23,0x32, // names: start_audio_streaming, self
    0x80,0x4d,0x27,0x29,0x29,0x2a, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x19, // LOAD_ATTR 'audio_started'
    0x50, // LOAD_CONST_FALSE
    0xde, // BINARY_OP 7 <is>
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'spectrogram'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x56, // STORE_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x13,0x18, // LOAD_ATTR 'pred_history'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x56, // STORE_SUBSCR
    0x12,0x09, // LOAD_GLOBAL 'audio'
    0x14,0x24, // LOAD_METHOD 'start_streaming'
    0xb0, // LOAD_FAST 0
    0x13,0x1e, // LOAD_ATTR 'audio_callback'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x52, // LOAD_CONST_TRUE
    0xb0, // LOAD_FAST 0
    0x18,0x19, // STORE_ATTR 'audio_started'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_apps_MicroSpeech_start_audio_streaming = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_apps_MicroSpeech_start_audio_streaming,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 51,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_ml_apps_MicroSpeech_start_audio_streaming + 4,
        .line_info_top = fun_data_ml_apps_MicroSpeech_start_audio_streaming + 10,
        .opcodes = fun_data_ml_apps_MicroSpeech_start_audio_streaming + 10,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_apps_MicroSpeech_start_audio_streaming fun_data_ml_apps_MicroSpeech_start_audio_streaming[0]
#endif

// child of ml_apps_MicroSpeech
// frozen bytecode for file ml/apps.py, scope ml_apps_MicroSpeech_stop_audio_streaming
static const byte fun_data_ml_apps_MicroSpeech_stop_audio_streaming[20] = {
    0x11,0x0a, // prelude
    0x25,0x32, // names: stop_audio_streaming, self
    0x80,0x54,0x27, // code info
    0x12,0x09, // LOAD_GLOBAL 'audio'
    0x14,0x26, // LOAD_METHOD 'stop_streaming'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x50, // LOAD_CONST_FALSE
    0xb0, // LOAD_FAST 0
    0x18,0x19, // STORE_ATTR 'audio_started'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_apps_MicroSpeech_stop_audio_streaming = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_apps_MicroSpeech_stop_audio_streaming,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 37,
        .line_info = fun_data_ml_apps_MicroSpeech_stop_audio_streaming + 4,
        .line_info_top = fun_data_ml_apps_MicroSpeech_stop_audio_streaming + 7,
        .opcodes = fun_data_ml_apps_MicroSpeech_stop_audio_streaming + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_apps_MicroSpeech_stop_audio_streaming fun_data_ml_apps_MicroSpeech_stop_audio_streaming[0]
#endif

// child of ml_apps_MicroSpeech
// frozen bytecode for file ml/apps.py, scope ml_apps_MicroSpeech_listen
static const byte fun_data_ml_apps_MicroSpeech_listen[184] = {
    0xf1,0x84,0x80,0x01,0x38, // prelude
    0x27,0x32,0x34,0x35,0x36,0x37, // names: listen, self, timeout, callback, threshold, filter
    0x80,0x58,0x26,0x27,0x20,0x2e,0x28,0x24,0x26,0x2a,0x29,0x29,0x25,0x25,0x26,0x25,0x26,0x25,0x25,0x31,0x26,0x25, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x23, // LOAD_METHOD 'start_audio_streaming'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x12,0x02, // LOAD_GLOBAL 'time'
    0x14,0x28, // LOAD_METHOD 'ticks_ms'
    0x36,0x00, // CALL_METHOD 0
    0xc5, // STORE_FAST 5
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x29, // LOAD_METHOD 'mean'
    0xb0, // LOAD_FAST 0
    0x13,0x18, // LOAD_ATTR 'pred_history'
    0x10,0x20, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x01, // CALL_METHOD 257
    0xc6, // STORE_FAST 6
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x2a, // LOAD_METHOD 'argmax'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0xc7, // STORE_FAST 7
    0xb6, // LOAD_FAST 6
    0xb7, // LOAD_FAST 7
    0x55, // LOAD_SUBSCR
    0xc8, // STORE_FAST 8
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'labels'
    0xb7, // LOAD_FAST 7
    0x55, // LOAD_SUBSCR
    0xc9, // STORE_FAST 9
    0xb8, // LOAD_FAST 8
    0xb3, // LOAD_FAST 3
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x72, // POP_JUMP_IF_FALSE 50
    0xb9, // LOAD_FAST 9
    0xb4, // LOAD_FAST 4
    0xdd, // BINARY_OP 6 <in>
    0x44,0x6d, // POP_JUMP_IF_FALSE 45
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x13,0x18, // LOAD_ATTR 'pred_history'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x56, // STORE_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb0, // LOAD_FAST 0
    0x13,0x16, // LOAD_ATTR 'spectrogram'
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x56, // STORE_SUBSCR
    0xb2, // LOAD_FAST 2
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x50, // POP_JUMP_IF_FALSE 16
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0xb0, // LOAD_FAST 0
    0x14,0x25, // LOAD_METHOD 'stop_audio_streaming'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0xb9, // LOAD_FAST 9
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
    0xb2, // LOAD_FAST 2
    0xb9, // LOAD_FAST 9
    0xb6, // LOAD_FAST 6
    0x34,0x02, // CALL_FUNCTION 2
    0x59, // POP_TOP
    0xb1, // LOAD_FAST 1
    0x7f, // LOAD_CONST_SMALL_INT -1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x45, // POP_JUMP_IF_FALSE 5
    0x51, // LOAD_CONST_NONE
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x57, // POP_JUMP_IF_FALSE 23
    0x12,0x02, // LOAD_GLOBAL 'time'
    0x14,0x28, // LOAD_METHOD 'ticks_ms'
    0x36,0x00, // CALL_METHOD 0
    0xb5, // LOAD_FAST 5
    0xf3, // BINARY_OP 28 __sub__
    0xb1, // LOAD_FAST 1
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x4b, // POP_JUMP_IF_FALSE 11
    0xb0, // LOAD_FAST 0
    0x14,0x25, // LOAD_METHOD 'stop_audio_streaming'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0x63, // RETURN_VALUE
    0x12,0x02, // LOAD_GLOBAL 'time'
    0x14,0x2b, // LOAD_METHOD 'sleep_ms'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x42,0xf8,0x7e, // JUMP -136
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_apps_MicroSpeech_listen = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_apps_MicroSpeech_listen,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 184,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 39,
        .line_info = fun_data_ml_apps_MicroSpeech_listen + 11,
        .line_info_top = fun_data_ml_apps_MicroSpeech_listen + 33,
        .opcodes = fun_data_ml_apps_MicroSpeech_listen + 33,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_apps_MicroSpeech_listen fun_data_ml_apps_MicroSpeech_listen[0]
#endif

static const mp_raw_code_t *const children_ml_apps_MicroSpeech[] = {
    (const mp_raw_code_t *)&proto_fun_ml_apps_MicroSpeech___init__,
    (const mp_raw_code_t *)&proto_fun_ml_apps_MicroSpeech_audio_callback,
    (const mp_raw_code_t *)&proto_fun_ml_apps_MicroSpeech_start_audio_streaming,
    (const mp_raw_code_t *)&proto_fun_ml_apps_MicroSpeech_stop_audio_streaming,
    (const mp_raw_code_t *)&proto_fun_ml_apps_MicroSpeech_listen,
};

static const mp_raw_code_truncated_t proto_fun_ml_apps_MicroSpeech = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_apps_MicroSpeech,
    .children = (void *)&children_ml_apps_MicroSpeech,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 64,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_apps_MicroSpeech + 3,
        .line_info_top = fun_data_ml_apps_MicroSpeech + 15,
        .opcodes = fun_data_ml_apps_MicroSpeech + 15,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_apps__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_apps_MicroSpeech,
};

static const mp_raw_code_truncated_t proto_fun_ml_apps__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_apps__lt_module_gt_,
    .children = (void *)&children_ml_apps__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 90,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_apps__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_apps__lt_module_gt_ + 13,
        .opcodes = fun_data_ml_apps__lt_module_gt_ + 13,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_apps[56] = {
    MP_QSTR_ml_slash_apps_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_time,
    MP_QSTR_Model,
    MP_QSTR_ml,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_numpy,
    MP_QSTR_ulab,
    MP_QSTR_audio,
    MP_QSTR_MicroSpeech,
    MP_QSTR_Yes,
    MP_QSTR_No,
    MP_QSTR___init__,
    MP_QSTR_preprocessor,
    MP_QSTR_labels,
    MP_QSTR_micro_speech,
    MP_QSTR_zeros,
    MP_QSTR_dtype,
    MP_QSTR_int16,
    MP_QSTR_audio_buffer,
    MP_QSTR_int8,
    MP_QSTR_spectrogram,
    MP_QSTR_float,
    MP_QSTR_pred_history,
    MP_QSTR_audio_started,
    MP_QSTR_init,
    MP_QSTR_channels,
    MP_QSTR_frequency,
    MP_QSTR_samples,
    MP_QSTR_audio_callback,
    MP_QSTR_roll,
    MP_QSTR_axis,
    MP_QSTR_frombuffer,
    MP_QSTR_predict,
    MP_QSTR_start_audio_streaming,
    MP_QSTR_start_streaming,
    MP_QSTR_stop_audio_streaming,
    MP_QSTR_stop_streaming,
    MP_QSTR_listen,
    MP_QSTR_ticks_ms,
    MP_QSTR_mean,
    MP_QSTR_argmax,
    MP_QSTR_sleep_ms,
    MP_QSTR_np,
    MP_QSTR_ImportError,
    MP_QSTR_AttributeError,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_buf,
    MP_QSTR_timeout,
    MP_QSTR_callback,
    MP_QSTR_threshold,
    MP_QSTR_filter,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_apps_0 = {{&mp_type_float}, (mp_float_t)0.65};
#define const_obj_ml_apps_0_macro MP_ROM_PTR(&const_obj_ml_apps_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_apps_0_macro ((mp_rom_obj_t)(0xbfa66666))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_apps_0_macro ((mp_rom_obj_t)(0xbfe8cccccccccccd))
#endif
static const mp_obj_str_t const_obj_ml_apps_1 = {{&mp_type_str}, 44101, 30, (const byte*)"\x2f\x72\x6f\x6d\x2f\x61\x75\x64\x69\x6f\x5f\x70\x72\x65\x70\x72\x6f\x63\x65\x73\x73\x6f\x72\x2e\x74\x66\x6c\x69\x74\x65"};
static const mp_rom_obj_tuple_t const_obj_ml_apps_3 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(1),
    MP_ROM_INT(480),
}};
static const mp_rom_obj_tuple_t const_obj_ml_apps_4 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(1),
    MP_ROM_INT(1960),
}};
static const mp_rom_obj_tuple_t const_obj_ml_apps_5 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(34),
    MP_ROM_INT(4),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_apps[6] = {
    const_obj_ml_apps_0_macro,
    MP_ROM_PTR(&const_obj_ml_apps_1),
    MP_ROM_QSTR(MP_QSTR__slash_rom_slash_micro_speech_dot_tflite),
    MP_ROM_PTR(&const_obj_ml_apps_3),
    MP_ROM_PTR(&const_obj_ml_apps_4),
    MP_ROM_PTR(&const_obj_ml_apps_5),
};

static const mp_frozen_module_t frozen_module_ml_apps = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_apps,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_apps,
    },
    .proto_fun = &proto_fun_ml_apps__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_model
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\model.mpy
// - frozen file name: ml/model.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/model.py, scope ml_model__lt_module_gt_
static const byte fun_data_ml_model__lt_module_gt_[47] = {
    0x18,0x0c, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x26,0x26,0x6c, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'uml'
    0x16,0x02, // STORE_NAME 'uml'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'image'
    0x16,0x03, // STORE_NAME 'image'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'Normalization'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'ml.preprocessing'
    0x1c,0x04, // IMPORT_FROM 'Normalization'
    0x16,0x04, // STORE_NAME 'Normalization'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x06, // LOAD_CONST_STRING 'Model'
    0x11,0x02, // LOAD_NAME 'uml'
    0x13,0x06, // LOAD_ATTR 'Model'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x06, // STORE_NAME 'Model'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_model__lt_module_gt_
// frozen bytecode for file ml/model.py, scope ml_model_Model
static const byte fun_data_ml_model_Model[30] = {
    0x08,0x0b, // prelude
    0x06, // names: Model
    0x88,0x22,0x86,0x08,0x00, // code info
    0x11,0x13, // LOAD_NAME '__name__'
    0x16,0x14, // STORE_NAME '__module__'
    0x10,0x06, // LOAD_CONST_STRING 'Model'
    0x16,0x15, // STORE_NAME '__qualname__'
    0xb0, // LOAD_FAST 0
    0x20,0x00,0x01, // MAKE_CLOSURE 0
    0x16,0x07, // STORE_NAME '__init__'
    0xb0, // LOAD_FAST 0
    0x20,0x01,0x01, // MAKE_CLOSURE 1
    0x16,0x0e, // STORE_NAME 'predict'
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
// child of ml_model_Model
// frozen bytecode for file ml/model.py, scope ml_model_Model___init__
static const byte fun_data_ml_model_Model___init__[83] = {
    0xde,0x80,0xc0,0x40,0x12, // prelude
    0x07,0x16,0x17, // names: __init__, *, self
    0x80,0x23,0x34,0x22,0x2f,0x57, // code info
    0x12,0x18, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x07, // LOAD_SUPER_METHOD '__init__'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x14,0x08, // LOAD_METHOD 'get'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x51, // LOAD_CONST_NONE
    0x36,0x02, // CALL_METHOD 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x37,0x02, // CALL_METHOD_VAR_KW 2
    0x59, // POP_TOP
    0x48,0x1f, // SETUP_EXCEPT 31
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x14,0x09, // LOAD_METHOD 'split'
    0x10,0x0a, // LOAD_CONST_STRING '.'
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x10,0x0b, // LOAD_CONST_STRING '.txt'
    0xf2, // BINARY_OP 27 __add__
    0xc4, // STORE_FAST 4
    0x32,0x00, // MAKE_FUNCTION 0
    0x12,0x19, // LOAD_GLOBAL 'open'
    0xb4, // LOAD_FAST 4
    0x10,0x0c, // LOAD_CONST_STRING 'r'
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x01, // CALL_FUNCTION 1
    0xb1, // LOAD_FAST 1
    0x18,0x0d, // STORE_ATTR 'labels'
    0x4a,0x0e, // POP_EXCEPT_JUMP 14
    0x57, // DUP_TOP
    0x12,0x1a, // LOAD_GLOBAL 'Exception'
    0xdf, // BINARY_OP 8 <exception match>
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0xb1, // LOAD_FAST 1
    0x18,0x0d, // STORE_ATTR 'labels'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_model_Model___init__
// frozen bytecode for file ml/model.py, scope ml_model_Model___init____lt_listcomp_gt_
static const byte fun_data_ml_model_Model___init____lt_listcomp_gt_[25] = {
    0x49,0x08, // prelude
    0x0f,0x16, // names: <listcomp>, *
    0x80,0x26, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x0c, // FOR_ITER 12
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x14,0x10, // LOAD_METHOD 'rstrip'
    0x10,0x11, // LOAD_CONST_STRING '\n'
    0x36,0x01, // CALL_METHOD 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x32, // JUMP -14
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_model_Model___init____lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_model_Model___init____lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 25,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_ml_model_Model___init____lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_ml_model_Model___init____lt_listcomp_gt_ + 6,
        .opcodes = fun_data_ml_model_Model___init____lt_listcomp_gt_ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_model_Model___init____lt_listcomp_gt_ fun_data_ml_model_Model___init____lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_ml_model_Model___init__[] = {
    (const mp_raw_code_t *)&proto_fun_ml_model_Model___init____lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_ml_model_Model___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_model_Model___init__,
    .children = (void *)&children_ml_model_Model___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 83,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 1,
        .scope_flags = 6,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_ml_model_Model___init__ + 8,
        .line_info_top = fun_data_ml_model_Model___init__ + 14,
        .opcodes = fun_data_ml_model_Model___init__ + 14,
    },
    #endif
    #endif
};

// child of ml_model_Model
// frozen bytecode for file ml/model.py, scope ml_model_Model_predict
static const byte fun_data_ml_model_Model_predict[32] = {
    0xcb,0x80,0x40,0x0e, // prelude
    0x0e,0x16,0x17,0x1b, // names: predict, *, self, args
    0x80,0x2b,0x26, // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc2, // STORE_FAST 2
    0x12,0x18, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x0e, // LOAD_SUPER_METHOD 'predict'
    0xb2, // LOAD_FAST 2
    0x53, // LOAD_NULL
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x37,0x82,0x01, // CALL_METHOD_VAR_KW 257
    0x63, // RETURN_VALUE
};
// child of ml_model_Model_predict
// frozen bytecode for file ml/model.py, scope ml_model_Model_predict__lt_listcomp_gt_
static const byte fun_data_ml_model_Model_predict__lt_listcomp_gt_[39] = {
    0x49,0x08, // prelude
    0x0f,0x16, // names: <listcomp>, *
    0x80,0x2b, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x1a, // FOR_ITER 26
    0xc1, // STORE_FAST 1
    0x12,0x1c, // LOAD_GLOBAL 'isinstance'
    0xb1, // LOAD_FAST 1
    0x12,0x03, // LOAD_GLOBAL 'image'
    0x13,0x12, // LOAD_ATTR 'Image'
    0x34,0x02, // CALL_FUNCTION 2
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x12,0x04, // LOAD_GLOBAL 'Normalization'
    0x34,0x00, // CALL_FUNCTION 0
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x42,0x41, // JUMP 1
    0xb1, // LOAD_FAST 1
    0x2f,0x14, // STORE_COMP 20
    0x42,0x24, // JUMP -28
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_model_Model_predict__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_model_Model_predict__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 39,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 15,
        .line_info = fun_data_ml_model_Model_predict__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_ml_model_Model_predict__lt_listcomp_gt_ + 6,
        .opcodes = fun_data_ml_model_Model_predict__lt_listcomp_gt_ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_model_Model_predict__lt_listcomp_gt_ fun_data_ml_model_Model_predict__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_ml_model_Model_predict[] = {
    (const mp_raw_code_t *)&proto_fun_ml_model_Model_predict__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_ml_model_Model_predict = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_model_Model_predict,
    .children = (void *)&children_ml_model_Model_predict,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 32,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 2,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_model_Model_predict + 8,
        .line_info_top = fun_data_ml_model_Model_predict + 11,
        .opcodes = fun_data_ml_model_Model_predict + 11,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_model_Model[] = {
    (const mp_raw_code_t *)&proto_fun_ml_model_Model___init__,
    (const mp_raw_code_t *)&proto_fun_ml_model_Model_predict,
};

static const mp_raw_code_truncated_t proto_fun_ml_model_Model = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_model_Model,
    .children = (void *)&children_ml_model_Model,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 30,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_ml_model_Model + 3,
        .line_info_top = fun_data_ml_model_Model + 7,
        .opcodes = fun_data_ml_model_Model + 8,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_model__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_model_Model,
};

static const mp_raw_code_truncated_t proto_fun_ml_model__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_model__lt_module_gt_,
    .children = (void *)&children_ml_model__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 47,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_model__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_model__lt_module_gt_ + 8,
        .opcodes = fun_data_ml_model__lt_module_gt_ + 8,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_model[29] = {
    MP_QSTR_ml_slash_model_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_uml,
    MP_QSTR_image,
    MP_QSTR_Normalization,
    MP_QSTR_ml_dot_preprocessing,
    MP_QSTR_Model,
    MP_QSTR___init__,
    MP_QSTR_get,
    MP_QSTR_split,
    MP_QSTR__dot_,
    MP_QSTR__dot_txt,
    MP_QSTR_r,
    MP_QSTR_labels,
    MP_QSTR_predict,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR_rstrip,
    MP_QSTR__0x0a_,
    MP_QSTR_Image,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR__star_,
    MP_QSTR_self,
    MP_QSTR_super,
    MP_QSTR_open,
    MP_QSTR_Exception,
    MP_QSTR_args,
    MP_QSTR_isinstance,
};

// constants

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_model[1] = {
    MP_ROM_QSTR(MP_QSTR_postprocess),
};

static const mp_frozen_module_t frozen_module_ml_model = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_model,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_model,
    },
    .proto_fun = &proto_fun_ml_model__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_preprocessing
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\preprocessing.mpy
// - frozen file name: ml/preprocessing.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/preprocessing.py, scope ml_preprocessing__lt_module_gt_
static const byte fun_data_ml_preprocessing__lt_module_gt_[36] = {
    0x10,0x0a, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x26,0x6c, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'image'
    0x16,0x02, // STORE_NAME 'image'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x03, // LOAD_CONST_STRING 'numpy'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x04, // IMPORT_NAME 'ulab'
    0x1c,0x03, // IMPORT_FROM 'numpy'
    0x16,0x1d, // STORE_NAME 'np'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x05, // LOAD_CONST_STRING 'Normalization'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'Normalization'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_preprocessing__lt_module_gt_
// frozen bytecode for file ml/preprocessing.py, scope ml_preprocessing_Normalization
static const byte fun_data_ml_preprocessing_Normalization[38] = {
    0x18,0x10, // prelude
    0x05, // names: Normalization
    0x88,0x21,0x40,0x22,0x22,0x8a,0x09, // code info
    0x11,0x1e, // LOAD_NAME '__name__'
    0x16,0x1f, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'Normalization'
    0x16,0x20, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x51, // LOAD_CONST_NONE
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x06, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0c, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_preprocessing_Normalization
// frozen bytecode for file ml/preprocessing.py, scope ml_preprocessing_Normalization___init__
static const byte fun_data_ml_preprocessing_Normalization___init__[39] = {
    0xb1,0x84,0x80,0x01,0x18, // prelude
    0x06,0x21,0x07,0x08,0x09,0x0a, // names: __init__, self, scale, mean, stdev, roi
    0x80,0x28,0x24,0x24,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x07, // STORE_ATTR 'scale'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x08, // STORE_ATTR 'mean'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x09, // STORE_ATTR 'stdev'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x0a, // STORE_ATTR 'roi'
    0x51, // LOAD_CONST_NONE
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR '_image'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_preprocessing_Normalization___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_preprocessing_Normalization___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 39,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 6,
        .line_info = fun_data_ml_preprocessing_Normalization___init__ + 11,
        .line_info_top = fun_data_ml_preprocessing_Normalization___init__ + 17,
        .opcodes = fun_data_ml_preprocessing_Normalization___init__ + 17,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_preprocessing_Normalization___init__ fun_data_ml_preprocessing_Normalization___init__[0]
#endif

// child of ml_preprocessing_Normalization
// frozen bytecode for file ml/preprocessing.py, scope ml_preprocessing_Normalization___call__
static const byte fun_data_ml_preprocessing_Normalization___call__[410] = {
    0xd1,0x90,0x80,0x40,0x4a, // prelude
    0x0c,0x21, // names: __call__, self
    0x80,0x2f,0x2a,0x24,0x2b,0x27,0x27,0x31,0x31,0x24,0x42,0x66,0x29,0x27,0x27,0x25,0x27,0x2a,0x67,0x30,0x34,0x77,0x34,0x74,0x6b,0x2b,0x2f,0x47,0x64,0x25,0x34,0x4c,0x38,0x6c,0x24, // code info
    0x12,0x22, // LOAD_GLOBAL 'len'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xc5,0x80, // POP_JUMP_IF_FALSE 69
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc2, // STORE_FAST 2
    0x12,0x23, // LOAD_GLOBAL 'isinstance'
    0xb2, // LOAD_FAST 2
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x13,0x0d, // LOAD_ATTR 'Image'
    0x34,0x02, // CALL_FUNCTION 2
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0x12,0x24, // LOAD_GLOBAL 'ValueError'
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'roi'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb2, // LOAD_FAST 2
    0x14,0x0e, // LOAD_METHOD 'width'
    0x36,0x00, // CALL_METHOD 0
    0xb2, // LOAD_FAST 2
    0x14,0x0f, // LOAD_METHOD 'height'
    0x36,0x00, // CALL_METHOD 0
    0x2a,0x04, // BUILD_TUPLE 4
    0xb0, // LOAD_FAST 0
    0x18,0x0a, // STORE_ATTR 'roi'
    0x12,0x05, // LOAD_GLOBAL 'Normalization'
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'scale'
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'mean'
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'stdev'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'roi'
    0x34,0x04, // CALL_FUNCTION 4
    0xc3, // STORE_FAST 3
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0x18,0x0b, // STORE_ATTR '_image'
    0xb3, // LOAD_FAST 3
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0x12,0x22, // LOAD_GLOBAL 'len'
    0xb5, // LOAD_FAST 5
    0x34,0x01, // CALL_FUNCTION 1
    0x84, // LOAD_CONST_SMALL_INT 4
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x24, // LOAD_GLOBAL 'ValueError'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xb5, // LOAD_FAST 5
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc7, // STORE_FAST 7
    0xc8, // STORE_FAST 8
    0xc9, // STORE_FAST 9
    0xca, // STORE_FAST 10
    0xb7, // LOAD_FAST 7
    0x81, // LOAD_CONST_SMALL_INT 1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x24, // LOAD_GLOBAL 'ValueError'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xba, // LOAD_FAST 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0xba, // LOAD_FAST 10
    0x83, // LOAD_CONST_SMALL_INT 3
    0xdc, // BINARY_OP 5 __ne__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x24, // LOAD_GLOBAL 'ValueError'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0xba, // LOAD_FAST 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x13,0x10, // LOAD_ATTR 'GRAYSCALE'
    0x42,0x44, // JUMP 4
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x13,0x11, // LOAD_ATTR 'RGB565'
    0xcb, // STORE_FAST 11
    0x12,0x22, // LOAD_GLOBAL 'len'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0xb9, // LOAD_FAST 9
    0xb8, // LOAD_FAST 8
    0xf4, // BINARY_OP 29 __mul__
    0xba, // LOAD_FAST 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x42,0x41, // JUMP 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf4, // BINARY_OP 29 __mul__
    0xf3, // BINARY_OP 28 __sub__
    0xcc, // STORE_FAST 12
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x14,0x0d, // LOAD_METHOD 'Image'
    0xb9, // LOAD_FAST 9
    0xb8, // LOAD_FAST 8
    0xbb, // LOAD_FAST 11
    0x10,0x12, // LOAD_CONST_STRING 'buffer'
    0x12,0x25, // LOAD_GLOBAL 'memoryview'
    0xb4, // LOAD_FAST 4
    0x34,0x01, // CALL_FUNCTION 1
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x82,0x03, // CALL_METHOD 259
    0xc2, // STORE_FAST 2
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x13,0x13, // LOAD_ATTR 'BILINEAR'
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x13,0x14, // LOAD_ATTR 'CENTER'
    0xed, // BINARY_OP 22 __or__
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x13,0x15, // LOAD_ATTR 'SCALE_ASPECT_IGNORE'
    0xed, // BINARY_OP 22 __or__
    0x12,0x02, // LOAD_GLOBAL 'image'
    0x13,0x16, // LOAD_ATTR 'BLACK_BACKGROUND'
    0xed, // BINARY_OP 22 __or__
    0xcd, // STORE_FAST 13
    0xb2, // LOAD_FAST 2
    0x14,0x17, // LOAD_METHOD 'draw_image'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR '_image'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x0a, // LOAD_CONST_STRING 'roi'
    0xb0, // LOAD_FAST 0
    0x13,0x0a, // LOAD_ATTR 'roi'
    0x10,0x18, // LOAD_CONST_STRING 'hint'
    0xbd, // LOAD_FAST 13
    0x36,0x84,0x03, // CALL_METHOD 515
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x14,0x19, // LOAD_METHOD 'to_ndarray'
    0xb6, // LOAD_FAST 6
    0x10,0x12, // LOAD_CONST_STRING 'buffer'
    0xb4, // LOAD_FAST 4
    0x36,0x82,0x01, // CALL_METHOD 257
    0xce, // STORE_FAST 14
    0xb6, // LOAD_FAST 6
    0x12,0x26, // LOAD_GLOBAL 'ord'
    0x10,0x1a, // LOAD_CONST_STRING 'f'
    0x34,0x01, // CALL_FUNCTION 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0xec,0x80, // POP_JUMP_IF_FALSE 108
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'scale'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'scale'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf3, // BINARY_OP 28 __sub__
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xf7, // BINARY_OP 32 __truediv__
    0xcf, // STORE_FAST 15
    0xb0, // LOAD_FAST 0
    0x13,0x07, // LOAD_ATTR 'scale'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x26,0x10, // STORE_FAST_N 16
    0x32,0x00, // MAKE_FUNCTION 0
    0x26,0x11, // STORE_FAST_N 17
    0xba, // LOAD_FAST 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x11, // LOAD_FAST_N 17
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'mean'
    0x34,0x01, // CALL_FUNCTION 1
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x11, // LOAD_FAST_N 17
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'stdev'
    0x34,0x01, // CALL_FUNCTION 1
    0xf7, // BINARY_OP 32 __truediv__
    0x26,0x10, // STORE_FAST_N 16
    0xbf, // LOAD_FAST 15
    0x24,0x11, // LOAD_FAST_N 17
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'stdev'
    0x34,0x01, // CALL_FUNCTION 1
    0xf7, // BINARY_OP 32 __truediv__
    0xcf, // STORE_FAST 15
    0x42,0x64, // JUMP 36
    0x24,0x10, // LOAD_FAST_N 16
    0x12,0x1d, // LOAD_GLOBAL 'np'
    0x14,0x1b, // LOAD_METHOD 'array'
    0xb0, // LOAD_FAST 0
    0x13,0x08, // LOAD_ATTR 'mean'
    0x36,0x01, // CALL_METHOD 1
    0xf3, // BINARY_OP 28 __sub__
    0x12,0x1d, // LOAD_GLOBAL 'np'
    0x14,0x1b, // LOAD_METHOD 'array'
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'stdev'
    0x36,0x01, // CALL_METHOD 1
    0xf7, // BINARY_OP 32 __truediv__
    0x26,0x10, // STORE_FAST_N 16
    0xbf, // LOAD_FAST 15
    0x12,0x1d, // LOAD_GLOBAL 'np'
    0x14,0x1b, // LOAD_METHOD 'array'
    0xb0, // LOAD_FAST 0
    0x13,0x09, // LOAD_ATTR 'stdev'
    0x36,0x01, // CALL_METHOD 1
    0xf7, // BINARY_OP 32 __truediv__
    0xcf, // STORE_FAST 15
    0xbe, // LOAD_FAST 14
    0xbf, // LOAD_FAST 15
    0xe7, // BINARY_OP 16 __imul__
    0xce, // STORE_FAST 14
    0xbe, // LOAD_FAST 14
    0x24,0x10, // LOAD_FAST_N 16
    0xe5, // BINARY_OP 14 __iadd__
    0xce, // STORE_FAST 14
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_preprocessing_Normalization___call__
// frozen bytecode for file ml/preprocessing.py, scope ml_preprocessing_Normalization___call___grayscale
static const byte fun_data_ml_preprocessing_Normalization___call___grayscale[27] = {
    0x19,0x08, // prelude
    0x1c,0x27, // names: grayscale, x
    0x80,0x56, // code info
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x23,0x08, // LOAD_CONST_OBJ 8
    0xf4, // BINARY_OP 29 __mul__
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x23,0x09, // LOAD_CONST_OBJ 9
    0xf4, // BINARY_OP 29 __mul__
    0xf2, // BINARY_OP 27 __add__
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0xf4, // BINARY_OP 29 __mul__
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_preprocessing_Normalization___call___grayscale = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_preprocessing_Normalization___call___grayscale,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 28,
        .line_info = fun_data_ml_preprocessing_Normalization___call___grayscale + 4,
        .line_info_top = fun_data_ml_preprocessing_Normalization___call___grayscale + 6,
        .opcodes = fun_data_ml_preprocessing_Normalization___call___grayscale + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_preprocessing_Normalization___call___grayscale fun_data_ml_preprocessing_Normalization___call___grayscale[0]
#endif

static const mp_raw_code_t *const children_ml_preprocessing_Normalization___call__[] = {
    (const mp_raw_code_t *)&proto_fun_ml_preprocessing_Normalization___call___grayscale,
};

static const mp_raw_code_truncated_t proto_fun_ml_preprocessing_Normalization___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_preprocessing_Normalization___call__,
    .children = (void *)&children_ml_preprocessing_Normalization___call__,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 410,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 27,
        .n_exc_stack = 0,
        .scope_flags = 4,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_ml_preprocessing_Normalization___call__ + 7,
        .line_info_top = fun_data_ml_preprocessing_Normalization___call__ + 42,
        .opcodes = fun_data_ml_preprocessing_Normalization___call__ + 42,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_preprocessing_Normalization[] = {
    (const mp_raw_code_t *)&proto_fun_ml_preprocessing_Normalization___init__,
    (const mp_raw_code_t *)&proto_fun_ml_preprocessing_Normalization___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_preprocessing_Normalization = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_preprocessing_Normalization,
    .children = (void *)&children_ml_preprocessing_Normalization,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 38,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_ml_preprocessing_Normalization + 3,
        .line_info_top = fun_data_ml_preprocessing_Normalization + 10,
        .opcodes = fun_data_ml_preprocessing_Normalization + 10,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_preprocessing__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_preprocessing_Normalization,
};

static const mp_raw_code_truncated_t proto_fun_ml_preprocessing__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_preprocessing__lt_module_gt_,
    .children = (void *)&children_ml_preprocessing__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 36,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_preprocessing__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_preprocessing__lt_module_gt_ + 7,
        .opcodes = fun_data_ml_preprocessing__lt_module_gt_ + 7,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_preprocessing[40] = {
    MP_QSTR_ml_slash_preprocessing_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_image,
    MP_QSTR_numpy,
    MP_QSTR_ulab,
    MP_QSTR_Normalization,
    MP_QSTR___init__,
    MP_QSTR_scale,
    MP_QSTR_mean,
    MP_QSTR_stdev,
    MP_QSTR_roi,
    MP_QSTR__image,
    MP_QSTR___call__,
    MP_QSTR_Image,
    MP_QSTR_width,
    MP_QSTR_height,
    MP_QSTR_GRAYSCALE,
    MP_QSTR_RGB565,
    MP_QSTR_buffer,
    MP_QSTR_BILINEAR,
    MP_QSTR_CENTER,
    MP_QSTR_SCALE_ASPECT_IGNORE,
    MP_QSTR_BLACK_BACKGROUND,
    MP_QSTR_draw_image,
    MP_QSTR_hint,
    MP_QSTR_to_ndarray,
    MP_QSTR_f,
    MP_QSTR_array,
    MP_QSTR_grayscale,
    MP_QSTR_np,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_len,
    MP_QSTR_isinstance,
    MP_QSTR_ValueError,
    MP_QSTR_memoryview,
    MP_QSTR_ord,
    MP_QSTR_x,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_0_0 = {{&mp_type_float}, (mp_float_t)0};
#define const_obj_ml_preprocessing_0_0_macro MP_ROM_PTR(&const_obj_ml_preprocessing_0_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_0_0_macro ((mp_rom_obj_t)(0x80800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_0_0_macro ((mp_rom_obj_t)(0x8004000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_0_1 = {{&mp_type_float}, (mp_float_t)1};
#define const_obj_ml_preprocessing_0_1_macro MP_ROM_PTR(&const_obj_ml_preprocessing_0_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_0_1_macro ((mp_rom_obj_t)(0xc0000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_0_1_macro ((mp_rom_obj_t)(0xbff4000000000000))
#endif
static const mp_rom_obj_tuple_t const_obj_ml_preprocessing_0 = {{&mp_type_tuple}, 2, {
    const_obj_ml_preprocessing_0_0_macro,
    const_obj_ml_preprocessing_0_1_macro,
}};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_1_0 = {{&mp_type_float}, (mp_float_t)0};
#define const_obj_ml_preprocessing_1_0_macro MP_ROM_PTR(&const_obj_ml_preprocessing_1_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_1_0_macro ((mp_rom_obj_t)(0x80800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_1_0_macro ((mp_rom_obj_t)(0x8004000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_1_1 = {{&mp_type_float}, (mp_float_t)0};
#define const_obj_ml_preprocessing_1_1_macro MP_ROM_PTR(&const_obj_ml_preprocessing_1_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_1_1_macro ((mp_rom_obj_t)(0x80800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_1_1_macro ((mp_rom_obj_t)(0x8004000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_1_2 = {{&mp_type_float}, (mp_float_t)0};
#define const_obj_ml_preprocessing_1_2_macro MP_ROM_PTR(&const_obj_ml_preprocessing_1_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_1_2_macro ((mp_rom_obj_t)(0x80800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_1_2_macro ((mp_rom_obj_t)(0x8004000000000000))
#endif
static const mp_rom_obj_tuple_t const_obj_ml_preprocessing_1 = {{&mp_type_tuple}, 3, {
    const_obj_ml_preprocessing_1_0_macro,
    const_obj_ml_preprocessing_1_1_macro,
    const_obj_ml_preprocessing_1_2_macro,
}};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_2_0 = {{&mp_type_float}, (mp_float_t)1};
#define const_obj_ml_preprocessing_2_0_macro MP_ROM_PTR(&const_obj_ml_preprocessing_2_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_2_0_macro ((mp_rom_obj_t)(0xc0000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_2_0_macro ((mp_rom_obj_t)(0xbff4000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_2_1 = {{&mp_type_float}, (mp_float_t)1};
#define const_obj_ml_preprocessing_2_1_macro MP_ROM_PTR(&const_obj_ml_preprocessing_2_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_2_1_macro ((mp_rom_obj_t)(0xc0000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_2_1_macro ((mp_rom_obj_t)(0xbff4000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_2_2 = {{&mp_type_float}, (mp_float_t)1};
#define const_obj_ml_preprocessing_2_2_macro MP_ROM_PTR(&const_obj_ml_preprocessing_2_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_2_2_macro ((mp_rom_obj_t)(0xc0000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_2_2_macro ((mp_rom_obj_t)(0xbff4000000000000))
#endif
static const mp_rom_obj_tuple_t const_obj_ml_preprocessing_2 = {{&mp_type_tuple}, 3, {
    const_obj_ml_preprocessing_2_0_macro,
    const_obj_ml_preprocessing_2_1_macro,
    const_obj_ml_preprocessing_2_2_macro,
}};
static const mp_obj_str_t const_obj_ml_preprocessing_4 = {{&mp_type_str}, 52555, 46, (const byte*)"\x45\x78\x70\x65\x63\x74\x65\x64\x20\x69\x6e\x70\x75\x74\x20\x74\x65\x6e\x73\x6f\x72\x20\x77\x69\x74\x68\x20\x73\x68\x61\x70\x65\x3a\x20\x28\x31\x2c\x20\x48\x2c\x20\x57\x2c\x20\x43\x29"};
static const mp_obj_str_t const_obj_ml_preprocessing_6 = {{&mp_type_str}, 31336, 30, (const byte*)"\x45\x78\x70\x65\x63\x74\x65\x64\x20\x63\x68\x61\x6e\x6e\x65\x6c\x73\x20\x74\x6f\x20\x62\x65\x20\x31\x20\x6f\x72\x20\x33"};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_7 = {{&mp_type_float}, (mp_float_t)255};
#define const_obj_ml_preprocessing_7_macro MP_ROM_PTR(&const_obj_ml_preprocessing_7)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_7_macro ((mp_rom_obj_t)(0xc3ff0002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_7_macro ((mp_rom_obj_t)(0xc073e00000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_8 = {{&mp_type_float}, (mp_float_t)0.299};
#define const_obj_ml_preprocessing_8_macro MP_ROM_PTR(&const_obj_ml_preprocessing_8)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_8_macro ((mp_rom_obj_t)(0xbf191686))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_8_macro ((mp_rom_obj_t)(0xbfd722d0e5604189))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_9 = {{&mp_type_float}, (mp_float_t)0.587};
#define const_obj_ml_preprocessing_9_macro MP_ROM_PTR(&const_obj_ml_preprocessing_9)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_9_macro ((mp_rom_obj_t)(0xbf9645a2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_9_macro ((mp_rom_obj_t)(0xbfe6c8b439581062))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_preprocessing_10 = {{&mp_type_float}, (mp_float_t)0.114};
#define const_obj_ml_preprocessing_10_macro MP_ROM_PTR(&const_obj_ml_preprocessing_10)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_preprocessing_10_macro ((mp_rom_obj_t)(0xbe6978d6))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_preprocessing_10_macro ((mp_rom_obj_t)(0xbfc12f1a9fbe76c9))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_preprocessing[11] = {
    MP_ROM_PTR(&const_obj_ml_preprocessing_0),
    MP_ROM_PTR(&const_obj_ml_preprocessing_1),
    MP_ROM_PTR(&const_obj_ml_preprocessing_2),
    MP_ROM_QSTR(MP_QSTR_Expected_space_an_space_image_space_input),
    MP_ROM_PTR(&const_obj_ml_preprocessing_4),
    MP_ROM_QSTR(MP_QSTR_Expected_space_batches_space_to_space_be_space_1),
    MP_ROM_PTR(&const_obj_ml_preprocessing_6),
    const_obj_ml_preprocessing_7_macro,
    const_obj_ml_preprocessing_8_macro,
    const_obj_ml_preprocessing_9_macro,
    const_obj_ml_preprocessing_10_macro,
};

static const mp_frozen_module_t frozen_module_ml_preprocessing = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_preprocessing,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_preprocessing,
    },
    .proto_fun = &proto_fun_ml_preprocessing__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_utils
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\utils.mpy
// - frozen file name: ml/utils.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/utils.py, scope ml_utils__lt_module_gt_
static const byte fun_data_ml_utils__lt_module_gt_[133] = {
    0x28,0x38, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x26,0x6c,0x64,0x20,0x64,0x20,0x89,0x0b,0x68,0x60,0x68,0x60,0x89,0x60,0x85,0x09,0x89,0x33,0x61,0x20,0x8b,0x0d,0x61,0x40,0x64, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'math'
    0x16,0x02, // STORE_NAME 'math'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x03, // LOAD_CONST_STRING 'numpy'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x04, // IMPORT_NAME 'ulab'
    0x1c,0x03, // IMPORT_FROM 'numpy'
    0x16,0x36, // STORE_NAME 'np'
    0x59, // POP_TOP
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x07, // STORE_NAME 'logit'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x09, // STORE_NAME 'sigmoid'
    0x50, // LOAD_CONST_FALSE
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x02, // MAKE_FUNCTION_DEFARGS 2
    0x16,0x0b, // STORE_NAME 'threshold'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x03, // MAKE_FUNCTION_DEFARGS 3
    0x16,0x10, // STORE_NAME 'quantize'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x04, // MAKE_FUNCTION_DEFARGS 4
    0x16,0x15, // STORE_NAME 'dequantize'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x05, // MAKE_FUNCTION 5
    0x10,0x05, // LOAD_CONST_STRING 'NMS'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x05, // STORE_NAME 'NMS'
    0x51, // LOAD_CONST_NONE
    0x10,0x06, // LOAD_CONST_STRING 'pascal_voc'
    0x88, // LOAD_CONST_SMALL_INT 8
    0x8a, // LOAD_CONST_SMALL_INT 10
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x2a,0x05, // BUILD_TUPLE 5
    0x53, // LOAD_NULL
    0x33,0x06, // MAKE_FUNCTION_DEFARGS 6
    0x16,0x16, // STORE_NAME 'draw_predictions'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x50, // LOAD_CONST_FALSE
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0x33,0x07, // MAKE_FUNCTION_DEFARGS 7
    0x16,0x23, // STORE_NAME 'draw_keypoints'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x50, // LOAD_CONST_FALSE
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x06, // BUILD_TUPLE 6
    0x53, // LOAD_NULL
    0x33,0x08, // MAKE_FUNCTION_DEFARGS 8
    0x16,0x25, // STORE_NAME 'draw_skeleton'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_logit
static const byte fun_data_ml_utils_logit[19] = {
    0x29,0x08, // prelude
    0x07,0x37, // names: logit, x
    0x80,0x21, // code info
    0x12,0x36, // LOAD_GLOBAL 'np'
    0x14,0x08, // LOAD_METHOD 'log'
    0xb0, // LOAD_FAST 0
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xb0, // LOAD_FAST 0
    0xf3, // BINARY_OP 28 __sub__
    0xf7, // BINARY_OP 32 __truediv__
    0x36,0x01, // CALL_METHOD 1
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_logit = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_logit,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 19,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 7,
        .line_info = fun_data_ml_utils_logit + 4,
        .line_info_top = fun_data_ml_utils_logit + 6,
        .opcodes = fun_data_ml_utils_logit + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_logit fun_data_ml_utils_logit[0]
#endif

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_sigmoid
static const byte fun_data_ml_utils_sigmoid[21] = {
    0x29,0x08, // prelude
    0x09,0x37, // names: sigmoid, x
    0x80,0x25, // code info
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x12,0x36, // LOAD_GLOBAL 'np'
    0x14,0x0a, // LOAD_METHOD 'exp'
    0xb0, // LOAD_FAST 0
    0xd1, // UNARY_OP 1 __neg__
    0x36,0x01, // CALL_METHOD 1
    0xf2, // BINARY_OP 27 __add__
    0xf7, // BINARY_OP 32 __truediv__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_sigmoid = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_sigmoid,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 21,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ml_utils_sigmoid + 4,
        .line_info_top = fun_data_ml_utils_sigmoid + 6,
        .opcodes = fun_data_ml_utils_sigmoid + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_sigmoid fun_data_ml_utils_sigmoid[0]
#endif

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_threshold
static const byte fun_data_ml_utils_threshold[79] = {
    0xc9,0x84,0x01,0x1c, // prelude
    0x0b,0x38,0x0b,0x22,0x39,0x3a, // names: threshold, scores, threshold, scale, find_max, find_max_axis
    0x80,0x29,0x25,0x23,0x2c,0x4c,0x23,0x2c, // code info
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x5b, // POP_JUMP_IF_FALSE 27
    0xb3, // LOAD_FAST 3
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x36, // LOAD_GLOBAL 'np'
    0x14,0x0c, // LOAD_METHOD 'max'
    0xb0, // LOAD_FAST 0
    0x10,0x0d, // LOAD_CONST_STRING 'axis'
    0xb4, // LOAD_FAST 4
    0x36,0x82,0x01, // CALL_METHOD 257
    0xc0, // STORE_FAST 0
    0x12,0x36, // LOAD_GLOBAL 'np'
    0x14,0x0e, // LOAD_METHOD 'nonzero'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0xd8, // BINARY_OP 1 __gt__
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
    0xb3, // LOAD_FAST 3
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x12,0x36, // LOAD_GLOBAL 'np'
    0x14,0x0f, // LOAD_METHOD 'min'
    0xb0, // LOAD_FAST 0
    0x10,0x0d, // LOAD_CONST_STRING 'axis'
    0xb4, // LOAD_FAST 4
    0x36,0x82,0x01, // CALL_METHOD 257
    0xc0, // STORE_FAST 0
    0x12,0x36, // LOAD_GLOBAL 'np'
    0x14,0x0e, // LOAD_METHOD 'nonzero'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0xd7, // BINARY_OP 0 __lt__
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_threshold = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_threshold,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 79,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 10,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_ml_utils_threshold + 10,
        .line_info_top = fun_data_ml_utils_threshold + 18,
        .opcodes = fun_data_ml_utils_threshold + 18,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_threshold fun_data_ml_utils_threshold[0]
#endif

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_quantize
static const byte fun_data_ml_utils_quantize[37] = {
    0xab,0x01,0x10, // prelude
    0x10,0x3b,0x3c,0x3d, // names: quantize, model, value, index
    0x80,0x34,0x2a,0x22, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'output_dtype'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0x10,0x12, // LOAD_CONST_STRING 'f'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'output_scale'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0xf7, // BINARY_OP 32 __truediv__
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'output_zero_point'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_quantize = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_quantize,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 37,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_ml_utils_quantize + 7,
        .line_info_top = fun_data_ml_utils_quantize + 11,
        .opcodes = fun_data_ml_utils_quantize + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_quantize fun_data_ml_utils_quantize[0]
#endif

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_dequantize
static const byte fun_data_ml_utils_dequantize[41] = {
    0xb3,0x01,0x10, // prelude
    0x15,0x3b,0x3c,0x3d, // names: dequantize, model, value, index
    0x80,0x3a,0x2a,0x22, // code info
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'output_dtype'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0x10,0x12, // LOAD_CONST_STRING 'f'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x42, // POP_JUMP_IF_FALSE 2
    0xb1, // LOAD_FAST 1
    0x63, // RETURN_VALUE
    0xb1, // LOAD_FAST 1
    0x12,0x3e, // LOAD_GLOBAL 'float'
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'output_zero_point'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xf3, // BINARY_OP 28 __sub__
    0xb0, // LOAD_FAST 0
    0x13,0x13, // LOAD_ATTR 'output_scale'
    0xb2, // LOAD_FAST 2
    0x55, // LOAD_SUBSCR
    0xf4, // BINARY_OP 29 __mul__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_dequantize = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_dequantize,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 41,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 21,
        .line_info = fun_data_ml_utils_dequantize + 7,
        .line_info_top = fun_data_ml_utils_dequantize + 11,
        .opcodes = fun_data_ml_utils_dequantize + 11,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_dequantize fun_data_ml_utils_dequantize[0]
#endif

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS
static const byte fun_data_ml_utils_NMS[42] = {
    0x08,0x0e, // prelude
    0x05, // names: NMS
    0x88,0x40,0x84,0x0d,0x88,0x0b, // code info
    0x11,0x3f, // LOAD_NAME '__name__'
    0x16,0x40, // STORE_NAME '__module__'
    0x10,0x05, // LOAD_CONST_STRING 'NMS'
    0x16,0x41, // STORE_NAME '__qualname__'
    0x32,0x00, // MAKE_FUNCTION 0
    0x16,0x28, // STORE_NAME '__init__'
    0x51, // LOAD_CONST_NONE
    0x2a,0x01, // BUILD_TUPLE 1
    0x53, // LOAD_NULL
    0x33,0x01, // MAKE_FUNCTION_DEFARGS 1
    0x16,0x2d, // STORE_NAME 'add_bounding_box'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x2a,0x02, // BUILD_TUPLE 2
    0x53, // LOAD_NULL
    0x33,0x02, // MAKE_FUNCTION_DEFARGS 2
    0x16,0x2f, // STORE_NAME 'get_bounding_boxes'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_utils_NMS
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS___init__
static const byte fun_data_ml_utils_NMS___init__[55] = {
    0xa8,0x04,0x18, // prelude
    0x28,0x54,0x29,0x2a,0x2b, // names: __init__, self, window_w, window_h, roi
    0x80,0x46,0x24,0x24,0x24,0x2e,0x27, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x29, // STORE_ATTR 'window_w'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x2a, // STORE_ATTR 'window_h'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x2b, // STORE_ATTR 'roi'
    0xb3, // LOAD_FAST 3
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x47, // POP_JUMP_IF_TRUE 7
    0xb3, // LOAD_FAST 3
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x12,0x55, // LOAD_GLOBAL 'ValueError'
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x34,0x01, // CALL_FUNCTION 1
    0x65, // RAISE_OBJ
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x18,0x2c, // STORE_ATTR 'boxes'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 55,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 40,
        .line_info = fun_data_ml_utils_NMS___init__ + 8,
        .line_info_top = fun_data_ml_utils_NMS___init__ + 15,
        .opcodes = fun_data_ml_utils_NMS___init__ + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_NMS___init__ fun_data_ml_utils_NMS___init__[0]
#endif

// child of ml_utils_NMS
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS_add_bounding_box
static const byte fun_data_ml_utils_NMS_add_bounding_box[150] = {
    0x90,0x91,0x04,0x26, // prelude
    0x2d,0x54,0x56,0x57,0x58,0x59,0x5a,0x5b,0x4c, // names: add_bounding_box, self, xmin, ymin, xmax, ymax, score, label_index, keypoints
    0x80,0x4e,0x2e,0x2f,0x2f,0x2f,0x2f,0x28,0x28,0x2a, // code info
    0xb5, // LOAD_FAST 5
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xdb, // BINARY_OP 4 __ge__
    0x44,0xf6,0x80, // POP_JUMP_IF_FALSE 118
    0xb5, // LOAD_FAST 5
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xda, // BINARY_OP 3 __le__
    0x44,0xef,0x80, // POP_JUMP_IF_FALSE 111
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x29, // LOAD_ATTR 'window_w'
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc1, // STORE_FAST 1
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 'window_h'
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc2, // STORE_FAST 2
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x29, // LOAD_ATTR 'window_w'
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc3, // STORE_FAST 3
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 'window_h'
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xc4, // STORE_FAST 4
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb3, // LOAD_FAST 3
    0xb1, // LOAD_FAST 1
    0xf3, // BINARY_OP 28 __sub__
    0x34,0x01, // CALL_FUNCTION 1
    0xc8, // STORE_FAST 8
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb4, // LOAD_FAST 4
    0xb2, // LOAD_FAST 2
    0xf3, // BINARY_OP 28 __sub__
    0x34,0x01, // CALL_FUNCTION 1
    0xc9, // STORE_FAST 9
    0xb8, // LOAD_FAST 8
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x5e, // POP_JUMP_IF_FALSE 30
    0xb9, // LOAD_FAST 9
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x59, // POP_JUMP_IF_FALSE 25
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'boxes'
    0x14,0x2e, // LOAD_METHOD 'append'
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb2, // LOAD_FAST 2
    0x34,0x01, // CALL_FUNCTION 1
    0xb8, // LOAD_FAST 8
    0xb9, // LOAD_FAST 9
    0xb5, // LOAD_FAST 5
    0xb6, // LOAD_FAST 6
    0xb7, // LOAD_FAST 7
    0x2b,0x07, // BUILD_LIST 7
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS_add_bounding_box = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS_add_bounding_box,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 150,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 19,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 8,
        .n_kwonly_args = 0,
        .n_def_pos_args = 1,
        .qstr_block_name_idx = 45,
        .line_info = fun_data_ml_utils_NMS_add_bounding_box + 13,
        .line_info_top = fun_data_ml_utils_NMS_add_bounding_box + 23,
        .opcodes = fun_data_ml_utils_NMS_add_bounding_box + 23,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_NMS_add_bounding_box fun_data_ml_utils_NMS_add_bounding_box[0]
#endif

// child of ml_utils_NMS
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS_get_bounding_boxes
static const byte fun_data_ml_utils_NMS_get_bounding_boxes[545] = {
    0xcb,0x90,0x01,0x64, // prelude
    0x2f,0x54,0x0b,0x5c, // names: get_bounding_boxes, self, threshold, sigma
    0x80,0x59,0x30,0x4f,0x83,0x0d,0x22,0x23,0x42,0x23,0x27,0x27,0x69,0x20,0x2a,0x28,0x36,0x29,0x70,0x20,0x2c,0x27,0x79,0x20,0x2e,0x2e,0x34,0x54,0x2b,0x32,0x32,0x30,0x30,0x27,0x27,0x38,0x1f,0x63,0x20,0x4d,0x2b,0x31,0x27,0x27,0x29,0x5d, // code info
    0x12,0x5d, // LOAD_GLOBAL 'sorted'
    0xb0, // LOAD_FAST 0
    0x13,0x2c, // LOAD_ATTR 'boxes'
    0x10,0x30, // LOAD_CONST_STRING 'key'
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x31, // LOAD_CONST_STRING 'reverse'
    0x52, // LOAD_CONST_TRUE
    0x34,0x84,0x01, // CALL_FUNCTION 513
    0xc3, // STORE_FAST 3
    0xb2, // LOAD_FAST 2
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x46, // POP_JUMP_IF_FALSE 6
    0x23,0x07, // LOAD_CONST_OBJ 7
    0xb2, // LOAD_FAST 2
    0xf7, // BINARY_OP 32 __truediv__
    0x42,0x42, // JUMP 2
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xc4, // STORE_FAST 4
    0x32,0x01, // MAKE_FUNCTION 1
    0xc5, // STORE_FAST 5
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc6, // STORE_FAST 6
    0x2b,0x00, // BUILD_LIST 0
    0xc7, // STORE_FAST 7
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc8, // STORE_FAST 8
    0x42,0xfc,0x80, // JUMP 124
    0xb3, // LOAD_FAST 3
    0x14,0x32, // LOAD_METHOD 'pop'
    0xb6, // LOAD_FAST 6
    0x36,0x01, // CALL_METHOD 1
    0xc9, // STORE_FAST 9
    0xb7, // LOAD_FAST 7
    0x14,0x2e, // LOAD_METHOD 'append'
    0xb9, // LOAD_FAST 9
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0xb8, // LOAD_FAST 8
    0xb9, // LOAD_FAST 9
    0x85, // LOAD_CONST_SMALL_INT 5
    0x55, // LOAD_SUBSCR
    0x34,0x02, // CALL_FUNCTION 2
    0xc8, // STORE_FAST 8
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x72, // JUMP 50
    0x57, // DUP_TOP
    0xca, // STORE_FAST 10
    0xb5, // LOAD_FAST 5
    0xb9, // LOAD_FAST 9
    0xb3, // LOAD_FAST 3
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x34,0x02, // CALL_FUNCTION 2
    0xcb, // STORE_FAST 11
    0xb3, // LOAD_FAST 3
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x12,0x02, // LOAD_GLOBAL 'math'
    0x14,0x0a, // LOAD_METHOD 'exp'
    0xb4, // LOAD_FAST 4
    0xbb, // LOAD_FAST 11
    0xf4, // BINARY_OP 29 __mul__
    0xbb, // LOAD_FAST 11
    0xf4, // BINARY_OP 29 __mul__
    0x36,0x01, // CALL_METHOD 1
    0xf4, // BINARY_OP 29 __mul__
    0xb3, // LOAD_FAST 3
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x56, // STORE_SUBSCR
    0xb3, // LOAD_FAST 3
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x47, // POP_JUMP_IF_FALSE 7
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xb3, // LOAD_FAST 3
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x09, // POP_JUMP_IF_TRUE -55
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x12,0x5e, // LOAD_GLOBAL 'list'
    0x12,0x5f, // LOAD_GLOBAL 'filter'
    0x32,0x02, // MAKE_FUNCTION 2
    0xb3, // LOAD_FAST 3
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x01, // CALL_FUNCTION 1
    0xc3, // STORE_FAST 3
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x44,0x51, // POP_JUMP_IF_FALSE 17
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x12,0x49, // LOAD_GLOBAL 'enumerate'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x10,0x30, // LOAD_CONST_STRING 'key'
    0x32,0x03, // MAKE_FUNCTION 3
    0x34,0x82,0x01, // CALL_FUNCTION 257
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc6, // STORE_FAST 6
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb3, // LOAD_FAST 3
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0xfc,0x7e, // POP_JUMP_IF_TRUE -132
    0xb0, // LOAD_FAST 0
    0x13,0x2b, // LOAD_ATTR 'roi'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x12,0x3e, // LOAD_GLOBAL 'float'
    0xb0, // LOAD_FAST 0
    0x13,0x29, // LOAD_ATTR 'window_w'
    0x34,0x01, // CALL_FUNCTION 1
    0xf7, // BINARY_OP 32 __truediv__
    0xcc, // STORE_FAST 12
    0xb0, // LOAD_FAST 0
    0x13,0x2b, // LOAD_ATTR 'roi'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0x12,0x3e, // LOAD_GLOBAL 'float'
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 'window_h'
    0x34,0x01, // CALL_FUNCTION 1
    0xf7, // BINARY_OP 32 __truediv__
    0xcd, // STORE_FAST 13
    0xb0, // LOAD_FAST 0
    0x13,0x2b, // LOAD_ATTR 'roi'
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x29, // LOAD_ATTR 'window_w'
    0xbc, // LOAD_FAST 12
    0xf4, // BINARY_OP 29 __mul__
    0xf3, // BINARY_OP 28 __sub__
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf7, // BINARY_OP 32 __truediv__
    0xb0, // LOAD_FAST 0
    0x13,0x2b, // LOAD_ATTR 'roi'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xce, // STORE_FAST 14
    0xb0, // LOAD_FAST 0
    0x13,0x2b, // LOAD_ATTR 'roi'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x2a, // LOAD_ATTR 'window_h'
    0xbd, // LOAD_FAST 13
    0xf4, // BINARY_OP 29 __mul__
    0xf3, // BINARY_OP 28 __sub__
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf7, // BINARY_OP 32 __truediv__
    0xb0, // LOAD_FAST 0
    0x13,0x2b, // LOAD_ATTR 'roi'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xcf, // STORE_FAST 15
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x86,0x81, // JUMP 134
    0x57, // DUP_TOP
    0xca, // STORE_FAST 10
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xbc, // LOAD_FAST 12
    0xf4, // BINARY_OP 29 __mul__
    0xbe, // LOAD_FAST 14
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x56, // STORE_SUBSCR
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xbd, // LOAD_FAST 13
    0xf4, // BINARY_OP 29 __mul__
    0xbf, // LOAD_FAST 15
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0x56, // STORE_SUBSCR
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xbc, // LOAD_FAST 12
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x82, // LOAD_CONST_SMALL_INT 2
    0x56, // STORE_SUBSCR
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xbd, // LOAD_FAST 13
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x83, // LOAD_CONST_SMALL_INT 3
    0x56, // STORE_SUBSCR
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0x26,0x10, // STORE_FAST_N 16
    0x24,0x10, // LOAD_FAST_N 16
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x70, // POP_JUMP_IF_FALSE 48
    0x24,0x10, // LOAD_FAST_N 16
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xbc, // LOAD_FAST 12
    0xf4, // BINARY_OP 29 __mul__
    0xbe, // LOAD_FAST 14
    0xf2, // BINARY_OP 27 __add__
    0x24,0x10, // LOAD_FAST_N 16
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0x24,0x10, // LOAD_FAST_N 16
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xbd, // LOAD_FAST 13
    0xf4, // BINARY_OP 29 __mul__
    0xbf, // LOAD_FAST 15
    0xf2, // BINARY_OP 27 __add__
    0x24,0x10, // LOAD_FAST_N 16
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xf4,0x7e, // POP_JUMP_IF_TRUE -140
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x32,0x04, // MAKE_FUNCTION 4
    0x12,0x60, // LOAD_GLOBAL 'range'
    0xb8, // LOAD_FAST 8
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x11, // STORE_FAST_N 17
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0xb7, // LOAD_FAST 7
    0x34,0x01, // CALL_FUNCTION 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xbf,0x80, // JUMP 63
    0x57, // DUP_TOP
    0xca, // STORE_FAST 10
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x51, // LOAD_CONST_NONE
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x2b,0x02, // BUILD_LIST 2
    0x26,0x12, // STORE_FAST_N 18
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x86, // LOAD_CONST_SMALL_INT 6
    0x55, // LOAD_SUBSCR
    0x26,0x10, // STORE_FAST_N 16
    0x24,0x10, // LOAD_FAST_N 16
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x49, // POP_JUMP_IF_FALSE 9
    0x24,0x12, // LOAD_FAST_N 18
    0x14,0x2e, // LOAD_METHOD 'append'
    0x24,0x10, // LOAD_FAST_N 16
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x24,0x11, // LOAD_FAST_N 17
    0xb7, // LOAD_FAST 7
    0xba, // LOAD_FAST 10
    0x55, // LOAD_SUBSCR
    0x85, // LOAD_CONST_SMALL_INT 5
    0x55, // LOAD_SUBSCR
    0x55, // LOAD_SUBSCR
    0x14,0x2e, // LOAD_METHOD 'append'
    0x12,0x61, // LOAD_GLOBAL 'tuple'
    0x24,0x12, // LOAD_FAST_N 18
    0x34,0x01, // CALL_FUNCTION 1
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xbb,0x7f, // POP_JUMP_IF_TRUE -69
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x24,0x11, // LOAD_FAST_N 17
    0x63, // RETURN_VALUE
};
// child of ml_utils_NMS_get_bounding_boxes
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_
static const byte fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_[10] = {
    0x11,0x08, // prelude
    0x33,0x37, // names: <lambda>, x
    0x80,0x59, // code info
    0xb0, // LOAD_FAST 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 10,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_ + 4,
        .line_info_top = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_ + 6,
        .opcodes = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_ fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_[0]
#endif

// child of ml_utils_NMS_get_bounding_boxes
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS_get_bounding_boxes_iou
static const byte fun_data_ml_utils_NMS_get_bounding_boxes_iou[127] = {
    0x72,0x1a, // prelude
    0x34,0x62,0x63, // names: iou, box1, box2
    0x80,0x5d,0x2b,0x2b,0x33,0x33,0x29,0x29,0x24,0x32, // code info
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x34,0x02, // CALL_FUNCTION 2
    0xc2, // STORE_FAST 2
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x34,0x02, // CALL_FUNCTION 2
    0xc3, // STORE_FAST 3
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0xb0, // LOAD_FAST 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x34,0x02, // CALL_FUNCTION 2
    0xc4, // STORE_FAST 4
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf2, // BINARY_OP 27 __add__
    0x34,0x02, // CALL_FUNCTION 2
    0xc5, // STORE_FAST 5
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb4, // LOAD_FAST 4
    0xb2, // LOAD_FAST 2
    0xf3, // BINARY_OP 28 __sub__
    0x34,0x02, // CALL_FUNCTION 2
    0xc6, // STORE_FAST 6
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xb5, // LOAD_FAST 5
    0xb3, // LOAD_FAST 3
    0xf3, // BINARY_OP 28 __sub__
    0x34,0x02, // CALL_FUNCTION 2
    0xc7, // STORE_FAST 7
    0xb6, // LOAD_FAST 6
    0xb7, // LOAD_FAST 7
    0xf4, // BINARY_OP 29 __mul__
    0xc8, // STORE_FAST 8
    0xb0, // LOAD_FAST 0
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf4, // BINARY_OP 29 __mul__
    0xb1, // LOAD_FAST 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0xf4, // BINARY_OP 29 __mul__
    0xf2, // BINARY_OP 27 __add__
    0xb8, // LOAD_FAST 8
    0xf3, // BINARY_OP 28 __sub__
    0xc9, // STORE_FAST 9
    0x12,0x3e, // LOAD_GLOBAL 'float'
    0xb8, // LOAD_FAST 8
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x3e, // LOAD_GLOBAL 'float'
    0xb9, // LOAD_FAST 9
    0x34,0x01, // CALL_FUNCTION 1
    0xf7, // BINARY_OP 32 __truediv__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS_get_bounding_boxes_iou = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS_get_bounding_boxes_iou,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 127,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 15,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 2,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 52,
        .line_info = fun_data_ml_utils_NMS_get_bounding_boxes_iou + 5,
        .line_info_top = fun_data_ml_utils_NMS_get_bounding_boxes_iou + 15,
        .opcodes = fun_data_ml_utils_NMS_get_bounding_boxes_iou + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_NMS_get_bounding_boxes_iou fun_data_ml_utils_NMS_get_bounding_boxes_iou[0]
#endif

// child of ml_utils_NMS_get_bounding_boxes
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2
static const byte fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2[13] = {
    0x11,0x08, // prelude
    0x33,0x37, // names: <lambda>, x
    0x80,0x7c, // code info
    0xb0, // LOAD_FAST 0
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xd8, // BINARY_OP 1 __gt__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 13,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2 + 4,
        .line_info_top = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2 + 6,
        .opcodes = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2 + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2 fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2[0]
#endif

// child of ml_utils_NMS_get_bounding_boxes
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3
static const byte fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3[12] = {
    0x11,0x08, // prelude
    0x33,0x37, // names: <lambda>, x
    0x80,0x7e, // code info
    0xb0, // LOAD_FAST 0
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x84, // LOAD_CONST_SMALL_INT 4
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 12,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 51,
        .line_info = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3 + 4,
        .line_info_top = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3 + 6,
        .opcodes = fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3 + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3 fun_data_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3[0]
#endif

// child of ml_utils_NMS_get_bounding_boxes
// frozen bytecode for file ml/utils.py, scope ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_
static const byte fun_data_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_[20] = {
    0x39,0x08, // prelude
    0x35,0x64, // names: <listcomp>, *
    0x80,0x93, // code info
    0x2b,0x00, // BUILD_LIST 0
    0xb0, // LOAD_FAST 0
    0x5f, // GET_ITER_STACK
    0x4b,0x07, // FOR_ITER 7
    0xc1, // STORE_FAST 1
    0x2b,0x00, // BUILD_LIST 0
    0x2f,0x14, // STORE_COMP 20
    0x42,0x37, // JUMP -9
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 20,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 53,
        .line_info = fun_data_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_ + 4,
        .line_info_top = fun_data_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_ + 6,
        .opcodes = fun_data_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_ + 6,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_ fun_data_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_[0]
#endif

static const mp_raw_code_t *const children_ml_utils_NMS_get_bounding_boxes[] = {
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_,
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS_get_bounding_boxes_iou,
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_2,
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS_get_bounding_boxes__lt_lambda_gt_3,
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS_get_bounding_boxes__lt_listcomp_gt_,
};

static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS_get_bounding_boxes = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS_get_bounding_boxes,
    .children = (void *)&children_ml_utils_NMS_get_bounding_boxes,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 545,
    .n_children = 5,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 26,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 3,
        .n_kwonly_args = 0,
        .n_def_pos_args = 2,
        .qstr_block_name_idx = 47,
        .line_info = fun_data_ml_utils_NMS_get_bounding_boxes + 8,
        .line_info_top = fun_data_ml_utils_NMS_get_bounding_boxes + 54,
        .opcodes = fun_data_ml_utils_NMS_get_bounding_boxes + 54,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_utils_NMS[] = {
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS___init__,
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS_add_bounding_box,
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS_get_bounding_boxes,
};

static const mp_raw_code_truncated_t proto_fun_ml_utils_NMS = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_NMS,
    .children = (void *)&children_ml_utils_NMS,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 42,
    .n_children = 3,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 5,
        .line_info = fun_data_ml_utils_NMS + 3,
        .line_info_top = fun_data_ml_utils_NMS + 9,
        .opcodes = fun_data_ml_utils_NMS + 9,
    },
    #endif
    #endif
};

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_draw_predictions
static const byte fun_data_ml_utils_draw_predictions[424] = {
    0xb1,0xa1,0x84,0x01,0x5a, // prelude
    0x16,0x42,0x2c,0x43,0x44,0x38,0x45,0x46,0x47,0x48, // names: draw_predictions, image, boxes, labels, colors, scores, format, font_width, font_height, text_color
    0x80,0xaa,0x26,0x66,0x30,0x30,0x24,0x24,0x36,0x25,0x26,0x2c,0x45,0x26,0x2a,0x2a,0x2d,0x4d,0x67,0x40,0x29,0x29,0x30,0x23,0x56,0x2e,0x23,0x7a,0x20,0x37,0x43,0x38,0x23,0x7c,0x20, // code info
    0xb0, // LOAD_FAST 0
    0x14,0x17, // LOAD_METHOD 'width'
    0x36,0x00, // CALL_METHOD 0
    0xc9, // STORE_FAST 9
    0xb0, // LOAD_FAST 0
    0x14,0x18, // LOAD_METHOD 'height'
    0x36,0x00, // CALL_METHOD 0
    0xca, // STORE_FAST 10
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0x84, // LOAD_CONST_SMALL_INT 4
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb9, // LOAD_FAST 9
    0x22,0x82,0x40, // LOAD_CONST_SMALL_INT 320
    0xf6, // BINARY_OP 31 __floordiv__
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xcb, // STORE_FAST 11
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0x83, // LOAD_CONST_SMALL_INT 3
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x81, // LOAD_CONST_SMALL_INT 1
    0xb9, // LOAD_FAST 9
    0x22,0x83,0x60, // LOAD_CONST_SMALL_INT 480
    0xf6, // BINARY_OP 31 __floordiv__
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0xcc, // STORE_FAST 12
    0xb6, // LOAD_FAST 6
    0xbb, // LOAD_FAST 11
    0xf4, // BINARY_OP 29 __mul__
    0xcd, // STORE_FAST 13
    0xb7, // LOAD_FAST 7
    0xbb, // LOAD_FAST 11
    0xf4, // BINARY_OP 29 __mul__
    0xce, // STORE_FAST 14
    0x12,0x49, // LOAD_GLOBAL 'enumerate'
    0xb1, // LOAD_FAST 1
    0x34,0x01, // CALL_FUNCTION 1
    0x5f, // GET_ITER_STACK
    0x4b,0xb7,0x02, // FOR_ITER 311
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xcf, // STORE_FAST 15
    0x30,0x04, // UNPACK_SEQUENCE 4
    0x26,0x10, // STORE_FAST_N 16
    0x26,0x11, // STORE_FAST_N 17
    0x26,0x12, // STORE_FAST_N 18
    0x26,0x13, // STORE_FAST_N 19
    0xb2, // LOAD_FAST 2
    0xbf, // LOAD_FAST 15
    0x55, // LOAD_SUBSCR
    0x26,0x14, // STORE_FAST_N 20
    0xb4, // LOAD_FAST 4
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0xd3, // UNARY_OP 3 <not>
    0x44,0x4c, // POP_JUMP_IF_FALSE 12
    0x10,0x19, // LOAD_CONST_STRING '%s %.2f'
    0x24,0x14, // LOAD_FAST_N 20
    0xb4, // LOAD_FAST 4
    0xbf, // LOAD_FAST 15
    0x55, // LOAD_SUBSCR
    0x2a,0x02, // BUILD_TUPLE 2
    0xf8, // BINARY_OP 33 __mod__
    0x26,0x14, // STORE_FAST_N 20
    0xb3, // LOAD_FAST 3
    0xbf, // LOAD_FAST 15
    0x55, // LOAD_SUBSCR
    0x26,0x15, // STORE_FAST_N 21
    0xb5, // LOAD_FAST 5
    0x10,0x06, // LOAD_CONST_STRING 'pascal_voc'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x6e, // POP_JUMP_IF_FALSE 46
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0x24,0x10, // LOAD_FAST_N 16
    0xb9, // LOAD_FAST 9
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x10, // STORE_FAST_N 16
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0x24,0x11, // LOAD_FAST_N 17
    0xba, // LOAD_FAST 10
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x11, // STORE_FAST_N 17
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0x24,0x12, // LOAD_FAST_N 18
    0xb9, // LOAD_FAST 9
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x24,0x10, // LOAD_FAST_N 16
    0xf3, // BINARY_OP 28 __sub__
    0x26,0x12, // STORE_FAST_N 18
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0x24,0x13, // LOAD_FAST_N 19
    0xba, // LOAD_FAST 10
    0xf4, // BINARY_OP 29 __mul__
    0x34,0x01, // CALL_FUNCTION 1
    0x24,0x11, // LOAD_FAST_N 17
    0xf3, // BINARY_OP 28 __sub__
    0x26,0x13, // STORE_FAST_N 19
    0xb5, // LOAD_FAST 5
    0x10,0x1a, // LOAD_CONST_STRING 'point'
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x80,0x81, // POP_JUMP_IF_FALSE 128
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x12, // LOAD_FAST_N 18
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf6, // BINARY_OP 31 __floordiv__
    0xf2, // BINARY_OP 27 __add__
    0x26,0x16, // STORE_FAST_N 22
    0x24,0x11, // LOAD_FAST_N 17
    0x24,0x13, // LOAD_FAST_N 19
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf6, // BINARY_OP 31 __floordiv__
    0xf2, // BINARY_OP 27 __add__
    0x26,0x17, // STORE_FAST_N 23
    0x12,0x0f, // LOAD_GLOBAL 'min'
    0x90, // LOAD_CONST_SMALL_INT 16
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x84, // LOAD_CONST_SMALL_INT 4
    0xb9, // LOAD_FAST 9
    0x22,0x3c, // LOAD_CONST_SMALL_INT 60
    0xf6, // BINARY_OP 31 __floordiv__
    0x34,0x02, // CALL_FUNCTION 2
    0x34,0x02, // CALL_FUNCTION 2
    0x26,0x18, // STORE_FAST_N 24
    0xb0, // LOAD_FAST 0
    0x14,0x1b, // LOAD_METHOD 'draw_circle'
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x17, // LOAD_FAST_N 23
    0x24,0x18, // LOAD_FAST_N 24
    0x2a,0x03, // BUILD_TUPLE 3
    0x10,0x1c, // LOAD_CONST_STRING 'color'
    0x24,0x15, // LOAD_FAST_N 21
    0x10,0x1d, // LOAD_CONST_STRING 'thickness'
    0xbc, // LOAD_FAST 12
    0x10,0x1e, // LOAD_CONST_STRING 'fill'
    0x52, // LOAD_CONST_TRUE
    0x36,0x86,0x01, // CALL_METHOD 769
    0x59, // POP_TOP
    0x12,0x0c, // LOAD_GLOBAL 'max'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x24,0x17, // LOAD_FAST_N 23
    0x24,0x18, // LOAD_FAST_N 24
    0xf3, // BINARY_OP 28 __sub__
    0xbe, // LOAD_FAST 14
    0xf3, // BINARY_OP 28 __sub__
    0x34,0x02, // CALL_FUNCTION 2
    0x26,0x19, // STORE_FAST_N 25
    0xb0, // LOAD_FAST 0
    0x14,0x1f, // LOAD_METHOD 'draw_rectangle'
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x19, // LOAD_FAST_N 25
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0x24,0x14, // LOAD_FAST_N 20
    0x34,0x01, // CALL_FUNCTION 1
    0xbd, // LOAD_FAST 13
    0xf4, // BINARY_OP 29 __mul__
    0xbe, // LOAD_FAST 14
    0x2a,0x04, // BUILD_TUPLE 4
    0x10,0x1e, // LOAD_CONST_STRING 'fill'
    0x52, // LOAD_CONST_TRUE
    0x10,0x1c, // LOAD_CONST_STRING 'color'
    0x24,0x15, // LOAD_FAST_N 21
    0x36,0x84,0x01, // CALL_METHOD 513
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x20, // LOAD_METHOD 'draw_string'
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x19, // LOAD_FAST_N 25
    0x2a,0x02, // BUILD_TUPLE 2
    0x24,0x14, // LOAD_FAST_N 20
    0x14,0x21, // LOAD_METHOD 'upper'
    0x36,0x00, // CALL_METHOD 0
    0xb8, // LOAD_FAST 8
    0x10,0x22, // LOAD_CONST_STRING 'scale'
    0xbb, // LOAD_FAST 11
    0x36,0x82,0x03, // CALL_METHOD 259
    0x59, // POP_TOP
    0x42,0x99,0x7e, // JUMP -231
    0xb0, // LOAD_FAST 0
    0x14,0x1f, // LOAD_METHOD 'draw_rectangle'
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x11, // LOAD_FAST_N 17
    0x24,0x12, // LOAD_FAST_N 18
    0x24,0x13, // LOAD_FAST_N 19
    0x2a,0x04, // BUILD_TUPLE 4
    0x10,0x1c, // LOAD_CONST_STRING 'color'
    0x24,0x15, // LOAD_FAST_N 21
    0x10,0x1d, // LOAD_CONST_STRING 'thickness'
    0xbc, // LOAD_FAST 12
    0x36,0x84,0x01, // CALL_METHOD 513
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x1f, // LOAD_METHOD 'draw_rectangle'
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x11, // LOAD_FAST_N 17
    0xbe, // LOAD_FAST 14
    0xf3, // BINARY_OP 28 __sub__
    0x12,0x4b, // LOAD_GLOBAL 'len'
    0x24,0x14, // LOAD_FAST_N 20
    0x34,0x01, // CALL_FUNCTION 1
    0xbd, // LOAD_FAST 13
    0xf4, // BINARY_OP 29 __mul__
    0xbe, // LOAD_FAST 14
    0x2a,0x04, // BUILD_TUPLE 4
    0x10,0x1e, // LOAD_CONST_STRING 'fill'
    0x52, // LOAD_CONST_TRUE
    0x10,0x1c, // LOAD_CONST_STRING 'color'
    0x24,0x15, // LOAD_FAST_N 21
    0x36,0x84,0x01, // CALL_METHOD 513
    0x59, // POP_TOP
    0xb0, // LOAD_FAST 0
    0x14,0x20, // LOAD_METHOD 'draw_string'
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x11, // LOAD_FAST_N 17
    0xbe, // LOAD_FAST 14
    0xf3, // BINARY_OP 28 __sub__
    0x2a,0x02, // BUILD_TUPLE 2
    0x24,0x14, // LOAD_FAST_N 20
    0x14,0x21, // LOAD_METHOD 'upper'
    0x36,0x00, // CALL_METHOD 0
    0xb8, // LOAD_FAST 8
    0x10,0x22, // LOAD_CONST_STRING 'scale'
    0xbb, // LOAD_FAST 11
    0x36,0x82,0x03, // CALL_METHOD 259
    0x59, // POP_TOP
    0x42,0xc6,0x7d, // JUMP -314
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_draw_predictions = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_draw_predictions,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 424,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 39,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 9,
        .n_kwonly_args = 0,
        .n_def_pos_args = 5,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_ml_utils_draw_predictions + 15,
        .line_info_top = fun_data_ml_utils_draw_predictions + 50,
        .opcodes = fun_data_ml_utils_draw_predictions + 50,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_draw_predictions fun_data_ml_utils_draw_predictions[0]
#endif

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_draw_keypoints
static const byte fun_data_ml_utils_draw_keypoints[106] = {
    0x9a,0x94,0x80,0x01,0x1e, // prelude
    0x23,0x42,0x4c,0x26,0x1c,0x1d,0x1e, // names: draw_keypoints, image, keypoints, radius, color, thickness, fill
    0x80,0xe4,0x25,0x25,0x1f,0x26,0x25,0x25, // code info
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd8, // BINARY_OP 1 __gt__
    0x44,0x6a, // POP_JUMP_IF_FALSE 42
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x24, // FOR_ITER 36
    0xc6, // STORE_FAST 6
    0xb0, // LOAD_FAST 0
    0x14,0x1b, // LOAD_METHOD 'draw_circle'
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb6, // LOAD_FAST 6
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xb2, // LOAD_FAST 2
    0x2a,0x03, // BUILD_TUPLE 3
    0x10,0x1c, // LOAD_CONST_STRING 'color'
    0xb3, // LOAD_FAST 3
    0x10,0x1d, // LOAD_CONST_STRING 'thickness'
    0xb4, // LOAD_FAST 4
    0x10,0x1e, // LOAD_CONST_STRING 'fill'
    0xb5, // LOAD_FAST 5
    0x36,0x86,0x01, // CALL_METHOD 769
    0x59, // POP_TOP
    0x42,0x1a, // JUMP -38
    0x42,0x65, // JUMP 37
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0xd9, // BINARY_OP 2 __eq__
    0x44,0x60, // POP_JUMP_IF_FALSE 32
    0xb1, // LOAD_FAST 1
    0x5f, // GET_ITER_STACK
    0x4b,0x1a, // FOR_ITER 26
    0xc6, // STORE_FAST 6
    0xb0, // LOAD_FAST 0
    0x14,0x24, // LOAD_METHOD 'set_pixel'
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb6, // LOAD_FAST 6
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb6, // LOAD_FAST 6
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x2a,0x02, // BUILD_TUPLE 2
    0xb3, // LOAD_FAST 3
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x42,0x24, // JUMP -28
    0x42,0x40, // JUMP 0
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_draw_keypoints = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_draw_keypoints,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 106,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 20,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 6,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 35,
        .line_info = fun_data_ml_utils_draw_keypoints + 12,
        .line_info_top = fun_data_ml_utils_draw_keypoints + 20,
        .opcodes = fun_data_ml_utils_draw_keypoints + 20,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_draw_keypoints fun_data_ml_utils_draw_keypoints[0]
#endif

// child of ml_utils__lt_module_gt_
// frozen bytecode for file ml/utils.py, scope ml_utils_draw_skeleton
static const byte fun_data_ml_utils_draw_skeleton[109] = {
    0xb1,0x90,0x85,0x01,0x20, // prelude
    0x25,0x42,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53, // names: draw_skeleton, image, keypoints, lines, kp_radius, kp_color, kp_thickness, kp_fill, line_color, line_thickness
    0x80,0xf7,0x54,0x25,0x23,0x36, // code info
    0x12,0x23, // LOAD_GLOBAL 'draw_keypoints'
    0xb0, // LOAD_FAST 0
    0xb1, // LOAD_FAST 1
    0x10,0x26, // LOAD_CONST_STRING 'radius'
    0xb3, // LOAD_FAST 3
    0x10,0x1c, // LOAD_CONST_STRING 'color'
    0xb4, // LOAD_FAST 4
    0x10,0x1d, // LOAD_CONST_STRING 'thickness'
    0xb5, // LOAD_FAST 5
    0x10,0x1e, // LOAD_CONST_STRING 'fill'
    0xb6, // LOAD_FAST 6
    0x34,0x88,0x02, // CALL_FUNCTION 1026
    0x59, // POP_TOP
    0xb2, // LOAD_FAST 2
    0x5f, // GET_ITER_STACK
    0x4b,0x3e, // FOR_ITER 62
    0xc9, // STORE_FAST 9
    0xb0, // LOAD_FAST 0
    0x14,0x27, // LOAD_METHOD 'draw_line'
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb1, // LOAD_FAST 1
    0xb9, // LOAD_FAST 9
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb1, // LOAD_FAST 1
    0xb9, // LOAD_FAST 9
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb1, // LOAD_FAST 1
    0xb9, // LOAD_FAST 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x12,0x4a, // LOAD_GLOBAL 'int'
    0xb1, // LOAD_FAST 1
    0xb9, // LOAD_FAST 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x55, // LOAD_SUBSCR
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x2a,0x04, // BUILD_TUPLE 4
    0x10,0x1c, // LOAD_CONST_STRING 'color'
    0xb7, // LOAD_FAST 7
    0x10,0x1d, // LOAD_CONST_STRING 'thickness'
    0xb8, // LOAD_FAST 8
    0x36,0x84,0x01, // CALL_METHOD 513
    0x59, // POP_TOP
    0x42,0x00, // JUMP -64
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_utils_draw_skeleton = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils_draw_skeleton,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 109,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 23,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 9,
        .n_kwonly_args = 0,
        .n_def_pos_args = 6,
        .qstr_block_name_idx = 37,
        .line_info = fun_data_ml_utils_draw_skeleton + 15,
        .line_info_top = fun_data_ml_utils_draw_skeleton + 21,
        .opcodes = fun_data_ml_utils_draw_skeleton + 21,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_utils_draw_skeleton fun_data_ml_utils_draw_skeleton[0]
#endif

static const mp_raw_code_t *const children_ml_utils__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_utils_logit,
    (const mp_raw_code_t *)&proto_fun_ml_utils_sigmoid,
    (const mp_raw_code_t *)&proto_fun_ml_utils_threshold,
    (const mp_raw_code_t *)&proto_fun_ml_utils_quantize,
    (const mp_raw_code_t *)&proto_fun_ml_utils_dequantize,
    (const mp_raw_code_t *)&proto_fun_ml_utils_NMS,
    (const mp_raw_code_t *)&proto_fun_ml_utils_draw_predictions,
    (const mp_raw_code_t *)&proto_fun_ml_utils_draw_keypoints,
    (const mp_raw_code_t *)&proto_fun_ml_utils_draw_skeleton,
};

static const mp_raw_code_truncated_t proto_fun_ml_utils__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_utils__lt_module_gt_,
    .children = (void *)&children_ml_utils__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 133,
    .n_children = 9,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_utils__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_utils__lt_module_gt_ + 30,
        .opcodes = fun_data_ml_utils__lt_module_gt_ + 30,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_utils[101] = {
    MP_QSTR_ml_slash_utils_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_math,
    MP_QSTR_numpy,
    MP_QSTR_ulab,
    MP_QSTR_NMS,
    MP_QSTR_pascal_voc,
    MP_QSTR_logit,
    MP_QSTR_log,
    MP_QSTR_sigmoid,
    MP_QSTR_exp,
    MP_QSTR_threshold,
    MP_QSTR_max,
    MP_QSTR_axis,
    MP_QSTR_nonzero,
    MP_QSTR_min,
    MP_QSTR_quantize,
    MP_QSTR_output_dtype,
    MP_QSTR_f,
    MP_QSTR_output_scale,
    MP_QSTR_output_zero_point,
    MP_QSTR_dequantize,
    MP_QSTR_draw_predictions,
    MP_QSTR_width,
    MP_QSTR_height,
    MP_QSTR__percent_s_space__percent__dot_2f,
    MP_QSTR_point,
    MP_QSTR_draw_circle,
    MP_QSTR_color,
    MP_QSTR_thickness,
    MP_QSTR_fill,
    MP_QSTR_draw_rectangle,
    MP_QSTR_draw_string,
    MP_QSTR_upper,
    MP_QSTR_scale,
    MP_QSTR_draw_keypoints,
    MP_QSTR_set_pixel,
    MP_QSTR_draw_skeleton,
    MP_QSTR_radius,
    MP_QSTR_draw_line,
    MP_QSTR___init__,
    MP_QSTR_window_w,
    MP_QSTR_window_h,
    MP_QSTR_roi,
    MP_QSTR_boxes,
    MP_QSTR_add_bounding_box,
    MP_QSTR_append,
    MP_QSTR_get_bounding_boxes,
    MP_QSTR_key,
    MP_QSTR_reverse,
    MP_QSTR_pop,
    MP_QSTR__lt_lambda_gt_,
    MP_QSTR_iou,
    MP_QSTR__lt_listcomp_gt_,
    MP_QSTR_np,
    MP_QSTR_x,
    MP_QSTR_scores,
    MP_QSTR_find_max,
    MP_QSTR_find_max_axis,
    MP_QSTR_model,
    MP_QSTR_value,
    MP_QSTR_index,
    MP_QSTR_float,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_image,
    MP_QSTR_labels,
    MP_QSTR_colors,
    MP_QSTR_format,
    MP_QSTR_font_width,
    MP_QSTR_font_height,
    MP_QSTR_text_color,
    MP_QSTR_enumerate,
    MP_QSTR_int,
    MP_QSTR_len,
    MP_QSTR_keypoints,
    MP_QSTR_lines,
    MP_QSTR_kp_radius,
    MP_QSTR_kp_color,
    MP_QSTR_kp_thickness,
    MP_QSTR_kp_fill,
    MP_QSTR_line_color,
    MP_QSTR_line_thickness,
    MP_QSTR_self,
    MP_QSTR_ValueError,
    MP_QSTR_xmin,
    MP_QSTR_ymin,
    MP_QSTR_xmax,
    MP_QSTR_ymax,
    MP_QSTR_score,
    MP_QSTR_label_index,
    MP_QSTR_sigma,
    MP_QSTR_sorted,
    MP_QSTR_list,
    MP_QSTR_filter,
    MP_QSTR_range,
    MP_QSTR_tuple,
    MP_QSTR_box1,
    MP_QSTR_box2,
    MP_QSTR__star_,
};

// constants
static const mp_rom_obj_tuple_t const_obj_ml_utils_0 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(255),
    MP_ROM_INT(255),
    MP_ROM_INT(255),
}};
static const mp_rom_obj_tuple_t const_obj_ml_utils_1 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(255),
    MP_ROM_INT(0),
    MP_ROM_INT(0),
}};
static const mp_rom_obj_tuple_t const_obj_ml_utils_2 = {{&mp_type_tuple}, 3, {
    MP_ROM_INT(0),
    MP_ROM_INT(255),
    MP_ROM_INT(0),
}};
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_utils_3 = {{&mp_type_float}, (mp_float_t)1};
#define const_obj_ml_utils_3_macro MP_ROM_PTR(&const_obj_ml_utils_3)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_utils_3_macro ((mp_rom_obj_t)(0xc0000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_utils_3_macro ((mp_rom_obj_t)(0xbff4000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_utils_4 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_ml_utils_4_macro MP_ROM_PTR(&const_obj_ml_utils_4)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_utils_4_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_utils_4_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_utils_6 = {{&mp_type_float}, (mp_float_t)0};
#define const_obj_ml_utils_6_macro MP_ROM_PTR(&const_obj_ml_utils_6)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_utils_6_macro ((mp_rom_obj_t)(0x80800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_utils_6_macro ((mp_rom_obj_t)(0x8004000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_utils_7 = {{&mp_type_float}, (mp_float_t)-1};
#define const_obj_ml_utils_7_macro MP_ROM_PTR(&const_obj_ml_utils_7)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_utils_7_macro ((mp_rom_obj_t)(0x140000002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_utils_7_macro ((mp_rom_obj_t)(0x13ff4000000000000))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_utils[8] = {
    MP_ROM_PTR(&const_obj_ml_utils_0),
    MP_ROM_PTR(&const_obj_ml_utils_1),
    MP_ROM_PTR(&const_obj_ml_utils_2),
    const_obj_ml_utils_3_macro,
    const_obj_ml_utils_4_macro,
    MP_ROM_QSTR(MP_QSTR_Invalid_space_ROI_space_dimensions_bang_),
    const_obj_ml_utils_6_macro,
    const_obj_ml_utils_7_macro,
};

static const mp_frozen_module_t frozen_module_ml_utils = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_utils,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_utils,
    },
    .proto_fun = &proto_fun_ml_utils__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml___init__
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\__init__.mpy
// - frozen file name: ml/__init__.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/__init__.py, scope ml___init____lt_module_gt_
static const byte fun_data_ml___init____lt_module_gt_[15] = {
    0x08,0x06, // prelude
    0x01, // names: <module>
    0x80,0x1e, // code info
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'model'
    0x69, // IMPORT_STAR
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml___init____lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml___init____lt_module_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 15,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 2,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml___init____lt_module_gt_ + 3,
        .line_info_top = fun_data_ml___init____lt_module_gt_ + 5,
        .opcodes = fun_data_ml___init____lt_module_gt_ + 5,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml___init____lt_module_gt_ fun_data_ml___init____lt_module_gt_[0]
#endif

static const qstr_short_t const_qstr_table_data_ml___init__[4] = {
    MP_QSTR_ml_slash___init___dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_model,
};

static const mp_frozen_module_t frozen_module_ml___init__ = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml___init__,
        .obj_table = NULL,
    },
    .proto_fun = &proto_fun_ml___init____lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_postprocessing_darknet
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\postprocessing\darknet.mpy
// - frozen file name: ml/postprocessing/darknet.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/postprocessing/darknet.py, scope ml_postprocessing_darknet__lt_module_gt_
static const byte fun_data_ml_postprocessing_darknet__lt_module_gt_[65] = {
    0x18,0x12, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x28,0x2c,0x8c,0x08,0x89,0x51, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'ml.utils'
    0x69, // IMPORT_STAR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'micropython'
    0x1c,0x04, // IMPORT_FROM 'const'
    0x16,0x04, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'numpy'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x07, // IMPORT_NAME 'ulab'
    0x1c,0x06, // IMPORT_FROM 'numpy'
    0x16,0x23, // STORE_NAME 'np'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x08, // LOAD_CONST_STRING 'YoloV2'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x08, // STORE_NAME 'YoloV2'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x09, // LOAD_CONST_STRING 'YoloLC'
    0x11,0x08, // LOAD_NAME 'YoloV2'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x09, // STORE_NAME 'YoloLC'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_darknet__lt_module_gt_
// frozen bytecode for file ml/postprocessing/darknet.py, scope ml_postprocessing_darknet_YoloV2
static const byte fun_data_ml_postprocessing_darknet_YoloV2[37] = {
    0x18,0x0e, // prelude
    0x08, // names: YoloV2
    0x88,0x27,0x80,0x07,0x8e,0x0f, // code info
    0x11,0x24, // LOAD_NAME '__name__'
    0x16,0x25, // STORE_NAME '__module__'
    0x10,0x08, // LOAD_CONST_STRING 'YoloV2'
    0x16,0x26, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x51, // LOAD_CONST_NONE
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0a, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x11, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_darknet_YoloV2
// frozen bytecode for file ml/postprocessing/darknet.py, scope ml_postprocessing_darknet_YoloV2___init__
static const byte fun_data_ml_postprocessing_darknet_YoloV2___init__[99] = {
    0xe1,0x84,0x80,0x01,0x24, // prelude
    0x0a,0x27,0x0b,0x0c,0x0d,0x0e, // names: __init__, self, threshold, anchors, nms_threshold, nms_sigma
    0x80,0x2f,0x24,0x24,0x24,0x44,0x27,0x2a,0x26,0x26,0x26,0x4d, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x0c, // STORE_ATTR 'anchors'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x0d, // STORE_ATTR 'nms_threshold'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x0e, // STORE_ATTR 'nms_sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'anchors'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x69, // POP_JUMP_IF_FALSE 41
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x0f, // LOAD_METHOD 'array'
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x0a, // LOAD_CONST_OBJ 10
    0x23,0x0b, // LOAD_CONST_OBJ 11
    0x2b,0x02, // BUILD_LIST 2
    0x2b,0x05, // BUILD_LIST 5
    0x36,0x01, // CALL_METHOD 1
    0xb0, // LOAD_FAST 0
    0x18,0x0c, // STORE_ATTR 'anchors'
    0x12,0x28, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'anchors'
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'anchors_len'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_darknet_YoloV2___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_darknet_YoloV2___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 99,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 13,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 5,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_postprocessing_darknet_YoloV2___init__ + 11,
        .line_info_top = fun_data_ml_postprocessing_darknet_YoloV2___init__ + 23,
        .opcodes = fun_data_ml_postprocessing_darknet_YoloV2___init__ + 23,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_darknet_YoloV2___init__ fun_data_ml_postprocessing_darknet_YoloV2___init__[0]
#endif

// child of ml_postprocessing_darknet_YoloV2
// frozen bytecode for file ml/postprocessing/darknet.py, scope ml_postprocessing_darknet_YoloV2___call__
static const byte fun_data_ml_postprocessing_darknet_YoloV2___call__[483] = {
    0xe8,0x24,0x50, // prelude
    0x11,0x27,0x29,0x2a,0x2b, // names: __call__, self, model, inputs, outputs
    0x80,0x3f,0x63,0x20,0x2b,0x26,0x2d,0x68,0x74,0x2a,0x28,0x27,0x63,0x72,0x28,0x29,0x67,0x71,0x6f,0x7b,0x34,0x33,0x1f,0x22,0x1f,0x62,0x2f,0x2a,0x2a,0x2a,0x4a,0x2f,0x2b,0x1f,0x2f, // code info
    0x32,0x00, // MAKE_FUNCTION 0
    0xc4, // STORE_FAST 4
    0xb1, // LOAD_FAST 1
    0x13,0x12, // LOAD_ATTR 'output_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0xc8, // STORE_FAST 8
    0xb1, // LOAD_FAST 1
    0x13,0x13, // LOAD_ATTR 'output_scale'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc9, // STORE_FAST 9
    0x12,0x2c, // LOAD_GLOBAL 'quantize'
    0xb1, // LOAD_FAST 1
    0x12,0x2d, // LOAD_GLOBAL 'logit'
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'threshold'
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x02, // CALL_FUNCTION 2
    0xca, // STORE_FAST 10
    0xb8, // LOAD_FAST 8
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors_len'
    0xf6, // BINARY_OP 31 __floordiv__
    0x85, // LOAD_CONST_SMALL_INT 5
    0xf3, // BINARY_OP 28 __sub__
    0xcb, // STORE_FAST 11
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x14,0x14, // LOAD_METHOD 'reshape'
    0xb6, // LOAD_FAST 6
    0xb7, // LOAD_FAST 7
    0xf4, // BINARY_OP 29 __mul__
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors_len'
    0xf4, // BINARY_OP 29 __mul__
    0x85, // LOAD_CONST_SMALL_INT 5
    0xbb, // LOAD_FAST 11
    0xf2, // BINARY_OP 27 __add__
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xcc, // STORE_FAST 12
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xcd, // STORE_FAST 13
    0x12,0x0b, // LOAD_GLOBAL 'threshold'
    0xbd, // LOAD_FAST 13
    0xba, // LOAD_FAST 10
    0xb9, // LOAD_FAST 9
    0x34,0x03, // CALL_FUNCTION 3
    0xcd, // STORE_FAST 13
    0x12,0x28, // LOAD_GLOBAL 'len'
    0xbd, // LOAD_FAST 13
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x0c, // LOAD_CONST_OBJ 12
    0x63, // RETURN_VALUE
    0x12,0x2e, // LOAD_GLOBAL 'dequantize'
    0xb1, // LOAD_FAST 1
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x15, // LOAD_METHOD 'take'
    0xbc, // LOAD_FAST 12
    0xbd, // LOAD_FAST 13
    0x10,0x16, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x34,0x02, // CALL_FUNCTION 2
    0xce, // STORE_FAST 14
    0xbd, // LOAD_FAST 13
    0xb7, // LOAD_FAST 7
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors_len'
    0xf4, // BINARY_OP 29 __mul__
    0xf6, // BINARY_OP 31 __floordiv__
    0xcf, // STORE_FAST 15
    0xbd, // LOAD_FAST 13
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors_len'
    0xf6, // BINARY_OP 31 __floordiv__
    0xb7, // LOAD_FAST 7
    0xf8, // BINARY_OP 33 __mod__
    0x26,0x10, // STORE_FAST_N 16
    0xbd, // LOAD_FAST 13
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors_len'
    0xf8, // BINARY_OP 33 __mod__
    0x26,0x11, // STORE_FAST_N 17
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x15, // LOAD_METHOD 'take'
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'anchors'
    0x24,0x11, // LOAD_FAST_N 17
    0x10,0x16, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x26,0x12, // STORE_FAST_N 18
    0x12,0x2f, // LOAD_GLOBAL 'sigmoid'
    0xbe, // LOAD_FAST 14
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x13, // STORE_FAST_N 19
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x17, // LOAD_METHOD 'argmax'
    0xb4, // LOAD_FAST 4
    0xbe, // LOAD_FAST 14
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x85, // LOAD_CONST_SMALL_INT 5
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0x10,0x16, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0x26,0x14, // STORE_FAST_N 20
    0x24,0x10, // LOAD_FAST_N 16
    0x12,0x2f, // LOAD_GLOBAL 'sigmoid'
    0xbe, // LOAD_FAST 14
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xb7, // LOAD_FAST 7
    0xf7, // BINARY_OP 32 __truediv__
    0x26,0x15, // STORE_FAST_N 21
    0xbf, // LOAD_FAST 15
    0x12,0x2f, // LOAD_GLOBAL 'sigmoid'
    0xbe, // LOAD_FAST 14
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xf2, // BINARY_OP 27 __add__
    0xb6, // LOAD_FAST 6
    0xf7, // BINARY_OP 32 __truediv__
    0x26,0x16, // STORE_FAST_N 22
    0x24,0x12, // LOAD_FAST_N 18
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x18, // LOAD_METHOD 'exp'
    0xbe, // LOAD_FAST 14
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xf4, // BINARY_OP 29 __mul__
    0xb7, // LOAD_FAST 7
    0xf7, // BINARY_OP 32 __truediv__
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x17, // STORE_FAST_N 23
    0x24,0x12, // LOAD_FAST_N 18
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x18, // LOAD_METHOD 'exp'
    0xbe, // LOAD_FAST 14
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xf4, // BINARY_OP 29 __mul__
    0xb6, // LOAD_FAST 6
    0xf7, // BINARY_OP 32 __truediv__
    0x23,0x0d, // LOAD_CONST_OBJ 13
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x18, // STORE_FAST_N 24
    0xb1, // LOAD_FAST 1
    0x13,0x19, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0x26,0x19, // STORE_FAST_N 25
    0x26,0x1a, // STORE_FAST_N 26
    0x26,0x1b, // STORE_FAST_N 27
    0x26,0x1c, // STORE_FAST_N 28
    0x24,0x15, // LOAD_FAST_N 21
    0x24,0x17, // LOAD_FAST_N 23
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x1b, // LOAD_FAST_N 27
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1d, // STORE_FAST_N 29
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x18, // LOAD_FAST_N 24
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x1a, // LOAD_FAST_N 26
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1e, // STORE_FAST_N 30
    0x24,0x15, // LOAD_FAST_N 21
    0x24,0x17, // LOAD_FAST_N 23
    0xf2, // BINARY_OP 27 __add__
    0x24,0x1b, // LOAD_FAST_N 27
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1f, // STORE_FAST_N 31
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x18, // LOAD_FAST_N 24
    0xf2, // BINARY_OP 27 __add__
    0x24,0x1a, // LOAD_FAST_N 26
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x20, // STORE_FAST_N 32
    0x12,0x30, // LOAD_GLOBAL 'NMS'
    0x24,0x1b, // LOAD_FAST_N 27
    0x24,0x1a, // LOAD_FAST_N 26
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x1a, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0x26,0x21, // STORE_FAST_N 33
    0xbe, // LOAD_FAST 14
    0x13,0x1b, // LOAD_ATTR 'shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x6a, // JUMP 42
    0x57, // DUP_TOP
    0x26,0x22, // STORE_FAST_N 34
    0x24,0x21, // LOAD_FAST_N 33
    0x14,0x1c, // LOAD_METHOD 'add_bounding_box'
    0x24,0x1d, // LOAD_FAST_N 29
    0x24,0x22, // LOAD_FAST_N 34
    0x55, // LOAD_SUBSCR
    0x24,0x1e, // LOAD_FAST_N 30
    0x24,0x22, // LOAD_FAST_N 34
    0x55, // LOAD_SUBSCR
    0x24,0x1f, // LOAD_FAST_N 31
    0x24,0x22, // LOAD_FAST_N 34
    0x55, // LOAD_SUBSCR
    0x24,0x20, // LOAD_FAST_N 32
    0x24,0x22, // LOAD_FAST_N 34
    0x55, // LOAD_SUBSCR
    0x24,0x13, // LOAD_FAST_N 19
    0x24,0x22, // LOAD_FAST_N 34
    0x55, // LOAD_SUBSCR
    0x24,0x14, // LOAD_FAST_N 20
    0x24,0x22, // LOAD_FAST_N 34
    0x55, // LOAD_SUBSCR
    0x36,0x06, // CALL_METHOD 6
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x11, // POP_JUMP_IF_TRUE -47
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x24,0x21, // LOAD_FAST_N 33
    0x14,0x1d, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0b, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x0d, // LOAD_ATTR 'nms_threshold'
    0x10,0x1e, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_darknet_YoloV2___call__
// frozen bytecode for file ml/postprocessing/darknet.py, scope ml_postprocessing_darknet_YoloV2___call___softmax
static const byte fun_data_ml_postprocessing_darknet_YoloV2___call___softmax[47] = {
    0x59,0x0a, // prelude
    0x1f,0x32, // names: softmax, x
    0x80,0x40,0x37, // code info
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x18, // LOAD_METHOD 'exp'
    0xb0, // LOAD_FAST 0
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x20, // LOAD_METHOD 'max'
    0xb0, // LOAD_FAST 0
    0x10,0x16, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x21, // LOAD_CONST_STRING 'keepdims'
    0x52, // LOAD_CONST_TRUE
    0x36,0x84,0x01, // CALL_METHOD 513
    0xf3, // BINARY_OP 28 __sub__
    0x36,0x01, // CALL_METHOD 1
    0xc1, // STORE_FAST 1
    0xb1, // LOAD_FAST 1
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x22, // LOAD_METHOD 'sum'
    0xb1, // LOAD_FAST 1
    0x10,0x16, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x10,0x21, // LOAD_CONST_STRING 'keepdims'
    0x52, // LOAD_CONST_TRUE
    0x36,0x84,0x01, // CALL_METHOD 513
    0xf7, // BINARY_OP 32 __truediv__
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_darknet_YoloV2___call___softmax = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_darknet_YoloV2___call___softmax,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 47,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 12,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 31,
        .line_info = fun_data_ml_postprocessing_darknet_YoloV2___call___softmax + 4,
        .line_info_top = fun_data_ml_postprocessing_darknet_YoloV2___call___softmax + 7,
        .opcodes = fun_data_ml_postprocessing_darknet_YoloV2___call___softmax + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_darknet_YoloV2___call___softmax fun_data_ml_postprocessing_darknet_YoloV2___call___softmax[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_darknet_YoloV2___call__[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_darknet_YoloV2___call___softmax,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_darknet_YoloV2___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_darknet_YoloV2___call__,
    .children = (void *)&children_ml_postprocessing_darknet_YoloV2___call__,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 483,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 46,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 17,
        .line_info = fun_data_ml_postprocessing_darknet_YoloV2___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_darknet_YoloV2___call__ + 43,
        .opcodes = fun_data_ml_postprocessing_darknet_YoloV2___call__ + 43,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_postprocessing_darknet_YoloV2[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_darknet_YoloV2___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_darknet_YoloV2___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_darknet_YoloV2 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_darknet_YoloV2,
    .children = (void *)&children_ml_postprocessing_darknet_YoloV2,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 37,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_ml_postprocessing_darknet_YoloV2 + 3,
        .line_info_top = fun_data_ml_postprocessing_darknet_YoloV2 + 9,
        .opcodes = fun_data_ml_postprocessing_darknet_YoloV2 + 9,
    },
    #endif
    #endif
};

// child of ml_postprocessing_darknet__lt_module_gt_
// frozen bytecode for file ml/postprocessing/darknet.py, scope ml_postprocessing_darknet_YoloLC
static const byte fun_data_ml_postprocessing_darknet_YoloLC[32] = {
    0x20,0x07, // prelude
    0x09, // names: YoloLC
    0x88,0x78,0x00, // code info
    0x11,0x24, // LOAD_NAME '__name__'
    0x16,0x25, // STORE_NAME '__module__'
    0x10,0x09, // LOAD_CONST_STRING 'YoloLC'
    0x16,0x26, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x51, // LOAD_CONST_NONE
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x21,0x00,0x01, // MAKE_CLOSURE_DEFARGS 0
    0x16,0x0a, // STORE_NAME '__init__'
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_darknet_YoloLC
// frozen bytecode for file ml/postprocessing/darknet.py, scope ml_postprocessing_darknet_YoloLC___init__
static const byte fun_data_ml_postprocessing_darknet_YoloLC___init__[80] = {
    0xea,0x84,0x80,0x01,0x1e, // prelude
    0x0a,0x02,0x27,0x0b,0x0c,0x0d,0x0e, // names: __init__, *, self, threshold, anchors, nms_threshold, nms_sigma
    0x80,0x79,0x25,0x2a,0x26,0x26,0x26,0x2b, // code info
    0xb3, // LOAD_FAST 3
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0x67, // POP_JUMP_IF_FALSE 39
    0x12,0x23, // LOAD_GLOBAL 'np'
    0x14,0x0f, // LOAD_METHOD 'array'
    0x23,0x0e, // LOAD_CONST_OBJ 14
    0x23,0x0f, // LOAD_CONST_OBJ 15
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x10, // LOAD_CONST_OBJ 16
    0x23,0x11, // LOAD_CONST_OBJ 17
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x12, // LOAD_CONST_OBJ 18
    0x23,0x13, // LOAD_CONST_OBJ 19
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x14, // LOAD_CONST_OBJ 20
    0x23,0x15, // LOAD_CONST_OBJ 21
    0x2b,0x02, // BUILD_LIST 2
    0x23,0x16, // LOAD_CONST_OBJ 22
    0x23,0x17, // LOAD_CONST_OBJ 23
    0x2b,0x02, // BUILD_LIST 2
    0x2b,0x05, // BUILD_LIST 5
    0x36,0x01, // CALL_METHOD 1
    0xc3, // STORE_FAST 3
    0x12,0x31, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x0a, // LOAD_SUPER_METHOD '__init__'
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0x36,0x04, // CALL_METHOD 4
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_darknet_YoloLC___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_darknet_YoloLC___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 80,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 14,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 6,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_postprocessing_darknet_YoloLC___init__ + 12,
        .line_info_top = fun_data_ml_postprocessing_darknet_YoloLC___init__ + 20,
        .opcodes = fun_data_ml_postprocessing_darknet_YoloLC___init__ + 20,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_darknet_YoloLC___init__ fun_data_ml_postprocessing_darknet_YoloLC___init__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_darknet_YoloLC[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_darknet_YoloLC___init__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_darknet_YoloLC = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_darknet_YoloLC,
    .children = (void *)&children_ml_postprocessing_darknet_YoloLC,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 32,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ml_postprocessing_darknet_YoloLC + 3,
        .line_info_top = fun_data_ml_postprocessing_darknet_YoloLC + 5,
        .opcodes = fun_data_ml_postprocessing_darknet_YoloLC + 6,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_postprocessing_darknet__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_darknet_YoloV2,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_darknet_YoloLC,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_darknet__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_darknet__lt_module_gt_,
    .children = (void *)&children_ml_postprocessing_darknet__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 65,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_postprocessing_darknet__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_postprocessing_darknet__lt_module_gt_ + 11,
        .opcodes = fun_data_ml_postprocessing_darknet__lt_module_gt_ + 11,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_postprocessing_darknet[51] = {
    MP_QSTR_ml_slash_postprocessing_slash_darknet_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_ml_dot_utils,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_numpy,
    MP_QSTR_ulab,
    MP_QSTR_YoloV2,
    MP_QSTR_YoloLC,
    MP_QSTR___init__,
    MP_QSTR_threshold,
    MP_QSTR_anchors,
    MP_QSTR_nms_threshold,
    MP_QSTR_nms_sigma,
    MP_QSTR_array,
    MP_QSTR_anchors_len,
    MP_QSTR___call__,
    MP_QSTR_output_shape,
    MP_QSTR_output_scale,
    MP_QSTR_reshape,
    MP_QSTR_take,
    MP_QSTR_axis,
    MP_QSTR_argmax,
    MP_QSTR_exp,
    MP_QSTR_input_shape,
    MP_QSTR_roi,
    MP_QSTR_shape,
    MP_QSTR_add_bounding_box,
    MP_QSTR_get_bounding_boxes,
    MP_QSTR_sigma,
    MP_QSTR_softmax,
    MP_QSTR_max,
    MP_QSTR_keepdims,
    MP_QSTR_sum,
    MP_QSTR_np,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_len,
    MP_QSTR_model,
    MP_QSTR_inputs,
    MP_QSTR_outputs,
    MP_QSTR_quantize,
    MP_QSTR_logit,
    MP_QSTR_dequantize,
    MP_QSTR_sigmoid,
    MP_QSTR_NMS,
    MP_QSTR_super,
    MP_QSTR_x,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_0 = {{&mp_type_float}, (mp_float_t)0.6};
#define const_obj_ml_postprocessing_darknet_0_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_0_macro ((mp_rom_obj_t)(0xbf99999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_0_macro ((mp_rom_obj_t)(0xbfe7333333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_1 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_ml_postprocessing_darknet_1_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_1_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_1_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_2 = {{&mp_type_float}, (mp_float_t)0.9883};
#define const_obj_ml_postprocessing_darknet_2_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_2_macro ((mp_rom_obj_t)(0xbffd013a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_2_macro ((mp_rom_obj_t)(0xbff3a027525460aa))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_3 = {{&mp_type_float}, (mp_float_t)3.3606};
#define const_obj_ml_postprocessing_darknet_3_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_3)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_3_macro ((mp_rom_obj_t)(0xc0d71412))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_3_macro ((mp_rom_obj_t)(0xc00ee28240b78034))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_4 = {{&mp_type_float}, (mp_float_t)2.1194};
#define const_obj_ml_postprocessing_darknet_4_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_4)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_4_macro ((mp_rom_obj_t)(0xc087a442))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_4_macro ((mp_rom_obj_t)(0xc004f487fcb923a3))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_5 = {{&mp_type_float}, (mp_float_t)5.3759};
#define const_obj_ml_postprocessing_darknet_5_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_5)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_5_macro ((mp_rom_obj_t)(0xc12c075e))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_5_macro ((mp_rom_obj_t)(0xc01980ebedfa43fe))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_6 = {{&mp_type_float}, (mp_float_t)3.052};
#define const_obj_ml_postprocessing_darknet_6_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_6)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_6_macro ((mp_rom_obj_t)(0xc0c353fa))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_6_macro ((mp_rom_obj_t)(0xc00c6a7ef9db22d1))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_7 = {{&mp_type_float}, (mp_float_t)9.133599999999999};
#define const_obj_ml_postprocessing_darknet_7_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_7)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_7_macro ((mp_rom_obj_t)(0xc192233a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_7_macro ((mp_rom_obj_t)(0xc0264467381d7dbf))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_8 = {{&mp_type_float}, (mp_float_t)5.5517};
#define const_obj_ml_postprocessing_darknet_8_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_8)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_8_macro ((mp_rom_obj_t)(0xc131a786))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_8_macro ((mp_rom_obj_t)(0xc01a34f0d844d014))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_9 = {{&mp_type_float}, (mp_float_t)9.3066};
#define const_obj_ml_postprocessing_darknet_9_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_9)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_9_macro ((mp_rom_obj_t)(0xc194e7d6))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_9_macro ((mp_rom_obj_t)(0xc0269cfaacd9e83e))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_10 = {{&mp_type_float}, (mp_float_t)9.726000000000001};
#define const_obj_ml_postprocessing_darknet_10_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_10)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_10_macro ((mp_rom_obj_t)(0xc19b9db2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_10_macro ((mp_rom_obj_t)(0xc02773b645a1cac1))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_11 = {{&mp_type_float}, (mp_float_t)11.1422};
#define const_obj_ml_postprocessing_darknet_11_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_11)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_11_macro ((mp_rom_obj_t)(0xc1b24676))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_11_macro ((mp_rom_obj_t)(0xc02a48ce703afb7f))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_13 = {{&mp_type_float}, (mp_float_t)0.5};
#define const_obj_ml_postprocessing_darknet_13_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_13)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_13_macro ((mp_rom_obj_t)(0xbf800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_13_macro ((mp_rom_obj_t)(0xbfe4000000000000))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_14 = {{&mp_type_float}, (mp_float_t)0.07602299999999999};
#define const_obj_ml_postprocessing_darknet_14_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_14)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_14_macro ((mp_rom_obj_t)(0xbe1bb1f2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_14_macro ((mp_rom_obj_t)(0xbfb7763e4abe6a33))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_15 = {{&mp_type_float}, (mp_float_t)0.258508};
#define const_obj_ml_postprocessing_darknet_15_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_15)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_15_macro ((mp_rom_obj_t)(0xbf045b2a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_15_macro ((mp_rom_obj_t)(0xbfd48b6523704791))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_16 = {{&mp_type_float}, (mp_float_t)0.163031};
#define const_obj_ml_postprocessing_darknet_16_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_16)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_16_macro ((mp_rom_obj_t)(0xbea6f19a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_16_macro ((mp_rom_obj_t)(0xbfc8de33269df97b))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_17 = {{&mp_type_float}, (mp_float_t)0.413531};
#define const_obj_ml_postprocessing_darknet_17_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_17)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_17_macro ((mp_rom_obj_t)(0xbf53ba56))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_17_macro ((mp_rom_obj_t)(0xbfde774aba387592))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_18 = {{&mp_type_float}, (mp_float_t)0.234769};
#define const_obj_ml_postprocessing_darknet_18_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_18)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_18_macro ((mp_rom_obj_t)(0xbef0674a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_18_macro ((mp_rom_obj_t)(0xbfd20ce91c8eac00))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_19 = {{&mp_type_float}, (mp_float_t)0.702585};
#define const_obj_ml_postprocessing_darknet_19_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_19)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_19_macro ((mp_rom_obj_t)(0xbfb3dc9e))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_19_macro ((mp_rom_obj_t)(0xbfea7b9389b52008))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_20 = {{&mp_type_float}, (mp_float_t)0.427054};
#define const_obj_ml_postprocessing_darknet_20_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_20)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_20_macro ((mp_rom_obj_t)(0xbf5aa6d2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_20_macro ((mp_rom_obj_t)(0xbfdf54da4ce8101f))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_21 = {{&mp_type_float}, (mp_float_t)0.715892};
#define const_obj_ml_postprocessing_darknet_21_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_21)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_21_macro ((mp_rom_obj_t)(0xbfb744b2))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_21_macro ((mp_rom_obj_t)(0xbfeae89656eefa1e))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_22 = {{&mp_type_float}, (mp_float_t)0.748154};
#define const_obj_ml_postprocessing_darknet_22_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_22)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_22_macro ((mp_rom_obj_t)(0xbfbf8706))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_22_macro ((mp_rom_obj_t)(0xbfebf0e0a84be404))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_darknet_23 = {{&mp_type_float}, (mp_float_t)0.857092};
#define const_obj_ml_postprocessing_darknet_23_macro MP_ROM_PTR(&const_obj_ml_postprocessing_darknet_23)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_darknet_23_macro ((mp_rom_obj_t)(0xbfdb6a62))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_darknet_23_macro ((mp_rom_obj_t)(0xbfef6d4c33b53932))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_postprocessing_darknet[24] = {
    const_obj_ml_postprocessing_darknet_0_macro,
    const_obj_ml_postprocessing_darknet_1_macro,
    const_obj_ml_postprocessing_darknet_2_macro,
    const_obj_ml_postprocessing_darknet_3_macro,
    const_obj_ml_postprocessing_darknet_4_macro,
    const_obj_ml_postprocessing_darknet_5_macro,
    const_obj_ml_postprocessing_darknet_6_macro,
    const_obj_ml_postprocessing_darknet_7_macro,
    const_obj_ml_postprocessing_darknet_8_macro,
    const_obj_ml_postprocessing_darknet_9_macro,
    const_obj_ml_postprocessing_darknet_10_macro,
    const_obj_ml_postprocessing_darknet_11_macro,
    MP_ROM_PTR(&mp_const_empty_tuple_obj),
    const_obj_ml_postprocessing_darknet_13_macro,
    const_obj_ml_postprocessing_darknet_14_macro,
    const_obj_ml_postprocessing_darknet_15_macro,
    const_obj_ml_postprocessing_darknet_16_macro,
    const_obj_ml_postprocessing_darknet_17_macro,
    const_obj_ml_postprocessing_darknet_18_macro,
    const_obj_ml_postprocessing_darknet_19_macro,
    const_obj_ml_postprocessing_darknet_20_macro,
    const_obj_ml_postprocessing_darknet_21_macro,
    const_obj_ml_postprocessing_darknet_22_macro,
    const_obj_ml_postprocessing_darknet_23_macro,
};

static const mp_frozen_module_t frozen_module_ml_postprocessing_darknet = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_postprocessing_darknet,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_postprocessing_darknet,
    },
    .proto_fun = &proto_fun_ml_postprocessing_darknet__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_postprocessing_edgeimpulse
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\postprocessing\edgeimpulse.mpy
// - frozen file name: ml/postprocessing/edgeimpulse.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/postprocessing/edgeimpulse.py, scope ml_postprocessing_edgeimpulse__lt_module_gt_
static const byte fun_data_ml_postprocessing_edgeimpulse__lt_module_gt_[63] = {
    0x10,0x12, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x28,0x2c,0x6c,0x60,0x89,0x39, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'ml.utils'
    0x69, // IMPORT_STAR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'micropython'
    0x1c,0x04, // IMPORT_FROM 'const'
    0x16,0x04, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'numpy'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x07, // IMPORT_NAME 'ulab'
    0x1c,0x06, // IMPORT_FROM 'numpy'
    0x16,0x21, // STORE_NAME 'np'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x08, // LOAD_CONST_STRING 'Fomo'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x08, // STORE_NAME 'Fomo'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x09, // LOAD_CONST_STRING 'YoloPro'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x09, // STORE_NAME 'YoloPro'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_edgeimpulse__lt_module_gt_
// frozen bytecode for file ml/postprocessing/edgeimpulse.py, scope ml_postprocessing_edgeimpulse_Fomo
static const byte fun_data_ml_postprocessing_edgeimpulse_Fomo[40] = {
    0x20,0x0e, // prelude
    0x08, // names: Fomo
    0x88,0x25,0x40,0x26,0x8b,0x07, // code info
    0x11,0x22, // LOAD_NAME '__name__'
    0x16,0x23, // STORE_NAME '__module__'
    0x10,0x08, // LOAD_CONST_STRING 'Fomo'
    0x16,0x24, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x2a,0x05, // BUILD_TUPLE 5
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0a, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x10, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_edgeimpulse_Fomo
// frozen bytecode for file ml/postprocessing/edgeimpulse.py, scope ml_postprocessing_edgeimpulse_Fomo___init__
static const byte fun_data_ml_postprocessing_edgeimpulse_Fomo___init__[40] = {
    0xba,0x85,0x80,0x01,0x1a, // prelude
    0x0a,0x25,0x0b,0x0c,0x0d,0x0e,0x0f, // names: __init__, self, threshold, w_scale, h_scale, nms_threshold, nms_sigma
    0x80,0x29,0x24,0x24,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x0c, // STORE_ATTR 'w_scale'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x0d, // STORE_ATTR 'h_scale'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x0e, // STORE_ATTR 'nms_threshold'
    0xb5, // LOAD_FAST 5
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_edgeimpulse_Fomo___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_edgeimpulse_Fomo___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 40,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 8,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 6,
        .n_kwonly_args = 0,
        .n_def_pos_args = 5,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_postprocessing_edgeimpulse_Fomo___init__ + 12,
        .line_info_top = fun_data_ml_postprocessing_edgeimpulse_Fomo___init__ + 18,
        .opcodes = fun_data_ml_postprocessing_edgeimpulse_Fomo___init__ + 18,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_edgeimpulse_Fomo___init__ fun_data_ml_postprocessing_edgeimpulse_Fomo___init__[0]
#endif

// child of ml_postprocessing_edgeimpulse_Fomo
// frozen bytecode for file ml/postprocessing/edgeimpulse.py, scope ml_postprocessing_edgeimpulse_Fomo___call__
static const byte fun_data_ml_postprocessing_edgeimpulse_Fomo___call__[391] = {
    0xc8,0x24,0x42, // prelude
    0x10,0x25,0x26,0x27,0x28, // names: __call__, self, model, inputs, outputs
    0x80,0x30,0x2b,0x26,0x69,0x6e,0x2d,0x2f,0x27,0x63,0x72,0x24,0x64,0x77,0x7a,0x28,0x28,0x35,0x75,0x2f,0x2a,0x2a,0x2a,0x4a,0x2f,0x2b,0x1f,0x2e, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x11, // LOAD_ATTR 'output_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0xb1, // LOAD_FAST 1
    0x13,0x12, // LOAD_ATTR 'output_scale'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc8, // STORE_FAST 8
    0x12,0x29, // LOAD_GLOBAL 'quantize'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'threshold'
    0x34,0x02, // CALL_FUNCTION 2
    0xc9, // STORE_FAST 9
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x14,0x13, // LOAD_METHOD 'reshape'
    0xb5, // LOAD_FAST 5
    0xb6, // LOAD_FAST 6
    0xf4, // BINARY_OP 29 __mul__
    0xb7, // LOAD_FAST 7
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xca, // STORE_FAST 10
    0xba, // LOAD_FAST 10
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xcb, // STORE_FAST 11
    0x12,0x0b, // LOAD_GLOBAL 'threshold'
    0xbb, // LOAD_FAST 11
    0xb9, // LOAD_FAST 9
    0xb8, // LOAD_FAST 8
    0x10,0x14, // LOAD_CONST_STRING 'find_max'
    0x52, // LOAD_CONST_TRUE
    0x10,0x15, // LOAD_CONST_STRING 'find_max_axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x84,0x03, // CALL_FUNCTION 515
    0xcb, // STORE_FAST 11
    0x12,0x2a, // LOAD_GLOBAL 'len'
    0xbb, // LOAD_FAST 11
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x63, // RETURN_VALUE
    0x12,0x2b, // LOAD_GLOBAL 'dequantize'
    0xb1, // LOAD_FAST 1
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x16, // LOAD_METHOD 'take'
    0xba, // LOAD_FAST 10
    0xbb, // LOAD_FAST 11
    0x10,0x17, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x34,0x02, // CALL_FUNCTION 2
    0xcc, // STORE_FAST 12
    0xbb, // LOAD_FAST 11
    0xb6, // LOAD_FAST 6
    0xf6, // BINARY_OP 31 __floordiv__
    0xcd, // STORE_FAST 13
    0xbb, // LOAD_FAST 11
    0xb6, // LOAD_FAST 6
    0xf8, // BINARY_OP 33 __mod__
    0xce, // STORE_FAST 14
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x18, // LOAD_METHOD 'max'
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x10,0x17, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0xcf, // STORE_FAST 15
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x19, // LOAD_METHOD 'argmax'
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x10,0x17, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0x81, // LOAD_CONST_SMALL_INT 1
    0xf2, // BINARY_OP 27 __add__
    0x26,0x10, // STORE_FAST_N 16
    0xbe, // LOAD_FAST 14
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xf2, // BINARY_OP 27 __add__
    0xb6, // LOAD_FAST 6
    0xf7, // BINARY_OP 32 __truediv__
    0x26,0x11, // STORE_FAST_N 17
    0xbd, // LOAD_FAST 13
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0xf7, // BINARY_OP 32 __truediv__
    0x26,0x12, // STORE_FAST_N 18
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x1a, // LOAD_METHOD 'full'
    0x12,0x2a, // LOAD_GLOBAL 'len'
    0xbe, // LOAD_FAST 14
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'w_scale'
    0xb6, // LOAD_FAST 6
    0xf7, // BINARY_OP 32 __truediv__
    0x36,0x02, // CALL_METHOD 2
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x13, // STORE_FAST_N 19
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x1a, // LOAD_METHOD 'full'
    0x12,0x2a, // LOAD_GLOBAL 'len'
    0xbd, // LOAD_FAST 13
    0x34,0x01, // CALL_FUNCTION 1
    0xb0, // LOAD_FAST 0
    0x13,0x0d, // LOAD_ATTR 'h_scale'
    0xb5, // LOAD_FAST 5
    0xf7, // BINARY_OP 32 __truediv__
    0x36,0x02, // CALL_METHOD 2
    0x23,0x06, // LOAD_CONST_OBJ 6
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x14, // STORE_FAST_N 20
    0xb1, // LOAD_FAST 1
    0x13,0x1b, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0x26,0x15, // STORE_FAST_N 21
    0x26,0x16, // STORE_FAST_N 22
    0x26,0x17, // STORE_FAST_N 23
    0x26,0x18, // STORE_FAST_N 24
    0x24,0x11, // LOAD_FAST_N 17
    0x24,0x13, // LOAD_FAST_N 19
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x17, // LOAD_FAST_N 23
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x19, // STORE_FAST_N 25
    0x24,0x12, // LOAD_FAST_N 18
    0x24,0x14, // LOAD_FAST_N 20
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x16, // LOAD_FAST_N 22
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1a, // STORE_FAST_N 26
    0x24,0x11, // LOAD_FAST_N 17
    0x24,0x13, // LOAD_FAST_N 19
    0xf2, // BINARY_OP 27 __add__
    0x24,0x17, // LOAD_FAST_N 23
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1b, // STORE_FAST_N 27
    0x24,0x12, // LOAD_FAST_N 18
    0x24,0x14, // LOAD_FAST_N 20
    0xf2, // BINARY_OP 27 __add__
    0x24,0x16, // LOAD_FAST_N 22
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1c, // STORE_FAST_N 28
    0x12,0x2c, // LOAD_GLOBAL 'NMS'
    0x24,0x17, // LOAD_FAST_N 23
    0x24,0x16, // LOAD_FAST_N 22
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x1c, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0x26,0x1d, // STORE_FAST_N 29
    0xbc, // LOAD_FAST 12
    0x13,0x1d, // LOAD_ATTR 'shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x69, // JUMP 41
    0x57, // DUP_TOP
    0x26,0x1e, // STORE_FAST_N 30
    0x24,0x1d, // LOAD_FAST_N 29
    0x14,0x1e, // LOAD_METHOD 'add_bounding_box'
    0x24,0x19, // LOAD_FAST_N 25
    0x24,0x1e, // LOAD_FAST_N 30
    0x55, // LOAD_SUBSCR
    0x24,0x1a, // LOAD_FAST_N 26
    0x24,0x1e, // LOAD_FAST_N 30
    0x55, // LOAD_SUBSCR
    0x24,0x1b, // LOAD_FAST_N 27
    0x24,0x1e, // LOAD_FAST_N 30
    0x55, // LOAD_SUBSCR
    0x24,0x1c, // LOAD_FAST_N 28
    0x24,0x1e, // LOAD_FAST_N 30
    0x55, // LOAD_SUBSCR
    0xbf, // LOAD_FAST 15
    0x24,0x1e, // LOAD_FAST_N 30
    0x55, // LOAD_SUBSCR
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x1e, // LOAD_FAST_N 30
    0x55, // LOAD_SUBSCR
    0x36,0x06, // CALL_METHOD 6
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x12, // POP_JUMP_IF_TRUE -46
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x24,0x1d, // LOAD_FAST_N 29
    0x14,0x1f, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0b, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR 'nms_threshold'
    0x10,0x20, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_edgeimpulse_Fomo___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_edgeimpulse_Fomo___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 391,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 42,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_ml_postprocessing_edgeimpulse_Fomo___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_edgeimpulse_Fomo___call__ + 36,
        .opcodes = fun_data_ml_postprocessing_edgeimpulse_Fomo___call__ + 36,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_edgeimpulse_Fomo___call__ fun_data_ml_postprocessing_edgeimpulse_Fomo___call__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_edgeimpulse_Fomo[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_edgeimpulse_Fomo___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_edgeimpulse_Fomo___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_edgeimpulse_Fomo = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_edgeimpulse_Fomo,
    .children = (void *)&children_ml_postprocessing_edgeimpulse_Fomo,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 40,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_ml_postprocessing_edgeimpulse_Fomo + 3,
        .line_info_top = fun_data_ml_postprocessing_edgeimpulse_Fomo + 9,
        .opcodes = fun_data_ml_postprocessing_edgeimpulse_Fomo + 9,
    },
    #endif
    #endif
};

// child of ml_postprocessing_edgeimpulse__lt_module_gt_
// frozen bytecode for file ml/postprocessing/edgeimpulse.py, scope ml_postprocessing_edgeimpulse_YoloPro
static const byte fun_data_ml_postprocessing_edgeimpulse_YoloPro[36] = {
    0x10,0x0e, // prelude
    0x09, // names: YoloPro
    0x88,0x5e,0x60,0x60,0x6d,0x40, // code info
    0x11,0x22, // LOAD_NAME '__name__'
    0x16,0x23, // STORE_NAME '__module__'
    0x10,0x09, // LOAD_CONST_STRING 'YoloPro'
    0x16,0x24, // STORE_NAME '__qualname__'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0a, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x10, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_edgeimpulse_YoloPro
// frozen bytecode for file ml/postprocessing/edgeimpulse.py, scope ml_postprocessing_edgeimpulse_YoloPro___init__
static const byte fun_data_ml_postprocessing_edgeimpulse_YoloPro___init__[27] = {
    0xa8,0x85,0x01,0x12, // prelude
    0x0a,0x25,0x0b,0x0e,0x0f, // names: __init__, self, threshold, nms_threshold, nms_sigma
    0x80,0x65,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x0e, // STORE_ATTR 'nms_threshold'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_edgeimpulse_YoloPro___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_edgeimpulse_YoloPro___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_postprocessing_edgeimpulse_YoloPro___init__ + 9,
        .line_info_top = fun_data_ml_postprocessing_edgeimpulse_YoloPro___init__ + 13,
        .opcodes = fun_data_ml_postprocessing_edgeimpulse_YoloPro___init__ + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_edgeimpulse_YoloPro___init__ fun_data_ml_postprocessing_edgeimpulse_YoloPro___init__[0]
#endif

// child of ml_postprocessing_edgeimpulse_YoloPro
// frozen bytecode for file ml/postprocessing/edgeimpulse.py, scope ml_postprocessing_edgeimpulse_YoloPro___call__
static const byte fun_data_ml_postprocessing_edgeimpulse_YoloPro___call__[325] = {
    0x90,0x24,0x36, // prelude
    0x10,0x25,0x26,0x27,0x28, // names: __call__, self, model, inputs, outputs
    0x80,0x6a,0x2a,0x26,0x69,0x6e,0x2d,0x2f,0x27,0x63,0x72,0x77,0x77,0x2d,0x2e,0x2d,0x2e,0x4d,0x2e,0x2b,0x1f,0x2d, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x11, // LOAD_ATTR 'output_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xb1, // LOAD_FAST 1
    0x13,0x12, // LOAD_ATTR 'output_scale'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc7, // STORE_FAST 7
    0x12,0x29, // LOAD_GLOBAL 'quantize'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'threshold'
    0x34,0x02, // CALL_FUNCTION 2
    0xc8, // STORE_FAST 8
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x14,0x13, // LOAD_METHOD 'reshape'
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0xf4, // BINARY_OP 29 __mul__
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xca, // STORE_FAST 10
    0x12,0x0b, // LOAD_GLOBAL 'threshold'
    0xba, // LOAD_FAST 10
    0xb8, // LOAD_FAST 8
    0xb7, // LOAD_FAST 7
    0x10,0x14, // LOAD_CONST_STRING 'find_max'
    0x52, // LOAD_CONST_TRUE
    0x10,0x15, // LOAD_CONST_STRING 'find_max_axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x84,0x03, // CALL_FUNCTION 515
    0xca, // STORE_FAST 10
    0x12,0x2a, // LOAD_GLOBAL 'len'
    0xba, // LOAD_FAST 10
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x63, // RETURN_VALUE
    0x12,0x2b, // LOAD_GLOBAL 'dequantize'
    0xb1, // LOAD_FAST 1
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x16, // LOAD_METHOD 'take'
    0xb9, // LOAD_FAST 9
    0xba, // LOAD_FAST 10
    0x10,0x17, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x34,0x02, // CALL_FUNCTION 2
    0xcb, // STORE_FAST 11
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x18, // LOAD_METHOD 'max'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x10,0x17, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0xcc, // STORE_FAST 12
    0x12,0x21, // LOAD_GLOBAL 'np'
    0x14,0x19, // LOAD_METHOD 'argmax'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x10,0x17, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0xcd, // STORE_FAST 13
    0xb1, // LOAD_FAST 1
    0x13,0x1b, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xce, // STORE_FAST 14
    0xcf, // STORE_FAST 15
    0x26,0x10, // STORE_FAST_N 16
    0x26,0x11, // STORE_FAST_N 17
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x24,0x10, // LOAD_FAST_N 16
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x12, // STORE_FAST_N 18
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xbf, // LOAD_FAST 15
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x13, // STORE_FAST_N 19
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x24,0x10, // LOAD_FAST_N 16
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x14, // STORE_FAST_N 20
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xbf, // LOAD_FAST 15
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x15, // STORE_FAST_N 21
    0x12,0x2c, // LOAD_GLOBAL 'NMS'
    0x24,0x10, // LOAD_FAST_N 16
    0xbf, // LOAD_FAST 15
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x1c, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0x26,0x16, // STORE_FAST_N 22
    0xbb, // LOAD_FAST 11
    0x13,0x1d, // LOAD_ATTR 'shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x68, // JUMP 40
    0x57, // DUP_TOP
    0x26,0x17, // STORE_FAST_N 23
    0x24,0x16, // LOAD_FAST_N 22
    0x14,0x1e, // LOAD_METHOD 'add_bounding_box'
    0x24,0x12, // LOAD_FAST_N 18
    0x24,0x17, // LOAD_FAST_N 23
    0x55, // LOAD_SUBSCR
    0x24,0x13, // LOAD_FAST_N 19
    0x24,0x17, // LOAD_FAST_N 23
    0x55, // LOAD_SUBSCR
    0x24,0x14, // LOAD_FAST_N 20
    0x24,0x17, // LOAD_FAST_N 23
    0x55, // LOAD_SUBSCR
    0x24,0x15, // LOAD_FAST_N 21
    0x24,0x17, // LOAD_FAST_N 23
    0x55, // LOAD_SUBSCR
    0xbc, // LOAD_FAST 12
    0x24,0x17, // LOAD_FAST_N 23
    0x55, // LOAD_SUBSCR
    0xbd, // LOAD_FAST 13
    0x24,0x17, // LOAD_FAST_N 23
    0x55, // LOAD_SUBSCR
    0x36,0x06, // CALL_METHOD 6
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x13, // POP_JUMP_IF_TRUE -45
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x24,0x16, // LOAD_FAST_N 22
    0x14,0x1f, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0b, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x0e, // LOAD_ATTR 'nms_threshold'
    0x10,0x20, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_edgeimpulse_YoloPro___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_edgeimpulse_YoloPro___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 325,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 35,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 16,
        .line_info = fun_data_ml_postprocessing_edgeimpulse_YoloPro___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_edgeimpulse_YoloPro___call__ + 30,
        .opcodes = fun_data_ml_postprocessing_edgeimpulse_YoloPro___call__ + 30,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_edgeimpulse_YoloPro___call__ fun_data_ml_postprocessing_edgeimpulse_YoloPro___call__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_edgeimpulse_YoloPro[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_edgeimpulse_YoloPro___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_edgeimpulse_YoloPro___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_edgeimpulse_YoloPro = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_edgeimpulse_YoloPro,
    .children = (void *)&children_ml_postprocessing_edgeimpulse_YoloPro,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 36,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ml_postprocessing_edgeimpulse_YoloPro + 3,
        .line_info_top = fun_data_ml_postprocessing_edgeimpulse_YoloPro + 9,
        .opcodes = fun_data_ml_postprocessing_edgeimpulse_YoloPro + 9,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_postprocessing_edgeimpulse__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_edgeimpulse_Fomo,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_edgeimpulse_YoloPro,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_edgeimpulse__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_edgeimpulse__lt_module_gt_,
    .children = (void *)&children_ml_postprocessing_edgeimpulse__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 63,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_postprocessing_edgeimpulse__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_postprocessing_edgeimpulse__lt_module_gt_ + 11,
        .opcodes = fun_data_ml_postprocessing_edgeimpulse__lt_module_gt_ + 11,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_postprocessing_edgeimpulse[45] = {
    MP_QSTR_ml_slash_postprocessing_slash_edgeimpulse_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_ml_dot_utils,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_numpy,
    MP_QSTR_ulab,
    MP_QSTR_Fomo,
    MP_QSTR_YoloPro,
    MP_QSTR___init__,
    MP_QSTR_threshold,
    MP_QSTR_w_scale,
    MP_QSTR_h_scale,
    MP_QSTR_nms_threshold,
    MP_QSTR_nms_sigma,
    MP_QSTR___call__,
    MP_QSTR_output_shape,
    MP_QSTR_output_scale,
    MP_QSTR_reshape,
    MP_QSTR_find_max,
    MP_QSTR_find_max_axis,
    MP_QSTR_take,
    MP_QSTR_axis,
    MP_QSTR_max,
    MP_QSTR_argmax,
    MP_QSTR_full,
    MP_QSTR_input_shape,
    MP_QSTR_roi,
    MP_QSTR_shape,
    MP_QSTR_add_bounding_box,
    MP_QSTR_get_bounding_boxes,
    MP_QSTR_sigma,
    MP_QSTR_np,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_model,
    MP_QSTR_inputs,
    MP_QSTR_outputs,
    MP_QSTR_quantize,
    MP_QSTR_len,
    MP_QSTR_dequantize,
    MP_QSTR_NMS,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_edgeimpulse_0 = {{&mp_type_float}, (mp_float_t)0.4};
#define const_obj_ml_postprocessing_edgeimpulse_0_macro MP_ROM_PTR(&const_obj_ml_postprocessing_edgeimpulse_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_edgeimpulse_0_macro ((mp_rom_obj_t)(0xbf4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_edgeimpulse_0_macro ((mp_rom_obj_t)(0xbfdd99999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_edgeimpulse_1 = {{&mp_type_float}, (mp_float_t)1.414214};
#define const_obj_ml_postprocessing_edgeimpulse_1_macro MP_ROM_PTR(&const_obj_ml_postprocessing_edgeimpulse_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_edgeimpulse_1_macro ((mp_rom_obj_t)(0xc03504f6))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_edgeimpulse_1_macro ((mp_rom_obj_t)(0xbffaa09edbf8b9bb))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_edgeimpulse_2 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_ml_postprocessing_edgeimpulse_2_macro MP_ROM_PTR(&const_obj_ml_postprocessing_edgeimpulse_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_edgeimpulse_2_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_edgeimpulse_2_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_edgeimpulse_3 = {{&mp_type_float}, (mp_float_t)0.001};
#define const_obj_ml_postprocessing_edgeimpulse_3_macro MP_ROM_PTR(&const_obj_ml_postprocessing_edgeimpulse_3)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_edgeimpulse_3_macro ((mp_rom_obj_t)(0xbb03126e))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_edgeimpulse_3_macro ((mp_rom_obj_t)(0xbf54624dd2f1a9fc))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_edgeimpulse_4 = {{&mp_type_float}, (mp_float_t)0.6};
#define const_obj_ml_postprocessing_edgeimpulse_4_macro MP_ROM_PTR(&const_obj_ml_postprocessing_edgeimpulse_4)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_edgeimpulse_4_macro ((mp_rom_obj_t)(0xbf99999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_edgeimpulse_4_macro ((mp_rom_obj_t)(0xbfe7333333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_edgeimpulse_6 = {{&mp_type_float}, (mp_float_t)0.5};
#define const_obj_ml_postprocessing_edgeimpulse_6_macro MP_ROM_PTR(&const_obj_ml_postprocessing_edgeimpulse_6)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_edgeimpulse_6_macro ((mp_rom_obj_t)(0xbf800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_edgeimpulse_6_macro ((mp_rom_obj_t)(0xbfe4000000000000))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_postprocessing_edgeimpulse[7] = {
    const_obj_ml_postprocessing_edgeimpulse_0_macro,
    const_obj_ml_postprocessing_edgeimpulse_1_macro,
    const_obj_ml_postprocessing_edgeimpulse_2_macro,
    const_obj_ml_postprocessing_edgeimpulse_3_macro,
    const_obj_ml_postprocessing_edgeimpulse_4_macro,
    MP_ROM_PTR(&mp_const_empty_tuple_obj),
    const_obj_ml_postprocessing_edgeimpulse_6_macro,
};

static const mp_frozen_module_t frozen_module_ml_postprocessing_edgeimpulse = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_postprocessing_edgeimpulse,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_postprocessing_edgeimpulse,
    },
    .proto_fun = &proto_fun_ml_postprocessing_edgeimpulse__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_postprocessing_mediapipe
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\postprocessing\mediapipe.mpy
// - frozen file name: ml/postprocessing/mediapipe.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe__lt_module_gt_
static const byte fun_data_ml_postprocessing_mediapipe__lt_module_gt_[111] = {
    0x18,0x22, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x28,0x2c,0x6c,0x60,0x89,0x5d,0x8b,0x07,0x8b,0x07,0x89,0x22,0x89,0x20, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'ml.utils'
    0x69, // IMPORT_STAR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'micropython'
    0x1c,0x04, // IMPORT_FROM 'const'
    0x16,0x04, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'numpy'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x07, // IMPORT_NAME 'ulab'
    0x1c,0x06, // IMPORT_FROM 'numpy'
    0x16,0x2c, // STORE_NAME 'np'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x08, // LOAD_CONST_STRING 'mediapipe_detection_postprocess'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x08, // STORE_NAME 'mediapipe_detection_postprocess'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x09, // LOAD_CONST_STRING 'BlazeFace'
    0x11,0x08, // LOAD_NAME 'mediapipe_detection_postprocess'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x09, // STORE_NAME 'BlazeFace'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x02, // MAKE_FUNCTION 2
    0x10,0x0a, // LOAD_CONST_STRING 'BlazePalm'
    0x11,0x08, // LOAD_NAME 'mediapipe_detection_postprocess'
    0x34,0x03, // CALL_FUNCTION 3
    0x16,0x0a, // STORE_NAME 'BlazePalm'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x03, // MAKE_FUNCTION 3
    0x10,0x0b, // LOAD_CONST_STRING 'HandLandmarks'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0b, // STORE_NAME 'HandLandmarks'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x04, // MAKE_FUNCTION 4
    0x10,0x0c, // LOAD_CONST_STRING 'FaceLandmarks'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0c, // STORE_NAME 'FaceLandmarks'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x05, // MAKE_FUNCTION 5
    0x10,0x0d, // LOAD_CONST_STRING 'MoveNet'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x0d, // STORE_NAME 'MoveNet'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe__lt_module_gt_
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_mediapipe_detection_postprocess
static const byte fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess[49] = {
    0x30,0x14, // prelude
    0x08, // names: mediapipe_detection_postprocess
    0x88,0x25,0x60,0x60,0x28,0x8b,0x18,0x84,0x0e, // code info
    0x11,0x2d, // LOAD_NAME '__name__'
    0x16,0x2e, // STORE_NAME '__module__'
    0x10,0x08, // LOAD_CONST_STRING 'mediapipe_detection_postprocess'
    0x16,0x2f, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2b,0x00, // BUILD_LIST 0
    0x2b,0x00, // BUILD_LIST 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x07, // BUILD_TUPLE 7
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0e, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x16, // STORE_NAME '__call__'
    0x32,0x02, // MAKE_FUNCTION 2
    0x16,0x1a, // STORE_NAME 'detection_post_process'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe_mediapipe_detection_postprocess
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__
static const byte fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__[190] = {
    0xf0,0x91,0x85,0x01,0x3c, // prelude
    0x0e,0x30,0x0f,0x10,0x25,0x11,0x12,0x14,0x15, // names: __init__, self, threshold, anchors, anchor_grid, scores, cords, nms_threshold, nms_sigma
    0x80,0x2d,0x24,0x24,0x24,0x44,0x28,0x2b,0x2d,0x42,0x28,0x27,0x27,0x26,0x27,0x27,0x29,0x29,0x1f,0x44,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'anchors'
    0xb4, // LOAD_FAST 4
    0xb0, // LOAD_FAST 0
    0x18,0x11, // STORE_ATTR 'scores'
    0xb5, // LOAD_FAST 5
    0xb0, // LOAD_FAST 0
    0x18,0x12, // STORE_ATTR 'cords'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors'
    0x51, // LOAD_CONST_NONE
    0xde, // BINARY_OP 7 <is>
    0x44,0xf9,0x80, // POP_JUMP_IF_FALSE 121
    0x12,0x31, // LOAD_GLOBAL 'sum'
    0x32,0x00, // MAKE_FUNCTION 0
    0xb3, // LOAD_FAST 3
    0x5e, // GET_ITER
    0x34,0x01, // CALL_FUNCTION 1
    0x34,0x01, // CALL_FUNCTION 1
    0xc8, // STORE_FAST 8
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x13, // LOAD_METHOD 'empty'
    0xb8, // LOAD_FAST 8
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xb0, // LOAD_FAST 0
    0x18,0x10, // STORE_ATTR 'anchors'
    0x80, // LOAD_CONST_SMALL_INT 0
    0xc9, // STORE_FAST 9
    0xb3, // LOAD_FAST 3
    0x5f, // GET_ITER_STACK
    0x4b,0x5b, // FOR_ITER 91
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xca, // STORE_FAST 10
    0xcb, // STORE_FAST 11
    0xba, // LOAD_FAST 10
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0xc7,0x80, // JUMP 71
    0x57, // DUP_TOP
    0xcc, // STORE_FAST 12
    0xbc, // LOAD_FAST 12
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf2, // BINARY_OP 27 __add__
    0xba, // LOAD_FAST 10
    0xf7, // BINARY_OP 32 __truediv__
    0xcd, // STORE_FAST 13
    0xba, // LOAD_FAST 10
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x71, // JUMP 49
    0x57, // DUP_TOP
    0xce, // STORE_FAST 14
    0xbe, // LOAD_FAST 14
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf2, // BINARY_OP 27 __add__
    0xba, // LOAD_FAST 10
    0xf7, // BINARY_OP 32 __truediv__
    0xcf, // STORE_FAST 15
    0xbb, // LOAD_FAST 11
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x5b, // JUMP 27
    0x57, // DUP_TOP
    0x26,0x10, // STORE_FAST_N 16
    0xbf, // LOAD_FAST 15
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors'
    0xb9, // LOAD_FAST 9
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xbd, // LOAD_FAST 13
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors'
    0xb9, // LOAD_FAST 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xb9, // LOAD_FAST 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0xc9, // STORE_FAST 9
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x20, // POP_JUMP_IF_TRUE -32
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x0a, // POP_JUMP_IF_TRUE -54
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0xb3,0x7f, // POP_JUMP_IF_TRUE -77
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x42,0xa3,0x7f, // JUMP -93
    0xb6, // LOAD_FAST 6
    0xb0, // LOAD_FAST 0
    0x18,0x14, // STORE_ATTR 'nms_threshold'
    0xb7, // LOAD_FAST 7
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_
static const byte fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_[28] = {
    0xc1,0x40,0x08, // prelude
    0x2b,0x02, // names: <genexpr>, *
    0x80,0x33, // code info
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x53, // LOAD_NULL
    0x53, // LOAD_NULL
    0x4b,0x0d, // FOR_ITER 13
    0x30,0x02, // UNPACK_SEQUENCE 2
    0xc1, // STORE_FAST 1
    0xc2, // STORE_FAST 2
    0xb1, // LOAD_FAST 1
    0xb1, // LOAD_FAST 1
    0xf4, // BINARY_OP 29 __mul__
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x67, // YIELD_VALUE
    0x59, // POP_TOP
    0x42,0x31, // JUMP -15
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 1,
    .fun_data = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 28,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 9,
        .n_exc_stack = 0,
        .scope_flags = 1,
        .n_pos_args = 1,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 43,
        .line_info = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_ + 5,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_ + 7,
        .opcodes = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_ + 7,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_ fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init____lt_genexpr_gt_,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__,
    .children = (void *)&children_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 190,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 31,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 8,
        .n_kwonly_args = 0,
        .n_def_pos_args = 7,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__ + 14,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__ + 35,
        .opcodes = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__ + 35,
    },
    #endif
    #endif
};

// child of ml_postprocessing_mediapipe_mediapipe_detection_postprocess
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__
static const byte fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__[145] = {
    0xb8,0x14,0x1e, // prelude
    0x16,0x30,0x32,0x33,0x34, // names: __call__, self, model, inputs, outputs
    0x80,0x45,0x2b,0x2c,0x48,0x33,0x4e,0x2b,0x33,0x4e, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x17, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0x12,0x35, // LOAD_GLOBAL 'NMS'
    0xb6, // LOAD_FAST 6
    0xb5, // LOAD_FAST 5
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x18, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0xc8, // STORE_FAST 8
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x19, // LOAD_ATTR 'shape'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xc9, // STORE_FAST 9
    0xb0, // LOAD_FAST 0
    0x14,0x1a, // LOAD_METHOD 'detection_post_process'
    0xb5, // LOAD_FAST 5
    0xb6, // LOAD_FAST 6
    0xb8, // LOAD_FAST 8
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'scores'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'cords'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors'
    0x51, // LOAD_CONST_NONE
    0xb9, // LOAD_FAST 9
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x0a, // CALL_METHOD 10
    0x59, // POP_TOP
    0xb9, // LOAD_FAST 9
    0x12,0x36, // LOAD_GLOBAL 'len'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors'
    0x34,0x01, // CALL_FUNCTION 1
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x61, // POP_JUMP_IF_FALSE 33
    0xb0, // LOAD_FAST 0
    0x14,0x1a, // LOAD_METHOD 'detection_post_process'
    0xb5, // LOAD_FAST 5
    0xb6, // LOAD_FAST 6
    0xb8, // LOAD_FAST 8
    0xb1, // LOAD_FAST 1
    0xb2, // LOAD_FAST 2
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x13,0x11, // LOAD_ATTR 'scores'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x12, // LOAD_ATTR 'cords'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x10, // LOAD_ATTR 'anchors'
    0xb9, // LOAD_FAST 9
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x55, // LOAD_SUBSCR
    0x36,0x0a, // CALL_METHOD 10
    0x59, // POP_TOP
    0xb8, // LOAD_FAST 8
    0x14,0x1b, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0f, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'nms_threshold'
    0x10,0x1c, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 145,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 24,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__ + 18,
        .opcodes = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__ + 18,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__ fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__[0]
#endif

// child of ml_postprocessing_mediapipe_mediapipe_detection_postprocess
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process
static const byte fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process[521] = {
    0x8b,0xb0,0x04,0x58, // prelude
    0x1a,0x30,0x37,0x38,0x39,0x32,0x33,0x34,0x3a,0x3b,0x3c,0x10, // names: detection_post_process, self, ih, iw, nms, model, inputs, outputs, score_idx, cords_idx, t, anchors
    0x80,0x53,0x2a,0x26,0x6f,0x2d,0x29,0x28,0x63,0x2f,0x71,0x2d,0x32,0x79,0x6f,0x2c,0x2c,0x31,0x31,0x31,0x71,0x28,0x33,0x1f,0x2d,0x1f,0x6d,0x29,0x29,0x29,0x49,0x2c, // code info
    0xb4, // LOAD_FAST 4
    0x13,0x1d, // LOAD_ATTR 'output_shape'
    0xb7, // LOAD_FAST 7
    0x55, // LOAD_SUBSCR
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xcb, // STORE_FAST 11
    0xcc, // STORE_FAST 12
    0xcd, // STORE_FAST 13
    0xb4, // LOAD_FAST 4
    0x13,0x1e, // LOAD_ATTR 'output_scale'
    0xb7, // LOAD_FAST 7
    0x55, // LOAD_SUBSCR
    0xce, // STORE_FAST 14
    0x12,0x3d, // LOAD_GLOBAL 'quantize'
    0xb4, // LOAD_FAST 4
    0x12,0x3e, // LOAD_GLOBAL 'logit'
    0xb9, // LOAD_FAST 9
    0x34,0x01, // CALL_FUNCTION 1
    0x10,0x1f, // LOAD_CONST_STRING 'index'
    0xb7, // LOAD_FAST 7
    0x34,0x82,0x02, // CALL_FUNCTION 258
    0xc9, // STORE_FAST 9
    0xb6, // LOAD_FAST 6
    0xb7, // LOAD_FAST 7
    0x55, // LOAD_SUBSCR
    0x14,0x20, // LOAD_METHOD 'reshape'
    0xbb, // LOAD_FAST 11
    0xbc, // LOAD_FAST 12
    0xf4, // BINARY_OP 29 __mul__
    0xbd, // LOAD_FAST 13
    0xf4, // BINARY_OP 29 __mul__
    0x36,0x01, // CALL_METHOD 1
    0xcf, // STORE_FAST 15
    0x12,0x0f, // LOAD_GLOBAL 'threshold'
    0xbf, // LOAD_FAST 15
    0xb9, // LOAD_FAST 9
    0xbe, // LOAD_FAST 14
    0x34,0x03, // CALL_FUNCTION 3
    0x26,0x10, // STORE_FAST_N 16
    0x12,0x36, // LOAD_GLOBAL 'len'
    0x24,0x10, // LOAD_FAST_N 16
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x63, // RETURN_VALUE
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x21, // LOAD_METHOD 'take'
    0xbf, // LOAD_FAST 15
    0x24,0x10, // LOAD_FAST_N 16
    0x10,0x22, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x26,0x11, // STORE_FAST_N 17
    0x12,0x3f, // LOAD_GLOBAL 'sigmoid'
    0x12,0x40, // LOAD_GLOBAL 'dequantize'
    0xb4, // LOAD_FAST 4
    0x24,0x11, // LOAD_FAST_N 17
    0x10,0x1f, // LOAD_CONST_STRING 'index'
    0xb7, // LOAD_FAST 7
    0x34,0x82,0x02, // CALL_FUNCTION 258
    0x34,0x01, // CALL_FUNCTION 1
    0x26,0x11, // STORE_FAST_N 17
    0xb4, // LOAD_FAST 4
    0x13,0x1d, // LOAD_ATTR 'output_shape'
    0xb8, // LOAD_FAST 8
    0x55, // LOAD_SUBSCR
    0x30,0x03, // UNPACK_SEQUENCE 3
    0x26,0x12, // STORE_FAST_N 18
    0x26,0x13, // STORE_FAST_N 19
    0x26,0x14, // STORE_FAST_N 20
    0xb6, // LOAD_FAST 6
    0xb8, // LOAD_FAST 8
    0x55, // LOAD_SUBSCR
    0x14,0x20, // LOAD_METHOD 'reshape'
    0x24,0x12, // LOAD_FAST_N 18
    0x24,0x13, // LOAD_FAST_N 19
    0xf4, // BINARY_OP 29 __mul__
    0x24,0x14, // LOAD_FAST_N 20
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0x26,0x15, // STORE_FAST_N 21
    0x12,0x40, // LOAD_GLOBAL 'dequantize'
    0xb4, // LOAD_FAST 4
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x21, // LOAD_METHOD 'take'
    0x24,0x15, // LOAD_FAST_N 21
    0x24,0x10, // LOAD_FAST_N 16
    0x10,0x22, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x10,0x1f, // LOAD_CONST_STRING 'index'
    0xb8, // LOAD_FAST 8
    0x34,0x82,0x02, // CALL_FUNCTION 258
    0x26,0x16, // STORE_FAST_N 22
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x21, // LOAD_METHOD 'take'
    0xba, // LOAD_FAST 10
    0x24,0x10, // LOAD_FAST_N 16
    0x10,0x22, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x26,0x17, // STORE_FAST_N 23
    0x24,0x17, // LOAD_FAST_N 23
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x26,0x18, // STORE_FAST_N 24
    0x24,0x17, // LOAD_FAST_N 23
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x26,0x19, // STORE_FAST_N 25
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0xf7, // BINARY_OP 32 __truediv__
    0x24,0x18, // LOAD_FAST_N 24
    0xf2, // BINARY_OP 27 __add__
    0x26,0x1a, // STORE_FAST_N 26
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0xf7, // BINARY_OP 32 __truediv__
    0x24,0x19, // LOAD_FAST_N 25
    0xf2, // BINARY_OP 27 __add__
    0x26,0x1b, // STORE_FAST_N 27
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0xf7, // BINARY_OP 32 __truediv__
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1c, // STORE_FAST_N 28
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0xf7, // BINARY_OP 32 __truediv__
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1d, // STORE_FAST_N 29
    0x24,0x16, // LOAD_FAST_N 22
    0x13,0x19, // LOAD_ATTR 'shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x26,0x1e, // STORE_FAST_N 30
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x13, // LOAD_METHOD 'empty'
    0x24,0x1e, // LOAD_FAST_N 30
    0x24,0x14, // LOAD_FAST_N 20
    0x84, // LOAD_CONST_SMALL_INT 4
    0xf3, // BINARY_OP 28 __sub__
    0x82, // LOAD_CONST_SMALL_INT 2
    0xf6, // BINARY_OP 31 __floordiv__
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x03, // BUILD_TUPLE 3
    0x36,0x01, // CALL_METHOD 1
    0x26,0x1f, // STORE_FAST_N 31
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x51, // LOAD_CONST_NONE
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2e,0x03, // BUILD_SLICE 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xb2, // LOAD_FAST 2
    0xf7, // BINARY_OP 32 __truediv__
    0x24,0x18, // LOAD_FAST_N 24
    0x14,0x20, // LOAD_METHOD 'reshape'
    0x24,0x1e, // LOAD_FAST_N 30
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xf2, // BINARY_OP 27 __add__
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x24,0x1f, // LOAD_FAST_N 31
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x03, // BUILD_TUPLE 3
    0x56, // STORE_SUBSCR
    0x24,0x16, // LOAD_FAST_N 22
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x85, // LOAD_CONST_SMALL_INT 5
    0x51, // LOAD_CONST_NONE
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2e,0x03, // BUILD_SLICE 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xb1, // LOAD_FAST 1
    0xf7, // BINARY_OP 32 __truediv__
    0x24,0x19, // LOAD_FAST_N 25
    0x14,0x20, // LOAD_METHOD 'reshape'
    0x24,0x1e, // LOAD_FAST_N 30
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0xf4, // BINARY_OP 29 __mul__
    0x24,0x1f, // LOAD_FAST_N 31
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x03, // BUILD_TUPLE 3
    0x56, // STORE_SUBSCR
    0x24,0x1a, // LOAD_FAST_N 26
    0x24,0x1c, // LOAD_FAST_N 28
    0xf3, // BINARY_OP 28 __sub__
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x20, // STORE_FAST_N 32
    0x24,0x1b, // LOAD_FAST_N 27
    0x24,0x1d, // LOAD_FAST_N 29
    0xf3, // BINARY_OP 28 __sub__
    0xb1, // LOAD_FAST 1
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x21, // STORE_FAST_N 33
    0x24,0x1a, // LOAD_FAST_N 26
    0x24,0x1c, // LOAD_FAST_N 28
    0xf2, // BINARY_OP 27 __add__
    0xb2, // LOAD_FAST 2
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x22, // STORE_FAST_N 34
    0x24,0x1b, // LOAD_FAST_N 27
    0x24,0x1d, // LOAD_FAST_N 29
    0xf2, // BINARY_OP 27 __add__
    0xb1, // LOAD_FAST 1
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x23, // STORE_FAST_N 35
    0x24,0x16, // LOAD_FAST_N 22
    0x13,0x19, // LOAD_ATTR 'shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x6d, // JUMP 45
    0x57, // DUP_TOP
    0x26,0x24, // STORE_FAST_N 36
    0xb3, // LOAD_FAST 3
    0x14,0x23, // LOAD_METHOD 'add_bounding_box'
    0x24,0x20, // LOAD_FAST_N 32
    0x24,0x24, // LOAD_FAST_N 36
    0x55, // LOAD_SUBSCR
    0x24,0x21, // LOAD_FAST_N 33
    0x24,0x24, // LOAD_FAST_N 36
    0x55, // LOAD_SUBSCR
    0x24,0x22, // LOAD_FAST_N 34
    0x24,0x24, // LOAD_FAST_N 36
    0x55, // LOAD_SUBSCR
    0x24,0x23, // LOAD_FAST_N 35
    0x24,0x24, // LOAD_FAST_N 36
    0x55, // LOAD_SUBSCR
    0x24,0x11, // LOAD_FAST_N 17
    0x24,0x24, // LOAD_FAST_N 36
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x24, // LOAD_CONST_STRING 'keypoints'
    0x24,0x1f, // LOAD_FAST_N 31
    0x24,0x24, // LOAD_FAST_N 36
    0x55, // LOAD_SUBSCR
    0x36,0x82,0x06, // CALL_METHOD 262
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x0e, // POP_JUMP_IF_TRUE -50
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 521,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 50,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 11,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 26,
        .line_info = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process + 16,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process + 48,
        .opcodes = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process + 48,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe_mediapipe_detection_postprocess[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess___call__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess_detection_post_process,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess,
    .children = (void *)&children_ml_postprocessing_mediapipe_mediapipe_detection_postprocess,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 49,
    .n_children = 3,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 7,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess + 3,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess + 12,
        .opcodes = fun_data_ml_postprocessing_mediapipe_mediapipe_detection_postprocess + 12,
    },
    #endif
    #endif
};

// child of ml_postprocessing_mediapipe__lt_module_gt_
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_BlazeFace
static const byte fun_data_ml_postprocessing_mediapipe_BlazeFace[32] = {
    0x20,0x07, // prelude
    0x09, // names: BlazeFace
    0x88,0x82,0x00, // code info
    0x11,0x2d, // LOAD_NAME '__name__'
    0x16,0x2e, // STORE_NAME '__module__'
    0x10,0x09, // LOAD_CONST_STRING 'BlazeFace'
    0x16,0x2f, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x51, // LOAD_CONST_NONE
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x21,0x00,0x01, // MAKE_CLOSURE_DEFARGS 0
    0x16,0x0e, // STORE_NAME '__init__'
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe_BlazeFace
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_BlazeFace___init__
static const byte fun_data_ml_postprocessing_mediapipe_BlazeFace___init__[60] = {
    0xaa,0x94,0x80,0x01,0x14, // prelude
    0x0e,0x02,0x30,0x0f,0x10,0x14,0x15, // names: __init__, *, self, threshold, anchors, nms_threshold, nms_sigma
    0x80,0x83,0x2f, // code info
    0x12,0x41, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x0e, // LOAD_SUPER_METHOD '__init__'
    0x10,0x0f, // LOAD_CONST_STRING 'threshold'
    0xb2, // LOAD_FAST 2
    0x10,0x10, // LOAD_CONST_STRING 'anchors'
    0xb3, // LOAD_FAST 3
    0x10,0x25, // LOAD_CONST_STRING 'anchor_grid'
    0x23,0x04, // LOAD_CONST_OBJ 4
    0x23,0x05, // LOAD_CONST_OBJ 5
    0x2b,0x02, // BUILD_LIST 2
    0x10,0x11, // LOAD_CONST_STRING 'scores'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2b,0x02, // BUILD_LIST 2
    0x10,0x12, // LOAD_CONST_STRING 'cords'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2b,0x02, // BUILD_LIST 2
    0x10,0x14, // LOAD_CONST_STRING 'nms_threshold'
    0xb4, // LOAD_FAST 4
    0x10,0x15, // LOAD_CONST_STRING 'nms_sigma'
    0xb5, // LOAD_FAST 5
    0x36,0x8e,0x00, // CALL_METHOD 1792
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_BlazeFace___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_BlazeFace___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 60,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 22,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 6,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_mediapipe_BlazeFace___init__ + 12,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_BlazeFace___init__ + 15,
        .opcodes = fun_data_ml_postprocessing_mediapipe_BlazeFace___init__ + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_BlazeFace___init__ fun_data_ml_postprocessing_mediapipe_BlazeFace___init__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe_BlazeFace[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_BlazeFace___init__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_BlazeFace = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_BlazeFace,
    .children = (void *)&children_ml_postprocessing_mediapipe_BlazeFace,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 32,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ml_postprocessing_mediapipe_BlazeFace + 3,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_BlazeFace + 5,
        .opcodes = fun_data_ml_postprocessing_mediapipe_BlazeFace + 6,
    },
    #endif
    #endif
};

// child of ml_postprocessing_mediapipe__lt_module_gt_
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_BlazePalm
static const byte fun_data_ml_postprocessing_mediapipe_BlazePalm[32] = {
    0x20,0x07, // prelude
    0x0a, // names: BlazePalm
    0x88,0x89,0x00, // code info
    0x11,0x2d, // LOAD_NAME '__name__'
    0x16,0x2e, // STORE_NAME '__module__'
    0x10,0x0a, // LOAD_CONST_STRING 'BlazePalm'
    0x16,0x2f, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x51, // LOAD_CONST_NONE
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x04, // BUILD_TUPLE 4
    0x53, // LOAD_NULL
    0xb0, // LOAD_FAST 0
    0x21,0x00,0x01, // MAKE_CLOSURE_DEFARGS 0
    0x16,0x0e, // STORE_NAME '__init__'
    0xb0, // LOAD_FAST 0
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe_BlazePalm
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_BlazePalm___init__
static const byte fun_data_ml_postprocessing_mediapipe_BlazePalm___init__[58] = {
    0xaa,0x94,0x80,0x01,0x14, // prelude
    0x0e,0x02,0x30,0x0f,0x10,0x14,0x15, // names: __init__, *, self, threshold, anchors, nms_threshold, nms_sigma
    0x80,0x8a,0x2f, // code info
    0x12,0x41, // LOAD_GLOBAL 'super'
    0x25,0x00, // LOAD_DEREF 0
    0xb1, // LOAD_FAST 1
    0x15,0x0e, // LOAD_SUPER_METHOD '__init__'
    0x10,0x0f, // LOAD_CONST_STRING 'threshold'
    0xb2, // LOAD_FAST 2
    0x10,0x10, // LOAD_CONST_STRING 'anchors'
    0xb3, // LOAD_FAST 3
    0x10,0x25, // LOAD_CONST_STRING 'anchor_grid'
    0x23,0x06, // LOAD_CONST_OBJ 6
    0x23,0x07, // LOAD_CONST_OBJ 7
    0x2b,0x02, // BUILD_LIST 2
    0x10,0x11, // LOAD_CONST_STRING 'scores'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2b,0x01, // BUILD_LIST 1
    0x10,0x12, // LOAD_CONST_STRING 'cords'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2b,0x01, // BUILD_LIST 1
    0x10,0x14, // LOAD_CONST_STRING 'nms_threshold'
    0xb4, // LOAD_FAST 4
    0x10,0x15, // LOAD_CONST_STRING 'nms_sigma'
    0xb5, // LOAD_FAST 5
    0x36,0x8e,0x00, // CALL_METHOD 1792
    0x59, // POP_TOP
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_BlazePalm___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_BlazePalm___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 58,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 22,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 6,
        .n_kwonly_args = 0,
        .n_def_pos_args = 4,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_mediapipe_BlazePalm___init__ + 12,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_BlazePalm___init__ + 15,
        .opcodes = fun_data_ml_postprocessing_mediapipe_BlazePalm___init__ + 15,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_BlazePalm___init__ fun_data_ml_postprocessing_mediapipe_BlazePalm___init__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe_BlazePalm[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_BlazePalm___init__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_BlazePalm = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_BlazePalm,
    .children = (void *)&children_ml_postprocessing_mediapipe_BlazePalm,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 32,
    .n_children = 1,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 5,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_postprocessing_mediapipe_BlazePalm + 3,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_BlazePalm + 5,
        .opcodes = fun_data_ml_postprocessing_mediapipe_BlazePalm + 6,
    },
    #endif
    #endif
};

// child of ml_postprocessing_mediapipe__lt_module_gt_
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_HandLandmarks
static const byte fun_data_ml_postprocessing_mediapipe_HandLandmarks[34] = {
    0x10,0x0a, // prelude
    0x0b, // names: HandLandmarks
    0x88,0x90,0x6d,0x40, // code info
    0x11,0x2d, // LOAD_NAME '__name__'
    0x16,0x2e, // STORE_NAME '__module__'
    0x10,0x0b, // LOAD_CONST_STRING 'HandLandmarks'
    0x16,0x2f, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0e, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x16, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe_HandLandmarks
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_HandLandmarks___init__
static const byte fun_data_ml_postprocessing_mediapipe_HandLandmarks___init__[27] = {
    0xa8,0x85,0x01,0x12, // prelude
    0x0e,0x30,0x0f,0x14,0x15, // names: __init__, self, threshold, nms_threshold, nms_sigma
    0x80,0x91,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x14, // STORE_ATTR 'nms_threshold'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_HandLandmarks___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_HandLandmarks___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_mediapipe_HandLandmarks___init__ + 9,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_HandLandmarks___init__ + 13,
        .opcodes = fun_data_ml_postprocessing_mediapipe_HandLandmarks___init__ + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_HandLandmarks___init__ fun_data_ml_postprocessing_mediapipe_HandLandmarks___init__[0]
#endif

// child of ml_postprocessing_mediapipe_HandLandmarks
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_HandLandmarks___call__
static const byte fun_data_ml_postprocessing_mediapipe_HandLandmarks___call__[252] = {
    0xd0,0x14,0x2e, // prelude
    0x16,0x30,0x32,0x33,0x34, // names: __call__, self, model, inputs, outputs
    0x80,0x96,0x2b,0x4c,0x27,0x27,0x43,0x6c,0x31,0x30,0x30,0x70,0x30,0x30,0x30,0x50,0x4b,0x31, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x17, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0x12,0x35, // LOAD_GLOBAL 'NMS'
    0xb6, // LOAD_FAST 6
    0xb5, // LOAD_FAST 5
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x18, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0xc8, // STORE_FAST 8
    0xb3, // LOAD_FAST 3
    0x82, // LOAD_CONST_SMALL_INT 2
    0x55, // LOAD_SUBSCR
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x55, // LOAD_SUBSCR
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'threshold'
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x63, // RETURN_VALUE
    0xb3, // LOAD_FAST 3
    0x83, // LOAD_CONST_SMALL_INT 3
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xca, // STORE_FAST 10
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x13, // LOAD_METHOD 'empty'
    0x12,0x36, // LOAD_GLOBAL 'len'
    0xba, // LOAD_FAST 10
    0x34,0x01, // CALL_FUNCTION 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0xf6, // BINARY_OP 31 __floordiv__
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xcb, // STORE_FAST 11
    0xba, // LOAD_FAST 10
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2e,0x03, // BUILD_SLICE 3
    0x55, // LOAD_SUBSCR
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xba, // LOAD_FAST 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2e,0x03, // BUILD_SLICE 3
    0x55, // LOAD_SUBSCR
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xba, // LOAD_FAST 10
    0x82, // LOAD_CONST_SMALL_INT 2
    0x51, // LOAD_CONST_NONE
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2e,0x03, // BUILD_SLICE 3
    0x55, // LOAD_SUBSCR
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x26, // LOAD_METHOD 'min'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xcc, // STORE_FAST 12
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x26, // LOAD_METHOD 'min'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xcd, // STORE_FAST 13
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x27, // LOAD_METHOD 'max'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xce, // STORE_FAST 14
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x27, // LOAD_METHOD 'max'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xcf, // STORE_FAST 15
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x23,0x08, // LOAD_CONST_OBJ 8
    0x55, // LOAD_SUBSCR
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xd8, // BINARY_OP 1 __gt__
    0x26,0x10, // STORE_FAST_N 16
    0xb8, // LOAD_FAST 8
    0x14,0x23, // LOAD_METHOD 'add_bounding_box'
    0xbc, // LOAD_FAST 12
    0xbd, // LOAD_FAST 13
    0xbe, // LOAD_FAST 14
    0xbf, // LOAD_FAST 15
    0xb9, // LOAD_FAST 9
    0x24,0x10, // LOAD_FAST_N 16
    0x10,0x24, // LOAD_CONST_STRING 'keypoints'
    0xbb, // LOAD_FAST 11
    0x36,0x82,0x06, // CALL_METHOD 262
    0x59, // POP_TOP
    0xb8, // LOAD_FAST 8
    0x14,0x1b, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0f, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'nms_threshold'
    0x10,0x1c, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_HandLandmarks___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_HandLandmarks___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 252,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 27,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_ml_postprocessing_mediapipe_HandLandmarks___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_HandLandmarks___call__ + 26,
        .opcodes = fun_data_ml_postprocessing_mediapipe_HandLandmarks___call__ + 26,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_HandLandmarks___call__ fun_data_ml_postprocessing_mediapipe_HandLandmarks___call__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe_HandLandmarks[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_HandLandmarks___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_HandLandmarks___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_HandLandmarks = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_HandLandmarks,
    .children = (void *)&children_ml_postprocessing_mediapipe_HandLandmarks,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 34,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 11,
        .line_info = fun_data_ml_postprocessing_mediapipe_HandLandmarks + 3,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_HandLandmarks + 7,
        .opcodes = fun_data_ml_postprocessing_mediapipe_HandLandmarks + 7,
    },
    #endif
    #endif
};

// child of ml_postprocessing_mediapipe__lt_module_gt_
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_FaceLandmarks
static const byte fun_data_ml_postprocessing_mediapipe_FaceLandmarks[34] = {
    0x10,0x0a, // prelude
    0x0c, // names: FaceLandmarks
    0x88,0xb2,0x6d,0x40, // code info
    0x11,0x2d, // LOAD_NAME '__name__'
    0x16,0x2e, // STORE_NAME '__module__'
    0x10,0x0c, // LOAD_CONST_STRING 'FaceLandmarks'
    0x16,0x2f, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0e, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x16, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe_FaceLandmarks
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_FaceLandmarks___init__
static const byte fun_data_ml_postprocessing_mediapipe_FaceLandmarks___init__[27] = {
    0xa8,0x85,0x01,0x12, // prelude
    0x0e,0x30,0x0f,0x14,0x15, // names: __init__, self, threshold, nms_threshold, nms_sigma
    0x80,0xb3,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x14, // STORE_ATTR 'nms_threshold'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_FaceLandmarks___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___init__ + 9,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___init__ + 13,
        .opcodes = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___init__ + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_FaceLandmarks___init__ fun_data_ml_postprocessing_mediapipe_FaceLandmarks___init__[0]
#endif

// child of ml_postprocessing_mediapipe_FaceLandmarks
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_FaceLandmarks___call__
static const byte fun_data_ml_postprocessing_mediapipe_FaceLandmarks___call__[247] = {
    0xc8,0x14,0x2c, // prelude
    0x16,0x30,0x32,0x33,0x34, // names: __call__, self, model, inputs, outputs
    0x80,0xb8,0x2b,0x4c,0x2b,0x27,0x43,0x6e,0x31,0x30,0x30,0x70,0x30,0x30,0x30,0x50,0x30, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x17, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0x12,0x35, // LOAD_GLOBAL 'NMS'
    0xb6, // LOAD_FAST 6
    0xb5, // LOAD_FAST 5
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x18, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0xc8, // STORE_FAST 8
    0x12,0x3f, // LOAD_GLOBAL 'sigmoid'
    0xb3, // LOAD_FAST 3
    0x81, // LOAD_CONST_SMALL_INT 1
    0x55, // LOAD_SUBSCR
    0x23,0x09, // LOAD_CONST_OBJ 9
    0x55, // LOAD_SUBSCR
    0x34,0x01, // CALL_FUNCTION 1
    0xc9, // STORE_FAST 9
    0xb9, // LOAD_FAST 9
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'threshold'
    0xd7, // BINARY_OP 0 __lt__
    0x44,0x43, // POP_JUMP_IF_FALSE 3
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x63, // RETURN_VALUE
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x04, // BUILD_TUPLE 4
    0x55, // LOAD_SUBSCR
    0xca, // STORE_FAST 10
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x13, // LOAD_METHOD 'empty'
    0x12,0x36, // LOAD_GLOBAL 'len'
    0xba, // LOAD_FAST 10
    0x34,0x01, // CALL_FUNCTION 1
    0x83, // LOAD_CONST_SMALL_INT 3
    0xf6, // BINARY_OP 31 __floordiv__
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xcb, // STORE_FAST 11
    0xba, // LOAD_FAST 10
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2e,0x03, // BUILD_SLICE 3
    0x55, // LOAD_SUBSCR
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xba, // LOAD_FAST 10
    0x81, // LOAD_CONST_SMALL_INT 1
    0x51, // LOAD_CONST_NONE
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2e,0x03, // BUILD_SLICE 3
    0x55, // LOAD_SUBSCR
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xba, // LOAD_FAST 10
    0x82, // LOAD_CONST_SMALL_INT 2
    0x51, // LOAD_CONST_NONE
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2e,0x03, // BUILD_SLICE 3
    0x55, // LOAD_SUBSCR
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x26, // LOAD_METHOD 'min'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xcc, // STORE_FAST 12
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x26, // LOAD_METHOD 'min'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xcd, // STORE_FAST 13
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x27, // LOAD_METHOD 'max'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xce, // STORE_FAST 14
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x27, // LOAD_METHOD 'max'
    0xbb, // LOAD_FAST 11
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0xcf, // STORE_FAST 15
    0xb8, // LOAD_FAST 8
    0x14,0x23, // LOAD_METHOD 'add_bounding_box'
    0xbc, // LOAD_FAST 12
    0xbd, // LOAD_FAST 13
    0xbe, // LOAD_FAST 14
    0xbf, // LOAD_FAST 15
    0xb9, // LOAD_FAST 9
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x24, // LOAD_CONST_STRING 'keypoints'
    0xbb, // LOAD_FAST 11
    0x36,0x82,0x06, // CALL_METHOD 262
    0x59, // POP_TOP
    0xb8, // LOAD_FAST 8
    0x14,0x1b, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0f, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'nms_threshold'
    0x10,0x1c, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_FaceLandmarks___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 247,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 26,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___call__ + 25,
        .opcodes = fun_data_ml_postprocessing_mediapipe_FaceLandmarks___call__ + 25,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_FaceLandmarks___call__ fun_data_ml_postprocessing_mediapipe_FaceLandmarks___call__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe_FaceLandmarks[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_FaceLandmarks___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_FaceLandmarks___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_FaceLandmarks = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_FaceLandmarks,
    .children = (void *)&children_ml_postprocessing_mediapipe_FaceLandmarks,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 34,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 12,
        .line_info = fun_data_ml_postprocessing_mediapipe_FaceLandmarks + 3,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_FaceLandmarks + 7,
        .opcodes = fun_data_ml_postprocessing_mediapipe_FaceLandmarks + 7,
    },
    #endif
    #endif
};

// child of ml_postprocessing_mediapipe__lt_module_gt_
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_MoveNet
static const byte fun_data_ml_postprocessing_mediapipe_MoveNet[34] = {
    0x10,0x0a, // prelude
    0x0d, // names: MoveNet
    0x88,0xd2,0x6d,0x40, // code info
    0x11,0x2d, // LOAD_NAME '__name__'
    0x16,0x2e, // STORE_NAME '__module__'
    0x10,0x0d, // LOAD_CONST_STRING 'MoveNet'
    0x16,0x2f, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0e, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x16, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_mediapipe_MoveNet
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_MoveNet___init__
static const byte fun_data_ml_postprocessing_mediapipe_MoveNet___init__[27] = {
    0xa8,0x85,0x01,0x12, // prelude
    0x0e,0x30,0x0f,0x14,0x15, // names: __init__, self, threshold, nms_threshold, nms_sigma
    0x80,0xd3,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0f, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x14, // STORE_ATTR 'nms_threshold'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x15, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_MoveNet___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_MoveNet___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_mediapipe_MoveNet___init__ + 9,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_MoveNet___init__ + 13,
        .opcodes = fun_data_ml_postprocessing_mediapipe_MoveNet___init__ + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_MoveNet___init__ fun_data_ml_postprocessing_mediapipe_MoveNet___init__[0]
#endif

// child of ml_postprocessing_mediapipe_MoveNet
// frozen bytecode for file ml/postprocessing/mediapipe.py, scope ml_postprocessing_mediapipe_MoveNet___call__
static const byte fun_data_ml_postprocessing_mediapipe_MoveNet___call__[348] = {
    0x98,0x24,0x3a, // prelude
    0x16,0x30,0x32,0x33,0x34, // names: __call__, self, model, inputs, outputs
    0x80,0xd8,0x6b,0x4e,0x2c,0x4c,0x2e,0x27,0x43,0x24,0x64,0x2d,0x2c,0x33,0x32,0x6b,0x2f,0x32,0x32,0x32,0x32,0x52,0x2e,0x37, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x1d, // LOAD_ATTR 'output_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xc7, // STORE_FAST 7
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x14,0x20, // LOAD_METHOD 'reshape'
    0xb5, // LOAD_FAST 5
    0xb6, // LOAD_FAST 6
    0xf4, // BINARY_OP 29 __mul__
    0xb7, // LOAD_FAST 7
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xc8, // STORE_FAST 8
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x28, // LOAD_METHOD 'argmax'
    0xb8, // LOAD_FAST 8
    0x10,0x22, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x01, // CALL_METHOD 257
    0xc9, // STORE_FAST 9
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x27, // LOAD_METHOD 'max'
    0xb8, // LOAD_FAST 8
    0x10,0x22, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x01, // CALL_METHOD 257
    0xca, // STORE_FAST 10
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x29, // LOAD_METHOD 'nonzero'
    0xba, // LOAD_FAST 10
    0xb0, // LOAD_FAST 0
    0x13,0x0f, // LOAD_ATTR 'threshold'
    0xd8, // BINARY_OP 1 __gt__
    0x36,0x01, // CALL_METHOD 1
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xcb, // STORE_FAST 11
    0x12,0x36, // LOAD_GLOBAL 'len'
    0xbb, // LOAD_FAST 11
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x03, // LOAD_CONST_OBJ 3
    0x63, // RETURN_VALUE
    0xb9, // LOAD_FAST 9
    0xb6, // LOAD_FAST 6
    0xf6, // BINARY_OP 31 __floordiv__
    0xcc, // STORE_FAST 12
    0xb9, // LOAD_FAST 9
    0xb6, // LOAD_FAST 6
    0xf8, // BINARY_OP 33 __mod__
    0xcd, // STORE_FAST 13
    0xb1, // LOAD_FAST 1
    0x13,0x17, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0xce, // STORE_FAST 14
    0xcf, // STORE_FAST 15
    0x26,0x10, // STORE_FAST_N 16
    0x26,0x11, // STORE_FAST_N 17
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x13, // LOAD_METHOD 'empty'
    0xb7, // LOAD_FAST 7
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0x26,0x12, // STORE_FAST_N 18
    0xbd, // LOAD_FAST 13
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf2, // BINARY_OP 27 __add__
    0xb6, // LOAD_FAST 6
    0xf7, // BINARY_OP 32 __truediv__
    0x24,0x10, // LOAD_FAST_N 16
    0xf4, // BINARY_OP 29 __mul__
    0x24,0x12, // LOAD_FAST_N 18
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xbc, // LOAD_FAST 12
    0x23,0x02, // LOAD_CONST_OBJ 2
    0xf2, // BINARY_OP 27 __add__
    0xb5, // LOAD_FAST 5
    0xf7, // BINARY_OP 32 __truediv__
    0xbf, // LOAD_FAST 15
    0xf4, // BINARY_OP 29 __mul__
    0x24,0x12, // LOAD_FAST_N 18
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0xba, // LOAD_FAST 10
    0x24,0x12, // LOAD_FAST_N 18
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x56, // STORE_SUBSCR
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x21, // LOAD_METHOD 'take'
    0x24,0x12, // LOAD_FAST_N 18
    0xbb, // LOAD_FAST 11
    0x10,0x22, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x26,0x13, // STORE_FAST_N 19
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x2a, // LOAD_METHOD 'mean'
    0x24,0x13, // LOAD_FAST_N 19
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x26,0x14, // STORE_FAST_N 20
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x26, // LOAD_METHOD 'min'
    0x24,0x13, // LOAD_FAST_N 19
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x26,0x15, // STORE_FAST_N 21
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x26, // LOAD_METHOD 'min'
    0x24,0x13, // LOAD_FAST_N 19
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x26,0x16, // STORE_FAST_N 22
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x27, // LOAD_METHOD 'max'
    0x24,0x13, // LOAD_FAST_N 19
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x26,0x17, // STORE_FAST_N 23
    0x12,0x2c, // LOAD_GLOBAL 'np'
    0x14,0x27, // LOAD_METHOD 'max'
    0x24,0x13, // LOAD_FAST_N 19
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x36,0x01, // CALL_METHOD 1
    0x26,0x18, // STORE_FAST_N 24
    0x12,0x35, // LOAD_GLOBAL 'NMS'
    0x24,0x10, // LOAD_FAST_N 16
    0xbf, // LOAD_FAST 15
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x18, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0x26,0x19, // STORE_FAST_N 25
    0x24,0x19, // LOAD_FAST_N 25
    0x14,0x23, // LOAD_METHOD 'add_bounding_box'
    0x24,0x15, // LOAD_FAST_N 21
    0x24,0x16, // LOAD_FAST_N 22
    0x24,0x17, // LOAD_FAST_N 23
    0x24,0x18, // LOAD_FAST_N 24
    0x24,0x14, // LOAD_FAST_N 20
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x24, // LOAD_CONST_STRING 'keypoints'
    0x24,0x12, // LOAD_FAST_N 18
    0x36,0x82,0x06, // CALL_METHOD 262
    0x59, // POP_TOP
    0x24,0x19, // LOAD_FAST_N 25
    0x14,0x1b, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0f, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x14, // LOAD_ATTR 'nms_threshold'
    0x10,0x1c, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x15, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_MoveNet___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_MoveNet___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 348,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 36,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 22,
        .line_info = fun_data_ml_postprocessing_mediapipe_MoveNet___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_MoveNet___call__ + 32,
        .opcodes = fun_data_ml_postprocessing_mediapipe_MoveNet___call__ + 32,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_mediapipe_MoveNet___call__ fun_data_ml_postprocessing_mediapipe_MoveNet___call__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe_MoveNet[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_MoveNet___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_MoveNet___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe_MoveNet = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe_MoveNet,
    .children = (void *)&children_ml_postprocessing_mediapipe_MoveNet,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 34,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 13,
        .line_info = fun_data_ml_postprocessing_mediapipe_MoveNet + 3,
        .line_info_top = fun_data_ml_postprocessing_mediapipe_MoveNet + 7,
        .opcodes = fun_data_ml_postprocessing_mediapipe_MoveNet + 7,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_postprocessing_mediapipe__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_mediapipe_detection_postprocess,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_BlazeFace,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_BlazePalm,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_HandLandmarks,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_FaceLandmarks,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_mediapipe_MoveNet,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_mediapipe__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_mediapipe__lt_module_gt_,
    .children = (void *)&children_ml_postprocessing_mediapipe__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 111,
    .n_children = 6,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 4,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_postprocessing_mediapipe__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_postprocessing_mediapipe__lt_module_gt_ + 19,
        .opcodes = fun_data_ml_postprocessing_mediapipe__lt_module_gt_ + 19,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_postprocessing_mediapipe[66] = {
    MP_QSTR_ml_slash_postprocessing_slash_mediapipe_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_ml_dot_utils,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_numpy,
    MP_QSTR_ulab,
    MP_QSTR_mediapipe_detection_postprocess,
    MP_QSTR_BlazeFace,
    MP_QSTR_BlazePalm,
    MP_QSTR_HandLandmarks,
    MP_QSTR_FaceLandmarks,
    MP_QSTR_MoveNet,
    MP_QSTR___init__,
    MP_QSTR_threshold,
    MP_QSTR_anchors,
    MP_QSTR_scores,
    MP_QSTR_cords,
    MP_QSTR_empty,
    MP_QSTR_nms_threshold,
    MP_QSTR_nms_sigma,
    MP_QSTR___call__,
    MP_QSTR_input_shape,
    MP_QSTR_roi,
    MP_QSTR_shape,
    MP_QSTR_detection_post_process,
    MP_QSTR_get_bounding_boxes,
    MP_QSTR_sigma,
    MP_QSTR_output_shape,
    MP_QSTR_output_scale,
    MP_QSTR_index,
    MP_QSTR_reshape,
    MP_QSTR_take,
    MP_QSTR_axis,
    MP_QSTR_add_bounding_box,
    MP_QSTR_keypoints,
    MP_QSTR_anchor_grid,
    MP_QSTR_min,
    MP_QSTR_max,
    MP_QSTR_argmax,
    MP_QSTR_nonzero,
    MP_QSTR_mean,
    MP_QSTR__lt_genexpr_gt_,
    MP_QSTR_np,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_sum,
    MP_QSTR_model,
    MP_QSTR_inputs,
    MP_QSTR_outputs,
    MP_QSTR_NMS,
    MP_QSTR_len,
    MP_QSTR_ih,
    MP_QSTR_iw,
    MP_QSTR_nms,
    MP_QSTR_score_idx,
    MP_QSTR_cords_idx,
    MP_QSTR_t,
    MP_QSTR_quantize,
    MP_QSTR_logit,
    MP_QSTR_sigmoid,
    MP_QSTR_dequantize,
    MP_QSTR_super,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_mediapipe_0 = {{&mp_type_float}, (mp_float_t)0.6};
#define const_obj_ml_postprocessing_mediapipe_0_macro MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_mediapipe_0_macro ((mp_rom_obj_t)(0xbf99999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_mediapipe_0_macro ((mp_rom_obj_t)(0xbfe7333333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_mediapipe_1 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_ml_postprocessing_mediapipe_1_macro MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_mediapipe_1_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_mediapipe_1_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_mediapipe_2 = {{&mp_type_float}, (mp_float_t)0.5};
#define const_obj_ml_postprocessing_mediapipe_2_macro MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_2)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_mediapipe_2_macro ((mp_rom_obj_t)(0xbf800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_mediapipe_2_macro ((mp_rom_obj_t)(0xbfe4000000000000))
#endif
static const mp_rom_obj_tuple_t const_obj_ml_postprocessing_mediapipe_4 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(16),
    MP_ROM_INT(2),
}};
static const mp_rom_obj_tuple_t const_obj_ml_postprocessing_mediapipe_5 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(8),
    MP_ROM_INT(6),
}};
static const mp_rom_obj_tuple_t const_obj_ml_postprocessing_mediapipe_6 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(24),
    MP_ROM_INT(2),
}};
static const mp_rom_obj_tuple_t const_obj_ml_postprocessing_mediapipe_7 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(12),
    MP_ROM_INT(6),
}};
static const mp_rom_obj_tuple_t const_obj_ml_postprocessing_mediapipe_8 = {{&mp_type_tuple}, 2, {
    MP_ROM_INT(0),
    MP_ROM_INT(0),
}};
static const mp_rom_obj_tuple_t const_obj_ml_postprocessing_mediapipe_9 = {{&mp_type_tuple}, 4, {
    MP_ROM_INT(0),
    MP_ROM_INT(0),
    MP_ROM_INT(0),
    MP_ROM_INT(0),
}};

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_postprocessing_mediapipe[10] = {
    const_obj_ml_postprocessing_mediapipe_0_macro,
    const_obj_ml_postprocessing_mediapipe_1_macro,
    const_obj_ml_postprocessing_mediapipe_2_macro,
    MP_ROM_PTR(&mp_const_empty_tuple_obj),
    MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_4),
    MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_5),
    MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_6),
    MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_7),
    MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_8),
    MP_ROM_PTR(&const_obj_ml_postprocessing_mediapipe_9),
};

static const mp_frozen_module_t frozen_module_ml_postprocessing_mediapipe = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_postprocessing_mediapipe,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_postprocessing_mediapipe,
    },
    .proto_fun = &proto_fun_ml_postprocessing_mediapipe__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// frozen module ml_postprocessing_ultralytics
// - original source file: D:/Single-chip/e2studio/ws7_xip/omv_ra8p1_xip/src/MicroPython/frozen_mpy/ml\postprocessing\ultralytics.mpy
// - frozen file name: ml/postprocessing/ultralytics.py
// - .mpy header: 4d:06:00:1f

// frozen bytecode for file ml/postprocessing/ultralytics.py, scope ml_postprocessing_ultralytics__lt_module_gt_
static const byte fun_data_ml_postprocessing_ultralytics__lt_module_gt_[63] = {
    0x10,0x12, // prelude
    0x01, // names: <module>
    0x80,0x1c,0x28,0x2c,0x6c,0x60,0x89,0x38, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x02, // LOAD_CONST_STRING '*'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x03, // IMPORT_NAME 'ml.utils'
    0x69, // IMPORT_STAR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x04, // LOAD_CONST_STRING 'const'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x05, // IMPORT_NAME 'micropython'
    0x1c,0x04, // IMPORT_FROM 'const'
    0x16,0x04, // STORE_NAME 'const'
    0x59, // POP_TOP
    0x80, // LOAD_CONST_SMALL_INT 0
    0x10,0x06, // LOAD_CONST_STRING 'numpy'
    0x2a,0x01, // BUILD_TUPLE 1
    0x1b,0x07, // IMPORT_NAME 'ulab'
    0x1c,0x06, // IMPORT_FROM 'numpy'
    0x16,0x1f, // STORE_NAME 'np'
    0x59, // POP_TOP
    0x54, // LOAD_BUILD_CLASS
    0x32,0x00, // MAKE_FUNCTION 0
    0x10,0x08, // LOAD_CONST_STRING 'YoloV5'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x08, // STORE_NAME 'YoloV5'
    0x54, // LOAD_BUILD_CLASS
    0x32,0x01, // MAKE_FUNCTION 1
    0x10,0x09, // LOAD_CONST_STRING 'YoloV8'
    0x34,0x02, // CALL_FUNCTION 2
    0x16,0x09, // STORE_NAME 'YoloV8'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_ultralytics__lt_module_gt_
// frozen bytecode for file ml/postprocessing/ultralytics.py, scope ml_postprocessing_ultralytics_YoloV5
static const byte fun_data_ml_postprocessing_ultralytics_YoloV5[36] = {
    0x10,0x0e, // prelude
    0x08, // names: YoloV5
    0x88,0x25,0x80,0x07,0x6d,0x40, // code info
    0x11,0x20, // LOAD_NAME '__name__'
    0x16,0x21, // STORE_NAME '__module__'
    0x10,0x08, // LOAD_CONST_STRING 'YoloV5'
    0x16,0x22, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0a, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0e, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_ultralytics_YoloV5
// frozen bytecode for file ml/postprocessing/ultralytics.py, scope ml_postprocessing_ultralytics_YoloV5___init__
static const byte fun_data_ml_postprocessing_ultralytics_YoloV5___init__[27] = {
    0xa8,0x85,0x01,0x12, // prelude
    0x0a,0x23,0x0b,0x0c,0x0d, // names: __init__, self, threshold, nms_threshold, nms_sigma
    0x80,0x2d,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x0c, // STORE_ATTR 'nms_threshold'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x0d, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_ultralytics_YoloV5___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_ultralytics_YoloV5___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_postprocessing_ultralytics_YoloV5___init__ + 9,
        .line_info_top = fun_data_ml_postprocessing_ultralytics_YoloV5___init__ + 13,
        .opcodes = fun_data_ml_postprocessing_ultralytics_YoloV5___init__ + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_ultralytics_YoloV5___init__ fun_data_ml_postprocessing_ultralytics_YoloV5___init__[0]
#endif

// child of ml_postprocessing_ultralytics_YoloV5
// frozen bytecode for file ml/postprocessing/ultralytics.py, scope ml_postprocessing_ultralytics_YoloV5___call__
static const byte fun_data_ml_postprocessing_ultralytics_YoloV5___call__[349] = {
    0xb8,0x24,0x40, // prelude
    0x0e,0x23,0x24,0x25,0x26, // names: __call__, self, model, inputs, outputs
    0x80,0x32,0x2a,0x26,0x29,0x64,0x70,0x2a,0x28,0x27,0x63,0x72,0x6a,0x77,0x2a,0x2b,0x2e,0x6e,0x2f,0x29,0x2a,0x29,0x4a,0x2f,0x2b,0x1f,0x2d, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x0f, // LOAD_ATTR 'output_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xb1, // LOAD_FAST 1
    0x13,0x10, // LOAD_ATTR 'output_scale'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc7, // STORE_FAST 7
    0x12,0x27, // LOAD_GLOBAL 'quantize'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'threshold'
    0x34,0x02, // CALL_FUNCTION 2
    0xc8, // STORE_FAST 8
    0xb6, // LOAD_FAST 6
    0x85, // LOAD_CONST_SMALL_INT 5
    0xf3, // BINARY_OP 28 __sub__
    0xc9, // STORE_FAST 9
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x14,0x11, // LOAD_METHOD 'reshape'
    0xb4, // LOAD_FAST 4
    0xb5, // LOAD_FAST 5
    0xf4, // BINARY_OP 29 __mul__
    0x85, // LOAD_CONST_SMALL_INT 5
    0xb9, // LOAD_FAST 9
    0xf2, // BINARY_OP 27 __add__
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0xca, // STORE_FAST 10
    0xba, // LOAD_FAST 10
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xcb, // STORE_FAST 11
    0x12,0x0b, // LOAD_GLOBAL 'threshold'
    0xbb, // LOAD_FAST 11
    0xb8, // LOAD_FAST 8
    0xb7, // LOAD_FAST 7
    0x34,0x03, // CALL_FUNCTION 3
    0xcb, // STORE_FAST 11
    0x12,0x28, // LOAD_GLOBAL 'len'
    0xbb, // LOAD_FAST 11
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x63, // RETURN_VALUE
    0x12,0x29, // LOAD_GLOBAL 'dequantize'
    0xb1, // LOAD_FAST 1
    0x12,0x1f, // LOAD_GLOBAL 'np'
    0x14,0x12, // LOAD_METHOD 'take'
    0xba, // LOAD_FAST 10
    0xbb, // LOAD_FAST 11
    0x10,0x13, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x34,0x02, // CALL_FUNCTION 2
    0xcc, // STORE_FAST 12
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xcd, // STORE_FAST 13
    0x12,0x1f, // LOAD_GLOBAL 'np'
    0x14,0x14, // LOAD_METHOD 'argmax'
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x85, // LOAD_CONST_SMALL_INT 5
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x10,0x13, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0xce, // STORE_FAST 14
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xcf, // STORE_FAST 15
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x26,0x10, // STORE_FAST_N 16
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x11, // STORE_FAST_N 17
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x12, // STORE_FAST_N 18
    0xb1, // LOAD_FAST 1
    0x13,0x15, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0x26,0x13, // STORE_FAST_N 19
    0x26,0x14, // STORE_FAST_N 20
    0x26,0x15, // STORE_FAST_N 21
    0x26,0x16, // STORE_FAST_N 22
    0xbf, // LOAD_FAST 15
    0x24,0x11, // LOAD_FAST_N 17
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x15, // LOAD_FAST_N 21
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x17, // STORE_FAST_N 23
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x12, // LOAD_FAST_N 18
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x14, // LOAD_FAST_N 20
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x18, // STORE_FAST_N 24
    0xbf, // LOAD_FAST 15
    0x24,0x11, // LOAD_FAST_N 17
    0xf2, // BINARY_OP 27 __add__
    0x24,0x15, // LOAD_FAST_N 21
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x19, // STORE_FAST_N 25
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x12, // LOAD_FAST_N 18
    0xf2, // BINARY_OP 27 __add__
    0x24,0x14, // LOAD_FAST_N 20
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1a, // STORE_FAST_N 26
    0x12,0x2a, // LOAD_GLOBAL 'NMS'
    0x24,0x15, // LOAD_FAST_N 21
    0x24,0x14, // LOAD_FAST_N 20
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x16, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0x26,0x1b, // STORE_FAST_N 27
    0xbc, // LOAD_FAST 12
    0x13,0x17, // LOAD_ATTR 'shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x68, // JUMP 40
    0x57, // DUP_TOP
    0x26,0x1c, // STORE_FAST_N 28
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x18, // LOAD_METHOD 'add_bounding_box'
    0x24,0x17, // LOAD_FAST_N 23
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x24,0x18, // LOAD_FAST_N 24
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x24,0x19, // LOAD_FAST_N 25
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x24,0x1a, // LOAD_FAST_N 26
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0xbd, // LOAD_FAST 13
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0xbe, // LOAD_FAST 14
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x36,0x06, // CALL_METHOD 6
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x13, // POP_JUMP_IF_TRUE -45
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x19, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0b, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'nms_threshold'
    0x10,0x1a, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x0d, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_ultralytics_YoloV5___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_ultralytics_YoloV5___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 349,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 40,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_ultralytics_YoloV5___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_ultralytics_YoloV5___call__ + 35,
        .opcodes = fun_data_ml_postprocessing_ultralytics_YoloV5___call__ + 35,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_ultralytics_YoloV5___call__ fun_data_ml_postprocessing_ultralytics_YoloV5___call__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_ultralytics_YoloV5[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_ultralytics_YoloV5___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_ultralytics_YoloV5___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_ultralytics_YoloV5 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_ultralytics_YoloV5,
    .children = (void *)&children_ml_postprocessing_ultralytics_YoloV5,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 36,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 8,
        .line_info = fun_data_ml_postprocessing_ultralytics_YoloV5 + 3,
        .line_info_top = fun_data_ml_postprocessing_ultralytics_YoloV5 + 9,
        .opcodes = fun_data_ml_postprocessing_ultralytics_YoloV5 + 9,
    },
    #endif
    #endif
};

// child of ml_postprocessing_ultralytics__lt_module_gt_
// frozen bytecode for file ml/postprocessing/ultralytics.py, scope ml_postprocessing_ultralytics_YoloV8
static const byte fun_data_ml_postprocessing_ultralytics_YoloV8[36] = {
    0x10,0x0e, // prelude
    0x09, // names: YoloV8
    0x88,0x5d,0x60,0x60,0x6d,0x40, // code info
    0x11,0x20, // LOAD_NAME '__name__'
    0x16,0x21, // STORE_NAME '__module__'
    0x10,0x09, // LOAD_CONST_STRING 'YoloV8'
    0x16,0x22, // STORE_NAME '__qualname__'
    0x23,0x00, // LOAD_CONST_OBJ 0
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x23,0x01, // LOAD_CONST_OBJ 1
    0x2a,0x03, // BUILD_TUPLE 3
    0x53, // LOAD_NULL
    0x33,0x00, // MAKE_FUNCTION_DEFARGS 0
    0x16,0x0a, // STORE_NAME '__init__'
    0x32,0x01, // MAKE_FUNCTION 1
    0x16,0x0e, // STORE_NAME '__call__'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
// child of ml_postprocessing_ultralytics_YoloV8
// frozen bytecode for file ml/postprocessing/ultralytics.py, scope ml_postprocessing_ultralytics_YoloV8___init__
static const byte fun_data_ml_postprocessing_ultralytics_YoloV8___init__[27] = {
    0xa8,0x85,0x01,0x12, // prelude
    0x0a,0x23,0x0b,0x0c,0x0d, // names: __init__, self, threshold, nms_threshold, nms_sigma
    0x80,0x64,0x24,0x24, // code info
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x18,0x0b, // STORE_ATTR 'threshold'
    0xb2, // LOAD_FAST 2
    0xb0, // LOAD_FAST 0
    0x18,0x0c, // STORE_ATTR 'nms_threshold'
    0xb3, // LOAD_FAST 3
    0xb0, // LOAD_FAST 0
    0x18,0x0d, // STORE_ATTR 'nms_sigma'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_ultralytics_YoloV8___init__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_ultralytics_YoloV8___init__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 27,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 3,
        .qstr_block_name_idx = 10,
        .line_info = fun_data_ml_postprocessing_ultralytics_YoloV8___init__ + 9,
        .line_info_top = fun_data_ml_postprocessing_ultralytics_YoloV8___init__ + 13,
        .opcodes = fun_data_ml_postprocessing_ultralytics_YoloV8___init__ + 13,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_ultralytics_YoloV8___init__ fun_data_ml_postprocessing_ultralytics_YoloV8___init__[0]
#endif

// child of ml_postprocessing_ultralytics_YoloV8
// frozen bytecode for file ml/postprocessing/ultralytics.py, scope ml_postprocessing_ultralytics_YoloV8___call__
static const byte fun_data_ml_postprocessing_ultralytics_YoloV8___call__[374] = {
    0xb8,0x24,0x40, // prelude
    0x0e,0x23,0x24,0x25,0x26, // names: __call__, self, model, inputs, outputs
    0x80,0x69,0x2a,0x26,0x29,0x64,0x72,0x2d,0x2f,0x27,0x63,0x72,0x77,0x77,0x2a,0x2b,0x2e,0x6e,0x2f,0x29,0x2a,0x29,0x4a,0x2f,0x2b,0x1f,0x2d, // code info
    0xb1, // LOAD_FAST 1
    0x13,0x0f, // LOAD_ATTR 'output_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x03, // UNPACK_SEQUENCE 3
    0xc4, // STORE_FAST 4
    0xc5, // STORE_FAST 5
    0xc6, // STORE_FAST 6
    0xb1, // LOAD_FAST 1
    0x13,0x10, // LOAD_ATTR 'output_scale'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0xc7, // STORE_FAST 7
    0x12,0x27, // LOAD_GLOBAL 'quantize'
    0xb1, // LOAD_FAST 1
    0xb0, // LOAD_FAST 0
    0x13,0x0b, // LOAD_ATTR 'threshold'
    0x34,0x02, // CALL_FUNCTION 2
    0xc8, // STORE_FAST 8
    0xb5, // LOAD_FAST 5
    0x84, // LOAD_CONST_SMALL_INT 4
    0xf3, // BINARY_OP 28 __sub__
    0xc9, // STORE_FAST 9
    0xb3, // LOAD_FAST 3
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x14,0x11, // LOAD_METHOD 'reshape'
    0xb4, // LOAD_FAST 4
    0x84, // LOAD_CONST_SMALL_INT 4
    0xb9, // LOAD_FAST 9
    0xf2, // BINARY_OP 27 __add__
    0xf4, // BINARY_OP 29 __mul__
    0xb6, // LOAD_FAST 6
    0x2a,0x02, // BUILD_TUPLE 2
    0x36,0x01, // CALL_METHOD 1
    0x13,0x1b, // LOAD_ATTR 'T'
    0xca, // STORE_FAST 10
    0xba, // LOAD_FAST 10
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xcb, // STORE_FAST 11
    0x12,0x0b, // LOAD_GLOBAL 'threshold'
    0xbb, // LOAD_FAST 11
    0xb8, // LOAD_FAST 8
    0xb7, // LOAD_FAST 7
    0x10,0x1c, // LOAD_CONST_STRING 'find_max'
    0x52, // LOAD_CONST_TRUE
    0x10,0x1d, // LOAD_CONST_STRING 'find_max_axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x34,0x84,0x03, // CALL_FUNCTION 515
    0xcb, // STORE_FAST 11
    0x12,0x28, // LOAD_GLOBAL 'len'
    0xbb, // LOAD_FAST 11
    0x34,0x01, // CALL_FUNCTION 1
    0x43,0x43, // POP_JUMP_IF_TRUE 3
    0x23,0x02, // LOAD_CONST_OBJ 2
    0x63, // RETURN_VALUE
    0x12,0x29, // LOAD_GLOBAL 'dequantize'
    0xb1, // LOAD_FAST 1
    0x12,0x1f, // LOAD_GLOBAL 'np'
    0x14,0x12, // LOAD_METHOD 'take'
    0xba, // LOAD_FAST 10
    0xbb, // LOAD_FAST 11
    0x10,0x13, // LOAD_CONST_STRING 'axis'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x36,0x82,0x02, // CALL_METHOD 258
    0x34,0x02, // CALL_FUNCTION 2
    0xcc, // STORE_FAST 12
    0x12,0x1f, // LOAD_GLOBAL 'np'
    0x14,0x1e, // LOAD_METHOD 'max'
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x10,0x13, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0xcd, // STORE_FAST 13
    0x12,0x1f, // LOAD_GLOBAL 'np'
    0x14,0x14, // LOAD_METHOD 'argmax'
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x84, // LOAD_CONST_SMALL_INT 4
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x10,0x13, // LOAD_CONST_STRING 'axis'
    0x81, // LOAD_CONST_SMALL_INT 1
    0x36,0x82,0x01, // CALL_METHOD 257
    0xce, // STORE_FAST 14
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0xcf, // STORE_FAST 15
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x81, // LOAD_CONST_SMALL_INT 1
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x26,0x10, // STORE_FAST_N 16
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x82, // LOAD_CONST_SMALL_INT 2
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x11, // STORE_FAST_N 17
    0xbc, // LOAD_FAST 12
    0x51, // LOAD_CONST_NONE
    0x51, // LOAD_CONST_NONE
    0x2e,0x02, // BUILD_SLICE 2
    0x83, // LOAD_CONST_SMALL_INT 3
    0x2a,0x02, // BUILD_TUPLE 2
    0x55, // LOAD_SUBSCR
    0x23,0x03, // LOAD_CONST_OBJ 3
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x12, // STORE_FAST_N 18
    0xb1, // LOAD_FAST 1
    0x13,0x15, // LOAD_ATTR 'input_shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x30,0x04, // UNPACK_SEQUENCE 4
    0x26,0x13, // STORE_FAST_N 19
    0x26,0x14, // STORE_FAST_N 20
    0x26,0x15, // STORE_FAST_N 21
    0x26,0x16, // STORE_FAST_N 22
    0xbf, // LOAD_FAST 15
    0x24,0x11, // LOAD_FAST_N 17
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x15, // LOAD_FAST_N 21
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x17, // STORE_FAST_N 23
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x12, // LOAD_FAST_N 18
    0xf3, // BINARY_OP 28 __sub__
    0x24,0x14, // LOAD_FAST_N 20
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x18, // STORE_FAST_N 24
    0xbf, // LOAD_FAST 15
    0x24,0x11, // LOAD_FAST_N 17
    0xf2, // BINARY_OP 27 __add__
    0x24,0x15, // LOAD_FAST_N 21
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x19, // STORE_FAST_N 25
    0x24,0x10, // LOAD_FAST_N 16
    0x24,0x12, // LOAD_FAST_N 18
    0xf2, // BINARY_OP 27 __add__
    0x24,0x14, // LOAD_FAST_N 20
    0xf4, // BINARY_OP 29 __mul__
    0x26,0x1a, // STORE_FAST_N 26
    0x12,0x2a, // LOAD_GLOBAL 'NMS'
    0x24,0x15, // LOAD_FAST_N 21
    0x24,0x14, // LOAD_FAST_N 20
    0xb2, // LOAD_FAST 2
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x13,0x16, // LOAD_ATTR 'roi'
    0x34,0x03, // CALL_FUNCTION 3
    0x26,0x1b, // STORE_FAST_N 27
    0xbc, // LOAD_FAST 12
    0x13,0x17, // LOAD_ATTR 'shape'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x55, // LOAD_SUBSCR
    0x80, // LOAD_CONST_SMALL_INT 0
    0x42,0x68, // JUMP 40
    0x57, // DUP_TOP
    0x26,0x1c, // STORE_FAST_N 28
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x18, // LOAD_METHOD 'add_bounding_box'
    0x24,0x17, // LOAD_FAST_N 23
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x24,0x18, // LOAD_FAST_N 24
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x24,0x19, // LOAD_FAST_N 25
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x24,0x1a, // LOAD_FAST_N 26
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0xbd, // LOAD_FAST 13
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0xbe, // LOAD_FAST 14
    0x24,0x1c, // LOAD_FAST_N 28
    0x55, // LOAD_SUBSCR
    0x36,0x06, // CALL_METHOD 6
    0x59, // POP_TOP
    0x81, // LOAD_CONST_SMALL_INT 1
    0xe5, // BINARY_OP 14 __iadd__
    0x58, // DUP_TOP_TWO
    0x5a, // ROT_TWO
    0xd7, // BINARY_OP 0 __lt__
    0x43,0x13, // POP_JUMP_IF_TRUE -45
    0x59, // POP_TOP
    0x59, // POP_TOP
    0x24,0x1b, // LOAD_FAST_N 27
    0x14,0x19, // LOAD_METHOD 'get_bounding_boxes'
    0x10,0x0b, // LOAD_CONST_STRING 'threshold'
    0xb0, // LOAD_FAST 0
    0x13,0x0c, // LOAD_ATTR 'nms_threshold'
    0x10,0x1a, // LOAD_CONST_STRING 'sigma'
    0xb0, // LOAD_FAST 0
    0x13,0x0d, // LOAD_ATTR 'nms_sigma'
    0x36,0x84,0x00, // CALL_METHOD 512
    0x63, // RETURN_VALUE
};
#if MICROPY_PERSISTENT_CODE_SAVE
static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_ultralytics_YoloV8___call__ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_ultralytics_YoloV8___call__,
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 374,
    .n_children = 0,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 40,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 4,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 14,
        .line_info = fun_data_ml_postprocessing_ultralytics_YoloV8___call__ + 8,
        .line_info_top = fun_data_ml_postprocessing_ultralytics_YoloV8___call__ + 35,
        .opcodes = fun_data_ml_postprocessing_ultralytics_YoloV8___call__ + 35,
    },
    #endif
    #endif
};
#else
#define proto_fun_ml_postprocessing_ultralytics_YoloV8___call__ fun_data_ml_postprocessing_ultralytics_YoloV8___call__[0]
#endif

static const mp_raw_code_t *const children_ml_postprocessing_ultralytics_YoloV8[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_ultralytics_YoloV8___init__,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_ultralytics_YoloV8___call__,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_ultralytics_YoloV8 = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_ultralytics_YoloV8,
    .children = (void *)&children_ml_postprocessing_ultralytics_YoloV8,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 36,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 9,
        .line_info = fun_data_ml_postprocessing_ultralytics_YoloV8 + 3,
        .line_info_top = fun_data_ml_postprocessing_ultralytics_YoloV8 + 9,
        .opcodes = fun_data_ml_postprocessing_ultralytics_YoloV8 + 9,
    },
    #endif
    #endif
};

static const mp_raw_code_t *const children_ml_postprocessing_ultralytics__lt_module_gt_[] = {
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_ultralytics_YoloV5,
    (const mp_raw_code_t *)&proto_fun_ml_postprocessing_ultralytics_YoloV8,
};

static const mp_raw_code_truncated_t proto_fun_ml_postprocessing_ultralytics__lt_module_gt_ = {
    .proto_fun_indicator[0] = MP_PROTO_FUN_INDICATOR_RAW_CODE_0,
    .proto_fun_indicator[1] = MP_PROTO_FUN_INDICATOR_RAW_CODE_1,
    .kind = MP_CODE_BYTECODE,
    .is_generator = 0,
    .fun_data = fun_data_ml_postprocessing_ultralytics__lt_module_gt_,
    .children = (void *)&children_ml_postprocessing_ultralytics__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .fun_data_len = 63,
    .n_children = 2,
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    #endif
    #if MICROPY_PY_SYS_SETTRACE
    .line_of_definition = 0,
    .prelude = {
        .n_state = 3,
        .n_exc_stack = 0,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data_ml_postprocessing_ultralytics__lt_module_gt_ + 3,
        .line_info_top = fun_data_ml_postprocessing_ultralytics__lt_module_gt_ + 11,
        .opcodes = fun_data_ml_postprocessing_ultralytics__lt_module_gt_ + 11,
    },
    #endif
    #endif
};

static const qstr_short_t const_qstr_table_data_ml_postprocessing_ultralytics[43] = {
    MP_QSTR_ml_slash_postprocessing_slash_ultralytics_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR__star_,
    MP_QSTR_ml_dot_utils,
    MP_QSTR_const,
    MP_QSTR_micropython,
    MP_QSTR_numpy,
    MP_QSTR_ulab,
    MP_QSTR_YoloV5,
    MP_QSTR_YoloV8,
    MP_QSTR___init__,
    MP_QSTR_threshold,
    MP_QSTR_nms_threshold,
    MP_QSTR_nms_sigma,
    MP_QSTR___call__,
    MP_QSTR_output_shape,
    MP_QSTR_output_scale,
    MP_QSTR_reshape,
    MP_QSTR_take,
    MP_QSTR_axis,
    MP_QSTR_argmax,
    MP_QSTR_input_shape,
    MP_QSTR_roi,
    MP_QSTR_shape,
    MP_QSTR_add_bounding_box,
    MP_QSTR_get_bounding_boxes,
    MP_QSTR_sigma,
    MP_QSTR_T,
    MP_QSTR_find_max,
    MP_QSTR_find_max_axis,
    MP_QSTR_max,
    MP_QSTR_np,
    MP_QSTR___name__,
    MP_QSTR___module__,
    MP_QSTR___qualname__,
    MP_QSTR_self,
    MP_QSTR_model,
    MP_QSTR_inputs,
    MP_QSTR_outputs,
    MP_QSTR_quantize,
    MP_QSTR_len,
    MP_QSTR_dequantize,
    MP_QSTR_NMS,
};

// constants
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_ultralytics_0 = {{&mp_type_float}, (mp_float_t)0.6};
#define const_obj_ml_postprocessing_ultralytics_0_macro MP_ROM_PTR(&const_obj_ml_postprocessing_ultralytics_0)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_ultralytics_0_macro ((mp_rom_obj_t)(0xbf99999a))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_ultralytics_0_macro ((mp_rom_obj_t)(0xbfe7333333333333))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_ultralytics_1 = {{&mp_type_float}, (mp_float_t)0.1};
#define const_obj_ml_postprocessing_ultralytics_1_macro MP_ROM_PTR(&const_obj_ml_postprocessing_ultralytics_1)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_ultralytics_1_macro ((mp_rom_obj_t)(0xbe4cccce))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_ultralytics_1_macro ((mp_rom_obj_t)(0xbfbd99999999999a))
#endif
#if MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_A || MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_B
static const mp_obj_float_t const_obj_ml_postprocessing_ultralytics_3 = {{&mp_type_float}, (mp_float_t)0.5};
#define const_obj_ml_postprocessing_ultralytics_3_macro MP_ROM_PTR(&const_obj_ml_postprocessing_ultralytics_3)
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_C
#define const_obj_ml_postprocessing_ultralytics_3_macro ((mp_rom_obj_t)(0xbf800002))
#elif MICROPY_OBJ_REPR == MICROPY_OBJ_REPR_D
#define const_obj_ml_postprocessing_ultralytics_3_macro ((mp_rom_obj_t)(0xbfe4000000000000))
#endif

// constant table
static const mp_rom_obj_t const_obj_table_data_ml_postprocessing_ultralytics[4] = {
    const_obj_ml_postprocessing_ultralytics_0_macro,
    const_obj_ml_postprocessing_ultralytics_1_macro,
    MP_ROM_PTR(&mp_const_empty_tuple_obj),
    const_obj_ml_postprocessing_ultralytics_3_macro,
};

static const mp_frozen_module_t frozen_module_ml_postprocessing_ultralytics = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data_ml_postprocessing_ultralytics,
        .obj_table = (mp_obj_t *)&const_obj_table_data_ml_postprocessing_ultralytics,
    },
    .proto_fun = &proto_fun_ml_postprocessing_ultralytics__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// collection of all frozen modules

const char mp_frozen_names[] = {
    #ifdef MP_FROZEN_STR_NAMES
    MP_FROZEN_STR_NAMES
    #endif
    "ml/apps.py\0"
    "ml/model.py\0"
    "ml/preprocessing.py\0"
    "ml/utils.py\0"
    "ml/__init__.py\0"
    "ml/postprocessing/darknet.py\0"
    "ml/postprocessing/edgeimpulse.py\0"
    "ml/postprocessing/mediapipe.py\0"
    "ml/postprocessing/ultralytics.py\0"
    "\0"
};

const mp_frozen_module_t *const mp_frozen_mpy_content[] = {
    &frozen_module_ml_apps,
    &frozen_module_ml_model,
    &frozen_module_ml_preprocessing,
    &frozen_module_ml_utils,
    &frozen_module_ml___init__,
    &frozen_module_ml_postprocessing_darknet,
    &frozen_module_ml_postprocessing_edgeimpulse,
    &frozen_module_ml_postprocessing_mediapipe,
    &frozen_module_ml_postprocessing_ultralytics,
};

#ifdef MICROPY_FROZEN_LIST_ITEM
MICROPY_FROZEN_LIST_ITEM("ml/apps", "ml/apps.py")
MICROPY_FROZEN_LIST_ITEM("ml/model", "ml/model.py")
MICROPY_FROZEN_LIST_ITEM("ml/preprocessing", "ml/preprocessing.py")
MICROPY_FROZEN_LIST_ITEM("ml/utils", "ml/utils.py")
MICROPY_FROZEN_LIST_ITEM("ml", "ml/__init__.py")
MICROPY_FROZEN_LIST_ITEM("ml/postprocessing/darknet", "ml/postprocessing/darknet.py")
MICROPY_FROZEN_LIST_ITEM("ml/postprocessing/edgeimpulse", "ml/postprocessing/edgeimpulse.py")
MICROPY_FROZEN_LIST_ITEM("ml/postprocessing/mediapipe", "ml/postprocessing/mediapipe.py")
MICROPY_FROZEN_LIST_ITEM("ml/postprocessing/ultralytics", "ml/postprocessing/ultralytics.py")
#endif

/*
byte sizes:
qstr content: 116 unique, 1611 bytes
bc content: 8403
const str content: 106
const int content: 0
const obj content: 672
const table qstr content: 0 entries, 0 bytes
const table ptr content: 71 entries, 284 bytes
raw code content: 77 * 4 = 1232
mp_frozen_mpy_names_content: 197
mp_frozen_mpy_content_size: 36
total: 12541
*/
