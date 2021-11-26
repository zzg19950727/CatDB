#ifndef EXPR_STMT_H
#define EXPR_STMT_H
#include "bit_set.h"
#include "object.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Parser {
		using Common::Object_s;
		DECLARE(Stmt);
		DECLARE(SelectStmt);
		DECLARE(ExprStmt);
		DECLARE(AggrStmt);
		DECLARE(ConstStmt);
		DECLARE(ColumnStmt);
		DECLARE(OpExprStmt);
		DECLARE(ListStmt);

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
			virtual u32 deep_copy(ExprStmt_s &expr, u32 flag)const = 0;
			virtual bool same_as(const ExprStmt_s &other) { return false;}
			//expr flag interface
			bool has_flag(StmtFlag flag) const { return flag == is_flag || flags.has_member(u32(flag)); }
			void add_flag(StmtFlag flag) { flags.add_member(u32(flag)); }
			void del_flag(StmtFlag flag) { flags.del_member(u32(flag)); }
			void set_is_flag(StmtFlag flag) { is_flag = flag; }
			const BitSet &get_flags() const { return flags; }
			void add_flags(const BitSet &other) { flags.add_members(other); }
			void clear_flag() { is_flag = INVALID_FLAG; flags.clear(); }
			String flag_to_string(u32 flag) const;
			String flags_to_string() const;
			//public function
			bool is_and_expr();
			void add_param(const ExprStmt_s& param) { params.push_back(param); }
			const BitSet& get_table_ids() const { return table_ids; }
			Vector<ExprStmt_s> &get_params() { return params; }
			//json print
			VIRTUAL_KV_STRING(
				K(alias_name)
			);
		public:
			StmtFlag is_flag;
			BitSet flags;
			Vector<ExprStmt_s> params;
			String alias_name;	//表达式的别名
			BitSet table_ids;
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
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			u32 formalize() override;
			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
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
			static ExprStmt_s make_exec_param_stmt(const ExprStmt_s& ref_expr);
			String to_string()const override;
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			u32 formalize() override;
			ExprStmt_s& get_ref_expr() { return ref_expr; }
			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
				K(ref_expr)
			);

		public:
			ExprStmt_s ref_expr;
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
			bool is_all_column()const;
			String to_string()const override;
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
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
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			bool same_as(const ExprStmt_s& other) override;
			KV_STRING_OVERRIDE(
				KV(type, SetOpTypeString[type]),
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
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			bool same_as(const ExprStmt_s &other) { return false;}
			Vector<ExecParamStmt_s>& get_corrected_exprs() { return exec_params; }
			void add_corrected_exprs(ExecParamStmt_s expr);
			void set_subquery_id(u32 id) { subquery_id = id; }
			u32 get_subquery_id() const { return subquery_id; }
			DECLARE_KV_STRING_OVERRIDE;

		public:
			SelectStmt_s query_stmt;
			bool is_any;
			bool is_all;
			Vector<ExecParamStmt_s> exec_params;
			u32 subquery_id;
			bool output_one_row;
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
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			u32 size() const { return params.size(); }
			ExprStmt_s& at(u32 i) { return params[i]; }
			void push_back(const ExprStmt_s &expr) { params.push_back(expr); }
			void clear() { params.clear(); }
			u32 formalize() override;
			bool same_as(const ExprStmt_s &other);
			KV_STRING_OVERRIDE(
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
			String aggr_func_name()const;
			u32 formalize() override;
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			bool same_as(const ExprStmt_s &other);
			void set_aggr_expr(const ExprStmt_s& expr);
			ExprStmt_s get_aggr_expr() const;

			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
				KV(aggr_func, aggr_func_name()),
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
			u32 deep_copy(ExprStmt_s &expr, u32 flag)const override;
			bool same_as(const ExprStmt_s &other);
			//json print
			static String op_string(OperationType op_type);
			
			KV_STRING_OVERRIDE(
				KV(flags, flags_to_string()),
				KV(op_type, op_string(op_type)),
				K(table_ids),
				K(params)
			);

		public:
			OperationType op_type;
		};
	}
}

#endif	//EXPR_STMT_H