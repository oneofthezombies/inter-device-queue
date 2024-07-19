#include "lfwfspscq.h"

#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#ifndef LFWFSPSCQ_INCLUDE
#define LFWFSPSCQ_INCLUDE
#endif // LFWFSPSCQ_INCLUDE

LFWFSPSCQ_INCLUDE

#ifndef LFWFSPSCQ_ALLOC
#define LFWFSPSCQ_ALLOC(size) malloc(size)
#endif // LFWFSPSCQ_ALLOC

#ifndef LFWFSPSCQ_FREE
#define LFWFSPSCQ_FREE(ptr) free(ptr)
#endif // LFWFSPSCQ_FREE

#ifndef LFWFSPSCQ_MEMCPY
#define LFWFSPSCQ_MEMCPY(dest, src, count) memcpy(dest, src, count)
#endif // LFWFSPSCQ_MEMCPY

struct lfwfspscq_t *lfwfspscq_create(const int element_size,
                                     const int element_count) {
  if (element_size <= 0) {
    return (void *)LFWFSPSCQ_ERROR_INVALID_PARAM_ELEMENT_SIZE;
  }

  if (element_count <= 0) {
    return (void *)LFWFSPSCQ_ERROR_INVALID_PARAM_ELEMENT_COUNT;
  }

  // To check full, one empty slot is required.
  const int slot_count = element_count + 1;
  const size_t total_size =
      sizeof(struct lfwfspscq_t) + element_size * slot_count;
  struct lfwfspscq_t *queue = (void *)LFWFSPSCQ_ALLOC(total_size);
  if (queue == NULL) {
    return (void *)LFWFSPSCQ_ERROR_ALLOC_FAILED;
  }

  queue->element_size = element_size;
  queue->slot_count = slot_count;
  atomic_init(&queue->head, 0);
  atomic_init(&queue->tail, 0);

  return queue;
}

void lfwfspscq_destroy(struct lfwfspscq_t *const self) {
  if (self == NULL) {
    return;
  }

  LFWFSPSCQ_FREE(self);
}

bool lfwfspscq_empty(const struct lfwfspscq_t *const self) {
  return atomic_load(&self->head) == atomic_load(&self->tail);
}

void *lfwfspscq_front(const struct lfwfspscq_t *const self) {
  if (lfwfspscq_empty(self)) {
    return NULL;
  }

  const int tail = atomic_load(&self->tail);
  return (void *)&self->buffer[tail * self->element_size];
}

bool lfwfspscq_try_pop(struct lfwfspscq_t *const self) {
  if (lfwfspscq_empty(self)) {
    return false;
  }

  const int tail = atomic_load(&self->tail);
  atomic_store(&self->tail, (tail + 1) % self->slot_count);
  return true;
}

bool lfwfspscq_full(const struct lfwfspscq_t *const self) {
  const int next_head = (atomic_load(&self->head) + 1) % self->slot_count;
  return next_head == atomic_load(&self->tail);
}

bool lfwfspscq_try_push(struct lfwfspscq_t *const self,
                        const void *const element) {
  const int head = atomic_load(&self->head);
  const int next_head = (head + 1) % self->slot_count;
  if (next_head == atomic_load(&self->tail)) {
    return false;
  }

  LFWFSPSCQ_MEMCPY(&self->buffer[head * self->element_size], element,
                   self->element_size);
  atomic_thread_fence(memory_order_release);
  atomic_store(&self->head, next_head);
  return true;
}
