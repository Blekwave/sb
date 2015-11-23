;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 5 - Fatorial (tfat.i)               ;
; Calcula o fatorial de um número inteiro.  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convenções
; R0 - sempre 0
; R1 - sempre 1

            LOAD R1 ONE

            READ R2 ; Entrada: número do qual se quer o fatorial
            COPY R3 R1 ; Inicializa saída como 1
            COPY R2 R2

FAT_LOOP:   JZ END
            COPY R14 R2
            COPY R15 R3
            MUL R3
            SUB R2 R1
            JMP FAT_LOOP


END:        WRITE R3

; Multiplicação inteira
; R14, R15: operandos
; Parametro: retorno
MUL:    BEGINMACRO RES
        PUSH R14
        PUSH R15
        COPY R10 R0
        COPY R14 R14
        JN FSTN
        COPY R15 R15
        JNN MULLP

SNDN:   NEG R14
        NEG R15
        JMP MULLP

BOTHN:  NEG R14
        NEG R15
        JMP MULLP

FSTN:   COPY R15 R15
        JN BOTHN

MULLP:  JZ MULE
        ADD R10 R14
        SUB R15 R1
        JMP MULLP

MULE:   POP R15
        POP R14

        COPY RES R10

        ENDMACRO

HALT

ONE: WORD 1

