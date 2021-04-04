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
			//��ȡ��ʱ��ID
			u32 get_table_id(const String& database, const String& table_name);
			//��ȡ����������
			u32 get_row_desc(const String& database, const String& table_name, RowDesc& row_desc);
			//��ȡ������
			u32 get_column_desc(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc);
			//�Ƿ�����
			bool have_column(const String& database, const String& table_name, const String& column_name);
			//ģʽ��Ϣ����һ�����ݿ�
			u32 add_database(const String& database);
			//ģʽ��Ϣɾ��һ�����ݿ�
			u32 delete_database(const String& database);
			//ģʽ��Ϣ����һ�ű�
			u32 add_table(const String& database, const String& table, const Vector<Pair<String, String>>& columns);
			//ģʽ��Ϣɾ��һ�ű�
			u32 delete_table(const String& database, const String& table);
			//��ģʽ���л�ȡ���е����ݿ���
			u32 show_database(Vector<String>& databases);
			//��ģʽ���л�ȡ���еı���
			u32 show_tables(const String& database, Vector<String>& tables);
			//��ȡ������Ϣ
			u32 desc_table(const String& database, const String& table, Vector<Pair<String, String>>& columns);
			//��ȡ���ݿ���ģʽ���е�ID
			u32 get_database_id(const String& database, u32& id);
			//��ȡ����ģʽ���е�ID
			u32 get_table_id(const String& database, const String& table, u32& id);

			//�ռ�ͳ����Ϣ
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
			//��ȡ���һ�����ݿ��ID
			u32 get_last_database_id(u32& id);
			//��ȡ���һ�ű���ID
			u32 get_last_table_id(const String& database, u32& id);
			//��ϵͳ��ѯ����н�����ID
			u32 get_id_from_result(u32& id, const Object_s& result);
			u32 get_value_from_result(double& value, const Object_s& result);
			//��ϵͳ��ѯ����н��������б�
			u32 get_name_from_result(Vector<String>& list, const Object_s& result);
			//��ϵͳ��ѯ�н�������Ϣ
			u32 get_columns_from_result(Vector<Pair<String, String>>& list, const Object_s& result);
			u32 get_columns_desc_from_result(ColumnDesc& col_desc, const Object_s& result);
			//��ϵͳ��ѯ�н���������
			u32 get_row_desc_from_result(u32 table_id, RowDesc& row_desc, const Object_s& result);
			//ִ��ϵͳSQL
			u32 execute_sys_sql(const String& sql, Object_s& result, double sample_size=1);
			//��ȡϵͳ����������
			u32 get_row_desc_system(const String& database, const String& table_name, RowDesc& row_desc);
			//��ȡϵͳ����������
			u32 get_column_desc_system(const String& database, const String& table_name, const String& column_name, ColumnDesc& col_desc);
			bool have_column_system(const String& database, const String& table_name, const String& column_name);

			u32 inner_analyze_table(const String& database, const String& table);
			u32 analyze_table_statis(const String& database, const String& table, double &sample_size);
			u32 analyze_column_statis(const String& database, const String& table, const String& column, double sample_size, bool varchar_type=false);
		};
	}
}

#endif	//SCHEMA_CHECKER_H