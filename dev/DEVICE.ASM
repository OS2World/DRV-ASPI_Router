
;; Declare segments in the correct order

              .386p
              .seq

_HEADER       segment word public use16 'DATA'
_HEADER       ends

CONST         segment word public use16 'DATA'
CONST         ends

CONST2        segment word public use16 'DATA'
CONST2        ends

_DATA         segment word public use16 'DATA'
_DATA         ends

_BSS          segment word public use16 'BSS'
_BSS          ends

_ENDDS        segment word public use16 'ENDDS'
_ENDDS        ends

_INITDATA     segment word public use16 'INITDATA'
_INITDATA     ends


_TEXT         segment word public use16 'CODE'
_TEXT         ends

_ENDCS        segment word public use16 'CODE'
_ENDCS        ends

_INITTEXT     segment word public use16 'CODE'
_INITTEXT     ends


DGROUP        group _HEADER, CONST, CONST2, _DATA, _BSS, _ENDDS, _INITDATA



;; DevHelp support routines and data

_DATA         segment word public use16 'DATA'

              public _Dev_Entry
              public _Dev_StdEntry
              public _Dev_DbgEntry
              public __wcpp_2_pure_error__
              public __wcpp_2_undef_vfun__

_DATA         ends


_TEXT         segment word public use16 'CODE'
              assume cs:CGROUP, ds:DGROUP, es:NOTHING, ss:NOTHING

Entry:
__wcpp_2_pure_error__:
__wcpp_2_undef_vfun__:
DevHelpDebug  proc far
              int 3
              call dword ptr _Dev_StdEntry
              int 3
              ret
DevHelpDebug  endp

_TEXT         ends


_DATA         segment word public use16 'DATA'

_Dev_Entry    dd DevHelpDebug
_Dev_StdEntry dd DevHelpDebug
_Dev_DbgEntry dd DevHelpDebug

_DATA         ends


_ENDDS        segment word public use16 'ENDDS'

              public ___OffFinalDS

___OffFinalDS db 0

_ENDDS        ends


_ENDCS        segment word public use16 'CODE'

              public ___OffFinalCS

___OffFinalCS proc near
              int 3
___OffFinalCS endp

_ENDCS        ends



              end      Entry
