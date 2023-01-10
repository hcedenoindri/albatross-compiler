#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "util.h"
#include "semantic_analysis_types.h"

ty_node * typeCheckExpr(exp_node * e, S_table global_types, S_table function_rets, frame * f) {
    UNUSED(global_types);
    UNUSED(function_rets);
    UNUSED(f);

    switch(e->kind){
        case int_exp: {
            return IntTyNode();
        }
        case string_exp: {
            return StringTyNode();
        }
        case binop_exp: {
            ty_node* lhs = typeCheckExpr(e->data.bin_ops.e1, global_types, function_rets, f);
            ty_node* rhs = typeCheckExpr(e->data.bin_ops.e2, global_types, function_rets, f);
            ty_node* exp = IntTyNode();

            if (lhs->kind != exp->kind || rhs->kind != exp->kind) {
                fprintf(stderr, "Type error when performing binary operation.\n");
                exit(3);
            }

            return IntTyNode();
        }
        case call_exp: {
            fundec_node* fundec = (fundec_node*) S_look(function_rets, S_Symbol(e->data.call_ops.name));
            assert(fundec != NULL);

            list* formalArgs = fundec->args;
            list* actualArgs = e->data.call_ops.args;
            int i = 1;
            while (formalArgs != NULL && actualArgs != NULL) {
                param* formalArg = (param*) formalArgs->head;
                exp_node* actualArg = (exp_node*) actualArgs->head;

                ty_node* expected = formalArg->ty;
                ty_node* actual   = typeCheckExpr(actualArg, global_types, function_rets, f);

                if (expected->kind != actual->kind) {
                    fprintf(stderr, "Type error when calling function %s on argument %d: expected %s got %s\n",
                            fundec->name, i, typeToStr(expected), typeToStr(actual));
                    exit(3);
                }

                formalArgs = formalArgs->next;
                actualArgs = actualArgs->next;
                i += 1;
            }

            if (formalArgs != NULL || actualArgs != NULL) {
                fprintf(stderr, "Function %s called with too many/few arguments.\n", fundec->name);
                exit(3);
            }

            return fundec->type;
        }
        case unop_exp: {
            ty_node* lhs = typeCheckExpr(e->data.un_ops.e, global_types, function_rets, f);
            ty_node* exp = IntTyNode();

            if (lhs->kind != exp->kind) {
                fprintf(stderr, "Type error when performing unary operation.\n");
                exit(3);
            }

            return IntTyNode();
        }
        case var_exp: {
            ty_node* expected = (ty_node*) S_look(global_types, S_Symbol(e->data.var_ops.name));
            if (expected == NULL) {
                assert(f != NULL);
                expected = (ty_node*) S_look(f->args_locs_types, S_Symbol(e->data.var_ops.name));
            }
            assert(expected != NULL);
            return expected;
        }
        default:
            assert(0); // Dead code
    }
}

void typeCheckStmts(list * l, S_table locals, S_table function_rets, frame * f);

void typeCheckStmt(stmt_node * s, S_table global_types, S_table function_rets, frame * f) {
    UNUSED(global_types);
    UNUSED(function_rets);
    UNUSED(f);
    if(!s) return;
    switch(s->kind){
        case assign_stmt: {
            ty_node* declared = (ty_node*) S_look(global_types, S_Symbol(s->data.assign_ops.lhs));
            if (declared == NULL) {
                assert(f != NULL);
                declared = (ty_node*) S_look(f->args_locs_types, S_Symbol(s->data.assign_ops.lhs));
            }
            assert(declared != NULL);

            ty_node* assignment = typeCheckExpr(s->data.assign_ops.rhs, global_types, function_rets, f);

            if (declared->kind != assignment->kind) {
                fprintf(stderr, "Type error on variable %s assignment, expected %s got %s.\n",
                        s->data.assign_ops.lhs, typeToStr(declared), typeToStr(assignment));
                exit(3);
            }
            break;
        }
        case if_stmt: {
            ty_node* type = typeCheckExpr(s->data.if_ops.cond, global_types, function_rets, f);
            ty_node* expe = IntTyNode();
            if (type->kind != expe->kind) {
                fprintf(stderr, "Type error when returning from top level code, expected %s got %s.\n",
                        typeToStr(expe), typeToStr(type));
                exit(3);
            }
            typeCheckStmts(s->data.if_ops.then_stmts, global_types, function_rets, f);
            typeCheckStmts(s->data.if_ops.else_stmts, global_types, function_rets, f);
            break;
        }
        case while_stmt: {
            ty_node* type = typeCheckExpr(s->data.while_ops.cond, global_types, function_rets, f);
            ty_node* expe = IntTyNode();
            if (type->kind != expe->kind) {
                fprintf(stderr, "Type error when returning from top level code, expected %s got %s.\n",
                        typeToStr(expe), typeToStr(type));
                exit(3);
            }
            typeCheckStmts(s->data.while_ops.body, global_types, function_rets, f);
            typeCheckStmts(s->data.while_ops.otherwise, global_types, function_rets, f);
            break;
        }
        case repeat_stmt: {
            ty_node* type = typeCheckExpr(s->data.repeat_ops.cond, global_types, function_rets, f);
            ty_node* expe = IntTyNode();
            if (type->kind != expe->kind) {
                fprintf(stderr, "Type error when returning from top level code, expected %s got %s.\n",
                        typeToStr(expe), typeToStr(type));
                exit(3);
            }
            typeCheckStmts(s->data.repeat_ops.body, global_types, function_rets, f);
            break;
        }
        case ret_stmt: {
            ty_node* type = VoidTyNode();
            ty_node* expe = (f == NULL) ? IntTyNode() : f->ret;

            if (s->data.ret_exp != NULL)
                type = typeCheckExpr(s->data.ret_exp, global_types, function_rets, f);

            if (type->kind != expe->kind) {
                if (f != NULL) {
                    fprintf(stderr, "Type error when returning from function, expected %s got %s\n",
                            typeToStr(expe), typeToStr(type));
                    exit(3);
                }
                fprintf(stderr, "Type error when returning from top level code, expected %s got %s.\n",
                        typeToStr(expe), typeToStr(type));
                exit(3);
            }
            break;
        }
        case call_stmt: {
            fundec_node* fundec = (fundec_node*) S_look(function_rets, S_Symbol(s->data.call_ops.name));
            assert(fundec != NULL);

            list* formalArgs = fundec->args;
            list* actualArgs = s->data.call_ops.args;
            int i = 1;
            while (formalArgs != NULL && actualArgs != NULL) {
                param* formalArg = (param*) formalArgs->head;
                exp_node* actualArg = (exp_node*) actualArgs->head;

                ty_node* expected = formalArg->ty;
                ty_node* actual   = typeCheckExpr(actualArg, global_types, function_rets, f);

                if (expected->kind != actual->kind) {
                    fprintf(stderr, "Type error when calling function %s on argument %d: expected %s got %s\n",
                            fundec->name, i, typeToStr(expected), typeToStr(actual));
                    exit(3);
                }

                formalArgs = formalArgs->next;
                actualArgs = actualArgs->next;
                i += 1;
            }

            if (formalArgs != NULL || actualArgs != NULL) {
                fprintf(stderr, "Function %s called with too many/few arguments.\n", fundec->name);
                exit(3);
            }

            break;
        }
        default:
            assert(0); // Dead code
    }
}

void typeCheckStmts(list * l, S_table globals_types, S_table function_rets, frame * f) {
    if (l == NULL) return;
    typeCheckStmt(l->head, globals_types, function_rets, f);
    typeCheckStmts(l->next, globals_types, function_rets, f);
}

void typeCheckVariable(vardec_node * vardec, S_table globals_types, S_table function_rets, frame * f) {
    ty_node* declared = vardec->type;
    ty_node* actual = typeCheckExpr(vardec->init, globals_types, function_rets, f);

    if (declared->kind != actual->kind) {
        fprintf(stderr, "Type error when initializing variable, expected %s got %s.\n",
                typeToStr(declared), typeToStr(actual));
        exit(3);
    }
}

void typeCheckVariables(list * l, S_table global_types, S_table function_rets, frame * f) {
    if (l == NULL) return;
    typeCheckVariable((vardec_node *)l->head, global_types, function_rets, f);
    typeCheckVariables(l->next, global_types, function_rets, f);
}

void typeCheckFunction(fundec_node * fundec, S_table globals_types, S_table functions_rets, frame * f) {
    typeCheckStmts(fundec->stmts, globals_types, functions_rets, f);

    list* i = fundec->locs;
    while (i != NULL) {
        vardec_node* vardec = (vardec_node*) i->head;
        typeCheckVariable(vardec, globals_types, functions_rets, f);
        i = i->next;
    }
}

void typeCheckFunctions(list * l, S_table global_types, S_table function_ret_types, S_table frames) {
    if (l == NULL) return;
    fundec_node * fundec = (fundec_node*) l->head;
    frame * f = S_look(frames, S_Symbol(fundec->name));
    assert(f);
    typeCheckFunction(fundec, global_types, function_ret_types, f);
    typeCheckFunctions(l->next, global_types, function_ret_types, frames);
}

void typeCheck(program * p, S_table global_types, S_table function_rets, S_table frames) {
    typeCheckVariables(p->variables, global_types, function_rets, NULL);
    typeCheckFunctions(p->functions, global_types, function_rets, frames);
    typeCheckStmts(p->statements, global_types, function_rets, NULL);
}
