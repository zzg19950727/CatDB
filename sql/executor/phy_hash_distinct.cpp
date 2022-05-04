#include "phy_hash_distinct.h"
#include "phy_expression.h"
#include "hash_table.h"
#include "object.h"
#include "error.h"
#include "row.h"
#include "log.h"
using namespace CatDB::Common;
using namespace CatDB::Sql;

CatDB::Sql::PhyHashDistinct::PhyHashDistinct(PhyOperator_s & child)
	:SingleChildPhyOperator(child)
{
}

CatDB::Sql::PhyHashDistinct::~PhyHashDistinct()
{
}

PhyOperator_s CatDB::Sql::PhyHashDistinct::make_hash_distinct(PhyOperator_s & child)
{
	PhyHashDistinct* op = new PhyHashDistinct(child);
	return PhyOperator_s(op);
}

u32 CatDB::Sql::PhyHashDistinct::inner_open()
{
	hash_table = HashTable::make_hash_table();
	hash_table->set_exec_ctx(exec_ctx);
	return child->open();
}

u32 CatDB::Sql::PhyHashDistinct::close()
{
	return child->close();
}

u32 CatDB::Sql::PhyHashDistinct::reset()
{
	hash_table->clear();
	return child->reset();
}

u32 CatDB::Sql::PhyHashDistinct::inner_get_next_row()
{
	u32 ret = SUCCESS;
	Row_s row;
	while (SUCC(child->get_next_row(row))) {
		if (SUCC(hash_table->probe(row))) {
			continue;
		} else if (ROW_NOT_FOUND != ret) {
			return ret;
		} else {
			row = Row::deep_copy(row);
			CHECK(hash_table->build(row));
			set_input_rows(row);
			return ret;
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashDistinct::type() const
{
	return PhyOperator::HASH_DISTINCT;
}

