#ifndef EXPR_STMT_H
#define EXPR_STMT_H
#include "select_stmt.h"
#include "bit_set.h"
#include "object.h"
#include "error.h"
#include "stmt.h"

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
		DECLARE(TableStmt);

		 enum StmtFlag {
			INVALID_FLAG = 0,
			IS_COLUMN,
			HAS_COLUMN,
			IS_CONST,
			HAS_CONST,
			IS_SET_EXPR,
			HAS_SET_EXPR,
			IS_AGG,
			HAS_AGG,
			IS_SUBQUERY,
			HAS_SUBQUERY,
			IS_LIST,
			IS_OP_EXPR,
			IS_EXEC_PARAM,
			HAS_EXEC_PARAM
		};
		//表达式语句
		DECLARE(ExprStmt);
		class ExprStmt
		{
		public:
			enum ExprType {
				Const = 0,
				ExecParam,
				Column,
				SetExpr,
				Aggregate,
				SubQuery,
				List,
				OperationExpr
			};
			
			ExprStmt();
			~ExprStmt();
			virtual ExprType expr_type()const = 0;
			virtual String to_string()const = 0;
			virtual u32 formalize() = 0;
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
			ExprType expr_type()const;
			static ExprStmt_s make_const_stmt(const Object_s& value);
			String to_string()const;
			bool same_as(const ExprStmt_s& other);
			u32 formalize();
			KV_STRING(
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
			ExprType expr_type()const;
			static ExprStmt_s make_exec_param_stmt(const ExprStmt_s& ref_expr);
			String to_string()const;
			bool same_as(const ExprStmt_s& other);
			u32 formalize();
			ExprStmt_s& get_ref_expr() { return ref_expr; }
			KV_STRING(
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
			ExprType expr_type()const;
			static ExprStmt_s make_column_stmt(const String& table, const String& column);
			static ExprStmt_s make_all_column_stmt();
			u32 formalize();
			bool is_all_column()const;
			String to_string()const;
			bool same_as(const ExprStmt_s& other);
			KV_STRING(
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
			ExprType expr_type()const;
			static ExprStmt_s make_set_expr(SetOpType type, u32 idx);
			u32 formalize();
			void set_index(u32 idx) { index = idx; }
			u32 get_index() const { return index; }
			String to_string()const;
			bool same_as(const ExprStmt_s& other);
			KV_STRING(
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
			ExprType expr_type()const;
			static ExprStmt_s make_query_stmt();
			String to_string()const;
			u32 formalize();
			bool same_as(const ExprStmt_s &other) { return false;}
			Vector<ExecParamStmt_s>& get_corrected_exprs() { return exec_params; }
			void add_corrected_exprs(ExecParamStmt_s& expr);
			void set_subquery_id(u32 id) { subquery_id = id; }
			u32 get_subquery_id() const { return subquery_id; }
			KV_STRING(
				KV(flags, flags_to_string()),
				K(table_ids),
				KV(corrected_exprss, params),
				K(query_stmt)
			);

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
			ExprType expr_type()const;
			static ExprStmt_s make_list_stmt();
			String to_string()const;
			u32 size() const { return params.size(); }
			ExprStmt_s& at(u32 i) { return params[i]; }
			void push_back(const ExprStmt_s &expr) { params.push_back(expr); }
			void clear() { params.clear(); }
			u32 formalize();
			bool same_as(const ExprStmt_s &other);
			KV_STRING(
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
			ExprType expr_type()const;
			static ExprStmt_s make_aggr_stmt();
			String to_string()const;
			String aggr_func_name()const;
			u32 formalize();
			bool same_as(const ExprStmt_s &other);
			void set_aggr_expr(const ExprStmt_s& expr);
			ExprStmt_s get_aggr_expr() const;

			KV_STRING(
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
			ExprType expr_type()const;
			static ExprStmt_s make_op_expr_stmt(OperationType op_type);
			String to_string()const;
			u32 formalize();
			bool same_as(const ExprStmt_s &other);
			//json print
			static String op_string(OperationType op_type);
			
			KV_STRING(
				KV(flags, flags_to_string()),
				KV(op_type, op_string(op_type)),
				K(table_ids),
				K(params)
			);

		public:
			OperationType op_type;
		};

		//表的描述语句
		DECLARE(TableStmt);
		class TableStmt
		{
		protected:
			TableStmt();
		public:
			~TableStmt();
			enum TableType {BasicTable = 0, JoinedTable, ViewTable};
			
			String to_string()const {return alias_name;}
			virtual u32 formalize() = 0;
			bool is_basic_table()const {return table_type == BasicTable;}
			bool is_joined_table()const {return table_type == JoinedTable;}
			bool is_view_table()const {return table_type == ViewTable;}
			virtual bool is_dual_table() const { return false; }
			bool is_basic_and_not_dual_table() const { return is_basic_table() && !is_dual_table(); }
			bool same_as(const TableStmt_s& other)	{ return false; }
			virtual u32 get_table_exprs(Vector<ExprStmt_s> &exprs);
			void set_alias_name(const String& alias_name);
			//json print
			static String get_table_type_name(TableType table_type);
			VIRTUAL_KV_STRING(
				K(alias_name),
				K(table_id)
			);

		public:
			Vector<ExprStmt_s> table_filter;
			TableType table_type;
			String alias_name;
			u32 table_id;
			BitSet table_ids;
		};

		DECLARE(BasicTableStmt);
		class BasicTableStmt : public TableStmt
		{
		private:
			BasicTableStmt(const String &database, const String& table_name);
		public:
			static TableStmt_s make_basic_table(const String &database, const String& table_name);
			static TableStmt_s make_dual_table();
			bool is_dual_table() const { return is_dual; }
			u32 formalize();
			bool same_as(const TableStmt_s& other);
			
			
			KV_STRING(
				KV(table_type, get_table_type_name(table_type)),
				K(database),
				K(table_name),
				K(alias_name),
				K(ref_table_id),
				K(table_id),
				K(table_filter),
				K(is_dual)
			);

			String database;
			String table_name;
			u32 ref_table_id;
			bool is_dual;
		};

		DECLARE(JoinedTableStmt);
		class JoinedTableStmt : public TableStmt
		{
		private:
			JoinedTableStmt()
			{ table_type = JoinedTable; }
		public:
			static TableStmt_s make_joined_table(TableStmt_s &left_table,
												 TableStmt_s &right_table,
												 JoinType join_type,
												 ExprStmt_s &join_condition);
			u32 formalize();
			u32 get_table_items(Vector<TableStmt_s> &table_items);
			u32 get_table_exprs(Vector<ExprStmt_s> &exprs)override;

			KV_STRING(
				KV(table_type, get_table_type_name(table_type)),
				KV(join_type, JoinTypeString[join_type]),
				K(table_id),
				K(left_table),
				K(right_table),
				K(join_condition),
				K(table_filter)
			);
			TableStmt_s left_table;
			TableStmt_s right_table;
			JoinType join_type;
			Vector<ExprStmt_s> join_condition;
		};

		DECLARE(ViewTableStmt);
		class ViewTableStmt : public TableStmt
		{
		private:
			ViewTableStmt(Stmt_s &ref_query)
				:ref_query(ref_query)
			{ table_type = TableStmt::ViewTable; }
		public:
			static TableStmt_s make_view_table(Stmt_s &ref_query);
			u32 formalize();
			
			KV_STRING(
				KV(table_type, get_table_type_name(table_type)),
				K(alias_name),
				K(table_id),
				K(table_filter),
				K(ref_query)
				);
			SelectStmt_s ref_query;
		};
	}
}

#endif	//EXPR_STMT_H