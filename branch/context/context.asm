global load_context:function
global store_context:function

global load_execution_context:function
global store_execution_context:function

section .text


; void store_context(synapse::branch::context&)
;
; Store Current CPU Context to the Context Block.
; ** System V ABI Specifies That First Argument Will Be Stored At RDI Register.


store_context:

; Saves Generic Register.

    mov  qword[rdi]      , rax
    mov  qword[rdi + 8]  , rbx
    mov  qword[rdi + 16] , rcx
    mov  qword[rdi + 24] , rdx ; Save Generic Register.

; Saves Stack Instruction Pointer.

    mov  qword[rdi + 32] , rsi
    mov  qword[rdi + 40] , rdi

; Save Generic Extended Register.

    mov  qword[rdi + 48] , r8
    mov  qword[rdi + 56] , r9
    mov  qword[rdi + 64] , r10
    mov  qword[rdi + 72] , r11
    mov  qword[rdi + 80] , r12
    mov  qword[rdi + 88] , r13
    mov  qword[rdi + 96] , r14
    mov  qword[rdi + 104], r15
    
    retq


; void load_context(synapse::branch::context&)
;
; Load Previous CPU Context From the Context Block.
; ** System V ABI Specifies That First Argument Will Be Stored At RDI Register.


load_context:
; Load Generic Register

    mov rax, qword[rdi]
    mov rbx, qword[rdi + 8]
    mov rcx, qword[rdi + 16]
    mov rdx, qword[rdi + 24]

; Load Stack Instructor Register.
; RDI Stores Parameter of the Function, Thus It Will Be Backed Up At Last.

    mov rsi, qword[rdi + 32]

    mov r8 , qword[rdi + 48]
    mov r9 , qword[rdi + 56]
    mov r10, qword[rdi + 64]
    mov r11, qword[rdi + 72]
    mov r12, qword[rdi + 80]
    mov r13, qword[rdi + 88]
    mov r14, qword[rdi + 96]
    mov r15, qword[rdi + 104]

    mov rdi, qword[rdi + 40]
    retq

store_execution_context:
; RBP Store.
;
;   +-------------------------+------------------------+
;   |                         |                        |
;   |   Previous Frame's RIP  |  Previous Frame's RBP  |
;   |                         |                        |
;   +-------------------------+------------------------+

    mov rbx             , qword[rbp]
    mov qword[rdi + 128], rbx

; RSP Store
    mov rbx             , rbp
    add rbx             , 0x10
    mov qword[rdi + 120], rbx

; Store Previous RIP.
    mov rbx             , qword[rbp + 8]
    mov qword[rdi + 112], rbx
    
    retq

; void load_execution_context(synapse::branch::context&)

load_execution_context:
; Load Execution Stack.
    mov   rbp, qword[rdi + 128]
    mov   rsp, qword[rdi + 120]

    jmp        qword[rdi + 112]
    