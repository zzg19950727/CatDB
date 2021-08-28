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
		return NO_MORE_ROWS;
	}else{
		row = rows[pos++];
		return SUCCESS;
	}
}

u32 PhySort::type() const
{
	return PhyOperator::SORT;
}

bool PhySort::greater(const Row_s & lhs, const Row_s & rhs) const
{
	for (u32 i = 0; i < sort_exprs.size(); ++i){
		Object_s left = sort_exprs[i]->get_result(lhs);
		Object_s right = sort_exprs[i]->get_result(rhs);
		if (left->is_null() && right->is_null()) {
			continue;
		}
		else if (left->is_null()) {
			return true;
		}
		else if (right->is_null()) {
			return false;
		}
		else {
			Object_s result = left->operator==(right);
			if (result->bool_value())
				continue;
			result = left->operator>(right);
			if (result->bool_value())
				return true;
			else
				return false;
		}
	}
	return false;
}

bool PhySort::less(const Row_s & lhs, const Row_s & rhs) const
{
	for (u32 i = 0; i < sort_exprs.size(); ++i){
		Object_s left = sort_exprs[i]->get_result(lhs);
		Object_s right = sort_exprs[i]->get_result(rhs);
		if (left->is_null() && right->is_null()) {
			continue;
		}
		else if (left->is_null()) {
			return true;
		}
		else if (right->is_null()) {
			return false;
		}
		else {
			Object_s result = left->operator==(right);
			if (result->bool_value())
				continue;
			result = left->operator>(right);
			if (result->bool_value())
				return false;
			else
				return true;
		}
	}
	return false;
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
			ret = null_first && asc[i] || !null_first && !asc[i];
		} else if (right->is_null()) {
			return !(null_first && asc[i] || !null_first && !asc[i]);
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
	rows.clear();
	Row_s row;
	while (child->get_next_row(row) == SUCCESS){
		row = Row::deep_copy(row);
		rows.push_back(row);
	}
	auto cmp = [&](const Row_s& lhs, const Row_s& rhs) {return this->compare(lhs, rhs); };
	std::sort(rows.begin(), rows.end(), cmp);
	pos = 0;
	is_start = true;
	return SUCCESS;
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
	rows.clear();
	Row_s row;
	bool have_make_heap = false;
	auto compare_func = [&](const Row_s& lhs, const Row_s& rhs) {return this->compare(lhs, rhs); };
	while (child->get_next_row(row) == SUCCESS){
		if (rows.size() == topn) {
			if (!have_make_heap) {
				std::make_heap(rows.begin(), rows.end(), compare_func);
				have_make_heap = true;
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
	std::sort(rows.begin(), rows.end(), compare_func);
	pos = 0;
	is_start = true;
	return SUCCESS;
}
