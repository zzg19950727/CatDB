﻿#include "phy_expression.h"
#include "query_result.h"
#include "phy_operator.h"
#include "sql_engine.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;

Operation::Operation(OperationType type)
	:type(type)
{
}

Operation::~Operation()
{
}

void Operation::set_type(OperationType type)
{
	this->type = type;
}

OperationType Operation::get_type() const
{
	return type;
}

Object_s Operation::calc(Object_s & obj)
{
	if (obj->get_type() == T_QUERY_RESULT 
		&& !(type == OP_EXISTS || type == OP_NOT_EXISTS)) {
		QueryResult_s result = obj;
		u32 ret = result->cast_to_simple_object(obj);
		if (ret != SUCCESS) {
			return Error::make_object(ret);
		}
	}
	switch (type)
	{
	case OP_MINUS:
		return do_minus(obj);
	case OP_NOT:
		return do_not(obj);
	case OP_EXISTS:
		return do_exists(obj);
	case OP_NOT_EXISTS:
		return do_not_exists(obj);
	case OP_IS_NULL:
		return do_is_null(obj);
	case OP_IS_NOT_NULL:
		return do_is_not_null(obj);
	default:
		LOG_ERR("wrong calc function called for opertion");
		return Error::make_object(WRONG_CALC_FOR_OP);
	}
}

Object_s Operation::calc(Object_s & first_obj, Object_s & second_obj)
{
	if (first_obj->get_type() == T_QUERY_RESULT) {
		QueryResult_s result = first_obj;
		u32 ret = result->cast_to_simple_object(first_obj);
		if (ret != SUCCESS) {
			return Error::make_object(ret);
		}
	}
	else if (second_obj->get_type() == T_QUERY_RESULT 
		&& !(type == OP_IN || type == OP_NOT_IN)) {
		QueryResult_s result = second_obj;
		u32 ret = result->cast_to_simple_object(second_obj);
		if (ret != SUCCESS) {
			return Error::make_object(ret);
		}
	}
	switch (type)
	{
	case OP_ADD:
		return do_add(first_obj, second_obj);
	case OP_SUB:
		return do_sub(first_obj, second_obj);
	case OP_MUL:
		return do_mul(first_obj, second_obj);
	case OP_DIV:
		return do_div(first_obj, second_obj);
	case OP_EQ:
		return do_equal(first_obj, second_obj);
	case OP_NE:
		return do_not_equal(first_obj, second_obj);
	case OP_GT:
		return do_greater(first_obj, second_obj);
	case OP_GE:
		return do_GE(first_obj, second_obj);
	case OP_LT:
		return do_less(first_obj, second_obj);
	case OP_LE:
		return do_LE(first_obj, second_obj);
	case OP_IN:
		return do_in(first_obj, second_obj);
	case OP_NOT_IN:
		return do_not_in(first_obj, second_obj);
	case OP_AND:
		return do_and(first_obj, second_obj);
	case OP_OR:
		return do_or(first_obj, second_obj);
	case OP_LIKE:
		return do_like(first_obj, second_obj);
	case OP_NOT_LIKE:
		return do_not_like(first_obj, second_obj);
	case OP_CAST:
		return do_cast(first_obj, second_obj);
	default:
		LOG_ERR("wrong calc function called for opertion");
		return Error::make_object(WRONG_CALC_FOR_OP);
	}
}

Object_s Operation::calc(Object_s & first_obj, Object_s & second_obj, Object_s & third_obj)
{
	switch (type)
	{
	case OP_BETWEEN:
		return do_between(first_obj, second_obj, third_obj);
	case OP_NOT_BETWEEN:
		return do_not_between(first_obj, second_obj, third_obj);
	default:
		LOG_ERR("wrong calc function called for opertion");
		return Error::make_object(WRONG_CALC_FOR_OP);
	}
}

Object_s Operation::do_add(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator+(second_obj);
}

Object_s Operation::do_sub(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator-(second_obj);
}

Object_s Operation::do_mul(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator*(second_obj);
}

Object_s Operation::do_div(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator/(second_obj);
}

Object_s Operation::do_equal(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator==(second_obj);
}

Object_s Operation::do_anti_equal(Object_s & first_obj, Object_s & second_obj)
{
	if (first_obj->is_null() || second_obj->is_null()) {
		return Bool::make_object(true);
	}
	else {
		return first_obj->operator==(second_obj);
	}
}

Object_s Operation::do_not_equal(Object_s & first_obj, Object_s & second_obj)
{
	Object_s result = first_obj->operator==(second_obj);
	return result->op_not();
}

Object_s Operation::do_greater(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator>(second_obj);
}

Object_s Operation::do_GE(Object_s & first_obj, Object_s & second_obj)
{
	Object_s result = first_obj->operator>(second_obj);
	if (result->get_type() == T_ERROR_RESULT) {
		return result;
	}
	else if (result->is_null()) {
		return result;
	}
	else if (result->bool_value()) {
		return result;
	}
	else {
		result = first_obj->operator==(second_obj);
		return result;
	}
}

Object_s Operation::do_less(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator<(second_obj);
}

Object_s Operation::do_LE(Object_s & first_obj, Object_s & second_obj)
{
	Object_s result = first_obj->operator<(second_obj);
	if (result->get_type() == T_ERROR_RESULT) {
		return result;
	}
	else if (result->is_null()) {
		return result;
	}
	else if (result->bool_value()) {
		return result;
	}
	else {
		result = first_obj->operator==(second_obj);
		return result;
	}
}

Object_s Operation::do_between(Object_s & first_obj, Object_s & second_obj, Object_s & third_obj)
{
	return first_obj->between(second_obj, third_obj);
}

Object_s Operation::do_not_between(Object_s & first_obj, Object_s & second_obj, Object_s & third_obj)
{
	Object_s result = first_obj->between(second_obj, third_obj);
	return result->op_not();
}

Object_s Operation::do_is_null(Object_s & first_obj)
{
	return Bool::make_object(first_obj->is_null());
}

Object_s Operation::do_is_not_null(Object_s & first_obj)
{
	return Bool::make_object(!first_obj->is_null());
}

Object_s Operation::do_in(Object_s & first_obj, Object_s & second_obj)
{
	return second_obj->in(first_obj);
}

Object_s Operation::do_not_in(Object_s & first_obj, Object_s & second_obj)
{
	return second_obj->not_in(first_obj);
}

Object_s Operation::do_exists(Object_s & first_obj)
{
	return first_obj->exists();
}

Object_s Operation::do_not_exists(Object_s & first_obj)
{
	Object_s result = first_obj->exists();
	return result->op_not();
}

Object_s Operation::do_and(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj-> op_and (second_obj);
}

Object_s Operation::do_or(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj-> op_or (second_obj);
}

Object_s Operation::do_not(Object_s & first_obj)
{
	return first_obj->op_not();
}

Object_s Operation::do_minus(Object_s & first_obj)
{
	return first_obj->op_minus();
}

Object_s Operation::do_like(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->like(second_obj);
}

Object_s Operation::do_not_like(Object_s & first_obj, Object_s & second_obj)
{
	Object_s result = first_obj->like(second_obj);
	return result->op_not();
}

Object_s Operation::do_cast(Object_s& first_obj, Object_s& second_obj)
{
	cast_to(second_obj->get_type(), first_obj);
	return first_obj;
}

Expression::Expression()
{
}

Expression::~Expression()
{
}

ConstExpression::ConstExpression(const Object_s& object)
	:const_object(object)
{
}

Expression_s ConstExpression::make_const_expression(const Object_s & object)
{
	return Expression_s(new ConstExpression(object));
}

u32 ConstExpression::set_object(Object_s & object)
{
	const_object = object;
	return SUCCESS;
}

Object_s ConstExpression::get_result(const Row_s & row)
{
	if (const_object)
		return const_object;
	else
		return Error::make_object(INVALID_OBJECT);
}

Expression::ExprType ConstExpression::get_type() const
{
	return Expression::Const;
}

ExecParamExpression::ExecParamExpression(const Expression_s& ref_expr)
	:ref_expr(ref_expr)
{

}

Expression_s ExecParamExpression::make_exec_param_expression(const Expression_s & ref_expr)
{
	return Expression_s(new ExecParamExpression(ref_expr));
}

u32 ExecParamExpression::set_value(const Row_s & row)
{
	Object_s value = ref_expr->get_result(row);
	return set_object(value);
}

Expression::ExprType ExecParamExpression::get_type() const
{
	return Expression::ExecParam;
}

ColumnExpression::ColumnExpression(const ColumnDesc& desc)
	:col_desc(desc)
{
}

Expression_s ColumnExpression::make_column_expression(const ColumnDesc & desc)
{
	return Expression_s(new ColumnExpression(desc));
}

u32 ColumnExpression::set_column_desc(ColumnDesc & desc)
{
	col_desc = desc;
	return SUCCESS;
}

const ColumnDesc & ColumnExpression::get_column_desc() const
{
	return col_desc;
}

Object_s ColumnExpression::get_result(const Row_s & row)
{
	if (!row) {
		return Error::make_object(COLUMN_NOT_EXISTS);
	}
	Object_s cell;
	u32 ret = row->get_cell(col_desc, cell);
	if (ret == SUCCESS)
		return cell;
	else
		return Error::make_object(ret);
}

Expression::ExprType ColumnExpression::get_type() const
{
	return Expression::Column;
}

SetExpression::SetExpression(u32 idx)
	:index(idx)
{

}

Expression_s SetExpression::make_set_expression(u32 idx)
{
	SetExpression *set_expr = new SetExpression(idx);
	return Expression_s(set_expr);
}

Object_s SetExpression::get_result(const Row_s& row)
{
	if (!row) {
		return Error::make_object(COLUMN_NOT_EXISTS);
	}
	Object_s cell;
	u32 ret = row->get_cell(index, cell);
	if (ret == SUCCESS)
		return cell;
	else
		return Error::make_object(ret);
}

Expression::ExprType SetExpression::get_type()const
{
	return Expression::SetExpr;
}

OpExpression::OpExpression(const Operation & op)
	:op(op)
{

}

OpExpression::~OpExpression()
{
}

Expression_s OpExpression::make_op_expression(OperationType op)
{
	return Expression_s(new OpExpression(Operation(op)));
}

Object_s OpExpression::get_result(const Row_s & row)
{
	Vector<Object_s> param_result;
	for (u32 i = 0; i < param_exprs.size(); ++i) {
		param_result.push_back(param_exprs[i]->get_result(row));
	}
	if (param_exprs.size() == 1) {
		return op.calc(param_result[0]);
	} else if (param_exprs.size() == 2) {
		return op.calc(param_result[0], param_result[1]);
	} else if (param_exprs.size() == 3) {
		return op.calc(param_result[0], param_result[1], param_result[2]);
	} else {
		LOG_ERR("wrong calc function called for opertion");
		return Error::make_object(WRONG_CALC_FOR_OP);
	}
}

Expression::ExprType OpExpression::get_type() const
{
	return Expression::OpExpr;
}

AggregateExpression::AggregateExpression(const Expression_s& expr, AggrType op, bool is_distinct)
	:expr(expr),
	is_distinct(is_distinct),
	op(op),
	row_count(0)
{
	hash_table.set_hash_expr(this->expr);
	hash_table.set_probe_expr(this->expr);
}

AggregateExpression::~AggregateExpression()
{
}

Expression_s AggregateExpression::make_aggregate_expression(const Expression_s& expr, AggrType op, bool is_distinct)
{
	return Expression_s(new AggregateExpression(expr, op, is_distinct));
}

Object_s AggregateExpression::get_result(const Row_s &)
{
	//如果聚合函数没有任何输入
	if (!result){
		//count函数返回零
		if (op == COUNT){
			result = Number::make_object(0);
			Number_s num = result;
			num->set_scale(-1);
			return result;
		}else{//其他函数返回null
			return Object::make_null_object();
		}
	}else if(op == AVG){
		result = result->operator/(Number::make_object(row_count));
		return result;
	}else{
		if (op == COUNT) {
			Number_s num = result;
			num->set_scale(-1);
		}
		return result;
	}
}

Expression::ExprType AggregateExpression::get_type() const
{
	return Expression::Aggregate;
}

u32 AggregateExpression::add_row(const Row_s & row)
{
	if (is_distinct && (op != MIN || MAX)) {
		if (hash_table.probe(row) == SUCCESS) {
			return SUCCESS;
		}else{
			hash_table.build(row);
		}
	}
	switch (op)
	{
	case SUM:
		return sum(row);
	case COUNT:
		return count(row);
	case AVG:
		return avg(row);
	case MIN:
		return min(row);
	case MAX:
		return max(row);
	default:
		return UNKNOWN_AGG_FUNC;
	}
}

void AggregateExpression::reset()
{
	result.reset();
	hash_table.clear();
	row_count = 0;
}

u32 AggregateExpression::sum(const Row_s & row)
{
	Object_s value = expr->get_result(row);
	if (value->get_type() == T_ERROR_RESULT)
		return EXPR_CALC_ERR;
	else if (value->is_null()) {
		result = value->copy();
		return SUCCESS;
	} else if (!result){
		result = value->copy();
	}else{
		result->accumulate(value);
	}
	return SUCCESS;
}

u32 AggregateExpression::count(const Row_s &row)
{
	if (expr) {
		Object_s value = expr->get_result(row);
		if (value->get_type() == T_ERROR_RESULT)
			return EXPR_CALC_ERR;
		else if (value->is_null())
			return SUCCESS;
	}
	if (!result){
		result = Number::make_object(1);
	}else{
		result->increase();
	}
	return SUCCESS;
}

u32 AggregateExpression::avg(const Row_s & row)
{
	sum(row);
	++row_count;
	return SUCCESS;
}

u32 AggregateExpression::max(const Row_s & row)
{
	Object_s value = expr->get_result(row);
	if (value->get_type() == T_ERROR_RESULT)
		return EXPR_CALC_ERR;
	else if (value->is_null()) {
		result = value;
		return SUCCESS;
	}
	if (!result){
		result = value;
	}else{
		if (value->operator>(result)->bool_value()){
			result = value;
		}
	}
	return SUCCESS;
}

u32 AggregateExpression::min(const Row_s & row)
{
	Object_s value = expr->get_result(row);
	if (value->get_type() == T_ERROR_RESULT)
		return EXPR_CALC_ERR;
	else if (value->is_null()) {
		result = value;
		return SUCCESS;
	}
	if (!result){
		result = value;
	}else{
		if (value->operator<(result)->bool_value()){
			result = value;
		}
	}
	return SUCCESS;
}

SubplanExpression::SubplanExpression(PhyOperator_s& subplan)
	:subplan(subplan)
{
}

Expression_s SubplanExpression::make_subplan_expression(PhyOperator_s& subplan)
{
	SubplanExpression* expr = new SubplanExpression(subplan);
	return Expression_s(expr);
}

Object_s SubplanExpression::get_result(const Row_s & row)
{
	if (result && exec_params.empty()) {
		return result;
	} else {
		for (u32 i = 0; i < exec_params.size(); ++i) {
			exec_params[i]->set_value(row);
		}
		u32 ret = SqlEngine::handle_subplan(subplan, result);
		if (ret != SUCCESS) {
			result = Error::make_object(ret);
		}
		return result;
	}
}

Expression::ExprType SubplanExpression::get_type() const
{
	return Expression::Subplan;
}