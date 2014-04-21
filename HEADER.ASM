              .386p

_HEADER       segment word public use16 'DATA'

              extrn Strategy_ : far

              dd 0FFFFFFFFh               ;; Link to next header
              dw 9980h                    ;; Device attribute word
              dw Strategy_                ;; Strategy entry point
              dw 0                        ;; IDC entry point
              db "ASPIROU$"               ;; Device driver name
              dq 0                        ;; Reserved
              dd 00000011h                ;; Capabilities bit strip

_HEADER       ends

_DATA   SEGMENT WORD PUBLIC USE16 'DATA'
_DATA   ENDS
_BSS    SEGMENT WORD PUBLIC USE16 'BSS'
_BSS    ENDS
CONST   SEGMENT WORD PUBLIC USE16 'DATA'
CONST   ENDS
CONST2  SEGMENT WORD PUBLIC USE16 'DATA'
CONST2  ENDS
ENDDATA SEGMENT WORD PUBLIC USE16 'ENDDATA'
ENDDATA ENDS

_TEXT   SEGMENT WORD PUBLIC USE16 'CODE'
_TEXT   ENDS
CODE    SEGMENT WORD PUBLIC USE16 'CODE'
CODE    ENDS

DGROUP  GROUP _DATA, _BSS, CONST, CONST2, ENDDATA
CGROUP  GROUP _TEXT, CODE


              end
