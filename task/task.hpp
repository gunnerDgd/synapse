#include <synapse/task/context/context.hpp>
#include <optional>
#include <map>

namespace context
{
	class task_base : public context
	{
	public:
		task_base() 
		{
			stack_bot = new uint8_t[64 * 1024];
			stack_top = stack_bot + 64 * 1024 - 8;
		}
			  
		~task_base() { delete stack_bot; }
	
	public:
		uint8_t  * stack_top     = nullptr,
			     * stack_bot     = nullptr;
				 
		task_base* task_prev     = nullptr;
	};
	
	class task_enumerator
	{
	public:
		using task_tree = std::map<uint64_t, task_base*>;
		task_enumerator()
		{
			task_base *tp_entry = new task_base();
			tp_entry			->save();
			
			add_task (*tp_entry);
		}
		
		void  add_task(task_base& tsk) { 
											task_map.insert(std::make_pair(tsk.cpu_ctx.rip, &tsk));
											tb_cur = &tsk;
									   }
		
		template <typename T>
		void 	  delete_task  (T tsk) { 
											task_base* tb_ptr = (*this)[tsk];
											tb_cur			  = tb_ptr->task_prev;
			
											task_map.erase		((uint64_t)tsk);
									   }
		
		task_base* current_task()	   { return tb_cur; }
		
		template <typename T>
		task_base* operator[] (T tsk)
		{
			auto    t_it = task_map.find((uint64_t)tsk);
			return (t_it != task_map.end())
				  ? t_it->second
				  : nullptr;
		}
		
		template <typename T, typename U>
		inline void IN_FUNC switch_task(T f_ca, U f_ce)
		{
			task_base* tb_ca = (*this)[f_ca], // Caller
					 * tb_ce = (*this)[f_ce]; // Callee
			
			if		  (tb_ca == nullptr || tb_ce == nullptr) return;
			tb_ca    ->save   ();
			tb_ce    ->restore(); // Restore Callee's Context.
			
			asm volatile
			(
				"movq 0x00(%1)   , %%rbp\n\t"
				"movq  %2        , %%rsp\n\t"
				
				"leaq 0x05(%%rip), %0   \n\t"
				"jmpq *%3				\n\t"
				
				"movq 0x00(%4)   , %%rbp\n\t" // Restore Caller's Stack
				"movq %5		 , %%rsp"	  // 
			:   "=g"			(tb_ca->cpu_ctx.rip)
				
			:   "g" ((uint64_t)&(tb_ce->cpu_ctx.rbp)),
				"g"			    (tb_ce->cpu_ctx.rsp) ,
				"g"			    (tb_ce->cpu_ctx.rip) ,
				
				"g" ((uint64_t)&(tb_ca->cpu_ctx.rbp)),
				"g"				(tb_ca->cpu_ctx.rsp)
			); 					 tb_ca->restore();
		}
	private:
		task_tree  task_map		   ;
		task_base* tb_cur = nullptr;
	};
	
	class task : public task_base
	{
	public:	
		template <typename tsk_fp, typename... tsk_arg> 
		inline IN_FUNC task(task_enumerator& tsk_enum ,
			 				tsk_fp           tsk_fptr ,	
			 				tsk_arg...		  tsk_par  ) : task_base	  ()		,
			 							   				   task_dispatcher(tsk_enum),
										   				   task_entry	  ((uint64_t)tsk_fptr)
		{
			task_base* tb_prv = task_dispatcher.current_task();
			
			tb_prv		  ->save();
			task_prev	  = tb_prv;
				
			asm volatile ( "movq 0x08(%%rbp), %0" : "=g"(tb_prv->cpu_ctx.rip));
			asm volatile
			(
				"movq %0, %%rsp"
			::  "g"		 (stack_top)
			);  tsk_fptr (task_dispatcher, std::forward(tsk_par...));
		}
		
		template <typename tsk_fp>
		inline IN_FUNC task(task_enumerator& tsk_enum ,
			 				tsk_fp           tsk_fptr ) : task_base	  	 ()		   ,
			 							  				  task_dispatcher(tsk_enum),
			 							  				  task_entry	 ((uint64_t)tsk_fptr)
		{
			task_base* tb_prv = task_dispatcher.current_task();
			
			tb_prv		  ->save();
			task_prev	  = tb_prv;
			
			asm volatile ( "movq %0, %%rsp" :: "g"(stack_top));
			tsk_fptr     (task_dispatcher);
			
			asm volatile 
			( 
				"movq 0x00(%0), %%rbp\n\t" // Restore Caller's Stack Context.
				"movq %1      , %%rsp"
			::  "g"((uint64_t)&(tb_prv->cpu_ctx.rbp)),
				"g"			   (tb_prv->cpu_ctx.rsp)
			);  tb_prv->restore();			
		}

		~task()
		{
			task_dispatcher.delete_task(task_entry);
		}

	private:
		task_enumerator& task_dispatcher;
		uint64_t		 task_entry;
	}; 
}