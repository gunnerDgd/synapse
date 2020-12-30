#pragma once
#include <synapse/container/list.h>
#include <Windows.h>

#include <stdint.h>
#include <string.h>

struct mblock
{
	uint8_t*		mblock_pointer;
	union
	{
		uint8_t     mblock_flags;
		struct
		{
			uint8_t dirty    : 1; // 1 : Dirty
			uint8_t location : 1; // 0 : Page, 1 : Heap
			uint8_t reserved : 6;
		};
	};
};

struct mblock* init_mblock_page();
struct mblock* init_mblock_heap();

struct msequence
{
	struct list_entry*  mseq_entry;
	
	size_t			    mseq_block_size,
					    mseq_block_offset;
	struct list_object* mseq_current_block;
};

void*			  init_memory_block	   (struct msequence* _mseq, size_t _len);
struct msequence* init_memory_sequence (size_t block_size);

void			  read_memory_sequence (uint8_t* r_ctx, size_t r_len, struct msequence* r_seq);
void			  write_memory_sequence(uint8_t* w_ctx, size_t w_len, struct msequence* w_seq);

void			  add_memory_block	   (struct msequence* _mseq, struct mblock* _block);
void			  delete_memory_block  (struct msequence* _mseq, size_t i);