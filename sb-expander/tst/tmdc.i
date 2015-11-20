;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 6 - Mínimo Divisor Comum (tmdc.i)    ;
; Calcula o MDC entre dois números inteiros. ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convenções
; R0 - sempre 0
; R1 - sempre 1

            LOAD R1 ONE

            READ R2
            READ R3

IS_R2_NEG:  COPY R2 R2
            JNN IS_R3_NEG
            NEG R2

IS_R3_NEG:  COPY R3 R3
            JNN SWAP
            NEG R3

SWAP:       COPY R4 R2
            SUB R4 R3
            JNN EUC_LOOP
            SWAP_R2_R3

EUC_LOOP:   COPY R0 R0; Chamadas de macro não podem vir após labels

            REMAINDER R6
            JZ END
            COPY R2 R3
            COPY R3 R6
            JMP EUC_LOOP

END:        WRITE R3

            HALT

ONE:        WORD 1

; Resto da divisão: assume que ambos operandos são positivos
; PARÂMETRO = R2 % R3
REMAINDER:  BEGINMACRO PARAM
            PUSH R2
            PUSH R3

REM_LOOP:   SUB R2 R3
            JNN REM_LOOP

            COPY PARAM R2
            ADD PARAM R3

            POP R3
            POP R2
            COPY PARAM PARAM; PSW
            ENDMACRO

SWAP_R2_R3: BEGINMACRO
            PUSH R2
            COPY R2 R3
            POP R3
            ENDMACRO

            END
