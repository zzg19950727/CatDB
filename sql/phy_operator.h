#ifndef PHY_OPERATOR_H
#define PHY_OPERATOR_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Sql {
		using Common::Row_s;
		using Common::RowDesc;
		DECLARE(PhyOperator);

		//物理运算符接口
		class PhyOperator
		{
		public:
			enum OperatorType {
				TABLE_SCAN = 0,
				HASH_JOIN,
				MERGE_JOIN,
				NESTED_LOOP_JOIN,
				SORT,
				TOP_N_SORT,
				HASH_DISTINCT,
				MERGE_DISTINCT,
				HASH_GROUP,
				MERGE_GROUP,
				SCALAR_GROUP,
				HASH_SET,
				MERGE_SET,
				LIMIT,
				SUBPLAN_SCAN,
				SUBPLAN_FILTER,
				PLAN_FILTER,
				INSERT,
				UPDATE,
				DELETE,
				QUERY
			};
			PhyOperator();
			virtual ~PhyOperator();

			//打开物理运算符。申请资源，打开子运算符等。
			virtual u32 open() = 0;
			//关闭物理运算符。释放资源，关闭子运算符等。
			virtual u32 close() = 0;
			//重置算子，以便重新读取行
			virtual u32 reset() = 0;
			//重新打开算子，包括清理缓存、重置状态、重置过滤器
			//为subplan filter设计
			virtual u32 reopen(const Row_s& row) = 0;
			/**
			* 获得下一行的引用
			* @note 在下次调用get_next或者close前，返回的row有效
			* @pre 调用open()
			* @return SUCCESS或ITER_END或错误码
			*/
			virtual u32 get_next_row(Row_s &row) = 0;
			//获取当前算子的类型
			virtual u32 type() const = 0;
			void set_access_desc(const RowDesc& desc);
			void set_output_desc(const RowDesc& desc);
		protected:
			RowDesc access_desc;
			RowDesc output_desc;
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
			enum JoinType {
				Join=0,
				SemiJoin,
				AntiJoin,
				LeftOuterJoin,
				RightOuterJoin,
				FullOuterJoin
			};
			JoinPhyOperator(const PhyOperator_s& left, const PhyOperator_s& right);
			virtual ~JoinPhyOperator();
			JoinType join_type()const;
			void set_join_type(JoinType type);
		protected:
			JoinType type;
		private:
			JoinPhyOperator() = delete;
		};
	}
}

#endif	//PHY_OPERATOR_H