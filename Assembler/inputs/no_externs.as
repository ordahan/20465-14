; file ps.as - includes main routine of reversing string "abcdef"

.entry STRADD
.entry MAIN

STRADD: .data 0
STR: .string "abcdef"
LASTCHAR: .data 0
LEN: .data 0
K: .data 0

; count length of string, print the original string, 
; reverse string, print reversed string.
; comments are trimmed to 80 chars as i decided
; this is a reasonable length for readbility of code
MAIN: lea/0 STR{LEN}, STRADD
jsr/0 COUNT
jsr/0 PRTSTR
mov/1/1/0 STRADD{-5}, LASTCHAR{r3}
mov/1/1/1 STR{7}, r7
add/0 COUNT{K}, r3
REVERSE:dec/1/1/1 LASTCHAR{K}
PRTSTR:inc/0 K
jsr/0 REVERSE
COUNT:jsr/0 PRTSTR

stop/0