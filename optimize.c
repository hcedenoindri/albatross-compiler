#include "optimize.h"
#include "util.h"
#include "assert.h"
#include <stdlib.h>

void optimize_ast(program * p, S_table globals_types, S_table function_rets, S_table frames) {
    UNUSED(p);
    UNUSED(globals_types);
    UNUSED(function_rets);
    UNUSED(frames);

}

ir_node * optimize_tree_ir(ir_node * ir) {
    return ir;
}

ir_node * optimize_list_ir(ir_node * ir) {
//    if (ir->next == NULL) return ir;
//
//    ir_node *n1, *n2, *op;
//
//    if (ir->kind == ir_iconst) {
//        n1 = ir;
//        if (ir->next != NULL) {
//            if (ir->next->kind == ir_not) {
//                op = n1->next;
//                n1->kind = ir_iconst;
//                n1->data.iconst = !n1->data.iconst;
//                n1->next = op->next;
//            }
//            if (ir->next->kind == ir_iconst) {
//                n2 = n1->next;
//                if (ir->next->next != NULL) {
//                    op = n2->next;
//                    n1->kind = ir_iconst;
//                    switch (ir->next->next->kind) {
//                        case ir_add:
//                            n1->data.iconst = n1->data.iconst + n2->data.iconst;
//                            break;
//                        case ir_sub:
//                            n1->data.iconst = n1->data.iconst - n2->data.iconst;
//                            break;
//                        case ir_bor:
//                            n1->data.iconst = n1->data.iconst | n2->data.iconst;
//                            break;
//                        case ir_band:
//                            n1->data.iconst = n1->data.iconst & n2->data.iconst;
//                            break;
//                        case ir_xor:
//                            n1->data.iconst = n1->data.iconst ^ n2->data.iconst;
//                            break;
//                        case ir_mul:
//                            n1->data.iconst = n1->data.iconst * n2->data.iconst;
//                            break;
//                        case ir_mod:
//                            n1->data.iconst = n1->data.iconst % n2->data.iconst;
//                            break;
//                        case ir_div:
//                            n1->data.iconst = n1->data.iconst / n2->data.iconst;
//                            break;
//                        case ir_lt:
//                            n1->data.iconst = n1->data.iconst < n2->data.iconst;
//                            break;
//                        case ir_gt:
//                            n1->data.iconst = n1->data.iconst > n2->data.iconst;
//                            break;
//                        case ir_eq:
//                            n1->data.iconst = n1->data.iconst == n2->data.iconst;
//                            break;
//                        case ir_or:
//                            n1->data.iconst = n1->data.iconst || n2->data.iconst;
//                            break;
//                        case ir_and:
//                            n1->data.iconst = n1->data.iconst && n2->data.iconst;
//                            break;
//                        default:
//                            assert(0);
//                    }
//                    n1->next = op->next;
//                    free(n2);
//                    free(op);
//                }
//            }
//        }
//    }
//
//    ir->next = optimize_list_ir(ir->next);
//    return ir;
    return ir;
}

ir_node * optimize_list_explicitPushPop_ir(ir_node * ir) {
    return ir;
}
