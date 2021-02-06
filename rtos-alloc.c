#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <stdbool.h>

#include "rtos-alloc.h"
//#define _XOPEN_SOURCE 500
//#define _DEFAULT_SOURCE
#include <unistd.h>

/*
typedef struct allocation{


    void *a_base;
    size_t a_len;

    struct allocation *a_next;
    struct allocation *a_prev;


} alloc_t;



alloc_t *head = NULL;


void add_block(alloc_t* block){

        block->a_prev = NULL;
        block->a_next = NULL;
        if(!head || ((unsigned long) head >  (unsigned long) block)){
            
            if(head){

                head->a_prev = block;    


            }
            block->a_next = head;
            head = block;

        }

        else{
            alloc_t *temp = head;
            
            while(temp->a_next &&((unsigned long) temp->a_next < (unsigned long)block)){

                temp = temp->a_next;

            }
            block->a_next = temp->a_next;
            temp->a_next = block;

        }
}



void remove_block(alloc_t* block){


        if(!block->a_prev){
                
            if(block->a_next){

                head = block->a_next;

            }
            else{

                head = NULL;

            }



        }
        else{

            block->a_prev->a_next = block->a_next;



        }


        if(block->a_next){

                block->a_next->a_prev = block->a_prev;



        }



}



void scan_and_merge(){

    alloc_t *temp = head;
    unsigned long head_curr;
    unsigned long head_next;
    unsigned long program_break = (unsigned long)sbrk(0);

    if(program_break == 0){
        return;

    }

    while(temp->a_next){

        head_curr = (unsigned long) temp;
        head_next = (unsigned long) temp->a_next;
        if((head_curr + temp->a_len + sizeof(alloc_t)) == head_next){

            temp->a_len += temp->a_next->a_len + sizeof(alloc_t);
            temp->a_next = temp->a_next->a_next;
            
            if(temp->a_next){
                
                temp->a_next->a_prev = temp;


            }
            else{

                break;

            }
            
        }

        temp = temp->a_next;

            
    }

    head_curr = (unsigned long) temp;

    if((head_curr + temp->a_len + sizeof(alloc_t) == program_break) && (temp->a_len >= sysconf(_SC_PAGESIZE))){

        remove_block(temp);
        //if(brk(temp) != 0){
          //  printf("error freeing memory");            
        //}

    }


}





alloc_t* split(alloc_t * block, size_t size){

    void *mem_block = (void*) ((unsigned long) block + sizeof(alloc_t));
    alloc_t *newptr = (alloc_t*) ((unsigned long)mem_block + size);
    newptr->a_len = block->a_len - (size + sizeof(alloc_t));
    block->a_len = size;

    return newptr;


}




void*	rtos_malloc(size_t size){


        void* block_mem;
        alloc_t *ptr, *newptr;
        size_t alloc_size = 0;
        if(size >= 3*sysconf(_SC_PAGESIZE)){
            size + sizeof(alloc_t);
        }
        else{
            size = 3*sysconf(_SC_PAGESIZE);

        }

        ptr = head;
        while(ptr){
        
            if(ptr->a_len >= size + sizeof(alloc_t)){
                block_mem = (void*) ((unsigned long) ptr + sizeof(alloc_t));
                remove_block(ptr);
                if(ptr->a_len == size){
                    
                    return block_mem;

                }

                newptr = split(ptr, size);
                add_block(newptr);

                return block_mem;
            }
            else{

                ptr = ptr->a_next;
            }


        }

        ptr = sbrk(alloc_size);
        if(!ptr){

            printf("failed to alloc due to alloc size");
            return NULL;

        }
        ptr->a_next = NULL;
        ptr->a_prev = NULL;
        ptr->a_len = alloc_size - sizeof(alloc_t);
        if(alloc_size > size + sizeof(alloc_t)){

            newptr = split(ptr, size);
            add_block(newptr);


        }
        return (void*) ((unsigned long)ptr + sizeof(alloc_t));



}






void*	rtos_realloc(void *ptr, size_t size){



        return ptr;



}



void	rtos_free(void *ptr){

        
    add_block((void *)((unsigned long)ptr - sizeof(alloc_t)));
    scan_and_merge();


}




size_t	rtos_alloc_size(void *ptr){


        return 0;


}




bool	rtos_allocated(void *ptr){

        return true;

}




size_t	rtos_total_allocated(void){


        return 2;

}


void _cleanup(){

    if(head){
        if(brk(head) != 0) {
        }
    } 


    head = NULL;

}

*/
int main(int argc, char argv[]){
    

    printf("start testing");


  //  int* x = rtos_malloc(5*sizeof(int));
    //printf("after allocation");
    //*x = 5;
    //int * x1 = x+1;
    //printf("first number is: %d\n", *x);

    //*x1 = 2;
    //printf("second number is: %d\n", *x1);


    //rtos_free(x);



}
