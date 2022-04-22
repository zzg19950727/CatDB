#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H
#include "expr_generator.h"
#include "type.h"

namespace CatDB {
    namespace Parser {
        DECLARE(BasicTableStmt);
    }
    namespace Storage {
        DECLARE(TableSpace);
    }
    namespace Optimizer {
        DECLARE(LogicalOperator);
        DECLARE(LogDelete);
        DECLARE(LogDistinct);
        DECLARE(LogDualTable);
        DECLARE(LogExprValue);
        DECLARE(LogGroupBy);
        DECLARE(LogInsert);
        DECLARE(LogJoin);
        DECLARE(LogLimit);
        DECLARE(LogScalarGroupBy);
        DECLARE(LogSet);
        DECLARE(LogSort);
        DECLARE(LogSubQueryEvaluate);
        DECLARE(LogTableScan);
        DECLARE(LogUpdate);
        DECLARE(LogView);
    }
    namespace Sql {
        DECLARE(PhyOperator);
        DECLARE(PhyDelete);
        DECLARE(PhyHashDistinct);
        DECLARE(PhyDualTable);
        DECLARE(PhyExprValue);
        DECLARE(PhyHashGroup);
        DECLARE(PhyInsert);
        DECLARE(PhyHashJoin);
        DECLARE(PhyNestedLoopJoin);
        DECLARE(PhyLimit);
        DECLARE(PhyScalarGroup);
        DECLARE(PhyHashSet);
        DECLARE(PhySort);
        DECLARE(PhyTopNSort);
        DECLARE(PhyTableScan);
        DECLARE(PhyUpdate);
        DECLARE(PhyView);

        using Optimizer::LogicalOperator_s;
        using Optimizer::LogDelete_s;
        using Optimizer::LogDistinct_s;
        using Optimizer::LogDualTable_s;
        using Optimizer::LogExprValue_s;
        using Optimizer::LogGroupBy_s;
        using Optimizer::LogInsert_s;
        using Optimizer::LogJoin_s;
        using Optimizer::LogLimit_s;
        using Optimizer::LogScalarGroupBy_s;
        using Optimizer::LogSet_s;
        using Optimizer::LogSort_s;
        using Optimizer::LogSubQueryEvaluate_s;
        using Optimizer::LogTableScan_s;
        using Optimizer::LogUpdate_s;
        using Optimizer::LogView_s;
        using Storage::TableSpace_s;
        using Parser::BasicTableStmt_s;

        class CodeGenerator {
        private:
            CodeGenerator() {}
        public:
            ~CodeGenerator() {}
            static u32 generate_phy_plan(ExprGenerateCtx &ctx, LogicalOperator_s& log_root, PhyOperator_s &phy_root);
        protected:
            static u32 generate_phy_plan_pre(ExprGenerateCtx &ctx, LogicalOperator_s& log_root);
            static u32 generate_child_phy_plan(ExprGenerateCtx &ctx, LogicalOperator_s& log_root);
            static u32 generate_phy_plan_post(ExprGenerateCtx &ctx, LogicalOperator_s& log_root, PhyOperator_s &phy_root);
            static u32 generate_delete_op(ExprGenerateCtx &ctx, LogDelete_s log_op, PhyOperator_s &phy_op);
            static u32 generate_distinct_op(ExprGenerateCtx &ctx, LogDistinct_s log_op, PhyOperator_s &phy_op);
            static u32 generate_dual_table_op(ExprGenerateCtx &ctx, LogDualTable_s log_op, PhyOperator_s &phy_op);
            static u32 generate_expr_value_op(ExprGenerateCtx &ctx, LogExprValue_s log_op, PhyOperator_s &phy_op);
            static u32 generate_group_by_op(ExprGenerateCtx &ctx, LogGroupBy_s log_op, PhyOperator_s &phy_op);
            static u32 generate_insert_op(ExprGenerateCtx &ctx, LogInsert_s log_op, PhyOperator_s &phy_op);
            static u32 generate_join_op(ExprGenerateCtx &ctx, LogJoin_s log_op, PhyOperator_s &phy_op);
            static u32 generate_hash_join_op(ExprGenerateCtx &ctx, LogJoin_s log_op, PhyOperator_s &phy_op);
            static u32 generate_nested_loop_join_op(ExprGenerateCtx &ctx, LogJoin_s log_op, PhyOperator_s &phy_op);
            static u32 generate_limit_op(ExprGenerateCtx &ctx, LogLimit_s log_op, PhyOperator_s &phy_op);
            static u32 generate_scalar_group_by_op(ExprGenerateCtx &ctx, LogScalarGroupBy_s log_op, PhyOperator_s &phy_op);
            static u32 generate_set_op(ExprGenerateCtx &ctx, LogSet_s log_op, PhyOperator_s &phy_op);
            static u32 generate_sort_op(ExprGenerateCtx &ctx, LogSort_s log_op, PhyOperator_s &phy_op);
            static u32 generate_normal_sort_op(ExprGenerateCtx &ctx, LogSort_s log_op, PhyOperator_s &phy_op);
            static u32 generate_topn_sort_op(ExprGenerateCtx &ctx, LogSort_s log_op, PhyOperator_s &phy_op);
            static u32 generate_subquery_evaluate_op(ExprGenerateCtx &ctx, LogicalOperator_s &log_op, PhyOperator_s &phy_op);
            static u32 generate_table_scan_op(ExprGenerateCtx &ctx, LogTableScan_s log_op, PhyOperator_s &phy_op);
            static u32 generate_update_op(ExprGenerateCtx &ctx, LogUpdate_s log_op, PhyOperator_s &phy_op);
            static u32 generate_view_op(ExprGenerateCtx &ctx, LogView_s log_op, PhyOperator_s &phy_op);

            static u32 generate_access_exprs(ExprGenerateCtx &ctx, LogicalOperator_s& log_root);
            static u32 generate_output_exprs(ExprGenerateCtx &ctx, LogicalOperator_s& log_root, PhyOperator_s &phy_root);
            
        };
    }
}

#endif  //CODE_GENERATOR_H