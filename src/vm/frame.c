#include "vm/frame.h"
#include "threads/synch.h"
#include "userprog/pagedir.h"
#include "threads/thread.h"
#include "threads/interrupt.h"
#include "threads/vaddr.h"

struct list page_frame;
struct lock frame_lock;
struct list_elem *frame_pos;

void frame_init() {
	list_init(&page_frame);
	lock_init(&frame_lock);
	frame_pos = NULL;
}

void insert_page_to_frame(struct page *pg) {
	lock_acquire(&frame_lock);
	list_push_back(&page_frame, &pg->frame_elem);
	lock_release(&frame_lock);
}

struct page* find_page(void *kaddr) {
	lock_acquire(&frame_lock);
	struct list_elem *e;
	for(e = list_begin(&page_frame);e != list_end(&page_frame);e = list_next(e)) {
		struct page *p = list_entry(e, struct page, frame_elem);
		if(p->kaddr == kaddr)
			return p;
	}
	lock_release(&frame_lock);
	return NULL;
}

void delete_from_frame(struct page* p) {
	lock_acquire(&frame_lock);
	if(frame_pos == &(p->frame_elem))
		frame_pos = list_remove(frame_pos);
	else
		list_remove(&(p->frame_elem));
	lock_release(&frame_lock);
}

struct list_elem *get_next() {
	if( frame_pos == NULL || frame_pos == list_end(&page_frame)) {
		if(list_empty(&page_frame))
			return NULL;
		else
			return (frame_pos = list_begin(&page_frame));
	}
	frame_pos = list_next(frame_pos);
	if(frame_pos == list_end(&page_frame))
		return get_next();
	return frame_pos;
}

struct page* find_victim() {
	struct page* p;
	struct list_elem *e;

	e = get_next();

	p = list_entry(e,struct page, frame_elem);
	// page frame에서 access 된 적이 없는 페이지를 찾는다. 액세스가 다 되어 있다면 false로 만든다.
	while(p->entry->pinned || pagedir_is_accessed(p->t->pagedir, p->entry->vaddr)) {
		pagedir_set_accessed(p->t->pagedir,p->entry->vaddr,false);
		e = get_next();
		p = list_entry(e,struct page,frame_elem);
	}
	return p;
}
