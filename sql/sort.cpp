#include "expression.h"
#include "object.h"
#include "error.h"
#include "sort.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

CatDB::Sql::Sort::Sort(PhyOperator_s & child)
	:SingleChildPhyOperator(child),
	pos(0),
	asc(true)
{
}

PhyOperator_s CatDB::Sql::Sort::make_sort(PhyOperator_s & child,
	const Vector<Expression_s>& sort_cols,
	bool asc)
{
	Sort* op = new Sort(child);
	op->sort_cols = sort_cols;
	op->set_asc(asc);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::Sort::add_sort_columns(const Expression_s & expr)
{
	sort_cols.push_back(expr);
	return SUCCESS;
}

void CatDB::Sql::Sort::set_asc(bool asc)
{
	this->asc = asc;
}

u32 CatDB::Sql::Sort::open()
{
	u32 ret = child->open();
	if (ret != SUCCESS){
		return ret;
	}else{
		return sort_rows();
	}
}

u32 CatDB::Sql::Sort::close()
{
	return child->close();
}

u32 CatDB::Sql::Sort::reset()
{
	pos = 0;
	return child->reset();
}

u32 CatDB::Sql::Sort::reopen(const Row_s & row)
{
	u32 ret = child->reopen(row);
	if (ret != SUCCESS){
		return ret;
	}else{
		return sort_rows();
	}
}

u32 CatDB::Sql::Sort::get_next_row(Row_s & row)
{
	if (pos >= rows.size()){
		return NO_MORE_ROWS;
	}else{
		row = rows[pos++];
		return SUCCESS;
	}
}

u32 CatDB::Sql::Sort::type() const
{
	return PhyOperator::SORT;
}

bool CatDB::Sql::Sort::greater(const Row_s & lhs, const Row_s & rhs) const
{
	for (u32 i = 0; i < sort_cols.size(); ++i){
		Object_s left = sort_cols[i]->get_result(lhs);
		Object_s right = sort_cols[i]->get_result(rhs);
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

bool CatDB::Sql::Sort::less(const Row_s & lhs, const Row_s & rhs) const
{
	for (u32 i = 0; i < sort_cols.size(); ++i){
		Object_s left = sort_cols[i]->get_result(lhs);
		Object_s right = sort_cols[i]->get_result(rhs);
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

u32 CatDB::Sql::Sort::sort_rows()
{
	rows.clear();
	Row_s row;
	while (child->get_next_row(row) == SUCCESS){
		rows.push_back(row);
	}
	if (asc){
		auto cmp = [&](const Row_s& lhs, const Row_s& rhs) {return this->less(lhs, rhs); };
		std::sort(rows.begin(), rows.end(), cmp);
	}else{
		auto cmp = [&](const Row_s& lhs, const Row_s& rhs) {return this->greater(lhs, rhs); };
		std::sort(rows.begin(), rows.end(), cmp);
	}
	pos = 0;
	return SUCCESS;
}

CatDB::Sql::TopNSort::TopNSort(PhyOperator_s & child, u32 topn)
	:SingleChildPhyOperator(child),
	pos(0),
	topn(topn),
	asc(true)
{
}

PhyOperator_s CatDB::Sql::TopNSort::make_topn_sort(PhyOperator_s & child,
	const Vector<Expression_s>& sort_cols,
	u32 topn,
	bool asc)
{
	TopNSort* op = new TopNSort(child, topn);
	op->sort_cols = sort_cols;
	op->set_asc(asc);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::TopNSort::add_sort_columns(const Expression_s & expr)
{
	sort_cols.push_back(expr);
	return SUCCESS;
}

void CatDB::Sql::TopNSort::set_asc(bool asc)
{
	this->asc = asc;
}

u32 CatDB::Sql::TopNSort::open()
{
	u32 ret = child->open();
	if (ret != SUCCESS){
		return ret;
	}else{
		return sort_rows();
	}
}

u32 CatDB::Sql::TopNSort::close()
{
	return child->close();
}

u32 CatDB::Sql::TopNSort::reset()
{
	pos = 0;
	return child->reset();
}

u32 CatDB::Sql::TopNSort::reopen(const Row_s & row)
{
	u32 ret = child->reopen(row);
	if (ret != SUCCESS){
		return ret;
	}else{
		return sort_rows();
	}
}

u32 CatDB::Sql::TopNSort::get_next_row(Row_s & row)
{
	if (pos >= topn || pos >= rows.size()) {
		return NO_MORE_ROWS;
	}else{
		row = rows[pos++];
		return SUCCESS;
	}
}

u32 CatDB::Sql::TopNSort::type() const
{
	return PhyOperator::TOP_N_SORT;
}

bool CatDB::Sql::TopNSort::greater(const Row_s & lhs, const Row_s & rhs) const
{
	for (u32 i = 0; i < sort_cols.size(); ++i){
		Object_s left = sort_cols[i]->get_result(lhs);
		Object_s right = sort_cols[i]->get_result(rhs);
		Object_s result = left->operator==(right);
		if (result->bool_value())
			continue;
		result = left->operator>(right);
		if (result->bool_value())
			return true;
		else
			return false;
	}
	return false;
}

bool CatDB::Sql::TopNSort::less(const Row_s & lhs, const Row_s & rhs) const
{
	for (u32 i = 0; i < sort_cols.size(); ++i){
		Object_s left = sort_cols[i]->get_result(lhs);
		Object_s right = sort_cols[i]->get_result(rhs);
		Object_s result = left->operator==(right);
		if (result->bool_value())
			continue;
		result = left->operator>(right);
		if (result->bool_value())
			return false;
		else
			return true;
	}
	return false;
}

u32 CatDB::Sql::TopNSort::sort_rows()
{
	rows.clear();
	Row_s row;
	bool have_make_heap = false;
	auto greater_func = [&](const Row_s& lhs, const Row_s& rhs) {return this->greater(lhs, rhs); };
	auto less_func = [&](const Row_s& lhs, const Row_s& rhs) {return this->less(lhs, rhs); };

	while (child->get_next_row(row) == SUCCESS){
		if (rows.size() == topn){
			if (!have_make_heap){
				if (asc)
					std::make_heap(rows.begin(), rows.end(), less_func);
				else
					std::make_heap(rows.begin(), rows.end(), greater_func);
				have_make_heap = true;
			}
			if (asc){
				if (greater(row, rows[0])){
					continue;
				}else{
					std::pop_heap(rows.begin(), rows.end(), less_func);
					rows[rows.size() - 1] = row;
					std::push_heap(rows.begin(), rows.end(), less_func);
				}
			}else{
				if (less(row, rows[0])){
					continue;
				}else{
					std::pop_heap(rows.begin(), rows.end(), greater_func);
					rows[rows.size() - 1] = row;
					std::push_heap(rows.begin(), rows.end(), greater_func);
				}
			}
		}else{
			rows.push_back(row);
		}
	}
	if (asc)
		std::sort(rows.begin(), rows.end(), less_func);
	else
		std::sort(rows.begin(), rows.end(), greater_func);
	pos = 0;
	return SUCCESS;
}
