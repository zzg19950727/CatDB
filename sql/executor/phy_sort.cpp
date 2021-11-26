#include "phy_expression.h"
#include "object.h"
#include "error.h"
#include "phy_sort.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

PhySort::PhySort(PhyOperator_s & child)
	:SingleChildPhyOperator(child),
	null_first(false),
	pos(0),
	is_start(false)
{
}

PhyOperator_s PhySort::make_sort(PhyOperator_s & child,
								const Vector<Expression_s>& sort_exprs,
								const Vector<bool> &asc)
{
	PhySort* op = new PhySort(child);
	op->sort_exprs = sort_exprs;
	op->asc = asc;
	return PhyOperator_s(op);
}

u32 PhySort::inner_open()
{
	return child->open();
}

u32 PhySort::close()
{
	return child->close();
}

u32 PhySort::reset()
{
	pos = 0;
	is_start = false;
	return child->reset();
}

u32 PhySort::inner_get_next_row(Row_s & row)
{
	u32 ret = SUCCESS;
	if (!is_start) {
		ret = sort_rows();
		if (FAIL(ret)) {
			return ret;
		}
	}
	if (pos >= rows.size()){
		ret = NO_MORE_ROWS;
	} else {
		row = rows[pos++];
	}
	return ret;
}

u32 PhySort::type() const
{
	return PhyOperator::SORT;
}

u32 PhySort::quick_sort(Vector<Row_s> &arr, int begin, int end)
{
	u32 ret = SUCCESS;
	//如果区间不只一个数
	if(begin < end)
	{
		Row_s temp = arr[begin]; //将区间的第一个数作为基准数
		int i = begin; //从左到右进行查找时的“指针”，指示当前左位置
		int j = end; //从右到左进行查找时的“指针”，指示当前右位置
		//不重复遍历
		while(i < j)
		{
			CHECK(check_status());
			//当右边的数大于基准数时，略过，继续向左查找
			//不满足条件时跳出循环，此时的j对应的元素是小于基准元素的
			while(i<j && compare(arr[j], temp)) {
				CHECK(check_status());
				j--;
			}
			//将右边小于等于基准元素的数填入右边相应位置
			arr[i] = arr[j];
			//当左边的数小于等于基准数时，略过，继续向右查找
			//(重复的基准元素集合到左区间)
			//不满足条件时跳出循环，此时的i对应的元素是大于等于基准元素的
			while(i<j && !compare(arr[i], temp)) {
				CHECK(check_status());
				i++;
			}
			//将左边大于基准元素的数填入左边相应位置
			arr[j] = arr[i];
		}
		//将基准元素填入相应位置
		arr[i] = temp;
		//此时的i即为基准元素的位置
		//对基准元素的左边子区间进行相似的快速排序
		quick_sort(arr,begin,i-1);
		//对基准元素的右边子区间进行相似的快速排序
		quick_sort(arr,i+1,end);
	}
	return ret;
}

bool PhySort::compare(const Row_s& lhs, const Row_s& rhs)const
{
	bool ret = true;
	for (u32 i = 0; ret && i < sort_exprs.size(); ++i){
		Object_s left = sort_exprs[i]->get_result(lhs);
		Object_s right = sort_exprs[i]->get_result(rhs);
		if (left->is_null() && right->is_null()) {
			ret = true;
		} else if (left->is_null()) {
			ret = (null_first && asc[i]) || (!null_first && !asc[i]);
		} else if (right->is_null()) {
			return !((null_first && asc[i]) || (!null_first && !asc[i]));
		} else {
			Object_s result = left->operator==(right);
			if (result->bool_value())
				continue;
			result = left->operator>(right);
			if (result->bool_value())
				return asc[i];
			else
				return !asc[i];
		}
	}
	return ret;
}

u32 PhySort::sort_rows()
{
	u32 ret = SUCCESS;
	rows.clear();
	Row_s row;
	while ((ret=child->get_next_row(row)) == SUCCESS) {
		row = Row::deep_copy(row);
		rows.push_back(row);
	}
	if (NO_MORE_ROWS == ret) {
		ret = SUCCESS;
	} else {
		return ret;
	}
	CHECK(quick_sort(rows, 0, rows.size() - 1));
	auto cmp = [&](const Row_s& lhs, const Row_s& rhs) {return this->compare(lhs, rhs); };
	//std::sort(rows.begin(), rows.end(), cmp);
	pos = 0;
	is_start = true;
	return ret;
}

PhyTopNSort::PhyTopNSort(PhyOperator_s & child, u32 topn)
	:PhySort(child),
	topn(topn)
{
}

PhyOperator_s PhyTopNSort::make_topn_sort(PhyOperator_s & child,
										const Vector<Expression_s>& sort_exprs,
										const Vector<bool> &asc,
										u32 topn)
{
	PhyTopNSort* op = new PhyTopNSort(child, topn);
	op->sort_exprs = sort_exprs;
	op->asc = asc;
	return PhyOperator_s(op);
}

u32 PhyTopNSort::type() const
{
	return PhyOperator::TOP_N_SORT;
}

u32 PhyTopNSort::sort_rows()
{
	u32 ret = SUCCESS;
	rows.clear();
	Row_s row;
	bool have_make_heap = false;
	auto compare_func = [&](const Row_s& lhs, const Row_s& rhs) {return this->compare(lhs, rhs); };
	while ((ret = child->get_next_row(row)) == SUCCESS) {
		if (rows.size() == topn) {
			if (!have_make_heap) {
				std::make_heap(rows.begin(), rows.end(), compare_func);
				have_make_heap = true;
			} else if (rows.empty()) {
				return ERR_UNEXPECTED;
			} else if (compare(row, rows[0])) {
				continue;
			} else {
				std::pop_heap(rows.begin(), rows.end(), compare_func);
				rows[rows.size() - 1] = Row::deep_copy(row);
				std::push_heap(rows.begin(), rows.end(), compare_func);
			}
		} else {
			row = Row::deep_copy(row);
			rows.push_back(row);
		}
	}
	quick_sort(rows, 0, rows.size() - 1);
	//std::sort(rows.begin(), rows.end(), compare_func);
	pos = 0;
	is_start = true;
	if (NO_MORE_ROWS == ret) {
		ret = SUCCESS;
	}
	return ret;
}
