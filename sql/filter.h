#ifndef FILTER_H
#define FILTER_H
#include "type.h"

namespace CatDB {
	namespace Common {
		DECLARE(Row);
		DECLARE(Object);
	}
	namespace Sql {
		using Common::Row_s;
		using Common::Object_s;
		DECLARE(Expression);
		DECLARE(Filter);

		class Filter
		{
		private:
			Filter() = delete;
			Filter(const Expression_s& expr);
		public:
			~Filter();
			static Filter_s make_filter(const Expression_s& expr);
			bool operator()(Row_s &row);
			Object_s get_result(const Row_s& row);
			void reset(const Row_s& row);
		private:
			Expression_s expr;
		};
	}
}

#endif	//FILTER_H