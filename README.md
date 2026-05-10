# Dynamic Memory Allocator in C

A custom heap memory allocator built from scratch in C, implementing `malloc()` and `free()` without any standard library memory functions.

## Why I Built This
To deeply understand how memory allocation works under the hood — from sbrk() system calls to free list management and fragmentation handling.

## Features
- Custom `my_malloc()` and `my_free()` implementation using `sbrk()`
- Explicit free list for reusing freed memory blocks
- Block coalescing — merges adjacent free blocks to reduce fragmentation
- First-fit and best-fit placement policies

## How to Run
```bash
gcc malloc.c -o malloc
./malloc
```

## How It Works
Each allocated block has a hidden header storing its size and status. A global linked list tracks all free blocks. On each allocation, the free list is searched first before requesting memory from the OS.
