#ifndef STMT_H
#define STMT_H
#include "error.h"
#include "type.h"

namespace CatDB {
	namespace Parser {
		DECLARE(ColumnDefineStmt);
		using Common::DataType;

        class ColumnDefineStmt
		{
		private:
			ColumnDefineStmt();
		public:
			~ColumnDefineStmt();
			static ColumnDefineStmt_s make_column_define_stmt(const String& name, const DataType &data_type);
            KV_STRING(
                K(column_name),
                K(data_type)
            );

		public:
			String column_name;
			DataType data_type;
		};
		
		DECLARE(Stmt);
		class Stmt
		{
		public:
			Stmt();
			virtual ~Stmt();
			virtual StmtType stmt_type()const = 0;
			virtual u32 formalize() = 0;
			bool is_select_stmt() const;
			VIRTUAL_KV_STRING(
				K("")
			);

		private:
			DISALLOW_COPY_AND_ASSIGN(Stmt);
		};
	}
}
#endif	//STMT_H