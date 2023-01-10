#define _POSIX_C_SOURCE 200809L // enable strdup

#include "ir.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ir_label * freshLabel() {
    static char buffer[1024];
    static int lastLabel = 0;

    snprintf(buffer, sizeof(buffer), "L%d", lastLabel);
    ir_label * ret = calloc(sizeof(ir_label), 1);
    lastLabel += 1;

    ret->name = strdup(buffer);

    return ret;
}

ir_label * namedLabel(char * name) {
    ir_label * ret = calloc(sizeof(ir_label), 1);
    ret->name = name;

    return ret;
}


ir_node * Iconst(int i) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_iconst;
    ret->data.iconst = i;
    return ret;
}

ir_node * Sconst(char * s) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_sconst;
    ret->data.sconst = s;
    return ret;
}

ir_node * Ops(enum ir_op ir) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir;
    return ret;
}

ir_node * Intrinsic(enum intrinsic ir) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_intrinsic;
    ret->data.intrinsic = ir;
    return ret;
}

ir_node * Reserve(int size, char * name, char * val) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_reserve;
    ret->data.reserve.name = name;
    ret->data.reserve.size = size;
    ret->data.reserve.val = val;
    return ret;
}

ir_node * Read(char * name) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_read;
    ret->data.sconst = name;
    return ret;
}

ir_node * Address(char * name) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_address;
    ret->data.sconst = name;
    return ret;
}

ir_node * Write(char * name) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_write;
    ret->data.sconst = name;
    return ret;
}

ir_node * ArgLocalRead(int i) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_arglocal_read;
    ret->data.iconst = i;
    return ret;
}

ir_node * ArgLocalWrite(int i) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_arglocal_write;
    ret->data.iconst = i;
    return ret;
}

ir_node * Label(ir_label * lbl) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_lbl;
    ret->data.lbl = lbl;
    return ret;
}

ir_node * Function(ir_label * lbl, int locs) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_function;
    ret->data.call_function.lbl = lbl;
    ret->data.call_function.vars = locs;
    return ret;
}

ir_node * Call(ir_label * lbl, int vars) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_call;
    ret->data.call_function.lbl = lbl;
    ret->data.call_function.vars = vars;
    return ret;
}

ir_node * Return() {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_ret;
    return ret;
}

ir_node * Jump(ir_label * lbl) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_jump;
    ret->data.lbl = lbl;
    return ret;
}

ir_node * Branch(ir_label * lbl) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_branchzero;
    ret->data.lbl = lbl;
    return ret;
}

ir_node * Seq(ir_node * i1, ir_node * i2) {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_seq;
    ret->data.seq.o1 = i1;
    ret->data.seq.o2 = i2;
    return ret;
}

ir_node * Nop() {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_nop;
    return ret;
}

ir_node * Pop() {
    ir_node * ret = calloc(sizeof(ir_node), 1);
    ret->kind = ir_pop;
    ret->data.iconst = -1;
    return ret;
}
