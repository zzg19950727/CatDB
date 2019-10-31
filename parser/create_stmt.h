#ifndef CREATE_STMT_H
#define CREATE_STMT_H

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
				VARCHAR,
				INT
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

		class CreateDatabaseStmt : public Stmt
		{
		public:
			CreateDatabaseStmt();
		public:
			~CreateDatabaseStmt();
			StmtType stmt_type()const;
			static Stmt_s make_create_database_stmt();
		public:
			String database;
		};
	}
}

#endif	//CREATE_STMT_H