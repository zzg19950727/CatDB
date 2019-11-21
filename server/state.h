#ifndef STATE_H
#define STATE_H

#include "error.h"

namespace CatDB {
	namespace Server {
		class State
		{
		public:
			struct ErrorStringMap
			{
				const char *jdbc_state;
				const char *odbc_state;
			};
		public:
			/**
			* @return single instance referrence
			*/
			static const State& get_instance()
			{
				return instance_;
			}
			/**
			* 返回odbc sqlstate
			*
			* @param err error code
			*
			* @return 1. 大于0和未定义的转换返回通用错误「S1000」
			*         2. 等于0则返回「00000」表示成功
			*         3. 其余则返回在maps中定义的sqlstate
			*/
			const char* get_odbc_state(int err) const;
			/**
			* 返回jdbc sqlstate
			*
			* @param err error code
			*
			* @return 1. 大于0和未定义的转换返回通用错误「HY000」
			*         2. 等于0则返回「00000」表示成功
			*         3. 其余则返回在maps中定义的sqlstate
			*/
			const char* get_jdbc_state(int err) const;
		private:
			/**
			* 添加一个error code到sqlstate的映射。
			*
			* @param err error code
			* @param jdbc_state jdbc sqlstate
			* @param odbc_state odbc sqlstate
			*/
			void ADD_SQLSTATE(int err, const char *jdbc_state, const char *odbc_state);
			State();
			State(const State &);
		private:
			static ErrorStringMap maps_[MAX_ERROR_CODE];
			static const State instance_;
		};
	}
}

#endif /* STATE_H */
