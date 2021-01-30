EXTERN bInitDebugBootMenuStep:QWORD
EXTERN bInitMoveMapListStep:QWORD
EXTERN ezSelectBotDebugBootMenuString:QWORD
EXTERN debugBootMenuStringModelViewer:QWORD
EXTERN debugBootMenuStringSoundTest:QWORD
EXTERN debugBootMenuStringMapViewer:QWORD
EXTERN debugBootMenuStringStageSelect:QWORD
EXTERN debugBootMenuStringAutoTest:QWORD
EXTERN debugBootMenuStringTitleBoot:QWORD
EXTERN debugBootMenuStringE3TitleBoot:QWORD
EXTERN debugBootMenuStringGamescomTitleBoot:QWORD
EXTERN debugBootMenuStringLoadTest:QWORD
EXTERN debugBoogMenuStringDebugMoveMapTest:QWORD
EXTERN debugBootMenuStringMoveMapTest:QWORD
EXTERN debugBootMenuStringMoveMapTestCeremony:QWORD
EXTERN debugBootMenuStringModuleTest:QWORD
EXTERN debugBootMenuStringMenuViewer:QWORD
EXTERN debugBootMenuStringSfxViewer:QWORD
EXTERN debugBootMenuString:QWORD
EXTERN debugBootMenuTextlistPath:QWORD
EXTERN debugBootMenuStepVtable:QWORD
EXTERN EzTextlistSelectorVtable:QWORD
EXTERN MoveMapListStepVtable:QWORD

.data
	gotAnymoreOfThemMemory QWORD 141769A30h
	pMainMem QWORD 144786988h
	pGameStepId QWORD 14476A994h
	initTaskItemAdapter QWORD 1408FF5E0h
.code

tGameStepSelection PROC
	mov r8, [pGameStepId]
	mov r8d, [r8]
	test r8d, r8d
	je moveMapListStep
	dec r8d
	je normalTitleStep
	dec r8d
	je trialTitleStep
	dec r8d
	jne finishStep
E3TitleStep:
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rsp+50h], r8
	mov edx, 8
	lea ecx, [rdx+48h]
	call gotAnymoreOfThemMemory
	mov rbx, rax
	mov [rsp+68h], rax
	test rax, rax
	je E3TitleStepEnd
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rsp+28h], r8
	mov edx, 8
	mov ecx, 88h
	call gotAnymoreOfThemMemory
	mov [rsp+30h], rax
	test rax, rax
	je E3TitleStepEnd
	mov rcx, rax
	;initE3TitleStep
	jmp finishStep
	xor r9d, r9d
	mov r8, rax
	lea edx, [r9+18h]
	mov rcx, rbx
	call initTaskItemAdapter
E3TitleStepEnd:
	jmp finishStep
moveMapListStep:
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rsp+50h], r8
	mov edx, 8
	lea ecx, [rdx+48h]
	call gotAnymoreOfThemMemory
	mov rbx, rax
	mov [rsp+68h], rax
	test rax, rax
	je moveMapListStepEnd
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rsp+28h], r8
	mov edx, 8
	mov ecx, 160h
	call gotAnymoreOfThemMemory
	mov [rsp+30h], rax
	test rax, rax
	je moveMapListStepEnd
	mov rcx, rax
	call initMoveMapListStep
	xor r9d, r9d
	mov r8, rax
	lea edx, [r9+18h]
	mov rcx, rbx
	call initTaskItemAdapter
moveMapListStepEnd:
	jmp finishStep
normalTitleStep:
	mov r8, [pMainMem]
	mov r8, [r8]
	mov rax, 1408FDC1Ah
	jmp rax
trialTitleStep:
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rsp+50h], r8
	mov edx, 8
	lea ecx, [rdx+48h]
	call gotAnymoreOfThemMemory
	mov rbx, rax
	mov [rsp+68h], rax
	test rax, rax
	je trialTitleStepEnd
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rsp+28h], r8
	mov edx, 8
	mov ecx, 88h
	call gotAnymoreOfThemMemory
	mov [rsp+30h], rax
	test rax, rax
	je trialTitleStepEnd
	mov rcx, rax
	;call initTrialTitleStep
	jmp finishStep
	xor r9d, r9d
	mov r8, rax
	lea edx, [r9+18h]
	mov rcx, rbx
	call initTaskItemAdapter
trialTitleStepEnd:
	jmp finishStep
finishStep:
	mov [rsi+50h], rax
	inc dword ptr [rax+14h]
	mov rax, 140E41E60h
	call rax
	inc dword ptr [rsi+10h]
	mov rax, 1408FDC89h
	jmp rax
tGameStepSelection ENDP

.data
	free QWORD 141FC2E38h
	pGameDataMan QWORD 144740178h
.code

MoveMapListStepDtor PROC
	mov [rsp+8], rbx
	push rdi
	sub rsp, 20h
	mov ebx, edx
	mov rdi, rcx
	call MoveMapListStepDtorNext
	test bl, 1
	je nofree
	mov rcx, rdi
	call free
nofree:
	mov rax, rdi
	mov rbx, [rsp+30h]
	add rsp, 20h
	pop rdi
	ret
MoveMapListStepDtor ENDP

MoveMapListStepDtorNext PROC
	mov [rsp+8], rcx
	push rdi
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov [rsp+50h], rbx
	mov rbx, rcx
	mov rax, [MoveMapListStepVtable]
	mov [rcx], rax
	mov rax, [pGameDataMan]
	mov rax, [rax]
	mov rdi, [rax+10h]
	test rdi, rdi
	je loc_140B8A88C
	lea rcx, [rsp+48h]
	mov dword ptr [rcx], 0
	mov rax, rcx
	mov edx, [rax]
	mov rcx, rdi
	mov rax, 1405A5DD0h
	call rax
	lea rdx, [rsp+48h]
	mov rcx, rdi
	mov rax, 1405A4740h
	call rax
loc_140B8A88C:
	mov rcx, [rbx+50h]
	mov rax, 1408E04E0h
	call rax
	mov [rbx+50h], rax
	lea rcx, [rbx+58h]
	mov rax, 140DD7C10h
	call rax
	lea rcx, [rbx+28h]
	mov rax, 142332BA0h
	call rax
	mov rax, 142849E98h
	mov [rbx], rax
	mov rcx, rbx
	mov rbx, [rsp+50h]
	add rsp, 30h
	pop rdi
	mov rax, 1408FEF30h
	jmp rax
MoveMapListStepDtorNext ENDP

.data
	pSprjFile QWORD 14473E888h
.code

EzTextlistSelectorDtor PROC
	mov [rsp+8], rbx
	push rdi
	sub rsp, 20h
	mov ebx, edx
	mov rdi, rcx
	call EzTextlistSelectorDtorNext
	test bl, 1
	je nofree
	mov rcx, rdi
	call free
nofree:
	mov rax, rdi
	mov rbx, [rsp+30h]
	add rsp, 20h
	pop rdi
	ret
EzTextlistSelectorDtor ENDP

EzTextlistSelectorDtorNext PROC
	mov [rsp+8], rcx
	push rdi
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov [rsp+50h], rbx
	mov [rsp+58h], rsi
	mov rbx, rcx
	mov rax, [EzTextlistSelectorVtable]
	mov [rcx], rax
	xor esi, esi
	cmp [rcx+0F8h], rsi
	je loc_140CF2D8B
	mov rcx, [pSprjFile]
	mov rcx, [rcx]
	mov rax, [rcx]
	mov rdx, [rbx+0F8h]
	call qword ptr [rax+30h]
	mov [rbx+0F8h], rsi
loc_140CF2D8B:
	lea rdi, [rbx+100h]
	mov [rsp+48h], rdi
	cmp qword ptr [rdi+18h], 8
	jb loc_140CF2DAB
	mov rcx, [rdi+20h]
	mov rax, [rcx]
	mov rdx, [rdi]
	call qword ptr [rax+68h]
loc_140CF2DAB:
	mov qword ptr [rdi+18h], 7
	mov [rdi+10h], rsi
	mov [rdi], si
	lea rdi, [rbx+0B8h]
	mov [rsp+48h], rdi
	cmp qword ptr [rdi+18h], 8
	jb loc_140CF2DDA
	mov rcx, [rdi+20h]
	mov rax, [rcx]
	mov rdx, [rdi]
	call qword ptr [rax+68h]
loc_140CF2DDA:
	mov qword ptr [rdi+18h], 7
	mov [rdi+10h], rsi
	mov [rdi], si
	mov rax, 142894958h
	mov [rbx], rax
	mov rcx, rbx
	mov rbx, [rsp+50h]
	mov rsi, [rsp+58h]
	add rsp, 30h
	pop rdi
	mov rax, 140A08E00h
	jmp rax
EzTextlistSelectorDtorNext ENDP

.data

.code

tInitMoveMapListStep PROC
	call initMoveMapListStep
	jmp [bInitMoveMapListStep]
tInitMoveMapListStep ENDP

.data
	initTask QWORD 1408FEF10h
	initCSNetworkFlowStep QWORD 140DD7A50h
.code

initMoveMapListStep PROC
	mov [rsp+8], rcx
	push rdi
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov [rsp+48h], rbx
	mov rbx, rcx
	call initTask
	mov rax, 142849E98h
	mov [rbx], rax
	xor edi, edi
	mov [rbx+10h], rdi
	mov [rbx+18h], edi
	mov [rbx+1Ch], dil
	mov rax, 1445881F0h
	mov [rbx+20h], rax
	mov rax, [MoveMapListStepVtable]
	mov [rbx], rax
	mov [rbx+28h], rdi
	mov [rbx+30h], rdi
	mov [rbx+38h], rdi
	mov [rbx+40h], rdi
	mov r8d, 0
	mov eax, 66666667h
	imul r8d
	sar edx, 2
	mov ecx, edx
	shr ecx, 1Fh
	add edx, ecx
	lea ecx, [rdx+rdx*4]
	add ecx, ecx
	sub r8d, ecx
	mov [rbx+48h], r8d
	mov ecx, 0
	mov eax, 66666667h
	imul ecx
	sar edx, 2
	mov eax, edx
	shr eax, 1Fh
	add edx, eax
	mov [rbx+4Ch], edx
	mov [rbx+50h], rdi
	lea rcx, [rbx+58h]
	call initCSNetworkFlowStep
	;mov dword ptr [rbx+148h], -1
	;mov byte ptr [rbx+14Ch], 1
	mov [rbx+148h], rdi ;what is this?
	mov dword ptr [rbx+150h], -1
	mov byte ptr [rbx+154h], 1
	mov rcx, [rbx+50h]
	mov rax, 1408E0450h
	call rax
	mov [rbx+50h], rax
	mov rax, rbx
	mov rbx, [rsp+48h]
	add rsp, 30h
	pop rdi
	ret
initMoveMapListStep ENDP

.data
	memset QWORD 141FC3570h
.code

initDebugBootMenuStepFunctions PROC
	sub rsp, 28h
	xor edx, edx
	mov rcx, 14476B2C0h
	lea r8d, [rdx+60h]
	call memset
	mov rax, 140935A00h
	mov rcx, 14476B2C0h
	mov [rcx], rax
	mov rax, 142855418h ;DebugBootMenuStep::STEP_Init
	add rcx, 8
	mov [rcx], rax
	lea rax, [DebugBootMenuStep_STEP_Update_forList]
	add rcx, 8
	mov [rcx], rax
	mov rax, 142855460h ;DebugBootMenuStep::STEP_Update_forList
	add rcx, 8
	mov [rcx], rax
	lea rax, [DebugBootMenuStep_STEP_Wait_forChild]
	add rcx, 8
	mov [rcx], rax
	mov rax, 1428554B0h ;DebugBootMenuStep::STEP_Wait_forChild
	add rcx, 8
	mov [rcx], rax
	lea rax, [DebugBootMenuStep_STEP_Finish_forChild]
	add rcx, 8
	mov [rcx], rax
	mov rax, 142855500h ;DebugBootMenuStep::STEP_Finish_forChild
	add rcx, 8
	mov [rcx], rax
	mov rax, 14091D9D0h
	add rcx, 8
	mov [rcx], rax
	mov rax, 142855550h ;DebugBootMenuStep::STEP_Finish
	add rcx, 8
	mov [rcx], rax
	add rsp, 28h
	ret
initDebugBootMenuStepFunctions ENDP

.data

.code

tInitDebugBootMenuStep PROC
	call initDebugBootMenuStep
	jmp [bInitDebugBootMenuStep]
tInitDebugBootMenuStep ENDP

.data
;140935A00 - DebugBootMenuStep::STEP_Init
;14091D9D0 - DebugBootMenuStep::STEP_Finish
	
	initEzChildStepBase QWORD 140F00A70h
.code

initDebugBootMenuStep PROC
	mov [rsp+8], rcx
	push rdi
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov [rsp+50h], rbx
	mov rdi, rcx
	mov rdx, 14476B2C0h
	mov rax, 14091BBF0h
	call rax
	mov rax, [debugBootMenuStepVtable]
	mov [rdi], rax
	lea rcx, [rdi+0C8h]
	lea r8, [debugBootMenuString]
	lea rdx, [debugBootMenuTextlistPath]
	call initEzTextlistSelector
	lea rbx, [rdi+218h]
	mov [rsp+48h], rbx
	mov rcx, rbx
	call initEzChildStepBase
	mov rax, 142855410h
	mov [rbx], rax
	mov rax, rdi
	mov rbx, [rsp+50h]
	add rsp, 30h
	pop rdi
	ret
initDebugBootMenuStep ENDP

.data

.code

debugBootMenuStepDtor PROC
	mov [rsp+8], rbx
	push rdi
	sub rsp, 20h
	mov ebx, edx
	mov rdi, rcx
	call debugBootMenuStepDtorNext
	test bl, 1
	je nofree
	mov rcx, rdi
	call free
nofree:
	mov rax, rdi
	mov rbx, [rsp+30h]
	add rsp, 20h
	pop rdi
	ret
debugBootMenuStepDtor ENDP

debugBootMenuStepDtorNext PROC
	mov [rsp+8], rcx
	push rbx
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov rbx, rcx
	mov rax, [debugBootMenuStepVtable]
	mov [rcx], rax
	add rcx, 218h
	mov [rsp+48h], rcx
	mov rax, 142855410h
	mov [rcx], rax
	mov rax, 140F00A90h
	call rax
	lea rcx, [rbx+0C8h]
	call EzTextlistSelectorDtorNext
	mov rax, 14285F098h
	mov [rbx], rax
	mov rax, 14285F008h
	mov [rbx], rax
	mov rcx, rbx
	add rsp, 30h
	pop rbx
	mov rax, 14091BE90h
	jmp rax
debugBootMenuStepDtorNext ENDP

.data

.code

initEzTextlistSelector PROC
	mov [rsp+8], rcx
	push rdi
	push r14
	push r15
	sub rsp, 30h
	mov qword ptr [rsp+20h], -2
	mov [rsp+58h], rbx
	mov [rsp+60h], rbp
	mov [rsp+68h], rsi
	mov rbp, r8
	mov rdi, rdx
	mov rsi, rcx
	mov rdx, 144772A10h
	mov rax, 140659EA0h
	call rax
	mov rax, [EzTextlistSelectorVtable]
	mov [rsi], rax
	mov rdx, [pMainMem]
	mov rdx, [rdx]
	lea rcx, [rsi+0B8h]
	mov rax, 14002F860h
	call rax
	mov dword ptr [rsi+0E8h], 1
	mov dword ptr [rsi+0ECh], 0Ah
	mov dword ptr [rsi+0F0h], 41700000h
	mov dword ptr [rsi+0F4h], 41200000h
	xor ebx, ebx
	mov [rsi+0F8h], rbx
	mov rdx, [pMainMem]
	mov rdx, [rdx]
	lea rcx, [rsi+100h]
	mov rax, 14002F860h
	call rax
	mov [rsi+130h], rbx
	mov dword ptr [rsi+138h], 1
	mov [rsi+13Ch], bx
	mov [rsi+140h], rbx
	mov [rsi+148h], ebx
	test rbp, rbp
	je loc_140CF291B
	cmp [rbp], bx
	jne loc_140CF28F4
	mov r8d, ebx
	jmp loc_140CF290C
loc_140CF28F4:
	or r8, -1
loc_140CF2900:
	inc r8
	cmp word ptr [rbp+r8*2], 0
	jne loc_140CF2900
loc_140CF290C:
	mov rdx, rbp
	lea rcx, [rsi+0B8h]
	mov rax, 140003C40h
	call rax
loc_140CF291B:
	test rdi, rdi
	je loc_140CF294D
	cmp word ptr [rdi], 0
	je loc_140CF293A
	or rbx, -1
loc_140CF2930:
	inc rbx
	cmp word ptr [rdi+rbx*2], 0
	jne loc_140CF2930
loc_140CF293A:
	mov r8, rbx
	mov rdx, rdi
	lea rcx, [rsi+100h]
	mov rax, 140003C40h
	call rax
loc_140CF294D:
	mov rax, rsi
	mov rbx, [rsp+58h]
	mov rbp, [rsp+60h]
	mov rsi, [rsp+68h]
	add rsp, 30h
	pop r15
	pop r14
	pop rdi
	ret
initEzTextlistSelector ENDP

.data

.code

DebugBootMenuStep_STEP_Wait_forChild PROC
	mov [rsp+8], rbx
	push rdi
	sub rsp, 20h
	mov rbx, rcx
	add rcx, 218h
	mov rax, 140F00B50h
	call rax
	mov rcx, rbx
	test al, al
	je loc_140BE503F
	mov rbx, [rsp+30h]
	add rsp, 20h
	pop rdi
	mov rax, 14091DFD0h
	jmp rax
loc_140BE503F:
	mov rax, 140F01520h
	call rax
	test al, al
	je loc_14091DFC4
	lea rcx, [rbx+218h]
	mov rax, 140F00C10h
	call rax
loc_14091DFC4:
	mov rbx, [rsp+30h]
	add rsp, 20h
	pop rdi
	ret
DebugBootMenuStep_STEP_Wait_forChild ENDP

DebugBootMenuStep_STEP_Finish_forChild PROC
	push rbx
	sub rsp, 20h
	;restore anti-tamper
	mov rbx, 145CFBB5Dh
	mov ax, 288h
	mov [rbx], ax
	mov rbx, 145721DD5h
	mov ax, 289h
	mov [rbx], ax
	mov rbx, rcx
	add rcx, 218h
	mov rax, 140F00AE0h
	call rax
	mov edx, 1
	mov rcx, rbx
	add rsp, 20h
	pop rbx
	mov rax, 14091E0F0h
	jmp rax
DebugBootMenuStep_STEP_Finish_forChild ENDP

DebugBootMenuStep_STEP_Finish PROC
	
DebugBootMenuStep_STEP_Finish ENDP

.data
	initSprjEzSelectBot QWORD 140ED82D0h
	wcsicmp QWORD 141FC5870h
	initSprjMoveMapListStep QWORD 140950040h
	initTitleFlowStep QWORD 14095DAD0h
.code

DebugBootMenuStep_STEP_Update_forList PROC
	mov rax, rsp
	push rbp
	lea rbp, [rax-5Fh]
	sub rsp, 90h
	mov qword ptr [rbp+7], -2
	mov [rax+18h], rbx
	mov [rax+20h], rdi
	mov rdi, rcx
	mov rax, [rcx+0C8h]
	add rcx, 0C8h
	call qword ptr [rax+0A8h]
	lea rdx, [ezSelectBotDebugBootMenuString]
	lea rcx, [rbp+0Fh]
	call initSprjEzSelectBot
	lea rdx, [rdi+0C8h]
	mov rcx, rax
	call sub_1416693B0
	lea rcx, [rbp+0Fh]
	mov rax, 140ED8360h
	call rax
	lea rcx, [rdi+0C8h]
	mov rax, 14095F360h
	call rax
	test al, al
	je loc_140BE4E60
	lea rcx, [rdi+0C8h]
	mov rax, 14095F280h
	call rax
	mov rbx, rax
	test rax, rax
	je loc_140BE4E60
	lea rdx, [debugBootMenuStringModelViewer]
	mov rcx, rax
	call wcsicmp
	test eax, eax
	jne loc_140BE4A0D
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-9], r8
	mov dword ptr [rsp+20h], 0A6h
	lea edx, [rax+10h]
	mov ecx, 590h
	call gotAnymoreOfThemMemory
	mov [rbp-1], rax
	test rax, rax
	je loc_140BE4A08
	mov dl, 1
	mov rcx, rax
	;call initModelViewerStep
loc_140BE4A08:
	jmp loc_140BE4E4C
loc_140BE4A0D:
	lea rdx, [debugBootMenuStringSoundTest]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4A5E
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0AAh
	lea edx, [rax+8]
	mov ecx, 188h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4A59
	mov rcx, rax
	;call initCSSoundTestStep
loc_140BE4A59:
	jmp loc_140BE4E4C
loc_140BE4A5E:
	lea rdx, [debugBootMenuStringMapViewer]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	je loc_140BE4E60
	lea rdx, [debugBootMenuStringStageSelect]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	je loc_140BE4E60
	lea rdx, [debugBootMenuStringAutoTest]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4AEB
	mov cl, 1
	;enable sth
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0B5h
	mov edx, 8
	mov ecx, 0D0h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4AE6
	mov edx, 1
	mov rcx, rax
	call initSprjMoveMapListStep
loc_140BE4AE6:
	jmp loc_140BE4E4C
loc_140BE4AEB:
	lea rdx, [debugBootMenuStringTitleBoot]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4B44
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0B8h
	lea edx, [rax+8]
	mov ecx, 0E8h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4B3F
	mov r8b, 1
	mov edx, 1
	mov rcx, rax
	call initTitleFlowStep
loc_140BE4B3F:
	jmp loc_140BE4E4C
loc_140BE4B44:
	lea rdx, [debugBootMenuStringE3TitleBoot]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4B9D
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0BCh
	lea edx, [rax+8]
	mov ecx, 0E8h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4B98
	mov r8b, 1
	mov edx, 3
	mov rcx, rax
	call initTitleFlowStep
loc_140BE4B98:
	jmp loc_140BE4E4C
loc_140BE4B9D:
	lea rdx, [debugBootMenuStringGamescomTitleBoot]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4BF6
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0C0h
	lea edx, [rax+8]
	mov ecx, 0E8h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4BF1
	mov r8b, 1
	mov edx, 2
	mov rcx, rax
	call initTitleFlowStep
loc_140BE4BF1:
	jmp loc_140BE4E4C
loc_140BE4BF6:
	lea rdx, [debugBootMenuStringLoadTest]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4C49
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0C4h
	lea edx, [rax+8]
	mov ecx, 238h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4C44
	mov dl, 1
	mov rcx, rax
	;call initLoadFileTestSelectStep
loc_140BE4C44:
	jmp loc_140BE4E4C
loc_140BE4C49:
	lea rdx, [debugBoogMenuStringDebugMoveMapTest]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4C9F
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0C8h
	lea edx, [rax+8]
	mov ecx, 0D0h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4C9A
	mov edx, 1
	mov rcx, rax
	call initSprjMoveMapListStep
loc_140BE4C9A:
	jmp loc_140BE4E4C
loc_140BE4C9F:
	lea rdx, [debugBootMenuStringMoveMapTest]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4CFE
	xor ecx, ecx
	mov rax, 144769744h
	mov [rax], cl
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0CDh
	mov edx, 8
	mov ecx, 0D0h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4CF9
	mov edx, 1
	mov rcx, rax
	call initSprjMoveMapListStep
loc_140BE4CF9:
	jmp loc_140BE4E4C
loc_140BE4CFE:
	lea rdx, [debugBootMenuStringMoveMapTestCeremony]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4D5D
	mov cl, 1
	mov rax, 144769744h
	mov [rax], cl
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0D1h
	mov edx, 8
	mov ecx, 0D0h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4D58
	mov edx, 1
	mov rcx, rax
	call initSprjMoveMapListStep
loc_140BE4D58:
	jmp loc_140BE4E4C
loc_140BE4D5D:
	lea rdx, [debugBootMenuStringModuleTest]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4DB0
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0D4h
	lea edx, [rax+8]
	mov ecx, 238h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4DAB
	mov dl, 1
	mov rcx, rax
	;call initModuleTestSelectStep
loc_140BE4DAB:
	jmp loc_140BE4E4C
loc_140BE4DB0:
	lea rdx, [debugBootMenuStringMenuViewer]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4E00
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0D8h
	lea edx, [rax+8]
	mov ecx, 130h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4DFE
	mov dl, 1
	mov rcx, rax
	;call initMenuViewerStep
loc_140BE4DFE:
	jmp loc_140BE4E4C
loc_140BE4E00:
	lea rdx, [debugBootMenuStringSfxViewer]
	mov rcx, rbx
	call wcsicmp
	test eax, eax
	jne loc_140BE4E60
	mov r8, [pMainMem]
	mov r8, [r8]
	mov [rbp-1], r8
	mov dword ptr [rsp+20h], 0DCh
	lea edx, [rax+10h]
	mov ecx, 3D0h
	call gotAnymoreOfThemMemory
	mov [rbp-9], rax
	test rax, rax
	je loc_140BE4E4C
	mov rcx, rax
	;call initCSSfxViewerStep
loc_140BE4E4C:
	mov r8, rax
	mov edx, 7
	lea rcx, [rdi+218h]
	mov rax, 140F00B90h
	call rax
loc_140BE4E60:
	mov rcx, rdi
	cmp qword ptr [rdi+220h], 0
	je loc_140BE4E74
	mov rax, 140935A00h
	call rax
	jmp loc_140BE4E8A
loc_140BE4E74:
	mov rax, 140F01520h
	call rax
	test al, al
	je loc_140BE4E8A
	mov edx, 4
	mov rcx, rdi
	mov rax, 14091CE50h
	call rax
loc_140BE4E8A:
	lea r11, [rsp+90h]
	mov rbx, [r11+20h]
	mov rdi, [r11+28h]
	mov rsp, r11
	pop rbp
	ret
DebugBootMenuStep_STEP_Update_forList ENDP

.data

.code

sub_1416693B0 PROC
	push rbp
	push rsi
	push rdi
	push r12
	push r13
	push r14
	push r15
	lea rbp, [rsp-27h]
	sub rsp, 0A0h
	mov qword ptr [rbp-51h], -2
	mov [rsp+0F0h], rbx
	mov r12, rdx
	mov rsi, rcx
	mov rax, 140ED8FB0h
	call rax
	test al, al
	je loc_14166951E
	lea rdx, [rbp-19h]
	mov rcx, rsi
	mov rax, 140ED8DC0h
	call rax
	mov rdi, rax
	mov r15, [rax+10h]
	cmp qword ptr [rax+18h], 8
	jb loc_141669418
	mov r14, [rax]
	jmp loc_14166941B
loc_141669418:
	mov r14, rax
loc_14166941B:
	mov rax, 14176C360h
	call rax
	mov rbx, rax
	mov rax, [rax]
	xor r8d, r8d
	lea rdx, [rbp-59h]
	mov rcx, rbx
	call qword ptr [rax+18h]
	mov ecx, [rax]
	shr ecx, 5
	test cl, 1
	jne loc_141669450
	mov r8, 1439E1800h
	mov edx, 3Eh
	xor ecx, ecx
	mov rax, 141769E10h
	call rax
loc_141669450:
	xor r13d, r13d
	mov [rbp-39h], r13
	mov [rbp-31h], r13
	mov [rbp-29h], rbx
	mov qword ptr [rbp-31h], 7
	mov [rbp-39h], r13
	mov word ptr [rbp-49h], r13w
	mov r8, r15
	mov rdx, r14
	lea rcx, [rbp-49h]
	mov rax, 1400023C0h
	call rax
	movzx eax, byte ptr [rdi+28h]
	mov [rbp-21h], al
	cmp qword ptr [rbp-1], 8
	jb loc_14166949C
	mov rcx, [rbp+7]
	mov rax, [rcx]
	mov rdx, [rbp-19h]
	call qword ptr [rax+68h]
loc_14166949C:
	mov qword ptr [rbp-1], 7
	mov [rbp-9], r13
	mov word ptr [rbp-19h], r13w
	cmp qword ptr [rbp-39h], 0
	jbe loc_1416694C4
	lea rbx, [rbp-49h]
	cmp qword ptr [rbp-31h], 8
	cmovnb rbx, [rbp-49h]
	jmp loc_1416694C7
loc_1416694C4:
	mov rbx, r13
loc_1416694C7:
	mov rcx, rsi
	mov rax, 140ED8D30h
	call rax
	mov edi, eax
	mov rcx, rsi
	mov rax, 140ED8FB0h
	call rax
	test al, al
	je loc_1416694F8
	mov r8d, edi
	mov rdx, rbx
	mov rcx, r12
	call sub_140CF3E70
	test al, al
	je loc_1416694F8
	mov rcx, rsi
	mov rax, 140ED8500h
loc_1416694F8:
	cmp qword ptr [rbp-31h], 8
	jb loc_14166950D
	mov rcx, [rbp-29h]
	mov rax, [rcx]
	mov rdx, [rbp-49h]
	call qword ptr [rax+68h]
loc_14166950D:
	mov qword ptr [rbp-31h], 7
	mov [rbp-39h], r13
	mov word ptr [rbp-49h], r13w
loc_14166951E:
	mov rbx, [rsp+0F0h]
	add rsp, 0A0h
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbp
	ret
sub_1416693B0 ENDP

sub_140CF3E70 PROC
	push rdi
	push r12
	push r15
	sub rsp, 20h
	mov rax, [rcx]
	mov r12d, r8d
	mov r15, rdx
	mov rdi, rcx
	call qword ptr [rax+50h]
	cmp eax, 5
	jge loc_140CF3E9A
	xor al, al
	add rsp, 20h
	pop r15
	pop r12
	pop rdi
	ret
loc_140CF3E9A:
	mov rax, [rdi]
	mov rcx, rdi
	call qword ptr [rax+50h]
	cmp eax, 5
	jne loc_140CF3FA3
	cmp byte ptr [rdi+13Ch], 0
	jne loc_140CF3FA3
	cmp byte ptr [rdi+13Dh], 0
	jne loc_140CF3FA3
loc_140CF3EC6:
	mov [rsp+48h], rbp
	or ebp, -1
	test r15, r15
	je loc_140CF3F52
	mov rax, [rdi+0F8h]
	test rax, rax
	je loc_140CF3F52
loc_140CF3EDF:
	mov [rsp+40h], rbx
	mov [rsp+50h], rsi
	xor esi, esi
	mov [rsp+58h], r14
	movsxd r14, dword ptr [rax+80h]
	mov ebx, esi
	test r14, r14
	jle loc_140CF3F43
loc_140CF3F00:
	mov rax, [rdi+0F8h]
	test rbx, rbx
	js loc_140CF3F35
	cmp esi, [rax+80h]
	jge loc_140CF3F35
	mov rcx, [rax+88h]
	test rcx, rcx
	je loc_140CF3F35
	mov rdx, [rcx+rbx*8]
	test rdx, rdx
	je loc_140CF3F35
	mov rcx, r15
	call wcsicmp
	test eax, eax
	je loc_140CF3F41
loc_140CF3F35:
	inc rbx
	inc esi
	cmp rbx, r14
	jl loc_140CF3F00
	jmp loc_140CF3F43
loc_140CF3F41:
	mov ebp, esi
loc_140CF3F43:
	mov rsi, [rsp+50h]
	mov rbx, [rsp+40h]
	mov r14, [rsp+58h]
loc_140CF3F52:
	test r12d, r12d
	js loc_140CF3F72
	test ebp, ebp
	jns loc_140CF3F76
	mov rax, [rsi+0F8h]
	test rax, rax
	je loc_140CF3F72
	cmp r12d, [rax+80h]
	cmovl ebp, r12d
loc_140CF3F72:
	test ebp, ebp
	js loc_140CF3F92
loc_140CF3F76:
	mov eax, ebp
	mov byte ptr [rdi+13Ch], 1
	cdq
	idiv dword ptr [rdi+0ECh]
	mov [rdi+130h], edx
	mov [rdi+134h], eax
loc_140CF3F92:
	mov al, 1
	mov rbp, [rsp+48h]
	add rsp, 20h
	pop r15
	pop r12
	pop rdi
	ret
loc_140CF3FA3:
	mov al, 1
	add rsp, 20h
	pop r15
	pop r12
	pop rdi
	ret
sub_140CF3E70 ENDP

;addition for EzTextlistSelector

.data
	numericString1 WORD 25h, 30h, 31h, 64h, 0
	numericString2 WORD 25h, 30h, 32h, 64h, 0
	numericString3 WORD 25h, 30h, 33h, 64h, 0
	numericString4 WORD 25h, 30h, 34h, 64h, 0
	sthFormatString QWORD 14001B4B0h
	dashesString WORD 0FF0Dh, 0FF0Dh, 0FF0Dh, 0
	filledEntryString WORD 25h, 73h, 3Ah, 25h, 73h, 20h, 3Ch, 25h, 73h, 3Eh, 0
	halfFilledEntryString WORD 25h, 73h, 3Ah, 0FF0Dh, 0FF0Dh, 0FF0Dh, 20h, 3Ch, 25h, 73h, 3Eh, 0
	emptyEntryString WORD 25h, 73h, 3Ah, 0FF0Dh, 0FF0Dh, 0FF0Dh, 0
.code

sub_140CF3520 PROC
	mov rax, rsp
	push rbp
	push rdi
	push r12
	push r14
	push r15
	mov rbp, rsp
	sub rsp, 80h
	mov qword ptr [rbp-48h], -2
	mov [rax+8], rbx
	mov [rax+18h], rsi
	mov rsi, rdx
	mov r14, rcx
	mov r15, [rbp+60h]
	mov rdi, [rbp+68h]
	mov rax, 14176C360h
	call rax
	mov rbx, rax
	mov rax, [rax]
	xor r8d, r8d
	lea rdx, [rsp+38h]
	mov rcx, rbx
	call qword ptr [rax+18h]
	mov ecx, [rax]
	shr ecx, 5
	test cl, 1
	jne loc_140CF3596
	mov r8, 1439E1800h
	mov edx, 3Eh
	xor ecx, ecx
	mov rax, 141769E10h
loc_140CF3596:
	xor r12d, r12d
	mov [rbp-30h], r12
	mov [rbp-28h], r12
	mov [rbp-20h], rbx
	mov qword ptr [rbp-28h], 7
	mov [rbp-30h], r12
	mov word ptr [rbp-40h], r12w
	mov byte ptr [rbp-18h], 1
	mov eax, [rbp+58h]
	cmp eax, 0Ah
	jge loc_140CF35CB
	lea rdx, [numericString1]
	jmp loc_140CF35EE
loc_140CF35CB:
	cmp eax, 64h
	jge loc_140CF35D9
	lea rdx, [numericString2]
	jmp loc_140CF35EE
loc_140CF35D9:
	cmp eax, 3E8h
	lea rdx, [numericString3]
	jl loc_140CF35EE
	lea rdx, [numericString4]
loc_140CF35EE:
	mov r8d, [rbp+50h]
	lea rcx, [rbp-40h]
	call sthFormatString
	lea r9, [dashesString]
	test r15, r15
	cmovne r9, r15
	mov ecx, [r14+0E8h]
	test ecx, ecx
	je loc_140CF36AF
	dec ecx
	je loc_140CF3686
	dec ecx
	je loc_140CF365D
	dec ecx
	jne loc_140CF36CD
	lea r8, [rbp-40h]
	mov rcx, rsi
	test rdi, rdi
	je loc_140CF3654
	cmp qword ptr [rbp-28h], 8
	cmovnb r8, [rbp-40h]
	mov [rsp+20h], r9
	mov r9, rdi
	lea rdx, [filledEntryString]
	call sthFormatString
	jmp loc_140CF36CD
loc_140CF3654:
	lea rdx, [halfFilledEntryString]
	jmp loc_140CF36BD
loc_140CF365D:
	lea r8, [rbp-40h]
	mov rcx, rsi
	test rdi, rdi
	je loc_140CF366E
	mov r9, rdi
	jmp loc_140CF36B6
loc_140CF366E:
	cmp qword ptr [rbp-28h], 8
	cmovnb r8, [rbp-40h]
	lea rdx, [emptyEntryString]
	call sthFormatString
	jmp loc_140CF36CD
loc_140CF3686:
	lea r8, [rbp-40h]
	mov rcx, rsi
	test rdi, rdi
	je loc_140CF36B6
	cmp qword ptr [rbp-28h], 8
	cmovnb r8, [rbp-40h]
	mov [rsp+20h], rdi
	lea rdx, [filledEntryString]
	call sthFormatString
	jmp loc_140CF36CD
loc_140CF36AF:
	lea r8, [rbp-40h]
	mov rcx, rsi
loc_140CF36B6:
	mov rdx, 142878EC0h
loc_140CF36BD:
	cmp qword ptr [rbp-28h], 8
	cmovnb r8, [rbp-40h]
	call sthFormatString
loc_140CF36CD:
	cmp qword ptr [rbp-28h], 8
	jb loc_140CF36E2
	mov rcx, [rbp-20h]
	mov rax, [rcx]
	mov rdx, [rbp-40h]
	call qword ptr [rax+68h]
loc_140CF36E2:
	mov qword ptr [rbp-28h], 7
	mov [rbp-30h], r12
	mov word ptr [rbp-40h], r12w
	lea r11, [rsp+80h]
	mov rbx, [r11+30h]
	mov rsi, [r11+40h]
	mov rsp, r11
	pop r15
	pop r14
	pop r12
	pop rdi
	pop rbp
	ret
sub_140CF3520 ENDP

patchMoveMapFinishAntiTamper PROC
	mov rcx, 145CFBB5Dh
	mov ax, 9090h
	mov [rcx], ax
	mov rcx, 145721DD5h
	mov [rcx], ax
	mov rax, 1408E7A90h
	jmp rax
patchMoveMapFinishAntiTamper ENDP

END