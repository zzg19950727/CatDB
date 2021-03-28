#ifndef SELECT_PLAN_H
#define SELECT_PLAN_H
#include "expr_stmt.h"
#include "type.h"
#include "plan.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);
		DECLARE(TableStmt);
		DECLARE(ColumnStmt);
	}
	namespace Common {
		DECLARE(Row);
	}
	namespace Sql {
		DECLARE(Plan);
		DECLARE(Filter);
		DECLARE(Expression);
		DECLARE(DeletePlan);
		DECLARE(UpdatePlan);
		DECLARE(SqlRewriter);
		using Common::Row_s;
		using Common::RowDesc;
		using Parser::Stmt_s;
		using Sql::Filter_s;
		using Sql::Expression_s;
		using Sql::DeletePlan;
		using Sql::UpdatePlan;
		using Parser::ExprStmt;
		using Parser::TableStmt;
		using Parser::ColumnStmt;
		using Parser::QueryStmt;

		class SelectPlan : public Plan
		{
		protected:
			SelectPlan();
		public:
			~SelectPlan();
			static Plan_s make_select_plan(const Stmt_s& lex_insert_stmt);
			static  Plan_s make_select_plan(const Vector<TableStmt*>& grandparent_query_tables,
				const Vector<TableStmt*>& parent_query_tables,
				const Stmt_s& lex_insert_stmt);
			u32 execute();
			u32 build_plan();
			u32 optimizer();
			PlanType type() const;
			bool send_plan_result()const;
			void set_alias_table_id(u32 id);
			u32 get_alias_table_id()const;
			u32 get_column_from_select_list(const String& column_name, ColumnDesc& col_desc);
			u32 get_query_row_desc(RowDesc& row_desc);
			const Vector<String>& get_all_output_column() const;
			void reset_for_correlated_subquery(const Row_s& row);
			bool is_simple_scalar_group()const;
			bool is_correlated_query()const;
			double get_select_rows();

		private:
			/*��һ��pairָ��join�����ű����ڶ���pairָ��join condition��join equal condition
			  join condition����join equal condition*/
			typedef Pair<TableStmt*, TableStmt*> JoinableTables;
			typedef Pair<Expression_s, Expression_s> JoinConditions;
			struct SubqueryInfo {
				Expression_s expr;
				ExprStmt::OperationType op;
				Plan_s subplan;
			};
			//����where�Ӿ䣬��ֳ�һ��һ��and���ӵ�stmt
			u32 resolve_where_stmt(const Stmt_s& where_stmt);
			//��������stmt������������ν�ʣ������ǻ�������ν�ʣ�Ҳ��������ͨ�Ĺ���ν��
			u32 resolve_simple_stmt(const Stmt_s& expr_stmt);
			u32 resolve_simple_expr(const Expression_s& expr);
			//��ǰ����������������򷵻������������򷵻�null
			ColumnStmt* resolve_column_stmt(const Stmt_s& stmt);
			//������ת���ɱ���ʽ
			u32 resolve_expr(const Stmt_s& expr_stmt, Expression_s& expr, bool& have_parent_column);
			//����select list�е�*��
			u32 resolve_all_column_in_select_list(const Stmt_s& stmt);
			//����count�ۺϺ����ڵ�*����ʽ
			u32 resolve_all_column_in_count_agg(const Stmt_s& stmt, Expression_s& expr);
			//�жϵ�ǰν�ʿ��Ƿ��ǻ�������ν��
			bool is_table_filter(const Stmt_s& expr_stmt, TableStmt*& table);
			bool is_table_filter(const Expression_s& expr, TableStmt*& table);
			//�����������ȡ������
			u32 resolve_column_desc(ColumnStmt* column_stmt, ColumnDesc& col_desc, bool& from_partent);
			//��from list��������
			u32 find_table(const String& table_name, TableStmt*& table);
			//�Ӹ���ѯ��������
			u32 find_table_from_parent(const String& table_name, TableStmt*& table);
			bool find_table_from_parent(TableStmt* table);
			//��from list����������ָ���еı�
			u32 who_have_column(ColumnStmt* column_stmt, TableStmt*& table);
			//��from list����������ָ���еı�
			u32 who_have_column(const String& column_name, TableStmt*& table);
			u32 who_have_column(const ColumnDesc& col_desc, TableStmt*& table);
			//�Ӹ���ѯ����������ָ���еı�
			u32 which_partent_table_have_column(const String& column_name, TableStmt*& table);
			//����from����Ϊtable list
			u32 get_ref_tables(const Stmt_s& from_stmt);
			//���from�г����Ӳ�ѯ
			u32 get_ref_table_from_query(QueryStmt* subquery);
			//�������ű����������ű�������ν��
			u32 search_jon_info(const JoinableTables& join_tables, JoinConditions& join_cond);
			u32 search_jon_info(const JoinableTables& join_tables, JoinConditions*& join_cond);
			//�������ű�������ν��
			u32 add_join_cond(const JoinableTables& join_tables, const Expression_s& expr);
			//�������ű��ĵ�ֵ����ν��
			u32 add_join_equal_cond(const JoinableTables& join_tables, const Expression_s& expr);
			//���ӻ�������ν��
			u32 add_table_filter(TableStmt* table, const Expression_s& filter);
			//expr = expr and other
			u32 make_and_expression(Expression_s& expr, const Expression_s& other);
			//ѡ�����ŵ�join order
			u32 choos_best_join_order();
			//����group��
			u32 resolve_group_stmt(const Stmt_s& group_stmt);
			//����having�Ӿ�
			u32 resolve_having_stmt(const Stmt_s& having_stmt);
			//����limit�Ӿ�
			u32 resolve_limit_stmt(const Stmt_s& limit_stmt);
			//����sort��
			u32 resolve_sort_stmt(const Stmt_s& sort_stmt);
			//����select list
			u32 resolve_select_list(const Stmt_s& select_list);
			//��������в��ڻ��������ڲ�ѯ�������н���
			u32 resolve_column_from_select_list(const Stmt_s& expr_stmt, Expression_s& expr);
			//����ÿ�ű���Ҫ���ʵ���
			u32 add_access_column(TableStmt* table, const ColumnDesc& col_desc);
			//���ɶ�Ӧ�ļƻ�
			u32 make_access_row_desc();
			u32 make_join_plan(PhyOperator_s& op);
			u32 make_table_scan(TableStmt* table, PhyOperator_s& op);
			u32 make_group_pan(PhyOperator_s& op);
			u32 make_sort_plan(PhyOperator_s& op);
			u32 make_query_plan(PhyOperator_s& op);
			u32 make_distinct_plan(PhyOperator_s& op);
			u32 make_limit_plan(PhyOperator_s& op);
			u32 make_set_plan(PhyOperator_s& op);
			
		private:

			//ÿ�ű���Ҫ���ʵ���
			HashMap<TableStmt*, Vector<ColumnDesc> > parent_table_access_column;
			HashMap<TableStmt*, Vector<ColumnDesc> > table_access_column; 
			HashMap<TableStmt*, RowDesc > table_access_row_desc;
			//��������������Ϣ
			HashMap<JoinableTables, JoinConditions> join_info;
			//ÿ�ű��Ĺ���ν��
			HashMap<TableStmt*, Expression_s> table_filters;
			//select list�����г��ֹ��ľۺϺ���
			Vector<Expression_s> aggr_exprs;
			//select list������ÿһ���������ʽ���ۺϺ���Ԥ�����������select_list��������
			Vector<Expression_s> select_list;
			//�µ��������
			Vector<String> select_list_name;
			//from list�ı�
			Vector<TableStmt*> table_list;
			//��������ѯ�ı�����ǰ��ѯ����������Ӳ�ѯ�������˸���ѯ�ı�
			Vector<TableStmt*> parent_table_list;
			//�����˵ĸ���ѯ�ı�
			Vector<TableStmt*> ref_parent_table_list;
			//��Ҫ������ʱ�������ã���ֹ�ڴ��ͷ�
			Vector<Stmt_s> tmp_table_handle;
			//�����Ӳ�ѯ������д������ͳһ���ɼƻ�
			Vector<Plan_s> subquerys;
			//����Ӳ�ѯ�����ν��
			Vector<Expression_s> corrected_predicates;
			//�ۺ���
			Vector<Expression_s> group_cols;
			//������
			Vector<Expression_s> sort_cols;
			//����Ǽ��ϲ�ѯ����ֱ𱣴����Ҽ��ϵļƻ�
			Plan_s first_plan, second_plan;
			//having����ν�ʱ���ʽ
			Expression_s having_filter;
			/*��������table scan��join��filter������or�������ӵ�˫��ν��*/
			Expression_s filter_after_join;
			//limit���
			u32 limit_offset, limit_size;
			//��ǰ�ڽ���select_list����having���飬�������־ۺϺ����Ľ�������
			u32 resolve_select_list_or_having;
			//��ѯ���ɵ���ʱ��ID
			u32 alias_table_id;
			//�û�ָ������ִ��
			u32 dop;
			//��ǰ�Ƿ����ڽ���where�Ӿ䣬�����ж��Ƿ��������ⲿ��ѯ��
			bool is_resolve_where;
			//��ѯ���Ƿ�ȥ��
			bool is_distinct;
			//�������Ƿ��ǲ�ѯ��������
			bool is_sort_query_result;
			//�Ƿ�����
			bool asc;
			//�Ƿ���limit
			bool have_limit;
			//��¼��ѯ���ĸ���Ϣ
			SelectPlan* root_plan;


			friend class DeletePlan;
			friend class UpdatePlan;
			friend class SqlRewriter;
		};
	}
}

#endif	//SELECT_PLAN_H