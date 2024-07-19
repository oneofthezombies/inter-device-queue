#include <assert.h>

#include "lfwfspscq.h"

int main() {
  struct lfwfspscq_t *queue = lfwfspscq_create(sizeof(int), 1);
  assert(queue);
  assert(lfwfspscq_empty(queue));

  int element = 1;
  assert(lfwfspscq_try_push(queue, &element));
  assert(lfwfspscq_full(queue));

  element = 2;
  assert(lfwfspscq_try_push(queue, &element) == false);

  assert(lfwfspscq_empty(queue) == false);
  assert(*(int *)lfwfspscq_front(queue) == 1);
  assert(lfwfspscq_try_pop(queue));
  assert(lfwfspscq_empty(queue));

  assert(lfwfspscq_try_pop(queue) == false);

  return 0;
}
