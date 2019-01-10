#include <stdio.h>
#include <assert.h>
#include "../proto/protobuf-c.h"
#include "../proto/sm.pb-c.h"
#include "../include/sm_api.h"
#include "../include/util.h"
#include "../include/data.h"
#include "../include/device.h"
#include "../include/context.h"
#include "_hardsm.h"


int api_init(uint8_t *out) {
    int error_code = YERR_SUCCESS;
    Response response = RESPONSE__INIT;

    error_code = init();
    if (error_code != YERR_SUCCESS) goto fail;

    return empty_response(&response, out);

fail:
    return fail_response(&response, error_code, out);
}

int api_final(uint8_t *out) {
    int error_code = YERR_SUCCESS;
    Response response = RESPONSE__INIT;

    error_code = final();
    if (error_code != YERR_SUCCESS) goto fail;

    return empty_response(&response, out);

fail:
    return fail_response(&response, error_code, out);
}

int api_print_context(bool verbose, uint8_t *out) {
    Response response = RESPONSE__INIT;

    char buf[32 * 1024] = {0};
    ctx_print_context(buf, sizeof(buf), verbose);

    return str_response(&response, buf, out);
}

int api_login_device(int device_index, const char *pin_code, uint8_t *out) {
    int error_code = YERR_SUCCESS;
    Response response = RESPONSE__INIT;

    error_code = ctx_open_device(device_index);
    if (error_code != YERR_SUCCESS) goto fail;

    error_code = ctx_check_device(device_index);
    if (error_code != YERR_SUCCESS) goto fail;

    error_code = ctx_open_all_pipes(device_index);
    if (error_code != YERR_SUCCESS) goto fail;

    error_code = ctx_login(device_index, pin_code);
    if (error_code != YERR_SUCCESS) goto fail;

    return empty_response(&response, out);

fail:
    ctx_free_device(device_index);
    return fail_response(&response, error_code, out);
}

int api_logout_device(int device_index, uint8_t *out) {
    int error_code = YERR_SUCCESS;
    Response response = RESPONSE__INIT;

    error_code = ctx_free_device(device_index);
    if (error_code != YERR_SUCCESS) goto fail;

    return empty_response(&response, out);

fail:
    return fail_response(&response, error_code, out);
}

int api_device_status(int device_index, uint8_t *out) {
    Response response = RESPONSE__INIT;

    DeviceStatus device_status = ctx_get_device_status(device_index);
    return device_status_response(&response, &device_status, out);
}

int api_device_count(uint8_t *out) {
    Response response = RESPONSE__INIT;

    int count = ctx_device_count();
    return int_response(&response, count, out);
}
