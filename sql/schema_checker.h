#ifndef SCHEMA_CHECKER_H
#define SCHEMA_CHECKER_H
#include "type.h"
#include "row.h"

namespace CatDB {
	namespace Common {
		DECLARE(Object);
	}
	namespace Sql {
		DECLARE(SchemaChecker);
		using Common::RowDesc;
		using Common::ColumnDesc;
		using Common::Object_s;

		class SchemaChecker
		{
		public:
			static SchemaChecker_s make_schema_checker();
			//获取临时表ID
			u32 get_table_id(const String& database, const String& table_name);
			//获取表的行描述
			u32 get_row_desc(const String& database, const String& table_name, RowDesc& row_desc);
			//获取列描述
			u32 get_column_desc(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc);
			//是否含有列
			bool have_column(const String& database, const String& table_name, const String& column_name);
			//模式信息添加一个数据库
			u32 add_database(const String& database);
			//模式信息删除一个数据库
			u32 delete_database(const String& database);
			//模式信息添加一张表
			u32 add_table(const String& database, const String& table, const Vector<Pair<String, String>>& columns);
			//模式信息删除一张表
			u32 delete_table(const String& database, const String& table);
			//从模式表中获取所有的数据库名
			u32 show_database(Vector<String>& databases);
			//从模式表中获取所有的表名
			u32 show_tables(const String& database, Vector<String>& tables);
			//获取建表信息
			u32 desc_table(const String& database, const String& table, Vector<Pair<String, String>>& columns);
			//获取数据库在模式表中的ID
			u32 get_database_id(const String& database, u32& id);
			//获取表再模式表中的ID
			u32 get_table_id(const String& database, const String& table, u32& id);

			//收集统计信息
			u32 analyze_table(const String& database, const String& table);
			u32 statis_table_row_count(const String& database, const String& table);
			u32 statis_column_ndv(const String& database, const String& table_name, const String& column_name);
			u32 statis_column_max_value(const String& database, const String& table_name, const String& column_name);
			u32 statis_column_min_value(const String& database, const String& table_name, const String& column_name);
			u32 statis_table_row_count(u32 tid);
			u32 statis_column_ndv(u32 tid, u32 cid);
			u32 statis_column_max_value(u32 tid, u32 cid);
			u32 statis_column_min_value(u32 tid, u32 cid);
			u32 statis_column_null_count(u32 tid, u32 cid);

			u32 init_db();
		private:
			//获取最后一个数据库的ID
			u32 get_last_database_id(u32& id);
			//获取最后一张表的ID
			u32 get_last_table_id(const String& database, u32& id);
			//从系统查询结果中解析列ID
			u32 get_id_from_result(u32& id, const Object_s& result);
			u32 get_value_from_result(double& value, const Object_s& result);
			//从系统查询结果中解析名字列表
			u32 get_name_from_result(Vector<String>& list, const Object_s& result);
			//从系统查询中解析表信息
			u32 get_columns_from_result(Vector<Pair<String, String>>& list, const Object_s& result);
			u32 get_columns_desc_from_result(ColumnDesc& col_desc, const Object_s& result);
			//从系统查询中解析行描述
			u32 get_row_desc_from_result(u32 table_id, RowDesc& row_desc, const Object_s& result);
			//执行系统SQL
			u32 execute_sys_sql(const String& sql, Object_s& result);
			//获取系统顶层行描述
			u32 get_row_desc_system(const String& database, const String& table_name, RowDesc& row_desc);
			//获取系统顶层列描述
			u32 get_column_desc_system(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc);
			bool have_column_system(const String& database, const String& table_name, const String& column_name);

			u32 analyze_table_system(const String& database, const String& table);
			u32 analyze_table_statis(const String& database, const String& table);
			u32 analyze_column_statis(const String& database, const String& table, const String& column, bool varchar_type=false);
		};
	}
}

#endif	//SCHEMA_CHECKER_H