#ifndef EXPR_GENERATOR_H
#define EXPR_GENERATOR_H
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(ExprStmt);
        DECLARE(ExecParamStmt);
        DECLARE(SubQueryStmt);
    }
    namespace Optimizer {
        DECLARE(LogicalOperator);
    }
    namespace Sql {
        DECLARE(Expression);
        DECLARE(PhyOperator);
        DECLARE(ExecParamExpression);
        using Parser::ExprStmt_s;
        using Parser::ExecParamStmt_s;
        using Parser::SubQueryStmt_s;
        using Optimizer::LogicalOperator_s;

        struct ExprGenerateCtx {
            template<typename T>
            u32 key(const shared_ptr<T> &obj)
            { return (u64)obj.get(); }
            HashMap<ExprStmt_s, Expression_s> exec_param_map;
            HashMap<u32, LogicalOperator_s> subplan_map;
            HashMap<ExprStmt_s, Expression_s> access_expr_map;
            Vector<PhyOperator_s> child_ops;
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
            static u32 generate_subquery_expr(ExprGenerateCtx &ctx, const SubQueryStmt_s &expr, Expression_s &rt_expr);
            static u32 generate_exec_params(ExprGenerateCtx &ctx, Vector<ExecParamStmt_s> &exprs, Vector<ExecParamExpression_s> &rt_exprs);
        };
    }
}

#endif  //EXPR_GENERATOR_H