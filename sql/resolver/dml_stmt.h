#ifndef DML_STMT_H
#define DML_STMT_H
#include "bit_set.h"
#include "stmt.h"

namespace CatDB {
	namespace Parser {
		DECLARE(TableStmt);
		DECLARE(ExprStmt);
		DECLARE(ColumnStmt);
		DECLARE(AggrStmt);
		DECLARE(SubQueryStmt);
		DECLARE(SelectStmt);
		DECLARE(DMLStmt);
		DECLARE(HintStmt);

		class HintStmt
		{
		protected:
			HintStmt(bool single = true) : is_single(single) {}
		public:
			enum HintType{QB_NAME = 0, NO_REWRITE, JOIN, LEADING};
			~HintStmt() {}
			static HintStmt_s make_hint_stmt(HintType type);
			String get_type_name() const;
			bool has_qb_name() const { return !qb_name.empty(); }
			void set_qb_name(const String& name) { qb_name = name; }
			const String& get_qb_name() const { return qb_name; }
			HintType get_hint_type() const { return type; }
			bool is_single_hint() const { return is_single; }
			virtual String print_outline();

			VIRTUAL_KV_STRING(
				KV(type, get_type_name()),
				K(qb_name)
			);
		protected:
			HintType type;
			String qb_name;
			bool is_single;
		};

		DECLARE(JoinHintStmt);
		class JoinHintStmt : public HintStmt
		{
		private:
			JoinHintStmt() : HintStmt(false) {}
		friend class HintStmt;
		public:
			~JoinHintStmt() {}
			void set_join_algo(JoinAlgo algo) { join_algo = algo; }
			JoinAlgo get_join_algo() const { return join_algo; }
			String print_outline() override;

			KV_STRING(
				KV(type, get_type_name()),
				K(qb_name),
				KV(join_algo, JoinAlgoString[join_algo]),
				K(table_names)
			);
		public:
			Vector<String> table_names;
			BitSet table_ids;
			JoinAlgo join_algo;
		};

		DECLARE(LeadingHintStmt);
		DECLARE(LeadingTable);
		class LeadingTable {
		public:
			LeadingTable()
				:is_base_table(true){}
			LeadingTable(bool is_base)
				:is_base_table(is_base){}
			~LeadingTable() {}
			static LeadingTable_s make_leading_table();
			String to_kv_string()const;

			String table_name;
			u32 table_id;
			Vector<LeadingTable_s> table_list;
			bool is_base_table;
		};

		class LeadingHintStmt : public HintStmt
		{
		private:
			LeadingHintStmt()
			:is_ordered(false) {}
		friend class HintStmt;
		public:
			~LeadingHintStmt() {}
			void set_is_ordered() { is_ordered = true; }
			String print_outline() override;

			KV_STRING(
				KV(type, get_type_name()),
				K(qb_name),
				K(is_ordered),
				K(tables)
			);
		public:
			LeadingTable_s tables;
			bool is_ordered;
		};

		class Hint {
		public:
			KV_STRING(
				K(all_hints)
			);
			bool has_qb_name() const;
			String generate_qb_name(u32 stmt_id);
			String get_qb_name() const;
			String print_outline();
			bool enable_no_rewrite() const;
			void get_join_hints(Vector<JoinHintStmt_s> &join_hints);
			bool has_leading_hint();
			LeadingHintStmt_s get_leading_hint();

		public:
			Vector<HintStmt_s> all_hints;
		};

		class DMLStmt : public Stmt
		{
		public:
			DMLStmt();
		public:
			virtual ~DMLStmt();
			virtual StmtType stmt_type() const = 0;
			virtual u32 formalize();
			u32 get_stmt_exprs(Vector<ExprStmt_s> &exprs);
			u32 collect_special_exprs();
			u32 get_column_exprs(u32 table_id, Vector<ExprStmt_s> &columns);
			u32 get_table_items(Vector<TableStmt_s> &tables);
			Vector<ExprStmt_s> &get_column_exprs() { return column_exprs; }
			Vector<SubQueryStmt_s> &get_subquery_exprs() { return subquery_exprs; }
			u32 get_child_stmts(Vector<SelectStmt_s> &child_stms);
			u32 update_table_hint();
		private:
			bool update_join_hint(JoinHintStmt_s join_hint);

			void change_ordered_to_leading(LeadingHintStmt_s &leading_hint);

			void change_table_item_to_leading(TableStmt_s &table_item, LeadingTable_s &leading_table);

			bool update_leading_table_hint(LeadingTable_s &leading_hint);

			bool find_table_id(const String &table_name, u32 &table_id);

		protected:
			u32 collect_special_exprs(ExprStmt_s& expr);
			virtual u32 inner_get_stmt_exprs(Vector<ExprStmt_s> &exprs);

		public:
			VIRTUAL_KV_STRING(
				K(is_explain),
				K(stmt_hint),
				K(from_stmts),
				K(where_stmt)
			);

		public:
			Vector<TableStmt_s> from_stmts;			//from语句块
			Vector<ExprStmt_s> where_stmt;			//where语句块
			Hint stmt_hint;
			BitSet table_ids;
			u32 stmt_id;
			bool is_explain;

			Vector<ExprStmt_s> column_exprs;
			Vector<ExprStmt_s> aggr_exprs;
			Vector<SubQueryStmt_s> subquery_exprs;
		};
	}
}

#endif	//DML_STMT_H