/*
 * ML Backend Stub Implementation for Step 1 Testing
 *
 * This file provides empty implementations of ml_backend_* functions
 * when MICROPY_PY_ML_TFLM is not defined, allowing us to test if
 * the ML module compilation affects USB reception without actually
 * linking the TFLM library.
 *
 * NOTE: This file is only compiled when MICROPY_PY_ML_TFLM is NOT defined.
 *       When TFLM is enabled, tflm_backend.cc provides the real implementation.
 */

#include "py/runtime.h"
#include "py/obj.h"
#include "imlib_config.h"
#include "py_ml.h"

#if MICROPY_PY_ML && !defined(MICROPY_PY_ML_TFLM)

// Initialize a model - stub returns error
int ml_backend_init_model(py_ml_model_obj_t *model) {
    mp_raise_msg(&mp_type_NotImplementedError,
                 MP_ERROR_TEXT("ML backend not available (TFLM disabled)"));
    return -1;
}

// Run inference - stub returns error
int ml_backend_run_inference(py_ml_model_obj_t *model) {
    mp_raise_msg(&mp_type_NotImplementedError,
                 MP_ERROR_TEXT("ML backend not available (TFLM disabled)"));
    return -1;
}

// Run any backend deinitialization needed when a model is released.
// 无后端可清理，直接返回（与 tflm_backend.cc 的同名函数一致）。
void ml_backend_deinit_model(py_ml_model_obj_t *model) {
    (void) model;
}

// Get input tensor - stub returns NULL
void *ml_backend_get_input(py_ml_model_obj_t *model, size_t index) {
    mp_raise_msg(&mp_type_NotImplementedError,
                 MP_ERROR_TEXT("ML backend not available (TFLM disabled)"));
    return NULL;
}

// Get output tensor - stub returns NULL
void *ml_backend_get_output(py_ml_model_obj_t *model, size_t index) {
    mp_raise_msg(&mp_type_NotImplementedError,
                 MP_ERROR_TEXT("ML backend not available (TFLM disabled)"));
    return NULL;
}

#endif // MICROPY_PY_ML && !defined(MICROPY_PY_ML_TFLM)
