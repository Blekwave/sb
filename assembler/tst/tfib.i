;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 1 - Fibonacci                                            ;
; Dado um índice, retorna o número de Fibonacci correspondente   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        LOAD R14 ONE; R14 é sempre 1
        COPY R1 R14
        READ R15
        SUB R15 R14
LOOP:   JZ END
        COPY R2 R1
        ADD R1 R0
        COPY R0 R2
        SUB R15 R14; decrementa contador de iterações
        JMP LOOP
END:    WRITE R0
        HALT
ONE:    WORD 1
        END
