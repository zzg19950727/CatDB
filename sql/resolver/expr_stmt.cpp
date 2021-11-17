#include "expr_stmt.h"
#include "expr_utils.h"
#include "select_stmt.h"

using namespace::CatDB::Parser;
using namespace::CatDB::Common;

ExprStmt::ExprStmt()
{
}

ExprStmt::~ExprStmt()
{
}


String ExprStmt::flag_to_string(u32 flag) const 
{
    if (IS_COLUMN == flag) {
        return N(IS_COLUMN);
    } else if (HAS_COLUMN == flag) {
        return N(HAS_COLUMN);
    } else if (IS_CONST == flag) {
        return N(IS_CONST);
    } else if (HAS_CONST == flag) {
        return N(HAS_CONST);
    } else if (IS_AGG == flag) {
        return N(IS_AGG);
    } else if (HAS_AGG == flag) {
        return N(HAS_AGG);
    } else if (IS_SUBQUERY == flag) {
        return N(IS_SUBQUERY);
    } else if (HAS_SUBQUERY == flag) {
        return N(HAS_SUBQUERY);
    } else if (IS_LIST == flag) {
        return N(IS_LIST);
    } else if (IS_OP_EXPR == flag) {
        return N(IS_OP_EXPR);
    } else if (IS_EXEC_PARAM == flag) {
        return N(IS_EXEC_PARAM);
    } else if (HAS_EXEC_PARAM == flag) {
        return N(HAS_EXEC_PARAM);
    } else {
        return N(UNKNOWN);
    }
}

String ExprStmt::flags_to_string() const
{
    String ret = "[";
    Vector<u32> members;
    flags.to_list(members);
    if (INVALID_FLAG != is_flag) {
        ret += flag_to_string(u32(is_flag));
    }
    for (u32 i = 0; i < members.size(); ++i) {
        ret += "," + flag_to_string(members[i]);
    }
    ret += "]";
    return ret;
}

bool ExprStmt::is_and_expr()
{
	if (OP_EXPR == expr_type()) {
		OpExprStmt *op_expr = dynamic_cast<OpExprStmt*>(this);
		if (OP_AND == op_expr->op_type) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

ConstStmt::ConstStmt()
{
}

ConstStmt::~ConstStmt()
{
}

ExprType ConstStmt::expr_type() const
{
	return CONST;
}

ExprStmt_s ConstStmt::make_const_stmt(const Object_s& value)
{
	ConstStmt* stmt = new ConstStmt();
	stmt->value = value;
	return ExprStmt_s(stmt);
}

String ConstStmt::to_string() const
{
	if (value) {
		return value->to_string();
	}
	else {
		return "";
	}
}

u32 ConstStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_const_stmt(value);
	expr->alias_name = alias_name;
	return ret;
}

bool ConstStmt::same_as(const ExprStmt_s& other)
{
	if (CONST != other->expr_type()) {
		return false;
	}
	ConstStmt_s const_stmt = other;
	return value->operator==(const_stmt->value);
}

u32 ConstStmt::formalize()
{
	u32 ret = SUCCESS;
	clear_flag();
	set_is_flag(IS_CONST);
	add_flag(HAS_CONST);
	return ret;
}

ExecParamStmt::ExecParamStmt()
{
}

ExecParamStmt::~ExecParamStmt()
{
}

ExprType ExecParamStmt::expr_type() const
{
	return EXEC_PARAM;
}

ExprStmt_s ExecParamStmt::make_exec_param_stmt(const ExprStmt_s& ref_expr)
{
	ExecParamStmt* stmt = new ExecParamStmt();
	stmt->ref_expr = ref_expr;
	return ExprStmt_s(stmt);
}

String ExecParamStmt::to_string() const
{
	return "?";
}

u32 ExecParamStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	ExprStmt_s copy_ref_expr;
	CHECK(ExprUtils::deep_copy_expr(ref_expr, copy_ref_expr, flag));
	expr = make_exec_param_stmt(copy_ref_expr);
	expr->alias_name = alias_name;
	return ret;
}

bool ExecParamStmt::same_as(const ExprStmt_s& other)
{
	if (EXEC_PARAM != other->expr_type()) {
		return false;
	}
	ExecParamStmt_s param_stmt = other;
	return param_stmt->ref_expr->same_as(ref_expr);
}

u32 ExecParamStmt::formalize()
{
	u32 ret = SUCCESS;
	clear_flag();
	set_is_flag(IS_EXEC_PARAM);
	add_flag(HAS_CONST);
	add_flag(HAS_EXEC_PARAM);
	return ret;
}

ColumnStmt::ColumnStmt()
	:is_row_id(false)
{
}

ColumnStmt::~ColumnStmt()
{
}

ExprType ColumnStmt::expr_type() const
{
	return COLUMN;
}

ExprStmt_s ColumnStmt::make_column_stmt(const String & table, const String & column)
{
	ColumnStmt* stmt = new ColumnStmt;
	stmt->table = table;
	stmt->column = column;
	return ExprStmt_s(stmt);
}

ExprStmt_s ColumnStmt::make_all_column_stmt()
{
	ColumnStmt* stmt = new ColumnStmt;
	stmt->table = "*";
	stmt->column = "*";
	return ExprStmt_s(stmt);
}

bool ColumnStmt::is_all_column() const
{
	return column == "*";
}

String ColumnStmt::to_string() const
{
	if (table.size() && table != "*")
		return table + "." + column;
	else
		return column;
}

u32 ColumnStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_column_stmt(table, column);
	ColumnStmt_s col_expr = expr;
	col_expr->is_row_id = is_row_id;
	col_expr->table_id = table_id;
	col_expr->column_id = column_id;
	expr->alias_name = alias_name;
	return ret;
}

u32 ColumnStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.add_member(table_id);
	clear_flag();
	set_is_flag(IS_COLUMN);
	add_flag(HAS_COLUMN);
	return ret;
}

bool ColumnStmt::same_as(const ExprStmt_s& other)
{
	if (COLUMN != other->expr_type()) {
		return false;
	}
	ColumnStmt_s column_stmt = other;
	return (table_id == column_stmt->table_id) && 
			   (column_id == column_stmt->column_id);
}

SetExprStmt::SetExprStmt()
	: type(UNION_ALL),
	index(0)
{

}

SetExprStmt::~SetExprStmt()
{

}

ExprType SetExprStmt::expr_type()const
{
	return SET_EXPR;
}

ExprStmt_s SetExprStmt::make_set_expr(SetOpType type, u32 idx)
{
	SetExprStmt *set_expr = new SetExprStmt();
	set_expr->type = type;
	set_expr->index = idx;
	return ExprStmt_s(set_expr);
}

u32 SetExprStmt::formalize()
{
	u32 ret = SUCCESS;
	clear_flag();
	set_is_flag(IS_SET_EXPR);
	add_flag(HAS_SET_EXPR);
	return ret;
}

String SetExprStmt::to_string()const
{
	String ret = SetOpTypeString[type];
	ret += "(" + std::to_string(index) + ")";
	return ret;
}

u32 SetExprStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_set_expr(type, index);
	expr->alias_name = alias_name;
	return ret;
}

bool SetExprStmt::same_as(const ExprStmt_s& other)
{
	if (!other || SET_EXPR != other->expr_type()) {
		return false;
	}
	SetExprStmt_s set_expr = other;
	return set_expr->type == type && set_expr->index == index;
}

DEFINE_KV_STRING(SubQueryStmt,
				KV(flags, flags_to_string()),
				K(table_ids),
				K(is_any),
				K(is_all),
				K(output_one_row),
				KV(corrected_exprss, params),
				K(query_stmt)
			);

SubQueryStmt::SubQueryStmt()
	: subquery_id(0),
	is_any(false),
	is_all(false),
	output_one_row(true)
{
}

SubQueryStmt::~SubQueryStmt()
{
}

ExprType SubQueryStmt::expr_type() const
{
	return SUBQUERY;
}

ExprStmt_s SubQueryStmt::make_query_stmt()
{
	return ExprStmt_s(new SubQueryStmt());
}

String SubQueryStmt::to_string() const
{
	String ret = String("(") + std::to_string(subquery_id) + ")";
	if (is_any) {
		ret = "subquery_any"+ret;
	} else if (is_all) {
		ret = "subquery_all" + ret;
	} else {
		ret = "subquery" + ret;
	}
	return ret;
}

u32 SubQueryStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_query_stmt();
	SubQueryStmt_s query_expr = expr;
	query_expr->query_stmt = query_stmt;
	query_stmt->increase_ref_count();
	query_expr->subquery_id = subquery_id;
	query_expr->output_one_row = output_one_row;
	query_expr->is_any = is_any;
	query_expr->is_all = is_all;
	ExprStmt_s copy_expr;
	for (u32 i = 0; i < exec_params.size(); ++i) {
		CHECK(ExprUtils::deep_copy_expr(exec_params[i], copy_expr, flag));
		query_expr->add_corrected_exprs(copy_expr);
	}
	expr->alias_name = alias_name;
	return ret;
}

u32 SubQueryStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	clear_flag();
	set_is_flag(IS_SUBQUERY);
	add_flag(HAS_SUBQUERY);
	for (u32 i = 0; i < params.size(); ++i) {
		CHECK(params[i]->formalize());
		table_ids.add_members(params[i]->table_ids);
		add_flags(params[i]->get_flags());
	}
	CHECK(query_stmt->formalize());
	return ret;
}

void SubQueryStmt::add_corrected_exprs(ExecParamStmt_s expr)
{
	exec_params.push_back(expr);
	params.push_back(expr->get_ref_expr());
}

ListStmt::ListStmt()
{
}

ListStmt::~ListStmt()
{
}

ExprType ListStmt::expr_type() const
{
	return EXPR_LIST;
}

ExprStmt_s ListStmt::make_list_stmt()
{
	return ExprStmt_s(new ListStmt());
}

String ListStmt::to_string() const
{
	String ret = "(";
	for (u32 i = 0; i < params.size(); ++i) {
		ret += params[i]->to_string();
		if (i != params.size() - 1) {
			ret += ",";
		}
	}
	ret += ")";
	return ret;
}

u32 ListStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_list_stmt();
	CHECK(ExprUtils::deep_copy_exprs(params, expr->params, flag));
	expr->alias_name = alias_name;
	return ret;
}

u32 ListStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	clear_flag();
	set_is_flag(IS_LIST);
	bool all_is_const = true;;
	for (u32 i = 0; i < params.size(); ++i) {
		CHECK(params[i]->formalize());
		table_ids.add_members(params[i]->table_ids);
		add_flags(params[i]->get_flags());
		if (!params[i]->has_flag(IS_CONST)) {
			all_is_const = false;
		}
	}
	if (all_is_const) {
		add_flag(IS_CONST);
	} else {
		del_flag(IS_CONST);
	}
	return ret;
}

bool ListStmt::same_as(const ExprStmt_s &other)
{
	if (EXPR_LIST != other->expr_type()) {
		return false;
	}
	ListStmt_s list = other;
	if (params.size() != list->size()) {
		return false;
	}
	for (u32 i = 0; i < params.size(); ++i) {
		if (!params[i]->same_as(list->at(i))) {
			return false;
		}
	}
	return true;
}

AggrStmt::AggrStmt()
	:distinct(false)
{
}

AggrStmt::~AggrStmt()
{
}

ExprType AggrStmt::expr_type() const
{
	return AGG_EXPR;
}

ExprStmt_s AggrStmt::make_aggr_stmt()
{
	return ExprStmt_s(new AggrStmt());
}

String AggrStmt::to_string() const
{
	String func = aggr_func_name();
	String distinct_str;
	if (distinct) {
		distinct_str = "DISTINCT ";
	}
	String expr_str = get_aggr_expr()->to_string();
	return func + "(" + distinct_str + expr_str + ")";
}

u32 AggrStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_aggr_stmt();
	AggrStmt_s aggr_expr = expr;
	aggr_expr->aggr_func = aggr_func;
	aggr_expr->distinct = distinct;
	CHECK(ExprUtils::deep_copy_exprs(params, expr->params, flag));
	expr->alias_name = alias_name;
	return ret;
}

String AggrStmt::aggr_func_name()const
{
	String func;
	switch (aggr_func)
	{
	case COUNT:
		func = "COUNT";
		break;
	case SUM:
		func = "SUM";
		break;
	case AVG:
		func = "AVG";
		break;
	case MIN:
		func = "MIN";
		break;
	case MAX:
		func = "MAX";
		break;
	default:
		func = "UNKNOWN";
	}
	return func;
}

u32 AggrStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	clear_flag();
	set_is_flag(IS_AGG);
	add_flag(HAS_AGG);
	ExprStmt_s aggr_expr = get_aggr_expr();
	CHECK(aggr_expr->formalize());
	table_ids.add_members(aggr_expr->table_ids);
	add_flags(aggr_expr->get_flags());
	return ret;
}

void AggrStmt::set_aggr_expr(const ExprStmt_s& expr)
{
	params.clear();
	params.push_back(expr);
}

ExprStmt_s AggrStmt::get_aggr_expr() const
{
	if (params.size() == 1) {
		return params[0];
	} else {
		return ExprStmt_s();
	}
}

bool AggrStmt::same_as(const ExprStmt_s &other)
{
	if (AGG_EXPR != other->expr_type()) {
		return false;
	}
	AggrStmt_s aggr = other;
	if (aggr->distinct != distinct) {
		return false;
	} else if (aggr->aggr_func != aggr_func) {
		return false;
	} else {
		return get_aggr_expr()->same_as(aggr->get_aggr_expr());
	}
}

OpExprStmt::OpExprStmt(OperationType op_type)
	:op_type(op_type)
{

}

OpExprStmt::~OpExprStmt()
{

}

ExprType OpExprStmt::expr_type()const
{
	return OP_EXPR;
}

ExprStmt_s OpExprStmt::make_op_expr_stmt(OperationType op_type)
{
	return ExprStmt_s(new OpExprStmt(op_type));
}

String OpExprStmt::to_string()const
{
	String ret;
	switch (op_type) {
		case OP_INVALID:
			ret += "UNKNOWN";
			break;
		case OP_MINUS:
			if (params.size() == 1) {
				ret += "-" + params[0]->to_string();
			}
			break;
		case OP_ADD:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "+";
				ret += params[1]->to_string();
			}
			break;
		case OP_SUB: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "-";
				ret += params[1]->to_string();
			}
			break;
		case OP_MUL: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "*";
				ret += params[1]->to_string();
			}
			break;
		case OP_DIV: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "/";
				ret += params[1]->to_string();
			}
			break;
		case OP_EQ: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "=";
				ret += params[1]->to_string();
			}
			break;
		case OP_NE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "!=";
				ret += params[1]->to_string();
			}
			break;
		case OP_GE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += ">=";
				ret += params[1]->to_string();
			}
			break;
		case OP_GT: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += ">";
				ret += params[1]->to_string();
			}
			break;
		case OP_LE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "<=";
				ret += params[1]->to_string();
			}
			break;
		case OP_LT: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += "<";
				ret += params[1]->to_string();
			}
			break;
		case OP_BETWEEN: 
			if (params.size() == 3) {
				ret += params[0]->to_string();
				ret += " BETWEEN ";
				ret += params[1]->to_string();
				ret += " AND ";
				ret += params[2]->to_string();
			}
			break;
		case OP_NOT_BETWEEN: 
			if (params.size() == 3) {
				ret += params[0]->to_string();
				ret += " NOT BETWEEN ";
				ret += params[1]->to_string();
				ret += " AND ";
				ret += params[2]->to_string();
			}
			break;
		case OP_IS_NULL: 
			if (params.size() == 1) {
				ret += params[0]->to_string();
				ret += " IS NULL";
			}
			break;
		case OP_IS_NOT_NULL: 
			if (params.size() == 1) {
				ret += params[0]->to_string();
				ret += " IS NOT NULL";
			}
			break;
		case OP_IN: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " IN ";
				ret += params[1]->to_string();
			}
			break;
		case OP_NOT_IN: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " NOT IN ";
				ret += params[1]->to_string();
			}
			break;
		case OP_EXISTS: 
			if (params.size() == 1) {
				ret += "EXISTS(";
				ret += params[0]->to_string();
				ret += ")";
			}
			break;
		case OP_NOT_EXISTS: 
			if (params.size() == 1) {
				ret += "NOT EXISTS(";
				ret += params[0]->to_string();
				ret += ")";
			}
			break;
		case OP_AND: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " AND ";
				ret += params[1]->to_string();
			}
			break;
		case OP_OR: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " OR ";
				ret += params[1]->to_string();
			}
			break;
		case OP_NOT: 
			if (params.size() == 1) {
				ret += "NOT ";
				ret += params[0]->to_string();
			}
			break;
		case OP_LIKE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " LIKE ";
				ret += params[1]->to_string();
			}
			break;
		case OP_NOT_LIKE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " NOT LIKE ";
				ret += params[1]->to_string();
			}
			break;
		case OP_CAST: 
			if (params.size() == 2) {
				ret += "CAST(";
				ret += params[0]->to_string();
				ret += " AS ";
				MY_ASSERT(CONST == params[1]->expr_type());
				ConstStmt_s const_value = params[1];
				ret += const_value->value->type();
				ret += ")";
			}
			break;
		case OP_CASE_WHEN:
			ret += "CASE ";
			if (params.size() % 2) {
				for (u32 i = 0; i < params.size() / 2; i += 2) {
					ret += "WHEN ";
					ret += params[i]->to_string();
					ret += " THEN ";
					ret += params[i+1]->to_string();
				}
				ret += " ELSE ";
				ret += params[params.size() - 1]->to_string();
				ret += " END";
			} else if (params.size() > 1) {
				ret += params[0]->to_string();
				for (u32 i = 1; i < params.size() / 2; i += 2) {
					ret += " WHEN ";
					ret += params[i]->to_string();
					ret += " THEN ";
					ret += params[i+1]->to_string();
				}
				ret += " ELSE ";
				ret += params[params.size() - 1]->to_string();
				ret += " END";
			}
			break;
		defualt: 
			ret += "UNKNOWN";
			break;
	}
	return ret;
}

u32 OpExprStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_op_expr_stmt(op_type);
	CHECK(ExprUtils::deep_copy_exprs(params, expr->params, flag));
	expr->alias_name = alias_name;
	return ret;
}

u32 OpExprStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	clear_flag();
	set_is_flag(IS_OP_EXPR);
	bool all_is_const = true;
	for (u32 i = 0; i < params.size(); ++i) {
		CHECK(params[i]->formalize());
		table_ids.add_members(params[i]->table_ids);
		add_flags(params[i]->get_flags());
		if (!params[i]->has_flag(IS_CONST)) {
			all_is_const = false;
		}
	}
	if (all_is_const) {
		add_flag(IS_CONST);
	}
	return ret;
}

bool OpExprStmt::same_as(const ExprStmt_s &other)
{
	if (OP_EXPR != other->expr_type()) {
		return false;
	}
	OpExprStmt_s expr = other;
	if (op_type != expr->op_type) {
		return false;
	} else if (params.size() != other->params.size()) {
		return false;
	} else {
		for (u32 i = 0; i < params.size(); ++i) {
			if (!params[i]->same_as(other->params[i])) {
				return false;
			}
		}
		return true;
	}
}


String OpExprStmt::op_string(OperationType op_type)
{
	switch (op_type) {
		case OP_INVALID: return N(OP_INVALID);
		case OP_MINUS: return N(OP_MINUS);
		case OP_ADD: return N(OP_ADD);
		case OP_SUB: return N(OP_SUB);
		case OP_MUL: return N(OP_MUL);
		case OP_DIV: return N(OP_DIV);
		case OP_EQ: return N(OP_EQ);
		case OP_NE: return N(OP_NE);
		case OP_GE: return N(OP_GE);
		case OP_GT: return N(OP_GT);
		case OP_LE: return N(OP_LE);
		case OP_LT: return N(OP_LT);
		case OP_BETWEEN: return N(OP_BETWEEN);
		case OP_NOT_BETWEEN: return N(OP_NOT_BETWEEN);
		case OP_IS_NULL: return N(OP_IS_NULL);
		case OP_IS_NOT_NULL: return N(OP_IS_NOT_NULL);
		case OP_IN: return N(OP_IN);
		case OP_NOT_IN: return N(OP_NOT_IN);
		case OP_EXISTS: return N(OP_EXISTS);
		case OP_NOT_EXISTS: return N(OP_NOT_EXISTS);
		case OP_AND: return N(OP_AND);
		case OP_OR: return N(OP_OR);
		case OP_NOT: return N(OP_NOT);
		case OP_LIKE: return N(OP_LIKE);
		case OP_NOT_LIKE: return N(OP_NOT_LIKE);
		case OP_CAST: return N(OP_CAST);
		case OP_CASE_WHEN: return N(OP_CASE_WHEN);
		defualt: return N(UNKNOWN);
	}
}