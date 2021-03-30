#ifndef __EVAL_H_
#define __EVAL_H_

#include "err.h"
extern "C" {
#include "mpc.h"
}

lval eval_op(lval x, char *op, lval y);

lval eval(mpc_ast_t *t);

#endif // __EVAL_H_
