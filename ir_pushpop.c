#include "ir_pushpop.h"

#include <assert.h>
#include <stdlib.h>

static ir_node * mips_ir_push(int i) {
    ir_node * ret = malloc(sizeof(ir_node));
    ret->kind = ir_push;
    ret->data.iconst = i;
    return ret;
}

static ir_node * mips_ir_pop(int i) {
    ir_node * ret = malloc(sizeof(ir_node));
    ret->kind = ir_pop;
    ret->data.iconst = i;
    return ret;
}

ir_node * addPushPop(ir_node * ir) {
    if (ir == NULL) {
        return NULL;
    }

    ir_node * next = ir->next;

    switch (ir->kind) {
        case ir_nop: {
            free(ir);
            return addPushPop(next);
        }
        case ir_iconst: {
            ir->next = mips_ir_push(0);
            ir->next->next = addPushPop(next);
            break;
        }
        case ir_sconst: {
            ir->next = mips_ir_push(0);
            ir->next->next = addPushPop(next);
            break;
        }
        case ir_add:
        case ir_sub:
        case ir_mul:
        case ir_div:
        case ir_mod:
        case ir_bor:
        case ir_band:
        case ir_xor:
        case ir_eq:
        case ir_lt:
        case ir_gt:
        case ir_or:
        case ir_and:
        {
            ir_node * next = ir->next;

            // BIN -> [next]
            // should be
            // POP1 -> POP0 -> BIN -> PUSH0 -> [next]


            ir_node * ret = mips_ir_pop(1);
            // POP1
            ret->next = mips_ir_pop(0);
            // POP1 -> POP0
            ret->next->next = ir;
            // POP1 -> POP0 -> BIN
            ret->next->next->next = mips_ir_push(0);
            // POP1 -> POP0 -> BIN -> PUSH0
            ret->next->next->next->next = addPushPop(next);
            // POP1 -> POP0 -> BIN -> PUSH0 -> [next]
            return ret;
        }
        case ir_not: {
            ir_node * next = ir->next;

            // UNOP -> [next]
            // should be
            // POP0 -> UNOP -> PUSH0 -> [next]

            ir_node * ret = mips_ir_pop(0);
            // POP0
            ret->next = ir;
            // POP0 -> UNOP
            ret->next->next = mips_ir_push(0);
            // POP0 -> UNOP -> PUSH0
            ret->next->next->next = addPushPop(next);
            // POP0 -> UNOP -> PUSH0 -> [next]

            return ret;
        }
        case ir_reserve: {
            ir->next = addPushPop(ir->next);
            return ir;
        }
        case ir_address:
        case ir_read:
        case ir_arglocal_read:
        {
            ir_node * next = ir->next;

            // READ -> [next]
            // should be
            // READ -> PUSH -> [next]

            ir->next = mips_ir_push(0);
            ir->next->next = addPushPop(next);

            return ir;
        }
        case ir_arglocal_write:
        case ir_write: {
            // WRITE -> [next]
            // should be
            // POP -> WRITE -> [next]
            ir_node * ret = mips_ir_pop(0);
            ret->next = ir;
            ir->next = addPushPop(ir->next);
            return ret;
        }
        case ir_lbl: {
            ir->next = addPushPop(ir->next);
            return ir;
        }
        case ir_function: {
            ir->next = addPushPop(ir->next);
            return ir;
        }
        case ir_jump: {
            ir->next = addPushPop(ir->next);
            return ir;
        }
        case ir_branchzero: {
            // BRANCH -> [next]
            // should be
            // POP0 -> BRANCH -> [next]

            ir_node * ret = mips_ir_pop(0);
            // POP0
            ret->next = ir;
            // POP0 -> BRANCH
            ir->next = addPushPop(ir->next);
            // POP0 -> BRANCH -> [next]
            return ret;
        }
        case ir_call: {
            ir->next = addPushPop(ir->next);
            return ir;
        }
        case ir_ret: {
            // RET
            // should be
            // POP0 RET
            ir_node * ret = mips_ir_pop(0);
            ret->next = ir;
            ir->next = addPushPop(ir->next);
            return ret;
        }
        case ir_intrinsic: {
            ir_node * i = ir;
            ir = mips_ir_pop(0);
            ir->next = i;
            i->next = addPushPop(i->next);
            break;
        }
        case ir_seq: {
            assert(0);
        }
        case ir_pop: {
            ir->next = addPushPop(ir->next);
            return ir;
        }
        default: {
            assert(0); // Not supported
        }
    }

    return ir;
}

