; Multiplicação inteira
; Operandos: R3 * R4
; Saída: R5

MUL:    PUSH R3
        PUSH R4

        COPY R5 R0; Inicializa saída como 0
        COPY R3 R3; Testa se primeiro operando é negativo
        JN FSTN
        COPY R4 R4; Testa se segundo operando é não-negativo
        JNN MULLP

SNDN:   NEG R3
        NEG R4
        JMP MULLP

BOTHN:  NEG R3
        NEG R4
        JMP MULLP

FSTN:   COPY R4 R4
        JN BOTHN

MULLP:  JZ MULE
        ADD R5 R3
        SUB R4 R1
        JMP MULLP

        POP R4
        POP R3

MULE:   RET
        END
