#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H
#include "object.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(QueryResult)

		class QueryResult : public Object
		{
		public:
			QueryResult();
			static QueryResult_s make_query_result();
			u32 size()const;
			u32 add_row(const Row_s& row);
			u32 get_row(u32 idx, Row_s& row);
			u32 cast_to_simple_object(Object_s& obj);

			u32 serialization(u8*& buffer) override;
			bool is_fixed_length() override;
			bool bool_value() override;
			u32 hash() override;
			String to_string()const override;

			Object_s in(const Object_s& other)override;
			Object_s not_in(const Object_s& other)override;
			Object_s exists()override;

			u32 init_title_for_explain(Row_s & result_title);
			u32 add_operation_info(u32 depth, const String& operator_name,
				const String& table_name, double rows, u32 times);
			KV_STRING(
				K(list)
			);

		private:
			Vector<Row_s> list;
		};
	}
}

#endif	//QUERY_RESULT_H