#ifndef LFWFSPSCQ_H
#define LFWFSPSCQ_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

enum lfwfspscq_error_t {
  LFWFSPSCQ_ERROR_ALLOC_FAILED = 0,
  LFWFSPSCQ_ERROR_INVALID_PARAM_ELEMENT_SIZE = -1,
  LFWFSPSCQ_ERROR_INVALID_PARAM_ELEMENT_COUNT = -2,
};

struct lfwfspscq_t {
  int element_size;
  int slot_count;
  atomic_int head;
  atomic_int tail;
  uint8_t buffer[];
};

struct lfwfspscq_t *lfwfspscq_create(const int element_size,
                                     const int element_count);
void lfwfspscq_destroy(struct lfwfspscq_t *const self);

// Consumer Only
bool lfwfspscq_empty(const struct lfwfspscq_t *const self);
void *lfwfspscq_front(const struct lfwfspscq_t *const self);
bool lfwfspscq_try_pop(struct lfwfspscq_t *const self);

// Producer Only
bool lfwfspscq_full(const struct lfwfspscq_t *const self);
bool lfwfspscq_try_push(struct lfwfspscq_t *const self,
                        const void *const element);

#endif // LFWFSPSCQ_H
