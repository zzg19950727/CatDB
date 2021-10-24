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
		DECLARE(LeadingTable);
	}
	namespace Optimizer {
		DECLARE(EstInfo);
		DECLARE(LogicalOperator);
		using Parser::Stmt_s;
		using Parser::ExprStmt_s;
		using Parser::TableStmt;
		using Parser::SelectStmt_s;
		using Parser::BasicTableStmt_s;
		using Parser::JoinedTableStmt_s;
		using Parser::ViewTableStmt_s;
		using Parser::LeadingTable_s;

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
			Vector<ConflictRule> conflict_rules;
			Vector<BitSet> cross_product_rules;
			JoinInfo join_info;
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
			static ConflictDetector_s make_conflict_detector()
			{ return ConflictDetector_s(new ConflictDetector); }
		};

		struct LeadingInfo {
			struct TablePair {
				KV_STRING(
					K(left_ids),
					K(right_ids)
				);
				BitSet left_ids;
				BitSet right_ids;
			};
			KV_STRING(
				K(table_pairs),
				K(table_ids)
			);
			Vector<TablePair> table_pairs;
			BitSet table_ids;
		};

		class DMLPlan : public Plan
		{
		protected:
			DMLPlan();
		public:
			~DMLPlan();
			virtual u32 build_plan();
			virtual PlanType type()const = 0;
			u32 generate_conflict_detecotrs(Vector<TableStmt_s> &tables, 
											Vector<ExprStmt_s> &conds,
											Vector<ConflictDetector_s> &detectors);
			u32 generate_cross_product_detector(Vector<TableStmt_s> &tables, 
												Vector<ConflictDetector_s> &detectors,
												Vector<ConflictDetector_s> &outer_join_detectors);
			u32 flatten_table_items(Vector<TableStmt_s> &tables, 
									Vector<ExprStmt_s> &conds);
			u32 flatten_table_items(const TableStmt_s &table, 
									Vector<TableStmt_s> &tables, 
									Vector<ExprStmt_s> &conds);
			u32 generate_outer_join_detecotrs(TableStmt_s &table, 
                                  			  Vector<ConflictDetector_s> &detectors);
			u32 change_right_join_to_left(JoinedTableStmt_s &joined_table);
			u32 pushdown_outer_join_filter(JoinedTableStmt_s &joined_table);
			u32 pushdown_on_condition(JoinedTableStmt_s &joined_table);
			u32 inner_generate_outer_join_detectors(JoinedTableStmt_s &joined_table, 
													ConflictDetector_s &detector);
			u32 generate_inner_join_detectors(Vector<ExprStmt_s> &join_conditions, 
											Vector<ConflictDetector_s> &outer_join_detectors, 
											Vector<ConflictDetector_s> &inner_join_detector);
			u32 inner_generate_inner_join_detectors(ConflictDetector_s &detector);
			u32 add_join_condition(const ExprStmt_s& cond, JoinInfo &info);
			u32 generate_conflict_rules(ConflictDetector_s &detector, 
										bool is_left_child, 
										Vector<ConflictDetector_s> &child_detectors);
			u32 generate_conflict_rules(ConflictDetector_s &detector, 
										bool is_left_child, 
										const ConflictDetector_s &child_detectors);
			u32 add_conflict_rule(Vector<ConflictRule> &conflict_rules, 
								const BitSet& condition, 
								const BitSet &constraint);
			u32 generate_join_operator(LogicalOperator_s &left,
									LogicalOperator_s &right,
									JoinInfo &join_info,
									JoinAlgo join_algo,
									LogicalOperator_s &op);
			u32 generate_join_order_with_joined_table(JoinedTableStmt_s table_stmt, LogicalOperator_s &op);
			u32 generate_join_order_with_basic_table(BasicTableStmt_s table_stmt, LogicalOperator_s &op);
			u32 generate_join_order_with_view_table(ViewTableStmt_s table_stmt, LogicalOperator_s &op);
			u32 generate_join_order_with_table_item(TableStmt_s& table_stmt, LogicalOperator_s &op);
			u32 generate_join_order();
			u32 generate_base_plan(Vector<TableStmt_s> &base_tables);
			u32 generate_join_order_with_DP(bool ignore_hint);
			u32 generate_join_order_with_DP(bool ignore_hint, u32 left_level, u32 right_level);
			u32 is_leading_legal(const BitSet &left_tables, 
								const BitSet &right_tables, 
								bool &is_legal);
			u32 choose_join_info(LogicalOperator_s &left_tree, 
								 LogicalOperator_s &right_tree, 
								 Vector<ConflictDetector_s> &detectors,
								 bool &is_legal);
			u32 is_join_legal(const BitSet &left_tables, 
							  const BitSet &right_tables, 
							  const ConflictDetector_s &detector, 
							  bool &is_legal);
			u32 merge_join_infos(const BitSet &left_tables, 
							  	 const BitSet &right_tables, 
							  	 Vector<ConflictDetector_s> &join_infos, 
                     			 JoinInfo &join_info);
			u32 generate_join_plan(LogicalOperator_s &left_tree, 
								   LogicalOperator_s &right_tree, 
								   JoinInfo &join_info,
								   LogicalOperator_s &join_plan);
			u32 get_join_method(const BitSet &left_tables,
								const BitSet &right_tables,
								JoinInfo &join_info,
								JoinAlgo &algo);
			u32 add_join_order(LogicalOperator_s& join_order, u32 level);
			u32 generate_sub_select_plan_tree(SelectStmt_s &sub_select, LogicalOperator_s &op);
			virtual u32 generate_plan_tree() = 0;
			u32 set_table_access_columns(LogicalOperator_s &op);
			u32 generate_subplan();
			u32 init_leading_info();
			u32 get_leading_info(const LeadingTable_s &leading_table, BitSet &table_ids);

		public:
			Vector<Vector<LogicalOperator_s>> join_orders;
			Vector<ConflictDetector_s> conflict_detectors;
			LeadingInfo leading_info;
			EstInfo_s est_info;
		};
	}
}

#endif	//DML_PLAN_H