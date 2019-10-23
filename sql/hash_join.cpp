#include "hash_join.h"
#include "expression.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Sql;
using namespace CatDB::Common;

CatDB::Sql::HashJoin::HashJoin(const PhyOperator_s & left_child, const PhyOperator_s & right_child)
	:DoubleChildPhyOperator(left_child, right_child)
{

}

CatDB::Sql::HashJoin::~HashJoin()
{
}

PhyOperator_s CatDB::Sql::HashJoin::make_hash_join(const PhyOperator_s & left_child, 
	const PhyOperator_s & right_child,
	const Expression_s& equal_cond,
	const Expression_s& join_cond,
	u32 build_table_id)
{
	HashJoin* op = new HashJoin(left_child, right_child);
	op->set_equal_condition(equal_cond);
	op->set_join_condition(join_cond);
	op->set_build_table_id(build_table_id);
	op->init_hash_table();
	return PhyOperator_s(op);
}

u32 CatDB::Sql::HashJoin::set_equal_condition(const Expression_s & equal_cond)
{
	equal_condition = equal_cond;
	return SUCCESS;
}

Expression_s CatDB::Sql::HashJoin::get_equal_condition() const
{
	return equal_condition;
}

u32 CatDB::Sql::HashJoin::set_join_condition(const Expression_s & join_cond)
{
	join_condition = join_cond;
	return SUCCESS;
}

Expression_s CatDB::Sql::HashJoin::get_join_condition() const
{
	return join_condition;
}

u32 CatDB::Sql::HashJoin::set_build_table_id(u32 table_id)
{
	build_table_id = table_id;
	return SUCCESS;
}

u32 CatDB::Sql::HashJoin::get_table_id() const
{
	return u32();
}

u32 CatDB::Sql::HashJoin::open()
{
	u32 ret = left_child->open();
	if (ret == SUCCESS){
		ret = build_hash_table();
		if (ret != SUCCESS){
			return ret;
		}
		ret = right_child->open();
		if (ret == SUCCESS){
			return SUCCESS;
		}else{
			return ret;
		}
	}else{
		return ret;
	}
}

u32 CatDB::Sql::HashJoin::close()
{
	u32 ret1 = left_child->close();
	u32 ret2 = right_child->close();
	if (ret1 == SUCCESS && ret2 == SUCCESS){
		return SUCCESS;
	}else{
		return ret2;
	}
}

u32 CatDB::Sql::HashJoin::reset()
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

u32 CatDB::Sql::HashJoin::reopen(const Row_s & row)
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

u32 CatDB::Sql::HashJoin::get_next_row(Row_s & row)
{
	if (!probe_result.empty()){
		Row_s left_row = probe_result.front();
		probe_result.pop();
		row = Row::join_row(left_row, last_probe_row);
		return SUCCESS;
	}
	while (right_child->get_next_row(row) == SUCCESS){
		u32 ret = hash_table.probe(row, probe_result);
		if (ret == SUCCESS){
			last_probe_row = row;
			Row_s left_row = probe_result.front();
			probe_result.pop();
			row = Row::join_row(left_row, last_probe_row);
			return SUCCESS;
		}
	}
	return NO_MORE_ROWS;
}

u32 CatDB::Sql::HashJoin::type() const
{
	return PhyOperator::HASH_JOIN;
}

u32 CatDB::Sql::HashJoin::init_hash_table()
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
			if (table_id == build_table_id)
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
				Log(LOG_ERR, "HashJoin", "either 'equal' or 'and' operation found in equal condition");
				return ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION;
			}
		}else{
			Log(LOG_ERR, "HashJoin", "either 'equal' or 'and' operation found in equal condition");
			return ERROR_OPERATION_IN_EUQAL_JOIN_CONDITION;
		}
	}
	return SUCCESS;
}

u32 CatDB::Sql::HashJoin::build_hash_table()
{
	hash_table.clear();
	Row_s row;
	while (left_child->get_next_row(row) == SUCCESS){
		hash_table.build(row);
	}
	return SUCCESS;
}
