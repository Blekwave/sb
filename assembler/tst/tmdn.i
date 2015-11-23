;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Teste 4 - Mediana (tmdn.i)                                     ;
; Obtém a mediana de sete valores definidos.                     ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Convenções
; R0 - sempre 0
; R1 - sempre 1

                        LOAD R1 ONE

                        LOAD R2 ARR_POS
                        LOAD R3 ARR_SIZE
                        ADD R3 R2; Posição final do vetor em R3

; Lê vetor de sete itens para as posições em [500, 507)
READ_LOOP:              COPY R4 R3
                        SUB R4 R2
                        JZ READ_LOOP_END
                        READ R15
                        COPY R14 R2
                        CALL STORER
                        ADD R2 R1
                        JMP READ_LOOP
READ_LOOP_END:          LOAD R12 ARR_POS
                        COPY R13 R3
                        CALL SORT

                        LOAD R2 MEDIAN_INDEX
                        ADD R2 R12
                        COPY R14 R2
                        CALL LOADR

                        WRITE R15

                        HALT

                        ; Constantes e parâmetros
ONE:                    WORD 1
ARR_POS:                WORD 500
ARR_SIZE:               WORD 7
MEDIAN_INDEX:           WORD 3
LOADR_PC_OFFSET:        WORD 75; Equivale a LOADR_POS
STORER_PC_OFFSET:       WORD 96; Equivale a STORER_POS

; [Explicações e funcionamento das subrotinas LOADR e STORER]
; A máquina virtual proposta não possui nenhuma instrução nativa que permita o
; endereçamento indireto de memória através de registradores. Isso limita o pro-
; gramador ao acesso apenas de endereços hard-coded no programa, e impede que
; seja possível, por exemplo, acessar um vetor por um índice. Essa limitação in-
; viabiliza um número imenso de algoritmos. De maneira mais geral (mas não pro-
; vada formalmente), é intuitivo pensar que, limitada às formas de endereçamento
; padrão definidas no conjunto de instruções, essa máquina não consegue ser Tu-
; ring-completa.

; Felizmente, existe uma maneira de simular esse comportamento, através de uso
; criativo da pseudo-instrução WORD e das labels. A ideia geral é editar o có-
; digo do próprio programa com a instrução STORE, trocando o endereço imediato
; de outra instrução LOAD ou STORE em tempo de execução. No entanto, como labels
; se localizam no início das linhas, definir uma label para um operando de uma
; instrução definida regularmente é impossível. Para isso, foi necessário defi-
; nir LOAD e STORE com três WORDs, correspondentes ao operador, ao primeiro ope-
; rando (registrador de entrada/saída) e ao segundo operando, que deve ser modi-
; ficado.

; Subrotinas para acesso à memória com endereço nos registradores
; R14 - Endereço de leitura/escrita
; R15 - Valor lido/a ser escrito

LOADR:                  PUSH R13
                        PUSH R14
                        LOAD R13 LOADR_PC_OFFSET
                        SUB R14 R13
                        STORE R14 LOADR_POS
                        WORD 1; LOAD
                        WORD 15; R15
LOADR_POS:              WORD 0; Endereço de onde o dado será lido
                        POP R14
                        POP R13
                        RET

STORER:                 PUSH R13
                        PUSH R14
                        LOAD R13 STORER_PC_OFFSET
                        SUB R14 R13
                        STORE R14 STORER_POS
                        WORD 2; STORE
                        WORD 15; R15
STORER_POS:             WORD 0; Endereço onde o dado será escrito
                        POP R14
                        POP R13
                        RET

; Ordena um vetor na memória em ordem decrescente
; R12 - Posição de início do vetor
; R13 - Posição seguinte à última posição do vetor
SORT:                   PUSH R2; Salva os registradores modificados
                        PUSH R3
                        PUSH R4
                        PUSH R5
                        PUSH R14
                        PUSH R15

                        COPY R2 R12; Itera pelo loop externo
                        COPY R3 R12; Itera pelo loop interno

SORT_OUTER_LOOP:        COPY R4 R13; 
                        SUB R4 R2
                        ADD R4 R1
                        JZ SORT_OUTER_LOOP_END

                        COPY R3 R2
                        ADD R3 R1

SORT_INNER_LOOP:        COPY R4 R13
                        SUB R4 R3
                        ADD R4 R1
                        JZ SORT_INNER_LOOP_END

                        ; Carrega V[i] e V[j]
                        COPY R14 R3
                        CALL LOADR
                        COPY R5 R15
                        COPY R14 R2
                        CALL LOADR
                        COPY R4 R15

                        SUB R4 R5; Calcula V[i] - V[j]
                        JNN SWAP_END; Troca elementos se V[i] < V[j]
                        COPY R14 R3
                        CALL STORER

                        COPY R14 R2
                        COPY R15 R5
                        CALL STORER

SWAP_END:               ADD R3 R1
                        JMP SORT_INNER_LOOP

SORT_INNER_LOOP_END:    ADD R2 R1
                        JMP SORT_OUTER_LOOP
                        
SORT_OUTER_LOOP_END:    POP R15
                        POP R14
                        POP R5
                        POP R4
                        POP R3
                        POP R2
                        RET


                        END
