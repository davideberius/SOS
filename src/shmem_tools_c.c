#define SHMEM_INTERNAL_INCLUDE
#include "shmem.h"
#include "shmem_internal.h"

#ifdef ENABLE_PROFILING
#include "pshmem.h"

#pragma weak shmem_t_init = pshmem_t_init
#define shmem_t_init pshmem_t_init

#pragma weak shmem_t_finalize = pshmem_t_finalize
#define shmem_t_finalize pshmem_t_finalize

#endif /* ENABLE_PROFILING */

void SHMEM_FUNCTION_ATTRIBUTES
shmem_t_init(void)
{
    int tl_provided, ret;

    if (shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("attempt to reinitialize tools");
    }

    ret = shmem_internal_tools_init(SHMEM_THREAD_SINGLE, &tl_provided);
    if (ret) abort();
}

void SHMEM_FUNCTION_ATTRIBUTES
shmem_t_finalize(void)
{
    SHMEM_ERR_CHECK_TOOLS_INITIALIZED();

    shmem_internal_tools_finalize();
}

void SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_get_num(int type, int *num)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    shmem_internal_tools_var_get_num(type, num);
}

void SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_avail(int type, shmem_t_var **list_vars)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    shmem_internal_tools_var_avail(type, list_vars);
}

void SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_handle_alloc(shmem_t_handle_config handle_config, shmem_t_var_handle *handle)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    shmem_internal_tools_var_handle_alloc(handle_config, handle);
}

void SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_handle_free(shmem_t_var_handle *handle)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    shmem_internal_tools_var_handle_free(handle);
}

int SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_reset(shmem_t_var_handle handle)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    return shmem_internal_tools_var_reset(handle);
}

int SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_read(shmem_t_var_handle handle, void *buf)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    return shmem_internal_tools_var_read(handle, buf);
}

int SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_write(shmem_t_var_handle handle, const void *buf)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    return shmem_internal_tools_var_write(handle, buf);
}

int SHMEM_FUNCTION_ATTRIBUTES
shmem_t_var_list_read(shmem_t_var_handle *handles, void **buf, int count)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    return shmem_internal_tools_var_list_read(handles, buf, count);
}

int SHMEM_FUNCTION_ATTRIBUTES
shmem_t_get_var(int var_type, int operation, int bind, int var_scope, int var_class, int var_offset, uint64_t *var)
{
    if (!shmem_internal_tools_initialized) {
        RAISE_ERROR_STR("tools interface not initialized");
    }
    return shmem_internal_tools_get_var(var_type, operation, bind, var_scope, var_class, var_offset, var);
}

