#include <string.h>

#include "containerslib/exceptions.h"
#include "containerslib/heap.h"

struct Heap {
    byte *values;
    byte *keys;

    size_t capacity;
    size_t len;

    size_t vsmemb;
    size_t ksmemb;

    free_fn vfreer;

    cmp_fn kcmp;
};

void __heap_grow(Heap *heap) {
    if (heap->len + 1 == heap->capacity) {
        heap->capacity <<= 1;
        heap->values = realloc(heap->values, heap->vsmemb * heap->capacity);
        heap->keys =
            realloc(heap->keys, heap->ksmemb * heap->capacity);
    }
}

void __heap_swap(Heap *map, size_t i, size_t j) {
    if (i > map->len || j > map->len)
        exception_throw_index("heap_swap - Index out_value of bounds");

    memcpy(map->keys, map->keys + i * map->ksmemb, map->ksmemb);
    memcpy(map->keys + i * map->ksmemb, map->keys + j * map->ksmemb, map->ksmemb);
    memcpy(map->keys + j * map->ksmemb, map->keys, map->ksmemb);

    memcpy(map->values, map->values + i * map->vsmemb, map->vsmemb);
    memcpy(map->values + i * map->vsmemb, map->values + j * map->vsmemb, map->vsmemb);
    memcpy(map->values + j * map->vsmemb, map->values, map->vsmemb);
}

/**
 * @brief Returns the diff priority of the child with the highest priority; if
 * the result is negative, the left child has the highest priority; if the
 * result is positive, the right child has the highest priority; if the result
 * is zero, the children have less priority than the parent.
 *
 * @param heap The heap.
 * @param i The index of the parent.
 * @return double
 */
int __heap_heapify_high(Heap *heap, size_t i) {
    size_t lindx = 2 * i;
    size_t rindx = 2 * i + 1;

    // I love Linux file permissions.
    int bits = 0b01 * (lindx > heap->len) + 0b10 * (rindx > heap->len);
    switch (bits) {
    case 0b01:;
        int rdiff =
            heap->kcmp(heap->keys + rindx * heap->ksmemb, heap->keys + i * heap->ksmemb);
        return rdiff > 0 ? rdiff : 0;

    case 0b10:;
        int ldiff =
            heap->kcmp(heap->keys + lindx * heap->ksmemb, heap->keys + i * heap->ksmemb);
        return ldiff > 0 ? ldiff * -1 : 0;

    case 0b11:
        return 0;
    }

    int ldiff = heap->kcmp(heap->keys + lindx * heap->ksmemb, heap->keys + i * heap->ksmemb);
    int rdiff = heap->kcmp(heap->keys + rindx * heap->ksmemb, heap->keys + i * heap->ksmemb);

    int bits2 = 0b01 * (ldiff <= 0) + 0b10 * (rdiff <= 0);
    switch (bits2) {
    case 0b01:
        return rdiff;

    case 0b10:
        return ldiff * -1;

    case 0b11:
        return 0;
    }

    return ldiff < rdiff ? rdiff : ldiff * -1;
}

void __heap_heapify_up(Heap *heap, size_t i) {
    if (heap->len == 1 || i == 1)
        return;

    size_t icurr = i >> 1;
    while (__heap_heapify_high(heap, icurr)) {
        size_t ichild =
            2 * icurr + (__heap_heapify_high(heap, icurr) < 0 ? 0 : 1);

        __heap_swap(heap, icurr, ichild);

        if (icurr == 1)
            break;
        icurr >>= 1;
    }
}

void __heap_heapify_down(Heap *heap) {
    size_t icurr = 1;
    while (__heap_heapify_high(heap, icurr)) {
        size_t ichild =
            2 * icurr + (__heap_heapify_high(heap, icurr) < 0 ? 0 : 1);

        __heap_swap(heap, icurr, ichild);

        icurr = ichild;
    }
}

void __heap_push(Heap *heap, byte *key, byte *value) {
    __heap_grow(heap);

    memcpy(heap->keys + (heap->len + 1) * heap->ksmemb, key, heap->ksmemb);
    memcpy(heap->values + (heap->len + 1) * heap->vsmemb, value, heap->vsmemb);
    heap->len++;

    __heap_heapify_up(heap, heap->len);
}

void __heap_peek(Heap *heap, byte *out_key, byte *out_value) {
    if (heap->len == 0)
        exception_throw_failure("heap_peek - Heap is empty");

    memcpy(out_key, heap->keys + heap->ksmemb, heap->ksmemb);
    memcpy(out_value, heap->values + heap->vsmemb, heap->vsmemb);
}

void __heap_pop(Heap *heap, byte *out_key, byte *out_value) {
    if (heap->len == 0)
        exception_throw_failure("heap_pop - Heap is empty");

    memcpy(out_key, heap->keys + heap->ksmemb, heap->ksmemb);
    memcpy(out_value, heap->values + heap->vsmemb, heap->vsmemb);

    if (heap->len == 1) {
        heap->len--;
        return;
    }

    // place the last element in the root
    __heap_swap(heap, 1, heap->len);
    heap->len--;

    __heap_heapify_down(heap);
}

Heap *heap_init(size_t initialPow2Capacity, size_t ksmemb, size_t vsmemb, free_fn vfreer, cmp_fn cmp) {
    Heap *heap = malloc(sizeof *heap);

    heap->ksmemb = ksmemb;
    heap->vsmemb = vsmemb;
    heap->vfreer = vfreer;
    heap->kcmp = cmp;

    heap->capacity = 1;
    heap->capacity <<= initialPow2Capacity;
    heap->len = 0;
    heap->values = malloc(heap->vsmemb * heap->capacity);
    heap->keys = malloc(heap->ksmemb * heap->capacity);

    return heap;
}

void heap_push(Heap *self, void *key, void *data) {
    __heap_push(self, key, data);
}

void heap_pop(Heap *self, void *out_key, void *out_value) {
    __heap_pop(self, out_key, out_value);
}

void heap_peek(Heap *self, void *out_key, void *out_value) {
    __heap_peek(self, out_key, out_value);
}

size_t heap_len(Heap *self) {
    return self->len;
}

void heap_free(Heap *self) {
    if (self->vfreer)
        for (size_t i = 1; i <= self->len; i++)
            self->vfreer(self->values + (i * self->vsmemb));

    free(self->values);
    free(self->keys);
    free(self);
}
