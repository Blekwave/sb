;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 3 - Exponenciação (texp.i)                               ;
; Realiza exponenciação de uma base a um expoente positivo.      ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

READ R0; Base
READ R1; Expoente
LOAD R2 ONE; Resultado
COPY R3 R2; Sempre 1, usado para decrementar R1
COPY R1 R1; Atualiza PSW

LOOP:
JZ END
COPY R8 R2; -\ Parâmetros
COPY R9 R0; -/ da função.
CALL MUL
COPY R2 R10; Valor de retorno em R10
SUB R1 R3
JMP LOOP

END:
WRITE R2
HALT

MUL:
LOAD R10 ZERO
COPY R8 R8
JN FIRST_NEG
COPY R9 R9
JNN MUL_LOOP

SECOND_NEG:
NEG R8
NEG R9
JMP MUL_LOOP

BOTH_NEG:
NEG R8
NEG R9
JMP MUL_LOOP

FIRST_NEG:
COPY R9 R9
JN BOTH_NEG

MUL_LOOP: JZ MUL_END
ADD R10 R8
SUB R9 R3
JMP MUL_LOOP

MUL_END:
RET

ZERO: WORD 0
ONE: WORD 1
END