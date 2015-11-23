;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 2 - Divisão inteira (tdiv.i)                             ;
; Realiza a divisão inteira de dois números, retornando quoci-   ;
; ente e resto.                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        LOAD R4 ONE; Incrementa ou decrementa quociente
        READ R0
        READ R1

        COPY R0 R0; Para testar se R0 é negativo e atualizar o PSW
        JN DVDNDN
        COPY R1 R1
        JNN LOOP

; Divisor is negative
DSORN:  NEG R1
        NEG R4
        JMP LOOP

; Dividend is negative
DVDNDN: ADD R5 R4; Flag para inverter o resto
        COPY R1 R1
        JN BOTHN
        NEG R0
        NEG R4
        JMP LOOP

; Both are negative
BOTHN:  NEG R0
        NEG R1

LOOP:   COPY R3 R0
        SUB R0 R1
        JN END
        ADD R2 R4
        JMP LOOP

END:    WRITE R2
        COPY R5 R5
        JZ WREMND
        NEG R3

; Write remainder
WREMND: WRITE R3
        HALT

ONE:    WORD 1
        END