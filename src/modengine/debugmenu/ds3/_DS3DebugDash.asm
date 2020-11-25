EXTERN bCheckDebugDashSwitch:QWORD

.data
	speedMulti DWORD 40A00000h
	pWorldChrManDbg QWORD 144768F98h
.code

tCheckDebugDashSwitch PROC
	movss dword ptr [rsi+8], xmm6
	movaps xmm7, xmm6
	movss xmm8, [speedMulti]
	mov rcx, [pWorldChrManDbg]
	mov rcx, [rcx]
	mov rax, [rcx+150h]
	test rax, rax
	jne loc_140B62A48
	mov rax, [r14+80h]
loc_140B62A48:
	cmp rbx, rax
	jne return
	mov r8, rbx
	movaps xmm1, xmm7
	call checkDebugDashSwitch
	test al, al
	je return
transfer:
	movaps xmm6, xmm7
	mulss xmm6, xmm8
	;jmp return
return:
	jmp [bCheckDebugDashSwitch]
tCheckDebugDashSwitch ENDP

.data
align 16
	specialvalue DWORD 3ECCCCCDh, 3ECCCCCDh, 3ECCCCCDh, 3ECCCCCDh
	specialvalue2 DWORD 0, 3F800000h, 0, 0
	pFD4PadManager QWORD 14494ECC0h
	debugDashSwitch QWORD 144768F7Eh
	checkButtonPress QWORD 14232F650h
.code

checkDebugDashSwitch PROC
	push rbx
	push rsi
	sub rsp, 78h
	movaps [rsp+50h], xmm7
	mov rsi, r8
	mov r8, [pFD4PadManager]
	mov r8, [r8]
	movaps xmm7, xmm1
	mov rbx, rcx
	mov rax, 143D6AD10h
	divss xmm7, dword ptr [rax]
	mov r9, [r8+38h]
	mov r10, 14473A7E7h
	mov rdx, [r9]
	mov rcx, rdx
	mov rax, [rdx+8]
	cmp byte ptr [rax+19h], 0
	jne loc_140B6E8F8
loc_140B6E8E0:
	cmp [rax+20h], r10
	jge loc_140B6E8EC
	mov rax, [rax+10h]
	jmp loc_140B6E8F2
loc_140B6E8EC:
	mov rcx, rax
	mov rax, [rax]
loc_140B6E8F2:
	cmp byte ptr [rax+19h], 0
	je loc_140B6E8E0
loc_140B6E8F8:
	cmp rcx, rdx
	je loc_140B6E908
	mov qword ptr [rsp+20h], rcx
	cmp r10, [rcx+20h]
	jge loc_140B6E90D
loc_140B6E908:
	mov qword ptr [rsp+20h], rdx
loc_140B6E90D:
	lea rax, [rsp+20h]
	mov [rsp+70h], rdi
	mov rax, [rax]
	cmp rax, rdx
	je loc_140B6E929
	mov rdi, [rax+28h]
	mov byte ptr [rax+30h], 1
	jmp loc_140B6E936
loc_140B6E929:
	xor edx, edx
	mov rcx, r8
	mov rax, 140462670h
	call rax
	mov rdi, rax
loc_140B6E936:
	mov rax, [debugDashSwitch]
	cmp byte ptr [rax], 0
	je loc_140B6E97B
	mov rcx, rdi
	call sub_1409BEF20
	test al, al
	jne loc_140B6E957
	mov rcx, rdi
	mov edx, 6
	call checkButtonPress
	test al, al
	je loc_140B6E97F
loc_140B6E957:
	cmp byte ptr [rbx+48h], 0
	lea rdx, [rsp+20h]
	mov rcx, rsi
	sete al
	mov [rbx+48h], al
	mov rax, 140882070h
	call rax
	mov rcx, rsi
	mov rdx, rax
	mov rax, 140887860h
	call rax
	jmp loc_140B6E97F
loc_140B6E97B:
	mov byte ptr [rbx+48h], 0
loc_140B6E97F:
	cmp byte ptr [rbx+48h], 0
	je loc_140B6EA47
loc_140B6E989:
	movaps [rsp+60h], xmm6
	movaps xmm6, [specialvalue]
	movss xmm0, xmm7
	mov rcx, rsi
	shufps xmm0, xmm0, 0
	mulps xmm6, [specialvalue2]
	mulps xmm6, xmm0
	call sub_1400421BD
	mov rcx, rdi
	mov edx, 1Ah
	call checkButtonPress
	test al, al
	je loc_140B6E9E0
	lea rdx, [rsp+30h]
	mov rcx, rsi
	mov rax, 140882070h
	call rax
	lea rdx, [rsp+20h]
	mov rcx, rsi
	movaps xmm0, [rax]
	addps xmm0, xmm6
	movaps [rsp+20h], xmm0
	mov rax, 140887860h
	call rax
loc_140B6E9E0:
	mov rcx, rdi
	mov edx, 1Bh
	call checkButtonPress
	test al, al
	je loc_140B6EA11
	lea rdx, [rsp+30h]
	mov rcx, rsi
	mov rax, 140882070h
	call rax
	lea rdx, [rsp+20h]
	mov rcx, rsi
	movaps xmm0, [rax]
	subps xmm0, xmm6
	movaps [rsp+20h], xmm0
	mov rax, 140887860h
	call rax
loc_140B6EA11:
	;there is no function to check the var here, so stub it
	movaps xmm6, [rsp+60h]
	mov al, 1 ;map hit default value
	movzx ebx, al
	test al, al
	je loc_140B6EA34
loc_140B6EA22:
	mov rcx, rdi
	mov edx, 1Ch
	call checkButtonPress
	xor ecx, ecx
	movzx ebx, bl
	test al, al
	cmovne ebx, ecx
loc_140B6EA34:
	mov rax, [rsi+50h]
	test bl, bl
	;sete cl
	jne clip
	or word ptr [rax+186h], 8
	jmp afterclip
clip:
	mov word ptr [rax+186h], 0
afterclip:
	;mov [rax+320], cl
	mov al, 1
	jmp loc_140B6EA54
loc_140B6EA47:
	mov rcx, [rsi+50h]
	xor al, al
	and byte ptr [rsi+1A08h], 0BFh
	;mov byte ptr [rcx+390h], 1
loc_140B6EA54:
	mov rdi, [rsp+70h]
	mov rcx, [rsp+40h]
	movaps xmm7, [rsp+50h]
	add rsp, 78h
	pop rsi
	pop rbx
	ret
checkDebugDashSwitch ENDP

sub_1409BEF20 PROC
	push rbx
	sub rsp, 20h
	mov rbx, rcx
	mov rax, 14080A970h
	call rax
	mov rcx, rbx
	mov edx, 19h
	test al, al
	jne loc_1409BEF3F
	mov edx, 18h
loc_1409BEF3F:
	add rsp, 20h
	pop rbx
	mov rax, 14232F650h
	jmp rax
sub_1409BEF20 ENDP

sub_1400421BD PROC
	or byte ptr [rcx+1A08h], 40h
	mov rax, [rcx+1F90h]
	mov rcx, [rax+68h]
	;mov byte ptr [rcx+390h], 0
	ret
sub_1400421BD ENDP

END