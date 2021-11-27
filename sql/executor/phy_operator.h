#ifndef PHY_OPERATOR_H
#define PHY_OPERATOR_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Sql {
		using Common::Row_s;
		using Common::Object_s;
		using Common::RowDesc;
		DECLARE(PhyOperator);
		DECLARE(Expression);
		DECLARE(PhyFilter);
		DECLARE(QueryCtx);
		
		//物理运算符接口
		class PhyOperator
		{
		public:
			enum OperatorType {
				TABLE_SCAN = 0,
				HASH_JOIN,
				NESTED_LOOP_JOIN,
				SORT,
				TOP_N_SORT,
				HASH_DISTINCT,
				HASH_GROUP,
				SCALAR_GROUP,
				HASH_SET,
				LIMIT,
				VIEW,
				INSERT,
				UPDATE,
				DELETE,
				EXPR_VALUE,
				DUAL_TABLE,
				SUBQUERY_EVALUATE
			};
			PhyOperator();
			virtual ~PhyOperator();

			//打开物理运算符。申请资源，打开子运算符等。
			virtual u32 open();
			virtual u32 inner_open() = 0;
			//关闭物理运算符。释放资源，关闭子运算符等。
			virtual u32 close() = 0;
			//重置算子，以便重新读取行
			virtual u32 reset() = 0;
			/**
			* 获得下一行的引用
			* @note 在下次调用get_next或者close前，返回的row有效
			* @pre 调用open()
			* @return SUCCESS或ITER_END或错误码
			*/
			u32 get_next_row(Row_s &row);
			virtual u32 inner_get_next_row(Row_s &row) = 0;
			//获取当前算子的类型
			virtual u32 type() const = 0;
			void set_output_exprs(u32 operator_id, const Vector<Expression_s> &exprs);
			u32 set_filter(const PhyFilter_s& filter);
			PhyFilter_s get_filter()const;
			void set_row_desc(RowDesc &desc) { output_desc = desc; }
			void set_query_ctx(QueryCtx_s &ctx);
		public:
			u32 make_row(Row_s &row);
			u32 make_const_row(Object_s &const_value, Row_s &row);
			u32 check_status();
			void increase_affected_rows();
			Vector<Expression_s> output_exprs;
			RowDesc output_desc;
			PhyFilter_s filter;
			Row_s cur_row;
			QueryCtx_s query_ctx;
		private:
			DISALLOW_COPY_AND_ASSIGN(PhyOperator)
		};

		class SingleChildPhyOperator :public PhyOperator
		{
		public:
			SingleChildPhyOperator(const PhyOperator_s& child);
			virtual ~SingleChildPhyOperator();
		protected:
			PhyOperator_s child;
		private:
			SingleChildPhyOperator() = delete;
		};

		class DoubleChildPhyOperator :public PhyOperator
		{
		public:
			DoubleChildPhyOperator(const PhyOperator_s& left, const PhyOperator_s& right);
			virtual ~DoubleChildPhyOperator();
		protected:
			PhyOperator_s left_child, right_child;
		private:
			DoubleChildPhyOperator() = delete;
		};

		class JoinPhyOperator :public DoubleChildPhyOperator
		{
		public:
			JoinPhyOperator(const PhyOperator_s& left, const PhyOperator_s& right);
			virtual ~JoinPhyOperator();
			JoinType join_type()const;
			void set_join_type(JoinType type);
			void set_outer_const_value(Object_s &value);
			u32 make_join_row(const Row_s &left_row, const Row_s &right_row, Row_s &row);
		protected:
			JoinType type;
			//outer join没有匹配时输出的默认值
			Object_s outer_const_value;
		private:
			JoinPhyOperator() = delete;
		};

		class MultiChildPhyOperator :public PhyOperator
		{
		public:
			MultiChildPhyOperator();
			virtual ~MultiChildPhyOperator();
			void add_child(const PhyOperator_s &child);
			void add_children(const Vector<PhyOperator_s> &children);
		protected:
			Vector<PhyOperator_s> children;
		};
	}
}

#endif	//PHY_OPERATOR_H