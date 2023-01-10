#include <stdio.h>
#include <assert.h>
#include "ast.h"
#include "frames.h"
#include "util.h"

void framesFunction(fundec_node * fundec, S_table globals, S_table functions_rets, S_table frames) {
    // Get frame created during symbol resolution
    frame * f = S_look(frames, S_Symbol(fundec->name));
    assert(f != NULL);

    long n = 0;
    list* i = fundec->args;
    while (i != NULL) {
        param* p = (param*) i->head;
        S_enter(f->indexes, S_Symbol(p->name), (void*) n);

        n += 1;
        i = i->next;
    }

    i = fundec->locs;
    while (i != NULL) {
        vardec_node* v = (vardec_node*) i->head;
        S_enter(f->indexes, S_Symbol(v->name), (void*) n);

        n += 1;
        i = i->next;
    }

    UNUSED(globals);
    UNUSED(functions_rets);
    UNUSED(f);
}

void framesFunctions(list * l, S_table global_types, S_table function_ret_types, S_table frames) {
    if (l == NULL) return;
    fundec_node * fundec = (fundec_node*) l->head;
    framesFunction(fundec, global_types, function_ret_types, frames);
    framesFunctions(l->next, global_types, function_ret_types, frames);
}

void frames(program * p, S_table global_types, S_table function_rets, S_table frames) {
    framesFunctions(p->functions, global_types, function_rets, frames);
}