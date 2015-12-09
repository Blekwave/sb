; Divisão inteira
; Operandos: R3 / R4
; Saídas: R5 (quociente), R6 (resto)

; R7: Incrementador ou decrementador
; R8: Flag que define se o resto é negativo ou não

DIV:    PUSH R3
        PUSH R4
        PUSH R7
        PUSH R8

        COPY R7 R1
        COPY R3 R3; Para testar se R3 é negativo e atualizar o PSW
        JN DVDNDN
        COPY R4 R4
        JNN DVLOOP

; Divisor is negative
DSORN:  NEG R4
        NEG R7
        JMP DVLOOP

; Dividend is negative
DVDNDN: ADD R8 R4; Flag para inverter o resto
        COPY R4 R4
        JN DBOTHN
        NEG R3
        NEG R7
        JMP DVLOOP

; Both are negative
DBOTHN: NEG R3
        NEG R4

DVLOOP: COPY R6 R3
        SUB R3 R4
        JN DVEND
        ADD R5 R7
        JMP DVLOOP

DVEND:  COPY R8 R8
        JZ WREMND
        NEG R6

; Write remainder
WREMND: POP R8
        POP R7
        POP R4
        POP R3

        RET
        END
