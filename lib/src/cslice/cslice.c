/*
** February 15, 2022
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file implements functions for working with slices.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

typedef struct Slice {
    uint32_t length;
    uint32_t capacity;
    void **keys;
} Slice;

typedef struct KeyIndex {
    void *key;
    uint32_t index;
} KeyIndex;

KeyIndex *key_index(void *key, uint32_t index) {
    KeyIndex *kx = malloc(sizeof(KeyIndex));
    kx->key = key;
    kx->index = index;
    return kx;
}

uint32_t kx_index(KeyIndex *kx) {
    return kx->index;
}

void *kx_key(KeyIndex *kx) {
    return kx->key;
}

uint32_t len(Slice *s) {
    return s->length;
}

uint32_t capacity(Slice *s) {
    return s->capacity;
}

void **keys(Slice *s) {
    return s->keys;
}

static void resize(Slice *s, uint32_t capacity) {
    if (capacity == s->length) {
        printf("resizing operation had no effect. passed in capacity equals current slice len");
        return;
    }
    if (capacity < s->length) {
        printf("cannot reallocate slice keys to buffer smaller than current slice len!");
        return;
    }
    s->keys = realloc(s->keys, sizeof(void *) * capacity);
    s->capacity = capacity;
}

static void autoresize(Slice *s) {
    if (s->length < floor((double) s->capacity / 4)) resize(s, s->capacity / 2);
    if (s->capacity < 10 && s->length == s->capacity) {
        resize(s, s->capacity * 2);
    } else {
        resize(s, ceil((double) s->capacity * 1.5));
    }
}

Slice *slice(uint32_t capacity) {
    if (capacity == 0) return NULL; // hippity hoppity, this null pointer is now your property
    Slice *s = malloc(sizeof(Slice));
    s->length = 0;
    s->capacity = capacity;
    s->keys = malloc(sizeof(void *) * s->capacity);
    return s;
}

void delete_slice(Slice *s) {
    s->keys = NULL;
    free(s->keys);
    s->length = 0;
    s->capacity = 0;
    s = NULL;
    free(s);
}

Slice *make_slice(void *keys, uint32_t capacity, size_t size) {
    Slice *s = slice(capacity);
    for (int i = 0; i < capacity; i++) s->keys[i] = keys + size * i;
    s->length = capacity;
    keys = NULL;
    free(keys);
    return s;
}

Slice *sslice(Slice *s, uint32_t start, uint32_t end) {
    Slice *ss = slice(end - start);
    memcpy(ss->keys, &(s->keys[start]), sizeof(void *) * (end - start));
    ss->length = end - start;
    return ss;
}

void unshift(Slice *s, void *key) {
    autoresize(s);
    memcpy(&(s->keys[1]), s->keys, sizeof(void *) * s->length);
    s->length++;
    s->keys[0] = key;
}

void push(Slice *s, void *key) {
    autoresize(s);
    s->keys[s->length++] = key;
}

void *shift(Slice *s) {
    void *key = s->keys[0];
    s->keys[0] = NULL;
    free(s->keys[0]);
    memcpy(s->keys, &(s->keys[1]), sizeof(void *) * (s->length - 1));
    s->length--;
    autoresize(s);
    return key;
}

void *pop(Slice *s) {
    void *key = s->keys[s->length - 1];
    s->keys[s->length - 1] = NULL;
    free(s->keys[s->length - 1]);
    s->length--;
    autoresize(s);
    return key;
}

void put_index(Slice *s, void *key, uint32_t index) {
    if (index > s->length) {
        printf("cannot write to index beyond slice len!\n");
        return;
    }
    if (index == s->length) return push(s, key);
    if (index == 0) return unshift(s, key);
    autoresize(s);
    memcpy(&(s->keys[index + 1]), &(s->keys[index]), sizeof(void *) * (s->length - index));
    s->keys[index] = key;
    s->length++;
}

void *remove_index(Slice *s, uint32_t index) {
    if (index >= s->length) {
        printf("cannot delete from index beyond slice len!\n");
        return NULL;
    }
    void *key = s->keys[index];
    s->keys[index] = NULL;
    free(s->keys[index]);
    memcpy(&(s->keys[index]), &(s->keys[index + 1]), sizeof(void *) * (s->length - index - 1));
    s->length--;
    return key;
}

void set_index(Slice *s, void *key, uint32_t index) {
    if (index >= s->length) {
        printf("index %d out of bounds for slice!", index);
        return;
    }
    s->keys[index] = key;
}

const void *get_index(Slice *s, uint32_t index) {
    if (index >= s->length) {
        printf("index %d out of bounds for slice!", index);
        return NULL;
    }
    return s->keys[index];
}

void fill(Slice *s, void **keys, uint32_t num_keys) {
    memcpy(s->keys, keys, sizeof(void *) * num_keys);
    s->length = num_keys;
}

void join(Slice *s1, Slice *s2) {
    if (s1->length + s2->length >= s1->capacity) resize(s1, s1->length + s2->length);
    memcpy(&(s1->keys[s1->length]), s2->keys, sizeof(void *) * s2->length);
    s1->length += s2->length;
    delete_slice(s2);
}

KeyIndex *find_index(const Slice *s, const void *key, int(*cmpfunc)(const void *, const void *)) {
    int32_t start = 0, end = (int32_t) s->length - 1;
    while (start <= end) {
        int32_t mid = (start + end) / 2;
        if (cmpfunc(s->keys[mid], key) == 0) return key_index(s->keys[mid], mid);
        if (cmpfunc(s->keys[mid], key) < 0) start = mid + 1;
        else end = mid - 1;
    }
    return key_index(NULL, end + 1);
}

void print(Slice *s, void(*printfunc)(const void *)) {
    for (int i = 0; i < s->length; i++) printfunc(s->keys[i]);
    printf("\n");
}


static void insertion_sort(Slice *s, int(*cmpfunc)(const void *, const void *)) {
    int j;
    for (int i = 0; i < len(s); i++) {
        void *key = keys(s)[i];
        j = i - 1;
        while (j >= 0 && cmpfunc(key, keys(s)[j]) <= 0) {
            keys(s)[j + 1] = keys(s)[j];
            j--;
        }
        keys(s)[j + 1] = key;
    }
}

static void merge(Slice *s, Slice *l, Slice *r, int(*cmpfunc)(const void *, const void *)) {
    int i = 0, j = 0, k = 0;
    while (i < len(l) && j < len(r)) {
        if (cmpfunc(keys(r)[j], keys(l)[i]) >= 0) {
            set_index(s, keys(l)[i], k);
            i++;
        } else {
            set_index(s, keys(r)[j], k);
            j++;
        }
        k++;
    }
    while (i < len(l)) {
        set_index(s, keys(l)[i], k);
        i++, k++;
    }
    while (j < len(r)) {
        set_index(s, keys(r)[j], k);
        j++, k++;
    }
}

void csort(Slice *s, int(*cmpfunc)(const void *, const void *)) {
    if (len(s) < 44) return insertion_sort(s, cmpfunc);
    unsigned int mid = len(s) / 2;
    Slice *l = slice(mid);
    Slice *r = slice(len(s) - mid);
    fill(l, keys(s), mid);
    fill(r, &(keys(s)[mid]), len(s) - mid);
    csort(l, cmpfunc);
    csort(r, cmpfunc);
    merge(s, l, r, cmpfunc);
}
