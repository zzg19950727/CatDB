#ifndef CREATE_TABLE_STMT_H
#define CREATE_TABLE_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class ColumnDefineStmt : public Stmt
		{
		public:
			enum DataType {
				NUMBER = 0,
				DATETIME,
				VARCHAR
			};
		private:
			ColumnDefineStmt();
		public:
			~ColumnDefineStmt();
			StmtType stmt_type()const;
			static Stmt_s make_column_define_stmt(const String& name, u32 data_type);
		public:
			String column_name;
			u32 data_type;
		};

		class CreateTableStmt : public Stmt
		{
		private:
			CreateTableStmt();
		public:
			~CreateTableStmt();
			StmtType stmt_type()const;
			static Stmt_s make_create_table_stmt();
		public:
			Stmt_s table;
			Stmt_s column_define_list;
		};
	}
}

#endif	//CREATE_TABLE_STMT_H