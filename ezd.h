#ifndef EZD_H
#define EZD_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


typedef struct
{
	char const *file;
	char const *func;
	unsigned int line;
	size_t size;
	void *ptr;
}
Allocation_Data;


typedef struct
{
	Allocation_Data *data;
	unsigned int size;
	unsigned int capacity;
}
Allocation_Array;


void  ezd_init(void);
void  ezd_terminate(void);
void  ezd_memory_print(void);
void* ezd_malloc(size_t size, char const *file, char const *func, int line);
void* ezd_calloc(size_t nmemb, size_t size, char const *file, char const *func, int line);
void  ezd_free(void const *ptr, char const *file, char const *func, int line);
void* ezd_realloc(void const *ptr, size_t size, char const *file, char const *func, int line);
void  ezd_allocation_add(char const *file, char const *func, int line, size_t size, void const *ptr);
void  ezd_allocation_remove(void const *ptr);
void  ezd_allocation_update(void const *old_ptr, void const *new_ptr);


#ifdef EZD_MEMORY_DEBUG

#define malloc(size) ezd_malloc(size, __FILE__, __func__, __LINE__)
#define calloc(nmemb, size) ezd_calloc(nmemb, size, __FILE__, __func__, __LINE__)
#define free(ptr) ezd_free(ptr, __FILE__, __func__, __LINE__)
#define realloc(ptr, size) ezd_realloc(ptr, size, __FILE__, __func__, __LINE__)

#endif

#endif