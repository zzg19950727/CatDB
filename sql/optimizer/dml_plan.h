#ifndef DML_PLAN_H
#define DML_PLAN_H
#include "expr_stmt.h"
#include "log_join.h"
#include "type.h"
#include "plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
		DECLARE(ExprStmt);
		DECLARE(TableStmt);
		DECLARE(SelectStmt);
	}
	namespace Optimizer {
		DECLARE(LogicalOperator);
		using Parser::Stmt_s;
		using Parser::ExprStmt_s;
		using Parser::TableStmt;
		using Parser::SelectStmt_s;
		using Parser::BasicTableStmt_s;
		using Parser::JoinedTableStmt_s;
		using Parser::ViewTableStmt_s;

		struct JoinInfo {
			Vector<ExprStmt_s> equal_join_condition;
			Vector<ExprStmt_s> other_join_condition;
			Vector<ExprStmt_s> outer_join_filter;
			JoinType join_type;
			KV_STRING(
				KV(join_type, JoinTypeString[join_type]),
				K(equal_join_condition),
				K(other_join_condition),
				K(outer_join_filter)
			);
		};
		struct ConflictRule {
			BitSet condition;
			BitSet constraint;
			KV_STRING(
				K(condition),
				K(constraint)
			);
		};
		DECLARE(ConflictDetector);
		struct ConflictDetector {
			JoinInfo join_info;
			Vector<ConflictRule> conflict_rules;
			Vector<ConflictRule> cross_product_rules;
			BitSet table_set;
			BitSet L_TES;
			BitSet R_TES;
			BitSet L_DS;
			BitSet R_DS;
			bool is_degenerate;
			bool is_symmetric;
			KV_STRING(
				K(join_info),
				K(conflict_rules),
				K(cross_product_rules),
				K(table_set),
				K(L_TES),
				K(R_TES),
				K(L_DS),
				K(R_DS),
				K(is_degenerate),
				K(is_symmetric)
			);
			static ConflictDetector_s make_conflict_detector();
		};

		class DMLPlan : public Plan
		{
		protected:
			DMLPlan();
		public:
			~DMLPlan();
			virtual u32 build_plan();
			virtual PlanType type()const = 0;
			u32 pushdown_quals();
			u32 generate_join_operator(LogicalOperator_s &left,
									LogicalOperator_s &right,
									JoinType join_type,
									Vector<ExprStmt_s> &join_condition,
									LogJoin::JoinAlgo join_algo,
									LogicalOperator_s &op);
			u32 generate_join_order_with_joined_table(JoinedTableStmt_s table_stmt, LogicalOperator_s &op);
			u32 generate_join_order_with_basic_table(BasicTableStmt_s table_stmt, LogicalOperator_s &op);
			u32 generate_join_order_with_view_table(ViewTableStmt_s table_stmt, LogicalOperator_s &op);
			u32 generate_join_order_with_table_item(TableStmt_s& table_stmt, LogicalOperator_s &op);
			u32 generate_join_order();
			u32 generate_sub_select_plan_tree(SelectStmt_s &sub_select, LogicalOperator_s &op);
			virtual u32 generate_plan_tree() = 0;
			u32 set_table_access_columns(LogicalOperator_s &op);
			u32 generate_subplan();

		public:
			Vector<Vector<LogicalOperator_s>> join_orders;
			Vector<ConflictDetector_s> conflict_detectors;
		};
	}
}

#endif	//DML_PLAN_H