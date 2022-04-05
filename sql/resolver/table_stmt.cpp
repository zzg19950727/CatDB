#include "expr_stmt.h"
#include "table_stmt.h"
#include "expr_utils.h"
#include "select_stmt.h"
#include "transform_utils.h"

using namespace::CatDB::Parser;
using namespace::CatDB::Transform;

TableStmt::TableStmt()
{
}

TableStmt::~TableStmt()
{
}

void TableStmt::set_alias_name(const String& alias_name)
{
	if (!alias_name.empty()) {
		this->alias_name = alias_name;
	}
}

u32 TableStmt::get_table_exprs(Vector<ExprStmt_s> &exprs)
{
	append(exprs, table_filter);
	return SUCCESS;
}

u32 TableStmt::replace_exprs(const Vector<ExprStmt_s> &old_exprs, 
                             const Vector<ExprStmt_s> &new_exprs)
{
	u32 ret = SUCCESS;
	CHECK(ExprUtils::replace_exprs(old_exprs, new_exprs, table_filter));
	return ret;
}

u32 TableStmt::inner_deep_copy(TableStmt_s &table, QueryCtx_s &ctx, u32 flag)const
{
	u32 ret = SUCCESS;
	MY_ASSERT(table);
	CHECK(ExprUtils::deep_copy_exprs(table_filter, table->table_filter, ctx, flag));
	table->table_type = table_type;
	table->alias_name = alias_name;
	table->table_id = table_id;
	return ret;
}

BasicTableStmt::BasicTableStmt(const String &database, const String& table_name)
	:database(database),
	table_name(table_name),
	is_dual(false)
{
	table_type = BasicTable;
}

TableStmt_s BasicTableStmt::make_basic_table(const String &database, const String& table_name)
{
	return TableStmt_s(new BasicTableStmt(database, table_name));
}

TableStmt_s BasicTableStmt::make_dual_table()
{
	BasicTableStmt* table = new BasicTableStmt("SYS", "DUAL");
	table->is_dual = true;
	return TableStmt_s(table);
}

u32 BasicTableStmt::deep_copy(TableStmt_s &table, QueryCtx_s &ctx, u32 flag)const
{
	u32 ret = SUCCESS;
	table = make_basic_table(database, table_name);
	BasicTableStmt_s basic_table = table;
	CHECK(inner_deep_copy(table, ctx, flag));
	basic_table->ref_table_id = ref_table_id;
	basic_table->is_dual = is_dual;
	return ret;
}

u32 BasicTableStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	table_ids.add_member(table_id);
	for(u32 i = 0; i < table_filter.size(); ++i) {
		CHECK(table_filter[i]->formalize());
	}
	return ret;
}

bool BasicTableStmt::same_as(const TableStmt_s& other)
{
	if (is_dual_table()) {
		return other->is_dual_table();
	} else if (other->is_basic_table()) {
		BasicTableStmt_s basic_other = other;
		return ref_table_id == basic_other->ref_table_id;
	} else {
		return false;
	}
}

TableStmt_s JoinedTableStmt::make_joined_table(TableStmt_s &left_table,
											TableStmt_s &right_table,
											JoinType join_type,
											ExprStmt_s &join_condition)
{
	JoinedTableStmt *joined_table = new JoinedTableStmt();
	joined_table->left_table = left_table;
	joined_table->right_table = right_table;
	joined_table->join_type = join_type;
	joined_table->join_condition.push_back(join_condition);
	return TableStmt_s(joined_table);
}

TableStmt_s JoinedTableStmt::make_joined_table(TableStmt_s &left_table,
											TableStmt_s &right_table,
											JoinType join_type)
{
	JoinedTableStmt *joined_table = new JoinedTableStmt();
	joined_table->left_table = left_table;
	joined_table->right_table = right_table;
	joined_table->join_type = join_type;
	return TableStmt_s(joined_table);
}

u32 JoinedTableStmt::deep_copy(TableStmt_s &table, QueryCtx_s &ctx, u32 flag)const
{
	u32 ret = SUCCESS;
	TableStmt_s copy_left_table;
	TableStmt_s copy_right_table;
	CHECK(left_table->deep_copy(copy_left_table, ctx, flag));
	CHECK(right_table->deep_copy(copy_right_table, ctx, flag));
	table = make_joined_table(copy_left_table, copy_right_table, join_type);
	CHECK(inner_deep_copy(table, ctx, flag));
	JoinedTableStmt_s joined_table = table;
	CHECK(ExprUtils::deep_copy_exprs(join_condition, joined_table->join_condition, ctx, flag));
	return ret;
}

u32 JoinedTableStmt::formalize()
{
	u32 ret = SUCCESS;
	MY_ASSERT(left_table, right_table);
	CHECK(left_table->formalize());
	CHECK(right_table->formalize());
	table_ids.clear();
	table_ids.add_members(left_table->table_ids);
	table_ids.add_members(right_table->table_ids);
	for(u32 i = 0; i < table_filter.size(); ++i) {
		CHECK(table_filter[i]->formalize());
	}
	for(u32 i = 0; i < join_condition.size(); ++i) {
		CHECK(join_condition[i]->formalize());
	}
	return ret;
}

u32 JoinedTableStmt::get_table_items(Vector<TableStmt_s> &table_items)
{
	u32 ret = SUCCESS;
	MY_ASSERT(left_table, right_table);
	if (left_table->is_joined_table()) {
		JoinedTableStmt_s left_joined_table = left_table;
		CHECK(left_joined_table->get_table_items(table_items));
	} else {
		table_items.push_back(left_table);
	}
	if (right_table->is_joined_table()) {
		JoinedTableStmt_s right_joined_table = right_table;
		CHECK(right_joined_table->get_table_items(table_items));
	} else {
		table_items.push_back(right_table);
	}
	return ret;
}

u32 JoinedTableStmt::get_table_exprs(Vector<ExprStmt_s> &exprs)
{
	u32 ret = SUCCESS;
	CHECK(TableStmt::get_table_exprs(exprs));
	append(exprs, join_condition);
	MY_ASSERT(left_table, right_table);
	CHECK(left_table->get_table_exprs(exprs));
	CHECK(right_table->get_table_exprs(exprs));
	return ret;
}

u32 JoinedTableStmt::replace_exprs(const Vector<ExprStmt_s> &old_exprs, 
                              	   const Vector<ExprStmt_s> &new_exprs)
{
	u32 ret = SUCCESS;
	CHECK(TableStmt::replace_exprs(old_exprs, new_exprs));
	CHECK(ExprUtils::replace_exprs(old_exprs, new_exprs, join_condition));
	MY_ASSERT(left_table, right_table);
	CHECK(left_table->replace_exprs(old_exprs, new_exprs));
	CHECK(right_table->replace_exprs(old_exprs, new_exprs));
	return ret;
}

DEFINE_KV_STRING(ViewTableStmt,
				KV(table_type, TableTypeString[table_type]),
				K(alias_name),
				K(table_id),
				K(table_filter),
				K(ref_query)
				);

ViewTableStmt::ViewTableStmt(Stmt_s &ref_query)
							:ref_query(ref_query)
{ 
	table_type = ViewTable; 
}

TableStmt_s ViewTableStmt::make_view_table(Stmt_s ref_query)
{
	return TableStmt_s(new ViewTableStmt(ref_query));
}

u32 ViewTableStmt::deep_copy(TableStmt_s &table, QueryCtx_s &ctx, u32 flag)const
{
	u32 ret = SUCCESS;
	SelectStmt_s copy_ref_query;
	if (ENABLE_COPY_ON_WRITE(flag)) {
		copy_ref_query = ref_query;
		ref_query->increase_ref_count();
	} else {
		CHECK(TransformUtils::deep_copy_stmt(ref_query,
											 copy_ref_query,
											 ctx,
											 flag));
	}
	table = make_view_table(copy_ref_query);
	CHECK(inner_deep_copy(table, ctx, flag));
	return ret;
}

u32 ViewTableStmt::formalize()
{
	u32 ret = SUCCESS;
	table_ids.clear();
	table_ids.add_member(table_id);
	for(u32 i = 0; i < table_filter.size(); ++i) {
		CHECK(table_filter[i]->formalize());
	}
	if (ref_query) {
		CHECK(ref_query->formalize());
	}
	return ret;
}
