#ifndef HINT_STMT_H
#define HINT_STMT_H
#include "bit_set.h"

namespace CatDB {
	namespace Parser {
		DECLARE(HintStmt);
		class HintStmt
		{
		protected:
			HintStmt(bool single = true) : 
				is_single(single),
				is_global(false) {}
		public:
			enum HintType{QB_NAME = 0, NO_REWRITE, JOIN, LEADING, PARALLEL};
			virtual ~HintStmt() {}
			static HintStmt_s make_hint_stmt(HintType type);
			String get_type_name() const;
			bool has_qb_name() const { return !qb_name.empty(); }
			void set_qb_name(const String& name) { qb_name = name; }
			const String& get_qb_name() const { return qb_name; }
			HintType get_hint_type() const { return type; }
			bool is_single_hint() const { return is_single; }
			bool is_global_hint() const { return is_global; }
			virtual String print_outline();
			virtual u32 deep_copy(HintStmt_s &hint, u32 flag) const;

			VIRTUAL_KV_STRING(
				KV(type, get_type_name()),
				K(qb_name)
			);
		public:
			HintType type;
			String qb_name;
			bool is_single;
			bool is_global;
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
			u32 deep_copy(HintStmt_s &hint, u32 flag) const override;

			KV_STRING_OVERRIDE(
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
			u32 deep_copy(LeadingTable_s &table, u32 flag) const;

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
			u32 deep_copy(HintStmt_s &hint, u32 flag) const override;

			KV_STRING_OVERRIDE(
				KV(type, get_type_name()),
				K(qb_name),
				K(is_ordered),
				K(tables)
			);
		public:
			LeadingTable_s tables;
			bool is_ordered;
		};

		DECLARE(ParallelHintStmt);
		class ParallelHintStmt : public HintStmt
		{
		private:
			ParallelHintStmt()
				:parallel(1),
				HintStmt(true) { is_global = true; }
			friend class HintStmt;
		public:
			~ParallelHintStmt() {}
			String print_outline() override;
			u32 deep_copy(HintStmt_s &hint, u32 flag) const override;

			KV_STRING_OVERRIDE(
				KV(type, get_type_name()),
				K(parallel)
			);
		public:
			int parallel;
		};

		class Hint {
		public:
			KV_STRING(
				K(all_hints)
			);
			bool has_qb_name() const;
			String generate_qb_name(u32 stmt_id);
			String get_qb_name() const;
			String print_outline(bool &print_global_hint);
			bool enable_no_rewrite() const;
			void get_join_hints(Vector<JoinHintStmt_s> &join_hints);
			bool has_leading_hint();
			LeadingHintStmt_s get_leading_hint();
			u32 deep_copy(Hint& hint, u32 flag)const;
			u32 formalize();
			u32 reset_qb_name(u32 stmt_id);

			void add_hint(HintStmt_s hint);
			bool has_hint(HintStmt::HintType type) const;
			void remove_hint(HintStmt::HintType type);
			HintStmt_s get_hint(HintStmt::HintType type) const;
			void get_hints(HintStmt::HintType type, Vector<HintStmt_s> &hints) const;

		public:
			Vector<HintStmt_s> all_hints;
			String qb_name;
		};
	}
}

#endif	//HINT_STMT_H