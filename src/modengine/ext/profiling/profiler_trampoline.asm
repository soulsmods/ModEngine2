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
    sub rsp, 28h
    mov rcx, [rbx+8h]
    call __profiler_begin
    add rsp, 28h
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

    ; notify the profiler about the end of the zone
    sub rsp, 28h
    call __profiler_end
    add rsp, 28h

    ; restore our saved register and original return value
    pop rbx
    pop rax
    push [rbx+18h]
    mov rbx, [rbx+38h]

    ; return back to function call site
    ret
profiler_zone_exit ENDP

END