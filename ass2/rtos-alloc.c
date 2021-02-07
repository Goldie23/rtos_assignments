#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "rtos-alloc.h"
#include <unistd.h>
#include <assert.h>
#include <time.h>




typedef struct allocation{


    void *a_base;
    size_t a_mem_size;
    size_t a_len;

    struct allocation *a_next;
    struct allocation *a_prev;


} alloc_t;


struct largest_block{

	alloc_t *block;
	size_t len;


};


static struct largest_block large_block =  {.block = NULL, .len = 0};
static struct largest_block* lb = &large_block;
static alloc_t *head, *tail;




size_t
get_pagesize()
{

        return sysconf(_SC_PAGESIZE);

}

void
set_largest_block(){


	if(head){
		alloc_t *temp = head;

		while(temp){
		
			if(temp->a_len > lb->block->a_len){
				lb->block = temp;
				lb->len = temp->a_len;
			}
			temp = temp->a_next;
		
		}
	
	
	}
	else{
		lb->block = NULL;
		lb->len = 0;
	
	}

}


void
rtos_alloc_init()
{
	head = tail = NULL;
//	lb->block = NULL;
//	lb->len = 0;
}






alloc_t	*
alloc_find(void* p)
{
	for (alloc_t *ap = head; ap != NULL; ap = ap->a_next)
        {
                //assert(ap->a_base != NULL);
                if (ap->a_base == p)
                {
                        return ap;
                }
        }

        return NULL;
	
}






void
add_block(alloc_t* block)
{


	if(!head || ((unsigned long) head > (unsigned long) block)){
			
		if(head){
		
			head->a_prev = block;
			block->a_next = head;
		}		
		head = tail = block;
			
			
	}else{
	
		alloc_t *temp = head;
		
		while(temp->a_next  &&((unsigned long) temp->a_next < (unsigned long)block)){

               		temp = temp->a_next;

           	 }
		if(temp->a_next){
		
			block->a_next = temp->a_next;
			block->a_next->a_prev = block;
			temp->a_next = block;
			block->a_prev = temp;
		
		}
		else{
			block->a_next = NULL;
			block->a_prev = temp;
			temp->a_next = block;
			tail = block;
		
		}


	}
	if(lb->block){
	
		if(block->a_len >= lb->block->a_len){
			lb->block = block;
			lb->len = block->a_len;
	
	
		}
	}
	else{
		lb->block = block;
		lb->len = block->a_len;

	}
}



void 
remove_block(alloc_t* block){



	if(!block->a_prev){
	
		if(block->a_next){
		
			head = block->a_next;
			head->a_prev = NULL;	
		}else{

			head = tail = NULL;
			if(brk(head) != 0){
				printf("problem freeing memory");
			}
		}		
	
	}

	else if(!block->a_next){
	
		if(block->a_prev){
		
			tail = block->a_prev;
			block->a_prev->a_next = block->a_next;
			tail->a_len += (sizeof(alloc_t*) + block->a_mem_size + block->a_len);
		}
		else{
		
			head = tail = NULL;
			if(brk(head) != 0){
                                printf("problem freeing memory");
                        }

		}	
		
	}else{
	
		block->a_prev->a_next = block->a_next;
		block->a_next->a_prev = block->a_prev;
		block->a_prev->a_len += (sizeof(alloc_t*) + block->a_mem_size + block->a_len);
	
	
	}
	set_largest_block();


}



/*
void scan_and_merge(){

	if(head){
		size_t PAGE_SIZE = get_pagesize();
		alloc_t *temp = head;
		unsigned long temp_curr;
		unsigned long temp_next;
		unsigned long program_break = (unsigned long) sbrk(0);
		if(program_break == 0){
			return;
		}


		while(temp->a_next){
		
		        temp_curr = (unsigned long) temp;
		        temp_next = (unsigned long) temp->a_next;
			
		
		
		}




	}

}*/




alloc_t	*
split(alloc_t *block, size_t size)
{

	alloc_t *new_block =/* (alloc_t*) ((unsigned long)*/ block->a_base + block->a_mem_size;
	new_block->a_len = block->a_len - (size + sizeof(alloc_t));
	block->a_len = 0;
	return new_block;


}







void	*
rtos_malloc(size_t size){



	void* block_mem;
	alloc_t *new_block;
	size_t alloc_size = 0;
	size_t PAGE_SIZE = get_pagesize();
	
	if(size + sizeof(alloc_t*) <= lb->len){

		new_block = split(lb->block, size);
		new_block->a_mem_size = size;
		new_block->a_base = /*(void*) (unsigned long)*/new_block + sizeof(alloc_t*);
		add_block(new_block);
		set_largest_block();
		return new_block->a_base;
	}

	else if(size + sizeof(alloc_t*) > 3*PAGE_SIZE){
	
		alloc_size = size + sizeof(alloc_t*);		
				
	}

	else{
		alloc_size = 3*PAGE_SIZE;
	}

	new_block = sbrk(alloc_size);
	if(!new_block){
	
		printf("couldnt increment program break");

	}

	new_block->a_len = alloc_size - (sizeof(alloc_t*) + size);
	new_block->a_base =/*(void*) (unsigned long)*/ new_block + sizeof(alloc_t*);
	new_block->a_mem_size = size;
	add_block(new_block);
	set_largest_block();
	
	return new_block->a_base;

}

void*
rtos_realloc(void *ptr, size_t size)
{

	return ptr;

}



void
rtos_free(void *ptr)
{
	if(ptr){

		alloc_t *block = alloc_find(ptr);
		if(block){
			remove_block(block);
			ptr = NULL;
		}
	}
}


size_t rtos_alloc_size(void *ptr)
{

	return alloc_find(ptr)->a_mem_size;

}



bool rtos_is_valid(void *ptr)
{
	return alloc_find(ptr);
}


size_t rtos_total_allocated(){


	size_t tot = 0;
	alloc_t *temp = head;
	while(temp){
	
		tot+=temp->a_mem_size;
	
		temp = temp->a_next;	
	}


	return tot;
}




int main(char argc, char* argv[]){



	int rtos_time1 = 0;
	int rtos_time2 = 0;
	double rtos_alloc_time = 0;
	
	int malloc_time1 = 0;
	int malloc_time2 = 0;
	double  malloc_time = 0;
	struct timespec rtos_alloc_start, rtos_alloc_finish, malloc_start, malloc_finish;
	size_t total = rtos_total_allocated();
	size_t t = 0;
	size_t arr[9] = {1, 2, 17, 42, 1049, 50, 20000, 30000, 100000};
	for(int i = 0; i < 9; i++){
		size_t s = arr[i];
		rtos_time1 = clock_gettime(CLOCK_REALTIME, &rtos_alloc_start);
	//	clock_t rtos_begin = clock();
		void *p1 = rtos_malloc(s);
	//	clock_t rtos_end = clock();

		rtos_time2 = clock_gettime(CLOCK_REALTIME,&rtos_alloc_finish);
		rtos_alloc_time = rtos_alloc_finish.tv_nsec - rtos_alloc_start.tv_nsec;
		//size_t size = rtos_alloc_size(p);
		malloc_time1 = clock_gettime(CLOCK_REALTIME,&malloc_start);
	//	clock_t malloc_begin = clock();
		void *p2 = malloc(s);
	//	clock_t malloc_end = clock();
		malloc_time2 = clock_gettime(CLOCK_REALTIME,&malloc_finish);
		malloc_time = malloc_finish.tv_nsec - malloc_start.tv_nsec;
		printf("size is: %ld, rtos_alloc time is:%f, malloc time is:%f\n", s, rtos_alloc_time, malloc_time);
	
//		t += s;
	}

//	total = rtos_total_allocated();
	
	//printf("total size is: %ld, total  size should be bigger than %ld\n", total, t);

	return 0;
}
