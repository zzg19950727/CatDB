#ifndef HINT_STMT_H
#define HINT_STMT_H
#include "bit_set.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(HintStmt);
		class HintStmt
		{
		protected:
			HintStmt(HintType type, bool is_enable = true) 
			: 	type(type),
				is_enable_(is_enable),
				is_invalid_(false) {}
		public:
			virtual ~HintStmt() {}
			static HintStmt_s make_hint_stmt(HintType type, bool is_enable = true);
			bool has_qb_name() const { return !qb_name.empty(); }
			void set_qb_name(const String& name) { qb_name = name; }
			const String& get_qb_name() const { return qb_name; }
			HintType get_hint_type() const { return type; }
			void set_invalid(bool value) { is_invalid_ = value; }
			bool is_invalid() const { return is_invalid_; }


			virtual bool is_same_hint_type(const HintStmt_s &other) const;
			virtual bool is_equal(const HintStmt_s &other) const;
			virtual bool is_excluse(const HintStmt_s &other) const;

			virtual bool is_enable(const Vector<const void*> &params = Vector<const void*>()) const { return is_enable_; }
			virtual bool is_disable(const Vector<const void*> &params = Vector<const void*>()) const { return !is_enable_; }

			virtual u32 generate_outline(const Vector<const void*> &params = Vector<const void*>());
			virtual String print_outline() const;
			virtual u32 deep_copy(HintStmt_s &hint) const;


			virtual bool is_transform_hint() const { return false; }
			virtual bool is_optimizer_hint() const { return false; }
			virtual bool is_global_hint() const { return false; }

			VIRTUAL_KV_STRING(
				KV(type, HintTypeString[type]),
				K(qb_name),
				K(is_enable_)
			);
		public:
			HintType type;
			String qb_name;
			bool is_enable_;
			bool is_invalid_;
		};

		class TransformHint : public HintStmt {
		protected:
			TransformHint(HintType type, bool is_enable = true) : 
				HintStmt(type, is_enable) {}
		friend class HintStmt;
		public:
			bool is_transform_hint() const override { return true; }
		};

		class OptimizerHint : public HintStmt {
		protected:
			OptimizerHint(HintType type, bool is_enable = true) : 
				HintStmt(type, is_enable) {}
		friend class HintStmt;
		public:
			bool is_optimizer_hint() const override { return true; }
		};

		class GlobalHint : public HintStmt {
		protected:
			GlobalHint(HintType type, bool is_enable = true) : 
				HintStmt(type, is_enable) {}
		friend class HintStmt;
		public:
			bool is_global_hint() const override { return true; }
		};

		DECLARE(NoRewriteHintStmt);
		class NoRewriteHintStmt : public TransformHint {
		private:
			NoRewriteHintStmt() 
			: TransformHint(NO_REWRITE, false) {}
		friend class HintStmt;
		public:
			~NoRewriteHintStmt() {}
		};

		DECLARE(UnnestHintStmt);
		class UnnestHintStmt : public TransformHint {
		private:
			UnnestHintStmt(bool is_enable) 
			: TransformHint(UNNEST, is_enable)
			{}
		friend class HintStmt;
		public:
			~UnnestHintStmt() {}
		};

		DECLARE(MergeHintStmt);
		class MergeHintStmt : public TransformHint {
		private:
			MergeHintStmt(bool is_enable) 
			: TransformHint(MERGE, is_enable)
			{}
		friend class HintStmt;
		public:
			~MergeHintStmt() {}
		};

		DECLARE(SimplifySQHintStmt);
		class SimplifySQHintStmt : public TransformHint {
		private:
			SimplifySQHintStmt(bool is_enable) 
			: TransformHint(SIMPLIFY_SQ, is_enable)
			{}
		friend class HintStmt;
		public:
			~SimplifySQHintStmt() {}
		};

		DECLARE(ExprNormalizeHintStmt);
		class ExprNormalizeHintStmt : public TransformHint {
		private:
			ExprNormalizeHintStmt(bool is_enable) 
			: TransformHint(EXPR_NORMALIZE, is_enable)
			{}
		friend class HintStmt;
		public:
			~ExprNormalizeHintStmt() {}
		};

		DECLARE(WinMagicHintStmt);
		class WinMagicHintStmt : public TransformHint {
		private:
			WinMagicHintStmt(bool is_enable) 
			: TransformHint(WIN_MAGIC, is_enable)
			{}
		friend class HintStmt;
		public:
			~WinMagicHintStmt() {}
			virtual bool is_equal(const HintStmt_s &other) const override;
			virtual String print_outline() const override;
			virtual u32 deep_copy(HintStmt_s &hint) const override;

			virtual bool is_enable(const Vector<const void*> &params = Vector<const void*>()) const override;
			virtual u32 generate_outline(const Vector<const void*> &params = Vector<const void*>()) override;

			KV_STRING_OVERRIDE(
				KV(type, HintTypeString[type]),
				K(qb_name),
				K(dst_qb_name),
				K(is_enable_)
			);

			String dst_qb_name;
		};

		DECLARE(JoinHintStmt);
		class JoinHintStmt : public OptimizerHint
		{
		private:
			JoinHintStmt(bool is_enable) : OptimizerHint(USE_JOIN, is_enable) {}
		friend class HintStmt;
		public:
			~JoinHintStmt() {}
			void set_join_algo(JoinAlgo algo) { join_algo = algo; }
			JoinAlgo get_join_algo() const { return join_algo; }
			String print_outline() const override;
			u32 deep_copy(HintStmt_s &hint) const override;

			virtual bool is_same_hint_type(const HintStmt_s &other) const override;
			virtual bool is_equal(const HintStmt_s &other) const override;
			virtual bool is_excluse(const HintStmt_s &other) const override;

			virtual bool is_enable(const Vector<const void*> &params = Vector<const void*>()) const override;
			virtual u32 generate_outline(const Vector<const void*> &params = Vector<const void*>()) override;

			KV_STRING_OVERRIDE(
				KV(type, HintTypeString[type]),
				K(qb_name),
				KV(join_algo, JoinAlgoString[join_algo]),
				K(table_names),
				K(is_enable_)
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
			u32 deep_copy(LeadingTable_s &table) const;
			bool is_same_hint_type(const LeadingTable_s &other) const;

			String table_name;
			u32 table_id;
			Vector<LeadingTable_s> table_list;
			bool is_base_table;
		};

		class LeadingHintStmt : public OptimizerHint
		{
		private:
			LeadingHintStmt()
			:OptimizerHint(LEADING, true),
			is_ordered(false) {}
		friend class HintStmt;
		public:
			~LeadingHintStmt() {}
			void set_is_ordered() { is_ordered = true; }
			String print_outline() const override;
			u32 deep_copy(HintStmt_s &hint) const override;
			virtual bool is_equal(const HintStmt_s &other) const override;
			virtual bool is_excluse(const HintStmt_s &other) const override;

			KV_STRING_OVERRIDE(
				KV(type, HintTypeString[type]),
				K(qb_name),
				K(is_ordered),
				K(tables),
				K(is_enable_)
			);
		public:
			LeadingTable_s tables;
			bool is_ordered;
		};

		DECLARE(ParallelHintStmt);
		class ParallelHintStmt : public GlobalHint
		{
		private:
			ParallelHintStmt()
				:GlobalHint(PARALLEL, true),
				parallel(1) { }
			friend class HintStmt;
		public:
			~ParallelHintStmt() {}
			String print_outline() const override;
			u32 deep_copy(HintStmt_s &hint) const override;

			KV_STRING_OVERRIDE(
				KV(type, HintTypeString[type]),
				K(parallel),
				K(is_enable_)
			);
		public:
			int parallel;
		};

		DECLARE(HintManager);
		class HintManager {
		public:
			HintManager() = default;
			virtual ~HintManager() = default;

			enum HintStatus {NOT_SET_HINT=0, FORCE_ENABLE, FORCE_DISABLE};

			virtual void get_all_hints(Vector<HintStmt_s> &all_hints)const = 0;
			virtual u32 add_hint(HintStmt_s &hint) = 0;
			virtual void get_hint_status(const String &qb_name, 
										 HintType type, 
										 HintStatus &status,
										 const Vector<const void*> &params = Vector<const void*>()) = 0;

			virtual void find_hints(const String &qb_name, 
									HintType type, 
									Vector<HintStmt_s> &hints)const = 0;

			virtual u32 copy_hints(const String &src_qb_name, 
								   const String &dst_qb_name) = 0;

			virtual u32 merge_hints(const String &src_qb_name, 
									const String &dst_qb_name){return 0;}

			VIRTUAL_KV_STRING("");
		};

		DECLARE(StmtHintManager);
		class StmtHintManager : public HintManager {
		public:
			StmtHintManager() = default;
			virtual ~StmtHintManager() = default;

			static StmtHintManager_s make_stmt_hint_manager();
			virtual void get_all_hints(Vector<HintStmt_s> &all_hints)const override;
			virtual u32 add_hint(HintStmt_s &hint)override;
			virtual void get_hint_status(const String &qb_name, 
										 HintType type, 
										 HintStatus &status,
										 const Vector<const void*> &params = Vector<const void*>()) override;

			virtual void find_hints(const String &qb_name, 
									HintType type, 
									Vector<HintStmt_s> &hints)const override;

			virtual u32 copy_hints(const String &src_qb_name, 
								   const String &dst_qb_name) override;
			DECLARE_KV_STRING_OVERRIDE;
		public:
			typedef Vector<HintStmt_s> 	HintArray;
			typedef HashMap<HintType, HintArray>	HintTypeMap;
			UnorderedHashMap<String, HintTypeMap> hint_map;
		};

		DECLARE(OutlineHintManager);
		class OutlineHintManager : public HintManager {
		public:
			OutlineHintManager():
			index(0)
			{}
			virtual ~OutlineHintManager() = default;

			static OutlineHintManager_s make_outline_hint_manager();
			virtual void get_all_hints(Vector<HintStmt_s> &all_hints)const override;
			virtual u32 add_hint(HintStmt_s &hint)override;
			virtual void get_hint_status(const String &qb_name, 
										 HintType type, 
										 HintStatus &status,
										 const Vector<const void*> &params = Vector<const void*>()) override;

			virtual void find_hints(const String &qb_name, 
									HintType type, 
									Vector<HintStmt_s> &hints)const override;

			virtual u32 copy_hints(const String &src_qb_name, 
								   const String &dst_qb_name) override;

			KV_STRING_OVERRIDE(
				K(outline_hints)
			);
		public:
			Vector<HintStmt_s> outline_hints;
			u32 index;
		};

		DECLARE(QueryHint);
		class QueryHint {
		public:
			KV_STRING(
				K(global_hints),
				K(optimizer_hints),
				K(transformer_hints),
				K(generate_hints)
			);
			static QueryHint_s make_query_hint();
			void reset();
			u32 init(Vector<HintStmt_s> &all_hints, bool is_outline);
			String print_outline() const;
			
			void get_join_hints(const String &qb_name, Vector<JoinHintStmt_s> &join_hints);
			LeadingHintStmt_s get_leading_hint(const String &qb_name);
			bool has_leading_hint(const String &qb_name);

			//void merge_hints(const String &src_qb_name, const String &dst_qb_name, u32 flag);
			void copy_hints(const String &src_qb_name, const String &dst_qb_name);

			bool enable_transform(const String &qb_name, HintType type);
			bool enable_no_rewrite(const String &qb_name) const;
			bool enable_unnest(const String &qb_name) const;
			bool enable_no_unnest(const String &qb_name) const;
			bool enable_merge(const String &qb_name) const;
			bool enable_no_merge(const String &qb_name) const;
			bool enable_simplify_sq(const String &qb_name) const;
			bool enable_no_simplify_sq(const String &qb_name) const;
			bool enable_expr_normalize(const String &qb_name) const;
			bool enable_no_expr_normalize(const String &qb_name) const;
			bool enable_win_magic(const String &qb_name, const String &dst_qb_name) const;
			bool enable_no_win_magic(const String &qb_name, const String &dst_qb_name) const;
			bool enable_join_algo(const String &qb_name, const BitSet &right_table_ids, JoinAlgo algo);

			u32 generate_transform_outline(const String &qb_name, HintType type);
			u32 generate_win_magic_outline(const String &qb_name, HintType type, const String &dst_qb_name);
			u32 generate_join_outline(const String &qb_name, const Vector<String> &table_names, JoinAlgo join_algo);
			u32 generate_leading_outline(const String &qb_name, const LeadingTable_s &tables);

		public:
			HintManager_s global_hints;
			HintManager_s optimizer_hints;
			HintManager_s transformer_hints;
			HintManager_s generate_hints;
		};

		class Hint {
		public:
			KV_STRING(
				K(is_outline),
				K(qb_name),
				K(all_hints)
			);
			String generate_qb_name(u32 stmt_id);
			bool has_qb_name() const { return !qb_name.empty(); }
			bool is_outline;
			String qb_name;
			Vector<HintStmt_s> all_hints;
		};
	}
}

#endif	//HINT_STMT_H