#include "stmt.h"
using namespace CatDB::Parser;

Stmt::Stmt()
{
}

Stmt::~Stmt()
{
}

bool Stmt::is_select_stmt() const 
{ 
    return stmt_type() == Select || SetOperation == stmt_type(); 
}