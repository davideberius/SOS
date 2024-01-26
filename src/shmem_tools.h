#ifndef SHMEM_INTERNAL_TOOLS_H
#define SHMEM_INTERNAL_TOOLS_H

#include <stdio.h>
#include <stdlib.h>

#include "shmem.h"

/* The SHMEM Tools properties bit string is allocated as follows:
 * Property             | Bits Used
 * Variable Type        | 2
 * Variable Class       | 4
 * Binding              | 4
 * Associated Operation | 12
 * Variable Scope       | 10
 * Other                | 32
 */
// Lowest two bits set to 1
//#define SHMEM_INTERNAL_TOOLS_TYPE_MASK 0x3
//#define 
extern const uint64_t SHMEM_INTERNAL_TOOLS_TYPE_MASK; // 3
#define SHMEM_INTERNAL_TOOLS_TYPE_SHIFT 0
// Next 4 bits set to 1
//#define SHMEM_INTERNAL_TOOLS_CLASS_MASK 0x0000000000000000
//#define 
extern const uint64_t SHMEM_INTERNAL_TOOLS_CLASS_MASK;// = 60
#define SHMEM_INTERNAL_TOOLS_CLASS_SHIFT 2
// Next 4 bits set to 1
//#define SHMEM_INTERNAL_TOOLS_BINDING_MASK 0x00000000000003C0
//#define 
extern const uint64_t SHMEM_INTERNAL_TOOLS_BINDING_MASK;// = 960
#define SHMEM_INTERNAL_TOOLS_BINDING_SHIFT 6
// Next 12 bits set to 1
//#define SHMEM_INTERNAL_TOOLS_OPERATION_MASK 0x00000000003FFC00
//#define 
extern const uint64_t SHMEM_INTERNAL_TOOLS_OPERATION_MASK;// = 4193280
#define SHMEM_INTERNAL_TOOLS_OPERATION_SHIFT 10
// Next 10 bits set to 1
//#define SHMEM_INTERNAL_TOOLS_SCOPE_MASK 0x00000000FFC00000
//#define 
extern const uint64_t SHMEM_INTERNAL_TOOLS_SCOPE_MASK;
#define SHMEM_INTERNAL_TOOLS_SCOPE_SHIFT 22

extern const uint64_t SHMEM_INTERNAL_TOOLS_OFFSET_MASK;
#define SHMEM_INTERNAL_TOOLS_OFFSET_SHIFT 32

extern uint64_t shmem_internal_tools_pvar_bitstrings[];
extern const uint64_t shmem_internal_tools_cvar_bitstrings[];

extern const char* const shmem_internal_tools_type_names[];
extern const char* const shmem_internal_tools_type_descriptions[];

extern const char* const shmem_internal_tools_class_names[];
extern const char* const shmem_internal_tools_class_descriptions[];

typedef enum SHMEM_INTERNAL_TOOLS_PVARS
{
  SHMEM_INTERNAL_TOOLS_PUT,
  SHMEM_INTERNAL_TOOLS_IPUT,
  SHMEM_INTERNAL_TOOLS_GET,
  SHMEM_INTERNAL_TOOLS_IGET,
  SHMEM_INTERNAL_TOOLS_BYTES_READ,
  SHMEM_INTERNAL_TOOLS_BYTES_WRITTEN,
  SHMEM_INTERNAL_TOOLS_BYTES_READ_USER,
  SHMEM_INTERNAL_TOOLS_BYTES_WRITTEN_USER,
  SHMEM_INTERNAL_TOOLS_NUM_PVARS
} SHMEM_INTERNAL_TOOLS_PVARS;

typedef enum SHMEM_INTERNAL_TOOLS_CVARS
{
  SHMEM_INTERNAL_TOOLS_BOUNCE_BUFFER_SIZE,
  SHMEM_INTERNAL_TOOLS_NUM_CVARS
} SHMEM_INTERNAL_TOOLS_CVARS;

typedef enum SHMEM_INTERNAL_TOOLS_TYPES
{
    SHMEM_INTERNAL_TOOLS_TYPE_PVAR,
    SHMEM_INTERNAL_TOOLS_TYPE_CVAR,
    SHMEM_INTERNAL_TOOLS_NUM_TYPES
} SHMEM_INTERNAL_TOOLS_TYPES;

typedef enum SHMEM_INTERNAL_TOOLS_CLASSES
{
    SHMEM_INTERNAL_TOOLS_CLASS_COUNTER,
    SHMEM_INTERNAL_TOOLS_CLASS_WATERMARK,
    SHMEM_INTERNAL_TOOLS_CLASS_CONTROL,
    SHMEM_INTERNAL_TOOLS_NUM_CLASSES
} SHMEM_INTERNAL_TOOLS_CLASSES;

extern const char* const shmem_internal_tools_pvar_names[];
extern const char* const shmem_internal_tools_pvar_descriptions[];

extern const char* const shmem_internal_tools_cvar_names[];
extern const char* const shmem_internal_tools_cvar_descriptions[];

extern const char* const shmem_internal_tools_type_names[];
extern const char* const shmem_internal_tools_type_descriptions[];

extern const char* const shmem_internal_tools_class_names[];
extern const char* const shmem_internal_tools_class_descriptions[];

#ifdef ENABLE_ERROR_CHECKING

#define SHMEM_ERR_CHECK_TOOLS_INITIALIZED()                              \
    do {                                                                 \
        if (!shmem_internal_tools_initialized) {                         \
            RETURN_ERROR_STR(PACKAGE_NAME " shmem tools not initialized\n"); \
            abort();                                                     \
        }                                                                \
    } while (0)

#else
#define SHMEM_ERR_CHECK_TOOLS_INITIALIZED()

#endif /* ENABLE_ERROR_CHECKING */

extern int shmem_internal_tools_initialized;
extern int shmem_internal_tools_finalized;
extern long long *shmem_internal_pvar_data;
extern long long *shmem_internal_cvar_data;

int shmem_internal_tools_init(int tl_requested, int *tl_provided);
void shmem_internal_tools_finalize();

void shmem_internal_tools_var_get_num(int type, int *num);
void shmem_internal_tools_var_avail(int type, shmem_t_var **list_vars);

void shmem_internal_tools_var_handle_alloc(shmem_t_handle_config handle_config, shmem_t_var_handle *handle);
void shmem_internal_tools_var_handle_free(shmem_t_var_handle *handle);

int shmem_internal_tools_var_reset(shmem_t_var_handle handle);
int shmem_internal_tools_var_read(shmem_t_var_handle handle, void *buf);
int shmem_internal_tools_var_write(shmem_t_var_handle handle, const void *buf);
int shmem_internal_tools_var_list_read(shmem_t_var_handle *handles, void **buf, int count);

int shmem_internal_tools_get_var(int var_type, int operation, int bind, int var_scope, int var_class, int var_offset, uint64_t *var);

int shmem_internal_tools_counter_update(int counter_id, long long value);

#endif
