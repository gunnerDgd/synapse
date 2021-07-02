
global save_context:function
global restore_context:function

; save_context function
; ** void save_context(synapse::context::context*)
;
; Saves Current CPU Context to Context Struct.
; Uses System V Calling Convention.

section .text
save_context:
    mov  qword[rdi]      , rax
    mov  qword[rdi + 8]  , rbx
    mov  qword[rdi + 16] , rcx
    mov  qword[rdi + 24] , rdx ; Save Generic Register.

    mov  qword[rdi + 32] , rsi
    mov  qword[rdi + 40] , rdi ; Save Stack Context Register.

    mov  qword[rdi + 48] , rbp ; Save Base Pointer.
    mov  qword[rdi + 56] , rsp
    sub  qword[rdi + 56] , 0x8 ; Save Stack Pointer. ( -0x8 : Previous Function Call Stack)

    mov  qword[rdi + 64] , r8
    mov  qword[rdi + 72] , r9
    mov  qword[rdi + 80] , r10
    mov  qword[rdi + 96] , r11
    mov  qword[rdi + 104], r12
    mov  qword[rdi + 112], r13
    mov  qword[rdi + 120], r14
    mov  qword[rdi + 128], r15 ; Save Generic Extended Register.
    
    retq

restore_context:
    mov rbx, qword[rax + 8]
    mov rcx, qword[rax + 16]
    mov rdx, qword[rax + 24]
    
    mov rsi, qword[rax + 32]
    mov rdi, qword[rax + 40]

    mov rbp, qword[rax + 48]
    mov rsp, qword[rax + 56]

    mov r8 , qword[rax + 64]
    mov r9 , qword[rax + 72]
    mov r10, qword[rax + 80]
    mov r11, qword[rax + 88]
    mov r12, qword[rax + 96]
    mov r13, qword[rax + 104]
    mov r14, qword[rax + 112]
    mov r15, qword[rax + 120]

;    push rax
    
    
