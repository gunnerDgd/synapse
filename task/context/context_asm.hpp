#pragma once
#include <iostream>

#define restore_context(tsk_ctx) asm volatile(\
"movq %0, %%rax\n\t"\
"movq %1, %%rbx\n\t"\
"movq %2, %%rcx\n\t"\
"movq %3, %%rdx\n\t"\
\
"movq %6, %%rsi\n\t"\
"movq %7, %%rdi\n\t"\
\
"movq %8, %%r8\n\t"\
"movq %9, %%r9\n\t"\
"movq %10, %%r10\n\t"\
"movq %11, %%r11\n\t"\
"movq %12, %%r12\n\t"\
"movq %13, %%r13\n\t"\
"movq %14, %%r14\n\t"\
"movq %15, %%r15\n\t"\
::\
"g"(tsk_ctx.rax), "g"(tsk_ctx.rbx), "g"(tsk_ctx.rcx), "g"(tsk_ctx.rdx),\
"g"(tsk_ctx.rsp), "g"(tsk_ctx.rbp),\
"g"(tsk_ctx.rsi), "g"(tsk_ctx.rdi),\
"g"(tsk_ctx.r8),\
"g"(tsk_ctx.r9),\
"g"(tsk_ctx.r10),\
"g"(tsk_ctx.r11),\
"g"(tsk_ctx.r12),\
"g"(tsk_ctx.r13),\
"g"(tsk_ctx.r14),\
"g"(tsk_ctx.r15)\
:\
"rax", "rbx", "rcx", "rdx",\
"rsi", "rdi",\
"r8",  "r9",  "r10", "r11",\
"r12", "r13", "r14", "r15" \
)
#define save_context(tsk_ctx) asm volatile(\
"movq %%rax, %0\n\t"\
"movq %%rbx, %1\n\t"\
"movq %%rcx, %2\n\t"\
"movq %%rdx, %3\n\t"\
\
"movq %%rsp, %4\n\t"\
"movq %%rbp, %5\n\t"\
\
"movq %%rsi, %6\n\t"\
"movq %%rdi, %7\n\t"\
\
"movq %%r8, %8\n\t"\
"movq %%r9, %9\n\t"\
"movq %%r10, %10\n\t"\
"movq %%r11, %11\n\t"\
"movq %%r12, %12\n\t"\
"movq %%r13, %13\n\t"\
"movq %%r14, %14\n\t"\
"movq %%r15, %15\n\t"\
"leaq 0x00(%%rip), %16"\
:\
"=g"(tsk_ctx.rax), "=g"(tsk_ctx.rbx), "=g"(tsk_ctx.rcx), "=g"(tsk_ctx.rdx),\
"=g"(tsk_ctx.rsp), "=g"(tsk_ctx.rbp),\
"=g"(tsk_ctx.rsi), "=g"(tsk_ctx.rdi),\
"=g"(tsk_ctx.r8),\
"=g"(tsk_ctx.r9),\
"=g"(tsk_ctx.r10),\
"=g"(tsk_ctx.r11),\
"=g"(tsk_ctx.r12),\
"=g"(tsk_ctx.r13),\
"=g"(tsk_ctx.r14),\
"=g"(tsk_ctx.r15),\
"=g"(tsk_ctx.rip)\
)

#define trace_bp(bp)  asm volatile ( "movq 0x08(%%rbp), %0" : "=g"(bp))
#define trace_ip(ip)  asm volatile ( "movq 0x10(%%rbp), %0" : "=g"(ip))

#define modify_bp(bp) asm volatile ( "movq %0, 0x08(%%rbp)" :: "g"(bp))
#define modify_ip(ip) asm volatile ( "movq %0, 0x10(%%rbp)" :: "g"(ip))