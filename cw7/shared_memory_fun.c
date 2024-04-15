#include "shared_memory_fun.h"

enum ERRORS 
{
    SHM_OPEN_ERROR = 1,
    FTRUNCATE_ERROR,
    CLOSE_ERROR,
    SHM_UNLINK_ERROR,
    MMAP_ERROR,
    MUNMAP_ERROR
};

void create_shared_memory(const char* name, int length)
{
    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0644);
    if(fd == -1)
    {
        perror("shm_open error");
        exit(SHM_OPEN_ERROR);
    }

    if(ftruncate(fd, length) == -1)
    {
        perror("ftruncate error");
        exit(FTRUNCATE_ERROR);        
    }

    close_shared_memory(fd);
}

int open_shared_memory(const char* name)
{
    int fd = shm_open(name, O_RDWR, 0644);
    if(fd == -1)
    {
        perror("shm_open error");
        exit(SHM_OPEN_ERROR);        
    }

    return fd; 
}

void close_shared_memory(int fd)
{
    if(close(fd) == -1)
    {
        perror("close error");
        exit(CLOSE_ERROR);
    }
}

void delete_shared_memory(const char* name)
{
    if(shm_unlink(name) == -1)
    {
        perror("shm_unlink error");
        exit(SHM_UNLINK_ERROR);
    }
}

void* map_shared_memory(int length, int fd)
{
    void *ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap error");
        exit(MMAP_ERROR);
    }

    return ptr;
}

void unmap_shared_memory(void* addr, int length)
{
    if(munmap(addr, length) == -1)
    {
        perror("munmap error");
        exit(MUNMAP_ERROR);
    }
}