#ifndef __SIMPLE_CONFIG_H__
#define __SIMPLE_CONFIG_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> /* malloc, free   */
#include <string.h> /* memcpy, memset */
#include <ctype.h>  /* isspace        */

struct scfg;

#define SCFG_ERR_NONE       (0)
#define SCFG_ERR_BAD_FILE   (1)
#define SCFG_ERR_BAD_SYNTAX (2)
#define SCFG_ERR_BAD_KEY    (3)
#define SCFG_ERR_BAD_VAL    (4)
#define SCFG_ERR_VALIDATE   (5)

typedef int (*scfg_validate_bool_fn_t)  (struct scfg*, int);
typedef int (*scfg_validate_int_fn_t)   (struct scfg*, int);
typedef int (*scfg_validate_float_fn_t) (struct scfg*, float);
typedef int (*scfg_validate_string_fn_t)(struct scfg*, const char*);

struct scfg* scfg_make(void);
void         scfg_free(struct scfg *cfg);

void         scfg_add_bool(struct scfg *cfg, const char *key, int *addr);
void         scfg_add_int(struct scfg *cfg, const char *key, int *addr);
void         scfg_add_float(struct scfg *cfg, const char *key, float *addr);
void         scfg_add_string(struct scfg *cfg, const char *key, const char **addr);

void         scfg_require(struct scfg *cfg, const char *key);

void         scfg_default_bool(struct scfg *cfg, const char *key, int def);
void         scfg_default_int(struct scfg *cfg, const char *key, int def);
void         scfg_default_float(struct scfg *cfg, const char *key, float def);
void         scfg_default_string(struct scfg *cfg, const char *key, const char *def);

void         scfg_validate_bool(struct scfg *cfg, const char *key, scfg_validate_bool_fn_t validate);
void         scfg_validate_int(struct scfg *cfg, const char *key, scfg_validate_int_fn_t validate);
void         scfg_validate_float(struct scfg *cfg, const char *key, scfg_validate_float_fn_t validate);
void         scfg_validate_string(struct scfg *cfg, const char *key, scfg_validate_string_fn_t validate);

void         scfg_validate_set_err(struct scfg *cfg, const char *msg);

int          scfg_parse(struct scfg *cfg, const char *path);

const char  *scfg_err_msg(struct scfg *cfg);

#endif

#ifdef SIMPLE_CONFIG_IMPL

#define SCFG_KIND_BOOL   (1)
#define SCFG_KIND_INT    (2)
#define SCFG_KIND_FLOAT  (3)
#define SCFG_KIND_STRING (4)

#define SCFG_ENTRY_REQUIRED    (0x1)
#define SCFG_ENTRY_HAS_DEFAULT (0x2)
#define SCFG_ENTRY_SET         (0x4)

typedef struct {
    int kind;
    int flags;
    union {
        int         *baddr;
        int         *iaddr;
        float       *faddr;
        const char **saddr;
        void        *_addr;
    };
    union {
        int         bdef;
        int         idef;
        float       fdef;
        const char *sdef;
    };
    void *validate;
} scfg_entry_t;

static void scfg_entry_free(scfg_entry_t *entry) {
    if (entry->kind == SCFG_KIND_STRING) {
        if (entry->sdef != NULL) {
            free((void*)entry->sdef);
        }
    }
}

typedef const char *scfg_str;

/*********************************************************************
    BEGIN HASH TABLE

    You can ignore this part.
    Generated by running github.com/kammerdienerb/hash_table.h through
    the preprocessor.
**********************************************************************/
uint64_t ht_prime_sizes[] = {5ULL,
                             11ULL,
                             23ULL,
                             47ULL,
                             97ULL,
                             199ULL,
                             409ULL,
                             823ULL,
                             1741ULL,
                             3469ULL,
                             6949ULL,
                             14033ULL,
                             28411ULL,
                             57557ULL,
                             116731ULL,
                             236897ULL,
                             480881ULL,
                             976369ULL,
                             1982627ULL,
                             4026031ULL,
                             8175383ULL,
                             16601593ULL,
                             33712729ULL,
                             68460391ULL,
                             139022417ULL,
                             282312799ULL,
                             573292817ULL,
                             1164186217ULL,
                             2364114217ULL,
                             4294967291ULL,
                             8589934583ULL,
                             17179869143ULL,
                             34359738337ULL,
                             68719476731ULL,
                             137438953447ULL,
                             274877906899ULL,
                             549755813881ULL,
                             1099511627689ULL,
                             2199023255531ULL,
                             4398046511093ULL,
                             8796093022151ULL,
                             17592186044399ULL,
                             35184372088777ULL,
                             70368744177643ULL,
                             140737488355213ULL,
                             281474976710597ULL,
                             562949953421231ULL,
                             1125899906842597ULL,
                             2251799813685119ULL,
                             4503599627370449ULL,
                             9007199254740881ULL,
                             18014398509481951ULL,
                             36028797018963913ULL,
                             72057594037927931ULL,
                             144115188075855859ULL,
                             288230376151711717ULL,
                             576460752303423433ULL,
                             1152921504606846883ULL,
                             2305843009213693951ULL,
                             4611686018427387847ULL,
                             9223372036854775783ULL,
                             18446744073709551557ULL};
struct _hash_table_scfg_str_scfg_entry_t;
typedef struct _hash_table_slot_scfg_str_scfg_entry_t {
    scfg_str _key;
    scfg_entry_t _val;
    uint64_t _hash;
    struct _hash_table_slot_scfg_str_scfg_entry_t * _next;
} * hash_table_slot_scfg_str_scfg_entry_t;
typedef void (*hash_table_scfg_str_scfg_entry_t_free_t)(
    struct _hash_table_scfg_str_scfg_entry_t *);
typedef scfg_str * (*hash_table_scfg_str_scfg_entry_t_get_key_t)(
    struct _hash_table_scfg_str_scfg_entry_t *, scfg_str);
typedef scfg_entry_t * (*hash_table_scfg_str_scfg_entry_t_get_val_t)(
    struct _hash_table_scfg_str_scfg_entry_t *, scfg_str);
typedef void (*hash_table_scfg_str_scfg_entry_t_insert_t)(
    struct _hash_table_scfg_str_scfg_entry_t *, scfg_str, scfg_entry_t);
typedef int (*hash_table_scfg_str_scfg_entry_t_delete_t)(
    struct _hash_table_scfg_str_scfg_entry_t *, scfg_str);
typedef uint64_t (*hash_table_scfg_str_scfg_entry_t_hash_t)(scfg_str);
typedef int (*hash_table_scfg_str_scfg_entry_t_equ_t)(scfg_str, scfg_str);
typedef struct _hash_table_scfg_str_scfg_entry_t {
    hash_table_slot_scfg_str_scfg_entry_t * _data;
    uint64_t len, _size_idx, _load_thresh;
    hash_table_scfg_str_scfg_entry_t_free_t const _free;
    hash_table_scfg_str_scfg_entry_t_get_key_t const _get_key;
    hash_table_scfg_str_scfg_entry_t_get_val_t const _get_val;
    hash_table_scfg_str_scfg_entry_t_insert_t const _insert;
    hash_table_scfg_str_scfg_entry_t_delete_t const _delete;
    hash_table_scfg_str_scfg_entry_t_hash_t const _hash;
    hash_table_scfg_str_scfg_entry_t_equ_t const _equ;
} * hash_table_scfg_str_scfg_entry_t;
static inline hash_table_slot_scfg_str_scfg_entry_t
hash_table_slot_scfg_str_scfg_entry_t_make(scfg_str key, scfg_entry_t val,
                                           uint64_t hash) {
    hash_table_slot_scfg_str_scfg_entry_t slot = malloc(sizeof(*slot));
    slot->_key = key;
    slot->_val = val;
    slot->_hash = hash;
    slot->_next =
        ((void *)0)
        ;
    return slot;
}
static inline void hash_table_scfg_str_scfg_entry_t_rehash_insert(
    hash_table_scfg_str_scfg_entry_t t,
    hash_table_slot_scfg_str_scfg_entry_t insert_slot) {
    uint64_t h, data_size, idx;
    hash_table_slot_scfg_str_scfg_entry_t slot, *slot_ptr;
    h = insert_slot->_hash;
    data_size = ht_prime_sizes[t->_size_idx];
    idx = h % data_size;
    slot_ptr = t->_data + idx;
    while ((slot = *slot_ptr)) {
        slot_ptr = &(slot->_next);
    }
    *slot_ptr = insert_slot;
}
static inline void hash_table_scfg_str_scfg_entry_t_update_load_thresh(
    hash_table_scfg_str_scfg_entry_t t) {
    uint64_t cur_size;
    cur_size = ht_prime_sizes[t->_size_idx];
    t->_load_thresh =
        ((double)((cur_size << 1ULL)) / ((double)(cur_size * 3))) * cur_size;
}
static inline void
hash_table_scfg_str_scfg_entry_t_rehash(hash_table_scfg_str_scfg_entry_t t) {
    uint64_t old_size, new_data_size;
    hash_table_slot_scfg_str_scfg_entry_t *old_data, slot, *slot_ptr, next;
    old_size = ht_prime_sizes[t->_size_idx];
    old_data = t->_data;
    t->_size_idx += 1;
    new_data_size = sizeof(hash_table_slot_scfg_str_scfg_entry_t) *
                    ht_prime_sizes[t->_size_idx];
    t->_data = malloc(new_data_size);
    memset(t->_data, 0, new_data_size);
    for (int i = 0; i < old_size; i += 1) {
        slot_ptr = old_data + i;
        next = *slot_ptr;
        while ((slot = next)) {
            next = slot->_next;
            slot->_next =
                ((void *)0)
                ;
            hash_table_scfg_str_scfg_entry_t_rehash_insert(t, slot);
        }
    }
    free(old_data);
    hash_table_scfg_str_scfg_entry_t_update_load_thresh(t);
}
static inline void
hash_table_scfg_str_scfg_entry_t_insert(hash_table_scfg_str_scfg_entry_t t,
                                        scfg_str key, scfg_entry_t val) {
    uint64_t h, data_size, idx;
    hash_table_slot_scfg_str_scfg_entry_t slot, *slot_ptr;
    h = t->_hash(key);
    data_size = ht_prime_sizes[t->_size_idx];
    idx = h % data_size;
    slot_ptr = t->_data + idx;
    while ((slot = *slot_ptr)) {
        if (((t)->_equ ? (t)->_equ((slot->_key), (key))
                       : (slot->_key) == (key))) {
            slot->_val = val;
            return;
        }
        slot_ptr = &(slot->_next);
    }
    *slot_ptr = hash_table_slot_scfg_str_scfg_entry_t_make(key, val, h);
    t->len += 1;
    if (t->len == t->_load_thresh) {
        hash_table_scfg_str_scfg_entry_t_rehash(t);
    }
}
static inline int
hash_table_scfg_str_scfg_entry_t_delete(hash_table_scfg_str_scfg_entry_t t,
                                        scfg_str key) {
    uint64_t h, data_size, idx;
    hash_table_slot_scfg_str_scfg_entry_t slot, prev, *slot_ptr;
    h = t->_hash(key);
    data_size = ht_prime_sizes[t->_size_idx];
    idx = h % data_size;
    slot_ptr = t->_data + idx;
    prev =
        ((void *)0)
        ;
    while ((slot = *slot_ptr)) {
        if (((t)->_equ ? (t)->_equ((slot->_key), (key))
                       : (slot->_key) == (key))) {
            break;
        }
        prev = slot;
        slot_ptr = &(slot->_next);
    }
    if ((slot = *slot_ptr)) {
        if (prev) {
            prev->_next = slot->_next;
        } else {
            *slot_ptr = slot->_next;
        }
        free(slot);
        t->len -= 1;
        return 1;
    }
    return 0;
}
static inline scfg_str *
hash_table_scfg_str_scfg_entry_t_get_key(hash_table_scfg_str_scfg_entry_t t,
                                         scfg_str key) {
    uint64_t h, data_size, idx;
    hash_table_slot_scfg_str_scfg_entry_t slot, *slot_ptr;
    h = t->_hash(key);
    data_size = ht_prime_sizes[t->_size_idx];
    idx = h % data_size;
    slot_ptr = t->_data + idx;
    while ((slot = *slot_ptr)) {
        if (((t)->_equ ? (t)->_equ((slot->_key), (key))
                       : (slot->_key) == (key))) {
            return &slot->_key;
        }
        slot_ptr = &(slot->_next);
    }
    return
        ((void *)0)
            ;
}
static inline scfg_entry_t *
hash_table_scfg_str_scfg_entry_t_get_val(hash_table_scfg_str_scfg_entry_t t,
                                         scfg_str key) {
    uint64_t h, data_size, idx;
    hash_table_slot_scfg_str_scfg_entry_t slot, *slot_ptr;
    h = t->_hash(key);
    data_size = ht_prime_sizes[t->_size_idx];
    idx = h % data_size;
    slot_ptr = t->_data + idx;
    while ((slot = *slot_ptr)) {
        if (((t)->_equ ? (t)->_equ((slot->_key), (key))
                       : (slot->_key) == (key))) {
            return &slot->_val;
        }
        slot_ptr = &(slot->_next);
    }
    return
        ((void *)0)
            ;
}
static inline void
hash_table_scfg_str_scfg_entry_t_free(hash_table_scfg_str_scfg_entry_t t) {
    for (int i = 0; i < ht_prime_sizes[t->_size_idx]; i += 1) {
        hash_table_slot_scfg_str_scfg_entry_t next, slot = t->_data[i];
        while (slot !=
               ((void *)0)
        ) {
            next = slot->_next;
            free(slot);
            slot = next;
        }
    }
    free(t->_data);
    free(t);
}
static inline hash_table_scfg_str_scfg_entry_t
hash_table_scfg_str_scfg_entry_t_make(
    hash_table_scfg_str_scfg_entry_t_hash_t hash, void * equ) {
    hash_table_scfg_str_scfg_entry_t t = malloc(sizeof(*t));
    uint64_t data_size =
        ht_prime_sizes[(3)] * sizeof(hash_table_slot_scfg_str_scfg_entry_t);
    hash_table_slot_scfg_str_scfg_entry_t * the_data = malloc(data_size);
    memset(the_data, 0, data_size);
    struct _hash_table_scfg_str_scfg_entry_t init = {
        ._size_idx = (3),
        ._data = the_data,
        .len = 0,
        ._free = hash_table_scfg_str_scfg_entry_t_free,
        ._get_key = hash_table_scfg_str_scfg_entry_t_get_key,
        ._get_val = hash_table_scfg_str_scfg_entry_t_get_val,
        ._insert = hash_table_scfg_str_scfg_entry_t_insert,
        ._delete = hash_table_scfg_str_scfg_entry_t_delete,
        ._equ = (hash_table_scfg_str_scfg_entry_t_equ_t)equ,
        ._hash = (hash_table_scfg_str_scfg_entry_t_hash_t)hash};
    memcpy(t, &init, sizeof(*t));
    hash_table_scfg_str_scfg_entry_t_update_load_thresh(t);
    return t;
};

#define STR(x) _STR(x)
#define _STR(x) #x

#define CAT2(x, y) _CAT2(x, y)
#define _CAT2(x, y) x##y

#define CAT3(x, y, z) _CAT3(x, y, z)
#define _CAT3(x, y, z) x##y##z

#define CAT4(a, b, c, d) _CAT4(a, b, c, d)
#define _CAT4(a, b, c, d) a##b##c##d

#define hash_table(K_T, V_T) CAT4(hash_table_, K_T, _, V_T)

#define hash_table_make(K_T, V_T, HASH) (CAT2(hash_table(K_T, V_T), _make)((HASH), NULL))
#define hash_table_make_e(K_T, V_T, HASH, EQU) (CAT2(hash_table(K_T, V_T), _make)((HASH), (EQU)))
#define hash_table_len(t) (t->len)
#define hash_table_free(t) (t->_free((t)))
#define hash_table_get_key(t, k) (t->_get_key((t), (k)))
#define hash_table_get_val(t, k) (t->_get_val((t), (k)))
#define hash_table_insert(t, k, v) (t->_insert((t), (k), (v)))
#define hash_table_delete(t, k) (t->_delete((t), (k)))
#define hash_table_traverse(t, key, val_ptr)                     \
    for (/* vars */                                              \
         uint64_t __i    = 0,                                    \
                  __size = ht_prime_sizes[t->_size_idx];         \
         /* conditions */                                        \
         __i < __size;                                           \
         /* increment */                                         \
         __i += 1)                                               \
        for (/* vars */                                          \
             __typeof__(*t->_data) *__slot_ptr = t->_data + __i, \
                                    __slot     = *__slot_ptr;    \
                                                                 \
             /* conditions */                                    \
             __slot != NULL                 &&                   \
             (key     = __slot->_key   , 1) &&                   \
             (val_ptr = &(__slot->_val), 1);                     \
                                                                 \
             /* increment */                                     \
             __slot_ptr = &(__slot->_next),                      \
             __slot = *__slot_ptr)                               \
            /* LOOP BODY HERE */
/*********************************************************************
    END HASH TABLE
**********************************************************************/

static uint64_t scfg_str_hash(scfg_str s) {
    unsigned long hash = 5381;
    int c;

    while ((c = *s++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static int scfg_str_equ(scfg_str a, scfg_str b) { return strcmp(a, b) == 0; }



typedef hash_table(scfg_str, scfg_entry_t) hash_table_t;


struct scfg {
    hash_table_t  table;
    const char   *err_msg;
    int           err_kind;
    const char   *valid_err_msg;
};

#define SCFG_SET_ERR(cfg, kind, fmt, ...)                 \
do {                                                      \
    char __scfg_err_buff[512];                            \
                                                          \
    snprintf(__scfg_err_buff, 512, (fmt), ##__VA_ARGS__); \
    if ((cfg)->err_msg) {                                 \
        free((void*)(cfg)->err_msg);                      \
    }                                                     \
    (cfg)->err_msg  = strdup(__scfg_err_buff);            \
    (cfg)->err_kind = (kind);                             \
} while (0)

struct scfg* scfg_make(void) {
    struct scfg *cfg;

    cfg = malloc(sizeof(*cfg));
    memset(cfg, 0, sizeof(*cfg));

    cfg->table = hash_table_make_e(scfg_str, scfg_entry_t, scfg_str_hash, scfg_str_equ);

    return cfg;
}

void scfg_free(struct scfg *cfg) {
    const char   *key;
    scfg_entry_t *val;

    hash_table_traverse(cfg->table, key, val) {
        scfg_entry_free(val);
        free((void*)key);
    }

    hash_table_free(cfg->table);
    free(cfg);
}

static void scfg_add_entry(struct scfg *cfg, const char *key, int kind, void *addr) {
    scfg_entry_t  new_entry,
                 *entry;

    entry = hash_table_get_val(cfg->table, key);

    if (!entry) {
        memset(&new_entry, 0, sizeof(new_entry));
        new_entry.kind  = kind;
        new_entry._addr = addr;
        hash_table_insert(cfg->table, strdup(key), new_entry);
    } else {
        scfg_entry_free(entry);
        memset(entry, 0, sizeof(*entry));
        entry->kind  = kind;
        entry->_addr = addr;
    }
}

void scfg_add_bool(struct scfg *cfg, const char *key, int *addr) {
    scfg_add_entry(cfg, key, SCFG_KIND_BOOL, addr);
}

void scfg_add_int(struct scfg *cfg, const char *key, int *addr) {
    scfg_add_entry(cfg, key, SCFG_KIND_INT, addr);
}

void scfg_add_float(struct scfg *cfg, const char *key, float *addr) {
    scfg_add_entry(cfg, key, SCFG_KIND_FLOAT, addr);
}

void scfg_add_string(struct scfg *cfg, const char *key, const char **addr) {
    scfg_add_entry(cfg, key, SCFG_KIND_STRING, addr);
}


#define GET_ENTRY(_cfg, _entry_ptr, _key, _kind)              \
    ((_entry_ptr) = hash_table_get_val((_cfg)->table, (_key)),\
    ((_entry_ptr) ? (_entry_ptr)->kind == (_kind) : 0))

void scfg_require(struct scfg *cfg, const char *key) {
    scfg_entry_t *entry;

    entry = hash_table_get_val(cfg->table, key);

    if (entry) {
        entry->flags |= SCFG_ENTRY_REQUIRED;
    }
}


void scfg_default_bool(struct scfg *cfg, const char *key, int def) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_BOOL)) { return; }

    entry->bdef   = !!def;
    entry->flags |= SCFG_ENTRY_HAS_DEFAULT;
}

void scfg_default_int(struct scfg *cfg, const char *key, int def) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_INT)) { return; }

    entry->idef   = def;
    entry->flags |= SCFG_ENTRY_HAS_DEFAULT;
}

void scfg_default_float(struct scfg *cfg, const char *key, float def) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_FLOAT)) { return; }

    entry->fdef   = def;
    entry->flags |= SCFG_ENTRY_HAS_DEFAULT;
}

void scfg_default_string(struct scfg *cfg, const char *key, const char *def) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_STRING)) { return; }

    entry->sdef   = strdup(def);
    entry->flags |= SCFG_ENTRY_HAS_DEFAULT;
}

void scfg_validate_bool(struct scfg *cfg, const char *key, scfg_validate_bool_fn_t validate) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_BOOL)) { return; }

    entry->validate = validate;
}

void scfg_validate_int(struct scfg *cfg, const char *key, scfg_validate_int_fn_t validate) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_INT)) { return; }

    entry->validate = validate;
}

void scfg_validate_float(struct scfg *cfg, const char *key, scfg_validate_float_fn_t validate) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_FLOAT)) { return; }

    entry->validate = validate;
}

void scfg_validate_string(struct scfg *cfg, const char *key, scfg_validate_string_fn_t validate) {
    scfg_entry_t *entry;

    if (!GET_ENTRY(cfg, entry, key, SCFG_KIND_STRING)) { return; }

    entry->validate = validate;
}


void scfg_validate_set_err(struct scfg *cfg, const char *msg) {
    if (cfg->valid_err_msg) {
        free((void*)cfg->valid_err_msg);
    }
    cfg->valid_err_msg = strdup(msg);
}

static int sh_split(char *s, char **words) {
    int      n;
    char    *copy,
            *sub,
            *sub_p;
    char     c, prev;
    int      len,
             start,
             end,
             q,
             sub_len,
             i;

#define SH_PUSH(word) \
do {                  \
    words[n]  = word; \
    n        += 1;    \
} while (0)

    n     = 0;
    copy  = strdup(s);
    len   = strlen(copy);
    start = 0;
    end   = 0;
    prev  = 0;

    while (start < len && isspace(copy[start])) { start += 1; }

    while (start < len) {
        c   = copy[start];
        q   = 0;
        end = start;

        if (c == '#' && prev != '\\') {
            break;
        } else if (c == '"') {
            start += 1;
            prev   = copy[end];
            while (end + 1 < len
            &&    (copy[end + 1] != '"' || prev == '\\')) {
                end += 1;
                prev = copy[end];
            }
            q = 1;
        } else if (c == '\'') {
            start += 1;
            prev   = copy[end];
            while (end + 1 < len
            &&    (copy[end + 1] != '\'' || prev == '\\')) {
                end += 1;
                prev = copy[end];
            }
            q = 1;
        } else {
            while (end + 1 < len
            &&     !isspace(copy[end + 1])) {
                end += 1;
            }
        }

        sub_len = end - start + 1;
        if (q && sub_len == 0 && start == len) {
            sub    = malloc(2);
            sub[0] = copy[end];
            sub[1] = 0;
        } else {
            sub   = malloc(sub_len + 1);
            sub_p = sub;
            for (i = 0; i < sub_len; i += 1) {
                c = copy[start + i];
                if (c == '\\'
                &&  i < sub_len - 1
                &&  (copy[start + i + 1] == '"'
                || copy[start + i + 1] == '\''
                || copy[start + i + 1] == '#')) {
                    continue;
                }
                *sub_p = c;
                sub_p += 1;
            }
            *sub_p = 0;
        }

        SH_PUSH(sub);

        end  += q;
        start = end + 1;

        while (start < len && isspace(copy[start])) { start += 1; }
    }

    free(copy);

    return n;

#undef SH_PUSH
}

static void scfg_validate_entry(struct scfg *cfg, scfg_entry_t *entry, char *key, const char *path, int line_nr) {
    int valid;

    scfg_validate_set_err(cfg, "failed validation");

    if (entry->validate == NULL) {
        return;
    }

    valid = 0;

    switch (entry->kind) {
        case SCFG_KIND_BOOL:
            valid = ((scfg_validate_bool_fn_t)entry->validate)(cfg, *entry->baddr);
            break;
        case SCFG_KIND_INT:
            valid = ((scfg_validate_int_fn_t)entry->validate)(cfg, *entry->iaddr);
            break;
        case SCFG_KIND_FLOAT:
            valid = ((scfg_validate_float_fn_t)entry->validate)(cfg, *entry->faddr);
            break;
        case SCFG_KIND_STRING:
            valid = ((scfg_validate_string_fn_t)entry->validate)(cfg, *entry->saddr);
            break;
    }

    if (!valid) {
        SCFG_SET_ERR(cfg, SCFG_ERR_VALIDATE, "%s line %d: option '%s': %s", path, line_nr, key, cfg->valid_err_msg);
    }
}

static void scfg_parse_bool(struct scfg *cfg, scfg_entry_t *entry, char *key, const char *path, int line_nr, char *val) {
    if (strcmp(val, "0")     == 0
    ||  strcmp(val, "false") == 0
    ||  strcmp(val, "False") == 0
    ||  strcmp(val, "FALSE") == 0
    ||  strcmp(val, "no")    == 0
    ||  strcmp(val, "No")    == 0
    ||  strcmp(val, "NO")    == 0) {
        *entry->baddr = 0;
    } else
    if (strcmp(val, "1")    == 0
    ||  strcmp(val, "true") == 0
    ||  strcmp(val, "True") == 0
    ||  strcmp(val, "TRUE") == 0
    ||  strcmp(val, "yes")  == 0
    ||  strcmp(val, "Yes")  == 0
    ||  strcmp(val, "YES")  == 0) {
        *entry->baddr = 1;
    } else {
        SCFG_SET_ERR(cfg, SCFG_ERR_BAD_KEY, "%s line %d: option '%s': could not parse bool from '%s'", path, line_nr, key, val);
    }
}

static void scfg_parse_int(struct scfg *cfg, scfg_entry_t *entry, char *key, const char *path, int line_nr, char *val) {
    if (!sscanf(val, "%d", entry->iaddr)) {
        SCFG_SET_ERR(cfg, SCFG_ERR_BAD_KEY, "%s line %d: option '%s': could not parse int from '%s'", path, line_nr, key, val);
    }
}

static void scfg_parse_float(struct scfg *cfg, scfg_entry_t *entry, char *key, const char *path, int line_nr, char *val) {
    if (!sscanf(val, "%f", entry->faddr)) {
        SCFG_SET_ERR(cfg, SCFG_ERR_BAD_KEY, "%s line %d: option '%s': could not parse float from '%s'", path, line_nr, key, val);
    }
}

static void scfg_parse_line(struct scfg *cfg, const char *path, char *line, int line_nr) {
    char         *words[64];
    int           n, i;
    char         *key;
    scfg_entry_t *entry;
    char         *val;

    n = sh_split(line, words);

    if (n == 0) { goto out; }
    if (n >= 3) {
        SCFG_SET_ERR(cfg, SCFG_ERR_BAD_SYNTAX,
                     "%s line %d: unexpected extra token%s after '%s'",
                     path, line_nr,
                     n == 3 ? "" : "s",
                     words[1]);
        goto out;
    }

    key   = words[0];
    entry = hash_table_get_val(cfg->table, key);

    if (!entry) {
        SCFG_SET_ERR(cfg, SCFG_ERR_BAD_KEY, "%s line %d: '%s' is not a configurable option", path, line_nr, key);
        goto out;
    }

    if (n == 1) {
        if (entry->kind != SCFG_KIND_BOOL) {
            SCFG_SET_ERR(cfg, SCFG_ERR_BAD_VAL, "%s line %d: missing value for option '%s'", path, line_nr, key);
            goto out;
        }

        *entry->baddr = 1;
    } else {
        val = words[1];

        switch (entry->kind) {
            case SCFG_KIND_BOOL:
                scfg_parse_bool(cfg, entry, key, path, line_nr, val);
                break;
            case SCFG_KIND_INT:
                scfg_parse_int(cfg, entry, key, path, line_nr, val);
                break;
            case SCFG_KIND_FLOAT:
                scfg_parse_float(cfg, entry, key, path, line_nr, val);
                break;
            case SCFG_KIND_STRING:
                *entry->saddr = strdup(val);
                break;
        }
    }

    if (cfg->err_kind == SCFG_ERR_NONE) {
        entry->flags |= SCFG_ENTRY_SET;
        scfg_validate_entry(cfg, entry, key, path, line_nr);
    }

out:
    for (i = 0; i < n; i += 1) {
        free(words[i]);
    }
}

static void scfg_apply_default(struct scfg *cfg, scfg_entry_t *entry) {
    switch (entry->kind) {
        case SCFG_KIND_BOOL:   *entry->baddr = entry->bdef; break;
        case SCFG_KIND_INT:    *entry->iaddr = entry->idef; break;
        case SCFG_KIND_FLOAT:  *entry->faddr = entry->fdef; break;
        case SCFG_KIND_STRING:
            *entry->saddr = entry->sdef;
            entry->sdef   = NULL;
            break;
    }
}

int scfg_parse(struct scfg *cfg, const char *path) {
    FILE         *f;
    int           line_nr;
    char          line[512];
    const char   *key;
    scfg_entry_t *entry;

    f = fopen(path, "r");

    if (!f) {
        SCFG_SET_ERR(cfg, SCFG_ERR_BAD_FILE, "unable to fopen path '%s'", path);
        return cfg->err_kind;
    }

    line_nr = 0;
    while (fgets(line, sizeof(line), f)) {
        line_nr += 1;

        scfg_parse_line(cfg, path, line, line_nr);
        if (cfg->err_kind != SCFG_ERR_NONE) { break; }
    }

    fclose(f);

    if (cfg->err_kind == SCFG_ERR_NONE) {
        hash_table_traverse(cfg->table, key, entry) {
            if (!(entry->flags & SCFG_ENTRY_SET)) {
                if (entry->flags & SCFG_ENTRY_REQUIRED) {
                    SCFG_SET_ERR(cfg, SCFG_ERR_BAD_FILE, "%s: required option '%s' not set", path, key);
                    return cfg->err_kind;
                } else if (entry->flags & SCFG_ENTRY_HAS_DEFAULT) {
                    scfg_apply_default(cfg, entry);
                }
            }
        }
    }

    return cfg->err_kind;
}

const char *scfg_err_msg(struct scfg *cfg) {
    if (cfg->err_msg) {
        return cfg->err_msg;
    }
    return "";
}


#undef SCFG_KIND_BOOL
#undef SCFG_KIND_INT
#undef SCFG_KIND_FLOAT
#undef SCFG_KIND_STRING
#undef SCFG_ENTRY_REQUIRED
#undef SCFG_ENTRY_HAS_DEFAULT
#undef SCFG_ENTRY_SET
#undef STR
#undef _STR
#undef CAT2
#undef _CAT2
#undef CAT3
#undef _CAT3
#undef CAT4
#undef _CAT4
#undef hash_table
#undef hash_table_make
#undef hash_table_make_e
#undef hash_table_len
#undef hash_table_free
#undef hash_table_get_key
#undef hash_table_get_val
#undef hash_table_insert
#undef hash_table_delete
#undef hash_table_traverse
#undef GET_ENTRY
#undef SCFG_SET_ERR

#endif
