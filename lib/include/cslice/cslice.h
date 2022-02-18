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
** This file defines the signatures for functions relating to slices.
*/

#ifndef CSLICE_SLICE_H
#define CSLICE_SLICE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct Slice Slice;
typedef struct KeyIndex KeyIndex;

Slice *slice(uint32_t);
Slice *sslice(Slice *, uint32_t, uint32_t);
Slice *make_slice(void *, uint32_t, size_t);

KeyIndex *find_index(const Slice *, const void *, int(*)(const void *, const void *));

uint32_t len(Slice *s);
uint32_t capacity(Slice *);
uint32_t kx_index(KeyIndex *);

void unshift(Slice *, void *);
void shift(Slice *);
void push(Slice *, void *);
void put_index(Slice *, void *, uint32_t);
void set_index(Slice *, void *, uint32_t);
void fill(Slice *, void **, uint32_t);
void join(Slice *, Slice *);
void delete_slice(Slice *);
void csort(Slice *, int(*)(const void *, const void *));
void print(Slice *, void(*)(const void *));

void *pop(Slice *);
void *remove_index(Slice *, uint32_t);
void *kx_key(KeyIndex *);

const void *get_index(Slice *, uint32_t);

void **keys(Slice *);

#endif //CSLICE_SLICE_H
