#include "expr_stmt.h"
#include "expr_utils.h"
#include "select_stmt.h"
#include "session_info.h"
#include "transform_utils.h"
#include "obj_cast_util.h"
#include "stmt_compare.h"

using namespace::CatDB::Parser;
using namespace::CatDB::Common;
using namespace::CatDB::Transform;

ExprStmt::ExprStmt()
:is_flag(INVALID_FLAG)
{
}

ExprStmt::~ExprStmt()
{
}

u32 ExprStmt::deduce_type()
{
	u32 ret = SUCCESS;
	return ret;
}

String ExprStmt::flags_to_string() const
{
    String ret = "[";
    Vector<u32> members;
    flags.to_list(members);
    if (INVALID_FLAG != is_flag) {
        ret += StmtFlagString[is_flag];
    }
    for (u32 i = 0; i < members.size(); ++i) {
        ret += "," + String(StmtFlagString[members[i]]);
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

u32 ExprStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr->is_flag = is_flag;
	expr->flags = flags;
	CHECK(ExprUtils::deep_copy_exprs(params, expr->params, flag));
	expr->alias_name = alias_name;	//表达式的别名
	expr->table_ids = table_ids;
	expr->res_type = res_type;
	return ret;
}

String ExprStmt::get_alias_name() const
{
	if (!alias_name.empty()) {
		return alias_name;
	} else {
		return to_string();
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
	stmt->deduce_type();
	return ExprStmt_s(stmt);
}

String ConstStmt::to_string() const
{
	if (value) {
		if (value->is_null()) {
			return "NULL";
		} else if (res_type.is_varchar()) {
			return "'" + value->to_string() + "'";
		} else {
			return value->to_string();
		}
	}
	else {
		return "";
	}
}

u32 ConstStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_const_stmt(value);
	ExprStmt::deep_copy(expr, flag);
	return ret;
}

bool ConstStmt::same_as(const ExprStmt_s& other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (CONST != other->expr_type()) {
		return false;
	} else if (!res_type.is_same(other->res_type)) {
		return false;
	}
	ConstStmt_s const_stmt = other;
	int res = 0;
	if (const_stmt->value->is_null() && 
		value->is_null()) {
		return true;
	}
	value->compare(const_stmt->value, res);
	return CMP_RES_EQ == res;
}

u32 ConstStmt::formalize()
{
	u32 ret = SUCCESS;
	clear_flag();
	set_is_flag(IS_CONST);
	add_flag(HAS_CONST);
	return ret;
}

u32 ConstStmt::deduce_type()
{
	u32 ret = SUCCESS;
	switch (value->get_type()) {
		case T_NUMBER:
			res_type = DataType::default_number_type();
			break;
		case T_VARCHAR:
			res_type = DataType::default_varchar_type();
			break;
		case T_DATETIME:
			res_type = DataType::default_datetime_type(DATETIME);
			break;
		case T_BOOL:
		case T_MAX_TYPE:
		default:
			ret = ERR_UNEXPECTED;
	}
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

ExprStmt_s ExecParamStmt::make_exec_param_stmt(u32 index)
{
	ExecParamStmt* stmt = new ExecParamStmt();
	stmt->param_index = index;
	return ExprStmt_s(stmt);
}

String ExecParamStmt::to_string() const
{
	return String("(?-") + std::to_string(param_index) + ")";
}

u32 ExecParamStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_exec_param_stmt(param_index);
	ExprStmt::deep_copy(expr, flag);
	return ret;
}

bool ExecParamStmt::same_as(const ExprStmt_s& other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (EXEC_PARAM != other->expr_type()) {
		return false;
	}
	ExecParamStmt_s param_stmt = other;
	return param_stmt->param_index == param_index;
}

u32 ExecParamStmt::formalize()
{
	u32 ret = SUCCESS;
	clear_flag();
	set_is_flag(IS_EXEC_PARAM);
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
	ExprStmt::deep_copy(expr, flag);
	return ret;
}

u32 ColumnStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	table_ids.add_member(table_id);
	clear_flag();
	set_is_flag(IS_COLUMN);
	add_flag(HAS_COLUMN);
	CHECK(deduce_type());
	return ret;
}

u32 ColumnStmt::deduce_type()
{
	u32 ret = SUCCESS;
	if (is_row_id) {
		res_type = DataType::default_int_type();
	}
	return ret;
}

bool ColumnStmt::same_as(const ExprStmt_s& other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (COLUMN != other->expr_type()) {
		return false;
	}
	ColumnStmt_s column_stmt = other;
	bool b_ret = (table_id == column_stmt->table_id) && 
			   (column_id == column_stmt->column_id);
	if (!b_ret && ctx) {
		ctx->compare_column(this, column_stmt.get(), b_ret);
	}
	return b_ret;
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
	ExprStmt::deep_copy(expr, flag);
	return ret;
}

bool SetExprStmt::same_as(const ExprStmt_s& other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (!other || SET_EXPR != other->expr_type()) {
		return false;
	}
	SetExprStmt_s set_expr = other;
	return set_expr->type == type && set_expr->index == index;
}

DEFINE_KV_STRING(SubQueryStmt,
				KV(flags, flags_to_string()),
				K(res_type),
				K(table_ids),
				KV(corrected_exprs, params),
				K(query_stmt)
			);

SubQueryStmt::SubQueryStmt()
	: subquery_id(0)
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
	String ret = String("subquery(") + std::to_string(subquery_id) + ")";
	return ret;
}

u32 SubQueryStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_query_stmt();
	SubQueryStmt_s query_expr = expr;
	query_expr->subquery_id = subquery_id;
	if (exec_params.empty()) {
		if (!ENABLE_COPY_ON_WRITE(flag)) {
			CHECK(TransformUtils::deep_copy_stmt(query_stmt,
												 query_expr->query_stmt,
												 COPY_SHARE));
		} else {
			query_expr->query_stmt = query_stmt;
			query_stmt->increase_ref_count();
		}
	} else {
		ExprStmt_s copy_expr;
		ExecParamStmt_s exec_param;
		for (u32 i = 0; i < exec_params.size(); ++i) {
			exec_params[i]->deep_copy(copy_expr, flag);
			exec_param = copy_expr;
			exec_param->set_param_index(QUERY_CTX->generate_param_index());
			query_expr->exec_params.push_back(copy_expr);
		}
		CHECK(TransformUtils::deep_copy_stmt(query_stmt,
											query_expr->query_stmt,
											COPY_SHARE));
		Vector<ExprStmt_s> old_exec_params, new_exec_params;
		append(old_exec_params, exec_params);
		append(new_exec_params, query_expr->exec_params);
		CHECK(query_expr->query_stmt->replace_stmt_exprs(old_exec_params, 
														 new_exec_params, 
														 true));
	}
	ExprStmt::deep_copy(expr, flag);
	return ret;
}

bool SubQueryStmt::same_as(const ExprStmt_s &other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (!other || SUBQUERY != other->expr_type()) {
		return false;
	}
	SubQueryStmt_s subquery_expr = other;
	bool b_ret = subquery_expr->query_stmt == query_stmt;
	if (!b_ret && ctx) {
		ctx->compare_query_ref(this, subquery_expr.get(), b_ret);
	}
	return b_ret;
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
	CHECK(deduce_type());
	return ret;
}

u32 SubQueryStmt::deduce_type()
{
	u32 ret = SUCCESS;
	MY_ASSERT(query_stmt->select_expr_list.size() > 0);
	res_type = query_stmt->select_expr_list[0]->res_type;
	return ret;
}

void SubQueryStmt::add_related_exprs(ExprStmt_s &related_expr, ExecParamStmt_s &exec_param)
{
	exec_params.push_back(exec_param);
	params.push_back(related_expr);
}

bool SubQueryStmt::get_related_exprs(ExecParamStmt_s &exec_param, ExprStmt_s &related_expr)
{
	bool ret = false;
	for (u32 i = 0; !ret && i < exec_params.size(); ++i) {
		if (exec_params[i]->get_param_index() == exec_param->get_param_index()) {
			ret = true;
			MY_ASSERT(i < params.size());
			related_expr = params[i];
		}
	}
	return ret;
}

u32 SubQueryStmt::get_all_exec_params(Vector< std::pair<ExecParamStmt_s, ExprStmt_s> > &all_exec_params)
{
	u32 ret = SUCCESS;
	if (exec_params.size() != params.size()) {
		ret = ERR_UNEXPECTED;
		return ret;
	}
	for (u32 i = 0; i < params.size(); ++i) {
		all_exec_params.push_back(std::pair<ExecParamStmt_s, ExprStmt_s>(exec_params[i], params[i]));
	}
	return ret;
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
			ret += ", ";
		}
	}
	ret += ")";
	return ret;
}

u32 ListStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_list_stmt();
	ExprStmt::deep_copy(expr, flag);
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
	CHECK(deduce_type());
	return ret;
}

u32 ListStmt::deduce_type()
{
	u32 ret = SUCCESS;
	Vector<DataType> src_types;
	Vector<bool> need_cast;
	for (u32 i = 0; i < params.size(); ++i) {
		src_types.push_back(params[i]->res_type);
	}
	CHECK(ObjCastUtil::get_result_type(src_types, need_cast, res_type));
	MY_ASSERT(src_types.size() == need_cast.size());
	for (u32 i = 0; i < params.size(); ++i) {
		if (need_cast[i]) {
			CHECK(ObjCastUtil::add_cast(params[i], res_type, params[i]));
		}
	}
	return ret;
}

bool ListStmt::same_as(const ExprStmt_s &other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (EXPR_LIST != other->expr_type()) {
		return false;
	}
	ListStmt_s list = other;
	if (params.size() != list->size()) {
		return false;
	}
	for (u32 i = 0; i < params.size(); ++i) {
		if (!params[i]->same_as(list->at(i), ctx)) {
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
	String func = AggrTypeString[aggr_func];
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
	ExprStmt::deep_copy(expr, flag);
	return ret;
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
	CHECK(deduce_type());
	return ret;
}

u32 AggrStmt::deduce_type()
{
	u32 ret = SUCCESS;
	bool need_cast = false;
	ExprStmt_s aggr_expr = get_aggr_expr();
	switch (aggr_func) {
		case SUM:
		case AVG:
			res_type = DataType::default_number_type();
			CHECK(ObjCastUtil::check_need_cast(aggr_expr->res_type, 
											   res_type, 
											   need_cast));
			if (need_cast) {
				CHECK(ObjCastUtil::add_cast(aggr_expr, res_type, aggr_expr));
				set_aggr_expr(aggr_expr);
			}
			break;
		case COUNT:
			res_type = DataType::default_int_type();
			break;
		case MAX:
		case MIN:
			res_type = aggr_expr->res_type;
			break;
	}
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

bool AggrStmt::same_as(const ExprStmt_s &other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (AGG_EXPR != other->expr_type()) {
		return false;
	}
	AggrStmt_s aggr = other;
	if (aggr->distinct != distinct) {
		return false;
	} else if (aggr->aggr_func != aggr_func) {
		return false;
	} else {
		return get_aggr_expr()->same_as(aggr->get_aggr_expr(), ctx);
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
		case OP_MAX:
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
				ret += " + ";
				ret += params[1]->to_string();
			}
			break;
		case OP_SUB: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " - ";
				ret += params[1]->to_string();
			}
			break;
		case OP_MUL: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " * ";
				ret += params[1]->to_string();
			}
			break;
		case OP_DIV: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " / ";
				ret += params[1]->to_string();
			}
			break;
		case OP_ASSIGN:
		case OP_EQ: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " = ";
				ret += params[1]->to_string();
			}
			break;
		case OP_NE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " != ";
				ret += params[1]->to_string();
			}
			break;
		case OP_GE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " >= ";
				ret += params[1]->to_string();
			}
			break;
		case OP_GT: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " > ";
				ret += params[1]->to_string();
			}
			break;
		case OP_LE: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " <= ";
				ret += params[1]->to_string();
			}
			break;
		case OP_LT: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " < ";
				ret += params[1]->to_string();
			}
			break;
		case OP_EQ_ANY:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " = ANY ";
				ret += params[1]->to_string();
			}
			break;
		case OP_NE_ANY:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " != ANY ";
				ret += params[1]->to_string();
			}
			break;
		case OP_GE_ANY:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " >= ANY ";
				ret += params[1]->to_string();
			}
			break;
		case OP_GT_ANY:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " > ANY ";
				ret += params[1]->to_string();
			}
			break;
		case OP_LE_ANY:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " <= ANY ";
				ret += params[1]->to_string();
			}
			break;
		case OP_LT_ANY:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " < ANY ";
				ret += params[1]->to_string();
			}
			break;
		case OP_EQ_ALL:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " = ALL ";
				ret += params[1]->to_string();
			}
			break;
		case OP_NE_ALL:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " != ALL ";
				ret += params[1]->to_string();
			}
			break;
		case OP_GE_ALL:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " >= ALL ";
				ret += params[1]->to_string();
			}
			break;
		case OP_GT_ALL:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " > ALL ";
				ret += params[1]->to_string();
			}
			break;
		case OP_LE_ALL:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " <= ALL ";
				ret += params[1]->to_string();
			}
			break;
		case OP_LT_ALL:
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " < ALL ";
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
		case OP_IN_LIST:
		case OP_IN: 
			if (params.size() == 2) {
				ret += params[0]->to_string();
				ret += " IN ";
				ret += params[1]->to_string();
			}
			break;
		case OP_NOT_IN_LIST:
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
			for (u32 i = 0; i < params.size(); ++i) {
				if (i > 0) {
					ret += " AND ";
				}
				ret += params[i]->to_string();
			}
			break;
		case OP_OR: 
			for (u32 i = 0; i < params.size(); ++i) {
				if (i > 0) {
					ret += " OR ";
				}
				ret += params[i]->to_string();
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
				ConstStmt_s const_value = params[1];
				ret += const_value->res_type.to_kv_string();
				ret += ")";
			}
			break;
		case OP_CASE_WHEN:
			ret += "CASE ";
			if (params.size() % 2) {
				for (u32 i = 0; i < params.size()-1; i += 2) {
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
				for (u32 i = 1; i < params.size()-1; i += 2) {
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
		case OP_IFNULL:
			if (params.size() == 2) {
				ret += "IFNULL(";
				ret += params[0]->to_string();
				ret += " , ";
				ret += params[1]->to_string();
				ret += ")";
			}
			break;
		case OP_TO_CHAR:
			if (params.size() == 1) {
				ret += "TO_CHAR(";
				ret += params[0]->to_string();
				ret += ")";
			} else if (params.size() == 2) {
				ret += "TO_CHAR(";
				ret += params[0]->to_string();
				ret += " , ";
				ret += params[1]->to_string();
				ret += ")";
			}
			break;
		case OP_SUBSTR:
			if (params.size() == 3) {
				ret += "SUBSTR(";
				ret += params[0]->to_string();
				ret += " , ";
				ret += params[1]->to_string();
				ret += " , ";
				ret += params[2]->to_string();
				ret += ")";
			}
			break;
		case OP_TO_NUMBER:
			if (params.size() == 1) {
				ret += "TO_NUMBER(";
				ret += params[0]->to_string();
				ret += ")";
			}
			break;
		case OP_DATE_ADD:
			if (params.size() == 2) {
				ret += "DATE_ADD(";
				ret += params[0]->to_string();
				ret += ", ?)";
			}
			break;
		case OP_DATE_SUB:
			if (params.size() == 2) {
				ret += "DATE_SUB(";
				ret += params[0]->to_string();
				ret += ", ?)";
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
	ExprStmt::deep_copy(expr, flag);
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
	} else {
		del_flag(IS_CONST);
	}
	CHECK(deduce_type());
	return ret;
}

u32 OpExprStmt::deduce_type()
{
	u32 ret = SUCCESS;
	u32 i = 0;
	Vector<DataType> src_types;
	Vector<bool> need_cast;
	bool l_need_cast = false;
	DataType l_dst_type;
	bool r_need_cast = false;
	DataType r_dst_type;
	DataType dst_type;
	switch (op_type) {
		case OP_INVALID:
		case OP_MAX:
			ret = ERR_UNEXPECTED;
			break;
		case OP_MINUS:
		case OP_NOT:
		case OP_IS_NULL:
		case OP_IS_NOT_NULL:
		case OP_EXISTS:
		case OP_NOT_EXISTS:
			CHECK(ObjCastUtil::get_result_type(params[0]->res_type,
											   l_need_cast,
											   l_dst_type,
											   params[0]->res_type,
											   r_need_cast,
											   r_dst_type,
											   op_type,
											   dst_type));
			if (l_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[0], l_dst_type, params[0]));
			}
			break;
		case OP_ASSIGN:
			dst_type = params[0]->res_type;
			CHECK(ObjCastUtil::check_need_cast(params[1]->res_type, 
											   dst_type, 
											   r_need_cast));
			if (r_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[1], dst_type, params[1]));
			}
			break;
		case OP_BETWEEN:
		case OP_NOT_BETWEEN:
			CHECK(ObjCastUtil::get_result_type(params[0]->res_type,
											   l_need_cast,
											   l_dst_type,
											   params[1]->res_type,
											   r_need_cast,
											   r_dst_type,
											   op_type,
											   dst_type));
			if (l_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[0], l_dst_type, params[0]));
			}
			if (r_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[1], r_dst_type, params[1]));
			}
			CHECK(ObjCastUtil::get_result_type(params[0]->res_type,
											   l_need_cast,
											   l_dst_type,
											   params[2]->res_type,
											   r_need_cast,
											   r_dst_type,
											   op_type,
											   dst_type));
			if (l_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[0], l_dst_type, params[0]));
			}
			if (r_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[2], r_dst_type, params[2]));
			}
			CHECK(ObjCastUtil::get_result_type(params[1]->res_type,
											   l_need_cast,
											   l_dst_type,
											   params[2]->res_type,
											   r_need_cast,
											   r_dst_type,
											   op_type,
											   dst_type));
			if (l_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[1], l_dst_type, params[1]));
			}
			if (r_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[2], r_dst_type, params[2]));
			}
			break;
		case OP_LIKE:
		case OP_NOT_LIKE: 
			if (!params[0]->res_type.is_varchar() || 
				!params[1]->res_type.is_varchar()) {
				ret = INVALID_CAST;
				return ret;
			}
			dst_type =  params[0]->res_type;
			break;
		case OP_CAST: 
			dst_type = params[1]->res_type;
			break;
		case OP_CASE_WHEN:
			if (params.size() % 2) {
				for (i = 0; i < params.size()-1; i += 2) {
					src_types.push_back(params[i+1]->res_type);
				}
				src_types.push_back(params[i]->res_type);
				CHECK(ObjCastUtil::get_result_type(src_types, need_cast, dst_type));
				MY_ASSERT(src_types.size() == need_cast.size());
				for (i = 0; i < src_types.size()-1; ++i) {
					if (need_cast[i]) {
						CHECK(ObjCastUtil::add_cast(params[i*2+1], dst_type, params[i*2+1]));
					}
				}
				if (need_cast[i]) {
					CHECK(ObjCastUtil::add_cast(params[i*2], dst_type, params[i*2]));
				}
			} else if (params.size() > 1) {
				src_types.push_back(params[0]->res_type);
				for (i = 1; i < params.size()-1; i += 2) {
					src_types.push_back(params[i]->res_type);
				}
				CHECK(ObjCastUtil::get_result_type(src_types, need_cast, dst_type));
				MY_ASSERT(src_types.size() == need_cast.size());
				if (need_cast[0]) {
					CHECK(ObjCastUtil::add_cast(params[0], dst_type, params[0]));
				}
				for (i = 1; i < src_types.size(); ++i) {
					if (need_cast[i]) {
						CHECK(ObjCastUtil::add_cast(params[i*2-1], dst_type, params[i*2-1]));
					}
				}
				src_types.clear();
				need_cast.clear();
				for (i = 1; i < params.size()-1; i += 2) {
					src_types.push_back(params[i+1]->res_type);
				}
				src_types.push_back(params[i]->res_type);
				CHECK(ObjCastUtil::get_result_type(src_types, need_cast, dst_type));
				MY_ASSERT(src_types.size() == need_cast.size());
				for (i = 0; i < src_types.size()-1; ++i) {
					if (need_cast[i]) {
						CHECK(ObjCastUtil::add_cast(params[i*2+2], dst_type, params[i*2+2]));
					}
				}
				if (need_cast[i]) {
					CHECK(ObjCastUtil::add_cast(params[i*2+1], dst_type, params[i*2+1]));
				}
			}
			break;
		case OP_IFNULL:
			for (i = 0; i < params.size(); ++i) {
				src_types.push_back(params[i]->res_type);
			}
			CHECK(ObjCastUtil::get_result_type(src_types, need_cast, dst_type));
			MY_ASSERT(src_types.size() == need_cast.size());
			for (u32 i = 0; i < params.size(); ++i) {
				if (need_cast[i]) {
					CHECK(ObjCastUtil::add_cast(params[i], dst_type, params[i]));
				}
			}
			break;
		case OP_TO_CHAR:
			if (params.size() == 2) {
				if (!params[1]->res_type.is_varchar()) {
					ret = INVALID_CAST;
					return ret;
				}
			}
			dst_type = DataType::default_varchar_type();
			break;
		case OP_SUBSTR:
			if (!params[0]->res_type.is_varchar()) {
				ret = INVALID_CAST;
				return ret;
			}
			if (!params[1]->res_type.is_number()) {
				ret = INVALID_CAST;
				return ret;
			}
			if (!params[2]->res_type.is_number()) {
				ret = INVALID_CAST;
				return ret;
			}
			dst_type = params[0]->res_type;
			break;
		case OP_TO_NUMBER:
			if (params[0]->res_type.is_varchar() ||
				params[0]->res_type.is_number()) {
				
			} else {
				//ret = INVALID_CAST;
				//return ret;
			}
			dst_type = DataType::default_number_type();
			break;
		case OP_DATE_ADD:
		case OP_DATE_SUB:
			if (params[0]->res_type.is_datetime() ||
				params[1]->res_type.is_number()) {
				
			} else {
				ret = INVALID_CAST;
				return ret;
			}
			dst_type = params[0]->res_type;
			break;
		case OP_ADD:
		case OP_SUB:
		case OP_MUL:
		case OP_DIV:
		case OP_EQ:
		case OP_NE:
		case OP_GE:
		case OP_GT:
		case OP_LE:
		case OP_LT:
		case OP_EQ_ANY:
		case OP_NE_ANY:
		case OP_GE_ANY:
		case OP_GT_ANY:
		case OP_LE_ANY:
		case OP_LT_ANY:
		case OP_EQ_ALL:
		case OP_NE_ALL:
		case OP_GE_ALL:
		case OP_GT_ALL:
		case OP_LE_ALL:
		case OP_LT_ALL:
		case OP_IN:
		case OP_NOT_IN:
		case OP_IN_LIST:
		case OP_NOT_IN_LIST:
			ret = (ObjCastUtil::get_result_type(params[0]->res_type,
											   l_need_cast,
											   l_dst_type,
											   params[1]->res_type,
											   r_need_cast,
											   r_dst_type,
											   op_type,
											   dst_type));
			if (FAIL(ret)) {
				LOG_ERR("deduce type failed", K(params), K(ret));
				return ret;
			}
			if (l_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[0], l_dst_type, params[0]));
			}
			if (r_need_cast) {
				CHECK(ObjCastUtil::add_cast(params[1], r_dst_type, params[1]));
			}
			break;
		case OP_AND:
		case OP_OR:
			for (u32 i = 0; i < params.size(); ++i) {
				CHECK(ObjCastUtil::check_logical_type(params[i]->res_type, dst_type));
			}
	}
	res_type = dst_type;
	return ret;
}

bool OpExprStmt::same_as(const ExprStmt_s &other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (OP_EXPR != other->expr_type()) {
		return false;
	}
	OpExprStmt_s expr = other;
	if (op_type != expr->op_type) {
		return false;
	} else if (params.size() != other->params.size()) {
		return false;
	} else {
		for (u32 i = 0; i < params.size(); ++i) {
			if (!params[i]->same_as(other->params[i], ctx)) {
				return false;
			}
		}
		return true;
	}
}


OrderStmt::OrderStmt()
	:asc(true)
{
}

OrderStmt::~OrderStmt()
{
}

ExprType OrderStmt::expr_type()const
{
	return ORDER_EXPR;
}

OrderStmt_s OrderStmt::make_order_stmt(const ExprStmt_s& order_expr, bool asc)
{
	OrderStmt* stmt = new OrderStmt;
	stmt->set_order_by_expr(order_expr);
	stmt->asc = asc;
	return OrderStmt_s(stmt);
}

String OrderStmt::to_string()const 
{
	String ret = get_order_by_expr()->to_string();
	if (asc) {
		ret += " ASC";
	} else {
		ret += " DESC";
	}
	return ret;
}

u32 OrderStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	clear_flag();
	set_is_flag(IS_ORDER_EXPR);
	for (u32 i = 0; i < params.size(); ++i) {
		CHECK(params[i]->formalize());
		table_ids.add_members(params[i]->table_ids);
		add_flags(params[i]->get_flags());
	}
	CHECK(deduce_type());
	return ret;
}

u32 OrderStmt::deduce_type()
{
	u32 ret = SUCCESS;
	res_type = get_order_by_expr()->res_type;
	return ret;
}

u32 OrderStmt::deep_copy(ExprStmt_s &order, u32 flag)const
{
	u32 ret = SUCCESS;
	ExprStmt_s copy_expr;
	CHECK(ExprUtils::deep_copy_expr(get_order_by_expr(), copy_expr, flag));
	order = make_order_stmt(copy_expr, asc);
	return ret;
}

bool OrderStmt::same_as(const ExprStmt_s &other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (ORDER_EXPR != other->expr_type()) {
		return false;
	}
	OrderStmt_s expr = other;
	if (asc != expr->asc) {
		return false;
	} else {
		return get_order_by_expr()->same_as(expr->get_order_by_expr(), ctx);
	}
}

void OrderStmt::set_order_by_expr(const ExprStmt_s& expr)
{
	params.clear();
	params.push_back(expr);
}

ExprStmt_s OrderStmt::get_order_by_expr() const
{
	return params[0];
}

WinExprStmt::WinExprStmt(WinType win_func)
	:win_func(win_func),
	is_distinct(false)
{

}

WinExprStmt::~WinExprStmt()
{

}

ExprType WinExprStmt::expr_type()const
{
	return WIN_EXPR;
}

ExprStmt_s WinExprStmt::make_win_expr_stmt(WinType win_func)
{
	return ExprStmt_s(new WinExprStmt(win_func));
}

String WinExprStmt::to_string()const
{
	String ret = WinTypeString[win_func];
	ret += "(";
	if (has_win_func_expr()) {
		ret += get_win_func_expr()->to_string();
	}
	ret += ")OVER(";
	if (has_part_by_expr()) {
		Vector<ExprStmt_s> exprs;
		get_win_part_by_exprs(exprs);
		ret += "PARTITION BY ";
		for (u32 i = 0; i < exprs.size(); ++i) {
			if (i > 0) {
				ret += ", ";
			}
			ret += exprs[i]->to_string();
		}
		ret += " ";
	}
	if (has_order_by_expr()) {
		Vector<ExprStmt_s> exprs;
		get_win_order_by_exprs(exprs);
		ret += "ORDER BY ";
		for (u32 i = 0; i < exprs.size(); ++i) {
			if (i > 0) {
				ret += ", ";
			}
			ret += exprs[i]->to_string();
		}
	}
	ret += ")";
	return ret;
}

u32 WinExprStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	clear_flag();
	set_is_flag(IS_WINFUNC);
	add_flag(HAS_WINFUNC);
	for (u32 i = 0; i < params.size(); ++i) {
		CHECK(params[i]->formalize());
		table_ids.add_members(params[i]->table_ids);
		add_flags(params[i]->get_flags());
	}
	CHECK(deduce_type());
	return ret;
}

u32 WinExprStmt::deduce_type()
{
	u32 ret = SUCCESS;
	bool need_cast = false;
	ExprStmt_s win_func_expr;
	switch (win_func) {
		case WIN_SUM:
		case WIN_AVG:
			win_func_expr = get_win_func_expr();
			res_type = DataType::default_number_type();
			CHECK(ObjCastUtil::check_need_cast(win_func_expr->res_type, 
											   res_type, 
											   need_cast));
			if (need_cast) {
				CHECK(ObjCastUtil::add_cast(win_func_expr, res_type, win_func_expr));
				set_win_func_expr(win_func_expr);
			}
			break;
		case WIN_COUNT:
			res_type = DataType::default_int_type();
			break;
		case WIN_MAX:
		case WIN_MIN:
			win_func_expr = get_win_func_expr();
			res_type = win_func_expr->res_type;
			break;
		case WIN_RANK:
		case WIN_DENSE_RANK:
		case WIN_ROW_NUMBER:
			res_type = DataType::default_int_type();
			break;
	}
	return ret;
}

u32 WinExprStmt::deep_copy(ExprStmt_s &expr, u32 flag)const
{
	u32 ret = SUCCESS;
	expr = make_win_expr_stmt(win_func);
	WinExprStmt_s win_expr = expr;
	win_expr->func_expr_idx = func_expr_idx;
	win_expr->partition_by_idx = partition_by_idx;
	win_expr->order_by_idx = order_by_idx;
	ExprStmt::deep_copy(expr, flag);
	return ret;
}

bool WinExprStmt::same_as(const ExprStmt_s &other, ExprCompareCtx *ctx)
{
	if (this == other.get()) {
		return true;
	} else if (WIN_EXPR != other->expr_type()) {
		return false;
	}
	WinExprStmt_s expr = other;
	if (win_func != expr->win_func) {
		return false;
	} else if (params.size() != other->params.size() ||
			   func_expr_idx.size() != expr->func_expr_idx.size() ||
			   partition_by_idx.size() != expr->partition_by_idx.size() ||
			   order_by_idx.size() != expr->order_by_idx.size()) {
		return false;
	} else {
		for (u32 i = 0; i < params.size(); ++i) {
			if (!params[i]->same_as(other->params[i], ctx)) {
				return false;
			}
		}
		return true;
	}
}

void WinExprStmt::set_win_func_expr(const ExprStmt_s& expr)
{
	if (params.empty()) {
		params.push_back(expr);
		func_expr_idx.push_back(0);
	} else {
		params[0] = expr;
	}
	
}

const ExprStmt_s& WinExprStmt::get_win_func_expr() const
{
	//MY_ASSERT(has_win_func_expr());
	return params[ func_expr_idx[0] ];
}

void WinExprStmt::set_win_part_by_exprs(const Vector<ExprStmt_s>& exprs)
{
	for (u32 i = 0; i < exprs.size(); ++i) {
		partition_by_idx.push_back( params.size() );
		params.push_back(exprs[i]);
	}
}

void WinExprStmt::get_win_part_by_exprs(Vector<ExprStmt_s>& exprs)const
{
	for (u32 i = 0; i < partition_by_idx.size(); ++i) {
		exprs.push_back(params[partition_by_idx[i]]);
	}
}

void WinExprStmt::set_win_order_by_exprs(const Vector<ExprStmt_s>& exprs)
{
	for (u32 i = 0; i < exprs.size(); ++i) {
		order_by_idx.push_back( params.size() );
		params.push_back(exprs[i]);
	}
}

void WinExprStmt::get_win_order_by_exprs(Vector<ExprStmt_s>& exprs)const
{
	for (u32 i = 0; i < order_by_idx.size(); ++i) {
		exprs.push_back(params[order_by_idx[i]]);
	}
}
