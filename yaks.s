YKEnterMutex:
	cli
	iret
YKExitMutex:
	sti
	iret


YKDispatcher:
	push bp
	mov bp, sp	
	pushf
	push cs
	cmp word[bp+4], 0
	je YKDispatcher_run
	call YKSaveContext
	mov sp, bp
	pop bp
	ret

YKSaveContext:
	push bp
	push dx
	push cx
	push bx
	push ax
	push es
	push di
	push si
	push ds
	mov bx, [bp+6]
	mov [bx], sp


	
YKDispatcher_run:
	mov bx, [RdyTask]
	mov sp, [bx]	
	pop ds
	pop si
	pop di
	pop es
	pop ax
	pop bx
	pop cx
	pop dx
	pop bp
	iret
	
	

	
	

