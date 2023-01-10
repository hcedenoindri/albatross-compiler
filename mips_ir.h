#ifndef MIPS_IR_H
#define MIPS_IR_H

#include <stdio.h>
#include "ir.h"

// generate MIPS from IR
void mips_ir(ir_node * ir, FILE * out);

#endif
