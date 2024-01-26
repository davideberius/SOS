#include "shmem_tools.h"
#include "shmem_team.h"
#include <inttypes.h>

int shmem_internal_tools_initialized = 0;
int shmem_internal_tools_finalized = 0;

long long *shmem_internal_pvar_data;
long long *shmem_internal_cvar_data;

const uint64_t SHMEM_INTERNAL_TOOLS_TYPE_MASK = 3;
const uint64_t SHMEM_INTERNAL_TOOLS_CLASS_MASK = 60;
const uint64_t SHMEM_INTERNAL_TOOLS_BINDING_MASK = 960;
const uint64_t SHMEM_INTERNAL_TOOLS_OPERATION_MASK = 4193280;
const uint64_t SHMEM_INTERNAL_TOOLS_SCOPE_MASK = 4290772992;
const uint64_t SHMEM_INTERNAL_TOOLS_OFFSET_MASK = 0xFFFFFFFF00000000;

const uint64_t SHMEM_INTERNAL_TOOLS_ZERO = 0;

uint64_t shmem_internal_tools_pvar_bitstrings[] = {
    // PUT
    SHMEM_INTERNAL_TOOLS_TYPE_PVAR |
    (SHMEM_INTERNAL_TOOLS_CLASS_COUNTER << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
    ((uint64_t)SHMEM_INTERNAL_TOOLS_PUT << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT),
    // GET
    SHMEM_INTERNAL_TOOLS_TYPE_PVAR |
    (SHMEM_INTERNAL_TOOLS_CLASS_COUNTER << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
    ((uint64_t)SHMEM_INTERNAL_TOOLS_GET << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT),
    // BYTES_READ
    SHMEM_INTERNAL_TOOLS_TYPE_PVAR |
    (SHMEM_INTERNAL_TOOLS_CLASS_COUNTER << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
    ((uint64_t)SHMEM_INTERNAL_TOOLS_BYTES_READ << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT),
    // BYTES_WRITTEN
    SHMEM_INTERNAL_TOOLS_TYPE_PVAR |
    (SHMEM_INTERNAL_TOOLS_CLASS_COUNTER << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
    ((uint64_t)SHMEM_INTERNAL_TOOLS_BYTES_WRITTEN << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT),
    // BYTES_READ_USER
    SHMEM_INTERNAL_TOOLS_TYPE_PVAR |
    (SHMEM_INTERNAL_TOOLS_CLASS_COUNTER << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
    ((uint64_t)SHMEM_INTERNAL_TOOLS_BYTES_READ_USER << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT),
    // BYTES_WRITTEN_USER
    SHMEM_INTERNAL_TOOLS_TYPE_PVAR |
    (SHMEM_INTERNAL_TOOLS_CLASS_COUNTER << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
    ((uint64_t)SHMEM_INTERNAL_TOOLS_BYTES_WRITTEN_USER << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT)
};

const uint64_t shmem_internal_tools_cvar_bitstrings[] = {
    // BOUNCE_BUFFER_SIZE
    SHMEM_INTERNAL_TOOLS_TYPE_CVAR |
    (SHMEM_INTERNAL_TOOLS_CLASS_CONTROL << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
    ((uint64_t)SHMEM_INTERNAL_TOOLS_BOUNCE_BUFFER_SIZE << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT)
};


const char* const shmem_internal_tools_pvar_names[] = {
    "SHMEM_PUT",
    "SHMEM_IPUT",
    "SHMEM_GET",
    "SHMEM_IGET",
    "BYTES_READ",
    "BYTES_WRITTEN",
    "BYTES_READ_USER",
    "BYTES_WRITTEN_USER",
    NULL
};

const char* const shmem_internal_tools_pvar_descriptions[] = {
    "The number of times shmem_put is called.",
    "The number of times shmem_iput is called.",
    "The number of times shmem_get is called.",
    "The number of times shmem_iget is called.",
    "The total number of bytes read through get or similar operations.",
    "The total number of bytes written through put or similar operations.",
    "The number of bytes read through get or similar operations explicitly initiated by the user.",
    "The number of bytes written through put or similar operations explicitly initiated by the user.",
    NULL
};

const char* const shmem_internal_tools_cvar_names[] = {
    "SHMEM_BOUNCE_BUFFER_SIZE",
    NULL
};

const char* const shmem_internal_tools_cvar_descriptions[] = {
    "The current size of the SHMEM internal bounce buffer.",
    NULL
};

const char* const shmem_internal_tools_type_names[] = {
    "PVAR",
    "CVAR",
    NULL
};

const char* const shmem_internal_tools_type_descriptions[] = {
    "Performance variables.",
    "Control variables.",
    NULL
};

const char* const shmem_internal_tools_class_names[] = {
    "Counter",
    "Watermark",
    "Control",
    NULL
};

const char* const shmem_internal_tools_class_descriptions[] = {
    "A variable that increments each time a certain event happens.",
    "A variable that keeps track of a high watermark for a value.",
    "A variable that stores a control value in the implementation.",
    NULL
};

/* SHMEM tools user-facing functions */
int
shmem_internal_tools_init(int tl_requested, int *tl_provided)
{
    int ret;

    if (shmem_internal_tools_initialized) {
        ret = -1;
        return ret;
    }

    shmem_internal_pvar_data = (long long*)calloc(SHMEM_INTERNAL_TOOLS_NUM_PVARS, sizeof(long long));
    shmem_internal_cvar_data = (long long*)calloc(SHMEM_INTERNAL_TOOLS_NUM_CVARS, sizeof(long long));

    shmem_internal_tools_initialized = 1;
    ret = 0;

    return ret;
}

void
shmem_internal_tools_finalize(void)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return;
    }

    shmem_internal_tools_finalized = 1;
    // Unset the initialized flag so API functions return early
    shmem_internal_tools_initialized = 0;

    int my_pe = shmem_internal_team_world.my_pe;

    printf("###### SHMEM Performance Variables ######\n");
    for(int i = 0; i < SHMEM_INTERNAL_TOOLS_NUM_PVARS; i++) {
        printf("[%d] %s: %lld\n", my_pe, shmem_internal_tools_pvar_names[i], shmem_internal_pvar_data[i]);
    }

    printf("###### SHMEM Control Variables ######\n");
    for(int i = 0; i < SHMEM_INTERNAL_TOOLS_NUM_CVARS; i++) {
        printf("[%d] %s: %lld\n", my_pe, shmem_internal_tools_cvar_names[i], shmem_internal_cvar_data[i]);
    }
    printf("#####################################\n");

    free(shmem_internal_pvar_data);
    free(shmem_internal_cvar_data);
}

void
shmem_internal_tools_var_get_num(int type, int *num)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return;
    }

    if(type == SHMEM_TOOLS_PVAR_TYPE)
        *num = SHMEM_INTERNAL_TOOLS_NUM_PVARS;
    else if(type == SHMEM_TOOLS_CVAR_TYPE)
        *num = SHMEM_INTERNAL_TOOLS_NUM_CVARS;
    else
        *num = 0;
}

void
shmem_internal_tools_var_avail(int type, shmem_t_var **list_vars)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return;
    }

    if(type == SHMEM_INTERNAL_TOOLS_TYPE_PVAR) {
        for(int i = 0; i < SHMEM_INTERNAL_TOOLS_NUM_PVARS; i++) {
            (*list_vars)[i].id = i;
            (*list_vars)[i].var = shmem_internal_tools_pvar_bitstrings[i];
        }
    }
    else if(type == SHMEM_INTERNAL_TOOLS_TYPE_CVAR) {
        for(int i = 0; i < SHMEM_INTERNAL_TOOLS_NUM_CVARS; i++) {
            (*list_vars)[i].id = i;
            (*list_vars)[i].var = shmem_internal_tools_cvar_bitstrings[i];
        }
    }
}

void
shmem_internal_tools_var_handle_alloc(shmem_t_handle_config handle_config, shmem_t_var_handle *handle)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return;
    }

    uint64_t var = handle_config.var;

    (*handle).type = (var & SHMEM_INTERNAL_TOOLS_TYPE_MASK) >> SHMEM_INTERNAL_TOOLS_TYPE_SHIFT;
    (*handle).offset = (var & SHMEM_INTERNAL_TOOLS_OFFSET_MASK) >> SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT;
    (*handle).var = handle_config.var;
    (*handle).shmem_obj_handle = handle_config.shmem_obj_handle;
}

void
shmem_internal_tools_var_handle_free(shmem_t_var_handle *handle)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return;
    }

    /* Set the type to a nonexistant type to identify this as a
     * stale handle.
     */
    (*handle).type = SHMEM_INTERNAL_TOOLS_NUM_TYPES;
    (*handle).offset = 0;
    (*handle).var = 0;
    (*handle).shmem_obj_handle = NULL;
}

int
shmem_internal_tools_var_reset(shmem_t_var_handle handle)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return -1;
    }

    if(handle.type == SHMEM_INTERNAL_TOOLS_TYPE_PVAR) {
        shmem_internal_pvar_data[handle.offset] = 0;
    }
    else if(handle.type == SHMEM_INTERNAL_TOOLS_TYPE_CVAR) {
        shmem_internal_cvar_data[handle.offset] = 0;
    }
    else {
        return -1;
    }
    return 0;
}

int
shmem_internal_tools_var_read(shmem_t_var_handle handle, void *buf)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return -1;
    }

    if(handle.type == SHMEM_INTERNAL_TOOLS_TYPE_PVAR) {
        *(long long*)buf = shmem_internal_pvar_data[handle.offset];
    }
    else if(handle.type == SHMEM_INTERNAL_TOOLS_TYPE_CVAR) {
        *(long long*)buf = shmem_internal_cvar_data[handle.offset];
    }
    else {
        return -1;
    }
    return 0;
}

int
shmem_internal_tools_var_write(shmem_t_var_handle handle, const void *buf)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return -1;
    }

    if(handle.type == SHMEM_INTERNAL_TOOLS_TYPE_PVAR) {
        shmem_internal_pvar_data[handle.offset] = *(long long*)buf;
    }
    else if(handle.type == SHMEM_INTERNAL_TOOLS_TYPE_CVAR) {
        shmem_internal_cvar_data[handle.offset] = *(long long*)buf;
    }
    else {
        return -1;
    }
    return 0;
}

int
shmem_internal_tools_var_list_read(shmem_t_var_handle *handles, void **buf, int count)
{
    long long** b = (long long**)buf;
    for(int i = 0; i < count; i++) {
        if(handles[i].type == SHMEM_INTERNAL_TOOLS_TYPE_PVAR) {
            (*b)[i] = shmem_internal_pvar_data[handles[i].offset];
        }
        else if(handles[i].type == SHMEM_INTERNAL_TOOLS_TYPE_CVAR) {
            (*b)[i] = shmem_internal_cvar_data[handles[i].offset];
        }
        else {
            return -1;
        }
    }
    return 0;
}

int
shmem_internal_tools_get_var(int var_type, int operation, int bind, int var_scope, int var_class, int var_offset, uint64_t *var)
{
    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        return -1;
    }

    *var = (var_type << SHMEM_INTERNAL_TOOLS_TYPE_SHIFT) |
        (operation << SHMEM_INTERNAL_TOOLS_OPERATION_SHIFT) |
        (bind << SHMEM_INTERNAL_TOOLS_BINDING_SHIFT) |
        (var_scope << SHMEM_INTERNAL_TOOLS_SCOPE_SHIFT) |
        (var_class << SHMEM_INTERNAL_TOOLS_CLASS_SHIFT) |
        (var_offset << SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT);
    return 0;
}

/* SHMEM tools internal use only functions */

int
shmem_internal_tools_counter_update(int counter_id, long long value)
{
    int ret = 0;

    if (!shmem_internal_tools_initialized ||
        shmem_internal_tools_finalized) {
        ret = -1;
        return ret;
    }
    

    if(counter_id > SHMEM_INTERNAL_TOOLS_NUM_PVARS || counter_id < 0) {
        ret = 1;
        fprintf(stderr, "Attempted to write to unknown tool variable.\n");
        return ret;
    }

    shmem_internal_pvar_data[counter_id] += value;

    return ret;
}

