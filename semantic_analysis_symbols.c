#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "util.h"
#include "semantic_analysis_symbols.h"

void symbolResolutionExpr(exp_node * e, S_table global_types, S_table function_rets, frame * f) {
    UNUSED(global_types);
    UNUSED(function_rets);
    UNUSED(f);

    if(!e) return;
    switch(e->kind){
        case int_exp: {
            break;
        }
        case string_exp: {
            break;
        }
        case binop_exp: {
            symbolResolutionExpr(e->data.bin_ops.e1, global_types, function_rets, f);
            symbolResolutionExpr(e->data.bin_ops.e2, global_types, function_rets, f);
            break;
        }
        case call_exp: {
            fundec_node* fundec = (fundec_node*) S_look(function_rets, S_Symbol(e->data.call_ops.name));
            if (fundec == NULL) {
                fprintf(stderr, "Function called without being declared %s\n", e->data.call_ops.name);
                exit(3);
            }

            list* i = e->data.call_ops.args;
            while (i != NULL) {
                exp_node* enode = (exp_node*) i->head;
                symbolResolutionExpr(enode, global_types, function_rets, f);
                i = i->next;
            }
            break;
        }
        case unop_exp: {
            symbolResolutionExpr(e->data.un_ops.e, global_types, function_rets, f);
            break;
        }
        case var_exp: {
            ty_node* type = (ty_node*) S_look(global_types, S_Symbol(e->data.var_ops.name));
            if (type == NULL) {
                if (f == NULL) {
                    fprintf(stderr, "Variable used without being declared: %s. \n", e->data.var_ops.name);
                    exit(3);
                }
                type = (ty_node*) S_look(f->args_locs_types, S_Symbol(e->data.var_ops.name));
                if (type == NULL) {
                    fprintf(stderr, "Variable used without being declared: %s. \n", e->data.var_ops.name);
                    exit(3);
                }
            }
            break;
        }
        default:
            assert(0); // Dead code
    }
}

void symbolResolutionStmts(list * l, S_table locals, S_table function_rets, frame * f);

void symbolResolutionStmt(stmt_node * s, S_table global_types, S_table function_rets, frame * f) {
    UNUSED(global_types);
    UNUSED(function_rets);
    UNUSED(f);
    if(!s) return;
    switch(s->kind){
        case assign_stmt: {
            ty_node* type = (ty_node*) S_look(global_types, S_Symbol(s->data.assign_ops.lhs));
            if (type == NULL) {
                if (f == NULL) {
                    fprintf(stderr, "Variable written without being declared: %s. \n", s->data.assign_ops.lhs);
                    exit(3);
                }
                type = (ty_node*) S_look(f->args_locs_types, S_Symbol(s->data.assign_ops.lhs));
                if (type == NULL) {
                    fprintf(stderr, "Variable written without being declared: %s. \n", s->data.assign_ops.lhs);
                    exit(3);
                }
            }
            symbolResolutionExpr(s->data.assign_ops.rhs, global_types, function_rets, f);
            break;
        }
        case if_stmt: {
            symbolResolutionExpr(s->data.if_ops.cond, global_types, function_rets, f);
            symbolResolutionStmts(s->data.if_ops.then_stmts, global_types, function_rets, f);
            symbolResolutionStmts(s->data.if_ops.else_stmts, global_types, function_rets, f);
            break;
        }
        case while_stmt: {
            symbolResolutionExpr(s->data.while_ops.cond, global_types, function_rets, f);
            symbolResolutionStmts(s->data.while_ops.body, global_types, function_rets, f);
            symbolResolutionStmts(s->data.while_ops.otherwise, global_types, function_rets, f);
            break;
        }
        case repeat_stmt: {
            symbolResolutionExpr(s->data.repeat_ops.cond, global_types, function_rets, f);
            symbolResolutionStmts(s->data.repeat_ops.body, global_types, function_rets, f);
            break;
        }
        case ret_stmt: {
            symbolResolutionExpr(s->data.ret_exp, global_types, function_rets, f);
            break;
        }
        case call_stmt: {
            fundec_node* fundec = (fundec_node*) S_look(function_rets, S_Symbol(s->data.call_ops.name));
            if (fundec == NULL) {
                fprintf(stderr, "Function called without being declared %s\n", s->data.call_ops.name);
                exit(3);
            }

            list* i = s->data.call_ops.args;
            while (i != NULL) {
                exp_node* enode = (exp_node*) i->head;
                symbolResolutionExpr(enode, global_types, function_rets, f);
                i = i->next;
            }
            break;
        }
        default:
            assert(0); // Dead code
    }
}

void symbolResolutionStmts(list * l, S_table globals_types, S_table function_rets, frame * f) {
    if (l == NULL) return;
    symbolResolutionStmt(l->head, globals_types, function_rets, f);
    symbolResolutionStmts(l->next, globals_types, function_rets, f);
}

void symbolResolutionVariable(vardec_node * vardec, S_table globals_types, S_table function_rets, frame * f) {
    assert(f == NULL);

    char* varname = vardec->name;
    ty_node* vartype = vardec->type;

    if (S_look(globals_types, S_Symbol(varname)) != NULL) {
        fprintf(stderr, "Variable defined with already used name: %s", varname);
        exit(3);
    }
    symbolResolutionExpr(vardec->init, globals_types, function_rets, f);
    S_enter(globals_types, S_Symbol(varname), vartype);

    UNUSED(vardec);
    UNUSED(globals_types);
    UNUSED(function_rets);
    UNUSED(f);
}

void symbolResolutionVariables(list * l, S_table global_types, S_table function_rets, frame * f) {
    if (l == NULL) return;
    symbolResolutionVariable((vardec_node *)l->head, global_types, function_rets, f);
    symbolResolutionVariables(l->next, global_types, function_rets, f);
}

void symbolResolutionFunction(fundec_node * fundec, S_table globals, S_table functions_rets, frame * f) {
    symbolResolutionStmts(fundec->stmts, globals, functions_rets, f);

    UNUSED(fundec);
    UNUSED(globals);
    UNUSED(functions_rets);
    UNUSED(f);
}

void symbolResolutionFunctions(list * l, S_table global_types, S_table function_ret_types, S_table frames) {
    if (l == NULL) return;
    fundec_node * fundec = (fundec_node*) l->head;

    if (S_look(function_ret_types, S_Symbol(fundec->name)) != NULL) {
        fprintf(stderr, "Duplicate function name: %s\n", fundec->name);
        exit(3);
    }

    S_enter(function_ret_types, S_Symbol(fundec->name), fundec);

    // Create a new frame
    frame * f = malloc(sizeof(frame));

    // Initialize all memory so we can read it later
    f->args_locs_types = S_empty();
    f->indexes = S_empty();
    f->ret = fundec->type;

    list* i = fundec->args;
    while (i != NULL) {
        param* arg = (param*) i->head;

        if (S_look(global_types, S_Symbol(arg->name)) != NULL) {
            fprintf(stderr, "function-declared argument has same name as global variable: %s\n",
                    arg->name);
            exit(3);
        }
        if (S_look(f->args_locs_types, S_Symbol(arg->name)) != NULL) {
            fprintf(stderr, "Function-declared duplicate argument: %s\n",
                    arg->name);
            exit(3);
        }

        S_enter(f->args_locs_types, S_Symbol(arg->name), arg->ty);
        i = i->next;
    }

    i = fundec->locs;
    while (i != NULL) {
        vardec_node* vardec = (vardec_node*) i->head;

        if (S_look(global_types, S_Symbol(vardec->name)) != NULL) {
            fprintf(stderr, "function-declared local variable has same name as global variable: %s\n",
                    vardec->name);
            exit(3);
        }
        if (S_look(f->args_locs_types, S_Symbol(vardec->name)) != NULL) {
            fprintf(stderr, "Function-declared duplicate local variable: %s\n",
                    vardec->name);
            exit(3);
        }

        S_enter(f->args_locs_types, S_Symbol(vardec->name), vardec->type);
        i = i->next;
    }

    // Add newly computed frame to frames
    S_enter(frames, S_Symbol(fundec->name), f);

    symbolResolutionFunction(fundec, global_types, function_ret_types, f);
    symbolResolutionFunctions(l->next, global_types, function_ret_types, frames);
}

static void registerIntrinsics(S_table function_rets) {
    fundec_node* f;

    f = FunDecNode("exit", VoidTyNode(), ListAddFirst(Param(IntTyNode(), "n"), NULL), NULL, NULL);
    S_enter(function_rets, S_Symbol("exit"), f);

    f = FunDecNode("printint", VoidTyNode(), ListAddFirst(Param(IntTyNode(), "n"), NULL), NULL, NULL);
    S_enter(function_rets, S_Symbol("printint"), f);

    f = FunDecNode("printstring", VoidTyNode(), ListAddFirst(Param(StringTyNode(), "n"), NULL), NULL, NULL);
    S_enter(function_rets, S_Symbol("printstring"), f);
}

void symbolResolution(program * p, S_table global_types, S_table function_rets, S_table frames) {
    registerIntrinsics(function_rets);
    symbolResolutionVariables(p->variables, global_types, function_rets, NULL);
    symbolResolutionFunctions(p->functions, global_types, function_rets, frames);
    symbolResolutionStmts(p->statements, global_types, function_rets, NULL);
}
