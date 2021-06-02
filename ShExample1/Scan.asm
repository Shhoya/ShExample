include macamd64.inc

; BOOLEAN
;     NTAPI
;     _CmpByte(
;         __in CHAR b1,
;         __in CHAR b2
;     );

    LEAF_ENTRY _CmpByte, _TEXT$00
        
        cmp cl, dl
        setnz al
        ret

    LEAF_END _CmpByte, _TEXT$00
        
; BOOLEAN
;     NTAPI
;     _CmpShort(
;         __in SHORT s1,
;         __in SHORT s2
;     );

    LEAF_ENTRY _CmpShort, _TEXT$00
        
        cmp cx, dx
        setnz al
        ret

    LEAF_END _CmpShort, _TEXT$00
    
; BOOLEAN
;     NTAPI
;     _CmpLong(
;         __in LONG l1,
;         __in LONG l2
;     );

    LEAF_ENTRY _CmpLong, _TEXT$00
        
        cmp ecx, edx
        setnz al
        ret

    LEAF_END _CmpLong, _TEXT$00
    
; BOOLEAN
;     NTAPI
;     _CmpLongLong(
;         __in LONGLONG ll1,
;         __in LONGLONG ll2
;     );

    LEAF_ENTRY _CmpLongLong, _TEXT$00
        
        cmp rcx, rdx
        setnz al
        ret

    LEAF_END _CmpLongLong, _TEXT$00

        end