#include <stdio.h>
#include "instr.h"

void operNop(Emulator *e){
    if (e->om == om_verbose){
        fprintf(e->out, "NOP\n");
    }
}

void operLoad(Emulator *e, int rd, int ms){
    if (e->om == om_verbose){
        fprintf(e->out, "LOAD R%d (%d) <- Mem[%d + PC] (%d)\n",
                rd, e->regs[rd], ms, e->mem[e->pc + ms]);
    }
    e->regs[rd] = e->mem[e->pc + ms];
}

void operStore(Emulator *e, int rs, int md){
    if (e->om == om_verbose){
        fprintf(e->out, "STORE R%d (%d) -> Mem[%d + PC] (%d)\n",
                rs, e->regs[rs], md, e->mem[e->pc + md]);
    }
    e->mem[e->pc + md] = e->regs[rs];
}

void operRead(Emulator *e, int rd){
    int prev_val = e->regs[rd];
    if (e->om == om_verbose){
        fprintf(e->out, "R%d> ", rd);
    } 
    fscanf(e->in, "%d", &(e->regs[rd]));
    if (e->om == om_verbose){
        fprintf(e->out, "READ R%d (%d) <- stdin (%d)\n",
                rd, prev_val, e->regs[rd]);
    }
}

void operWrite(Emulator *e, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "WRITE R%d (%d) -> stdout\n",
                rs, e->regs[rs]);
    }
    fprintf(e->out, "%d\n", e->regs[rs]);
}

void operCopy(Emulator *e, int rd, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "COPY R%d (%d) <- R%d (%d)\n",
                rd, e->regs[rd], rs, e->regs[rs]);
    }
    e->regs[rd] = e->regs[rs];
}

void operNeg(Emulator *e, int rd){
    if (e->om == om_verbose){
        fprintf(e->out, "NEG R%d (%d)\n",
                rd, e->regs[rd]);
    }
    e->regs[rd] = -1 * e->regs[rd];
}

void operSub(Emulator *e, int rd, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "SUB R%d (%d) -= R%d (%d)\n",
                rd, e->regs[rd], rs, e->regs[rs]);
    }
    e->regs[rd] -= e->regs[rs];
}

void operAdd(Emulator *e, int rd, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "ADD R%d (%d) += R%d (%d)\n",
                rd, e->regs[rd], rs, e->regs[rs]);
    }
    e->regs[rd] += e->regs[rs];
}

void operAnd(Emulator *e, int rd, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "AND R%d (%d) &= R%d (%d)\n",
                rd, e->regs[rd], rs, e->regs[rs]);
    }
    e->regs[rd] &= e->regs[rs];
}

void operOr(Emulator *e, int rd, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "OR R%d (%d) |= R%d (%d)\n",
                rd, e->regs[rd], rs, e->regs[rs]);
    }
    e->regs[rd] |= e->regs[rs];
}

void operXor(Emulator *e, int rd, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "XOR R%d (%d) ^= R%d (%d)\n",
                rd, e->regs[rd], rs, e->regs[rs]);
    }
    e->regs[rd] ^= e->regs[rs];
}

void operNot(Emulator *e, int rd){
    if (e->om == om_verbose){
        fprintf(e->out, "NOT R%d (%d)\n",
                rd, e->regs[rd]);
    }
    e->regs[rd] = ~e->regs[rd];
}

void operJmp(Emulator *e, int ms){
    if (e->om == om_verbose){
        fprintf(e->out, "JMP PC <- PC + %d (%d)\n",
                ms, e->pc + ms);
    }
    e->pc += ms - 2; // This instruction has a single operand. Therefore, PC
                     // would be increased by 2 after the instruction.
}

void operJz(Emulator *e, int ms){
    if (e->om == om_verbose){
        fprintf(e->out, "JZ PC <- PC + %d (%d) [%s]\n",
                ms, e->pc + ms, e->psw == zero ? "Tomado" : "Não tomado");
    }
    if (e->psw == zero)
        e->pc += ms - 2;
}

void operJnz(Emulator *e, int ms){
    if (e->om == om_verbose){
        fprintf(e->out, "JNZ PC <- PC + %d (%d) [%s]\n",
                ms, e->pc + ms, e->psw != zero ? "Tomado" : "Não tomado");
    }
    if (e->psw != zero)
        e->pc += ms - 2;
}

void operJn(Emulator *e, int ms){
    if (e->om == om_verbose){
        fprintf(e->out, "JN PC <- PC + %d (%d) [%s]\n",
                ms, e->pc + ms, e->psw == negative ? "Tomado" : "Não tomado");
    }
    if (e->psw == negative)
        e->pc += ms - 2;
}

void operJnn(Emulator *e, int ms){
    if (e->om == om_verbose){
        fprintf(e->out, "JNN PC <- PC + %d (%d) [%s]\n",
                ms, e->pc + ms, e->psw != negative ? "Tomado" : "Não tomado");
    }
    if (e->psw != negative)
        e->pc += ms - 2;
}

void operPush(Emulator *e, int rs){
    if (e->om == om_verbose){
        fprintf(e->out, "PUSH R%d (%d) -> Mem[SP = %d] (%d)\n",
                rs, e->regs[rs], e->sp - 1, e->mem[e->sp - 1]);
    }
    e->sp--;
    e->mem[e->sp] = e->regs[rs];
}

void operPop(Emulator *e, int rd){
    if (e->om == om_verbose){
        fprintf(e->out, "POP R%d (%d) <- Mem[SP = %d] (%d)\n",
                rd, e->regs[rd], e->sp, e->mem[e->sp]);
    }
    e->regs[rd] = e->mem[e->sp];
    e->sp++;
}

void operCall(Emulator *e, int ms){
    if (e->om == om_verbose){
        fprintf(e->out, "CALL Mem[PC + %d] (%d), Mem[SP - 1] <- PC\n",
                ms, e->mem[e->pc + ms]);
    }
    e->sp--;
    e->mem[e->sp] = e->pc;
    e->pc += ms - 2;
}

void operRet(Emulator *e){
    if (e->om == om_verbose){
        fprintf(e->out, "RET PC <- Mem[SP]\n");
    }
    e->pc = e->mem[e->sp];
    e->sp++;
}

void operHalt(Emulator *e){
    if (e->om == om_verbose){
        fprintf(e->out, "HALT\n");
    }
}

/**
 * Returns the corresponding function for a given instruction specifier in a
 * union.
 * @param  ins Instruction specifier (instr enum)
 * @return     oper union containing the function. The function may be in ei-
 *             ther union.zero, .one or .two, depending on its number of para-
 *             meters.
 */
oper fetchInstr(instr ins){
    oper out;

    switch(ins){
        case instr_nop:
            out.zero = operNop;
            break;
        case instr_load:
            out.two = operLoad;
            break;
        case instr_store:
            out.two = operStore;
            break;
        case instr_read:
            out.one = operRead;
            break;
        case instr_write:
            out.one = operWrite;
            break;
        case instr_copy:
            out.two = operCopy;
            break;
        case instr_neg:
            out.one = operNeg;
            break;
        case instr_sub:
            out.two = operSub;
            break;
        case instr_add:
            out.two = operAdd;
            break;
        case instr_and:
            out.two = operAnd;
            break;
        case instr_or:
            out.two = operOr;
            break;
        case instr_xor:
            out.two = operXor;
            break;
        case instr_not:
            out.one = operNot;
            break;
        case instr_jmp:
            out.one = operJmp;
            break;
        case instr_jz:
            out.one = operJz;
            break;
        case instr_jnz:
            out.one = operJnz;
            break;
        case instr_jn:
            out.one = operJn;
            break;
        case instr_jnn:
            out.one = operJnn;
            break;
        case instr_push:
            out.one = operPush;
            break;
        case instr_pop:
            out.one = operPop;
            break;
        case instr_call:
            out.one = operCall;
            break;
        case instr_ret:
            out.zero = operRet;
            break;
        case instr_halt:
            out.zero = operHalt;
            break;
    }

    return out;
}