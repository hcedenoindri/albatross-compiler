#include "ir_tree_to_list.h"

#include <assert.h>
#include <stdlib.h>

static ir_node* findLast (ir_node* ir) {
    while (ir->next != NULL)
        ir = ir->next;
    return ir;
}

ir_node * ir_tree_to_list(ir_node * ir) {
    if (ir == NULL) {
        return Nop();
    }

    switch (ir->kind) {
        case ir_nop: {
            return ir;
        }
        case ir_iconst: {
            return ir;
        }
        case ir_sconst: {
            return ir;
        }
        case ir_add: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_sub: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_mul: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_div: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_mod: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_bor: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_band: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_xor: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_or: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_and: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_eq: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_lt: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_gt: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_not: {
            ir_node* ret;
            ret = ir_tree_to_list(ir->tree_ir_1);
            assert(ret != NULL);
            findLast(ret)->next = ir;
            return ret;
        }
        case ir_reserve: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_read: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_write: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_lbl: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_jump: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_branchzero: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_function: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_call: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_ret: {
            ir_node* left  = ir_tree_to_list(ir->tree_ir_1);
            ir_node* right = ir_tree_to_list(ir->tree_ir_2);
            findLast(left)->next = right;
            findLast(right)->next = ir;
            return left;
        }
        case ir_intrinsic: {
            ir_node* ret;
            ret = ir_tree_to_list(ir->tree_ir_1);
            assert(ret != NULL);
            findLast(ret)->next = ir;
            return ret;
//            switch (ir->data.intrinsic) {
//                case intrinsic_exit:
//                    ret = ir_tree_to_list(ir->tree_ir_1);
//                    findLast(ret)->next = ir;
//                    return ret;
//                case intrinsic_print_int:
//                    ret = ir_tree_to_list(ir->tree_ir_1);
//                    findLast(ret)->next = ir;
//                    return ret;
//                default:
//                    assert(0);
//            }
        }
        case ir_seq: {
            ir_node* fst = ir_tree_to_list(ir->data.seq.o1);
            ir_node* snd = ir_tree_to_list(ir->data.seq.o2);
            free(ir);
            findLast(fst)->next = snd;
            return fst;
        }
        case ir_pop: {
            ir_node* fst = ir_tree_to_list(ir->data.seq.o1);
            ir_node* snd = ir_tree_to_list(ir->data.seq.o2);
            free(ir);
            findLast(fst)->next = snd;
            return fst;
        }
        default: {
            assert(0); // Not supported
        }
    }
}