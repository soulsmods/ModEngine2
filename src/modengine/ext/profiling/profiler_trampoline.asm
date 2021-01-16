; zone_ctx* __profiler_zone_begin(char* name)
EXTERN __profiler_begin:PROC
; void __profiler_zone_end(zone_ctx* ctx);
EXTERN __profiler_end:PROC

EXTERN  __imp_TlsGetValue:PROC
EXTERN  __imp_TlsSetValue:PROC
EXTERN  __imp_GlobalAlloc:PROC

.code

profiler_zone PROC
    ; save argument registers
    push rcx
    push rdx
    push r8
    push r9

    sub rsp, 48h

    ; get TLS index and get pointer to the context stack
    mov ecx, DWORD PTR [rbx+28h]
    call QWORD PTR __imp_TlsGetValue

    ; See if the value is null and if we need to allocate a stack
    test rax, rax
    jne SHORT $noalloc

    ; Allocate a context stack if it's null
    mov edx, 4000h
    xor ecx, ecx
    call    QWORD PTR __imp_GlobalAlloc

$noalloc:
    ; r12 is our resident context pointer
    mov QWORD PTR [rax+10h], r12
    mov r12, rax

    ; Increment the context stack and store is back into TLS
    lea rdx, QWORD PTR [rax+18h]
    mov ecx, DWORD PTR [rbx+28h]
    call    QWORD PTR __imp_TlsSetValue

    add rsp, 48h

    ; notify the profiler that we're in the prelude of a marked zone
    push rbx

    push rdi
    push rsi
    push r12
    push r13
    push r14
    push r15
    sub rsp, 16
    movdqu  [rsp], xmm0
    sub rsp, 16
    movdqu  [rsp], xmm1
    sub rsp, 16
    movdqu  [rsp], xmm2
    sub rsp, 16
    movdqu  [rsp], xmm3
    sub rsp, 16
    movdqu  [rsp], xmm6
    sub rsp, 16
    movdqu  [rsp], xmm7
    sub rsp, 16
    movdqu  [rsp], xmm8
    sub rsp, 16
    movdqu  [rsp], xmm9
    sub rsp, 16
    movdqu  [rsp], xmm10
    sub rsp, 16
    movdqu  [rsp], xmm11
    sub rsp, 16
    movdqu  [rsp], xmm12
    sub rsp, 16
    movdqu  [rsp], xmm13
    sub rsp, 16
    movdqu  [rsp], xmm14
    sub rsp, 16
    movdqu  [rsp], xmm15

    sub rsp, 38h
    mov rcx, [rbx]
    mov rdx, rcx
    call __profiler_begin
    add rsp, 38h

    movdqu  xmm15, [rsp]
    add rsp, 16
    movdqu  xmm14, [rsp]
    add rsp, 16
    movdqu  xmm13, [rsp]
    add rsp, 16
    movdqu  xmm12, [rsp]
    add rsp, 16
    movdqu  xmm11, [rsp]
    add rsp, 16
    movdqu  xmm10, [rsp]
    add rsp, 16
    movdqu  xmm9, [rsp]
    add rsp, 16
    movdqu  xmm8, [rsp]
    add rsp, 16
    movdqu  xmm7, [rsp]
    add rsp, 16
    movdqu  xmm6, [rsp]
    add rsp, 16
    movdqu  xmm3, [rsp]
    add rsp, 16
    movdqu  xmm2, [rsp]
    add rsp, 16
    movdqu  xmm1, [rsp]
    add rsp, 16
    movdqu  xmm0, [rsp]
    add rsp, 16
    pop r15
    pop r14
    pop r13
    pop r12
    pop rsi
    pop rdi

    pop rbx

    ; store the "context" of the profiled zone if required
    ;mov [rbx], rax ; prelude->zone_ctx = profiler_begin(name)
;
    ; restore parameters from stack back into registers
    pop r9
    pop r8
    pop rdx
    pop rcx

    ; pop old rbx off the stack and store it
    pop r10
    mov QWORD PTR [r12+8h], r10

    ; pop original return address off the stack
    mov r10, [rsp]

    ; store it in the context data frame
    mov [r12], r10

    ; set profiler epilogue as new return offset
    mov r10, [rbx+20h]
    mov [rsp], r10

    mov r10, [rbx+8h]
    jmp r10

profiler_zone ENDP

profiler_zone_exit PROC
    ; store return value, if any
    push rax
    push rbx

    push rdi
    push rsi
    push r12
    push r13
    push r14
    push r15
    sub rsp, 16
    movdqu  [rsp], xmm0
    sub rsp, 16
    movdqu  [rsp], xmm1
    sub rsp, 16
    movdqu  [rsp], xmm2
    sub rsp, 16
    movdqu  [rsp], xmm3
    sub rsp, 16
    movdqu  [rsp], xmm6
    sub rsp, 16
    movdqu  [rsp], xmm7
    sub rsp, 16
    movdqu  [rsp], xmm8
    sub rsp, 16
    movdqu  [rsp], xmm9
    sub rsp, 16
    movdqu  [rsp], xmm10
    sub rsp, 16
    movdqu  [rsp], xmm11
    sub rsp, 16
    movdqu  [rsp], xmm12
    sub rsp, 16
    movdqu  [rsp], xmm13
    sub rsp, 16
    movdqu  [rsp], xmm14
    sub rsp, 16
    movdqu  [rsp], xmm15

    ; notify the profiler about the end of the zone
    sub rsp, 30h
    call __profiler_end
    add rsp, 30h

    movdqu  xmm15, [rsp]
    add rsp, 16
    movdqu  xmm14, [rsp]
    add rsp, 16
    movdqu  xmm13, [rsp]
    add rsp, 16
    movdqu  xmm12, [rsp]
    add rsp, 16
    movdqu  xmm11, [rsp]
    add rsp, 16
    movdqu  xmm10, [rsp]
    add rsp, 16
    movdqu  xmm9, [rsp]
    add rsp, 16
    movdqu  xmm8, [rsp]
    add rsp, 16
    movdqu  xmm7, [rsp]
    add rsp, 16
    movdqu  xmm6, [rsp]
    add rsp, 16
    movdqu  xmm3, [rsp]
    add rsp, 16
    movdqu  xmm2, [rsp]
    add rsp, 16
    movdqu  xmm1, [rsp]
    add rsp, 16
    movdqu  xmm0, [rsp]
    add rsp, 16
    pop r15
    pop r14
    pop r13
    pop r12
    pop rsi
    pop rdi

    ; restore our saved register and original return value
    pop rbx

    sub rsp, 48h
    ; Get pointer to context
    mov ecx, DWORD PTR [rbx+28h]
    call QWORD PTR __imp_TlsGetValue

    ; Pop context stack frame and store it
    lea rdx, QWORD PTR [rax-18h]
    mov r12, rdx
    mov ecx, DWORD PTR [rbx+28h]
    call    QWORD PTR __imp_TlsSetValue
    add rsp, 48h

    ; Restore return address, rbx, and r12
    pop rax
    push [r12]
    mov rbx, [r12+8h]
    mov r12, [r12+10h]

    ; return back to function call site
    ret
profiler_zone_exit ENDP

END