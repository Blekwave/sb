;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 5 - Outras instruções                                    ;
; Demonstra as instruções não testadas nas outras instâncias.    ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Convenções
; R0 - sempre 0
; R1 - sempre 1
        LOAD R1 ONE

        READ R2
        READ R3

        COPY R2 R2; Para atualizar PSW
        JNZ R2NZ
        ADD R4 R1

R2NZ:   COPY R3 R3
        JNZ R3NZ
        ADD R5 R1

R3NZ:   COPY R6 R4
        AND R6 R5
        WRITE R6

        COPY R6 R4
        OR R6 R5
        WRITE R6

        COPY R6 R4
        XOR R6 R5
        WRITE R6

        NOT R4
        WRITE R4

        HALT

ONE:    WORD 1
        
        END
