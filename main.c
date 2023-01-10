#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "ast_to_ir.h"
#include "ir.h"
#include "ir_tree_to_list.h"
#include "ir_pushpop.h"
#include "mips_ir.h"
#include "symbol.h"
#include "y.tab.h"
#include "semantic_analysis_symbols.h"
#include "semantic_analysis_types.h"
#include "frames.h"
#include "transform.h"
#include "optimize.h"

extern FILE *yyin;
extern int lexer();

program p;

static void * g1, *g2, *g3, *ir1, *ir2, *ir3;

int main(int argc, char **argv) {
 char * fname;
 char * outname;
 int optimize = 0;
 if (argc>4) {fprintf(stderr,"usage: albatrosscc <in.albatross> <out.mips> [-o]\n"); return 1;}

 fname=argv[1];
 outname=argv[2];

 if (argc == 4) {
     optimize = !strcmp("-o", argv[3]);
 }

 p.variables = NULL;
 p.functions = NULL;
 p.statements = NULL;

 yyin = fopen(fname,"r");
 if (!yyin) { fprintf(stderr, "cannot open %s\n", fname); return 1; }


 int ret = yyparse();

 // Map from S_Symbol to ty_node
 S_table globals_types = S_empty();

 // Map from S_Symbol to ty_node
 S_table functions_ret_types = S_empty();

 // Map from S_Symbol to fun_frame
 S_table functions_frames = S_empty();

 // symbol analysis
 symbolResolution(&p, globals_types, functions_ret_types, functions_frames);

 // type checking
 typeCheck(&p, globals_types, functions_ret_types, functions_frames);

 // compute frames
 frames(&p, globals_types, functions_ret_types, functions_frames);

 // transform AST as needed
 transform(&p, globals_types, functions_ret_types, functions_frames);

 // perform AST optimizations
  if (optimize) {
      optimize_ast(&p, globals_types, functions_ret_types, functions_frames);
  }

 // transform AST to IR tree
 ir_node * tree_ir = ast_to_ir(&p, globals_types, functions_ret_types, functions_frames);

 // perform Tree IR optimizations
 if (optimize) {
     tree_ir = optimize_tree_ir(tree_ir);
 }

 // transform IR tree to IR list
 ir_node * list_ir = ir_tree_to_list(tree_ir);

 // perform List IR optimizations
 if (optimize) {
     list_ir = optimize_list_ir(list_ir);
 }

 // add explicit push/pop operations to IR
 ir_node * push_pop_ir = addPushPop(list_ir);

 // perform optimizations on List IR with explicit push/pop
 if (optimize) {
     push_pop_ir = optimize_list_explicitPushPop_ir(push_pop_ir);
 }

 // generate assembly
 FILE * out = fopen(outname, "w");
 mips_ir(push_pop_ir, out);
 fclose(out);

 // The library that defines the symbol tables does not provide a destructor to free all memory
 // HACK: Let's put our variables on globals so the leak sanitizer is happy most of the time
 g1 = globals_types;
 g2 = functions_ret_types;
 g3 = functions_frames;
 ir1 = tree_ir;
 ir2 = list_ir;
 ir3 = push_pop_ir;


 return ret;
}

