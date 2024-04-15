#ifndef SHARED_MEMORY_FUN_H
#define SHARED_MEMORY_FUN_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

void create_shared_memory(const char* name, int length);

int open_shared_memory(const char* name);

void close_shared_memory(int fd);

void delete_shared_memory(const char* name);

void* map_shared_memory(int length, int fd);

void unmap_shared_memory(void* addr, int length);

#endif // SHARED_MEMORY_FUN_H