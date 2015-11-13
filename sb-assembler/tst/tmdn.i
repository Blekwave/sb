;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 4 - Mediana (tmdn.i)                                     ;
; Obtém a mediana de sete valores definidos.                     ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convenções
; R0 - sempre 0
; R1 - sempre 1

; Acrônimos nas labels
; LP: loop
; LPE: loop end
; OLP: outer loop
; ILP: inner loop
; SRT: sort
; OPP: operator position

        LOAD R1 ONE

        LOAD R2 ARR_POS
        LOAD R3 ARR_SIZE
        ADD R3 R2; Posição final do vetor em R3

; Lê vetor de sete itens para as posições em [500, 507)
        
INLP:   COPY R4 R3
        SUB R4 R2
        JZ INLPE
        READ R15
        COPY R14 R2
        CALL STORER
        ADD R2 R1
INLPE:  JMP INLP        

        LOAD R12 ARR_POS
        COPY R13 R3
        CALL SORT

        LOAD R2 MEDIAN_INDEX
        ADD R2 R12
        COPY R14 R2
        CALL LOADR

        WRITE R15

        HALT

; Constantes e parâmetros
        ONE: WORD 1
        ARR_POS: WORD 500
        ARR_SIZE: WORD 7
        MEDIAN_INDEX: WORD 3
        LOADR_PC_OFFSET: WORD 75
        STORER_PC_OFFSET: WORD 96

; Subrotinas para acesso à memória com endereço nos registradores
; R14 - Endereço de leitura/escrita
; R15 - Valor lido/a ser escrito
LOADR:  PUSH R13
        PUSH R14
        LOAD R13 LOADR_PC_OFFSET
        SUB R14 R13
        STORE R14 LDOPP
        WORD 1; LOAD
        WORD 15; R15
LDOPP:  WORD 0; Endereço de onde o dado será lido
        POP R14
        POP R13
        RET

STORER: PUSH R13
        PUSH R14
        LOAD R13 STORER_PC_OFFSET
        SUB R14 R13
        STORE R14 STROPP
        WORD 2; STORE
        WORD 15; R15
STROPP: WORD 0; Endereço onde o dado será escrito
        POP R14
        POP R13
        RET

; Ordena um vetor na memória por Bubble Sort
; R12 - Posição de início do vetor
; R13 - Posição seguinte à última posição do vetor  
SORT:   PUSH R2
        PUSH R3
        PUSH R4
        PUSH R5
        PUSH R14
        PUSH R15

        COPY R2 R12
        COPY R3 R12

; Loop externo
SRTOLP: COPY R4 R13
        SUB R4 R2
        ADD R4 R1
        JZ SRTOLPE

        COPY R3 R2 ; j = i + 1
        ADD R3 R1

; Loop interno
SRTILP: COPY R4 R13
        SUB R4 R3
        ADD R4 R1
        JZ SRTILPE

; Carrega os elementos V[i] e V[j] para comparação
        COPY R14 R3
        CALL LOADR
        COPY R5 R15
        COPY R14 R2
        CALL LOADR
        COPY R4 R15

        SUB R4 R5
        JNN SWAPE
        COPY R14 R3 ; Swap
        CALL STORER

        COPY R14 R2
        COPY R15 R5
        CALL STORER

SWAPE:  ADD R3 R1
        JMP SRTILP

SRTILPE:ADD R2 R1
        JMP SRTOLP

SRTOLPE:POP R15
        POP R14
        POP R5
        POP R4
        POP R3
        POP R2
        RET


        END
