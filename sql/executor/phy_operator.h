﻿#ifndef PHY_OPERATOR_H
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
		
		DECLARE(ExecCtx);
		DECLARE(ParamStore);
		class ParamStore {
		public:
			static ParamStore_s make_param_store();
			const Object_s& get_value(u32 index);
			void set_value(u32 index, const Object_s &value);
			HashMap<u32, Object_s> value_map;
		};
		class ExecCtx {
		public:
			static ExecCtx_s make_exec_ctx();
			static ExecCtx_s make_exec_ctx(const ParamStore_s& param_store);
			u32 set_input_rows(const Row_s &row);
			u32 set_input_rows(const Row_s &row1, const Row_s &row2);
			Vector<Row_s> input_rows;
			ParamStore_s param_store;
			Object_s output_result;
			PhyOperator* cur_op;
			bool bool_result;
		};
		
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
				PARTITION_SORT,
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
				SUBQUERY_EVALUATE,
				WINDOWN_FUNCTION
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
			virtual u32 inner_get_next_row() = 0;
			//获取当前算子的类型
			virtual u32 type() const = 0;
			void set_output_exprs(const Vector<Expression_s> &exprs);
			void set_null_values(const Vector<Object_s> &null_values);
			u32 set_filter(const Vector<Expression_s>& filter);
			void set_operator_id(u32 id);
			void set_exec_ctx(ExecCtx_s &ctx);
			void set_input_rows(const Row_s &row);
			void set_input_rows(const Row_s &row1, const Row_s &row2);
		public:
			u32 make_row(Row_s &row);
			u32 make_const_row(Row_s &row);
			u32 check_status();
			void increase_affected_rows();
			Vector<Expression_s> output_exprs;
			Vector<Object_s> const_values;
			Vector<Expression_s> filters;
			Row_s cur_row;
            u32 operator_id;
			ExecCtx_s exec_ctx;
			u32 check_row_count;
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
		protected:
			JoinType type;
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