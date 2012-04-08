; file ps.as – includes main routine of reversing string "abcdef"
		.entry		STRADD
		.entry		MAIN
		.extern		REVERSE
		.extern		PRTSTR
		.extern		COUNT
STRADD:		.data		0
STR:		.string		"abcdef"
LASTCHAR:	.data		0
LEN:		.data		0
K:		.data		0

; count length of string, print the original string, reverse string, print reversed string
MAIN:		lea		STR[%LEN], STRADD
		jsr		COUNT
		jsr		PRTSTR
		mov		STRADD, [K]LASTCHAR[r3]
		mov		STR[%K], r7
		add		COUNT[%LEN],r3
		dec		LASTCHAR[%K]
		inc		K
		jsr		REVERSE
		jsr		PRTSTR
		stop
		