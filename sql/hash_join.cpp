#include "hash_join.h"
#include "expression.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

HashJoin::HashJoin(const PhyOperator_s & left_child, const PhyOperator_s & right_child)
	:JoinPhyOperator(left_child, right_child)
{

}

HashJoin::~HashJoin()
{
}

PhyOperator_s HashJoin::make_hash_join(const PhyOperator_s & left_child, 
	const PhyOperator_s & right_child,
	const Expression_s& equal_cond,
	const Expression_s& join_cond,
	u32 probe_table_id)
{
	HashJoin* op = new HashJoin(left_child, right_child);
	op->set_equal_condition(equal_cond);
	op->set_join_condition(join_cond);
	op->set_probe_table_id(probe_table_id);
	op->init_hash_table();
	return PhyOperator_s(op);
}

u32 HashJoin::set_equal_condition(const Expression_s & equal_cond)
{
	equal_condition = equal_cond;
	return SUCCESS;
}

Expression_s HashJoin::get_equal_condition() const
{
	return equal_condition;
}

u32 HashJoin::set_join_condition(const Expression_s & join_cond)
{
	join_condition = join_cond;
	return SUCCESS;
}

Expression_s HashJoin::get_join_condition() const
{
	return join_condition;
}

void HashJoin::set_probe_table_id(u32 id)
{
	probe_table_id = id;
}

u32 HashJoin::get_probe_table_id() const
{
	return probe_table_id;
}

u32 HashJoin::open()
{
	u32 ret = left_child->open();
	if (ret == SUCCESS){
		ret = right_child->open();
		if (ret == SUCCESS){
			ret = build_hash_table();
			return ret;
		}else{
			return ret;
		}
	}else{
		return ret;
	}
}

u32 HashJoin::close()
{
	u32 ret1 = left_child->close();
	u32 ret2 = right_child->close();
	if (ret1 == SUCCESS && ret2 == SUCCESS){
		return SUCCESS;
	}else{
		return ret2;
	}
}

u32 HashJoin::reset()
{
	u32 ret = left_child->reset();
	if (ret == SUCCESS){
		ret = right_child->reset();
		if (ret == SUCCESS){
			while (!probe_result.empty())
				probe_result.pop();
			last_probe_row.reset();
			return SUCCESS;
		}else{
			return ret;
		}
	}else{
		return ret;
	}
}

u32 HashJoin::reopen(const Row_s & row)
{
	u32 ret1 = left_child->reopen(row);
	u32 ret2 = right_child->reopen(row);
	build_hash_table();
	while (!probe_result.empty())
		probe_result.pop();
	last_probe_row.reset();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 HashJoin::get_next_row(Row_s & row)
{
	switch (JoinPhyOperator::type) {
	case JoinPhyOperator::Join:
		return join(row);
	case JoinPhyOperator::SemiJoin:
		return semi_join(row);
	case JoinPhyOperator::LeftOuterJoin:
		return left_outer_join(row);
	case JoinPhyOperator::RightOuterJoin:
		return right_outer_join(row);
	case JoinPhyOperator::FullOuterJoin:
		return full_outer_join(row);
	default:
		return NO_MORE_ROWS;
	}
}

u32 HashJoin::type() const
{
	return PhyOperator::HASH_JOIN;
}

u32 HashJoin::explain_operator(u32 depth, QueryResult * result)
{
	String join_name;
	switch (JoinPhyOperator::type) {
	case JoinPhyOperator::Join:
		join_name = "hash join";
		break;
	case JoinPhyOperator::SemiJoin:
		join_name = "hash semi join";
		break;
	case JoinPhyOperator::LeftOuterJoin:
		join_name = "hash left outer join";
		break;
	case JoinPhyOperator::RightOuterJoin:
		join_name = "hash right outer join";
		break;
	case JoinPhyOperator::FullOuterJoin:
		join_name = "hash full outer join";
		break;
	}
	result->add_operation_info(depth, join_name, "", output_rows, finished_time);
	left_child->explain_operator(depth + 1, result);
	return right_child->explain_operator(depth + 1, result);
}

u32 HashJoin::init_hash_table()
{
	hash_table.clear_hash_columns();
	hash_table.clear_probe_columns();
	hash_table.set_probe_condition(join_condition);
	Queue<Expression_s> expr_queue;
	expr_queue.push(equal_condition);
	while (!expr_queue.empty()){
		Expression_s expr = expr_queue.front();
		expr_queue.pop();
		if (expr->get_type() == Expression::Column){
			ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
			const ColumnDesc& desc = col_expr->get_column_desc();
			u32 table_id, column_id;
			desc.get_tid_cid(table_id, column_id);
			if (table_id != probe_table_id)
				hash_table.add_hash_column(expr);
			else
				hash_table.add_probe_column(expr);
		}else if (expr->get_type() == Expression::Binary){
			BinaryExpression* bin_expr = dynamic_cast<BinaryExpression*>(expr.get());
			if (bin_expr->op.get_type() == Parser::ExprStmt::OP_AND){
				expr_queue.push(bin_expr->first_expr);
				expr_queue.push(bin_expr->second_expr);
			}else if (bin_expr->op.get_type() == Parser::ExprStmt::OP_EQ){
				expr_queue.push(bin_expr->first_expr);
				expr_queue.push(bin_expr->second_expr);
			}else{
				LOG_ERR("either 'equal' or 'and' operation found in equal condition");
				return ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION;
			}
		}else{
			LOG_ERR("either 'equal' or 'and' operation found in equal condition");
			return ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION;
		}
	}
	return SUCCESS;
}

u32 HashJoin::build_hash_table()
{
	hash_table.clear();
	Row_s row;
	while (left_child->get_next_row(row) == SUCCESS) {
		hash_table.build(row);
	}
	return SUCCESS;
}

u32 HashJoin::join(Row_s & row)
{
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		row = Row::join_row(left_row, last_probe_row);
		return SUCCESS;
	}
	while (right_child->get_next_row(row) == SUCCESS) {
		u32 ret = hash_table.probe(row, probe_result);
		if (ret == SUCCESS) {
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			row = Row::join_row(left_row, last_probe_row);
			return SUCCESS;
		}
	}
	return NO_MORE_ROWS;
}

u32 HashJoin::semi_join(Row_s & row)
{
	if (!probe_result.empty()) {
		Row_s left_row = probe_result.front();
		probe_result.pop();
		row = left_row;
		return SUCCESS;
	}
	while (right_child->get_next_row(row) == SUCCESS) {
		u32 ret = hash_table.probe(row, probe_result);
		if (ret == SUCCESS) {
			hash_table.probe_and_drop(row);
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			row = left_row;
			return SUCCESS;
		}
	}
	return NO_MORE_ROWS;
}

u32 HashJoin::anti_join(Row_s & row)
{
	return NO_MORE_ROWS;
}

u32 HashJoin::left_outer_join(Row_s & row)
{
	return NO_MORE_ROWS;
}

u32 HashJoin::right_outer_join(Row_s & row)
{
	return NO_MORE_ROWS;
}

u32 HashJoin::full_outer_join(Row_s & row)
{
	return NO_MORE_ROWS;
}

HashNullAwareAntiJoin::HashNullAwareAntiJoin(const PhyOperator_s & left_child, const PhyOperator_s & right_child)
	:JoinPhyOperator(left_child, right_child),
	cur_bucket_pos(0)
{
	cur_bucket = hash_table.end_bucket();
}

HashNullAwareAntiJoin::~HashNullAwareAntiJoin()
{
}

PhyOperator_s HashNullAwareAntiJoin::make_hash_null_aware_anti_join(
	const PhyOperator_s & left_child, 
	const PhyOperator_s & right_child, 
	const Expression_s & anti_equal_cond, 
	const Expression_s& equal_cond,
	const Expression_s & join_cond, 
	u32 probe_table_id)
{
	HashNullAwareAntiJoin* op = new HashNullAwareAntiJoin(left_child, right_child);
	op->set_anti_equal_condition(anti_equal_cond);
	op->set_equal_condition(equal_cond);
	op->set_join_condition(join_cond);
	op->set_probe_table_id(probe_table_id);
	op->init_hash_table();
	return PhyOperator_s(op);
}

u32 HashNullAwareAntiJoin::set_anti_equal_condition(const Expression_s & anti_equal_cond)
{
	anti_equal_condition = anti_equal_cond;
	return SUCCESS;
}

Expression_s HashNullAwareAntiJoin::get_anti_equal_condition() const
{
	return anti_equal_condition;
}

u32 HashNullAwareAntiJoin::set_equal_condition(const Expression_s & equal_cond)
{
	equal_condition = equal_cond;
	return SUCCESS;
}

Expression_s HashNullAwareAntiJoin::get_equal_condition() const
{
	return equal_condition;
}

u32 HashNullAwareAntiJoin::set_join_condition(const Expression_s & join_cond)
{
	join_condition = join_cond;
	return SUCCESS;
}

Expression_s HashNullAwareAntiJoin::get_join_condition() const
{
	return join_condition;
}

void HashNullAwareAntiJoin::set_probe_table_id(u32 id)
{
	probe_table_id = id;
}

u32 HashNullAwareAntiJoin::get_probe_table_id() const
{
	return probe_table_id;
}

u32 HashNullAwareAntiJoin::open()
{
	u32 ret = left_child->open();
	if (ret == SUCCESS) {
		ret = right_child->open();
		if (ret == SUCCESS) {
			ret = build_hash_table();
			return ret;
		}
		else {
			return ret;
		}
	}
	else {
		return ret;
	}
}

u32 HashNullAwareAntiJoin::close()
{
	u32 ret1 = left_child->close();
	u32 ret2 = right_child->close();
	if (ret1 == SUCCESS && ret2 == SUCCESS) {
		return SUCCESS;
	}
	else {
		return ret2;
	}
}

u32 HashNullAwareAntiJoin::reset()
{
	u32 ret = left_child->reset();
	if (ret == SUCCESS) {
		ret = right_child->reset();
		if (ret == SUCCESS) {
			return SUCCESS;
		}
		else {
			return ret;
		}
	}
	else {
		return ret;
	}
}

u32 HashNullAwareAntiJoin::reopen(const Row_s & row)
{
	u32 ret1 = left_child->reopen(row);
	u32 ret2 = right_child->reopen(row);
	build_hash_table();
	if (ret1 != SUCCESS)
		return ret1;
	else if (ret2 != SUCCESS)
		return ret2;
	else
		return SUCCESS;
}

u32 HashNullAwareAntiJoin::get_next_row(Row_s & row)
{
	while (cur_bucket != hash_table.end_bucket()) {
		const Vector<Row_s>& bucket = hash_table.bucket(cur_bucket);
		while (cur_bucket_pos < bucket.size()) {
			row = bucket[cur_bucket_pos++];
			return SUCCESS;
		}
		hash_table.next_none_empty_bucket(cur_bucket);
		cur_bucket_pos = 0;
	}
	return NO_MORE_ROWS;
}

u32 HashNullAwareAntiJoin::type() const
{
	return PhyOperator::HASH_NULL_AWARE_ANTI_JOIN;
}

u32 HashNullAwareAntiJoin::explain_operator(u32 depth, QueryResult * result)
{
	result->add_operation_info(depth, "hash null aware anti join", "", output_rows, finished_time);
	left_child->explain_operator(depth + 1, result);
	return right_child->explain_operator(depth + 1, result);
}

u32 HashNullAwareAntiJoin::init_hash_table()
{
	hash_table.clear_hash_columns();
	hash_table.clear_probe_columns();
	hash_table.set_probe_condition(join_condition);
	if (equal_condition) {
		Queue<Expression_s> expr_queue;
		expr_queue.push(equal_condition);
		while (!expr_queue.empty()) {
			Expression_s expr = expr_queue.front();
			expr_queue.pop();
			if (expr->get_type() == Expression::Column) {
				ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(expr.get());
				const ColumnDesc& desc = col_expr->get_column_desc();
				u32 table_id, column_id;
				desc.get_tid_cid(table_id, column_id);
				if (table_id != probe_table_id) {
					hash_table.add_hash_column(expr);
				}
				else {
					hash_table.add_probe_column(expr);
				}
			}
			else if (expr->get_type() == Expression::Binary) {
				BinaryExpression* bin_expr = dynamic_cast<BinaryExpression*>(expr.get());
				if (bin_expr->op.get_type() == Parser::ExprStmt::OP_AND) {
					expr_queue.push(bin_expr->first_expr);
					expr_queue.push(bin_expr->second_expr);
				}
				else if (bin_expr->op.get_type() == Parser::ExprStmt::OP_EQ) {
					expr_queue.push(bin_expr->first_expr);
					expr_queue.push(bin_expr->second_expr);
				}
				else {
					LOG_ERR("either 'anti equal' or 'and' operation found in equal condition");
					return ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION;
				}
			}
			else {
				LOG_ERR("either 'anti equal' or 'and' operation found in equal condition");
				return ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION;
			}
		}
	}
	if (anti_equal_condition) {
		if (anti_equal_condition->get_type() == Expression::Binary) {
			BinaryExpression* bin_expr = dynamic_cast<BinaryExpression*>(anti_equal_condition.get());
			if (bin_expr->first_expr->get_type() == Expression::Column) {
				ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(bin_expr->first_expr.get());
				u32 table_id, column_id;
				col_expr->col_desc.get_tid_cid(table_id, column_id);
				if (table_id != probe_table_id) {
					anti_build_col = bin_expr->first_expr;
					hash_table.add_hash_column(bin_expr->first_expr);
				}
				else {
					anti_probe_col = bin_expr->first_expr;
					hash_table.add_probe_column(bin_expr->first_expr);
				}
			}
			if (bin_expr->second_expr->get_type() == Expression::Column) {
				ColumnExpression* col_expr = dynamic_cast<ColumnExpression*>(bin_expr->second_expr.get());
				u32 table_id, column_id;
				col_expr->col_desc.get_tid_cid(table_id, column_id);
				if (table_id != probe_table_id) {
					anti_build_col = bin_expr->second_expr;
					hash_table.add_hash_column(bin_expr->second_expr);
				}
				else {
					anti_probe_col = bin_expr->second_expr;
					hash_table.add_probe_column(bin_expr->second_expr);
				}
			}
		}
	}
	return SUCCESS;
}

u32 HashNullAwareAntiJoin::build_hash_table()
{
	hash_table.clear();
	Row_s row;
	bool right_empty = true;
	bool right_have_null = false;

	if (anti_probe_col) {
		while (right_child->get_next_row(row) == SUCCESS) {
			right_empty = false;
			Object_s result = anti_probe_col->get_result(row);
			if (result->is_null()) {
				right_have_null = true;
				break;
			}
		}
	}
	if (!right_have_null) {
		while (left_child->get_next_row(row) == SUCCESS) {
			if (!right_empty && anti_build_col) {
				if (anti_build_col) {
					Object_s result = anti_build_col->get_result(row);
					if (result->is_null()) {
						continue;
					}
				}
			}
			hash_table.build(row);
		}
		right_child->reset();
		while (right_child->get_next_row(row) == SUCCESS) {
			hash_table.probe_and_drop(row);
		}
	}
	cur_bucket = hash_table.begin_bucket();
	cur_bucket_pos = 0;
	return SUCCESS;
}
