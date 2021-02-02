#include <synapse/task/context/context_asm.hpp>
#include <thread>

#define IN_FUNC __attribute__((always_inline))

namespace  context
{
	struct cpu_context
	{
		union
		{
			struct
			{
				uint64_t rax,
				 		 rbx,
				 		 rcx,
				 		 rdx,
		
				 		 rsp,
				 		 rbp,
		
				 		 rsi,
				 		 rdi,
		
				 		 r8,
				 		 r9,
						 r10,
				 		 r11,
				 		 r12,
				 		 r13,
				 		 r14,
				 		 r15,
		
				 		 rip;
			};
			uint64_t	 ctx_array[16] = { 0x00, };
		};
	};
	
	class context
	{
	public:
		void IN_FUNC save    () { save_context   (cpu_ctx); }
		void IN_FUNC restore () { restore_context(cpu_ctx); }
		
	public:
		cpu_context cpu_ctx;
	};
}

