#define _POSIX_C_SOURCE 200809L // enable strdup
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "transform.h"
#include "util.h"

extern program p;

static char * generateFreshGlobalVar() {
    static int lastVar = 0;
    static char buffer[1024];

    snprintf(buffer, sizeof(buffer), "_var%d", lastVar);
    lastVar += 1;
    return strdup(buffer);
}


void transformExpr(exp_node * e, S_table global_types, S_table function_rets, frame * f) {
    UNUSED(global_types);
    UNUSED(function_rets);
    UNUSED(f);
    if(!e) return;
    switch(e->kind){
        case int_exp: {
            break;
        }
        case string_exp: {
            // Create a fresh global variable of type string
            char * name = generateFreshGlobalVar();
            // Initialize that global variable with the contents of the string
            p.variables = ListAddFirst(
                    VarDecNode(name, StringTyNode(), StringNode(e->data.sval)),
                    p.variables);
            // Register the new global variable in the right table
            S_enter(global_types, S_Symbol(name), StringTyNode());
            // Replace the string_exp with a var_exp for that fresh global variable
            e->kind = var_exp;
            e->data.var_ops.name = name;
            break;
        }
        case binop_exp: {
            break;
        }
        case call_exp: {
            break;
        }
        case unop_exp: {
            break;
        }
        case var_exp: {
            break;
        }
        default:
            assert(0);
    }
}

void transformStmts(list * l, S_table globals_types, S_table function_rets, frame * f) {
    UNUSED(globals_types);
    UNUSED(function_rets);
    UNUSED(f);
    if (l == NULL) return;
    stmt_node * s = l->head;
    if(s) {
        switch(s->kind){
            case assign_stmt: {
                break;
            }
            case if_stmt: {
                transformExpr(s->data.if_ops.cond, globals_types, function_rets, f);
                transformStmts(s->data.if_ops.then_stmts, globals_types, function_rets, f);
                transformStmts(s->data.if_ops.else_stmts, globals_types, function_rets, f);
                break;
            }
            case while_stmt: {
                transformExpr(s->data.while_ops.cond, globals_types, function_rets, f);
                transformStmts(s->data.while_ops.body, globals_types, function_rets, f);
                transformStmts(s->data.while_ops.otherwise, globals_types, function_rets, f);

                exp_node* cond = s->data.while_ops.cond;
                list* body  = s->data.while_ops.body;
                list* other = s->data.while_ops.otherwise;

                s->kind = if_stmt;
                s->data.if_ops.then_stmts = ListCopy(body);
                s->data.if_ops.then_stmts = ListAddLast(
                        WhileNode(cond, body, NULL),
                        s->data.if_ops.then_stmts);
                s->data.if_ops.else_stmts = other;

                break;
            }
            case repeat_stmt: {
                transformExpr(s->data.repeat_ops.cond, globals_types, function_rets, f);
                // Create a fresh global variable of type int
                char* name = generateFreshGlobalVar();
                // Initialize that global variable with zero
                p.variables = ListAddFirst(
                        VarDecNode(name, IntTyNode(), IntNode(0)),
                        p.variables);
                // Register the new global variable in the right table
                S_enter(globals_types, S_Symbol(name), IntTyNode());

                exp_node* init = s->data.repeat_ops.cond;
                list* body = s->data.repeat_ops.body;
                list* next = l->next;

                s->kind = assign_stmt;
                s->data.assign_ops.lhs = name;
                s->data.assign_ops.rhs = init;

                stmt_node* whl = WhileNode(
                        BinOpNode(gt_op, VarOpNode(name), IntNode(0)),
                        ListAddLast(
                                AssignNode(name,
                                           BinOpNode(
                                                   minus_op,
                                                   VarOpNode(name),
                                                   IntNode(1))),
                                body),
                        NULL);
                l->next = ListAddFirst(whl, next);
                break;
            }
            case ret_stmt: {
                transformExpr(s->data.ret_exp, globals_types, function_rets, f);
                if (f == NULL) {
                    // Top-level code, replace with exit()
                    exp_node* ret = s->data.ret_exp;
                    s->kind = intrinsic_stmt;
                    s->data.intrinsic_ops.name = "exit";
                    s->data.intrinsic_ops.args = ListAddFirst(ret, NULL);
                }
                break;
            }
            case call_stmt: {
                list* li = s->data.call_ops.args;
                while (li != NULL) {
                    transformExpr(li->head, globals_types, function_rets, f);
                    li = li->next;
                }
                if (!strcmp("exit", s->data.call_ops.name)) {
                    char* name = s->data.call_ops.name;
                    list* args = s->data.call_ops.args;
                    s->kind = intrinsic_stmt;
                    s->data.intrinsic_ops.name = name;
                    s->data.intrinsic_ops.args = args;
                    break;
                }
                if (!strcmp("printint", s->data.call_ops.name)) {
                    char* name = s->data.call_ops.name;
                    list* args = s->data.call_ops.args;
                    s->kind = intrinsic_stmt;
                    s->data.intrinsic_ops.name = name;
                    s->data.intrinsic_ops.args = args;
                    break;
                }
                if (!strcmp("printstring", s->data.call_ops.name)) {
                    char* name = s->data.call_ops.name;
                    list* args = s->data.call_ops.args;
                    s->kind = intrinsic_stmt;
                    s->data.intrinsic_ops.name = name;
                    s->data.intrinsic_ops.args = args;
                    break;
                }
                break;
            }
            default:
                assert(0);
        }
    }

    transformStmts(l->next, globals_types, function_rets, f);
}

void transformVariable(vardec_node * node, S_table globals_types, S_table function_rets, frame * f) {
    UNUSED(node); UNUSED(globals_types); UNUSED(function_rets); UNUSED(f);
//    transformExpr(node->init, globals_types, function_rets, f);
}

void transformVariables(list * l, S_table global_types, S_table function_rets, frame * f) {
    if (l == NULL) return;
    transformVariable((vardec_node *)l->head, global_types, function_rets, f);
    transformVariables(l->next, global_types, function_rets, f);
}

void transformFunction(fundec_node * fundec, S_table globals, S_table functions_rets, frame * f) {
    UNUSED(fundec);
    transformStmts(fundec->stmts, globals, functions_rets, f);
    if (fundec->type->kind == void_ty)
        ListAddLast(RetNode(NULL), fundec->stmts);
}

void transformFunctions(list * l, S_table global_types, S_table function_ret_types, S_table frames) {
    if (l == NULL) return;
    fundec_node * fundec = (fundec_node*) l->head;
    frame * f = S_look(frames, S_Symbol(fundec->name));
    transformFunction(fundec, global_types, function_ret_types, f);
    transformFunctions(l->next, global_types, function_ret_types, frames);
}

void transform(program * p, S_table global_types, S_table function_rets, S_table frames) {

    transformVariables(p->variables, global_types, function_rets, NULL);
    transformFunctions(p->functions, global_types, function_rets, frames);
    transformStmts(p->statements, global_types, function_rets, NULL);
}
