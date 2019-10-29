#include "expression.h"
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
	switch (type)
	{
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
		Log(LOG_ERR, "Operation", "wrong calc function called for opertion %u", type);
		return Error::make_object(WRONG_CALC_FOR_OP);
	}
}

Object_s Operation::calc(Object_s & first_obj, Object_s & second_obj)
{
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
		Log(LOG_ERR, "Operation", "wrong calc function called for opertion %u", type);
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
		Log(LOG_ERR, "Operation", "wrong calc function called for opertion %u", type);
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

Object_s CatDB::Sql::Operation::do_not_equal(Object_s & first_obj, Object_s & second_obj)
{
	Object_s result = first_obj->operator==(second_obj);
	return result->op_not();
}

Object_s Operation::do_greater(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->operator>(second_obj);
}

Object_s CatDB::Sql::Operation::do_GE(Object_s & first_obj, Object_s & second_obj)
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

Object_s CatDB::Sql::Operation::do_LE(Object_s & first_obj, Object_s & second_obj)
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

Object_s CatDB::Sql::Operation::do_not_between(Object_s & first_obj, Object_s & second_obj, Object_s & third_obj)
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

Object_s CatDB::Sql::Operation::do_like(Object_s & first_obj, Object_s & second_obj)
{
	return first_obj->like(second_obj);
}

Object_s CatDB::Sql::Operation::do_not_like(Object_s & first_obj, Object_s & second_obj)
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

Expression::ExprType CatDB::Sql::ConstExpression::get_type() const
{
	return Expression::Const;
}

void CatDB::Sql::ConstExpression::reset(const Row_s & row)
{
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

const ColumnDesc & CatDB::Sql::ColumnExpression::get_column_desc() const
{
	return col_desc;
}

u32 CatDB::Sql::ColumnExpression::set_alias_table_id(u32 table_id)
{
	alias_table_id = table_id;
	return SUCCESS;
}

u32 CatDB::Sql::ColumnExpression::get_alias_table_id() const
{
	return alias_table_id;
}

Object_s ColumnExpression::get_result(const Row_s & row)
{
	Object_s result;

	u32 ret = row->get_cell(col_desc, result);
	if (ret == SUCCESS)
		return result;
	else
		return Error::make_object(ret);
}

Expression::ExprType CatDB::Sql::ColumnExpression::get_type() const
{
	return Expression::Column;
}

void CatDB::Sql::ColumnExpression::reset(const Row_s & row)
{
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

Expression::ExprType CatDB::Sql::UnaryExpression::get_type() const
{
	return Expression::Unary;
}

void CatDB::Sql::UnaryExpression::reset(const Row_s & row)
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

Expression::ExprType CatDB::Sql::BinaryExpression::get_type() const
{
	return Expression::Binary;
}

void CatDB::Sql::BinaryExpression::reset(const Row_s & row)
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

Expression::ExprType CatDB::Sql::TernaryExpression::get_type() const
{
	return Expression::Ternary;
}

void CatDB::Sql::TernaryExpression::reset(const Row_s & row)
{
	first_expr->reset(row);
	second_expr->reset(row);
	third_expr->reset(row);
}

CatDB::Sql::AggregateExpression::AggregateExpression(const Expression_s& expr, AggrType op)
	:expr(expr),
	op(op),
	row_count(0)
{
	
}

CatDB::Sql::AggregateExpression::~AggregateExpression()
{
}

Expression_s CatDB::Sql::AggregateExpression::make_aggregate_expression(const Expression_s& expr, AggrType op)
{
	return Expression_s(new AggregateExpression(expr, op));
}

Object_s CatDB::Sql::AggregateExpression::get_result(const Row_s &)
{
	//如果聚合函数没有任何输入
	if (!result){
		//count函数返回零
		if (op == AggrStmt::COUNT){
			return Number::make_object(0);
		}else{//其他函数返回null
			return Object::make_null_object();
		}
	}else if(op == AggrStmt::AVG){
		result = result->operator/(Number::make_object(row_count));
		return result;
	}else{
		return result;
	}
}

Expression::ExprType CatDB::Sql::AggregateExpression::get_type() const
{
	return Expression::Aggregate;
}

void CatDB::Sql::AggregateExpression::reset(const Row_s & row)
{
	expr->reset(row);
}

u32 CatDB::Sql::AggregateExpression::add_row(const Row_s & row)
{
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

void CatDB::Sql::AggregateExpression::reset()
{
	result.reset();
	row_count = 0;
}

u32 CatDB::Sql::AggregateExpression::sum(const Row_s & row)
{
	if (!result){
		result = expr->get_result(row);
		if (result->get_type() == T_ERROR_RESULT)
			return EXPR_CALC_ERR;
		else
			return SUCCESS;
	}else{
		Object_s obj = expr->get_result(row);
		if (result->get_type() == T_ERROR_RESULT)
			return EXPR_CALC_ERR;
		result->accumulate(obj);
		return SUCCESS;
	}
}

u32 CatDB::Sql::AggregateExpression::count(const Row_s &)
{
	if (!result){
		result = Number::make_object(1);
	}else{
		result->increase();
	}
	return SUCCESS;
}

u32 CatDB::Sql::AggregateExpression::avg(const Row_s & row)
{
	sum(row);
	++row_count;
	return SUCCESS;
}

u32 CatDB::Sql::AggregateExpression::max(const Row_s & row)
{
	Object_s obj = expr->get_result(row);
	if (!result){
		result = obj;
	}else{
		if (obj->operator>(result)->bool_value()){
			result = obj;
		}
	}
	return SUCCESS;
}

u32 CatDB::Sql::AggregateExpression::min(const Row_s & row)
{
	Object_s obj = expr->get_result(row);
	if (!result){
		result = obj;
	}else{
		if (obj->operator<(result)->bool_value()){
			result = obj;
		}
	}
	return SUCCESS;
}

CatDB::Sql::SubplanFilterExpression::SubplanFilterExpression(const Plan_s& subplan)
	:subplan(subplan)
{
}

Expression_s CatDB::Sql::SubplanFilterExpression::make_subplan_filter_expression(const Plan_s& subplan)
{
	return Expression_s(new SubplanFilterExpression(subplan));
}

u32 CatDB::Sql::SubplanFilterExpression::set_alias_table(u32 table)
{
	alias_table = table;
	return SUCCESS;
}

u32 CatDB::Sql::SubplanFilterExpression::get_alias_table() const
{
	return alias_table;
}

Object_s CatDB::Sql::SubplanFilterExpression::get_result(const Row_s & row)
{
	if (!result)
		return Object::make_null_object();
	else
		return result;
}

Expression::ExprType CatDB::Sql::SubplanFilterExpression::get_type() const
{
	return Expression::SubplanFilter;
}

void CatDB::Sql::SubplanFilterExpression::reset(const Row_s & row)
{
	
}
