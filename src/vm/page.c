#include "page.h"
#include "threads/thread.h"
#include "vm/frame.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "filesys/file.h"
#include "threads/palloc.h"

extern struct list page_frame;

static unsigned vm_table_hash_func(const struct hash_elem *, void * UNUSED);
static bool vm_table_less_func(const struct hash_elem *, const struct hash_elem *, void * UNUSED);
static void vm_table_destroy_func(struct hash_elem *, void * UNUSED);

void vm_table_init(struct hash *h) {
	ASSERT (h != NULL);
	hash_init(h, vm_table_hash_func, vm_table_less_func, NULL);
}

void vm_table_destroy(struct hash *h) {
	ASSERT (h != NULL);
	hash_destroy (h , vm_table_destroy_func);
}

static unsigned vm_table_hash_func(const struct hash_elem *e, void *aux UNUSED) {
	ASSERT (e != NULL);
	struct page_entry *pe = hash_entry(e,struct page_entry, elem);
	return hash_bytes (&(pe->vaddr),sizeof(pe->vaddr));
}

static bool vm_table_less_func(const struct hash_elem *a, const struct hash_elem *b, void *aux UNUSED) {
	ASSERT (a != NULL);
	ASSERT (b != NULL);
	struct page_entry *pa = hash_entry(a, struct page_entry, elem);
	struct page_entry *pb = hash_entry(b, struct page_entry, elem);
	return pa->vaddr < pb->vaddr;
}

bool insert_page_entry(struct hash *h, struct page_entry *pe) {
	return hash_insert(h,&(pe->elem));
}

bool delete_page_entry(struct hash *h, struct page_entry *pe) {
	if(!hash_delete(h, &pe->elem))
		return false;
	free_page_virtual(pe->vaddr);
	// TODO : manage swap table information
	free(pe);
	return true;
}

void free_page(struct page *p) {
	pagedir_clear_page(p->t->pagedir, p->entry->vaddr);
	delete_from_frame(p);
	palloc_free_page(p->kaddr);
	free(p);
}

void free_page_kernel(void *kaddr) {
	lock_acquire(&frame_lock);

	struct page *p = find_page(kaddr);
	if(p)
		free_page(p);

	lock_release(&frame_lock);
}

void free_page_virtual(void *vaddr) {
	free_page_kernel(pagedir_get_page(thread_current()->pagedir,vaddr));
}

static void vm_table_destroy_func(struct hash_elem *e, void *aux UNUSED) {
	ASSERT (e != NULL);
	struct page_entry *pe = hash_entry(e, struct page_entry, elem);
	// TODO : need to make free page function with virtual address
	free_page_virtual(pe->vaddr);
	// TODO : need to make managing swap table function
	// fill this line
	free(pe);
}

// search page_entry in current thread's page table
struct page_entry *search_vm_table(void *vaddr) {
	struct hash *h;
	struct page_entry pe;
	struct hash_elem* e;
	struct thread *t = thread_current();
	h = &(t->page_table);
	pe.vaddr = pg_round_down(vaddr);
	e = hash_find(h,&(pe.elem));
	if(e != NULL) 
		return hash_entry(e,struct page_entry,elem);
	else
		return NULL;
}

void replace_page() {
	lock_acquire(&frame_lock);
	struct page *victim = find_victim();
	bool dirty = pagedir_is_dirty(victim->t->pagedir, victim->entry->vaddr);
	if(dirty) {
		// TODO : swap out victim page
	}
	victim->entry->loaded = false;
	free_page(victim);
	lock_release(&frame_lock);
}

// 페이지를 만들고 페이지 프레임에 페이지를 넣는다.
struct page *malloc_page(enum palloc_flags flags) {
	struct page *p;
	p = (struct page*)malloc(sizeof(struct page));
	if(p == NULL)
		return NULL;
	memset(p,0,sizeof(struct page));
	p->t = thread_current();
	p->kaddr = palloc_get_page(flags);
	// if there's no page in user pool, keep trying
	while(p->kaddr == NULL) {
		replace_page();
		p->kaddr = palloc_get_page(flags);
	}
	return p;
}
