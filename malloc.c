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
      int *p1 = (int *)my_malloc(sizeof(int));
      *p1 = 42;
       printf("p1 value:  %d\n", *p1);
       printf("p1 address: %p\n", p1);

       my_free(p1);
       printf("freed p1!\n");
       
       int *p2 = (int *)my_malloc(sizeof(int));
       *p2 = 99;
       printf("p2 value: %d\n", *p2);
       printf("p2 address: %p\n" , p2);

       return 0;
    }
