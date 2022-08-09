#ifndef TABLE_STMT_H
#define TABLE_STMT_H
#include "bit_set.h"

namespace CatDB {
	namespace Parser {
		DECLARE(SelectStmt);
        DECLARE(ExprStmt);
		DECLARE(Stmt);

		//表的描述语句
		DECLARE(TableStmt);
		class TableStmt
		{
		protected:
			TableStmt();
		public:
			virtual ~TableStmt();
			
			String to_string()const {return alias_name;}
			virtual u32 deep_copy(TableStmt_s &table, u32 flag)const = 0;
			virtual u32 formalize() = 0;
			bool is_basic_table()const {return table_type == BasicTable;}
			bool is_joined_table()const {return table_type == JoinedTable;}
			bool is_view_table()const {return table_type == ViewTable;}
			virtual bool is_dual_table() const { return false; }
			bool is_basic_and_not_dual_table() const { return is_basic_table() && !is_dual_table(); }
			virtual u32 get_table_exprs(Vector<ExprStmt_s> &exprs);
			virtual u32 replace_exprs(const Vector<ExprStmt_s> &old_exprs, 
                                	  const Vector<ExprStmt_s> &new_exprs);
			void set_alias_name(const String& alias_name);
			VIRTUAL_KV_STRING(
				K(alias_name),
				K(table_id)
			);
			
		protected:
			u32 inner_deep_copy(TableStmt_s &table, u32 flag)const;

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
			bool is_dual_table() const override { return is_dual; }
			u32 deep_copy(TableStmt_s &table, u32 flag)const override;
			u32 formalize() override;
			
			KV_STRING_OVERRIDE(
				KV(table_type, TableTypeString[table_type]),
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
			static TableStmt_s make_joined_table(TableStmt_s &left_table,
												 TableStmt_s &right_table,
												 JoinType join_type);
			u32 deep_copy(TableStmt_s &table, u32 flag)const override;
			u32 formalize() override;
			u32 get_table_items(Vector<TableStmt_s> &table_items);
			u32 get_table_exprs(Vector<ExprStmt_s> &exprs)override;
			u32 replace_exprs(const Vector<ExprStmt_s> &old_exprs, 
                              const Vector<ExprStmt_s> &new_exprs)override;

			KV_STRING_OVERRIDE(
				KV(table_type, TableTypeString[table_type]),
				KV(join_type, JoinTypeString[join_type]),
				K(table_id),
				K(table_ids),
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
			ViewTableStmt(Stmt_s &ref_query);
		public:
			static TableStmt_s make_view_table(Stmt_s ref_query);
			u32 deep_copy(TableStmt_s &table, u32 flag)const override;
			u32 formalize() override;
			DECLARE_KV_STRING_OVERRIDE;
			SelectStmt_s ref_query;
		};
	}
}

#endif	//TABLE_STMT_H