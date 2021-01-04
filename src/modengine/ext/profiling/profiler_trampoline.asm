; zone_ctx* __profiler_zone_begin(char* name)
EXTERN __profiler_begin:PROC
; void __profiler_zone_end(zone_ctx* ctx);
EXTERN __profiler_end:PROC

.code

profiler_zone PROC
    ; pop original return address off the stack
    mov r10, [rsp]

    ; store it in the profiler data block
    mov [rbx+18h], r10

    ; set profiler prologue as new return offset
    mov r10, [rbx+30h]
    mov [rsp], r10

    push rcx
    push rdx
    push r8
    push r9

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

    sub rsp, 30h
    mov rcx, [rbx+8h]
    mov rdx, rcx
    call __profiler_begin
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

    pop rbx

    ; store the "context" of the profiled zone if required
    mov [rbx], rax ; prelude->zone_ctx = profiler_begin(name)
;
    ; restore parameters from stack back into registers
    pop r9
    pop r8
    pop rdx
    pop rcx

    mov r10, [rbx+10h]
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
    pop rax
    push [rbx+18h]
    mov rbx, [rbx+38h]

    ; return back to function call site
    ret
profiler_zone_exit ENDP

END