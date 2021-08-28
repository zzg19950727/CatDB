#include "phy_hash_distinct.h"
#include "phy_expression.h"
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
	return child->open();
}

u32 CatDB::Sql::PhyHashDistinct::close()
{
	return child->close();
}

u32 CatDB::Sql::PhyHashDistinct::reset()
{
	hash_table.clear();
	return child->reset();
}

u32 CatDB::Sql::PhyHashDistinct::inner_get_next_row(Row_s & row)
{
	u32 ret = SUCCESS;
	while ((ret = child->get_next_row(row)) == SUCCESS) {
		if (hash_table.probe(row) == SUCCESS) {
			continue;
		} else {
			row = Row::deep_copy(row);
			hash_table.build(row);
			return ret;
		}
	}
	return ret;
}

u32 CatDB::Sql::PhyHashDistinct::type() const
{
	return PhyOperator::HASH_DISTINCT;
}

