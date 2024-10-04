#include "cpen212vm.h"

typedef struct {
    size_t num_of_physpages;
    FILE *swap_file;
    size_t num_of_swappages;
    void *physical_mem;
    char *first_free_page;
} mercury_vm;

typedef struct {
    uint32_t valid     : 1;
    uint32_t present   : 1;
    uint32_t readable  : 1;
    uint32_t writable  : 1;
    uint32_t executable: 1;
    uint32_t user      : 1;
    uint32_t accessed  : 1;
    uint32_t reserved  : 5;
    uint32_t ppn       : 20;
} mercury_pte;

typedef struct {
    char* next;
    uint32_t ppn;
} page_header;

void *vm_init(void *phys_mem, size_t num_phys_pages, FILE *swap, size_t num_swap_pages) {
    // TODO
    

    mercury_vm *vm = (mercury_vm *)(phys_mem);
    
    vm->physical_mem = phys_mem;
    vm->num_of_physpages = num_phys_pages;
    vm->swap_file = swap;
    vm->num_of_swappages = num_swap_pages;
    vm->first_free_page = (char *)phys_mem + 4096;

    //intialize free page
    uint32_t ppn = 1;
    page_header *current_page = (page_header *) vm->first_free_page;
    for(int i = 1; i <= num_phys_pages; i++){
        current_page -> ppn = i;

        if(i == num_phys_pages) break;

        current_page -> next = (char *) current_page + 4096;
        current_page = (page_header *) current_page -> next;
    }


    return vm;
    
}

void vm_deinit(void *vm) {
    // TODO
}

vm_result_t vm_map_page(void *vm, bool new_process, paddr_t pt, vaddr_t addr,
                        bool user, bool exec, bool write, bool read) {
                             
    mercury_vm *virtual = (mercury_vm *) vm;

    uintptr_t first_idx = (addr >> 22) & 0x3FF;
    uintptr_t second_idx = (addr >> 12) & 0x3FF;
    uintptr_t offset = addr & 0xFFF;

    if(new_process){ //allocate new top level and new second level
        // find the first two free pages two allocate 
        char *first_free_page = virtual->first_free_page; //first free page
        page_header *first_page = (page_header *)first_free_page;

        char *next_free_page = first_page->next; // second free page
        page_header *next_page = (page_header *) next_free_page;

        char *physical_mem_page = next_page->next; // third free page
        page_header *phys_page = (page_header *) physical_mem_page;
        
        if(next_free_page == NULL || first_free_page == NULL || physical_mem_page == NULL){
            return (vm_result_t){VM_OUT_OF_MEM};
        }
        // what is the current and next free pages ppn
        uint32_t current_ppn = first_page->ppn;
        uint32_t next_ppn = next_page->ppn;
        uint32_t phys_ppn = phys_page->ppn;
        
        uintptr_t top_entry = (uintptr_t) first_free_page; // top level

        uintptr_t top_level_idx = top_entry + 4 * first_idx; // index at top level

        mercury_pte *first_pte = (mercury_pte *) top_level_idx; // first pte

        first_pte->valid = 1;
        first_pte->present = 1;
        first_pte->readable = (uint32_t) read;
        first_pte->writable = (uint32_t) write;
        first_pte->executable = (uint32_t) exec;
        first_pte->user = (uint32_t) user;
        first_pte->accessed = 1;
        first_pte->reserved = 1;
        first_pte->ppn = next_ppn;

        uintptr_t second_entry = (uintptr_t) next_free_page; // second level

        uintptr_t second_level_idx = second_entry + 4 * second_idx; // index at second level

        mercury_pte *second_pte = (mercury_pte *) second_level_idx; // second pte

        second_pte->valid = 1;
        second_pte->present = 1;
        second_pte->readable = (uint32_t) read;
        second_pte->writable = (uint32_t) write;
        second_pte->executable = (uint32_t) exec;
        second_pte->user = (uint32_t) user;
        second_pte->accessed = 1;
        second_pte->reserved = 1;
        second_pte->ppn = phys_ppn;

        // no need to care about physical mem
        
        // now remove allocated three pages from free list

        char *current_page = virtual->first_free_page;
        char *prev_page = NULL;

        while(current_page != NULL){
            page_header *current_header = (page_header *) current_page;
            if(current_header->ppn == current_ppn || current_header->ppn == next_ppn || current_header->ppn == phys_ppn){
                if(prev_page == NULL){
                    virtual->first_free_page = current_header->next;
                }
                else{
                    page_header *prev_header = (page_header *) prev_page;
                    prev_header->next = current_header->next;
                }
            }
            prev_page = current_page;
            current_page = current_header->next;
        }
        
        return (vm_result_t){VM_OK, (paddr_t)top_entry};

    }
    else{ //shared top level and we don't know second level is allocated
        // find the first two free pages two allocate 

        uintptr_t top_lvl_entry = (uintptr_t) pt;

        // case 1 : already existing 2nd level
        uintptr_t top_lvl_idx = top_lvl_entry + 4 * first_idx; // index at top level

        mercury_pte *top_pte = (mercury_pte *) top_lvl_idx; // top pte



        if(top_pte->valid == 1){ //existing 2nd level
            uint32_t second_page_ppn = top_pte->ppn;

            uintptr_t second_lvl_entry = (uintptr_t) vm + 4096 * second_page_ppn; // go to second page 
            uintptr_t second_lvl_idx = second_lvl_entry + 4 * second_idx; // index at second level

            mercury_pte *second_pte = (mercury_pte *) second_lvl_idx; // second pte

            if(second_pte->valid == 1){ //already existing third level
                return (vm_result_t) {VM_DUPLICATE};
            }

            else{ // new allocatable third level
                char *physical_mem_page = virtual->first_free_page; // third free page to physical mem
                page_header *phys_page = (page_header *) physical_mem_page;
                
                if(physical_mem_page == NULL){
                    return (vm_result_t){VM_OUT_OF_MEM};
                }

                uint32_t phys_ppn = phys_page->ppn;

                second_pte->valid = 1;
                second_pte->present = 1;
                second_pte->readable = (uint32_t) read;
                second_pte->writable = (uint32_t) write;
                second_pte->executable = (uint32_t) exec;
                second_pte->user = (uint32_t) user;
                second_pte->accessed = 1;
                second_pte->reserved = 1;
                second_pte->ppn = phys_ppn;

                // now remove allocated page from free list
                char *current_page = virtual->first_free_page;
                char *prev_page = NULL;

                while(current_page != NULL){
                    page_header *current_header = (page_header *) current_page;
                    if(current_header->ppn == phys_ppn){
                        if(prev_page == NULL){
                            virtual->first_free_page = current_header->next;
                        }
                        else{
                            page_header *prev_header = (page_header *) prev_page;
                            prev_header->next = current_header->next;
                        }
                    }
                    prev_page = current_page;
                    current_page = current_header->next;
                }

                return (vm_result_t){VM_OK, (paddr_t)pt};
            }
        }

        else{ //non-existing 2nd level

            char *next_free_page = virtual->first_free_page; // free page (2nd level)
            page_header *next_page = (page_header *) next_free_page;

            if(next_free_page == NULL) return (vm_result_t){VM_OUT_OF_MEM};

            uint32_t second_level_ppn = next_page->ppn;

            //fill in top_pte content for top level
            top_pte->valid = 1;
            top_pte->present = 1;
            top_pte->readable = (uint32_t) read;
            top_pte->writable = (uint32_t) write;
            top_pte->executable = (uint32_t) exec;
            top_pte->user = (uint32_t) user;
            top_pte->accessed = 1;
            top_pte->reserved = 1;
            top_pte->ppn = second_level_ppn;

            char *physical_mem_page = next_page->next;
            page_header *phys_page = (page_header *) physical_mem_page;

            if(physical_mem_page == NULL) return (vm_result_t){VM_OUT_OF_MEM};

            uint32_t phys_ppn = phys_page->ppn;

            uintptr_t second_level_entry = (uintptr_t) next_free_page;
            uintptr_t second_level_idx = second_level_entry + 4 * second_idx;

            mercury_pte *second_pte = (mercury_pte *) second_level_idx;

            second_pte->valid = 1;
            second_pte->present = 1;
            second_pte->readable = (uint32_t) read;
            second_pte->writable = (uint32_t) write;
            second_pte->executable = (uint32_t) exec;
            second_pte->user = (uint32_t) user;
            second_pte->accessed = 1;
            second_pte->reserved = 1;
            second_pte->ppn = phys_ppn;

            //now remove allocated pages from free list

            char *current = virtual->first_free_page;
            char *prev = NULL;

            while(current != NULL){
                page_header *current_header = (page_header *) current;
                if(current_header->ppn == second_level_ppn || current_header->ppn == phys_ppn){
                    if(prev == NULL){
                        virtual->first_free_page = current_header->next;
                    }
                    else{
                        page_header *prev_header = (page_header *) prev;
                        prev_header->next = current_header->next;
                    }
                }
                prev = current;
                current = current_header->next;
            }

            return (vm_result_t) {VM_OK, (paddr_t)pt};
        }

        
    }
}

vm_status_t vm_unmap_page(void *vm, paddr_t pt, vaddr_t addr) {
    // TODO
}

vm_result_t vm_translate(void *vm, paddr_t pt, vaddr_t addr, access_type_t acc, bool user) {
    // TODO

    uintptr_t first_idx = (addr >> 22) & 0x3FF; //first index
    uintptr_t second_idx = (addr >> 12) & 0x3FF; //second index
    uintptr_t offset = addr & 0xFFF; //offset

    uintptr_t start = (uintptr_t) vm;
    start += first_idx * 4 + pt; //located at the index of the first table

    mercury_pte *addr_2nd_table = (mercury_pte *) start; //located at the index of the first table , current value is offeset of 2nd table
    uint32_t addr_2nd_table_bshift = addr_2nd_table->ppn;
    

    if(addr_2nd_table->valid == 0){
        return (vm_result_t) {VM_BAD_PERM};
    }
    else if(acc == VM_EXEC && addr_2nd_table->executable == 0){

        return (vm_result_t) {VM_BAD_PERM};
    }
    else if(acc == VM_WRITE && addr_2nd_table->writable == 0){

        return (vm_result_t) {VM_BAD_PERM};
    }
    else if(acc == VM_READ && addr_2nd_table->readable == 0){

        return (vm_result_t) {VM_BAD_PERM};
    }

    else if(user  && addr_2nd_table->user == 0){

        return (vm_result_t) {VM_BAD_PERM};
    }
    
    else if(addr_2nd_table->valid == 0){

        return (vm_result_t) {VM_BAD_IO};
    }
    else if(addr_2nd_table->present == 0){
        return (vm_result_t) {VM_BAD_ADDR};
    }

    else{
        uintptr_t start2 = (uintptr_t) vm;

        start2 += second_idx * 4 + addr_2nd_table_bshift * 4096; //located at the index of the second table

        mercury_pte *physical_page_addr = (mercury_pte *)start2;
        uint32_t physical_page_addr_bshift = physical_page_addr->ppn;

        uintptr_t start3 = (uintptr_t) vm;

        start3 = offset + physical_page_addr_bshift * 4096; //located at the physical memory we want

        
        return (vm_result_t){VM_OK, (uint32_t)start3};  
    }
}

void vm_reset_accessed(void *vm, paddr_t pt) {
    // TODO
}
