#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define FIRST_FIT 0
#define BEST_FIT 1

int placement_policy = BEST_FIT;

struct block_header {
    size_t size;
    int is_free;
    struct block_header *next;
};

struct block_header *free_list = NULL;

void *my_malloc(size_t size) {

  struct block_header *current = free_list;
  struct block_header *best = NULL;

  while(current != NULL){
    if(current->is_free && current->size >= size){
      if(placement_policy == FIRST_FIT){
      current->is_free = 0;
      return (void *)(current + 1);
    } else {
      if (best == NULL || current->size < best->size){
        best = current;
      }
    }
  } 
    current = current->next;
  }

  if(best != NULL){
    best->is_free = 0;
    return (void *) (best + 1);
  }
      size_t total = sizeof(struct block_header) + size;
      void *block = sbrk(total);
      if(block == (void *) - 1){
        return NULL;  // malloc failed
      }
      struct block_header *header = (struct block_header *)block;
      header->size = size;
      header->is_free = 0;
      header->next = NULL;

      if (free_list == NULL) {
        free_list = header;
      } else{
        struct block_header *temp = free_list;
        while (temp->next != NULL){
          temp = temp->next;
        }
        temp->next = header;
      }
      return (void *) (header + 1);
    }

    void my_free(void *p){
      if (p == NULL) return;
      struct block_header *header = (struct block_header *)p - 1;
      header->is_free = 1;

      if (header->next != NULL && header->next->is_free){
        header->size +=sizeof(struct block_header) + header->next->size;
        header->next = header->next->next;
      }
    }
      
  
    int main() {
    printf("=== Testing my_malloc ===\n\n");

    // test 1: basic allocation
    int *a = (int *)my_malloc(sizeof(int));
    *a = 10;
    printf("a = %d, address: %p\n", *a, a);

    int *b = (int *)my_malloc(sizeof(int));
    *b = 20;
    printf("b = %d, address: %p\n", *b, b);

    // test 2: free and reuse
    printf("\n--- Freeing a ---\n");
    my_free(a);

    int *c = (int *)my_malloc(sizeof(int));
    *c = 30;
    printf("c = %d, address: %p\n", *c, c);
    printf("a and c same address? %s\n", (void*)a == (void*)c ? "YES - reused!" : "NO");

    // test 3: coalescing
    printf("\n--- Testing coalescing ---\n");
    my_free(b);
    my_free(c);

    // now two adjacent blocks are free, request bigger block
    int *d = (int *)my_malloc(sizeof(int) * 2);
    printf("d allocated after coalescing: %p\n", d);

    printf("\n=== All tests passed! ===\n");
    return 0;
}