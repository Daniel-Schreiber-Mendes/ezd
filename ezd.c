#include "ezd.h"

static FILE *file_ptr;
static Allocation_Array allocations;

void ezd_init(void)
{
   file_ptr = fopen("ezdlog.txt","w");
   assert(file_ptr);

   allocations.size = 0;
   allocations.capacity = 10;
   allocations.data = malloc(sizeof *allocations.data * allocations.capacity);
}


void ezd_terminate(void)
{
	Allocation_Data data;
	for (unsigned int i=0; i < allocations.size; ++i)
	{
		data = allocations.data[i];
		fprintf(file_ptr, "Leaked Memory: %p\nAllocated %lu bytes\nFile: %s\nFunc: %s\nLine:%i\n\n", data.ptr, data.size, data.file, data.func, data.line);
	}
	if (allocations.size != 0)
	{
		printf("EZD: Memory Leak found\n");
	}

	fclose (file_ptr);
	free(allocations.data);
}


void* ezd_malloc(size_t size, char const *file, char const *func, int line)
{
	void *ptr = malloc(size);
	ezd_allocation_add(file, func, line, size, ptr);
	return ptr;
}


void* ezd_calloc(size_t nmemb, size_t size, char const *file, char const *func, int line)
{	
	void *ptr = calloc(nmemb, size);
	ezd_allocation_add(file, func, line, size, ptr);
	return ptr;
}


void* ezd_realloc(void const *ptr, size_t size, char const *file, char const *func, int line)
{
	void *new_ptr = realloc(ptr, size);
	ezd_allocation_update(ptr, new_ptr);
	return new_ptr;
}


void  ezd_free(void const *ptr, char const *file, char const *func, int line)
{
	ezd_allocation_remove(ptr);
	free(ptr);
}


void ezd_allocation_add(char const *file, char const *func, int line, size_t size, void const *ptr)
{
	if (allocations.size == allocations.capacity)
	{
		allocations.data = realloc(allocations.data, sizeof *allocations.data * (allocations.capacity *= 2));
	}
	allocations.data[allocations.size++] = (Allocation_Data){file, func, line, size, ptr};
}


void ezd_allocation_remove(void const *ptr)
{
	Allocation_Data data;
	for (unsigned int i=0; i < allocations.size; ++i)
	{
		data = allocations.data[i];
		if (data.ptr == ptr)
		{
			allocations.data[i] = allocations.data[--allocations.size];
		}
	}
}


void ezd_allocation_update(void const *old_ptr, void const *new_ptr)
{
	for (unsigned int i=0; i < allocations.size; ++i)
	{
		Allocation_Data *data = &allocations.data[i];
		if (data->ptr == old_ptr)
		{
			data->ptr = new_ptr;
		}
	}
	//it is possible that no element is found if realloc is used as malloc, meaning realloc argument is NULL
}