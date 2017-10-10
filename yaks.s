YKEnterMutex:
	cli
	iret
YKExitMutex:
	sti
	iret

GetSP:
	push bp
	mov bp, sp
	push ax
	mov ax, sp
	mov sp, bp
	pop bp
	ret

GetIP:
	push ax
	mov ax, ip
	ret
	

GetFlags:
	push bp
	mov bp, sp
	pushf
	push ax
	mov ax, [bp+4]
	mov sp, bp
	pop bp
	ret
	

