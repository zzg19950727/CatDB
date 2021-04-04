﻿#include "expression.h"
#include "query_result.h"
#include "select_plan.h"
#include "object.h"
#include "error.h"
#include "log.h"

using namespace CatDB::Sql;
using namespace CatDB::Common;
using CatDB::Parser::ExprStmt;
using CatDB::Parser::AggrStmt;

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

Operation::OperationType Operation::get_type() const
{
	return type;
}

Object_s Operation::calc(Object_s & obj)
{
	if (obj->get_type() == T_QUERY_RESULT 
		&& !(type == ExprStmt::OP_EXISTS || type == ExprStmt::OP_NOT_EXISTS)) {
		QueryResult* result = dynamic_cast<QueryResult*>(obj.get());
		u32 ret = result->cast_to_simple_object(obj);
		if (ret != SUCCESS) {
			return Error::make_object(ret);
		}
	}
	switch (type)
	{
	case ExprStmt::OP_MINUS:
		return do_minus(obj);
	case ExprStmt::OP_NOT:
		return do_not(obj);
	case ExprStmt::OP_EXISTS:
		return do_exists(obj);
	case ExprStmt::OP_NOT_EXISTS:
		return do_not_exists(obj);
	case ExprStmt::OP_IS_NULL:
		return do_is_null(obj);
	case ExprStmt::OP_IS_NOT_NULL:
		return do_is_not_null(obj);
	default:
		LOG_ERR("wrong calc function called for opertion", K(ExprStmt::op_string(type)));
		return Error::make_object(WRONG_CALC_FOR_OP);
	}
}

Object_s Operation::calc(Object_s & first_obj, Object_s & second_obj)
{
	if (first_obj->get_type() == T_QUERY_RESULT) {
		QueryResult* result = dynamic_cast<QueryResult*>(first_obj.get());
		u32 ret = result->cast_to_simple_object(first_obj);
		if (ret != SUCCESS) {
			return Error::make_object(ret);
		}
	}
	else if (second_obj->get_type() == T_QUERY_RESULT 
		&& !(type == ExprStmt::OP_IN || type == ExprStmt::OP_NOT_IN)) {
		QueryResult* result = dynamic_cast<QueryResult*>(second_obj.get());
		u32 ret = result->cast_to_simple_object(second_obj);
		if (ret != SUCCESS) {
			return Error::make_object(ret);
		}
	}
	switch (type)
	{
	case ExprStmt::OP_ADD:
		return do_add(first_obj, second_obj);
	case ExprStmt::OP_SUB:
		return do_sub(first_obj, second_obj);
	case ExprStmt::OP_MUL:
		return do_mul(first_obj, second_obj);
	case ExprStmt::OP_DIV:
		return do_div(first_obj, second_obj);
	case ExprStmt::OP_EQ:
		return do_equal(first_obj, second_obj);
	case ExprStmt::OP_ANTI_EQ:
		return do_anti_equal(first_obj, second_obj);
	case ExprStmt::OP_NE:
		return do_not_equal(first_obj, second_obj);
	case ExprStmt::OP_GT:
		return do_greater(first_obj, second_obj);
	case ExprStmt::OP_GE:
		return do_GE(first_obj, second_obj);
	case ExprStmt::OP_LT:
		return do_less(first_obj, second_obj);
	case ExprStmt::OP_LE:
		return do_LE(first_obj, second_obj);
	case ExprStmt::OP_IN:
		return do_in(first_obj, second_obj);
	case ExprStmt::OP_NOT_IN:
		return do_not_in(first_obj, second_obj);
	case ExprStmt::OP_AND:
		return do_and(first_obj, second_obj);
	case ExprStmt::OP_OR:
		return do_or(first_obj, second_obj);
	case ExprStmt::OP_LIKE:
		return do_like(first_obj, second_obj);
	case ExprStmt::OP_NOT_LIKE:
		return do_not_like(first_obj, second_obj);
	default:
		LOG_ERR("wrong calc function called for opertion", K(ExprStmt::op_string(type)));
		return Error::make_object(WRONG_CALC_FOR_OP);
	}
}

Object_s Operation::calc(Object_s & first_obj, Object_s & second_obj, Object_s & third_obj)
{
	switch (type)
	{
	case ExprStmt::OP_BETWEEN:
		return do_between(first_obj, second_obj, third_obj);
	case ExprStmt::OP_NOT_BETWEEN:
		return do_not_between(first_obj, second_obj, third_obj);
	default:
		LOG_ERR("wrong calc function called for opertion", K(ExprStmt::op_string(type)));
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

void ConstExpression::reset(const Row_s & row)
{
}

ColumnExpression::ColumnExpression(const ColumnDesc& desc)
	:col_desc(desc),
	table(nullptr)
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
	if (result) {
		return result;
	}
	else if (!row) {
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

void ColumnExpression::reset(const Row_s & row)
{
	row->get_cell(col_desc, result);
}

UnaryExpression::UnaryExpression(const Expression_s& expr, const Operation & op)
	:expr(expr),
	op(op)
{
}

UnaryExpression::~UnaryExpression()
{
}

Expression_s UnaryExpression::make_unary_expression(const Expression_s& expr, Operation::OperationType op)
{
	return Expression_s(new UnaryExpression(expr, Operation(op)));
}

Object_s UnaryExpression::get_result(const Row_s & row)
{
	auto obj = expr->get_result(row);
	return op.calc(obj);
}

Expression::ExprType UnaryExpression::get_type() const
{
	return Expression::Unary;
}

void UnaryExpression::reset(const Row_s & row)
{
	expr->reset(row);
}

BinaryExpression::BinaryExpression(const Expression_s& first_expr,
	const Expression_s& second_expr,
	const Operation & op)
	:first_expr(first_expr),
	second_expr(second_expr),
	op(op)
{
}

BinaryExpression::~BinaryExpression()
{
}

Expression_s BinaryExpression::make_binary_expression(const Expression_s& first_expr,
	const Expression_s& second_expr,
	Operation::OperationType op)
{
	return Expression_s(new BinaryExpression(first_expr, second_expr, Operation(op)));
}

Object_s BinaryExpression::get_result(const Row_s & row)
{
	auto first_obj = first_expr->get_result(row);
	auto second_obj = second_expr->get_result(row);
	return op.calc(first_obj, second_obj);
}

Expression::ExprType BinaryExpression::get_type() const
{
	return Expression::Binary;
}

void BinaryExpression::reset(const Row_s & row)
{
	first_expr->reset(row);
	second_expr->reset(row);
}

TernaryExpression::TernaryExpression(const Expression_s& first_expr,
	const Expression_s& second_expr,
	const Expression_s& third_expr,
	const Operation & op)
	:first_expr(first_expr),
	second_expr(second_expr),
	third_expr(third_expr),
	op(op)
{

}

TernaryExpression::~TernaryExpression()
{
}

Expression_s TernaryExpression::make_ternary_expression(const Expression_s& first_expr,
	const Expression_s& second_expr,
	const Expression_s& third_expr,
	Operation::OperationType op)
{
	return Expression_s(new TernaryExpression(first_expr, second_expr, third_expr, Operation(op)));
}

Object_s TernaryExpression::get_result(const Row_s & row)
{
	auto first_obj = first_expr->get_result(row);
	auto second_obj = second_expr->get_result(row);
	auto third_obj = third_expr->get_result(row);
	return op.calc(first_obj, second_obj, third_obj);
}

Expression::ExprType TernaryExpression::get_type() const
{
	return Expression::Ternary;
}

void TernaryExpression::reset(const Row_s & row)
{
	first_expr->reset(row);
	second_expr->reset(row);
	third_expr->reset(row);
}

AggregateExpression::AggregateExpression(const Expression_s& expr, AggrType op, bool is_distinct)
	:expr(expr),
	is_distinct(is_distinct),
	op(op),
	row_count(0)
{
	hash_table.add_hash_column(expr);
	hash_table.add_probe_column(expr);
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
		if (op == AggrStmt::COUNT){
			result = Number::make_object(0);
			Number* num = dynamic_cast<Number*>(result.get());
			num->set_scale(-1);
			return result;
		}else{//其他函数返回null
			return Object::make_null_object();
		}
	}else if(op == AggrStmt::AVG){
		result = result->operator/(Number::make_object(row_count));
		return result;
	}else{
		if (op == AggrStmt::COUNT) {
			Number* num = dynamic_cast<Number*>(result.get());
			num->set_scale(-1);
		}
		return result;
	}
}

Expression::ExprType AggregateExpression::get_type() const
{
	return Expression::Aggregate;
}

void AggregateExpression::reset(const Row_s & row)
{
	expr->reset(row);
	hash_table.clear();
}

u32 AggregateExpression::add_row(const Row_s & row)
{
	if (is_distinct && (op != AggrStmt::MIN || AggrStmt::MAX)) {
		if (hash_table.probe(row) == SUCCESS){
			return SUCCESS;
		}else{
			hash_table.build(row);
		}
	}
	switch (op)
	{
	case AggrStmt::SUM:
		return sum(row);
	case AggrStmt::COUNT:
		return count(row);
	case AggrStmt::AVG:
		return avg(row);
	case AggrStmt::MIN:
		return min(row);
	case AggrStmt::MAX:
		return max(row);
	default:
		return UNKNOWN_AGG_FUNC;
	}
}

void AggregateExpression::reset()
{
	result.reset();
	row_count = 0;
}

u32 AggregateExpression::sum(const Row_s & row)
{
	Object_s value = expr->get_result(row);
	if (value->get_type() == T_ERROR_RESULT)
		return EXPR_CALC_ERR;
	else if (value->is_null())
		return SUCCESS;
	if (!result){
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
	else if (value->is_null())
		return SUCCESS;
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
	else if (value->is_null())
		return SUCCESS;
	if (!result){
		result = value;
	}else{
		if (value->operator<(result)->bool_value()){
			result = value;
		}
	}
	return SUCCESS;
}

SubplanExpression::SubplanExpression(const Plan_s& subplan)
	:subplan(subplan)
{
}

Expression_s SubplanExpression::make_subplan_expression(const Plan_s& subplan, bool correlated, const String& alias)
{
	SubplanExpression* expr = new SubplanExpression(subplan);
	expr->is_correlated = correlated;
	expr->alias_name = alias;
	return Expression_s(expr);
}

Object_s SubplanExpression::get_result(const Row_s & row)
{
	if (result && !is_correlated) {
		return result;
	}
	else if (subplan->type() != Plan::SELECT) {
		return Error::make_object(ERROR_LEX_STMT);
	}
	SelectPlan* query = dynamic_cast<SelectPlan*>(subplan.get());
	if (is_correlated) {
		query->reset_for_correlated_subquery(row);
	}
	u32 ret = query->execute();
	if (ret != SUCCESS) {
		result = Error::make_object(ret);
	}
	else {
		result = query->get_result();
	}
	return result;
}

Expression::ExprType SubplanExpression::get_type() const
{
	return Expression::Subplan;
}

void SubplanExpression::reset(const Row_s & row)
{
	SelectPlan* plan = dynamic_cast<SelectPlan*>(subplan.get());
	if (plan) {
		plan->reset_for_correlated_subquery(row);
	}
}
