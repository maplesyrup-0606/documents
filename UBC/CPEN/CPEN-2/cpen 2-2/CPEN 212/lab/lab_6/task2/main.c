#include "cpen212vm.h"
#include <stdlib.h>
#include <assert.h>

#define USED 0b11001
#define UNUSED 0b11000

typedef struct {
    uint64_t *phys_mem; //physical memory location    
    FILE *swap; //swap file location
    size_t num_phys_pages; // total number of physical pages
    size_t num_swap_pages; // total number of swap pages

    size_t first_free_phys_page_index; // index of the first free physical page -> each index multiplied by 4096 gives the physical address
    size_t num_process; // number of processes
    size_t remaining_free_pages;
    /**
    include locks later
    */
} vm_handler_t;

typedef struct {
    unsigned int valid : 1; //valid
    unsigned int present : 1; //present
    unsigned int readable : 1; //read
    unsigned int writable : 1; //write
    unsigned int executable : 1; //execute
    unsigned int user : 1; //user
    unsigned int accessed : 1; //accessed
    unsigned int reserved : 5; //reserved
    unsigned int page_num : 20; //physical address
} pte_t;

typedef struct {
    unsigned int data_offset: 12;
    unsigned int p2_offset: 10;
    unsigned int p1_offset: 10;
} va_t;

int main() {
   task2Test();
//    task3Test();
}

void test_vm(int test_num, vm_handler_t *vm, uint64_t phys_mem, size_t num_phys_pages, FILE *swap, size_t num_swap_pages, size_t first_free_phys_page_index, size_t num_process) {
   assert(vm->phys_mem == phys_mem);
   assert(vm->num_phys_pages == num_phys_pages);
   assert(vm->num_swap_pages == num_swap_pages);
   assert(vm->first_free_phys_page_index == first_free_phys_page_index);
   assert(vm->num_process == num_process);
   assert(vm->swap == swap);
   // printf("[PASSED] Test %d for test_vm\n", test_num);
}

void test_result(int test_num, vm_result_t result ,vm_status_t status, paddr_t addr) {
   assert(result.status == status);
   if (result.status == VM_OK) {
      assert(result.addr == addr);
   }
   printf("[PASSED] Test %d for test_result\n", test_num);
}

void p_page(vm_handler_t *vm, size_t page_num) {
   for (int i = 0; i < 1024; i++) {
      pte_t *pte = (pte_t *)((char *)vm->phys_mem + page_num * 4096 + i * 4);
      printf("Entry: %d ", i);
      printf("page_num: %d", pte->page_num);
      printf(", valid: %d", pte->valid);
      printf(", present: %d", pte->present);
      printf(", reserved: %d", pte->reserved);
      printf(", accessed: %d", pte->accessed);
      printf(", user: %d", pte->user);
      printf(", executable: %d", pte->executable);
      printf(", writable: %d", pte->writable);
      printf(", readable: %d\n", pte->readable);
   }
}

void print_free_list(vm_handler_t *vm) {
   size_t curr_page_num = vm->first_free_phys_page_index;
   pte_t *pte = (pte_t *)((char *)vm->phys_mem + curr_page_num * 4096);
   while (curr_page_num != pte->page_num) {
      if (pte->reserved == USED) {
         printf("ERROR: USED PAGE %d SHOULD NOT BE IN FREE LIST", curr_page_num);
      } else {
         printf("%d -> ", curr_page_num);
         curr_page_num = pte->page_num;
      }
   }
}

vaddr_t make_vaddr(unsigned int p1, unsigned int p2, unsigned int data) {
   vaddr_t vaddr = p1;
   vaddr = vaddr << 10 + p2;
   vaddr = vaddr << 12 + data;
   return vaddr;
}

void task2Test() {
   int test_num = 1;

   /**
   Test Cases 1
      - basic tranlation
   */
   void * phys_mem = malloc(10*4096);
   vm_handler_t *vm = vm_init(phys_mem, 10, NULL, NULL);

   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 1, 0);

   vaddr_t vm_adr1 = 0;
   vm_result_t pt1 = vm_map_page(vm, true, NULL, vm_adr1, true, true, true, true);

   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 4, 1);
   test_result(test_num++, pt1, VM_OK, 4096);

   vm_result_t pa1 = vm_translate(vm, pt1.addr, vm_adr1, VM_READ, true);

   test_result(test_num++, pa1, VM_OK, 4096*3);

   /**
   Test Cases 2
      - permission error
      - did not create new tables
   */
   vaddr_t vm_adr2 = 0b00000000000000000001000000000000;
   //should not have created a new page but should have 2 more entries
   pt1 = vm_map_page(vm, false, pt1.addr, vm_adr2, false, false, false, false);
   // p_page(vm, 2, NULL);
   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 5, 1);
   test_result(test_num++, pt1, VM_OK, 4096);

   // not user perm
   vm_result_t pa2 = vm_translate(vm, pt1.addr, vm_adr2, VM_READ, true);
   test_result(test_num++, pa2, VM_BAD_PERM, NULL);

   // not readable
   pa2 = vm_translate(vm, pt1.addr, vm_adr2, VM_READ, false);
   test_result(test_num++, pa2, VM_BAD_PERM, NULL);

   // not writable
   pa2 = vm_translate(vm, pt1.addr, vm_adr2, VM_WRITE, false);
   test_result(test_num++, pa2, VM_BAD_PERM, NULL);

   // not executable
   pa2 = vm_translate(vm, pt1.addr, vm_adr2, VM_EXEC, false);
   test_result(test_num++, pa2, VM_BAD_PERM, NULL);
   /**
   Test Cases 3
      - 
   */
   free(phys_mem);
   return 0;
}

void task3Test() {
   int test_num = 1;
   /*
   Test Case 1:
      - make sure all the pte are checked before deleting a page
   */
   void * phys_mem = malloc(10*4096);
   vm_handler_t *vm = vm_init(phys_mem, 10, NULL, NULL);

   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 1, 0);

   vaddr_t adr1 = make_vaddr(0,0,0);
   // adding 3 pages
   vm_result_t pt1 = vm_map_page(vm, true, NULL, adr1, true, true, true, true);
   test_result(test_num++, pt1, VM_OK, 4096);
   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 4, 1);
   // print_free_list(vm);

   //deleting the 3 pages
   vm_status_t status = vm_unmap_page(vm, pt1.addr, adr1);
   assert(status == VM_OK);
   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 1, 0);

   // deleting the same 3 pages again
   vm_status_t status2 = vm_unmap_page(vm, pt1.addr, adr1);
   assert(status2 == VM_BAD_ADDR);
   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 1, 0);

   // adding 3 pages again
   vaddr_t adr2 = make_vaddr(8, 8, 67);
   vm_result_t pt2 = vm_map_page(vm, true, NULL, adr2, true, true, true, true);
   test_result(test_num++, pt2, VM_OK, 4096);
   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 4, 1);
   // print_free_list(vm);

   // adding 2 pages (one second level one data)
   vaddr_t adr3 = make_vaddr(1023, 8, 1688);
   vm_result_t pt2_v2 = vm_map_page(vm, false, pt2.addr, adr3, true, true, true, true);
   assert(pt2.addr == pt2_v2.addr);
   test_result(test_num++, pt2_v2, VM_OK, 4096);
   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 6, 1);
   // print_free_list(vm);

   // 1 data page
   vm_status_t status3 = vm_unmap_page(vm, pt2_v2.addr, adr3);
   assert(status3 == VM_OK);
   // test_vm(test_num++, vm, phys_mem, 10, NULL, NULL, 4, 1);
}