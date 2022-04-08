#ifndef RESULT_TYPE_H
#define RESULT_TYPE_H
#include <string>
typedef std::string String;
#define MAX_PREC				72
#define MAX_SMALLINT_PREC		9
#define MAX_MEDIUMINT_PREC		18
#define MAX_INT_PREC			36
#define MAX_BIGINT_PREC			72
#define MAX_NUM_SCALE	38
#define MAX_STR_LENGTH	10240
#define MAX_TIME_SCALE	9

namespace CatDB {
	namespace Common {
		struct DataType {
			OBJ_TYPE res_type;
			union {
				//for number
				int precision;
				//for varchar, char
				int length;
				TimeType time_type;
			};
			//for number 0-38
			//for timestamp 0-9
			int scale;

			DataType()
			:res_type(T_MAX_TYPE),
			precision(0),
			scale(0)
			{}

			DataType(OBJ_TYPE type)
			:res_type(type),
			precision(0),
			scale(0)
			{}

			DataType(OBJ_TYPE res_type, int length)
				:res_type(res_type),
				length(length),
				scale(0)
			{}

			DataType(OBJ_TYPE res_type, int precision, int scale)
				:res_type(res_type),
				precision(precision),
				scale(scale)
			{}

			DataType(OBJ_TYPE res_type, TimeType time_type, int scale)
				:res_type(res_type),
				time_type(time_type),
				scale(scale)
			{}

			DataType(int type, int prec, int sc)
			{
				if (0 == type) {
					res_type = T_NUMBER;
					precision = prec;
					scale = sc;
				} else if (1 == type) {
					res_type = T_VARCHAR;
					length = prec;
					scale = 0;
				} else if (2 == type) {
					res_type = T_DATETIME;
					if (0 == prec) {
						time_type = TIME;
					} else if (1 == prec) {
						time_type = DATE;
					} else if (2 == prec) {
						time_type = DATETIME;
					} else if (3 == prec) {
						time_type = TIMESTAMP;
					}
					scale = sc;
				}
			}

			bool is_number() const { return res_type == T_NUMBER; }
			bool is_datetime() const { return res_type == T_DATETIME; }
			bool is_varchar() const { return res_type == T_VARCHAR; }
			bool is_bool() const { return res_type == T_BOOL; }

			static DataType default_int_type() { return DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
			static DataType default_number_type() { return DataType(T_NUMBER, MAX_BIGINT_PREC, MAX_NUM_SCALE); }
			static DataType default_datetime_type(TimeType time_type) { return DataType(T_DATETIME, time_type, MAX_TIME_SCALE); }
			static DataType default_varchar_type() { return DataType(T_VARCHAR, MAX_STR_LENGTH); }
			static DataType default_bool_type() { return DataType(T_BOOL); }

			String to_kv_string()const
			{
				if (T_NUMBER == res_type) {
					return String(obj_type_str[res_type]) + "(" +
							std::to_string(precision) + ", " +
							std::to_string(scale) + ")";
				} else if (T_VARCHAR == res_type) {
					return String(obj_type_str[res_type]) + "(" +
							std::to_string(length) + ")";
				} else if (T_DATETIME == res_type) {
					return String(TimeTypeString[time_type]);
				} else if (T_BOOL == res_type) {
					return String(obj_type_str[res_type]);
				} else {
					return "";
				}
			}
		};
    }
}

#endif  //RESULT_TYPE_H