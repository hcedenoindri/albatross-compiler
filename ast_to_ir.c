#include "ast_to_ir.h"
#include "util.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static ir_node * ast_to_ir_expr(exp_node * e, S_table global_types, S_table function_ret_types, frame * f) {
    UNUSED(global_types);
    UNUSED(function_ret_types);
    UNUSED(f);

    if(!e) return Nop();
    switch(e->kind){
        case int_exp: {
            return Iconst(e->data.ival);
        }
        case string_exp: {
            return Sconst(e->data.sval);
        }
        case binop_exp: {
            switch (e->data.bin_ops.op) {
                case plus_op: {
                    ir_node* ret = Ops(ir_add);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case minus_op: {
                    ir_node* ret = Ops(ir_sub);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case bor_op: {
                    ir_node* ret = Ops(ir_bor);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case ban_op: {
                    ir_node* ret = Ops(ir_band);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case xor_op: {
                    ir_node* ret = Ops(ir_xor);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case times_op: {
                    ir_node* ret = Ops(ir_mul);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case rem_op: {
                    ir_node* ret = Ops(ir_mod);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case div_op: {
                    ir_node* ret = Ops(ir_div);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case lt_op: {
                    ir_node* ret = Ops(ir_lt);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case gt_op: {
                    ir_node* ret = Ops(ir_gt);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case eq_op: {
                    ir_node* ret = Ops(ir_eq);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case or_op: {
                    ir_node* ret = Ops(ir_or);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case and_op: {
                    ir_node* ret = Ops(ir_and);
                    ret->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    ret->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    return ret;
                }
                case ne_op: {
                    ir_node* ret = Ops(ir_not);
                    ir_node* eq = Ops(ir_eq);
                    eq->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    eq->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    ret->tree_ir_1 = eq;
                    return ret;
                }
                case le_op: {
                    ir_node* lt = Ops(ir_lt);
                    lt->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    lt->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    ir_node* eq = Ops(ir_eq);
                    eq->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    eq->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    ir_node* ret = Ops(ir_or);
                    ret->tree_ir_1 = lt;
                    ret->tree_ir_2 = eq;
                    return ret;
                }
                case ge_op: {
                    ir_node* gt = Ops(ir_gt);
                    gt->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    gt->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    ir_node* eq = Ops(ir_eq);
                    eq->tree_ir_1 = ast_to_ir_expr(e->data.bin_ops.e1, global_types, function_ret_types, f);
                    eq->tree_ir_2 = ast_to_ir_expr(e->data.bin_ops.e2, global_types, function_ret_types, f);
                    ir_node* ret = Ops(ir_or);
                    ret->tree_ir_1 = gt;
                    ret->tree_ir_2 = eq;
                    return ret;
                }
                default:
                    assert(0);
            }
        }
        case call_exp: {
            assert(e->data.call_ops.args == NULL);
            ir_node* call = Call(namedLabel(e->data.call_ops.name), 0);
            return Seq(Iconst(0), call);
        }
        case unop_exp: {
            ir_node* ret = Ops(ir_not);
            ret->tree_ir_1 = ast_to_ir_expr(e->data.un_ops.e, global_types, function_ret_types, f);
            return ret;
        }
        case var_exp: {
            return Read(e->data.var_ops.name);
        }
        default:
            assert(0); // Dead code
    }
}

static ir_node * ast_to_ir_stmts(list * l, S_table globals_types, S_table function_rets, frame * f);

static ir_node * ast_to_ir_stmt(stmt_node * s, S_table global_types, S_table function_ret_types, frame * f) {
    UNUSED(global_types);
    UNUSED(function_ret_types);
    UNUSED(f);
    if(!s) return Nop();
    switch(s->kind){
        case assign_stmt: {
            ir_node* exp = ast_to_ir_expr(s->data.assign_ops.rhs, global_types,
                                          function_ret_types, f);
            ir_node* wrt = Write(s->data.assign_ops.lhs);
            wrt->tree_ir_1 = exp;
            return wrt;
        }
        case if_stmt: {
            ir_node* ifCond = ast_to_ir_expr(s->data.if_ops.cond, global_types, function_ret_types, f);
            ir_node* condJump = Branch(freshLabel());
            condJump->tree_ir_1 = ifCond;

            ir_node* thenStmts = ast_to_ir_stmts(s->data.if_ops.then_stmts,
                                                 global_types, function_ret_types, f);
            ir_node* jumpElse = Jump(freshLabel());

            ir_node* elseLabel = Label(condJump->data.lbl);
            ir_node* elseStmts = ast_to_ir_stmts(s->data.if_ops.else_stmts,
                                                 global_types, function_ret_types, f);
            ir_node* endIfLabel = Label(jumpElse->data.lbl);

            ir_node* seq4 = Seq(elseLabel, elseStmts);
            ir_node* seq3 = Seq(seq4, endIfLabel);
            ir_node* seq2 = Seq(thenStmts, jumpElse);
            ir_node* seq1 = Seq(seq2, seq3);

            return Seq(condJump, seq1);
        }
        case while_stmt: {
            assert(s->data.while_ops.otherwise == NULL);

            ir_node* startLabel = Label(freshLabel());
            ir_node* whileCond = ast_to_ir_expr(s->data.while_ops.cond, global_types, function_ret_types, f);
            ir_node* evalCond = Branch(freshLabel());
            ir_node* body = ast_to_ir_stmts(s->data.while_ops.body, global_types, function_ret_types, f);
            ir_node* loopBack = Jump(startLabel->data.lbl);
            ir_node* endLabel = Label(evalCond->data.lbl);

            ir_node* seq5 = Seq(loopBack, endLabel);
            ir_node* seq4 = Seq(body, seq5);
            ir_node* seq3 = Seq(whileCond, evalCond);
            ir_node* seq2 = Seq(seq3, seq4);
            ir_node* seq1 = Seq(startLabel, seq2);

            return seq1;
        }
        case repeat_stmt: {
            assert(0);
        }
        case ret_stmt: {
            assert(f != NULL);
            ir_node* retExp = NULL;
            switch (f->ret->kind) {
                case int_ty:
                    retExp = ast_to_ir_expr(s->data.ret_exp, global_types, function_ret_types, f);
                    break;
                case void_ty:
                    retExp = Iconst(0);
                    break;
                case string_ty:
                    retExp = ast_to_ir_expr(s->data.ret_exp, global_types, function_ret_types, f);
                    break;
                default:
                    assert(0);
            }

            return Seq(retExp, Return());
        }
        case call_stmt: {
            assert(s->data.call_ops.args == NULL);

            ir_node* call = Call(namedLabel(s->data.call_ops.name), 0);

            return Seq(Iconst(0), call);
        }
        case intrinsic_stmt: {
            ir_node* ret = NULL;

            if (!strcmp(s->data.intrinsic_ops.name, "exit"))
                ret = Intrinsic(intrinsic_exit);
            if (!strcmp(s->data.intrinsic_ops.name, "printint"))
                ret = Intrinsic(intrinsic_print_int);
            if (!strcmp(s->data.intrinsic_ops.name, "printstring"))
                ret = Intrinsic(intrinsic_print_string);
            assert(ret != NULL);

            ret->tree_ir_1 = ast_to_ir_expr(s->data.intrinsic_ops.args->head, global_types,
                                            function_ret_types, f);
            return ret;
        }
        default:
            assert(0); // Dead code
    }
}

static ir_node * ast_to_ir_stmts(list * l, S_table globals_types, S_table function_rets, frame * f) {
    if (l == NULL)
        return Nop();

    ir_node * ir1 = ast_to_ir_stmt(l->head, globals_types, function_rets, f);
    ir_node * ir2 = ast_to_ir_stmts(l->next, globals_types, function_rets, f);

    return Seq(ir1, ir2);
}

static ir_node * ast_to_ir_function(fundec_node * fundec, S_table global_types, S_table function_ret_types, frame * f) {
    assert(fundec->locs == NULL);
    assert(fundec->args == NULL);

    ir_node* funcBody = ast_to_ir_stmts(fundec->stmts, global_types, function_ret_types, f);
    return Seq(Function(namedLabel(fundec->name), 0), funcBody);
}

static ir_node * ast_to_ir_functions(list * l, S_table global_types, S_table function_ret_types, S_table frames) {
    if (l == NULL)
        return Nop();
    fundec_node* fundec = (fundec_node*) l->head;
    frame* f = S_look(frames, S_Symbol(fundec->name));
    ir_node *ir1 = ast_to_ir_function( fundec, global_types, function_ret_types, f);
    ir_node *ir2 = ast_to_ir_functions(l->next, global_types, function_ret_types, frames);

    return Seq(ir1, ir2);
}

static ir_node * ast_to_ir_var_decl(vardec_node* node, S_table globals_types, S_table function_rets, frame* f) {
    UNUSED(globals_types);
    UNUSED(function_rets);
    UNUSED(f);
    switch (node->type->kind) {
        case int_ty:
            return Reserve(4, node->name, NULL);
        case string_ty:
            assert(node->init->kind == string_exp);
            ir_node* res = Reserve(4, node->name, NULL);
            Sconst(node->init->data.sval);
            ir_node* wrt = Write(node->name);
            wrt->tree_ir_1 = res;
            return wrt;
        default:
            assert(0);
    }
}

static ir_node * ast_to_ir_vars_decl(list * l, S_table globals_types, S_table function_rets, frame* f) {
    UNUSED(globals_types);
    UNUSED(function_rets);
    UNUSED(f);
    if (l == NULL) return Nop();

    ir_node* seq1 = ast_to_ir_var_decl( (vardec_node*) l->head,
                                         globals_types, function_rets, f);
    ir_node* seq2 = ast_to_ir_vars_decl(l->next, globals_types, function_rets, f);

    return Seq(seq1, seq2);
}

static ir_node * ast_to_ir_var_init(vardec_node* node, S_table globals_types, S_table function_rets, frame* f) {
    UNUSED(globals_types);
    UNUSED(function_rets);
    UNUSED(f);
    ir_node* exp = ast_to_ir_expr(node->init, globals_types,
                                  function_rets, f);
    ir_node* wrt = Write(node->name);
    wrt->tree_ir_1 = exp;
    return wrt;
}

static ir_node * ast_to_ir_vars_init(list * l, S_table globals_types, S_table function_rets, frame* f) {
    UNUSED(globals_types);
    UNUSED(function_rets);
    UNUSED(f);
    if (l == NULL) return Nop();

    ir_node* seq1 = ast_to_ir_var_init( (vardec_node*) l->head,
                                        globals_types, function_rets, f);
    ir_node* seq2 = ast_to_ir_vars_init(l->next, globals_types, function_rets, f);

    return Seq(seq1, seq2);
}

static ir_node * ast_to_ir_vars (list * l, S_table global_types, S_table function_ret_types, frame * f) {
    UNUSED(global_types);
    UNUSED(function_ret_types);
    UNUSED(f);
    if (l == NULL) return Nop();

    ir_node* seq1 = ast_to_ir_vars_decl(l, global_types, function_ret_types, f);
    ir_node* seq2 = ast_to_ir_vars_init(l, global_types, function_ret_types, f);

    return Seq(seq1, seq2);
}

ir_node * ast_to_ir(program* p, S_table globals_types, S_table functions_ret, S_table frames) {
    UNUSED(ast_to_ir_expr);

    ir_node * vars = ast_to_ir_vars(p->variables, globals_types, functions_ret, NULL);

    ir_node * functions = ast_to_ir_functions(p->functions, globals_types,
                                              functions_ret, frames);

    ir_node * mainLabel = Label(namedLabel("_main"));

    ir_node * stmts = ast_to_ir_stmts(p->statements, globals_types, functions_ret, NULL);

    ir_node* seq3 = Seq(mainLabel, stmts);
    ir_node* seq2 = Seq(Jump(mainLabel->data.lbl), functions);
    ir_node* seq1 = Seq(vars, seq2);

    return Seq(seq1, seq3);

}
