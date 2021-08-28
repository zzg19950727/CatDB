#ifndef PHY_FILTER_H
#define PHY_FILTER_H
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
		DECLARE(PhyFilter);

		class PhyFilter
		{
		private:
			PhyFilter() = delete;
			PhyFilter(const Expression_s& expr);
			PhyFilter(const Vector<Expression_s>& exprs);
		public:
			~PhyFilter();
			static PhyFilter_s make_filter(const Expression_s& expr);
			static PhyFilter_s make_filter(const Vector<Expression_s>& exprs);
			bool operator()(Row_s &row);
		private:
			Vector<Expression_s> exprs;
		};
	}
}

#endif	//PHY_FILTER_H