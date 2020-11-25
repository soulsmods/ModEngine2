EXTERN bLoadDbgFont:QWORD
EXTERN bIncWindowCounter:QWORD
EXTERN bDecWindowCounter:QWORD
EXTERN TalkFontTpfPath:QWORD
EXTERN TalkFontCcmPath:QWORD
EXTERN DSFontTpfPath:QWORD
EXTERN DSFontCcmPath:QWORD

.data
	pFD4FontManager QWORD 14494EE58h
.code

tLoadDbgFont PROC
	mov r9, 14299EC68h
	mov r8, 14299EC98h
	mov rcx, [pFD4FontManager]
	mov rcx, [rcx]
	xor edx, edx
	call fLoadDbgFont
	lea r9, [DSFontTpfPath]
	lea r8, [DSFontCcmPath]
	mov rcx, [pFD4FontManager]
	mov rcx, [rcx]
	mov edx, 1
	call fLoadDbgFont
	lea r9, [TalkFontTpfPath]
	lea r8, [TalkFontCcmPath]
	mov rcx, [pFD4FontManager]
	mov rcx, [rcx]
	mov edx, 2
	call fLoadDbgFont
	mov r9, 14299EC68h
	mov r8, 14299EC98h
	mov rcx, [pFD4FontManager]
	mov rcx, [rcx]
	mov edx, 3
	call fLoadDbgFont
	mov rax, [r15+140h]
	jmp [bLoadDbgFont]
tLoadDbgFont ENDP

.data

.code

fLoadDbgFont PROC
	push rbx
	push rdi
	push r15
	sub rsp, 40h
	mov rbx, r9
	mov rax, r8
	mov r15d, edx
	mov rdi, rcx
	test r8, r8
	je nevercamethisearly
	test rbx, rbx
	je nevercamethisearly
	mov r8, [rcx+88h]
	lea rcx, [rsp+30h]
	mov [rsp+68h], rsi
	mov rdx, rax
	mov [rsp+78h], r14
	xor r14b, r14b
	mov rax, 14232A4B0h
	call rax
	mov rsi, rax
	test rax, rax
	je loc_142F6AB82
	mov r8, [rdi+88h]
	lea rcx, [rsp+70h]
	mov rdx, rbx
	mov [rsp+60h], rbp
	mov rax, 14232A4B0h
	call rax
	mov r9, [rsp+30h]
	mov rbp, rax
	mov rax, [rsp+70h]
	mov r8, rsi
	mov [rsp+28h], rax
	mov edx, r15d
	mov rcx, rdi
	mov [rsp+20h], rbp
	call sub_142F6ABB0
	movzx r14d, al
	mov rbp, [rsp+60h]
loc_142F6AB82:
	mov rsi, [rsp+68h]
	movzx eax, r14b
	mov r14, [rsp+78h]
	add rsp, 40h
	pop r15
	pop rdi
	pop rbx
	ret
nevercamethisearly:
	xor al, al
	add rsp, 40h
	pop r15
	pop rdi
	pop rbx
	ret
fLoadDbgFont ENDP

.data
	someByte BYTE 0
.code

sub_142F6ABB0 PROC
	mov [rsp+8], rcx
	push rsi
	push rdi
	push r14
	sub rsp, 60h
	mov qword ptr [rsp+30h], -2
	mov [rsp+88h], rbx
	mov r14, r8
	mov ebx, edx
	mov rdi, rcx
	test r8, r8
	je loc_142F6ADCB
	cmp qword ptr [rsp+0A0h], 0
	je loc_142F6ADCB
	lea rsi, [rcx+70h]
	mov rax, [rsi]
	mov r8, [rax+8]
	mov rdx, rax
	cmp byte ptr [r8+19h], 0
	jne loc_142F6AC1C
loc_142F6AC03:
	cmp [r8+20h], ebx
	jnb loc_142F6AC0F
	mov r8, [r8+10h]
	jmp loc_142F6AC15
loc_142F6AC0F:
	mov rdx, r8
	mov r8, [r8]
loc_142F6AC15:
	cmp byte ptr [r8+19h], 0
	je loc_142F6AC03
loc_142F6AC1C:
	cmp rdx, rax
	je loc_142F6AC2E
	cmp ebx, [rdx+20h]
	mov [rsp+90h], rdx
	jnb loc_142F6AC36
loc_142F6AC2E:
	mov [rsp+90h], rax
loc_142F6AC36:
	lea rax, [rsp+90h]
	mov rax, [rax]
	cmp rax, [rsi]
	je loc_142F6AC83
	cmp qword ptr [rax+28h], 0
	je loc_142F6AC83
	jmp loc_142F6ADCB
loc_142F6AC83:
	mov dword ptr [rsp+20h], 0F2h
	mov r9, 1455FFFF0h
	mov r8, [rcx+88h]
	mov edx, 8
	lea ecx, [rdx+20h]
	mov rax, 141769A30h
	call rax
	mov [rsp+90h], rax
	test rax, rax
	je loc_142F6ACC7
	mov rdx, [rdi+88h]
	mov rcx, rax 
	call initThingy
	mov rdi, rax
	jmp loc_142F6ACC9
loc_142F6ACC7:
	xor edi, edi
loc_142F6ACC9:
loc_142F6ACF0:
	mov r8, [rsp+0A0h]
	mov rdx, r14
	mov rcx, rdi
	call sub_14303F1A0
	test al, al
	jne loc_142F6AD9E
	mov rcx, rdi
	mov rax, 14176A8E0h
	mov rbx, rax
	mov r8, [rdi]
	xor edx, edx
	mov rcx, rdi
	call qword ptr [r8]
	mov r8, [rbx]
	mov rdx, rdi
	mov rcx, rbx
	call qword ptr [r8+68h]
	jmp returnzero
loc_142F6AD9E:
	mov [rsp+38h], ebx
	mov [rsp+40h], rdi
	mov rax, 14494E1E7h
	movzx eax, byte ptr [rax]
	mov [rsp+20h], al
	lea r9, [rsp+38h]
	xor r8d, r8d
	lea rdx, [rsp+48h]
	mov rcx, rsi
	;call sub_142F6BAD0
	mov rax, 140129960h
	call rax
	mov al, 1
	jmp ending
loc_142F6ADCB:
returnzero:
	xor al, al
ending:
	mov rbx, [rsp+88h]
	add rsp, 60h
	pop r14
	pop rdi
	pop rsi
	ret
sub_142F6ABB0 ENDP

.data
	thingyVtable QWORD 0, 0, 0
.code

initThingy PROC
	mov [rsp+8], rbx
	push rdi
	sub rsp, 20h
	mov rbx, rdx
	mov rdi, rcx
	call initThingySuper
	lea rax, [thingyMember2]
	mov [thingyVtable+8], rax
	lea rax, [thingyMember3]
	mov [thingyVtable+10h], rax
	lea rax, [thingyVtable]
	mov [rdi+10h], rbx
	mov rbx, [rsp+30h]
	mov [rdi], rax
	mov qword ptr [rdi+18h], 0
	mov dword ptr [rdi+20h], 12h
	mov rax, rdi
	add rsp, 20h
	pop rdi
	ret
initThingy ENDP

.data
	thingySuperVtable QWORD 0, 0, 0
.code

initThingySuper PROC
	mov rax, 141FC367Ch
	mov [thingySuperVtable+8], rax
	mov rax, 141FC367Ch
	mov [thingySuperVtable+10h], rax
	lea rax, [thingySuperVtable]
	mov qword ptr [rcx+8], 0
	mov [rcx], rax
	mov rax, rcx
	ret
initThingySuper ENDP

thingyMember2 PROC
	mov eax, [rcx+20h]
	ret
thingyMember2 ENDP

thingyMember3 PROC
	mov rax, [rcx+18h]
	ret
thingyMember3 ENDP

.data

.code

;took this from PS4
sub_46E9C0 PROC
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push rbx
	sub rsp, 18h
	mov r8, rcx
	mov r15, rdi
	mov rax, [r14]
	mov r10, [rsi+8]
	mov rax, [r10+8]
	cmp byte ptr [rax+19h], 0
	je loc_46E9FB
	lea r9, [rbp-30h]
	mov rdi, r10
	mov rcx, r10
	jmp loc_46EA35
loc_46E9FB:
	mov edi, [r8]
loc_46EA00:
	mov rcx, rax
	mov eax, [rcx+20h]
	cmp eax, edi
	setnb bl
	cmp edi, eax
	setb al
	test dl, dl
	jne loc_46EA16
	mov bl, al
loc_46EA16:
	lea rax, [rcx+10h]
	test bl, bl
	cmovne rax, rcx
	mov rax, [rax]
	cmp byte ptr [rax+19h], 0
	je loc_46EA00
	lea r9, [rbp-30h]
	test bl, 1
	je loc_46EAA2
	mov rdi, rcx
loc_46EA35:
	cmp rcx, [r10]
	je loc_46EA49
	cmp byte ptr [rcx+19h], 0
	je loc_46EA50
	mov rdi, [rcx+10h]
	mov r10b, 1
	jmp loc_46EABC
loc_46EA49:
	mov edx, 1
	jmp loc_46EAC8
loc_46EA50:
	mov rax, [rcx]
	cmp byte ptr [rax+19h], 0
	je loc_46EA90
	mov rbx, [rcx+8]
	cmp byte ptr [rbx+19h], 0
	jne loc_46EAB6
	mov rax, rcx
loc_46EA70:
	mov rdx, rbx
	cmp rax, [rdx]
	jne loc_46EAAA
	mov rbx, [rdx+8]
	mov rdi, rdx
	mov rax, rdx
	cmp byte ptr [rbx+19h], 0
	je loc_46EA70
	jmp loc_46EAAD
loc_46EA90:
	mov rdi, rax
	mov rax, [rdi+10h]
	cmp byte ptr [rax+19h], 0
	je loc_46EA90
	mov r10b, 1
	jmp loc_46EABC
loc_46EAA2:
	xor r10d, r10d
	mov rdi, rcx
	jmp loc_46EABC
loc_46EAAA:
	mov rbx, rdx
loc_46EAAD:
	cmp byte ptr [rax+19h], 0
	mov r10b, 1
	jne loc_46EABC
loc_46EAB6:
	mov r10b, 1
	mov rdi, rbx
loc_46EABC:
	mov eax, [rdi+20h]
	cmp eax, [r8]
	jnb loc_46EAD7
	movzx edx, r10b
loc_46EAC8:
	mov rdi, r15
	call sub_46EB10
	mov byte ptr [r15+18h], 1
	jmp loc_46EAEA
loc_46EAD7:
	vmovups xmm0, xmmword ptr [r9]
	vmovups xmmword ptr [r15], xmm0
	mov [r15+10h], rdi
	mov byte ptr [r15+18h], 0
loc_46EAEA:
	mov rax, r15
	add rsp, 18h
	pop rbx
	pop r14
	pop r15
	pop rbp
	ret
sub_46E9C0 ENDP

.data
	initGXTexture2D QWORD 14002AC20h
	pGXDeviceMaybe QWORD 14494BFD0h
.code

sub_14303F1A0 PROC
	mov [rsp+8], rcx
	push rbp
	push rsi
	push rdi
	sub rsp, 50h
	mov qword ptr [rsp+40h], -2
	mov [rsp+80h], rbx
	mov rdi, r8
	mov rsi, rcx
	test rdx, rdx
	je loc_14303F2B8
	test r8, r8
	je loc_14303F2B8
	mov r8, [rcx+10h]
	add rcx, 8
	call sub_14246B840
	test eax, eax
	jne loc_14303F2B8
	xor ebp, ebp
	mov [rsp+78h], rbp
	mov [rsp+88h], rbp
	mov rax, [pGXDeviceMaybe]
	mov rax, [rax]
	mov r8, [rax]
	mov [rsp+30h], ebp
	mov [rsp+28h], rbp
	mov rax, [rsi+10h]
	mov [rsp+20h], rax
	mov r9, rdi
	lea rdx, [rsp+88h]
	lea rcx, [rsp+78h]
	;mov rax, 14212B5E0h wrong!!
	mov rax, 14212B8C0h
	call rax
	test eax, eax
	je loc_14303F2BF
	;cmp [rsi+8], rbp
loc_14303F2B8:
	xor al, al
	jmp loc_14303F3AB
loc_14303F2BF:
	mov r8, [rsi+10h]
	mov edx, 10h
	lea ecx, [rdx+60h]
	;mov rax, 1421D71B0h lol
	mov rax, 141769A30h
	call rax
	mov [rsp+48h], rax
	test rax, rax
	je loc_14303F2F6
	mov rcx, rax
	call initGXTexture2D
	mov rdi, rax
	jmp loc_14303F2F9
loc_14303F2F6:
	mov rdi, rbp
loc_14303F2F9:
loc_14303F382:
	mov rax, [rdi]
	mov r8, [rsp+88h]
	mov rdx, [rsp+78h]
	mov rcx, rdi
	call qword ptr [rax+0A8h]
	mov [rsi+18h], rdi
	mov rsi, [rsp+78h]
	test rsi, rsi
	je setaltoone
	lea rcx, [rsi+10h]
	mov rbx, 141795640h
	call rbx
	cmp eax, 1
	jne setaltoone
	mov rbx, [rsi+8]
	mov rax, [rsi]
	xor edx, edx
	mov rcx, rsi
	call qword ptr [rax]
	mov rax, [rbx]
	mov rdx, rsi
	mov rcx, rbx
	call qword ptr [rax+68h]
setaltoone:
	mov al, 1
loc_14303F3AB:
	mov rbx, [rsp+80h]
	add rsp, 50h
	pop rdi
	pop rsi
	pop rbp
	ret
sub_14303F1A0 ENDP

.data
	cgFontDataString WORD 43h, 47h, 5Fh, 46h, 4Fh, 4Eh, 54h, 5Fh, 44h, 41h, 54h, 41h, 0

.code

sub_14246B840 PROC
	mov rax, rsp
	push rdi
	push r12
	push r13
	push r14
	push r15
	sub rsp, 50h
	mov qword ptr [rax-48h], -2
	mov [rax+8], rbx
	mov [rax+18h], rbp
	mov [rax+20h], rsi
	mov r14, r8
	mov rbx, rdx
	mov r13, rcx
	mov edi, [rdx+10h]
	lea ebp, [rdi*4]
	mov eax, edi	
	shl eax, 5
	add ebp, 90h
	add ebp, eax
	mov esi, ebp
	mov rax, [r14]
	mov edx, ebp
	mov rcx, r14
	call qword ptr [rax+48h] ;check this
	mov [rsp+88h], rax
	lea rcx, [rsp+88h]
	mov [rsp+38h], rcx
	mov [rsp+40h], r14
	test rax, rax
	je loc_14246BDC8
	mov r8, rsi
	xor edx, edx
	mov rcx, rax
	mov rax, 141FC3570h
	call rax
	xor esi, esi
	mov rax, [rsp+88h]
	mov [rax], esi
	mov rax, [rsp+88h]
	mov [rax+4], esi
	mov rax, [rsp+88h]
	mov [rax+8], edi
	movsx eax, word ptr [rbx+8]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	mov rax, [rsp+88h]
	movss dword ptr [rax+0Ch], xmm0
	movsx eax, word ptr [rbx+0Ah]
	movd xmm2, eax
	cvtdq2ps xmm2, xmm2
	movsx eax, word ptr [rbx+0Ch]
	movd xmm3, eax
	cvtdq2ps xmm3, xmm3
	mov r10d, [rbx+18h]
	add r10, rbx
	mov rbp, [rsp+88h]
	mov r8d, esi
	mov r9d, esi
	lea rdx, [rbp+10h]
loc_14246B9C2:
	mov eax, r8d
	lea rcx, [rax+rax*2]
	cmp r9d, [r10+rcx*8]
	jne loc_14246B9D7
	mov [rdx], r8b
	inc r8d
	jmp loc_14246B9DA
loc_14246B9D7:
	mov byte ptr [rdx], 0FFh
loc_14246B9DA:
	inc r9d
	inc rdx
	cmp r9d, 80h
	jb loc_14246B9C2
	lea r15, [rdi*4]
	mov r14d, esi
	mov r11, rsi
	cmp edi, 4
	jl loc_14246BCC9
	lea r12, [rbp+98h]
	lea rdx, [rbp+98h]
	add rdx, r15
	lea r8, [r10+0Ch]
	lea eax, [rdi-4]
	shr eax, 2
	inc eax
	mov r9d, eax
	lea r14d, [rax*4]
	lea r11, [rax*4]
loc_14246BA30:
	mov eax, [r8-0Ch]
	mov [r12-8], eax
	movsx eax, word ptr [r8-2]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx-8], xmm0
	movsx eax, word ptr [r8]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	movss dword ptr [rdx-4], xmm1
	movsx ecx, word ptr [r8+2]
	movsx eax, word ptr [r8]
	sub ecx, eax
	movd xmm0, ecx
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx], xmm0
	movzx eax, word ptr [r8-4]
	mov [rdx+4], eax
	mov ecx, [r8-8]
	add rcx, rbx
	movsx eax, word ptr [rcx]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+8], xmm0
	movsx eax, word ptr [rcx+2]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+0Ch], xmm1
	movsx eax, word ptr [rcx+4]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+10h], xmm0
	movsx eax, word ptr [rcx+6]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+14h], xmm1
	mov eax, [r8+0Ch]
	mov [r12-4], eax
	movsx eax, word ptr [r8+16h]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+18h], xmm0
	movsx eax, word ptr [r8+18h]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+1Ch], xmm0
	movsx ecx, word ptr [r8+1Ah]
	movsx eax, word ptr [r8+18h]
	sub ecx, eax
	movd xmm0, ecx
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+20h], xmm0
	movzx eax, word ptr [r8+14h]
	mov [rdx+24h], eax
	mov ecx, [r8+10h]
	add rcx, rbx
	movsx eax, word ptr [rcx]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+28h], xmm0
	movsx eax, word ptr [rcx+2]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+2Ch], xmm1
	movsx eax, word ptr [rcx+4]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+30h], xmm0
	movsx eax, word ptr [rcx+6]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+34h], xmm1
	mov eax, [r8+24h]
	mov [r12], eax
	movsx eax, word ptr [r8+2Eh]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+38h], xmm0
	movsx eax, word ptr [r8+30h]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+3Ch], xmm0
	movsx ecx, word ptr [r8+32h]
	movsx eax, word ptr [r8+30h]
	sub ecx, eax
	movd xmm0, ecx
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+40h], xmm0
	movzx eax, word ptr [r8+2Ch]
	mov [rdx+44h], eax
	mov ecx, [r8+28h]
	add rcx, rbx
	movsx eax, word ptr [rcx]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+48h], xmm0
	movsx eax, word ptr [rcx+2]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+4Ch], xmm1
	movsx eax, word ptr [rcx+4]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+50h], xmm0
	movsx eax, word ptr [rcx+6]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+54h], xmm1
	mov eax, [r8+3Ch]
	mov [r12+4], eax
	movsx eax, word ptr [r8+46h]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+58h], xmm0
	movsx eax, word ptr [r8+48h]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+5Ch], xmm0
	movsx ecx, word ptr [r8+4Ah]
	movsx eax, word ptr [r8+48h]
	sub ecx, eax
	movd xmm0, ecx
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx+60h], xmm0
	movzx eax, word ptr [r8+44h]
	mov [rdx+64h], eax
	mov ecx, [r8+40h]
	add rcx, rbx
	movsx eax, word ptr [rcx]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+68h], xmm0
	movsx eax, word ptr [rcx+2]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+6Ch], xmm1
	movsx eax, word ptr [rcx+4]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+70h], xmm0
	movsx eax, word ptr [rcx+6]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+74h], xmm1
	lea r12, [r12+10h]
	lea r8, [r8+60h]
	lea rdx, [rdx+80h]
	dec r9
	jne loc_14246BA30
loc_14246BCC9:
	cmp r14d, edi
	jnb loc_14246BDB4
	lea r9, [rbp+90h]
	lea r9, [r9+r11*4]
	mov rax, r11
	shl rax, 5
	lea rdx, [rbp+98h]
	add rdx, r15
	add rdx, rax
	lea rax, [r11+r11*2]
	lea r8, [r10+0Ch]
	lea r8, [r8+rax*8]
	sub edi, r14d
	mov r10d, edi
loc_14246BD03:
	mov eax, [r8-0Ch]
	mov [r9], eax
	movsx eax, word ptr [r8-2]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx-8], xmm0
	movsx eax, word ptr [r8]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	movss dword ptr[rdx-4], xmm1
	movsx eax, word ptr [r8]
	movsx ecx, word ptr [r8+2]
	sub ecx, eax
	movd xmm0, ecx
	cvtdq2ps xmm0, xmm0
	movss dword ptr [rdx], xmm0
	movzx eax, word ptr [r8-4]
	mov [rdx+4], eax
	mov ecx, [r8-8]
	add rcx, rbx
	movsx eax, word ptr [rcx]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+8], xmm0
	movsx eax, word ptr [rcx+2]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+0Ch], xmm1
	movsx eax, word ptr [rcx+4]
	movd xmm0, eax
	cvtdq2ps xmm0, xmm0
	divss xmm0, xmm2
	movss dword ptr [rdx+10h], xmm0
	movsx eax, word ptr [rcx+6]
	movd xmm1, eax
	cvtdq2ps xmm1, xmm1
	divss xmm1, xmm3
	movss dword ptr [rdx+14h], xmm1
	lea r9, [r9+4]
	lea r8, [r8+18h]
	lea rdx, [rdx+20h]
	dec r10
	jne loc_14246BD03
loc_14246BDB4:
	mov rcx, [rsp+88h]
	mov [r13], rcx
	mov [rsp+88h], rsi
loc_14246BDC8:
	mov eax, esi
	lea r11, [rsp+50h]
	mov rbx, [r11+30h]
	mov rbp, [r11+40h]
	mov rsi, [r11+48h]
	mov rsp, r11
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	ret
sub_14246B840 ENDP

.data

.code

sub_46EB10 PROC
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push r13
	push r12
	push rbx
	push rax
	mov r13, rsi
	mov rax, 555555555555554h
	mov r14, r8
	mov r12, rcx
	mov ebx, edx
	mov [rbp-30h], rdi
	cmp [r13+10h], rax
	jnb loc_46EE2E
	mov rdi, [r13+18h]
	mov esi, 30h
	mov edx, 8
	mov rax, [rdi]
	call qword ptr [rax+58h]
	mov r15, rax
	;test r15, r15
	;jmp loc_46EB8B
loc_46EB8B:
	mov rax, [r13+8]
	mov [r15], rax
	mov rax, [r13+8]
	mov [r15+8], rax
	mov rax, [r13+8]
	mov [r15+10h], rax
	mov word ptr [r15+18h], 0
	mov eax, [r14]
	mov [r15+20h], eax
	mov rax, [r14+8]
	mov [r15+28h], rax
	inc qword ptr [r13+10h]
	mov [r15+8], r12
	cmp [r13+8], r12
	je loc_46EBDC
	test bl, bl
	je loc_46EBEE
	mov [r12], r15
	mov rax, [r13+8]
	cmp [rax], r12
	jne loc_46EC01
	mov [rax], r15
	jmp loc_46EC01
loc_46EBDC:
	mov [r12+8], r15
	mov rax, [r13+8]
	mov [rax], r15
	mov rax, [r13+8]
	jmp loc_46EBFD
loc_46EBEE:
	mov [r12+10h], r15
	mov rax, [r13+8]
	cmp [rax+10h], r12
	jne loc_46EC01
loc_46EBFD:
	mov [rax+10h], r15
loc_46EC01:
	mov rax, [r15+8]
	cmp byte ptr [rax+18h], 0
	jne loc_46EE0B
	mov rdx, r15
	lea rdi, [rax+18h]
	mov rcx, r15
	add rdx, 8
loc_46EC20:
	mov rsi, [rax+8]
	mov rbx, [rsi]
	cmp rax, rbx
	je loc_46EC50
	cmp byte ptr [rbx+18h], 0
	je loc_46EC70
	cmp rcx, [rax]
	je loc_46EC8E
	mov rdx, [rcx+8]
	mov rax, rcx
	jmp loc_46ED19
loc_46EC50:
	mov rbx, [rsi+10h]
	cmp byte ptr [rbx+18h], 0
	je loc_46EC70
	cmp rcx, [rax+10h]
	je loc_46ECC4
	mov rdx, [rcx+8]
	mov rax, rcx
	jmp loc_46ED8F
loc_46EC70:
	mov byte ptr [rdi], 1
	mov byte ptr [rbx+18h], 1
	mov rax, [rdx]
	mov rax, [rax+8]
	mov byte ptr [rax+18h], 0
	mov rax, [rdx]
	mov rax, [rax+8]
	jmp loc_46EDF2
loc_46EC8E:
	mov rdx, [rcx+10h]
	mov [rax], rdx
	mov rdx, [rcx+10h]
	cmp byte ptr [rdx+19h], 0
	jne loc_46ECA7
	mov [rdx+8], rax
	mov rsi, [rax+8]
loc_46ECA7:
	mov [rcx+8], rsi
	mov rdx, [r13+8]
	cmp [rdx+8], rax
	je loc_46ED04
	mov rdx, [rax+8]
	cmp [rdx+10h], rax
	je loc_46ED0A
	mov [rdx], rcx
	jmp loc_46ED0E
loc_46ECC4:
	mov rdx, [rcx]
	mov [rax+10h], rdx
	mov rdx, [rcx]
	cmp byte ptr [rdx+19h], 0
	jne loc_46ECDC
	mov [rdx+8], rax
	mov rsi, [rax+8]
loc_46ECDC:
	mov [rcx+8], rsi
	mov rdx, [r13+8]
	cmp [rdx+8], rax
	je loc_46ED7C
	mov rdx, [rax+8]
	cmp [rdx], rax
	je loc_46ED82
	mov [rdx+10h], rcx
	jmp loc_46ED85
loc_46ED04:
	mov [rdx+8], rcx
	jmp loc_46ED0E
loc_46ED0A:
	mov [rdx+10h], rcx
loc_46ED0E:
	mov rdx, rcx
	mov [rcx+10h], rax
	mov [rax+8], rcx
loc_46ED19:
	mov byte ptr [rdx+18h], 1
	mov rcx, [rax+8]
	mov rcx, [rcx+8]
	mov byte ptr [rcx+18h], 0
	mov rcx, [rax+8]
	mov rcx, [rcx+8]
	mov rdx, [rcx+10h]
	mov rsi, [rdx]
	mov [rcx+10h], rsi
	mov rsi, [rdx]
	cmp byte ptr [rsi+19h], 0
	jne loc_46ED49
	mov [rsi+8], rcx
loc_46ED49:
	mov rsi, [rcx+8]
	mov [rdx+8], rsi
	mov rsi, [r13+8]
	cmp [rsi+8], rcx
	je loc_46ED6A
	mov rsi, [rcx+8]
	cmp [rsi], rcx
	je loc_46ED70
	mov [rsi+10h], rdx
	jmp loc_46ED73
loc_46ED6A:
	mov [rsi+8], rdx
	jmp loc_46ED73
loc_46ED70:
	mov [rsi], rdx
loc_46ED73:
	mov [rdx], rcx
	mov [rcx+8], rdx
	jmp loc_46EDF2
loc_46ED7C:
	mov [rdx+8], rcx
	jmp loc_46ED85
loc_46ED82:
	mov [rdx], rcx
loc_46ED85:
	mov rdx, rcx
	mov [rcx], rax
	mov [rax+8], rcx
loc_46ED8F:
	mov byte ptr [rdx+18h], 1
	mov rcx, [rax+8]
	mov rcx, [rcx+8]
	mov byte ptr [rcx+18h], 0
	mov rcx, [rax+8]
	mov rcx, [rcx+8]
	mov rdx, [rcx]
	mov rsi, [rdx+10h]
	mov [rcx], rsi
	mov rsi, [rdx+10h]
	cmp byte ptr [rsi+19h], 0
	jne loc_46EDBF
	mov [rsi+8], rcx
loc_46EDBF:
	mov rsi, [rcx+8]
	mov [rdx+8], rsi
	mov rsi, [r13+8]
	cmp [rsi+8], rcx
	je loc_46EDE0
	mov rsi, [rcx+8]
	cmp [rsi+10h], rcx
	je loc_46EDE6
	mov [rsi], rdx
	jmp loc_46EDEA
loc_46EDE0:
	mov [rsi+8], rdx
	jmp loc_46EDEA
loc_46EDE6:
	mov [rsi+10h], rdx
loc_46EDEA:
	mov [rdx+10h], rcx
	mov [rcx+8], rdx
loc_46EDF2:
	mov rcx, rax
	mov rax, [rcx+8]
	lea rdx, [rcx+8]
	cmp byte ptr [rax+18h], 0
	lea rdi, [rax+18h]
	je loc_46EC20
loc_46EE0B:
	mov rax, [r13+8]
	mov rax, [rax+8]
	mov byte ptr [rax+18h], 1
	mov rax, [rbp-30h]
	mov [rax+10h], r15
	add rsp, 8
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret
loc_46EE2E:
	nop
	ret
sub_46EB10 ENDP

.data

.code

;140308F80 needed !!!
sub_142F6BAD0 PROC
	push rdi
	sub rsp, 40h
	mov qword ptr [rsp+30h], -2
	mov [rsp+58h], rbx
	mov [rsp+60h], rsi
	mov rdi, r9
	movzx r10d, r8b
	mov rbx, rdx
	mov rsi, rcx
	mov r11, [rcx]
	mov rax, [r11+8]
	mov r9, r11
	mov r8b, 1
	cmp byte ptr [rax+19h], 0
	jne loc_142F6BB3C
	mov edx, [rdi]
loc_142F6BB10:
	mov r9, rax
	test r10b, r10b
	je loc_142F6BB21
	cmp [rax+20h], edx
	setnb r8b
	jmp loc_142F6BB28
loc_142F6BB21:
	cmp edx, [rax+20h]
	setb r8b
loc_142F6BB28:
	test r8b, r8b
	je loc_142F6BB32
	mov rax, [rax]
	jmp loc_142F6BB36
loc_142F6BB32:
	mov rax, [rax+10h]
loc_142F6BB36:
	cmp byte ptr [rax+19h], 0
	je loc_142F6BB10
loc_142F6BB3C:
	mov rax, r9
	test r8b, r8b
	je loc_142F6BBDA
	cmp r9, [r11]
	jne loc_142F6BB7A
	movzx eax, byte ptr [rsp+70h]
	mov [rsp+28h], al
	mov [rsp+20h], rdi
	mov r8b, 1
	lea rdx, [rsp+50h]
	call sub_142F6B800
	mov rax, [rax]
	mov [rbx], rax
	mov byte ptr [rbx+8], 1
	mov rax, rbx
	jmp loc_142F6BC15
loc_142F6BB7A:
	cmp byte ptr [r9+19h], 0
	je loc_142F6BB87
	mov rax, [r9+10h]
	jmp loc_142F6BBDA
loc_142F6BB87:
	mov rcx, [r9]
	cmp byte ptr [rcx+19h], 0
	jne loc_142F6BBAF
	mov rax, rcx
	cmp byte ptr [rcx+19h], 0
	jne loc_142F6BBDA
loc_142F6BBA0:
	mov rax, rcx
	mov rcx, [rcx+10h]
	cmp byte ptr [rcx+19h], 0
	je loc_142F6BBA0
	jmp loc_142F6BBDA
loc_142F6BBAF:
	mov rcx, [r9+8]
	cmp byte ptr [rcx+19h], 0
	jne loc_142F6BBD2
loc_142F6BBC0:
	cmp rax, [rcx]
	jne loc_142F6BBD2
	mov rax, rcx
	mov rcx, [rcx+8]
	cmp byte ptr [rcx+19h], 0
	je loc_142F6BBC0
loc_142F6BBD2:
	cmp byte ptr [rax+19h], 0
	cmove rax, rcx
loc_142F6BBDA:
	mov ecx, [rdi]
	cmp [rax+20h], ecx
	jnb loc_142F6BC0B
	movzx eax, byte ptr [rsp+70h]
	mov [rsp+28h], al
	mov [rsp+20h], rdi
	lea rdx, [rsp+50h]
	mov rcx, rsi
	call sub_142F6B800
	mov rax, [rax]
	mov [rbx], rax
	mov byte ptr [rbx+8], 1
	mov rax, rbx
	jmp loc_142F6BC15
loc_142F6BC0B:
	mov [rbx], rax
	mov byte ptr [rbx+8], 0
	mov rax, rbx
loc_142F6BC15:
	mov rbx, [rsp+58h]
	mov rsi, [rsp+60h]
	add rsp, 40h
	pop rdi
	ret
sub_142F6BAD0 ENDP
;
sub_142F6B800 PROC
	mov [rsp+8], rbx
	mov [rsp+10h], rsi
	mov [rsp+18h], rdi
	push r14
	sub rsp, 20h
	mov rax, 555555555555554h
	mov rdi, r9
	movzx esi, r8b
	mov r14, rdx
	mov rbx, rcx
	cmp [rcx+8], rax
	jnb loc_142F6BAB7
	mov rdx, [rsp+50h]
	call sub_142F6B730
	inc qword ptr [rbx+8]
	mov [rax+8], rdi
	mov rcx, [rbx]
	mov r9, rax
	cmp rdi, rcx
	jne loc_142F6B866
	mov [rcx+8], rax
	mov rcx, [rbx]
	mov [rcx], rax
	mov rcx, [rbx]
	mov [rcx+10h], rax
	jmp loc_142F6B88C
loc_142F6B866:
	test sil, sil
	je loc_142F6B87B
	mov [rdi], rax
	mov rax, [rbx]
	cmp rdi, [rax]
	jne loc_142F6B88C
	mov [rax], r9
	jmp loc_142F6B88C
loc_142F6B87B:
	mov [rdi+10h], rax
	mov rax, [rbx]
	cmp rdi, [rax+10h]
	mov [rax+10h], r9
loc_142F6B88C:
	mov rcx, [r9+8]
	mov rax, r9
	cmp byte ptr [rcx+18h], 0
	jne loc_142F6BA90
loc_142F6B8A0:
	mov rcx, [rax+8]
	mov r8, [rcx+8]
	mov rdx, [r8]
	cmp rcx, rdx
	jne loc_142F6B9A5
	mov rdx, [r8+10h]
	cmp byte ptr [rdx+18h], 0
	jne loc_142F6B8DF
	mov byte ptr [rcx+18h], 1
	mov byte ptr [rdx+18h], 1
	mov rcx, [rax+8]
	mov rdx, [rcx+8]
	mov byte ptr [rdx+18h], 0
	mov rcx, [rax+8]
	mov rax, [rcx+8]
	jmp loc_142F6BA82
loc_142F6B8DF:
	cmp rax, [rcx+10h]
	jne loc_142F6B930
	mov rdx, [rcx+10h]
	mov rax, rcx
	mov rcx, [rdx]
	mov [rax+10h], rcx
	mov rcx, [rdx]
	cmp byte ptr [rcx+19h], 0
	jne loc_142F6B900
	mov [rcx+8], rax
loc_142F6B900:
	mov rcx, [rax+8]
	mov [rdx+8], rcx
	mov rcx, [rbx]
	cmp rax, [rcx+8]
	jne loc_142F6B917
	mov [rcx+8], rdx
	jmp loc_142F6B929
loc_142F6B917:
	mov rcx, [rax+8]
	cmp rax, [rcx]
	jne loc_142F6B925
	mov [rcx], rdx
	jmp loc_142F6B929
loc_142F6B925:
	mov [rcx+10h], rdx
loc_142F6B929:
	mov [rdx], rax
	mov [rax+8], rdx
loc_142F6B930:
	mov rcx, [rax+8]
	mov byte ptr [rcx+18h], 1
	mov rcx, [rax+8]
	mov rdx, [rcx+8]
	mov byte ptr [rdx+18h], 0
	mov rcx, [rax+8]
	mov rdx, [rcx+8]
	mov r8, [rdx]
	mov rcx, [r8+10h]
	mov [rdx], rcx
	mov rcx, [r8+10h]
	cmp byte ptr [rcx+19h], 0
	jne loc_142F6B964
	mov [rcx+8], rdx
loc_142F6B964:
	mov rcx, [rdx+8]
	mov [r8+8], rcx
	mov rcx, [rbx]
	cmp rdx, [rcx+8]
	jne loc_142F6B982
	mov [rcx+8], r8
	mov [r8+10h], rdx
	jmp loc_142F6BA7E
loc_142F6B982:
	mov rcx, [rdx+8]
	cmp rdx, [rcx+10h]
	jne loc_142F6B999
	mov [rcx+10h], r8
	mov [r8+10h], rdx
	jmp loc_142F6BA7E
loc_142F6B999:
	mov [rcx], r8
	mov [r8+10h], rdx
	jmp loc_142F6BA7E
loc_142F6B9A5:
	cmp byte ptr [rdx+18h], 0
	jne loc_142F6B9CC
	mov byte ptr [rcx+18h], 1
	mov byte ptr [rdx+18h], 1
	mov rcx, [rax+8]
	mov rdx, [rcx+8]
	mov byte ptr [rdx+18h], 0
	mov rcx, [rax+8]
	mov rax, [rcx+8]
	jmp loc_142F6BA82
loc_142F6B9CC:
	cmp rax, [rcx]
	jne loc_142F6BA1E
	mov rdx, [rcx]
	mov rax, rcx
	mov rcx, [rdx+10h]
	mov [rax], rcx
	mov rcx, [rdx+10h]
	cmp byte ptr [rcx+19h], 0
	jne loc_142F6B9EC
	mov [rcx+8], rax
loc_142F6B9EC:
	mov rcx, [rax+8]
	mov [rdx+8], rcx
	mov rcx, [rbx]
	cmp rax, [rcx+8]
	jne loc_142F6BA03
	mov [rcx+8], rdx
	jmp loc_142F6BA16
loc_142F6BA03:
	mov rcx, [rax+8]
	cmp rax, [rcx+10h]
	jne loc_142F6BA13
	mov [rcx+10h], rdx
	jmp loc_142F6BA16
loc_142F6BA13:
	mov [rcx], rdx
loc_142F6BA16:
	mov [rdx+10h], rax
	mov [rax+8], rdx
loc_142F6BA1E:
	mov rcx, [rax+8]
	mov byte ptr [rcx+18h], 1
	mov rcx, [rax+8]
	mov rdx, [rcx+8]
	mov byte ptr [rdx+18h], 0
	mov rcx, [rax+8]
	mov rdx, [rcx+8]
	mov r8, [rdx+10h]
	mov rcx, [r8]
	mov [rdx+10h], rcx
	mov rcx, [r8]
	cmp byte ptr [rcx+19h], 0
	jne loc_142F6BA52
	mov [rcx+8], rdx
loc_142F6BA52:
	mov rcx, [rdx+8]
	mov [r8+8], rcx
	mov rcx, [rbx]
	cmp rdx, [rcx+8]
	jne loc_142F6BA69
	mov [rcx+8], r8
	jmp loc_142F6BA7B
loc_142F6BA69:
	mov rcx, [rdx+8]
	cmp rdx, [rcx]
	jne loc_142F6BA77
	mov [rcx], r8
	jmp loc_142F6BA7B
loc_142F6BA77:
	mov [rcx+10h], r8
loc_142F6BA7B:
	mov [r8], rdx
loc_142F6BA7E:
	mov [rdx+8], r8
loc_142F6BA82:
	mov rcx, [rax+8]
	cmp byte ptr [rcx+18h], 0
	je loc_142F6B8A0
loc_142F6BA90:
	mov rax, [rbx]
	mov rsi, [rsp+38h]
	mov rdi, [rsp+40h]
	mov rcx, [rax+8]
	mov rbx, [rsp+30h]
	mov [r14], r9
	mov byte ptr [rcx+18h], 1
	mov rax, r14
	add rsp, 20h
	pop r14
	ret
loc_142F6BAB7:
	int 3
sub_142F6B800 ENDP

.data
	
.code

sub_142F6B730 PROC
	mov [rsp+8], rcx
	push rbx
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov rbx, rdx
	call sub_142F6C440
	mov [rsp+50h], rax
	mov word ptr [rax+18h], 0
	lea r8, [rax+20h]
	test r8, r8
	je loc_142F6B76C
	mov ecx, [rbx]
	mov [r8], ecx
	mov rcx, [rbx+8]
	mov [r8+8], rcx
loc_142F6B76C:
	add rsp, 30h
	pop rbx
	ret
sub_142F6B730 ENDP

sub_142F6C440 PROC
	mov [rsp+8], rcx
	push rbx
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov rbx, rcx
	add rcx, 10h
	;mov edx, 1
	mov rdx, 140308FD0h
	call rdx
	mov rdx, rax
	mov [rsp+48h], rax
	test rax, rax
	je loc_142F6C477
	mov rcx, [rbx]
	mov [rax], rcx
loc_142F6C477:
	lea rcx, [rax+8]
	test rcx, rcx
	je loc_142F6C486
	mov rax, [rbx]
	mov [rcx], rax
loc_142F6C486:
	lea rcx, [rdx+10h]
	test rcx, rcx
	je loc_142F6C495
	mov rax, [rbx]
	mov [rcx], rax
loc_142F6C495:
	mov rax, rdx
	add rsp, 30h
	pop rbx
	ret
sub_142F6C440 ENDP

.data
	pSprjFile QWORD 14473E888h
	pSprjFlipper QWORD 144888440h
	fpsString WORD 46h, 50h, 53h, 3Ah, 25h, 34h, 2Eh, 31h, 66h, 0
	loadingString WORD 25A0h, 4Ch, 6Fh, 61h, 64h, 69h, 6Eh, 67h, 5Bh, 25h, 64h, 3Ah, 25h, 64h, 5Dh, 0 
	sthFormatString QWORD 14001B4B0h
	pRendMan QWORD 1447809C8h

.code

fDbgDispLoading PROC
	mov rax, rsp
	push rbp
	lea rbp, [rax-18h]
	sub rsp, 110h
	mov qword ptr [rsp+40h], -2
	mov [rax+8], rbx
	mov [rax+10h], rsi
	mov [rax+18h], rdi
	mov [rax+20h], r14
	movaps xmmword ptr [rax-18h], xmm6
	movaps xmmword ptr [rax-28h], xmm7
	movaps xmmword ptr [rax-38h], xmm8
	movaps xmmword ptr [rax-48h], xmm9
	movaps xmmword ptr [rax-58h], xmm10
	movaps xmmword ptr [rax-68h], xmm11
	movaps xmmword ptr [rax-78h], xmm12
	mov rax, [pGXDeviceMaybe]
	mov rax, [rax]
	mov ecx, [rax+120h]
	xorps xmm12, xmm12
	cvtsi2ss xmm12, rcx
	mov rax, [pSprjFlipper]
	mov rax, [rax]
	xor r14d, r14d
	mov rdx, 143D6ADB0h
	movss xmm6, dword ptr [rdx]
	mov rdx, 142825C14h
	movss xmm11, dword ptr [rdx]
	mov rdx, 142825C20h
	movss xmm7, dword ptr [rdx]
	cmp [rax+362h], r14b	;display fps
	je loc_140B84441
	mov rdx, 14176C360h
	call rdx
	mov rbx, rax
	mov rax, [rax]
	xor r8d, r8d
	lea rdx, [rsp+20h]
	mov rcx, rbx
	call qword ptr [rax+18h]
	mov ecx, [rax]
	shr ecx, 5
	test cl, 1
	jne loc_140B842AA
	mov r8, 1439E1800h
	lea edx, [r14+3Eh]
	xor ecx, ecx
	mov rax, 141769E10h
	call rax
loc_140B842AA:
	mov [rsp+58h], r14
	mov [rsp+60h], r14
	mov [rsp+68h], rbx
	mov qword ptr [rsp+60h], 7
	mov [rsp+58h], r14
	mov word ptr [rsp+48h], r14w
	mov byte ptr [rsp+70h], 1
	mov rax, [pSprjFlipper]
	mov rax, [rax]
	movss xmm2, dword ptr [rax+2BCh]
	cvtps2pd xmm2, xmm2
	movq r8, xmm2
	lea rdx, [fpsString]
	lea rcx, [rsp+48h]
	call sthFormatString
	mov rbx, [pRendMan]
	mov rbx, [rbx]
	mov rbx, [rbx+38h]
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	mov rcx, [rcx+40h]
	mov rax, [rcx]
	or edx, -1
	call qword ptr [rax+20h]
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	mov rax, [rcx+40h]
	cmp dword ptr [rax+70h], 0
	je loc_140B8438F
	mov [rax+70h], r14d
	or dword ptr [rax+8], 80h
loc_140B8438F:
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	movaps xmm2, xmm6
	movaps xmm1, xmm6
	mov rcx, [rcx+40h]
	mov rax, 14030DBE0h
	call rax
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	mov rax, [rcx+40h]
	movss xmm0, dword ptr [rax+78h]
	ucomiss xmm0, xmm11
	jp loc_140B843C1
	je loc_140B843CF
loc_140B843C1:
	mov dword ptr [rax+78h], 41E00000h
	or dword ptr [rax+8], 100h
loc_140B843CF:
	lea r9, [rsp+48h]
	cmp qword ptr [rsp+60h], 8
	cmovnb r9, [rsp+48h]
	movaps xmm1, xmm12
	mov rcx, 143D6AF98h
	subss xmm1, dword ptr [rcx]
	mov rcx, 1445B6460h
	movss xmm2, dword ptr [rcx]
	mov rcx, rbx
	call sub_140031188
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	movaps xmm2, xmm7
	movaps xmm1, xmm7
	mov rcx, [rcx+40h]
	mov rax, 14030DBE0h
	call rax
	cmp qword ptr [rsp+60h], 8
	jb loc_140B8442D
	mov rcx, [rsp+68h]
	mov rax, [rcx]
	mov rdx, [rsp+48h]
	call qword ptr [rax+68h]
loc_140B8442D:
	mov qword ptr [rsp+60h], 7
	mov [rsp+58h], r14
	mov word ptr [rsp+48h], r14w
loc_140B84441:
	mov rcx, [pSprjFile]
	mov rcx, [rcx]
	cmp byte ptr [rcx+98h], 0
	jne settoone
	cmp byte ptr [rcx+99h], 0
	jne settoone
	xor al, al
	jmp pastcheck
settoone:
	mov al, 1
pastcheck:
	test al, al
	je loc_140B846B0
	mov edi, r14d
	mov rax, 143D6AFB8h
	movss xmm8, dword ptr [rax]
	mov rax, 143D6AF08h
	movss xmm9, dword ptr [rax]
	mov rax, 143D6AF6Ch
	movss xmm10, dword ptr [rax]
loc_140B844A0:
	mov rcx, [pSprjFile]
	mov rcx, [rcx]
	mov edx, edi
	call sub_142F340E0
	mov esi, eax
	test eax, eax
	jle loc_140B846A5
	mov rdx, 14176C360h
	call rdx
	mov rbx, rax
	mov r9, [rax]
	xor r8d, r8d
	lea rdx, [rsp+20h]
	mov rcx, rax
	call qword ptr [r9+18h]
	mov ecx, [rax]
	shr ecx, 5
	test cl, 1
	jne loc_140B8451A
	mov r8, 1439E1800h
	lea edx, [r14+3Eh]
	xor ecx, ecx
	mov rax, 141769E10h
	call rax
loc_140B8451A:
	mov [rsp+58h], r14
	mov [rsp+60h], r14
	mov [rsp+68h], rbx
	mov qword ptr [rsp+60h], 7
	mov [rsp+58h], r14
	mov word ptr [rsp+48h], r14w
	mov byte ptr [rsp+70h], 1
	mov r9d, esi
	mov r8d, edi
	lea rdx, [loadingString]
	lea rcx, [rsp+48h]
	call sthFormatString
	mov rbx, [pRendMan]
	mov rbx, [rbx]
	mov rbx, [rbx+38h]
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	mov rcx, [rcx+40h]
	mov rax, [rcx]
	or edx, -1
	call qword ptr [rax+20h]
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	mov rax, [rcx+40h]
	cmp dword ptr [rax+70h], 0
	je loc_140B845C3
	mov [rax+70h], r14d
	or dword ptr [rax+8], 80h
loc_140B845C3:
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	movaps xmm2, xmm6
	movaps xmm1, xmm6
	mov rcx, [rcx+40h]
	mov rax, 14030DBE0h
	call rax
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	mov rax, [rcx+40h]
	movss xmm0, dword ptr [rax+78h]
	ucomiss xmm0, xmm11
	jp loc_140B845F5
	je loc_140B84603
loc_140B845F5:
	mov dword ptr [rax+78h], 41E00000h
	or dword ptr [rax+8], 100h
loc_140B84603:
	lea r8, [rsp+48h]
	cmp qword ptr [rsp+60h], 8
	cmovnb r8, [rsp+48h]
	movaps xmm0, xmm12
	subss xmm0, xmm8
	movss dword ptr [rsp+30h], xmm0
	movd xmm1, edi
	cvtdq2ps xmm1, xmm1
	mulss xmm1, xmm9
	addss xmm1, xmm10
	movss dword ptr [rsp+34h], xmm1
	mov dword ptr [rsp+38h], 0
	mov dword ptr [rsp+3Ch], 3F800000h
	movaps xmm0, [rsp+30h]
	movdqa [rbp-80h], xmm0
	lea rdx, [rbp-80h]
	mov rcx, rbx
	mov rax, 142321330h
	call rax
	movsxd rax, dword ptr [rbx+20h]
	mov rcx, [rbx+rax*8+10h]
	movaps xmm2, xmm7
	movaps xmm1, xmm7
	mov rcx, [rcx+40h]
	mov rax, 14030DBE0h
	call rax
	cmp qword ptr [rsp+60h], 8
	jb loc_140B84691
	mov rcx, [rsp+68h]
	mov rax, [rcx]
	mov rdx, [rsp+48h]
	call qword ptr [rax+68h]
loc_140B84691:
	mov qword ptr [rsp+60h], 7
	mov [rsp+58h], r14
	mov word ptr [rsp+48h], r14w
loc_140B846A5:
	inc edi
	cmp edi, 5
	jb loc_140B844A0
loc_140B846B0:
	lea r11, [rsp+110h]
	mov rbx, [r11+10h]
	mov rsi, [r11+18h]
	mov rdi, [r11+20h]
	mov r14, [r11+28h]
	movaps xmm6, xmmword ptr [r11-10h]
	movaps xmm7, xmmword ptr [r11-20h]
	movaps xmm8, xmmword ptr [r11-30h]
	movaps xmm9, xmmword ptr [r11-40h]
	movaps xmm10, xmmword ptr [r11-50h]
	movaps xmm11, xmmword ptr [r11-60h]
	movaps xmm12, xmmword ptr [r11-70h]
	mov rsp, r11
	pop rbp
	ret
fDbgDispLoading ENDP

sub_140031188 PROC
	sub rsp, 48h
	movss dword ptr [rsp+20h], xmm1
	mov rax, 142825C20h
	movss xmm1, dword ptr [rax]
	xorps xmm0, xmm0
	lea rdx, [rsp+20h]
	mov r8, r9
	movss dword ptr [rsp+2Ch], xmm1
	movss dword ptr [rsp+24h], xmm2
	movss dword ptr [rsp+28h], xmm0
	movaps xmm0, [rsp+20h]
	movdqa [rsp+20h], xmm0
	mov rax, 142321330h
	call rax
	add rsp, 48h
	ret
sub_140031188 ENDP

sub_142F340E0 PROC
	mov rcx, [rcx+8]
	jmp sub_142F52D00
sub_142F340E0 ENDP

sub_142F52D00 PROC
	mov eax, edx
	mov r8, [rcx+rax*8+0C8h]
	mov edx, [r8+38h]
	mov ecx, [r8+3Ch]
	cmp edx, ecx
	jg loc_142F52D1B
	sub ecx, edx
	mov eax, ecx
	ret
loc_142F52D1B:
	mov eax, [r8+40h]
	sub eax, edx
	add eax, ecx
	ret
sub_142F52D00 ENDP

.data
	windowCounter DWORD 0
	pMenuMan QWORD 14474C2E8h
.code

incWindowCounter PROC
	mov eax, [windowCounter]
	test eax, eax
	jne cursorEnabled
	mov r14, [pMenuMan]
	mov r14, [r14]
	mov byte ptr [r14+54h], 1
cursorEnabled:
	inc [windowCounter]
	sub rsp, 80h
	jmp [bIncWindowCounter]
incWindowCounter ENDP

decWindowCounter PROC
	mov eax, [windowCounter]
	test eax, eax
	je epilogue
	dec eax
	jne epilogue
cursorDisabled:
	mov rcx, [pMenuMan]
	mov rcx, [rcx]
	mov byte ptr [rcx+54h], 0
epilogue:
	mov [windowCounter], eax
	mov rax, [rdi]
	mov rcx, rdi
	jmp [bDecWindowCounter]
decWindowCounter ENDP

END