#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include "ast.h"
#include "mips_ast.h"
#include "util.h"

static FILE * out;

// Push to register v0
static void push0();

// Push to register v1
static void push1();

// Pop to register v0
static void pop0();

// Pop to register v1
static void pop1();

static void emitInstruction(char * instruction, char * comment, ...);

static void emitLabel(char * label, char * comment, ...);

void mips_astExpr(exp_node * e, S_table global_types, S_table function_rets, frame * f) {
    UNUSED(global_types);
    UNUSED(function_rets);
    UNUSED(f);
    if(!e) return;
    switch(e->kind){
        case int_exp: {
            assert(0);
        }
        case string_exp: {
            assert(0);
        }
        case binop_exp: {
            assert(0);
        }
        case call_exp: {
            assert(0);
        }
        case unop_exp: {
            assert(0);
        }
        case var_exp: {
            assert(0);
        }
        case intrinsic_exp: {
            assert(0);
        }
        default:
            assert(0);
    }
}

static char * generateFreshLabel() {
    static int lastLabel = 0;

    char * ret = malloc(5*sizeof(char));
    snprintf(ret, sizeof(5*sizeof(char)), "L%d", lastLabel);
    lastLabel += 1;
    return ret;
}

void mips_astStmts(list * l, S_table globals, S_table function_rets, frame * f);

void mips_astStmt(stmt_node * s, S_table global_types, S_table function_rets, frame * f) {
    UNUSED(global_types);
    UNUSED(function_rets);
    UNUSED(f);
    if(!s) return;
    switch(s->kind){
        case assign_stmt: {
            assert(0);
        }
        case if_stmt: {
            assert(0);
        }
        case while_stmt: {
            assert(0);
        }
        case repeat_stmt: {
            assert(0);
        }
        case ret_stmt: {
            assert(0);
        }
        case call_stmt: {
            assert(0);
        }
        case intrinsic_stmt: {
             assert(0);
         }
        default:
            assert(0);
    }
}

void mips_astStmts(list * l, S_table globals_types, S_table function_rets, frame * f) {
    if (l == NULL) return;
    mips_astStmt(l->head, globals_types, function_rets, f);
    mips_astStmts(l->next, globals_types, function_rets, f);
}

void mips_astVariable(vardec_node * node, S_table globals_types, S_table function_rets, frame * f) {
    mips_astExpr(node->init, globals_types, function_rets, f);
}

void mips_astVariables(list * l, S_table global_types, S_table function_rets, frame * f) {
    if (l == NULL) return;

    fprintf(out, ".data\n");
    mips_astVariable((vardec_node *)l->head, global_types, function_rets, f);
    mips_astVariables(l->next, global_types, function_rets, f);
}

void mips_astFunction(fundec_node * fundec, S_table globals, S_table functions_rets, frame * f) {
    UNUSED(fundec);
    mips_astStmts(fundec->stmts, globals, functions_rets, f);
}

void mips_astFunctions(list * l, S_table global_types, S_table function_ret_types, S_table frames) {
    if (l == NULL) return;
    fundec_node * fundec = (fundec_node*) l->head;
    frame * f = S_look(frames, S_Symbol(fundec->name));
    mips_astFunction(fundec, global_types, function_ret_types, f);
    mips_astFunctions(l->next, global_types, function_ret_types, frames);
}

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

static void push0() {
    // Push register v0
    assert(0);
}

static void push1() {
    // Push register v1
    assert(0);
}

static void pop0() {
    // Pop to register v0
    assert(0);
}

static void pop1() {
    // Pop to register v1
    assert(0);
}

// Used for code and variables outside of functions
static frame global_empty_frame;

void mips_ast(program * p, S_table global_types, S_table function_rets, S_table frames, FILE * o) {
    out = o;

    UNUSED(generateFreshLabel);
    UNUSED(emitInstruction);
    UNUSED(emitLabel);
    UNUSED(push0);
    UNUSED(push1);
    UNUSED(pop0);
    UNUSED(pop1);

    global_empty_frame.args_locs_types = S_empty();
    global_empty_frame.indexes = S_empty();

    mips_astVariables(p->variables, global_types, function_rets, &global_empty_frame);
    mips_astFunctions(p->functions, global_types, function_rets, frames);
    mips_astStmts(p->statements, global_types, function_rets, &global_empty_frame);
}
