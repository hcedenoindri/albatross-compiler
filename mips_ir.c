#define _POSIX_C_SOURCE 200809L // enable strdup

#include "mips_ir.h"
#include "util.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static FILE * out;

static void emitLabel(char * label, char * comment, ...) {
    static char buffer[1024];
    va_list argp;

    assert(comment != NULL && strcmp("",comment));
    assert(label != NULL);

    va_start(argp, comment);
    vsnprintf(buffer, sizeof(buffer), label, argp);
    va_end(argp);
    fprintf(out, "%s:\n", buffer);
    fflush(out);
}

static void emitInstruction(char * instruction, char * comment, ...) {
    static char buffer[1024];

    va_list argp;
    assert(comment != NULL && strcmp("",comment));
    assert(instruction != NULL);

    va_start(argp, comment);
    vsnprintf(buffer, sizeof(buffer), instruction, argp);
    va_end(argp);
    fprintf(out, "\t\t%s\t\t\t# %s\n", buffer, comment);
    fflush(out);
}

static void mips_ir_translate(ir_node * ir) {
    if (ir == NULL) {
        return;
    }

    switch (ir->kind) {
        case ir_nop: {
            break;
        }
        case ir_iconst: {
            emitInstruction("li $v0,%d", "INT constant", ir->data.iconst);
            break;
        }
        case ir_sconst: {
            assert(0);
        }
        case ir_add: {
            emitInstruction("add $v0,$v0,$v1", "BINOP add");
            break;
        }
        case ir_sub: {
            emitInstruction("sub $v0,$v0,$v1", "BINOP sub");
            break;
        }
        case ir_mul: {
            emitInstruction("mul $v0,$v0,$v1", "BINOP mul");
            break;
        }
        case ir_div: {
            emitInstruction("div $v0,$v1", "BINOP div perform operation");
            emitInstruction("mflo $v0", "BINOP get result");
            break;
        }
        case ir_mod: {
            emitInstruction("div $v0,$v1", "BINOP rem perform operation");
            emitInstruction("mfhi $v0", "BINOP get result");
            break;
        }
        case ir_bor: {
            emitInstruction("or $v0,$v0,$v1", "BINOP bor");
            break;
        }
        case ir_band: {
            emitInstruction("and $v0,$v0,$v1", "BINOP band");
            break;
        }
        case ir_xor: {
            emitInstruction("xor $v0,$v0,$v1", "BINOP xor");
            break;
        }
        case ir_or: {
            emitInstruction("sne $v0,$v0,$zero", "BINOP or");
            emitInstruction("sne $v1,$v1,$zero", "BINOP or");
            emitInstruction("or $v0,$v0,$v1", "BINOP or");
            break;
        }
        case ir_and: {
            emitInstruction("sne $v0,$v0,$zero", "BINOP and");
            emitInstruction("sne $v1,$v1,$zero", "BINOP and");
            emitInstruction("and $v0,$v0,$v1", "BINOP and");
            break;
        }
        case ir_eq: {
            emitInstruction( "seq $v0,$v0,$v1", "BINOP eq");
            break;
        }
        case ir_lt: {
            emitInstruction( "slt $v0,$v0,$v1", "BINOP lt");
            break;
        }
        case ir_gt: {
            emitInstruction( "sgt $v0,$v0,$v1", "BINOP gt");
            break;
        }
        case ir_not: {
            emitInstruction("seq $v0,$v0,$zero", "UNOP not");
            break;
        }
        case ir_reserve: {
            break;
        }
        case ir_read: {
            emitInstruction("lw $v0,%s", "READ var", ir->data.read_write.name);
            break;
        }
        case ir_address: {
            emitInstruction("la $v0,%s", "ADDRESS var", ir->data.read_write.name);
            break;
        }
        case ir_write: {
            emitInstruction("sw $v0,%s", "WRITE var", ir->data.read_write.name);
            break;
        }
        case ir_arglocal_read: {
            emitInstruction("lw $v0,-%i($fp)", "READ local var", 4*(ir->data.iconst+1));
            break;
        }
        case ir_arglocal_write: {
            emitInstruction("sw $v0,-%i($fp)", "WRITE local var", 4*(ir->data.iconst+1));
            break;
        }
        case ir_lbl: {
            emitLabel(ir->data.lbl->name, "LABEL");
            break;
        }
        case ir_jump: {
            emitInstruction("j %s", "JUMP", ir->data.lbl->name);
            break;
        }
        case ir_branchzero: {
            emitInstruction("beq $v0,$zero,%s", "IF jump if zero", ir->data.lbl->name);
            break;
        }
        case ir_call: {
            // Move $fp to top of frame
            emitInstruction("sw $fp,%d($sp)", "CALL save old frame pointer", 4*(ir->data.call_function.vars+1));

            emitInstruction("move $fp, $sp", "CALL set frame pointer");
            emitInstruction("addi $fp, $fp, %d", "CALL adjust frame pointer", 4*(ir->data.call_function.vars+1));

            emitInstruction("jal %s", "CALL jump to start of function", ir->data.call_function.lbl->name);
            break;
        }
        case ir_function: {
            // Create label for function
            emitLabel(ir->data.call_function.lbl->name, "FUNCTION start of function");
            // Make room for local variables
            emitInstruction("sub $sp, $sp, %d", "FUNCTION make room for locals", (ir->data.call_function.vars*4));
            // Save return address on the stack
            emitInstruction("sw $ra, 0($sp)", "FUNCTION push ret address");
            emitInstruction("sub $sp, $sp, 4", "FUNCTION increment sp");
            break;
        }
        case ir_ret: {
            // Register $v0 has the return value (popped from prev instruction)

            emitInstruction("lw $ra, 4($sp)", "RETURN move ret address");
            // Register $ra now holds the return address

            // Restore previous $fp
            emitInstruction("move $sp, $fp", "RETURN set $sp to $fp");
            emitInstruction("lw $fp, 0($sp)", "RETURN restore previous $fp");

            // Leave return value on top of the stack
            emitInstruction("sw $v0, 0($sp)", "RETURN push return value");
            emitInstruction("subi $sp, $sp, 4", "RETURN push return value");

            emitInstruction("jr $ra", "RETURN jump back to caller");
            break;
        }
        case ir_intrinsic: {
            if (ir->data.intrinsic == intrinsic_exit) {
                emitInstruction("move $a0, $v0", "EXIT pass argument to syscall");
                emitInstruction("li $v0, 17", "EXIT specify exit2 syscall");
                emitInstruction("syscall", "EXIT perform syscall");
                break;
            } else if (ir->data.intrinsic == intrinsic_print_int) {
                emitInstruction("move $a0, $v0", "PRINTINT pass argument to syscall");
                emitInstruction("li $v0, 1", "PRINTINT specify print_int syscall");
                emitInstruction("syscall", "PRINTINT perform syscall");
                break;
            } else if (ir->data.intrinsic == intrinsic_print_string) {
                emitInstruction("move $a0, $v0", "PRINTSTRING pass address of string to print");
                emitInstruction("li $v0, 4", "PRINTSTRING specify print_string as the syscall to perform");
                emitInstruction("syscall", "PRINTSTRING perform syscall");
                break;
            } else {
                assert(0);
            }
        }
        case ir_seq: {
            assert(0);
        }
        case ir_push: {
            emitInstruction("sw $v%d, 0($sp)", "PUSH write val on stack", ir->data.iconst);
            emitInstruction("sub $sp, $sp, 4", "PUSH increment sp");
            break;
        }
        case ir_pop: {
            emitInstruction("addi $sp, $sp, 4", "POP decrement sp");
            emitInstruction("lw $v%d, 0($sp)", "POP load top of stack", (ir->data.iconst == -1 ? 0 : ir->data.iconst));
            break;
        }
        default: {
            assert(0); // Not supported
        }
    }

    mips_ir_translate(ir->next);
}

static void mips_ir_variables(ir_node * ir) {
    static int strConsts = 0;
    static char buffer[1024];

    while (ir != NULL) {
        if (ir->kind == ir_reserve) {
            fprintf(out, "%s: .word 0\n", ir->data.reserve.name);
            fflush(out);
        }

        if (ir->kind == ir_sconst) {
            snprintf(buffer, sizeof(buffer), "_stringConstant%d", strConsts);
            strConsts += 1;

            fprintf(out, "%s: .asciiz \"%s\"\n", buffer, ir->data.sconst);
            fflush(out);

            ir->kind = ir_address;
            ir->data.read_write.name = strdup(buffer);
        }

        ir = ir->next;
    }
}

// generate MIPS from IR
void mips_ir(ir_node * ir, FILE * o) {
    out = o;

    fprintf(out, ".data\n");
    fflush(out);
    mips_ir_variables(ir);

    fprintf(out, ".text\n");
    fflush(out);
    mips_ir_translate(ir);
}