#include "msequence.h"

void*			  init_memory_block   (struct msequence* _mseq, size_t _len)
{
	return malloc(_mseq->mseq_block_size * _len);
}

struct msequence* init_memory_sequence(size_t block_size)
{
	struct msequence* _mseq   = malloc(sizeof(struct msequence));
	
	_mseq->mseq_entry		  = init_list();
	_mseq->mseq_block_size    = block_size;
	_mseq->mseq_current_block = NULL;

	return _mseq;
}

struct mblock* init_mblock_page()
{
	struct mblock* _mblock = malloc(sizeof(struct mblock));
	
	_mblock->location	   = 0;
	_mblock->dirty		   = 0;
}

struct mblock* init_mblock_heap()
{

}

void			  read_memory_sequence (uint8_t* r_ctx, size_t r_len, struct msequence* r_seq)
{
	struct list_object* cur_block  = r_seq->mseq_entry->l_entry;
	size_t				cur_offset   = 0,
						cur_iosize   = 0,
						total_iosize = 0;

	while (r_len > 0)
	{
		cur_offset = r_seq->mseq_block_offset % r_seq->mseq_block_size;

		if (r_len > r_seq->mseq_block_size)
			cur_iosize = r_seq->mseq_block_size - cur_offset;
		else
			cur_iosize = r_len - cur_offset;

		memcpy(r_ctx + total_iosize,
			  ((struct mblock*)cur_block->l_ctx)->mblock_pointer + cur_offset,
			  cur_iosize);

		r_len					 -= cur_iosize;
		r_seq->mseq_block_offset += cur_iosize;
		total_iosize			 += cur_iosize;

		if (cur_block->l_next == NULL) return;
	}
}

void			  write_memory_sequence(uint8_t* w_ctx, size_t w_len, struct msequence* w_seq)
{
	struct list_object* cur_block	 = w_seq->mseq_entry->l_entry;
	size_t				cur_offset   = 0,
						cur_iosize   = 0,
						total_iosize = 0;

	while (w_len > 0)
	{
		cur_offset = w_seq->mseq_block_offset % w_seq->mseq_block_size;

		if (w_len > w_seq->mseq_block_size)
			cur_iosize = w_seq->mseq_block_size - cur_offset;
		else
			cur_iosize = w_len - cur_offset;

		memcpy(((struct mblock*)cur_block->l_ctx)->mblock_pointer + cur_offset,
			   w_ctx + total_iosize, cur_iosize);

		w_len					 -= cur_iosize;
		w_seq->mseq_block_offset += cur_iosize;
		total_iosize			 += cur_iosize;
	}

	
}

void			  add_memory_block(struct msequence* _mseq, struct mblock* _block)
{
	add_list_last((void*)_block, _mseq->mseq_entry);
}

void			  delete_memory_block(struct msequence* _mseq, size_t i)
{
	delete_list  (_mseq->mseq_entry, i);
}