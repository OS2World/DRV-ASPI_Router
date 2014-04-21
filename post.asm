;*****************************************************************************
;*                                                                           *
;*  Assembler module for ASPI Router                                         *
;*                                                                           *
;*  This is the entry code for the post callback routine called              *
;*  from OS2ASPI.DMD.                                                        *
;*                                                                           *
;*****************************************************************************

        .386p

        extrn   _aspiPost : near
        extrn   _postInProgress : near db ?     ; added 6.7.97 for v 1.01 abort SRB
        public  postEntry_
        PUBLIC  cstart_

_TEXT   segment word public use16 'CODE'

        assume        cs:_TEXT, ds:DGROUP, es:NOTHING, ss:NOTHING

postEntry_ proc far
cstart_:
        mov     byte ptr _postInProgress, 1     ; added 6.7.97 for v 1.01 abort SRB
        push    bp
        mov     bp, sp
        pusha
        mov     ds, [bp+6]
        push    dword ptr [bp+8]
        call    _aspiPost
        add     sp, 4
        popa
        pop     bp
        mov     byte ptr _postInProgress, 0
        ret
postEntry_ endp

_TEXT   ends

        end
