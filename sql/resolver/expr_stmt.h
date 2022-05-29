#ifndef EXPR_STMT_H
#define EXPR_STMT_H
#include "bit_set.h"
#include "object.h"
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Sql {
		DECLARE(QueryCtx);
	}
	namespace Parser {
		using Common::Object_s;
		using Sql::QueryCtx_s;
		using Common::DataType;
		DECLARE(Stmt);
		DECLARE(SelectStmt);
		DECLARE(ExprStmt);
		DECLARE(AggrStmt);
		DECLARE(ConstStmt);
		DECLARE(ColumnStmt);
		DECLARE(OpExprStmt);
		DECLARE(ListStmt);
		DECLARE(OrderStmt);

		//表达式语句
		DECLARE(ExprStmt);
		class ExprStmt
		{
		public:
			ExprStmt();
			virtual ~ExprStmt();
			virtual ExprType expr_type()const = 0;
			virtual String to_string()const = 0;
			virtual u32 formalize() = 0;
			virtual u32 deduce_type();
			virtual u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const;
			virtual bool same_as(const ExprStmt_s &other) { return false;}
			//expr flag interface
			bool has_flag(StmtFlag flag) const { return flag == is_flag || flags.has_member(u32(flag)); }
			void add_flag(StmtFlag flag) { flags.add_member(u32(flag)); }
			void del_flag(StmtFlag flag) { flags.del_member(u32(flag)); }
			void set_is_flag(StmtFlag flag) { is_flag = flag; }
			const BitSet &get_flags() const { return flags; }
			void add_flags(const BitSet &other) { flags.add_members(other); }
			void clear_flag() { is_flag = INVALID_FLAG; flags.clear(); }
			String flags_to_string() const;
			//public function
			bool is_and_expr();
			void add_param(const ExprStmt_s& param) { params.push_back(param); }
			const BitSet& get_table_ids() const { return table_ids; }
			Vector<ExprStmt_s> &get_params() { return params; }
			String get_alias_name() const;
			//json print
			VIRTUAL_KV_STRING(
				K(res_type),
				K(alias_name)
			);
		public:
			StmtFlag is_flag;
			BitSet flags;
			Vector<ExprStmt_s> params;
			String alias_name;	//表达式的别名
			BitSet table_ids;
			DataType res_type;
		};

		//单个常量表达式
		DECLARE(ConstStmt);
		class ConstStmt : public ExprStmt
		{
		private:
			ConstStmt();
		public:
			~ConstStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_const_stmt(const Object_s& value);
			String to_string()const override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			u32 formalize() override;
			u32 deduce_type() override;
			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
				K(res_type),
				K(value)
			);

		public:
			Object_s value;
		};

		DECLARE(ExecParamStmt);
		class ExecParamStmt : public ExprStmt
		{
		private:
			ExecParamStmt();
		public:
			~ExecParamStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_exec_param_stmt(u32 index);
			String to_string()const override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			u32 formalize() override;
			u32 get_param_index() const { return param_index; }
			void set_param_index(u32 index) { param_index = index; }
			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
				K(res_type),
				K(param_index)
			);

		public:
			u32 param_index;
		};

		/*单列表达式
		column为*时表示表的所有列，
		table、column同时为*时表示from列表所有表的所有列
		*/
		DECLARE(ColumnStmt);
		class ColumnStmt : public ExprStmt
		{
		private:
			ColumnStmt();
		public:
			~ColumnStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_column_stmt(const String& table, const String& column);
			static ExprStmt_s make_all_column_stmt();
			u32 formalize() override;
			u32 deduce_type() override;
			bool is_all_column()const;
			String to_string()const override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
				K(res_type),
				K(table),
				K(column),
				K(table_id),
				K(column_id)
			);

		public:
			String table;	//所属表
			String column;	//列名
			bool is_row_id;
			u32 table_id;
			u32 column_id;
		};

		DECLARE(SetExprStmt);
		class SetExprStmt : public ExprStmt
		{
		private:
			SetExprStmt();
		public:
			~SetExprStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_set_expr(SetOpType type, u32 idx);
			u32 formalize() override;
			void set_index(u32 idx) { index = idx; }
			u32 get_index() const { return index; }
			String to_string()const override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			KV_STRING_OVERRIDE(
				KV(type, SetOpTypeString[type]),
				K(res_type),
				K(index)
			);

		public:
			u32 index;
			SetOpType type;
		};

		//子查询语句
		DECLARE(SubQueryStmt);
		class SubQueryStmt : public ExprStmt
		{
		private:
			SubQueryStmt();
		public:
			~SubQueryStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_query_stmt();
			String to_string()const override;
			u32 formalize() override;
			u32 deduce_type() override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s &other) override;
			void add_related_exprs(ExprStmt_s &related_expr, ExecParamStmt_s &exec_param);
			void set_subquery_id(u32 id) { subquery_id = id; }
			bool has_related_expr() const { return !exec_params.empty(); }
			u32 get_subquery_id() const { return subquery_id; }
			u32 get_all_exec_params(Vector< std::pair<ExecParamStmt_s, ExprStmt_s> > &all_exec_params);
			DECLARE_KV_STRING_OVERRIDE;

		public:
			SelectStmt_s query_stmt;
			Vector<ExecParamStmt_s> exec_params;
			u32 subquery_id;
		};

		//表达式列表
		DECLARE(ListStmt);
		class ListStmt : public ExprStmt
		{
		public:
			ListStmt();
		public:
			~ListStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_list_stmt();
			String to_string()const override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			u32 size() const { return params.size(); }
			ExprStmt_s& at(u32 i) { return params[i]; }
			void push_back(const ExprStmt_s &expr) { params.push_back(expr); }
			void clear() { params.clear(); }
			u32 formalize() override;
			u32 deduce_type() override;
			bool same_as(const ExprStmt_s &other) override;
			KV_STRING_OVERRIDE(
				K(res_type),
				V(params)
			);
		};

		//聚合函数语句
		DECLARE(AggrStmt);
		class AggrStmt : public ExprStmt
		{
		private:
			AggrStmt();
		public:
			~AggrStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_aggr_stmt();
			String to_string()const override;
			u32 formalize() override;
			u32 deduce_type() override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s &other) override;
			void set_aggr_expr(const ExprStmt_s& expr);
			ExprStmt_s get_aggr_expr() const;

			KV_STRING_OVERRIDE(
				KV(aggr_func, AggrTypeString[aggr_func]),
				KV(flags, flags_to_string()),
				K(res_type),
				K(table_ids),
				KV(aggr_expr, get_aggr_expr()),
				K(distinct)
			);

		public:
			AggrType aggr_func;		//聚合函数类型
			bool distinct;
		};
		
		DECLARE(OpExprStmt);
		class OpExprStmt :public ExprStmt
		{
		private:
			OpExprStmt(OperationType op_type);
		public:
			~OpExprStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_op_expr_stmt(OperationType op_type);
			String to_string()const override;
			u32 formalize() override;
			u32 deduce_type() override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s &other) override;
			
			KV_STRING_OVERRIDE(
				KV(op_type, OperationTypeString[op_type]),
				KV(flags, flags_to_string()),
				K(res_type),
				K(table_ids),
				K(params)
			);

		public:
			OperationType op_type;
		};

		class OrderStmt : public ExprStmt
		{
		private:
			OrderStmt();
		public:
			~OrderStmt();
			ExprType expr_type()const override;
			static OrderStmt_s make_order_stmt(const ExprStmt_s& order_expr, bool asc = true);
			String to_string()const override;
			u32 formalize() override;
			u32 deduce_type() override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s &other) override;
			void set_order_by_expr(const ExprStmt_s& expr);
			ExprStmt_s get_order_by_expr() const;

			KV_STRING_OVERRIDE(
				KV(order_expr, get_order_by_expr()),
				K(asc)
			);

		public:
			bool asc;//升序还是降序,true为asc，false为desc，默认asc
		};

				DECLARE(WinExprStmt);
		class WinExprStmt : public ExprStmt
		{
		private:
			WinExprStmt(WinType win_func);
		public:
			~WinExprStmt();
			ExprType expr_type()const override;
			static ExprStmt_s make_win_expr_stmt(WinType win_func);
			String to_string()const override;
			u32 formalize() override;
			u32 deduce_type() override;
			u32 deep_copy(ExprStmt_s &expr, QueryCtx_s &ctx, u32 flag)const override;
			bool same_as(const ExprStmt_s &other) override;
			void set_win_func_expr(const ExprStmt_s& expr);
			const ExprStmt_s& get_win_func_expr() const;
			void set_win_part_by_exprs(const Vector<ExprStmt_s>& exprs);
			void get_win_part_by_exprs(Vector<ExprStmt_s>& exprs)const;
			void set_win_order_by_exprs(const Vector<ExprStmt_s>& exprs);
			void get_win_order_by_exprs(Vector<ExprStmt_s>& exprs)const;
			bool has_win_func_expr() const { return !func_expr_idx.empty(); }
			bool has_part_by_expr() const { return !partition_by_idx.empty(); }
			bool has_order_by_expr() const { return !order_by_idx.empty(); }

			KV_STRING_OVERRIDE(
				KV(win_func, WinTypeString[win_func]),
				K(is_distinct),
				K(params),
				KV(flags, flags_to_string()),
				K(res_type),
				K(table_ids)
			);
		public:
			WinType win_func;
			bool is_distinct;
			Vector<u32> func_expr_idx;
			Vector<u32> partition_by_idx;
			Vector<u32> order_by_idx;
		};
	}
}

#endif	//EXPR_STMT_H