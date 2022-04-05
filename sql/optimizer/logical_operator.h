#ifndef LOGICAL_OPERATOR_H
#define LOGICAL_OPERATOR_H
#include "plan_info.h"
#include "bit_set.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(TableStmt);
		DECLARE(BasicTableStmt);
		DECLARE(ViewTableStmt);
		DECLARE(ExprStmt);
		DECLARE(ColumnStmt);
	}
	namespace Sql {
		DECLARE(QueryCtx);
	}
	namespace Optimizer {
		DECLARE(LogicalOperator);
        DECLARE(SingleChildLogicalOperator);
        DECLARE(DoubleChildLogicalOperator);
		DECLARE(DMLPlan);
		DECLARE(EstInfo);
		DECLARE(ConflictDetector);
		using Sql::QueryCtx_s;
		using Parser::ExprStmt_s;
		using Parser::ColumnStmt_s;
		using Parser::TableStmt_s;
		using Parser::BasicTableStmt_s;
		using Parser::ViewTableStmt_s;

		struct ExprCtx {
			Vector<ExprStmt_s> expr_consume;
			Vector<ExprStmt_s> expr_produce;
		};

		//物理运算符接口
		class LogicalOperator
		{
		public:
			LogicalOperator();
			virtual ~LogicalOperator();
			virtual u32 type() const = 0;
			String get_op_name();
			u32 init(QueryCtx_s &query_ctx, EstInfo_s& est_info);
			u32 compute_property();
			virtual u32 est_row_count();
			virtual u32 est_cost();
			double get_output_rows() { return output_rows; }
			double get_cost() { return cost; }
			const BitSet &get_tables_ids() const { return table_ids; }
			void add_table_ids(const BitSet& ids) { table_ids.add_members(ids); }
			virtual u32 allocate_expr_pre();
			virtual u32 allocate_expr_post(Vector<ExprStmt_s> &expr_input, 
										   Vector<ExprStmt_s> &expr_output);
			u32 expr_can_be_consumed(Vector<ExprStmt_s>& expr_consume, 
									 Vector<ExprStmt_s>& expr_produce, 
									 bool &can_be);
			u32 expr_can_be_consumed(ExprStmt_s& expr_consume, 
									 Vector<ExprStmt_s>& expr_produce, 
									 bool &can_be);
			u32 produce_more_expr(Vector<ExprStmt_s>& expr_consume, 
								  Vector<ExprStmt_s>& expr_produce, 
								  Vector<ExprStmt_s>& expr_output);
			u32 find_expr_will_use(ExprStmt_s& expr_consume, 
								   Vector<ExprStmt_s>& expr_produce, 
								   Vector<ExprStmt_s>& expr_output);
			u32 generate_operator_id_pre(u32 &id);
			u32 visit_plan();
			virtual void print_plan(u32 depth, Vector<PlanInfo> &plan_info) = 0;
			void print_basic_info(u32 depth, PlanInfo& info, const String &type="");
			void print_exprs(Vector<ExprStmt_s> &exprs, 
							 const String &title, 
							 PlanInfo& info);
			void print_exprs(Vector<ExprStmt_s> &exprs, 
							 const String& type, 
							 const String &title, 
							 PlanInfo& info);
			void print_expr(ExprStmt_s &expr, String &out);
			Vector<ConflictDetector_s> &get_used_conflict_detectors() 
			{ return used_conflict_detectors; }

		public:
			Vector<ConflictDetector_s> used_conflict_detectors;
			Vector<LogicalOperator_s> childs;
			Vector<ExprStmt_s> filters;
			Vector<ExprStmt_s> output_exprs;
			Vector<ExprStmt_s> access_exprs;
			LogicalOperator_s parent;
			EstInfo_s est_info;
			QueryCtx_s query_ctx;
			BitSet table_ids;
			DMLPlan_s plan;
            u32 operator_id;
            double output_rows;
            double cost;
		protected:
			ExprCtx expr_ctx;
		private:
			DISALLOW_COPY_AND_ASSIGN(LogicalOperator)
		};

		class SingleChildLogicalOperator : public LogicalOperator
		{
		public:
			SingleChildLogicalOperator(const LogicalOperator_s& child)
                :LogicalOperator() {childs.push_back(child);}
			virtual ~SingleChildLogicalOperator(){}
			LogicalOperator_s& child() { return childs[0]; }
		protected:
		private:
			SingleChildLogicalOperator() = delete;
		};

		class DoubleChildLogicalOperator : public LogicalOperator
		{
		public:
			DoubleChildLogicalOperator(const LogicalOperator_s& left, const LogicalOperator_s& right)
                :LogicalOperator()
                {childs.push_back(left); childs.push_back(right);}
			virtual ~DoubleChildLogicalOperator() {}
			LogicalOperator_s& left_child() { return childs[0]; }
			LogicalOperator_s& right_child() { return childs[1]; }

		protected:
		private:
			DoubleChildLogicalOperator() = delete;
		};
	}
}

#endif	//LOGICAL_OPERATOR_H