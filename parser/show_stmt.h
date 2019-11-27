#ifndef SHOW_STMT_H
#define SHOW_STMT_H

#include "stmt.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(Stmt);

		class ShowTablesStmt : public Stmt
		{
		private:
			ShowTablesStmt(const String& database);
		public:
			~ShowTablesStmt();
			StmtType stmt_type()const;
			static Stmt_s make_show_tables_stmt(const String& database);
		public:
			String database;
		};

		class ShowDatabasesStmt : public Stmt
		{
		private:
			ShowDatabasesStmt(bool is_select_current_database);
		public:
			~ShowDatabasesStmt();
			StmtType stmt_type()const;
			static Stmt_s make_show_databases_stmt(bool is_select_current_database);
		public:
			bool is_select_current_database;
		};

		class DescTableStmt : public Stmt
		{
		private:
			DescTableStmt();
		public:
			~DescTableStmt();
			StmtType stmt_type()const;
			static Stmt_s make_desc_table_stmt();
		public:
			String database;
			Stmt_s table;
		};

		class UseDatabaseStmt : public Stmt
		{
		private:
			UseDatabaseStmt();
		public:
			~UseDatabaseStmt();
			StmtType stmt_type()const;
			static Stmt_s make_use_database_stmt();
		public:
			String database;
		};

		class AnalyzeStmt : public Stmt
		{
		private:
			AnalyzeStmt();
		public:
			~AnalyzeStmt();
			StmtType stmt_type()const;
			static Stmt_s make_analyze_stmt();
		public:
			String database;
			String table;
		};
	}
}

#endif	//SHOW_STMT_H