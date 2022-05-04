#ifndef EXPR_GENERATOR_H
#define EXPR_GENERATOR_H
#include "phy_operator.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(ExprStmt);
        DECLARE(ExecParamStmt);
    }
    namespace Sql {
        DECLARE(ExecCtx);
        DECLARE(Expression);
        DECLARE(PhyOperator);
        DECLARE(ExecParamExpression);
        using Parser::ExprStmt_s;
        using Parser::ExecParamStmt_s;

        struct ExprGenerateCtx {
            ExprGenerateCtx();
            ~ExprGenerateCtx();
            HashMap<ExprStmt_s, Expression_s> access_expr_map;
            Vector<PhyOperator_s> child_ops;
            ExecCtx_s exec_ctx;
        };

        class ExprGenerator {
        private:
            ExprGenerator() {}
        public:
            ~ExprGenerator() {}
            static u32 generate_exprs(ExprGenerateCtx &ctx, const Vector<ExprStmt_s> &exprs, Vector<Expression_s> &rt_exprs);
            static u32 generate_expr(ExprGenerateCtx &ctx, const ExprStmt_s &expr, Expression_s &rt_expr);
        protected:
            static u32 inner_generate_expr(ExprGenerateCtx &ctx, const ExprStmt_s &expr, Expression_s &rt_expr);
            static u32 generate_exec_params(ExprGenerateCtx &ctx, 
                                            Vector<std::pair<ExecParamStmt_s, ExprStmt_s>> &exprs, 
                                            Vector<std::pair<ExecParamExpression_s, Expression_s>> &rt_exprs);
        };
    }
}

#endif  //EXPR_GENERATOR_H