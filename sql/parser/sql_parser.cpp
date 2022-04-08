// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.
// //                    "%code top" blocks.
#line 38 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:397

	#include "sql_parser.hpp"
	#include "sql_scanner.h"
	#include "sql_driver.h"
	#include "location.hh"
	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
	#include "dml_stmt.h"
	#include "cmd_stmt.h"
	#include "expr_stmt.h"
    #include "table_stmt.h"
	#include "object.h"
	
	/*注意：这里的参数由%parse-param决定*/
	static CatDB::SqlParser::symbol_type yylex(CatDB::SqlScanner& scanner,CatDB::SqlDriver &driver)
	{
		return scanner.get_next_token();
	}
	using namespace CatDB;
	using namespace CatDB::Parser;
	using namespace CatDB::Common;

#line 59 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:397


// First part of user declarations.
#line 63 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:404

	
#define yyerror(fmt, ...) \
{ \
	char tmp[255]; \
	sprintf(tmp, fmt, ##__VA_ARGS__); \
	driver.set_sys_error(tmp); \
}
 
#define check(stmt) \
{ \
	if(!stmt) \
	{ \
		yyerror("make stmt error"); \
		YYABORT; \
	} \
}

#define make_list(ret, from) \
{ \
	ListStmt_s list = ListStmt::make_list_stmt(); \
	check(list); \
	list->push_back(from); \
	ret = list; \
}

#define list_push(ret, list, from) \
{ \
	ListStmt_s stmt = list; \
	check(stmt); \
	stmt->push_back(from); \
	ret = list; \
}

#define make_aggr_stmt(stmt, function_name) \
{ \
	ExprStmt_s expr = AggrStmt::make_aggr_stmt(); \
	check(expr); \
	aggr = expr; \
	stmt = aggr; \
	if(function_name == "sum") \
		aggr->aggr_func = SUM; \
	else if(function_name == "avg") \
		aggr->aggr_func = AVG; \
	else if(function_name == "count") \
		aggr->aggr_func = COUNT; \
	else if(function_name == "min") \
		aggr->aggr_func = MIN; \
	else if(function_name == "max") \
		aggr->aggr_func = MAX; \
	else \
	{ \
		yyerror("unknow aggregate function %s", function_name.c_str()); \
		YYABORT; \
	} \
}

#define make_unary_stmt(stmt, stmt1, op) \
{ \
	stmt = OpExprStmt::make_op_expr_stmt(op); \
	check(stmt); \
	stmt->params.push_back(stmt1); \
}

#define make_binary_stmt(stmt, stmt1, stmt2, op) \
{ \
	stmt = OpExprStmt::make_op_expr_stmt(op); \
	check(stmt); \
	stmt->params.push_back(stmt1); \
	stmt->params.push_back(stmt2); \
}

#define make_ternary_stmt(stmt, stmt1, stmt2, stmt3, op) \
{ \
	stmt = OpExprStmt::make_op_expr_stmt(op); \
	check(stmt); \
	stmt->params.push_back(stmt1); \
	stmt->params.push_back(stmt2); \
	stmt->params.push_back(stmt3); \
}
	
#define str_to_lower(str) \
{\
	for(u32 i = 0; i<str.size(); ++i){\
		if(str[i] >= 'A' && str[i] <= 'Z'){\
			str[i] -= 'A';\
			str[i] += 'a';\
		}\
	}\
}

#line 155 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sql_parser.hpp"

// User implementation prologue.

#line 169 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:412


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 3 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:479
namespace CatDB {
#line 255 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (CatDB::SqlScanner& scanner_yyarg, CatDB::SqlDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 228: // relation_factor
        value.move< BasicTableStmt_s > (that.value);
        break;

      case 212: // column_definition
        value.move< ColumnDefineStmt_s > (that.value);
        break;

      case 214: // data_type
        value.move< DataType > (that.value);
        break;

      case 174: // projection
      case 182: // simple_expr
      case 183: // seconds_expr
      case 184: // arith_expr
      case 186: // cmp_expr
      case 189: // query_ref_expr
      case 190: // case_when_expr
      case 193: // logical_expr
      case 194: // column_ref
      case 195: // expr_const
      case 196: // func_expr
      case 203: // update_asgn_factor
        value.move< ExprStmt_s > (that.value);
        break;

      case 152: // opt_hint
        value.move< Hint > (that.value);
        break;

      case 155: // single_hint
        value.move< HintStmt_s > (that.value);
        break;

      case 160: // leading_hint_table
        value.move< LeadingTable_s > (that.value);
        break;

      case 172: // opt_select_limit
        value.move< LimitStmt_s > (that.value);
        break;

      case 187: // cmp_type
      case 188: // sq_cmp_type
        value.move< OperationType > (that.value);
        break;

      case 170: // order_by
        value.move< OrderStmt_s > (that.value);
        break;

      case 144: // sql_stmt
      case 145: // stmt
      case 146: // cmd_stmt
      case 147: // select_stmt
      case 148: // set_select
      case 149: // sub_set_select
      case 150: // select_with_parens
      case 151: // simple_select
      case 198: // insert_stmt
      case 201: // update_stmt
      case 204: // delete_stmt
      case 205: // explain_stmt
      case 206: // explainable_stmt
      case 207: // create_stmt
      case 218: // drop_stmt
      case 220: // show_stmt
      case 222: // use_stmt
      case 223: // desc_stmt
      case 224: // analyze_stmt
      case 226: // set_var_stmt
      case 227: // kill_stmt
        value.move< Stmt_s > (that.value);
        break;

      case 175: // basic_table_factor
      case 176: // view_table_factor
      case 177: // joined_table_factor
      case 179: // sub_table_factor
      case 180: // table_factor
        value.move< TableStmt_s > (that.value);
        break;

      case 211: // table_element_list
        value.move< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 163: // select_expr_list
      case 165: // opt_where
      case 166: // opt_groupby
      case 167: // opt_having
      case 185: // arith_expr_list
      case 191: // when_then_list1
      case 192: // when_then_list2
      case 200: // insert_value
      case 202: // update_asgn_list
        value.move< Vector<ExprStmt_s> > (that.value);
        break;

      case 153: // opt_hint_list
      case 154: // hint_list
        value.move< Vector<HintStmt_s> > (that.value);
        break;

      case 161: // leading_hint_table_list
        value.move< Vector<LeadingTable_s> > (that.value);
        break;

      case 168: // opt_order_by
      case 169: // order_by_list
        value.move< Vector<OrderStmt_s> > (that.value);
        break;

      case 158: // hint_table_list
      case 208: // opt_view_column_define
      case 209: // view_column_define
      case 217: // opt_engine_def
        value.move< Vector<String> > (that.value);
        break;

      case 164: // from_list
        value.move< Vector<TableStmt_s> > (that.value);
        break;

      case 199: // insert_value_list
        value.move< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 159: // opt_split
      case 162: // opt_distinct
      case 171: // opt_asc_desc
      case 178: // opt_outer
      case 197: // distinct_or_all
      case 213: // opt_not_null
      case 219: // opt_if_exists
        value.move< bool > (that.value);
        break;

      case 225: // opt_sample_size
        value.move< double > (that.value);
        break;

      case 173: // limit_expr
      case 215: // opt_time_precision
      case 216: // opt_char_length
      case 238: // int_value
        value.move< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 156: // opt_qb_name
      case 157: // opt_qb_name_single
      case 181: // opt_alias
      case 210: // beg_view_define
      case 221: // op_from_database
      case 229: // database_name
      case 230: // relation_name
      case 231: // column_name
      case 232: // function_name
      case 233: // column_label
      case 234: // ident
      case 235: // datetime
      case 236: // string
      case 237: // number
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 228: // relation_factor
        value.copy< BasicTableStmt_s > (that.value);
        break;

      case 212: // column_definition
        value.copy< ColumnDefineStmt_s > (that.value);
        break;

      case 214: // data_type
        value.copy< DataType > (that.value);
        break;

      case 174: // projection
      case 182: // simple_expr
      case 183: // seconds_expr
      case 184: // arith_expr
      case 186: // cmp_expr
      case 189: // query_ref_expr
      case 190: // case_when_expr
      case 193: // logical_expr
      case 194: // column_ref
      case 195: // expr_const
      case 196: // func_expr
      case 203: // update_asgn_factor
        value.copy< ExprStmt_s > (that.value);
        break;

      case 152: // opt_hint
        value.copy< Hint > (that.value);
        break;

      case 155: // single_hint
        value.copy< HintStmt_s > (that.value);
        break;

      case 160: // leading_hint_table
        value.copy< LeadingTable_s > (that.value);
        break;

      case 172: // opt_select_limit
        value.copy< LimitStmt_s > (that.value);
        break;

      case 187: // cmp_type
      case 188: // sq_cmp_type
        value.copy< OperationType > (that.value);
        break;

      case 170: // order_by
        value.copy< OrderStmt_s > (that.value);
        break;

      case 144: // sql_stmt
      case 145: // stmt
      case 146: // cmd_stmt
      case 147: // select_stmt
      case 148: // set_select
      case 149: // sub_set_select
      case 150: // select_with_parens
      case 151: // simple_select
      case 198: // insert_stmt
      case 201: // update_stmt
      case 204: // delete_stmt
      case 205: // explain_stmt
      case 206: // explainable_stmt
      case 207: // create_stmt
      case 218: // drop_stmt
      case 220: // show_stmt
      case 222: // use_stmt
      case 223: // desc_stmt
      case 224: // analyze_stmt
      case 226: // set_var_stmt
      case 227: // kill_stmt
        value.copy< Stmt_s > (that.value);
        break;

      case 175: // basic_table_factor
      case 176: // view_table_factor
      case 177: // joined_table_factor
      case 179: // sub_table_factor
      case 180: // table_factor
        value.copy< TableStmt_s > (that.value);
        break;

      case 211: // table_element_list
        value.copy< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 163: // select_expr_list
      case 165: // opt_where
      case 166: // opt_groupby
      case 167: // opt_having
      case 185: // arith_expr_list
      case 191: // when_then_list1
      case 192: // when_then_list2
      case 200: // insert_value
      case 202: // update_asgn_list
        value.copy< Vector<ExprStmt_s> > (that.value);
        break;

      case 153: // opt_hint_list
      case 154: // hint_list
        value.copy< Vector<HintStmt_s> > (that.value);
        break;

      case 161: // leading_hint_table_list
        value.copy< Vector<LeadingTable_s> > (that.value);
        break;

      case 168: // opt_order_by
      case 169: // order_by_list
        value.copy< Vector<OrderStmt_s> > (that.value);
        break;

      case 158: // hint_table_list
      case 208: // opt_view_column_define
      case 209: // view_column_define
      case 217: // opt_engine_def
        value.copy< Vector<String> > (that.value);
        break;

      case 164: // from_list
        value.copy< Vector<TableStmt_s> > (that.value);
        break;

      case 199: // insert_value_list
        value.copy< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 159: // opt_split
      case 162: // opt_distinct
      case 171: // opt_asc_desc
      case 178: // opt_outer
      case 197: // distinct_or_all
      case 213: // opt_not_null
      case 219: // opt_if_exists
        value.copy< bool > (that.value);
        break;

      case 225: // opt_sample_size
        value.copy< double > (that.value);
        break;

      case 173: // limit_expr
      case 215: // opt_time_precision
      case 216: // opt_char_length
      case 238: // int_value
        value.copy< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 156: // opt_qb_name
      case 157: // opt_qb_name_single
      case 181: // opt_alias
      case 210: // beg_view_define
      case 221: // op_from_database
      case 229: // database_name
      case 230: // relation_name
      case 231: // column_name
      case 232: // function_name
      case 233: // column_label
      case 234: // ident
      case 235: // datetime
      case 236: // string
      case 237: // number
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 228: // relation_factor
        yylhs.value.build< BasicTableStmt_s > ();
        break;

      case 212: // column_definition
        yylhs.value.build< ColumnDefineStmt_s > ();
        break;

      case 214: // data_type
        yylhs.value.build< DataType > ();
        break;

      case 174: // projection
      case 182: // simple_expr
      case 183: // seconds_expr
      case 184: // arith_expr
      case 186: // cmp_expr
      case 189: // query_ref_expr
      case 190: // case_when_expr
      case 193: // logical_expr
      case 194: // column_ref
      case 195: // expr_const
      case 196: // func_expr
      case 203: // update_asgn_factor
        yylhs.value.build< ExprStmt_s > ();
        break;

      case 152: // opt_hint
        yylhs.value.build< Hint > ();
        break;

      case 155: // single_hint
        yylhs.value.build< HintStmt_s > ();
        break;

      case 160: // leading_hint_table
        yylhs.value.build< LeadingTable_s > ();
        break;

      case 172: // opt_select_limit
        yylhs.value.build< LimitStmt_s > ();
        break;

      case 187: // cmp_type
      case 188: // sq_cmp_type
        yylhs.value.build< OperationType > ();
        break;

      case 170: // order_by
        yylhs.value.build< OrderStmt_s > ();
        break;

      case 144: // sql_stmt
      case 145: // stmt
      case 146: // cmd_stmt
      case 147: // select_stmt
      case 148: // set_select
      case 149: // sub_set_select
      case 150: // select_with_parens
      case 151: // simple_select
      case 198: // insert_stmt
      case 201: // update_stmt
      case 204: // delete_stmt
      case 205: // explain_stmt
      case 206: // explainable_stmt
      case 207: // create_stmt
      case 218: // drop_stmt
      case 220: // show_stmt
      case 222: // use_stmt
      case 223: // desc_stmt
      case 224: // analyze_stmt
      case 226: // set_var_stmt
      case 227: // kill_stmt
        yylhs.value.build< Stmt_s > ();
        break;

      case 175: // basic_table_factor
      case 176: // view_table_factor
      case 177: // joined_table_factor
      case 179: // sub_table_factor
      case 180: // table_factor
        yylhs.value.build< TableStmt_s > ();
        break;

      case 211: // table_element_list
        yylhs.value.build< Vector<ColumnDefineStmt_s> > ();
        break;

      case 163: // select_expr_list
      case 165: // opt_where
      case 166: // opt_groupby
      case 167: // opt_having
      case 185: // arith_expr_list
      case 191: // when_then_list1
      case 192: // when_then_list2
      case 200: // insert_value
      case 202: // update_asgn_list
        yylhs.value.build< Vector<ExprStmt_s> > ();
        break;

      case 153: // opt_hint_list
      case 154: // hint_list
        yylhs.value.build< Vector<HintStmt_s> > ();
        break;

      case 161: // leading_hint_table_list
        yylhs.value.build< Vector<LeadingTable_s> > ();
        break;

      case 168: // opt_order_by
      case 169: // order_by_list
        yylhs.value.build< Vector<OrderStmt_s> > ();
        break;

      case 158: // hint_table_list
      case 208: // opt_view_column_define
      case 209: // view_column_define
      case 217: // opt_engine_def
        yylhs.value.build< Vector<String> > ();
        break;

      case 164: // from_list
        yylhs.value.build< Vector<TableStmt_s> > ();
        break;

      case 199: // insert_value_list
        yylhs.value.build< Vector<Vector<ExprStmt_s>> > ();
        break;

      case 159: // opt_split
      case 162: // opt_distinct
      case 171: // opt_asc_desc
      case 178: // opt_outer
      case 197: // distinct_or_all
      case 213: // opt_not_null
      case 219: // opt_if_exists
        yylhs.value.build< bool > ();
        break;

      case 225: // opt_sample_size
        yylhs.value.build< double > ();
        break;

      case 173: // limit_expr
      case 215: // opt_time_precision
      case 216: // opt_char_length
      case 238: // int_value
        yylhs.value.build< int > ();
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 156: // opt_qb_name
      case 157: // opt_qb_name_single
      case 181: // opt_alias
      case 210: // beg_view_define
      case 221: // op_from_database
      case 229: // database_name
      case 230: // relation_name
      case 231: // column_name
      case 232: // function_name
      case 233: // column_label
      case 234: // ident
      case 235: // datetime
      case 236: // string
      case 237: // number
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 363 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 1147 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 370 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1153 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 371 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1159 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 372 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1165 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 373 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1171 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 374 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1177 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 375 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1183 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 376 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow stmt"); }
#line 1189 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 385 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1195 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 386 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1201 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 387 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1207 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 388 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1213 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 389 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 390 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1225 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 391 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1231 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 392 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1237 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 397 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1245 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 401 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1253 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 408 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1262 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 413 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1271 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 418 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1280 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 423 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1289 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 428 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1298 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 433 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1307 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 438 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1316 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 443 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1325 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 451 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1333 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 455 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1341 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 462 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1349 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 472 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建select stmt
		SelectStmt_s select_stmt = SelectStmt::make_select_stmt();
		select_stmt->stmt_hint = yystack_[9].value.as< Hint > ();
		select_stmt->is_distinct = yystack_[8].value.as< bool > ();
		select_stmt->select_expr_list = yystack_[7].value.as< Vector<ExprStmt_s> > ();
		select_stmt->from_stmts = yystack_[5].value.as< Vector<TableStmt_s> > ();
		select_stmt->where_stmt = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		select_stmt->group_exprs = yystack_[3].value.as< Vector<ExprStmt_s> > ();
		select_stmt->having_stmt = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		select_stmt->order_exprs = yystack_[1].value.as< Vector<OrderStmt_s> > ();
		select_stmt->limit_stmt = yystack_[0].value.as< LimitStmt_s > ();
		yylhs.value.as< Stmt_s > () = select_stmt;
    }
#line 1368 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 489 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Hint > () = Hint(); }
#line 1374 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 491 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[1].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = false;
	}
#line 1384 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 497 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[2].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = true;
	}
#line 1394 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 506 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
	}
#line 1402 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 510 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[0].value.as< Vector<HintStmt_s> > ();
	}
#line 1410 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 517 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1421 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 524 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[1].value.as< Vector<HintStmt_s> > ();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1432 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 535 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(NO_REWRITE);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1441 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 540 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1450 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 545 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1459 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 550 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1468 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 555 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1477 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 561 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1489 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 569 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1501 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 577 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1513 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 585 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1525 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 593 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->set_is_ordered();
	}
#line 1536 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 600 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1549 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 610 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(PARALLEL);
		ParallelHintStmt_s hint = yylhs.value.as< HintStmt_s > ();
		hint->parallel = yystack_[1].value.as< int > ();
	}
#line 1559 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 618 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1565 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 620 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1573 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 626 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1579 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 628 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "";
	}
#line 1587 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 632 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > ();
	}
#line 1595 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 639 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1604 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 644 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1613 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 651 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1619 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 652 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1625 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 657 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = true;
		yylhs.value.as< LeadingTable_s > ()->table_name = yystack_[0].value.as< std::string > ();
	}
#line 1635 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 663 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = false;
		yylhs.value.as< LeadingTable_s > ()->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1645 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 672 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = Vector<LeadingTable_s>();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1654 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 677 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = yystack_[2].value.as< Vector<LeadingTable_s> > ();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1663 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 684 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1669 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 685 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1675 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 690 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1684 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 695 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yystack_[2].value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
    }
#line 1693 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 703 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = Vector<TableStmt_s>();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1702 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 708 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = yystack_[2].value.as< Vector<TableStmt_s> > ();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1711 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 715 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1717 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 716 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1723 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 720 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1729 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 721 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1735 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 725 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1741 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 726 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1747 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 730 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>(); }
#line 1753 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 731 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[0].value.as< Vector<OrderStmt_s> > (); }
#line 1759 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 736 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1768 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 741 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[2].value.as< Vector<OrderStmt_s> > ();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1777 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 749 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OrderStmt_s > () = OrderStmt::make_order_stmt(yystack_[1].value.as< ExprStmt_s > (), yystack_[0].value.as< bool > ());
    }
#line 1785 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 755 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1791 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 756 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1797 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 757 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1803 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 761 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< LimitStmt_s > () = NULL;}
#line 1809 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 763 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
    }
#line 1820 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 770 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
	}
#line 1831 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 780 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = yystack_[0].value.as< int > ();
	}
#line 1839 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 787 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< ExprStmt_s > ()->to_string();
    }
#line 1849 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 793 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1859 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 799 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[2].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1869 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 808 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< BasicTableStmt_s > ();
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
    }
#line 1878 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 816 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		yylhs.value.as< TableStmt_s > () = ViewTableStmt::make_view_table(yystack_[2].value.as< Stmt_s > ());
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1888 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 825 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), LeftOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1896 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 829 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), RightOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1904 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 833 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), FullOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1912 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 837 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), Inner, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1920 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 844 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 1928 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 848 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 1936 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 855 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1944 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 859 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1952 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 863 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< TableStmt_s > ();
	}
#line 1960 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 870 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1968 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 874 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1976 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 878 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1984 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 884 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1990 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 886 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1998 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 890 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2006 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 902 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2014 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 906 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2022 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 910 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2030 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 914 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2038 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 918 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2046 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 925 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_day(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2060 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 935 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_month(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2074 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 945 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_year(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2088 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 958 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2096 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 962 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2105 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 967 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MINUS);
    }
#line 2114 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 972 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_ADD);
	}
#line 2123 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 977 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_SUB);
	}
#line 2132 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 982 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MUL);
	}
#line 2141 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 987 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DIV);
	}
#line 2150 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 992 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_ADD);
  }
#line 2159 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 997 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_SUB);
  }
#line 2168 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 1002 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2176 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 1010 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2186 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1016 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2196 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1025 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2205 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1030 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), OP_IS_NULL);
    }
#line 2214 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1035 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), OP_IS_NOT_NULL);
    }
#line 2223 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1040 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[4].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_BETWEEN);
    }
#line 2232 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1045 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_BETWEEN);
    }
#line 2241 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1050 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		ExprStmt_s stmt = OpExprStmt::make_op_expr_stmt(OP_IN_LIST);
		check(stmt);
		stmt->params.push_back(yystack_[4].value.as< ExprStmt_s > ());
		ExprStmt_s list = ListStmt::make_list_stmt();
		list->params = yystack_[1].value.as< Vector<ExprStmt_s> > ();
		stmt->params.push_back(list);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2256 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1061 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		ExprStmt_s stmt = OpExprStmt::make_op_expr_stmt(OP_NOT_IN_LIST);
		check(stmt);
		stmt->params.push_back(yystack_[5].value.as< ExprStmt_s > ());
		ExprStmt_s list = ListStmt::make_list_stmt();
		list->params = yystack_[1].value.as< Vector<ExprStmt_s> > ();
		stmt->params.push_back(list);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2271 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1072 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_IN);
    }
#line 2280 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1077 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_IN);
    }
#line 2289 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1082 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EXISTS);
    }
#line 2297 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1086 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_EXISTS);
    }
#line 2306 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1091 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2315 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1099 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE;
	}
#line 2323 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1103 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT;
	}
#line 2331 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1107 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ;
	}
#line 2339 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1111 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE;
	}
#line 2347 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1115 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT;
	}
#line 2355 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1119 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE;
	}
#line 2363 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1123 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LIKE;
	}
#line 2371 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1127 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NOT_LIKE;
	}
#line 2379 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1134 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ANY;
	}
#line 2387 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1138 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ANY;
	}
#line 2395 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1142 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ANY;
	}
#line 2403 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1146 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ANY;
	}
#line 2411 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1150 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ANY;
	}
#line 2419 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1154 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ANY;
	}
#line 2427 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1158 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ALL;
	}
#line 2435 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1162 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ALL;
	}
#line 2443 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1166 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ALL;
	}
#line 2451 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1170 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ALL;
	}
#line 2459 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1174 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ALL;
	}
#line 2467 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1178 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ALL;
	}
#line 2475 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1185 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< ExprStmt_s > () = query_expr;
    }
#line 2485 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1194 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[4].value.as< ExprStmt_s > ());
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2496 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1201 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2506 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1210 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2516 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1216 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2526 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1225 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2536 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1231 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2546 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1240 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2554 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1244 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2562 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1248 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_AND);
    }
#line 2571 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1253 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_OR);
    }
#line 2580 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1261 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2591 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1268 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2601 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1274 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2612 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1282 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2623 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1289 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2635 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1297 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2647 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1308 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2660 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1317 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		if (!DateTime::is_valid_datetime(yystack_[0].value.as< std::string > ())) {
			yyerror("invalid datetime");
			YYABORT;
		}
		Object_s value = DateTime::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2677 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1330 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2690 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1339 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2703 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1351 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[3].value.as< std::string > ());
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->set_aggr_expr(col);
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 2716 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1360 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[4].value.as< std::string > ());
		aggr->distinct = yystack_[2].value.as< bool > ();
		aggr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 2728 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1368 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[3].value.as< std::string > ());
		aggr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 2739 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1375 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_CHAR);
  }
#line 2747 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1379 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	Object_s value = Varchar::make_object(yystack_[1].value.as< std::string > ());
	check(value);
	ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
	check(stmt);
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), stmt, OP_TO_CHAR);
  }
#line 2759 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1387 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	Object_s value1 = Number::make_object(yystack_[3].value.as< std::string > ());
	check(value1);
	ExprStmt_s stmt1 = ConstStmt::make_const_stmt(value1);
	check(stmt1);
	Object_s value2 = Number::make_object(yystack_[1].value.as< std::string > ());
	check(value2);
	ExprStmt_s stmt2 = ConstStmt::make_const_stmt(value2);
	check(stmt2);
	make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), stmt1, stmt2, OP_SUBSTR);
  }
#line 2775 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1399 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_IFNULL);
  }
#line 2783 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1403 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_NUMBER);
  }
#line 2791 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1410 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 2799 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1414 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 2807 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 1426 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		insert_stmt->value_list = yystack_[0].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2820 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 1435 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[1].value.as< BasicTableStmt_s > ();
		insert_stmt->query_values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2832 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 1446 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = Vector<Vector<ExprStmt_s>>();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
    }
#line 2841 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 1451 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = yystack_[4].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
	}
#line 2850 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 1459 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2859 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 1464 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2868 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 1477 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		UpdateStmt_s update_stmt = UpdateStmt::make_update_stmt();
		check(update_stmt);
		update_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		update_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		update_stmt->from_stmts.push_back(yystack_[3].value.as< BasicTableStmt_s > ());
		update_stmt->update_assign_stmt = yystack_[1].value.as< Vector<ExprStmt_s> > ();
		update_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = update_stmt;
    }
#line 2883 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 1488 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		UpdateStmt_s update_stmt = UpdateStmt::make_update_stmt();
		check(update_stmt);
		update_stmt->stmt_hint = yystack_[6].value.as< Hint > ();
		update_stmt->table = yystack_[5].value.as< BasicTableStmt_s > ();
		update_stmt->update_assign_stmt = yystack_[3].value.as< Vector<ExprStmt_s> > ();
		update_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		update_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = update_stmt;
    }
#line 2898 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 1502 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2908 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 1508 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2918 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 1517 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), col, yystack_[0].value.as< ExprStmt_s > (), OP_ASSIGN);
    }
#line 2929 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 1532 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		delete_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		yystack_[2].value.as< BasicTableStmt_s > ()->set_alias_name(yystack_[1].value.as< std::string > ());
		delete_stmt->from_stmts.push_back(yystack_[2].value.as< BasicTableStmt_s > ());
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 2944 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 1543 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[5].value.as< Hint > ();
		delete_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		yystack_[2].value.as< BasicTableStmt_s > ()->set_alias_name(yystack_[1].value.as< std::string > ()); 
		delete_stmt->from_stmts.push_back(yystack_[2].value.as< BasicTableStmt_s > ());
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 2959 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 204:
#line 1554 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		delete_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		delete_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 2973 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 205:
#line 1572 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		DMLStmt_s dml = yylhs.value.as< Stmt_s > ();
		dml->is_explain = true;
    }
#line 2983 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 206:
#line 1580 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2989 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 207:
#line 1581 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2995 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 208:
#line 1582 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3001 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 209:
#line 1583 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3007 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 210:
#line 1593 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateTable);
		check(cmd_stmt);
		cmd_stmt->params.create_table_params.table = yystack_[4].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.create_table_params.column_define_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		cmd_stmt->params.create_table_params.engine_args = yystack_[0].value.as< Vector<String> > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3020 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 211:
#line 1602 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3031 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 212:
#line 1609 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateView);
		check(cmd_stmt);
		cmd_stmt->params.create_view_params.database = driver.get_global_database();
		cmd_stmt->params.create_view_params.view_name = yystack_[4].value.as< std::string > ();
		cmd_stmt->params.create_view_params.column_define = yystack_[3].value.as< Vector<String> > ();
		cmd_stmt->params.create_view_params.view_define_sql = yystack_[1].value.as< std::string > ();
		cmd_stmt->params.create_view_params.ref_query = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3046 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 213:
#line 1623 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
	}
#line 3054 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 214:
#line 1627 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[1].value.as< Vector<String> > ();
	}
#line 3062 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 215:
#line 1634 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3071 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 216:
#line 1639 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3080 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 217:
#line 1647 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.left_string();
	}
#line 3088 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 218:
#line 1654 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3097 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 219:
#line 1659 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3106 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 220:
#line 1667 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[2].value.as< std::string > (), yystack_[1].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
    }
#line 3116 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 221:
#line 1676 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 3124 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 222:
#line 1680 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		
	}
#line 3132 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 223:
#line 1687 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3138 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 224:
#line 1689 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3144 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 225:
#line 1691 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_MEDIUMINT_PREC, 0); }
#line 3150 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 226:
#line 1693 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3156 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 227:
#line 1695 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3162 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 228:
#line 1697 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
#line 3168 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 229:
#line 1699 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3174 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 230:
#line 1701 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3180 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 231:
#line 1703 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3186 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 232:
#line 1705 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3192 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 233:
#line 1707 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3198 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 234:
#line 1709 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3204 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 235:
#line 1711 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3210 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 236:
#line 1713 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3216 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 237:
#line 1715 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3222 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 238:
#line 1717 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3228 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 239:
#line 1719 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3234 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 240:
#line 1721 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3240 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 241:
#line 1723 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIMESTAMP, yystack_[0].value.as< int > ()); }
#line 3246 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 242:
#line 1725 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATETIME); }
#line 3252 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 243:
#line 1727 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATE); }
#line 3258 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 244:
#line 1729 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIME); }
#line 3264 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 245:
#line 1733 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_TIME_SCALE; }
#line 3270 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 246:
#line 1735 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		if (yystack_[1].value.as< int > () > MAX_TIME_SCALE) {
			yyerror("max time prec:%d", MAX_TIME_SCALE);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	}
#line 3282 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 247:
#line 1745 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_STR_LENGTH; }
#line 3288 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 248:
#line 1747 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		  if (yystack_[1].value.as< int > () > MAX_STR_LENGTH) {
			yyerror("max char length:%d", MAX_STR_LENGTH);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	  }
#line 3300 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 249:
#line 1758 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<String> > () = Vector<String>(); 
		yylhs.value.as< Vector<String> > ().push_back("CAT");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
	}
#line 3312 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 250:
#line 1766 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back("CSV");
		yylhs.value.as< Vector<String> > ().push_back(yystack_[10].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back(yystack_[5].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back("new line");
	}
#line 3324 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 251:
#line 1782 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_table_params.table = yystack_[0].value.as< TableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3336 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 252:
#line 1790 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3348 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 253:
#line 1798 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropView);
		check(cmd_stmt);
		cmd_stmt->params.drop_view_params.database = driver.get_global_database();
		cmd_stmt->params.drop_view_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_view_params.view_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3361 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 254:
#line 1809 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3367 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 255:
#line 1810 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3373 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 256:
#line 1820 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
 	}
#line 3384 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 257:
#line 1827 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3395 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 258:
#line 1834 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3406 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 259:
#line 1841 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3417 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 260:
#line 1848 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3425 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 261:
#line 1852 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3433 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 262:
#line 1856 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3445 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 263:
#line 1864 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3457 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 264:
#line 1872 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowProcesslist);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3467 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 265:
#line 1878 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowMemory);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3477 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 266:
#line 1887 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 3485 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 267:
#line 1891 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 3493 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 268:
#line 1898 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(UseDatabase);
		check(cmd_stmt);
		cmd_stmt->params.use_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3504 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 269:
#line 1913 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3515 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 270:
#line 1920 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3526 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 271:
#line 1934 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = yystack_[1].value.as< std::string > ();
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3539 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 272:
#line 1943 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3552 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 273:
#line 1952 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3565 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 274:
#line 1963 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {	yylhs.value.as< double > () = 0.1; }
#line 3571 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 275:
#line 1965 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< double > () = std::stod(yystack_[0].value.as< std::string > ());
	}
#line 3579 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 276:
#line 1972 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(SetVar);
		check(cmd_stmt);
		cmd_stmt->params.set_var_params.var_name = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.set_var_params.var_value = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3591 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 277:
#line 1983 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		cmd_stmt->params.kill_params.thread_id = yystack_[0].value.as< int > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3602 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 278:
#line 1997 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3614 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 279:
#line 2005 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3626 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 280:
#line 2013 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3637 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 281:
#line 2022 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3643 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 282:
#line 2026 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3649 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 283:
#line 2030 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3655 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 284:
#line 2034 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3661 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 285:
#line 2038 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 3667 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 286:
#line 2042 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3673 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 287:
#line 2046 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3679 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 288:
#line 2047 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3685 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 289:
#line 2051 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3691 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 290:
#line 2055 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3697 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 291:
#line 2060 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 3705 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;


#line 3709 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int parser::yypact_ninf_ = -480;

  const short int parser::yytable_ninf_ = -285;

  const short int
  parser::yypact_[] =
  {
     759,   -67,    40,    38,    19,    19,    45,   171,     3,    53,
     -54,    38,   134,   602,    38,   134,   176,    67,  -480,  -480,
     417,   601,  -480,   -23,  -480,  -480,  -480,  -480,  -480,  -480,
    -480,  -480,  -480,  -480,  -480,  -480,    82,   134,    19,   134,
     212,   317,  -480,  -480,  -480,    86,  -480,   101,  -480,   124,
     124,   124,  -480,  -480,  -480,  -480,  -480,    19,  -480,  -480,
    -480,   108,   172,   200,   120,   186,  -480,   139,   216,  -480,
    -480,  -480,   197,   219,    19,  -480,  -480,  -480,  -480,   135,
     -54,   -54,   142,   -54,   -54,   215,   220,  -480,   252,   260,
     829,   284,   299,   299,   299,   299,   313,   314,   299,   319,
     299,   322,   331,   283,   829,  -480,    19,   338,   360,   134,
     367,   134,   302,   134,   -36,  -480,  -480,   749,   423,    19,
      19,   219,    19,    19,   134,  -480,   334,   -54,  -480,  -480,
    -480,  -480,   -54,  -480,  -480,  -480,   361,   194,   134,   134,
     438,   393,   435,    28,  -480,  -480,  -480,  -480,   435,   435,
    -480,    53,  -480,   435,   435,  -480,  -480,    23,    19,   302,
    -480,  -480,  -480,  -480,   -54,  -480,  -480,  -480,   482,    23,
    -480,   384,  -480,  -480,  -480,   572,   423,   423,   388,   649,
     749,  -480,   749,  -480,   389,   391,   394,  -480,     0,  -480,
    -480,   166,  -480,  -480,  -480,  -480,  -480,   378,  -480,   399,
     -47,  -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,
    -480,   134,  -480,  -480,   372,   372,   372,    41,  -480,   611,
    -480,   131,  -480,  -480,   428,  -480,     5,   380,  -480,   134,
     134,   381,   134,   134,   134,   365,  -480,    23,   -33,   482,
     386,   398,   421,   398,   398,  -480,   749,   466,   521,   159,
      10,  -480,  -480,   749,   619,   488,  -480,  -480,   749,   749,
     749,   749,   302,   134,   749,   721,   749,   721,  -480,  -480,
      61,   660,   113,  -480,   498,   401,  -480,  -480,  -480,   134,
     453,  -480,  -480,   434,   434,  -480,  -480,   437,  -480,  -480,
    -480,  -480,  -480,   441,  -480,  -480,   444,   444,  -480,   434,
     434,   525,   134,  -480,   -54,  -480,     5,  -480,   151,  -480,
    -480,   153,  -480,   156,  -480,   182,   249,  -480,   521,  -480,
     365,   302,  -480,    23,  -480,   458,   330,   462,   472,   511,
     255,   460,   491,   473,   442,   851,  -480,     9,   749,    43,
     749,   521,   497,  -480,   529,   379,   670,  -480,   -33,  -480,
    -480,   423,  -480,   -28,  -480,  -480,   -28,  -480,  -480,  -480,
    -480,  -480,   448,   711,   749,   302,   134,  -480,   749,    53,
    -480,   550,  -480,    53,  -480,  -480,    53,    53,    53,  -480,
    -480,  -480,  -480,   543,  -480,  -480,  -480,   256,  -480,  -480,
       5,  -480,   134,  -480,  -480,  -480,    42,  -480,   482,  -480,
     330,    18,  -480,  -480,   477,   330,   330,   749,  -480,   749,
     473,  -480,   426,    12,   318,   158,   286,   356,   387,   397,
     431,  -480,   749,   495,    60,   749,   473,   521,   521,   749,
     357,   749,   749,   341,    20,   749,    53,   423,  -480,  -480,
     517,    -2,  -480,  -480,   746,   -33,  -480,   511,  -480,   544,
     487,   559,   560,   499,  -480,  -480,  -480,  -480,   516,   -54,
     500,   482,   521,   528,   530,   511,   264,  -480,  -480,  -480,
     749,   542,  -480,  -480,  -480,  -480,  -480,  -480,  -480,  -480,
    -480,  -480,  -480,  -480,    95,   649,  -480,   613,  -480,   511,
    -480,   626,  -480,   511,   749,   403,   402,  -480,   749,   790,
     596,   527,   604,   578,  -480,  -480,  -480,  -480,  -480,   612,
    -480,    53,    53,  -480,   521,   541,  -480,    42,   521,   521,
    -480,   145,   649,  -480,   749,   511,   270,  -480,   511,  -480,
     749,   511,  -480,    53,  -480,   749,   521,   554,   582,   551,
     556,    42,   824,    42,    42,   749,   271,   511,   749,  -480,
     511,   557,   620,    42,   635,   591,   667,  -480,  -480,   511,
    -480,   511,  -480,   749,    53,  -480,   423,   640,  -480,   247,
     648,  -480,   651,   749,  -480,  -480,  -480,    53,   628,  -480,
    -480,   579,   664,   423,   662,   618,   586,   672,   423,  -480
  };

  const unsigned short int
  parser::yydefact_[] =
  {
       9,     0,     0,    32,     0,     0,     0,     0,     0,     0,
       0,    32,     0,     0,    32,     0,     0,     0,     3,     4,
      19,     0,    29,    28,     5,     6,     7,     8,    11,    12,
      10,    14,    13,    15,    16,    17,     0,     0,     0,     0,
      35,     0,   286,   280,   270,     0,   278,   282,   269,   254,
     254,   254,   206,   208,   209,   207,   205,     0,   290,   291,
     277,     0,    64,     0,     0,     0,   256,     0,     0,   265,
     264,   261,     0,   267,     0,   268,   281,     1,     2,     0,
       0,     0,     0,     0,     0,     0,     0,   211,     0,   213,
      35,     0,    53,    53,    53,    53,     0,     0,    53,     0,
      53,     0,     0,     0,    36,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    65,     0,     0,     0,
       0,   267,     0,     0,     0,   258,     0,     0,    21,    28,
      27,    25,     0,    20,    26,    24,     0,     0,     0,     0,
       0,     0,    51,     0,    42,    39,    41,    43,    51,    51,
      48,     0,    40,    51,    51,    33,    38,   105,     0,     0,
     279,   282,   255,   252,     0,   102,   103,   104,   251,   105,
     253,     0,   192,   180,   289,     0,     0,     0,     0,     0,
       0,   172,     0,   175,     0,     0,     0,   160,     0,    66,
     116,    88,   112,   125,   108,   109,   111,     0,   171,     0,
     283,   178,   177,   179,   276,   263,   259,   257,   260,   262,
     266,     0,    23,    22,   274,   274,   274,     0,   218,     0,
     283,     0,   215,   217,     0,    52,     0,     0,    54,     0,
       0,     0,     0,     0,     0,    71,   106,   105,    71,    68,
       0,    97,     0,    97,    97,    91,     0,   191,     0,     0,
       0,   288,   287,     0,   160,     0,   118,   117,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,   285,
       0,     0,    71,   199,     0,     0,   273,   272,   271,     0,
     249,   233,   228,   247,   247,   243,   242,   229,   236,   234,
     226,   227,   225,   230,   235,   224,   245,   245,   223,   247,
     247,   221,     0,   214,     0,    34,     0,    62,    58,    60,
      55,    58,    56,    58,    50,    58,    58,   107,     0,   202,
      71,     0,   204,   105,    98,     0,     0,     0,     0,   195,
       0,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,    67,    71,    90,
     122,     0,   124,   120,   121,   123,   119,   174,   176,   173,
     189,   190,   172,     0,     0,     0,     0,   197,     0,     0,
     219,     0,   210,     0,   238,   237,     0,     0,     0,   244,
     241,   240,   239,     0,   220,   216,   212,    58,    59,    49,
       0,    45,     0,    47,    44,    46,    70,   203,    69,    92,
       0,     0,    99,   100,     0,     0,     0,     0,   193,     0,
       0,   137,     0,     0,     0,   140,   141,   142,   144,   143,
     145,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,   188,
      73,     0,   181,   183,     0,    71,   200,   201,   275,     0,
       0,     0,     0,     0,   222,    61,    63,    57,     0,     0,
     104,     0,     0,     0,     0,   196,     0,   138,   168,   147,
       0,     0,   154,   148,   155,   149,   156,   150,   158,   152,
     157,   151,   159,   153,     0,     0,   135,     0,   129,   128,
     139,   170,   169,   165,     0,     0,     0,   162,     0,     0,
       0,     0,     0,    75,   113,   114,   115,   182,   198,     0,
     248,     0,     0,   246,     0,     0,   101,    96,     0,     0,
     194,     0,     0,   136,     0,   126,     0,   130,   163,   161,
       0,   166,   187,     0,   185,     0,     0,    76,     0,     0,
       0,    95,   105,    93,    94,     0,     0,   131,     0,   133,
     164,     0,    72,    74,     0,    84,     0,   231,   232,   132,
     134,   127,   186,     0,     0,    31,     0,    77,    78,    81,
      86,    87,     0,     0,    82,    83,    80,     0,     0,    79,
      85,     0,     0,     0,     0,     0,     0,     0,     0,   250
  };

  const short int
  parser::yypgoto_[] =
  {
    -480,  -480,  -480,  -480,    17,  -480,   804,    13,   826,   234,
     623,  -480,   606,   242,   479,    98,  -286,   324,   409,  -480,
    -480,  -233,  -207,  -480,  -480,  -480,  -480,   143,  -480,  -480,
     140,   465,  -127,  -125,   320,  -184,  -203,  -108,    47,  -480,
     452,    90,  -479,  -480,  -480,  -480,  -312,  -480,  -480,  -480,
    -304,  -480,  -480,  -480,  -480,   720,  -480,   321,   722,  -480,
     366,   726,  -480,  -480,  -480,  -480,  -480,  -480,  -480,   455,
    -480,  -480,   445,  -214,  -480,  -480,   328,  -480,   630,  -480,
    -480,  -480,   177,  -480,  -480,   168,    36,     6,  -133,  -480,
     486,    -3,  -480,  -115,    -9,    -1
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    16,    17,    18,    61,    20,    21,   187,    23,    41,
     103,   104,   105,   226,   144,   311,   392,   307,   308,   117,
     188,   238,   319,   503,   537,   555,   567,   568,   576,   565,
     570,   189,   165,   166,   167,   325,   404,   239,   399,   190,
     352,   335,   526,   336,   425,   426,   192,   193,   339,   250,
     337,   194,   195,   196,   364,    24,   247,   330,    25,   272,
     273,    26,    27,    56,    28,   140,   221,   304,   217,   218,
     384,   301,   379,   374,   372,    29,   111,    30,   125,    31,
      32,    33,   276,    34,    35,   169,    45,   197,   198,   199,
     268,   200,   201,   202,   203,   571
  };

  const short int
  parser::yytable_[] =
  {
      59,    47,    47,   204,   168,   219,   222,   321,    60,    63,
      46,    46,    76,    22,   396,   427,   428,    19,   427,   428,
      22,   411,   390,    22,    52,   264,   427,   428,    42,   348,
     413,   322,    10,    76,    76,    47,    89,   434,    47,  -284,
     261,    42,    42,   546,    46,   504,    42,    46,   427,   428,
      10,    75,   234,   227,    47,    36,   552,  -282,    11,   327,
     328,   251,   252,    46,   266,   367,   262,   340,   487,    40,
     375,    47,    86,    87,    43,    43,    11,    58,   274,   488,
      46,   279,    57,    37,    42,   381,   382,   -18,    49,   505,
     -18,   306,    22,    22,    22,    22,    22,    22,   467,   171,
     431,   390,   524,    47,   459,    42,   161,   318,    76,    47,
     170,   486,    46,   397,   490,   160,    47,    47,    46,    47,
      47,    76,   468,   491,   492,    46,    46,    22,    46,    46,
      11,   172,   445,   429,   161,   220,   220,   359,   228,   506,
      22,   440,    59,   216,   498,    22,   219,   163,   264,   341,
     231,   280,   545,   357,   161,    47,    47,    42,   517,   523,
     210,   127,    38,   236,    46,    46,   161,    50,   132,   385,
     358,   302,    44,    48,    85,   236,    77,    22,    39,   365,
      78,   240,   432,    51,   472,   265,   473,   266,   269,    42,
     109,   388,   254,   388,   110,   263,   388,   458,   264,   402,
     267,   403,   463,   464,   235,  -281,    88,   191,   220,   108,
     541,   118,   264,   398,   543,   544,   245,    42,   115,   264,
       3,    10,   388,   309,   116,   114,   312,   312,    10,   312,
     312,   161,   553,   274,   161,   265,   441,   266,   508,   119,
     317,   303,   126,   236,    90,    62,     8,    11,    74,   265,
     267,   266,   120,   318,    11,   366,   265,    10,   266,    47,
     269,   389,   121,   391,   267,   249,   393,   220,    46,   255,
     256,   267,   257,   402,   157,   403,   220,   574,   402,   402,
     403,   403,   122,    11,   320,   124,   215,   205,   206,   388,
     208,   209,   394,   461,    91,   407,   388,   575,   338,   220,
     264,    92,    14,   309,   407,    93,    94,    95,    96,    97,
     548,   548,   474,    98,   475,    99,   123,    22,    47,   136,
     161,   386,   501,    47,   137,    42,   237,    46,   313,   236,
     315,   316,    46,   469,   470,   471,   329,   265,   138,   266,
      42,   155,   100,   342,   101,   102,   139,   254,   344,   345,
     346,   191,   267,    42,   350,   353,   354,   356,    43,   395,
     448,   363,    47,   220,    59,   408,   455,    59,    59,    59,
     142,    46,   450,    43,   520,   451,   452,   453,   112,   113,
     549,   560,   476,   106,   477,   143,    43,   309,   164,   457,
     229,   230,   277,   278,   264,   232,   233,    47,    47,   148,
     149,   497,    47,    47,   158,   151,    46,    46,   153,   107,
     264,    46,    46,   478,    22,   479,   401,   154,   240,   437,
      79,    80,    81,   480,   412,   481,   159,   500,   430,   162,
     433,   265,   264,   266,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   174,   267,   265,   211,   266,
     332,   572,   224,   214,   444,   264,   264,   482,   447,   483,
     225,   173,   267,   529,   174,    42,    58,   223,   584,   265,
     246,   266,    22,   589,   253,   258,   515,   259,   175,   264,
     260,   494,   270,   275,   267,   271,   305,   176,   177,   438,
     310,   314,   265,   265,   266,   266,   323,   465,   254,   329,
     324,   326,    59,    59,   333,   318,   331,   267,   267,   368,
     539,   540,   484,   178,   371,   489,   265,   369,   266,   493,
     373,   495,   496,   376,   551,   499,   530,   377,   334,   332,
     378,   267,   180,   383,   181,   254,   343,   435,   400,   161,
     173,   264,   405,   174,    42,    58,   409,   182,   236,   241,
     264,   183,   406,   410,    11,    59,   242,   175,   442,    10,
     521,   449,   454,   184,   264,   243,   176,   177,    59,   436,
     185,   186,   145,   146,   147,   525,   462,   150,   265,   152,
     266,   485,   264,   333,   528,   502,   509,   265,   531,   266,
     244,   173,   178,   267,   174,    42,    58,   510,   343,   511,
     512,   265,   267,   266,    82,    83,    84,   334,   175,   513,
     516,   180,   525,   181,   547,   514,   267,   176,   177,   265,
     550,   266,   -29,   -29,   -29,   525,   182,   518,   522,   519,
     183,   281,   527,   428,   267,   559,   533,   534,   561,   535,
      64,    65,   184,   178,   282,   283,    66,   536,   284,   185,
     186,   542,   538,   569,   554,   556,   285,   286,   179,   287,
     548,   557,   180,   569,   181,   288,   558,   562,   173,    67,
     563,   174,    42,    58,    68,   564,   289,   182,   566,   173,
     573,   183,   174,    42,    58,   175,   360,   290,   577,   291,
      69,   578,   581,   184,   176,   177,   175,   582,   292,   583,
     185,   186,   585,   586,   587,   176,   177,   588,    70,   293,
     156,   248,   361,   141,   456,   387,   579,   580,   294,   355,
     178,   460,    71,   264,    72,    73,   347,    53,   295,    54,
     466,   178,   446,    55,   370,   179,   296,   297,   298,   180,
     173,   181,   380,   174,    42,    58,   179,   299,   300,   349,
     180,   207,   362,     0,   182,     0,     0,   175,   183,     0,
     265,    11,   266,     0,   264,   182,   176,   177,   173,   183,
     184,   174,    42,    58,     0,   267,     0,   185,   186,     0,
     439,   184,     0,     0,     0,   175,     1,     0,   185,   186,
       0,     0,   178,     0,   176,   177,     0,     0,   351,   264,
       2,   265,     0,   266,     0,     0,     0,   179,     3,     4,
       5,   180,     0,   181,     6,     0,   267,     0,     0,     0,
     178,   443,     7,     0,     0,     0,   182,   -30,   -30,   -30,
     183,     0,     0,     0,     8,   179,   265,     0,   266,   180,
       9,   181,   184,   264,     0,    10,     0,    42,     0,   185,
     186,   267,     0,   234,   182,     0,   507,     0,   183,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     184,    11,     0,    12,    13,     0,     0,   185,   186,     0,
     265,     0,   266,   128,   130,   131,   133,   134,   135,     0,
      14,     0,     0,    15,     0,   267,     0,     0,     0,     0,
     532,     0,     0,     0,   264,   129,   129,   129,   129,   129,
     129,    91,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
      98,   212,    99,     0,     0,     0,   213,     0,     0,     0,
       0,   265,     0,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   129,     0,     0,   267,     0,   129,   100,
       0,   101,   102
  };

  const short int
  parser::yycheck_[] =
  {
       9,     4,     5,   118,   112,   138,   139,    40,     9,    12,
       4,     5,    15,     0,   318,     6,     7,     0,     6,     7,
       7,   333,   308,    10,     7,    53,     6,     7,    23,   262,
     334,   238,    86,    36,    37,    38,    39,   341,    41,    86,
      40,    23,    23,   522,    38,    47,    23,    41,     6,     7,
      86,    15,    29,    25,    57,   122,   535,   104,   112,   243,
     244,   176,   177,    57,    92,   272,    66,    57,     8,    31,
     284,    74,    36,    37,    56,    56,   112,    24,   211,    19,
      74,    40,    79,    43,    23,   299,   300,   110,    43,    91,
     113,    86,    79,    80,    81,    82,    83,    84,   410,   135,
      57,   387,     7,   106,    86,    23,   109,   140,   111,   112,
     113,   423,   106,   320,   426,   109,   119,   120,   112,   122,
     123,   124,   110,   427,   428,   119,   120,   114,   122,   123,
     112,   114,   365,   124,   137,   138,   139,   270,   110,   141,
     127,   348,   151,   137,   124,   132,   279,   111,    53,   139,
     151,   110,     7,    92,   157,   158,   159,    23,   462,   471,
     124,    26,   122,   157,   158,   159,   169,   122,    26,   302,
     109,    40,     4,     5,    92,   169,     0,   164,   138,    66,
     113,   164,   139,   138,    26,    90,    28,    92,   191,    23,
     104,    40,   179,    40,    70,    29,    40,   400,    53,   326,
     105,   326,   405,   406,   157,   104,    38,   117,   211,    41,
     514,    11,    53,   321,   518,   519,   169,    23,   110,    53,
      49,    86,    40,   226,    52,    57,   229,   230,    86,   232,
     233,   234,   536,   366,   237,    90,   351,    92,   445,   119,
     234,   110,    74,   237,    32,    11,    75,   112,    14,    90,
     105,    92,    66,   140,   112,   142,    90,    86,    92,   262,
     263,   110,   123,   110,   105,   175,   110,   270,   262,   179,
     180,   105,   182,   400,   106,   400,   279,    30,   405,   406,
     405,   406,    66,   112,   237,    66,    92,   119,   120,    40,
     122,   123,   110,   401,    82,    40,    40,    50,   139,   302,
      53,    89,   131,   306,    40,    93,    94,    95,    96,    97,
      40,    40,    26,   101,    28,   103,   119,   304,   321,   104,
     323,   304,   437,   326,   104,    23,   158,   321,   230,   323,
     232,   233,   326,    15,    16,    17,   246,    90,    86,    92,
      23,    58,   130,   253,   132,   133,    86,   334,   258,   259,
     260,   261,   105,    23,   264,   265,   266,   267,    56,   110,
     369,   271,   365,   366,   373,   110,   110,   376,   377,   378,
      86,   365,   373,    56,   110,   376,   377,   378,    50,    51,
     110,   110,    26,    66,    28,    86,    56,   390,    86,   392,
     148,   149,   215,   216,    53,   153,   154,   400,   401,    86,
      86,    60,   405,   406,    66,    86,   400,   401,    86,    92,
      53,   405,   406,    26,   401,    28,    86,    86,   401,    40,
       3,     4,     5,    26,   334,    28,    66,   436,   338,    62,
     340,    90,    53,    92,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    22,   105,    90,   114,    92,
       8,   566,    59,    92,   364,    53,    53,    26,   368,    28,
      25,    19,   105,    60,    22,    23,    24,    29,   583,    90,
      86,    92,   459,   588,    86,    86,   459,    86,    36,    53,
      86,   124,   104,   111,   105,    86,    58,    45,    46,   110,
     110,   110,    90,    90,    92,    92,   110,   407,   485,   409,
     102,    80,   511,   512,    62,   140,    40,   105,   105,    11,
     511,   512,   422,    71,    61,   425,    90,   116,    92,   429,
      86,   431,   432,    86,   533,   435,   124,    86,    86,     8,
      86,   105,    90,     8,    92,   522,   110,    40,    80,   542,
      19,    53,    80,    22,    23,    24,    86,   105,   542,    67,
      53,   109,    80,    62,   112,   564,    74,    36,   110,    86,
     470,    11,    19,   121,    53,    83,    45,    46,   577,    40,
     128,   129,    93,    94,    95,   485,    99,    98,    90,   100,
      92,    86,    53,    62,   494,    68,    42,    90,   498,    92,
     108,    19,    71,   105,    22,    23,    24,   110,   110,    40,
      40,    90,   105,    92,     3,     4,     5,    86,    36,   110,
     110,    90,   522,    92,   524,    99,   105,    45,    46,    90,
     530,    92,     3,     4,     5,   535,   105,    99,    86,    99,
     109,    20,    19,     7,   105,   545,    40,   110,   548,    35,
      38,    39,   121,    71,    33,    34,    44,    69,    37,   128,
     129,   110,    40,   563,   100,    73,    45,    46,    86,    48,
      40,   110,    90,   573,    92,    54,   110,   110,    19,    67,
      35,    22,    23,    24,    72,    84,    65,   105,    11,    19,
      40,   109,    22,    23,    24,    36,    26,    76,    40,    78,
      88,    40,    64,   121,    45,    46,    36,   118,    87,    35,
     128,   129,    40,    85,   118,    45,    46,    35,   106,    98,
     104,   139,    52,    90,   390,   306,   573,   577,   107,   267,
      71,   401,   120,    53,   122,   123,   261,     7,   117,     7,
     409,    71,   366,     7,   279,    86,   125,   126,   127,    90,
      19,    92,   297,    22,    23,    24,    86,   136,   137,   263,
      90,   121,    92,    -1,   105,    -1,    -1,    36,   109,    -1,
      90,   112,    92,    -1,    53,   105,    45,    46,    19,   109,
     121,    22,    23,    24,    -1,   105,    -1,   128,   129,    -1,
     110,   121,    -1,    -1,    -1,    36,    27,    -1,   128,   129,
      -1,    -1,    71,    -1,    45,    46,    -1,    -1,    77,    53,
      41,    90,    -1,    92,    -1,    -1,    -1,    86,    49,    50,
      51,    90,    -1,    92,    55,    -1,   105,    -1,    -1,    -1,
      71,   110,    63,    -1,    -1,    -1,   105,     3,     4,     5,
     109,    -1,    -1,    -1,    75,    86,    90,    -1,    92,    90,
      81,    92,   121,    53,    -1,    86,    -1,    23,    -1,   128,
     129,   105,    -1,    29,   105,    -1,   110,    -1,   109,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     121,   112,    -1,   114,   115,    -1,    -1,   128,   129,    -1,
      90,    -1,    92,    79,    80,    81,    82,    83,    84,    -1,
     131,    -1,    -1,   134,    -1,   105,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    53,    79,    80,    81,    82,    83,
      84,    82,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    -1,    -1,    -1,
     101,   127,   103,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,   105,    -1,   132,   130,
      -1,   132,   133
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    27,    41,    49,    50,    51,    55,    63,    75,    81,
      86,   112,   114,   115,   131,   134,   144,   145,   146,   147,
     148,   149,   150,   151,   198,   201,   204,   205,   207,   218,
     220,   222,   223,   224,   226,   227,   122,    43,   122,   138,
      31,   152,    23,    56,   228,   229,   230,   234,   228,    43,
     122,   138,   147,   198,   201,   204,   206,    79,    24,   237,
     238,   147,   152,   234,    38,    39,    44,    67,    72,    88,
     106,   120,   122,   123,   152,   229,   234,     0,   113,     3,
       4,     5,     3,     4,     5,    92,   229,   229,   228,   234,
      32,    82,    89,    93,    94,    95,    96,    97,   101,   103,
     130,   132,   133,   153,   154,   155,    66,    92,   228,   104,
      70,   219,   219,   219,   228,   110,    52,   162,    11,   119,
      66,   123,    66,   119,    66,   221,   228,    26,   149,   151,
     149,   149,    26,   149,   149,   149,   104,   104,    86,    86,
     208,   153,    86,    86,   157,   157,   157,   157,    86,    86,
     157,    86,   157,    86,    86,    58,   155,   228,    66,    66,
     230,   234,    62,   229,    86,   175,   176,   177,   180,   228,
     234,   135,   147,    19,    22,    36,    45,    46,    71,    86,
      90,    92,   105,   109,   121,   128,   129,   150,   163,   174,
     182,   184,   189,   190,   194,   195,   196,   230,   231,   232,
     234,   235,   236,   237,   236,   228,   228,   221,   228,   228,
     229,   114,   149,   149,    92,    92,   230,   211,   212,   231,
     234,   209,   231,    29,    59,    25,   156,    25,   110,   156,
     156,   238,   156,   156,    29,   181,   230,   228,   164,   180,
     147,    67,    74,    83,   108,   181,    86,   199,   139,   184,
     192,   236,   236,    86,   150,   184,   184,   184,    86,    86,
      86,    40,    66,    29,    53,    90,    92,   105,   233,   234,
     104,    86,   202,   203,   231,   111,   225,   225,   225,    40,
     110,    20,    33,    34,    37,    45,    46,    48,    54,    65,
      76,    78,    87,    98,   107,   117,   125,   126,   127,   136,
     137,   214,    40,   110,   210,    58,    86,   160,   161,   234,
     110,   158,   234,   158,   110,   158,   158,   230,   140,   165,
     181,    40,   165,   110,   102,   178,    80,   178,   178,   184,
     200,    40,     8,    62,    86,   184,   186,   193,   139,   191,
      57,   139,   184,   110,   184,   184,   184,   174,   164,   233,
     184,    77,   183,   184,   184,   183,   184,    92,   109,   231,
      26,    52,    92,   184,   197,    66,   142,   165,    11,   116,
     212,    61,   217,    86,   216,   216,    86,    86,    86,   215,
     215,   216,   216,     8,   213,   231,   147,   161,    40,   110,
     159,   110,   159,   110,   110,   110,   193,   165,   180,   181,
      80,    86,   175,   176,   179,    80,    80,    40,   110,    86,
      62,   189,   184,   193,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,   187,   188,     6,     7,   124,
     184,    57,   139,   184,   193,    40,    40,    40,   110,   110,
     165,   236,   110,   110,   184,   164,   203,   184,   237,    11,
     238,   238,   238,   238,    19,   110,   160,   234,   179,    86,
     177,   180,    99,   179,   179,   184,   200,   189,   110,    15,
      16,    17,    26,    28,    26,    28,    26,    28,    26,    28,
      26,    28,    26,    28,   184,    86,   189,     8,    19,   184,
     189,   193,   193,   184,   124,   184,   184,    60,   124,   184,
     237,   236,    68,   166,    47,    91,   141,   110,   165,    42,
     110,    40,    40,   110,    99,   147,   110,   193,    99,    99,
     110,   184,    86,   189,     7,   184,   185,    19,   184,    60,
     124,   184,   110,    40,   110,    35,    69,   167,    40,   238,
     238,   193,   110,   193,   193,     7,   185,   184,    40,   110,
     184,   237,   185,   193,   100,   168,    73,   110,   110,   184,
     110,   184,   110,    35,    84,   172,    11,   169,   170,   184,
     173,   238,   236,    40,    30,    50,   171,    40,    40,   170,
     173,    64,   118,    35,   236,    40,    85,   118,    35,   236
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   143,   144,   145,   145,   145,   145,   145,   145,   145,
     146,   146,   146,   146,   146,   146,   146,   146,   147,   147,
     148,   148,   148,   148,   148,   148,   148,   148,   149,   149,
     150,   151,   152,   152,   152,   153,   153,   154,   154,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   156,   156,   157,   157,   157,   158,   158,   159,   159,
     160,   160,   161,   161,   162,   162,   163,   163,   164,   164,
     165,   165,   166,   166,   167,   167,   168,   168,   169,   169,
     170,   171,   171,   171,   172,   172,   172,   173,   174,   174,
     174,   175,   176,   177,   177,   177,   177,   178,   178,   179,
     179,   179,   180,   180,   180,   181,   181,   181,   182,   182,
     182,   182,   182,   183,   183,   183,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   185,   185,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     187,   187,   187,   187,   187,   187,   187,   187,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     189,   190,   190,   191,   191,   192,   192,   193,   193,   193,
     193,   194,   194,   194,   194,   194,   194,   195,   195,   195,
     195,   196,   196,   196,   196,   196,   196,   196,   196,   197,
     197,   198,   198,   199,   199,   200,   200,   201,   201,   202,
     202,   203,   204,   204,   204,   205,   206,   206,   206,   206,
     207,   207,   207,   208,   208,   209,   209,   210,   211,   211,
     212,   213,   213,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   215,   215,   216,   216,   217,
     217,   218,   218,   218,   219,   219,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   221,   221,   222,   223,
     223,   224,   224,   224,   225,   225,   226,   227,   228,   228,
     228,   229,   230,   231,   232,   233,   234,   235,   235,   236,
     237,   238
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     4,     4,     3,     3,     3,     3,     1,     1,
       3,    11,     0,     3,     5,     0,     1,     1,     2,     2,
       2,     2,     2,     2,     5,     5,     5,     5,     2,     5,
       4,     0,     1,     0,     2,     3,     1,     3,     0,     1,
       1,     3,     1,     3,     0,     1,     1,     3,     1,     3,
       2,     0,     3,     0,     2,     0,     0,     3,     1,     3,
       2,     0,     1,     1,     0,     4,     2,     1,     1,     2,
       3,     2,     4,     7,     7,     7,     6,     0,     1,     1,
       1,     3,     1,     1,     1,     0,     1,     2,     1,     1,
       3,     1,     1,     3,     3,     3,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     1,     1,     3,     3,     3,
       4,     5,     6,     5,     6,     3,     4,     2,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     6,     5,     4,     5,     4,     5,     1,     3,     3,
       3,     1,     1,     3,     3,     1,     3,     1,     1,     1,
       1,     4,     5,     4,     4,     6,     8,     6,     4,     1,
       1,     5,     4,     3,     5,     1,     3,     6,     8,     1,
       3,     3,     6,     7,     6,     2,     1,     1,     1,     1,
       7,     3,     7,     0,     3,     1,     3,     0,     1,     3,
       3,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     6,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     1,     1,     2,     0,     3,     0,     3,     0,
      17,     4,     4,     4,     0,     2,     2,     4,     3,     4,
       4,     2,     4,     4,     2,     2,     2,     0,     2,     2,
       2,     6,     6,     6,     0,     3,     4,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END", "error", "$undefined", "UNION", "EXCEPT", "INTERSECT", "OR",
  "AND", "NOT", "CMP_LE", "CMP_LT", "CMP_EQ", "CMP_GT", "CMP_GE", "CMP_NE",
  "LIKE", "BETWEEN", "IN", "IS", "NULLX", "BOOL", "UMINUS", "STRING",
  "IDENT", "NUMERIC", "QB_NAME_IDENT", "ALL", "ANALYZE", "ANY", "AS",
  "ASC", "BEGIN_HINT", "BEGIN_OUTLINE_DATA", "BIGINT", "BINARY", "BY",
  "CASE", "CHAR", "COLUMN", "COLUMNS", "\",\"", "CREATE", "CSV",
  "DATABASE", "DATABASES", "DATE", "DATETIME", "DAY", "DECIMAL", "DELETE",
  "DESC", "DESCRIBE", "DISTINCT", "\"/\"", "DOUBLE", "DROP", "DUAL",
  "ELSE", "END_HINT", "END_OUTLINE_DATA", "END_SYM", "ENGINE", "EXISTS",
  "EXPLAIN", "FIELD", "FLOAT", "FROM", "FULL", "GROUP", "HAVING", "IF",
  "IFNULL", "INDEX", "INFILE", "INNER", "INSERT", "INT", "INTERVAL",
  "INTEGER", "INTO", "JOIN", "KILL", "LEADING", "LEFT", "LIMIT", "LINE",
  "\"(\"", "MEDIUMINT", "MEMORY", "MERGE", "\"-\"", "MONTH", "\"*\"",
  "NO_REWRITE", "NO_UNNEST", "NO_MERGE", "NO_USE_HASH", "NO_USE_NL",
  "NUMERIC_SYM", "ON", "ORDER", "ORDERED", "OUTER", "PARALLEL", "\".\"",
  "\"+\"", "PROCESSLIST", "REAL", "RIGHT", "ROWID", "\")\"", "SAMPLE",
  "SELECT", "\";\"", "SET", "SHOW", "SIZE", "SMALLINT", "SPLIT", "STATIS",
  "STATUS", "SUBSTR", "TABLE", "TABLES", "THEN", "TIME", "TIMESTAMP_SYM",
  "TINYINT", "TO_CHAR", "TO_NUMBER", "UNNEST", "UPDATE", "USE_HASH",
  "USE_NL", "USING", "VALUES", "VARBINARY", "VARCHAR", "VIEW", "WHEN",
  "WHERE", "YEAR", "','", "$accept", "sql_stmt", "stmt", "cmd_stmt",
  "select_stmt", "set_select", "sub_set_select", "select_with_parens",
  "simple_select", "opt_hint", "opt_hint_list", "hint_list", "single_hint",
  "opt_qb_name", "opt_qb_name_single", "hint_table_list", "opt_split",
  "leading_hint_table", "leading_hint_table_list", "opt_distinct",
  "select_expr_list", "from_list", "opt_where", "opt_groupby",
  "opt_having", "opt_order_by", "order_by_list", "order_by",
  "opt_asc_desc", "opt_select_limit", "limit_expr", "projection",
  "basic_table_factor", "view_table_factor", "joined_table_factor",
  "opt_outer", "sub_table_factor", "table_factor", "opt_alias",
  "simple_expr", "seconds_expr", "arith_expr", "arith_expr_list",
  "cmp_expr", "cmp_type", "sq_cmp_type", "query_ref_expr",
  "case_when_expr", "when_then_list1", "when_then_list2", "logical_expr",
  "column_ref", "expr_const", "func_expr", "distinct_or_all",
  "insert_stmt", "insert_value_list", "insert_value", "update_stmt",
  "update_asgn_list", "update_asgn_factor", "delete_stmt", "explain_stmt",
  "explainable_stmt", "create_stmt", "opt_view_column_define",
  "view_column_define", "beg_view_define", "table_element_list",
  "column_definition", "opt_not_null", "data_type", "opt_time_precision",
  "opt_char_length", "opt_engine_def", "drop_stmt", "opt_if_exists",
  "show_stmt", "op_from_database", "use_stmt", "desc_stmt", "analyze_stmt",
  "opt_sample_size", "set_var_stmt", "kill_stmt", "relation_factor",
  "database_name", "relation_name", "column_name", "function_name",
  "column_label", "ident", "datetime", "string", "number", "int_value", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   362,   362,   370,   371,   372,   373,   374,   375,   376,
     385,   386,   387,   388,   389,   390,   391,   392,   396,   400,
     407,   412,   417,   422,   427,   432,   437,   442,   450,   454,
     461,   468,   489,   490,   496,   506,   509,   516,   523,   534,
     539,   544,   549,   554,   560,   568,   576,   584,   592,   599,
     609,   618,   619,   626,   627,   631,   638,   643,   651,   652,
     656,   662,   671,   676,   684,   685,   689,   694,   702,   707,
     715,   716,   720,   721,   725,   726,   730,   731,   735,   740,
     748,   755,   756,   757,   761,   762,   769,   779,   786,   792,
     798,   807,   815,   824,   828,   832,   836,   844,   847,   854,
     858,   862,   869,   873,   877,   884,   885,   889,   901,   905,
     909,   913,   917,   924,   934,   944,   957,   961,   966,   971,
     976,   981,   986,   991,   996,  1001,  1009,  1015,  1024,  1029,
    1034,  1039,  1044,  1049,  1060,  1071,  1076,  1081,  1085,  1090,
    1098,  1102,  1106,  1110,  1114,  1118,  1122,  1126,  1133,  1137,
    1141,  1145,  1149,  1153,  1157,  1161,  1165,  1169,  1173,  1177,
    1184,  1193,  1200,  1209,  1215,  1224,  1230,  1239,  1243,  1247,
    1252,  1260,  1267,  1273,  1281,  1288,  1296,  1307,  1316,  1329,
    1338,  1350,  1359,  1367,  1374,  1378,  1386,  1398,  1402,  1409,
    1413,  1425,  1434,  1445,  1450,  1458,  1463,  1476,  1487,  1501,
    1507,  1516,  1531,  1542,  1553,  1571,  1580,  1581,  1582,  1583,
    1592,  1601,  1608,  1623,  1626,  1633,  1638,  1647,  1653,  1658,
    1666,  1676,  1679,  1686,  1688,  1690,  1692,  1694,  1696,  1698,
    1700,  1702,  1704,  1706,  1708,  1710,  1712,  1714,  1716,  1718,
    1720,  1722,  1724,  1726,  1728,  1733,  1734,  1745,  1746,  1758,
    1765,  1781,  1789,  1797,  1809,  1810,  1819,  1826,  1833,  1840,
    1847,  1851,  1855,  1863,  1871,  1877,  1886,  1891,  1897,  1912,
    1919,  1933,  1942,  1951,  1963,  1964,  1971,  1982,  1996,  2004,
    2012,  2022,  2026,  2030,  2034,  2038,  2042,  2046,  2047,  2051,
    2055,  2059
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 3 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1167
} // CatDB
#line 4585 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:1167
#line 2063 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
