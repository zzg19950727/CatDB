#ifndef EXPRESSION_CALC_H
#define EXPRESSION_CALC_H
#include "type.h"
namespace CatDB {
	namespace Sql {
        DECLARE(Expression);
        DECLARE(ExecCtx);
        typedef u32 (*OpFuncType)(const Vector<Expression_s>&, ExecCtx_s&);
    }
}

#endif  //EXPRESSION_CALC_H