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
      case 217: // relation_factor
        value.move< BasicTableStmt_s > (that.value);
        break;

      case 202: // column_definition
        value.move< ColumnDefineStmt_s > (that.value);
        break;

      case 203: // data_type
        value.move< DataType > (that.value);
        break;

      case 169: // projection
      case 176: // simple_expr
      case 177: // arith_expr
      case 179: // cmp_expr
      case 182: // query_ref_expr
      case 183: // case_when_expr
      case 186: // logical_expr
      case 187: // column_ref
      case 188: // expr_const
      case 189: // func_expr
      case 196: // update_asgn_factor
        value.move< ExprStmt_s > (that.value);
        break;

      case 147: // opt_hint
        value.move< Hint > (that.value);
        break;

      case 150: // single_hint
        value.move< HintStmt_s > (that.value);
        break;

      case 155: // leading_hint_table
        value.move< LeadingTable_s > (that.value);
        break;

      case 167: // opt_select_limit
        value.move< LimitStmt_s > (that.value);
        break;

      case 180: // cmp_type
      case 181: // sq_cmp_type
        value.move< OperationType > (that.value);
        break;

      case 165: // order_by
        value.move< OrderStmt_s > (that.value);
        break;

      case 139: // sql_stmt
      case 140: // stmt
      case 141: // cmd_stmt
      case 142: // select_stmt
      case 143: // set_select
      case 144: // sub_set_select
      case 145: // select_with_parens
      case 146: // simple_select
      case 191: // insert_stmt
      case 194: // update_stmt
      case 197: // delete_stmt
      case 198: // explain_stmt
      case 199: // explainable_stmt
      case 200: // create_stmt
      case 207: // drop_stmt
      case 209: // show_stmt
      case 211: // use_stmt
      case 212: // desc_stmt
      case 213: // analyze_stmt
      case 215: // set_var_stmt
      case 216: // kill_stmt
        value.move< Stmt_s > (that.value);
        break;

      case 170: // basic_table_factor
      case 171: // view_table_factor
      case 172: // joined_table_factor
      case 173: // sub_table_factor
      case 174: // table_factor
        value.move< TableStmt_s > (that.value);
        break;

      case 201: // table_element_list
        value.move< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 158: // select_expr_list
      case 160: // opt_where
      case 161: // opt_groupby
      case 162: // opt_having
      case 178: // arith_expr_list
      case 184: // when_then_list1
      case 185: // when_then_list2
      case 193: // insert_value
      case 195: // update_asgn_list
        value.move< Vector<ExprStmt_s> > (that.value);
        break;

      case 148: // opt_hint_list
      case 149: // hint_list
        value.move< Vector<HintStmt_s> > (that.value);
        break;

      case 156: // leading_hint_table_list
        value.move< Vector<LeadingTable_s> > (that.value);
        break;

      case 163: // opt_order_by
      case 164: // order_by_list
        value.move< Vector<OrderStmt_s> > (that.value);
        break;

      case 153: // hint_table_list
      case 206: // opt_engine_def
        value.move< Vector<String> > (that.value);
        break;

      case 159: // from_list
        value.move< Vector<TableStmt_s> > (that.value);
        break;

      case 192: // insert_value_list
        value.move< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 154: // opt_split
      case 157: // opt_distinct
      case 166: // opt_asc_desc
      case 190: // distinct_or_all
      case 208: // opt_if_exists
        value.move< bool > (that.value);
        break;

      case 214: // opt_sample_size
        value.move< double > (that.value);
        break;

      case 168: // limit_expr
      case 204: // opt_time_precision
      case 205: // opt_char_length
      case 227: // int_value
        value.move< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 151: // opt_qb_name
      case 152: // opt_qb_name_single
      case 175: // opt_alias
      case 210: // op_from_database
      case 218: // database_name
      case 219: // relation_name
      case 220: // column_name
      case 221: // function_name
      case 222: // column_label
      case 223: // ident
      case 224: // datetime
      case 225: // string
      case 226: // number
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
      case 217: // relation_factor
        value.copy< BasicTableStmt_s > (that.value);
        break;

      case 202: // column_definition
        value.copy< ColumnDefineStmt_s > (that.value);
        break;

      case 203: // data_type
        value.copy< DataType > (that.value);
        break;

      case 169: // projection
      case 176: // simple_expr
      case 177: // arith_expr
      case 179: // cmp_expr
      case 182: // query_ref_expr
      case 183: // case_when_expr
      case 186: // logical_expr
      case 187: // column_ref
      case 188: // expr_const
      case 189: // func_expr
      case 196: // update_asgn_factor
        value.copy< ExprStmt_s > (that.value);
        break;

      case 147: // opt_hint
        value.copy< Hint > (that.value);
        break;

      case 150: // single_hint
        value.copy< HintStmt_s > (that.value);
        break;

      case 155: // leading_hint_table
        value.copy< LeadingTable_s > (that.value);
        break;

      case 167: // opt_select_limit
        value.copy< LimitStmt_s > (that.value);
        break;

      case 180: // cmp_type
      case 181: // sq_cmp_type
        value.copy< OperationType > (that.value);
        break;

      case 165: // order_by
        value.copy< OrderStmt_s > (that.value);
        break;

      case 139: // sql_stmt
      case 140: // stmt
      case 141: // cmd_stmt
      case 142: // select_stmt
      case 143: // set_select
      case 144: // sub_set_select
      case 145: // select_with_parens
      case 146: // simple_select
      case 191: // insert_stmt
      case 194: // update_stmt
      case 197: // delete_stmt
      case 198: // explain_stmt
      case 199: // explainable_stmt
      case 200: // create_stmt
      case 207: // drop_stmt
      case 209: // show_stmt
      case 211: // use_stmt
      case 212: // desc_stmt
      case 213: // analyze_stmt
      case 215: // set_var_stmt
      case 216: // kill_stmt
        value.copy< Stmt_s > (that.value);
        break;

      case 170: // basic_table_factor
      case 171: // view_table_factor
      case 172: // joined_table_factor
      case 173: // sub_table_factor
      case 174: // table_factor
        value.copy< TableStmt_s > (that.value);
        break;

      case 201: // table_element_list
        value.copy< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 158: // select_expr_list
      case 160: // opt_where
      case 161: // opt_groupby
      case 162: // opt_having
      case 178: // arith_expr_list
      case 184: // when_then_list1
      case 185: // when_then_list2
      case 193: // insert_value
      case 195: // update_asgn_list
        value.copy< Vector<ExprStmt_s> > (that.value);
        break;

      case 148: // opt_hint_list
      case 149: // hint_list
        value.copy< Vector<HintStmt_s> > (that.value);
        break;

      case 156: // leading_hint_table_list
        value.copy< Vector<LeadingTable_s> > (that.value);
        break;

      case 163: // opt_order_by
      case 164: // order_by_list
        value.copy< Vector<OrderStmt_s> > (that.value);
        break;

      case 153: // hint_table_list
      case 206: // opt_engine_def
        value.copy< Vector<String> > (that.value);
        break;

      case 159: // from_list
        value.copy< Vector<TableStmt_s> > (that.value);
        break;

      case 192: // insert_value_list
        value.copy< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 154: // opt_split
      case 157: // opt_distinct
      case 166: // opt_asc_desc
      case 190: // distinct_or_all
      case 208: // opt_if_exists
        value.copy< bool > (that.value);
        break;

      case 214: // opt_sample_size
        value.copy< double > (that.value);
        break;

      case 168: // limit_expr
      case 204: // opt_time_precision
      case 205: // opt_char_length
      case 227: // int_value
        value.copy< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 151: // opt_qb_name
      case 152: // opt_qb_name_single
      case 175: // opt_alias
      case 210: // op_from_database
      case 218: // database_name
      case 219: // relation_name
      case 220: // column_name
      case 221: // function_name
      case 222: // column_label
      case 223: // ident
      case 224: // datetime
      case 225: // string
      case 226: // number
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
      case 217: // relation_factor
        yylhs.value.build< BasicTableStmt_s > ();
        break;

      case 202: // column_definition
        yylhs.value.build< ColumnDefineStmt_s > ();
        break;

      case 203: // data_type
        yylhs.value.build< DataType > ();
        break;

      case 169: // projection
      case 176: // simple_expr
      case 177: // arith_expr
      case 179: // cmp_expr
      case 182: // query_ref_expr
      case 183: // case_when_expr
      case 186: // logical_expr
      case 187: // column_ref
      case 188: // expr_const
      case 189: // func_expr
      case 196: // update_asgn_factor
        yylhs.value.build< ExprStmt_s > ();
        break;

      case 147: // opt_hint
        yylhs.value.build< Hint > ();
        break;

      case 150: // single_hint
        yylhs.value.build< HintStmt_s > ();
        break;

      case 155: // leading_hint_table
        yylhs.value.build< LeadingTable_s > ();
        break;

      case 167: // opt_select_limit
        yylhs.value.build< LimitStmt_s > ();
        break;

      case 180: // cmp_type
      case 181: // sq_cmp_type
        yylhs.value.build< OperationType > ();
        break;

      case 165: // order_by
        yylhs.value.build< OrderStmt_s > ();
        break;

      case 139: // sql_stmt
      case 140: // stmt
      case 141: // cmd_stmt
      case 142: // select_stmt
      case 143: // set_select
      case 144: // sub_set_select
      case 145: // select_with_parens
      case 146: // simple_select
      case 191: // insert_stmt
      case 194: // update_stmt
      case 197: // delete_stmt
      case 198: // explain_stmt
      case 199: // explainable_stmt
      case 200: // create_stmt
      case 207: // drop_stmt
      case 209: // show_stmt
      case 211: // use_stmt
      case 212: // desc_stmt
      case 213: // analyze_stmt
      case 215: // set_var_stmt
      case 216: // kill_stmt
        yylhs.value.build< Stmt_s > ();
        break;

      case 170: // basic_table_factor
      case 171: // view_table_factor
      case 172: // joined_table_factor
      case 173: // sub_table_factor
      case 174: // table_factor
        yylhs.value.build< TableStmt_s > ();
        break;

      case 201: // table_element_list
        yylhs.value.build< Vector<ColumnDefineStmt_s> > ();
        break;

      case 158: // select_expr_list
      case 160: // opt_where
      case 161: // opt_groupby
      case 162: // opt_having
      case 178: // arith_expr_list
      case 184: // when_then_list1
      case 185: // when_then_list2
      case 193: // insert_value
      case 195: // update_asgn_list
        yylhs.value.build< Vector<ExprStmt_s> > ();
        break;

      case 148: // opt_hint_list
      case 149: // hint_list
        yylhs.value.build< Vector<HintStmt_s> > ();
        break;

      case 156: // leading_hint_table_list
        yylhs.value.build< Vector<LeadingTable_s> > ();
        break;

      case 163: // opt_order_by
      case 164: // order_by_list
        yylhs.value.build< Vector<OrderStmt_s> > ();
        break;

      case 153: // hint_table_list
      case 206: // opt_engine_def
        yylhs.value.build< Vector<String> > ();
        break;

      case 159: // from_list
        yylhs.value.build< Vector<TableStmt_s> > ();
        break;

      case 192: // insert_value_list
        yylhs.value.build< Vector<Vector<ExprStmt_s>> > ();
        break;

      case 154: // opt_split
      case 157: // opt_distinct
      case 166: // opt_asc_desc
      case 190: // distinct_or_all
      case 208: // opt_if_exists
        yylhs.value.build< bool > ();
        break;

      case 214: // opt_sample_size
        yylhs.value.build< double > ();
        break;

      case 168: // limit_expr
      case 204: // opt_time_precision
      case 205: // opt_char_length
      case 227: // int_value
        yylhs.value.build< int > ();
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 151: // opt_qb_name
      case 152: // opt_qb_name_single
      case 175: // opt_alias
      case 210: // op_from_database
      case 218: // database_name
      case 219: // relation_name
      case 220: // column_name
      case 221: // function_name
      case 222: // column_label
      case 223: // ident
      case 224: // datetime
      case 225: // string
      case 226: // number
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
#line 352 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 1129 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 359 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1135 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 360 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1141 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 361 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1147 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 362 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1153 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 363 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1159 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 364 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1165 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 365 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow stmt"); }
#line 1171 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 374 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1177 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 375 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1183 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 376 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1189 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 377 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1195 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 378 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1201 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 379 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1207 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 380 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1213 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 381 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 386 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1227 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 390 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1235 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 397 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1244 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 402 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1253 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 407 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1262 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 412 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1271 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 417 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1280 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 422 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1289 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 427 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1298 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 432 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1307 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 440 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1315 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 444 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1323 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 451 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1331 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 461 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 1350 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 478 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Hint > () = Hint(); }
#line 1356 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 480 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[1].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = false;
	}
#line 1366 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 486 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[2].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = true;
	}
#line 1376 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 495 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
	}
#line 1384 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 499 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[0].value.as< Vector<HintStmt_s> > ();
	}
#line 1392 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 506 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1403 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 513 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[1].value.as< Vector<HintStmt_s> > ();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1414 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 524 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(NO_REWRITE);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1423 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 529 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1432 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 534 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1441 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 540 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1453 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 548 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1465 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 556 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1477 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 564 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1489 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 572 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->set_is_ordered();
	}
#line 1500 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 579 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1513 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 589 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(PARALLEL);
		ParallelHintStmt_s hint = yylhs.value.as< HintStmt_s > ();
		hint->parallel = yystack_[1].value.as< int > ();
	}
#line 1523 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 597 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1529 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 599 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1537 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 605 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1543 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 607 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "";
	}
#line 1551 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 611 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > ();
	}
#line 1559 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 618 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1568 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 623 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1577 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 630 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1583 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 631 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1589 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 636 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = true;
		yylhs.value.as< LeadingTable_s > ()->table_name = yystack_[0].value.as< std::string > ();
	}
#line 1599 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 642 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = false;
		yylhs.value.as< LeadingTable_s > ()->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1609 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 651 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = Vector<LeadingTable_s>();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1618 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 656 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = yystack_[2].value.as< Vector<LeadingTable_s> > ();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1627 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 663 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1633 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 664 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1639 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 669 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1648 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 674 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yystack_[2].value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
    }
#line 1657 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 682 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = Vector<TableStmt_s>();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1666 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 687 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = yystack_[2].value.as< Vector<TableStmt_s> > ();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1675 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 694 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1681 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 695 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1687 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 699 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1693 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 700 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1699 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 704 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1705 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 705 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1711 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 709 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>(); }
#line 1717 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 710 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[0].value.as< Vector<OrderStmt_s> > (); }
#line 1723 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 715 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1732 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 720 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[2].value.as< Vector<OrderStmt_s> > ();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1741 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 728 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OrderStmt_s > () = OrderStmt::make_order_stmt(yystack_[1].value.as< ExprStmt_s > (), yystack_[0].value.as< bool > ());
    }
#line 1749 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 734 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1755 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 735 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1761 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 736 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1767 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 740 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< LimitStmt_s > () = NULL;}
#line 1773 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 742 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
    }
#line 1784 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 749 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
	}
#line 1795 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 759 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = yystack_[0].value.as< int > ();
	}
#line 1803 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 766 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< ExprStmt_s > ()->to_string();
    }
#line 1813 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 772 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1823 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 778 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[2].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1833 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 787 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< BasicTableStmt_s > ();
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
    }
#line 1842 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 795 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		yylhs.value.as< TableStmt_s > () = ViewTableStmt::make_view_table(yystack_[2].value.as< Stmt_s > ());
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1852 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 804 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), LeftOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1860 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 808 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), RightOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1868 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 812 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), FullOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1876 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 816 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), Inner, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1884 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 823 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1892 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 827 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1900 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 831 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< TableStmt_s > ();
	}
#line 1908 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 838 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1916 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 842 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1924 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 846 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1932 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 852 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1938 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 854 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1946 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 858 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1954 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 870 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 1962 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 874 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 1970 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 878 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 1978 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 882 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 1986 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 886 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 1994 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 893 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2002 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 897 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2011 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 902 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MINUS);
    }
#line 2020 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 907 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_ADD);
	}
#line 2029 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 912 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_SUB);
	}
#line 2038 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 917 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MUL);
	}
#line 2047 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 922 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DIV);
	}
#line 2056 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 927 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2064 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 934 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2074 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 940 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2084 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 949 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2093 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 954 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), OP_IS_NULL);
    }
#line 2102 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 959 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), OP_IS_NOT_NULL);
    }
#line 2111 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 964 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[4].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_BETWEEN);
    }
#line 2120 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 969 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_BETWEEN);
    }
#line 2129 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 974 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2144 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 985 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2159 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 996 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_IN);
    }
#line 2168 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1001 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_IN);
    }
#line 2177 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1006 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EXISTS);
    }
#line 2185 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1010 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_EXISTS);
    }
#line 2194 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1015 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2203 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1023 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE;
	}
#line 2211 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1027 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT;
	}
#line 2219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1031 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ;
	}
#line 2227 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1035 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE;
	}
#line 2235 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1039 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT;
	}
#line 2243 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1043 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE;
	}
#line 2251 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1047 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LIKE;
	}
#line 2259 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1051 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NOT_LIKE;
	}
#line 2267 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1058 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ANY;
	}
#line 2275 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1062 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ANY;
	}
#line 2283 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1066 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ANY;
	}
#line 2291 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1070 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ANY;
	}
#line 2299 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1074 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ANY;
	}
#line 2307 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1078 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ANY;
	}
#line 2315 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1082 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ALL;
	}
#line 2323 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1086 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ALL;
	}
#line 2331 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1090 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ALL;
	}
#line 2339 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1094 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ALL;
	}
#line 2347 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1098 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ALL;
	}
#line 2355 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1102 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ALL;
	}
#line 2363 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1109 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< ExprStmt_s > () = query_expr;
    }
#line 2373 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1118 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[4].value.as< ExprStmt_s > ());
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2384 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1125 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2394 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1134 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2404 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1140 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2414 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1149 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2424 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1155 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2434 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1164 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2442 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1168 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2450 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1172 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_AND);
    }
#line 2459 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1177 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_OR);
    }
#line 2468 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1185 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2479 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1192 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2489 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1198 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2500 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1206 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2511 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1213 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2523 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1221 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2535 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1232 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2548 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1241 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2565 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1254 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_day(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2579 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1264 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_month(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2593 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1274 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_year(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2607 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1284 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2620 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1293 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2633 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1305 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[3].value.as< std::string > ());
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->set_aggr_expr(col);
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 2646 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1314 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[4].value.as< std::string > ());
		aggr->distinct = yystack_[2].value.as< bool > ();
		aggr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 2658 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1322 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[3].value.as< std::string > ());
		aggr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 2669 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1329 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_CHAR);
  }
#line 2677 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1333 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	Object_s value = Varchar::make_object(yystack_[1].value.as< std::string > ());
	check(value);
	ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
	check(stmt);
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), stmt, OP_TO_CHAR);
  }
#line 2689 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1341 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2705 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1353 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_IFNULL);
  }
#line 2713 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1357 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_NUMBER);
  }
#line 2721 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1364 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 2729 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1368 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 2737 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1380 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		insert_stmt->value_list = yystack_[0].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2750 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1389 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[1].value.as< BasicTableStmt_s > ();
		insert_stmt->query_values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2762 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1400 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = Vector<Vector<ExprStmt_s>>();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
    }
#line 2771 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1405 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = yystack_[4].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
	}
#line 2780 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1413 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2789 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1418 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2798 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 1431 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2813 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 1442 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2828 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 1456 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2838 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 1462 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2848 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 1471 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), col, yystack_[0].value.as< ExprStmt_s > (), OP_ASSIGN);
    }
#line 2859 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 1486 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2874 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 1497 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2889 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 1508 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		delete_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		delete_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 2903 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 1526 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		DMLStmt_s dml = yylhs.value.as< Stmt_s > ();
		dml->is_explain = true;
    }
#line 2913 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 1534 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2919 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 1535 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2925 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 1536 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2931 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 1537 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2937 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 204:
#line 1547 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateTable);
		check(cmd_stmt);
		cmd_stmt->params.create_table_params.table = yystack_[4].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.create_table_params.column_define_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		cmd_stmt->params.create_table_params.engine_args = yystack_[0].value.as< Vector<String> > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 2950 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 205:
#line 1556 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2961 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 206:
#line 1566 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 2970 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 207:
#line 1571 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 2979 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 208:
#line 1579 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as< std::string > (), yystack_[0].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
    }
#line 2989 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 209:
#line 1588 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 2995 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 210:
#line 1590 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3001 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 211:
#line 1592 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_MEDIUMINT_PREC, 0); }
#line 3007 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 212:
#line 1594 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3013 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 213:
#line 1596 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
#line 3019 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 214:
#line 1598 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3025 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 215:
#line 1600 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3031 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 216:
#line 1602 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3037 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 217:
#line 1604 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3043 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 218:
#line 1606 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3049 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 219:
#line 1608 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3055 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 220:
#line 1610 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3061 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 221:
#line 1612 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3067 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 222:
#line 1614 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3073 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 223:
#line 1616 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3079 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 224:
#line 1618 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3085 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 225:
#line 1620 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3091 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 226:
#line 1622 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIMESTAMP, yystack_[0].value.as< int > ()); }
#line 3097 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 227:
#line 1624 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATETIME); }
#line 3103 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 228:
#line 1626 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATE); }
#line 3109 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 229:
#line 1628 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIME); }
#line 3115 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 230:
#line 1632 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_TIME_SCALE; }
#line 3121 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 231:
#line 1634 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		if (yystack_[1].value.as< int > () > MAX_TIME_SCALE) {
			yyerror("max time prec:%d", MAX_TIME_SCALE);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	}
#line 3133 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 232:
#line 1644 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_STR_LENGTH; }
#line 3139 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 233:
#line 1646 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		  if (yystack_[1].value.as< int > () > MAX_STR_LENGTH) {
			yyerror("max char length:%d", MAX_STR_LENGTH);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	  }
#line 3151 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 234:
#line 1657 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<String> > () = Vector<String>(); 
		yylhs.value.as< Vector<String> > ().push_back("CAT");
	}
#line 3160 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 235:
#line 1662 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back("CSV");
		yylhs.value.as< Vector<String> > ().push_back(yystack_[10].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back(yystack_[5].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back("new line");
	}
#line 3172 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 236:
#line 1678 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_table_params.table = yystack_[0].value.as< TableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3184 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 237:
#line 1686 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3196 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 238:
#line 1696 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3202 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 239:
#line 1697 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3208 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 240:
#line 1707 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
 	}
#line 3219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 241:
#line 1714 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3230 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 242:
#line 1721 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3241 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 243:
#line 1728 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3252 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 244:
#line 1735 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3260 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 245:
#line 1739 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3268 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 246:
#line 1743 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3280 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 247:
#line 1751 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3292 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 248:
#line 1759 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowProcesslist);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3302 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 249:
#line 1765 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowMemory);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3312 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 250:
#line 1774 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 3320 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 251:
#line 1778 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 3328 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 252:
#line 1785 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(UseDatabase);
		check(cmd_stmt);
		cmd_stmt->params.use_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3339 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 253:
#line 1800 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3350 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 254:
#line 1807 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3361 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 255:
#line 1821 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = yystack_[1].value.as< std::string > ();
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3374 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 256:
#line 1830 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3387 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 257:
#line 1839 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3400 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 258:
#line 1850 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {	yylhs.value.as< double > () = 0.1; }
#line 3406 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 259:
#line 1852 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< double > () = std::stod(yystack_[0].value.as< std::string > ());
	}
#line 3414 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 260:
#line 1859 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(SetVar);
		check(cmd_stmt);
		cmd_stmt->params.set_var_params.var_name = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.set_var_params.var_value = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3426 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 261:
#line 1870 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		cmd_stmt->params.kill_params.thread_id = yystack_[0].value.as< int > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3437 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 262:
#line 1884 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3449 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 263:
#line 1892 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3461 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 264:
#line 1900 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3472 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 265:
#line 1909 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3478 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 266:
#line 1913 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3484 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 267:
#line 1917 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3490 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 268:
#line 1921 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3496 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 269:
#line 1925 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 3502 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 270:
#line 1929 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3508 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 271:
#line 1933 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3514 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 272:
#line 1934 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3520 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 273:
#line 1938 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3526 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 274:
#line 1942 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3532 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 275:
#line 1947 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 3540 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;


#line 3544 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -444;

  const short int parser::yytable_ninf_ = -269;

  const short int
  parser::yypact_[] =
  {
     208,   -67,   -19,    39,     9,     9,   -13,   205,     5,    51,
     -54,    39,    67,   471,    39,    67,   173,    74,  -444,  -444,
     294,   425,  -444,   125,  -444,  -444,  -444,  -444,  -444,  -444,
    -444,  -444,  -444,  -444,  -444,  -444,   100,    67,     9,   365,
     213,  -444,  -444,  -444,    88,  -444,    93,  -444,   132,   132,
    -444,  -444,  -444,  -444,  -444,     9,  -444,  -444,  -444,   134,
     154,   201,   130,   190,  -444,   142,   210,  -444,  -444,  -444,
     169,   235,     9,  -444,  -444,  -444,  -444,    -6,   -54,   -54,
      12,   -54,   -54,   217,   229,  -444,   245,   784,   248,   255,
     255,   256,   263,   255,   264,   255,   284,   297,   327,   784,
    -444,     9,   322,   328,    67,   336,    67,   259,    96,  -444,
    -444,   747,   395,     9,     9,   235,     9,     9,    67,  -444,
     308,   -54,  -444,  -444,  -444,  -444,   -54,  -444,  -444,  -444,
     334,   113,    67,   373,   416,   -10,  -444,  -444,   416,   416,
    -444,    51,  -444,   416,   416,  -444,  -444,   163,     9,   259,
    -444,  -444,  -444,  -444,   -54,  -444,  -444,  -444,   305,   163,
     359,  -444,  -444,  -444,   622,   395,   395,   360,   395,   714,
     747,  -444,   747,  -444,   369,   377,   380,  -444,   114,  -444,
    -444,   432,  -444,  -444,  -444,  -444,  -444,   366,  -444,   401,
     -37,  -444,  -444,  -444,  -444,  -444,  -444,  -444,  -444,  -444,
    -444,    67,  -444,  -444,   390,   390,   390,   -21,  -444,   347,
    -444,   449,  -444,   147,   410,  -444,    67,    67,   419,    67,
      67,    67,   396,  -444,   163,   -35,   305,   428,   456,   462,
     463,   467,  -444,   747,   516,   572,   138,   -47,  -444,  -444,
     747,    -1,   616,   439,  -444,  -444,   747,   747,   747,   747,
     259,    67,   747,   747,   747,   747,  -444,  -444,    84,   696,
     109,  -444,   548,   450,  -444,  -444,  -444,    67,   500,  -444,
    -444,   481,   481,  -444,  -444,   494,  -444,  -444,  -444,  -444,
     497,  -444,  -444,   507,   507,  -444,   481,   481,  -444,  -444,
     147,  -444,   -14,  -444,  -444,   -11,  -444,   131,  -444,   137,
     145,  -444,   572,  -444,   396,   259,  -444,   163,   286,   286,
     286,   286,   418,   159,   508,   535,   518,   459,   845,  -444,
       7,   747,    27,   747,   572,   484,  -444,  -444,  -444,  -444,
     510,   337,   464,  -444,   -35,  -444,  -444,   -26,  -444,   -26,
    -444,  -444,  -444,  -444,  -444,   495,   549,   747,   259,    67,
    -444,   747,    51,  -444,   558,  -444,    51,  -444,  -444,    51,
      51,    51,  -444,  -444,  -444,  -444,   168,  -444,  -444,   147,
    -444,    67,  -444,  -444,  -444,   285,  -444,   305,  -444,   144,
    -444,  -444,   511,   517,   519,   526,   747,  -444,   747,   518,
    -444,   346,    11,   486,    21,   196,   409,   423,   461,   485,
    -444,   747,   521,    31,   747,   518,   572,   572,   747,   180,
     747,   747,   571,    37,   747,    51,   395,  -444,  -444,   544,
    -444,  -444,   563,   -35,  -444,   418,  -444,   581,   522,   590,
     592,   527,  -444,  -444,  -444,   -54,   529,   305,   572,   572,
     572,   572,   418,   171,  -444,  -444,  -444,   747,   551,  -444,
    -444,  -444,  -444,  -444,  -444,  -444,  -444,  -444,  -444,  -444,
    -444,   119,   714,  -444,   621,  -444,   418,  -444,   641,  -444,
     418,   747,   661,   233,  -444,   747,   594,   612,   550,   619,
     597,  -444,  -444,   630,  -444,    51,    51,  -444,   568,  -444,
     285,   285,   285,   285,  -444,   172,   714,  -444,   747,   418,
     179,  -444,   418,  -444,   747,   418,  -444,    51,  -444,   747,
     572,   578,   605,   573,   574,   291,   747,   183,   418,   747,
    -444,   418,   575,   645,   285,   652,   607,   677,  -444,  -444,
     418,  -444,   418,  -444,   747,    51,  -444,   395,   651,  -444,
     498,   654,  -444,   658,   747,  -444,  -444,  -444,    51,   628,
    -444,  -444,   588,   668,   395,   664,   624,   591,   671,   395,
    -444
  };

  const unsigned short int
  parser::yydefact_[] =
  {
       9,     0,     0,    32,     0,     0,     0,     0,     0,     0,
       0,    32,     0,     0,    32,     0,     0,     0,     3,     4,
      19,     0,    29,    28,     5,     6,     7,     8,    11,    12,
      10,    14,    13,    15,    16,    17,     0,     0,     0,    35,
       0,   270,   264,   254,     0,   262,   266,   253,   238,   238,
     200,   202,   203,   201,   199,     0,   274,   275,   261,     0,
      62,     0,     0,     0,   240,     0,     0,   249,   248,   245,
       0,   251,     0,   252,   265,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     0,   205,     0,    35,     0,    51,
      51,     0,     0,    51,     0,    51,     0,     0,     0,    36,
      37,     0,     0,     0,     0,     0,     0,     0,     0,    30,
      63,     0,     0,     0,     0,   251,     0,     0,     0,   242,
       0,     0,    21,    28,    27,    25,     0,    20,    26,    24,
       0,     0,     0,     0,    49,     0,    39,    41,    49,    49,
      46,     0,    40,    49,    49,    33,    38,   101,     0,     0,
     263,   266,   239,   237,     0,    98,    99,   100,   236,   101,
       0,   186,   174,   273,     0,     0,     0,     0,     0,     0,
       0,   163,     0,   166,     0,     0,     0,   151,     0,    64,
     109,    86,   108,   116,   104,   105,   107,     0,   162,     0,
     267,   169,   168,   173,   260,   247,   243,   241,   244,   246,
     250,     0,    23,    22,   258,   258,   258,     0,   206,     0,
     267,     0,    50,     0,     0,    52,     0,     0,     0,     0,
       0,     0,    69,   102,   101,    69,    66,     0,     0,     0,
       0,     0,    89,     0,   185,     0,     0,     0,   272,   271,
       0,     0,   151,     0,   111,   110,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,   269,     0,     0,
      69,   193,     0,     0,   257,   256,   255,     0,   234,   218,
     213,   232,   232,   228,   227,   214,   221,   219,   212,   211,
     215,   220,   210,   230,   230,   209,   232,   232,   208,    34,
       0,    60,    56,    58,    53,    56,    54,    56,    48,    56,
      56,   103,     0,   196,    69,     0,   198,   101,     0,     0,
       0,     0,   189,     0,     0,     0,     0,     0,     0,   158,
       0,     0,     0,     0,     0,     0,   170,   171,   172,   106,
       0,     0,     0,    65,    69,    88,   115,   113,   114,   112,
     165,   167,   164,   183,   184,   163,     0,     0,     0,     0,
     191,     0,     0,   207,     0,   204,     0,   223,   222,     0,
       0,     0,   229,   226,   225,   224,    56,    57,    47,     0,
      43,     0,    45,    42,    44,    68,   197,    67,    90,     0,
      95,    96,     0,     0,     0,     0,     0,   187,     0,     0,
     128,     0,     0,     0,   131,   132,   133,   135,   134,   136,
     137,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,   182,    71,
     175,   177,     0,    69,   194,   195,   259,     0,     0,     0,
       0,     0,    59,    61,    55,     0,   100,     0,     0,     0,
       0,     0,   190,     0,   129,   159,   138,     0,     0,   145,
     139,   146,   140,   147,   141,   149,   143,   148,   142,   150,
     144,     0,     0,   126,     0,   120,   119,   130,   161,   160,
     156,     0,     0,     0,   153,     0,     0,     0,     0,     0,
      73,   176,   192,     0,   233,     0,     0,   231,     0,    97,
      93,    94,    91,    92,   188,     0,     0,   127,     0,   117,
       0,   121,   154,   152,     0,   157,   181,     0,   179,     0,
       0,    74,     0,     0,     0,   101,     0,     0,   122,     0,
     124,   155,     0,    70,    72,     0,    82,     0,   216,   217,
     123,   125,   118,   180,     0,     0,    31,     0,    75,    76,
      79,    84,    85,     0,     0,    80,    81,    78,     0,     0,
      77,    83,     0,     0,     0,     0,     0,     0,     0,     0,
     235
  };

  const short int
  parser::yypgoto_[] =
  {
    -444,  -444,  -444,  -444,     4,  -444,   748,    61,   763,   239,
     626,  -444,   610,   466,   296,   191,  -247,   348,   426,  -444,
    -444,  -242,  -218,  -444,  -444,  -444,  -444,   181,  -444,  -444,
     176,   477,    94,   164,   349,   316,  -104,  -131,  -444,   167,
    -443,  -444,  -444,  -444,  -280,  -444,  -444,  -444,  -244,  -444,
    -444,  -444,  -444,   722,  -444,   342,   724,  -444,   385,   728,
    -444,  -444,  -444,  -444,   473,  -444,   460,  -205,  -444,  -444,
     694,  -444,   637,  -444,  -444,  -444,   106,  -444,  -444,    36,
      41,    17,  -107,  -444,   502,    -3,  -444,  -106,    -9,    14
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    16,    17,    18,    59,    20,    21,   177,    23,    40,
      98,    99,   100,   213,   136,   295,   371,   291,   292,   111,
     178,   225,   303,   480,   511,   526,   538,   539,   547,   536,
     541,   179,   155,   156,   157,   382,   226,   378,   180,   318,
     500,   319,   404,   405,   182,   183,   322,   237,   320,   184,
     185,   186,   347,    24,   234,   313,    25,   260,   261,    26,
      27,    54,    28,   207,   208,   288,   362,   357,   355,    29,
     106,    30,   119,    31,    32,    33,   264,    34,    35,   159,
      44,   187,   188,   189,   256,   190,   191,   192,   193,   542
  };

  const short int
  parser::yytable_[] =
  {
      57,    46,    46,   158,    19,   305,   194,   306,   334,    61,
     323,    50,    74,   406,   407,   214,   222,   406,   407,   267,
     121,    45,    45,    58,    37,   209,   367,   252,   232,   367,
      48,    10,    41,    74,    74,    46,   390,    46,   126,   464,
      43,    47,   350,   406,   407,   369,   326,   449,  -268,   450,
     465,    36,    46,   517,    11,    45,    73,    45,   375,   238,
     239,    22,   241,  -266,   254,    42,   523,   358,    22,    46,
      39,    22,    45,   392,    86,    56,   103,    84,    85,    10,
     413,   364,   365,    55,   410,   268,   376,   324,   327,    45,
      41,   108,   368,   304,   262,   370,   215,    10,    46,    38,
     302,   151,    11,    74,    46,    49,   423,    41,   120,   444,
      46,    46,   161,    46,    46,    74,   419,   445,    45,   369,
      11,   150,   463,    41,    45,   467,   498,   408,   151,   210,
      45,    45,    57,    45,    45,   328,    41,   147,    22,    22,
      22,    22,    22,    22,   151,    46,    46,   153,   206,   195,
     196,   342,   198,   199,   249,   218,   151,   475,   227,   200,
     209,   411,   468,   469,   223,    45,    45,    41,   497,    22,
      41,   367,   252,    75,   340,   348,   223,   367,   257,   516,
     250,    10,    22,    76,   224,   367,    41,    22,   104,   341,
      83,   252,   221,  -265,   490,   491,   492,   493,   210,   386,
      42,   377,   105,   205,    11,   482,   110,   253,   367,   254,
     293,   386,   112,   296,   296,    22,   296,   296,   151,   519,
     255,   151,   451,   519,   452,   252,   253,   160,   254,   435,
     242,   -18,   290,   252,   -18,     1,    41,   372,   301,   255,
     109,   223,   262,   373,   302,   113,   349,    46,   257,     2,
      60,   374,    11,    72,     3,   210,   114,     3,     4,     5,
     253,   115,   254,     6,   210,   387,   524,    45,   253,    42,
     254,     7,   321,   255,   432,   437,   116,   494,   181,   101,
       8,   255,    41,     8,   117,   520,   252,   293,     9,   531,
      10,   406,   407,    10,   -30,   -30,   -30,    77,    78,    79,
     471,   118,    46,   102,   151,    46,    46,    46,    46,    41,
     478,   265,   266,    11,    41,    42,    11,   130,    12,    13,
     221,   253,    45,   254,   223,    45,    45,    45,    45,   131,
     132,   236,    14,   134,   255,    14,   243,   244,    15,   245,
     135,   138,    42,   426,   154,    46,   210,    57,   139,   141,
      57,    57,    57,   504,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,    45,   293,   269,   434,   143,
     428,   379,   228,   429,   430,   431,    46,   416,   242,   229,
     270,   271,   144,   227,   272,   145,   137,   230,   148,   140,
     252,   142,   273,   274,   149,   275,    45,    87,   152,   252,
     312,   276,   380,   380,   380,   380,   477,   325,   297,   231,
     299,   300,   277,   330,   331,   332,   181,   163,   201,   336,
     337,   338,   339,   278,   204,   253,   346,   254,    80,    81,
      82,   543,   211,   279,   253,   453,   254,   454,   255,   488,
      22,   212,   280,   417,   233,   240,    88,   255,   555,   455,
     281,   456,   329,   560,   246,    41,    89,    90,    91,    92,
     282,   251,   247,    93,    94,   248,   258,   315,   283,   284,
     285,   252,   381,   381,   381,   381,    57,    57,   162,   286,
     287,   163,    41,    56,   391,   252,   259,   457,   409,   458,
     412,    95,   252,    96,    97,   164,    22,   263,   522,   513,
     514,   446,   447,   448,   165,   166,   253,   289,   254,    62,
      63,   459,   151,   460,   422,    64,   294,   252,   425,   255,
     253,   316,   254,   242,   414,   298,    57,   253,   545,   254,
     167,   302,   223,   255,   307,   308,   168,   252,    65,    57,
     255,   309,   310,    66,   317,   329,   311,   170,   546,   171,
     415,   252,   253,   442,   254,   312,   314,   242,    67,   351,
     172,   354,   352,   252,   173,   255,   356,    11,   461,   427,
     418,   466,   253,    68,   254,   470,   174,   472,   473,   359,
     315,   476,   360,   175,   176,   255,   253,    69,   254,    70,
      71,   162,   361,   388,   163,    41,    56,   389,   253,   255,
     254,   420,   252,    10,   216,   217,   462,   438,   164,   219,
     220,   255,   479,   439,   495,   440,   252,   165,   166,   -29,
     -29,   -29,   441,   483,   252,   383,   384,   385,   484,   499,
     485,   474,   486,   487,   316,   489,   496,   253,   502,   254,
     501,   162,   505,   167,   163,    41,    56,   252,   407,   168,
     255,   253,   507,   254,   509,   421,   508,   317,   164,   253,
     170,   254,   171,   499,   255,   518,   510,   165,   166,   481,
     512,   521,   255,   172,   515,   525,   499,   173,   527,   528,
     529,   533,   253,   530,   254,   519,   532,   534,   537,   174,
     535,   544,   552,   167,   548,   255,   175,   176,   549,   168,
     506,   540,   553,   554,   556,   558,   559,   169,   557,   146,
     170,   540,   171,   133,   252,   162,   366,   433,   163,    41,
      56,   503,   343,   172,   551,   550,   333,   173,   436,    51,
     443,    52,   164,   162,   424,    53,   163,    41,    56,   174,
     353,   165,   166,   107,   363,     0,   175,   176,   344,   253,
     164,   254,   197,   335,     0,     0,   235,     0,     0,   165,
     166,     0,   255,     0,     0,     0,   162,   167,     0,   163,
      41,    56,     0,   168,     0,     0,     0,     0,     0,     0,
       0,   169,     0,   164,   170,   167,   345,     0,     0,     0,
       0,   168,   165,   166,     0,     0,     0,   172,     0,   169,
       0,   173,   170,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,   174,     0,   172,     0,     0,   167,   173,
     175,   176,    11,     0,   168,   122,   124,   125,   127,   128,
     129,   174,   169,     0,     0,   170,     0,   171,   175,   176,
     123,   123,   123,   123,   123,   123,     0,     0,   172,     0,
       0,     0,   173,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   174,    88,     0,     0,     0,   202,
       0,   175,   176,     0,   203,    89,    90,    91,    92,     0,
       0,     0,    93,    94,   123,     0,     0,     0,     0,   123,
       0,     0,     0,     0,     0,     0,     0,     0,   252,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,    96,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   253,     0,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255
  };

  const short int
  parser::yycheck_[] =
  {
       9,     4,     5,   107,     0,    40,   112,   225,   250,    12,
      57,     7,    15,     6,     7,    25,   147,     6,     7,    40,
      26,     4,     5,     9,    43,   132,    40,    53,   159,    40,
      43,    85,    23,    36,    37,    38,   316,    40,    26,     8,
       4,     5,   260,     6,     7,   292,    47,    26,    85,    28,
      19,   118,    55,   496,   108,    38,    15,    40,   302,   165,
     166,     0,   168,   100,    90,    56,   509,   272,     7,    72,
      31,    10,    55,   317,    38,    24,    40,    36,    37,    85,
     324,   286,   287,    78,    57,   106,   304,   134,    89,    72,
      23,    55,   106,   224,   201,   106,   106,    85,   101,   118,
     135,   104,   108,   106,   107,   118,   348,    23,    72,   389,
     113,   114,   108,   116,   117,   118,   334,   106,   101,   366,
     108,   104,   402,    23,   107,   405,     7,   120,   131,   132,
     113,   114,   141,   116,   117,   136,    23,   101,    77,    78,
      79,    80,    81,    82,   147,   148,   149,   106,   131,   113,
     114,   258,   116,   117,    40,   141,   159,   120,   154,   118,
     267,   134,   406,   407,   147,   148,   149,    23,   448,   108,
      23,    40,    53,     0,    90,    66,   159,    40,   181,     7,
      66,    85,   121,   109,   148,    40,    23,   126,   100,   105,
      90,    53,    29,   100,   438,   439,   440,   441,   201,    40,
      56,   305,    70,    90,   108,   423,    52,    88,    40,    90,
     213,    40,    11,   216,   217,   154,   219,   220,   221,    40,
     101,   224,    26,    40,    28,    53,    88,   131,    90,    85,
     169,   106,    85,    53,   109,    27,    23,   106,   221,   101,
     106,   224,   349,   106,   135,   115,   137,   250,   251,    41,
      11,   106,   108,    14,    49,   258,    66,    49,    50,    51,
      88,   119,    90,    55,   267,   106,   510,   250,    88,    56,
      90,    63,   134,   101,   106,   379,    66,   106,   111,    66,
      75,   101,    23,    75,   115,   106,    53,   290,    80,   106,
      85,     6,     7,    85,     3,     4,     5,     3,     4,     5,
     120,    66,   305,    90,   307,   308,   309,   310,   311,    23,
     416,   205,   206,   108,    23,    56,   108,   100,   110,   111,
      29,    88,   305,    90,   307,   308,   309,   310,   311,   100,
      85,   164,   127,    85,   101,   127,   169,   170,   130,   172,
      85,    85,    56,   352,    85,   348,   349,   356,    85,    85,
     359,   360,   361,   120,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,   348,   369,    20,   371,    85,
     356,    85,    67,   359,   360,   361,   379,    40,   317,    74,
      33,    34,    85,   379,    37,    58,    90,    82,    66,    93,
      53,    95,    45,    46,    66,    48,   379,    32,    62,    53,
     233,    54,   308,   309,   310,   311,   415,   240,   217,   104,
     219,   220,    65,   246,   247,   248,   249,    22,   110,   252,
     253,   254,   255,    76,    90,    88,   259,    90,     3,     4,
       5,   537,    59,    86,    88,    26,    90,    28,   101,   435,
     379,    25,    95,   106,    85,    85,    81,   101,   554,    26,
     103,    28,   106,   559,    85,    23,    91,    92,    93,    94,
     113,    29,    85,    98,    99,    85,   100,     8,   121,   122,
     123,    53,   308,   309,   310,   311,   485,   486,    19,   132,
     133,    22,    23,    24,   317,    53,    85,    26,   321,    28,
     323,   126,    53,   128,   129,    36,   435,   107,   507,   485,
     486,    15,    16,    17,    45,    46,    88,    58,    90,    38,
      39,    26,   515,    28,   347,    44,   106,    53,   351,   101,
      88,    62,    90,   462,    40,   106,   535,    88,    30,    90,
      71,   135,   515,   101,   106,    79,    77,    53,    67,   548,
     101,    79,    79,    72,    85,   106,    79,    88,    50,    90,
      40,    53,    88,   386,    90,   388,    40,   496,    87,    11,
     101,    61,   112,    53,   105,   101,    85,   108,   401,    11,
     106,   404,    88,   102,    90,   408,   117,   410,   411,    85,
       8,   414,    85,   124,   125,   101,    88,   116,    90,   118,
     119,    19,    85,    85,    22,    23,    24,    62,    88,   101,
      90,   106,    53,    85,   138,   139,    85,    96,    36,   143,
     144,   101,    68,    96,   447,    96,    53,    45,    46,     3,
       4,     5,    96,    42,    53,   309,   310,   311,   106,   462,
      40,    60,    40,   106,    62,   106,    85,    88,   471,    90,
      19,    19,   475,    71,    22,    23,    24,    53,     7,    77,
     101,    88,    40,    90,    35,   106,   106,    85,    36,    88,
      88,    90,    90,   496,   101,   498,    69,    45,    46,   106,
      40,   504,   101,   101,   106,    97,   509,   105,    73,   106,
     106,   106,    88,   516,    90,    40,   519,    35,    11,   117,
      83,    40,    64,    71,    40,   101,   124,   125,    40,    77,
     106,   534,   114,    35,    40,   114,    35,    85,    84,    99,
      88,   544,    90,    87,    53,    19,   290,   369,    22,    23,
      24,    60,    26,   101,   548,   544,   249,   105,   379,     7,
     388,     7,    36,    19,   349,     7,    22,    23,    24,   117,
     267,    45,    46,    49,   284,    -1,   124,   125,    52,    88,
      36,    90,   115,   251,    -1,    -1,   134,    -1,    -1,    45,
      46,    -1,   101,    -1,    -1,    -1,    19,    71,    -1,    22,
      23,    24,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    36,    88,    71,    90,    -1,    -1,    -1,
      -1,    77,    45,    46,    -1,    -1,    -1,   101,    -1,    85,
      -1,   105,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   101,    -1,    -1,    71,   105,
     124,   125,   108,    -1,    77,    77,    78,    79,    80,    81,
      82,   117,    85,    -1,    -1,    88,    -1,    90,   124,   125,
      77,    78,    79,    80,    81,    82,    -1,    -1,   101,    -1,
      -1,    -1,   105,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,   117,    81,    -1,    -1,    -1,   121,
      -1,   124,   125,    -1,   126,    91,    92,    93,    94,    -1,
      -1,    -1,    98,    99,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    27,    41,    49,    50,    51,    55,    63,    75,    80,
      85,   108,   110,   111,   127,   130,   139,   140,   141,   142,
     143,   144,   145,   146,   191,   194,   197,   198,   200,   207,
     209,   211,   212,   213,   215,   216,   118,    43,   118,    31,
     147,    23,    56,   217,   218,   219,   223,   217,    43,   118,
     142,   191,   194,   197,   199,    78,    24,   226,   227,   142,
     147,   223,    38,    39,    44,    67,    72,    87,   102,   116,
     118,   119,   147,   218,   223,     0,   109,     3,     4,     5,
       3,     4,     5,    90,   218,   218,   217,    32,    81,    91,
      92,    93,    94,    98,    99,   126,   128,   129,   148,   149,
     150,    66,    90,   217,   100,    70,   208,   208,   217,   106,
      52,   157,    11,   115,    66,   119,    66,   115,    66,   210,
     217,    26,   144,   146,   144,   144,    26,   144,   144,   144,
     100,   100,    85,   148,    85,    85,   152,   152,    85,    85,
     152,    85,   152,    85,    85,    58,   150,   217,    66,    66,
     219,   223,    62,   218,    85,   170,   171,   172,   174,   217,
     131,   142,    19,    22,    36,    45,    46,    71,    77,    85,
      88,    90,   101,   105,   117,   124,   125,   145,   158,   169,
     176,   177,   182,   183,   187,   188,   189,   219,   220,   221,
     223,   224,   225,   226,   225,   217,   217,   210,   217,   217,
     218,   110,   144,   144,    90,    90,   219,   201,   202,   220,
     223,    59,    25,   151,    25,   106,   151,   151,   227,   151,
     151,    29,   175,   219,   217,   159,   174,   142,    67,    74,
      82,   104,   175,    85,   192,   134,   177,   185,   225,   225,
      85,   225,   145,   177,   177,   177,    85,    85,    85,    40,
      66,    29,    53,    88,    90,   101,   222,   223,   100,    85,
     195,   196,   220,   107,   214,   214,   214,    40,   106,    20,
      33,    34,    37,    45,    46,    48,    54,    65,    76,    86,
      95,   103,   113,   121,   122,   123,   132,   133,   203,    58,
      85,   155,   156,   223,   106,   153,   223,   153,   106,   153,
     153,   219,   135,   160,   175,    40,   160,   106,    79,    79,
      79,    79,   177,   193,    40,     8,    62,    85,   177,   179,
     186,   134,   184,    57,   134,   177,    47,    89,   136,   106,
     177,   177,   177,   169,   159,   222,   177,   177,   177,   177,
      90,   105,   220,    26,    52,    90,   177,   190,    66,   137,
     160,    11,   112,   202,    61,   206,    85,   205,   205,    85,
      85,    85,   204,   204,   205,   205,   156,    40,   106,   154,
     106,   154,   106,   106,   106,   186,   160,   174,   175,    85,
     170,   171,   173,   173,   173,   173,    40,   106,    85,    62,
     182,   177,   186,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,   180,   181,     6,     7,   120,   177,
      57,   134,   177,   186,    40,    40,    40,   106,   106,   160,
     106,   106,   177,   159,   196,   177,   226,    11,   227,   227,
     227,   227,   106,   155,   223,    85,   172,   174,    96,    96,
      96,    96,   177,   193,   182,   106,    15,    16,    17,    26,
      28,    26,    28,    26,    28,    26,    28,    26,    28,    26,
      28,   177,    85,   182,     8,    19,   177,   182,   186,   186,
     177,   120,   177,   177,    60,   120,   177,   226,   225,    68,
     161,   106,   160,    42,   106,    40,    40,   106,   142,   106,
     186,   186,   186,   186,   106,   177,    85,   182,     7,   177,
     178,    19,   177,    60,   120,   177,   106,    40,   106,    35,
      69,   162,    40,   227,   227,   106,     7,   178,   177,    40,
     106,   177,   226,   178,   186,    97,   163,    73,   106,   106,
     177,   106,   177,   106,    35,    83,   167,    11,   164,   165,
     177,   168,   227,   225,    40,    30,    50,   166,    40,    40,
     165,   168,    64,   114,    35,   225,    40,    84,   114,    35,
     225
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   138,   139,   140,   140,   140,   140,   140,   140,   140,
     141,   141,   141,   141,   141,   141,   141,   141,   142,   142,
     143,   143,   143,   143,   143,   143,   143,   143,   144,   144,
     145,   146,   147,   147,   147,   148,   148,   149,   149,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   151,
     151,   152,   152,   152,   153,   153,   154,   154,   155,   155,
     156,   156,   157,   157,   158,   158,   159,   159,   160,   160,
     161,   161,   162,   162,   163,   163,   164,   164,   165,   166,
     166,   166,   167,   167,   167,   168,   169,   169,   169,   170,
     171,   172,   172,   172,   172,   173,   173,   173,   174,   174,
     174,   175,   175,   175,   176,   176,   176,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   178,   178,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   180,   180,   180,   180,   180,   180,   180,   180,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   182,   183,   183,   184,   184,   185,   185,   186,   186,
     186,   186,   187,   187,   187,   187,   187,   187,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   189,   189,   189,
     189,   189,   189,   190,   190,   191,   191,   192,   192,   193,
     193,   194,   194,   195,   195,   196,   197,   197,   197,   198,
     199,   199,   199,   199,   200,   200,   201,   201,   202,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     204,   204,   205,   205,   206,   206,   207,   207,   208,   208,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     210,   210,   211,   212,   212,   213,   213,   213,   214,   214,
     215,   216,   217,   217,   217,   218,   219,   220,   221,   222,
     223,   224,   224,   225,   226,   227
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     4,     4,     3,     3,     3,     3,     1,     1,
       3,    11,     0,     3,     5,     0,     1,     1,     2,     2,
       2,     2,     5,     5,     5,     5,     2,     5,     4,     0,
       1,     0,     2,     3,     1,     3,     0,     1,     1,     3,
       1,     3,     0,     1,     1,     3,     1,     3,     2,     0,
       3,     0,     2,     0,     0,     3,     1,     3,     2,     0,
       1,     1,     0,     4,     2,     1,     1,     2,     3,     2,
       4,     6,     6,     6,     6,     1,     1,     3,     1,     1,
       1,     0,     1,     2,     1,     1,     3,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     1,     1,     3,     3,
       3,     4,     5,     6,     5,     6,     3,     4,     2,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     6,     5,     4,     5,     4,     5,     1,     3,
       3,     3,     1,     1,     3,     3,     1,     3,     1,     1,
       3,     3,     3,     1,     1,     4,     5,     4,     4,     6,
       8,     6,     4,     1,     1,     5,     4,     3,     5,     1,
       3,     6,     8,     1,     3,     3,     6,     7,     6,     2,
       1,     1,     1,     1,     7,     3,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     6,     6,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     1,     1,     2,
       0,     3,     0,     3,     0,    17,     4,     4,     0,     2,
       2,     4,     3,     4,     4,     2,     4,     4,     2,     2,
       2,     0,     2,     2,     2,     6,     6,     6,     0,     3,
       4,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1
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
  "INTO", "JOIN", "KILL", "LEADING", "LEFT", "LIMIT", "LINE", "\"(\"",
  "MEDIUMINT", "MEMORY", "\"-\"", "MONTH", "\"*\"", "NO_REWRITE",
  "NO_UNNEST", "NO_USE_HASH", "NO_USE_NL", "NUMERIC_SYM", "ON", "ORDER",
  "ORDERED", "PARALLEL", "\".\"", "\"+\"", "PROCESSLIST", "REAL", "RIGHT",
  "ROWID", "\")\"", "SAMPLE", "SELECT", "\";\"", "SET", "SHOW", "SIZE",
  "SMALLINT", "SPLIT", "STATIS", "STATUS", "SUBSTR", "TABLE", "TABLES",
  "THEN", "TIME", "TIMESTAMP_SYM", "TINYINT", "TO_CHAR", "TO_NUMBER",
  "UNNEST", "UPDATE", "USE_HASH", "USE_NL", "USING", "VALUES", "VARBINARY",
  "VARCHAR", "WHEN", "WHERE", "YEAR", "','", "$accept", "sql_stmt", "stmt",
  "cmd_stmt", "select_stmt", "set_select", "sub_set_select",
  "select_with_parens", "simple_select", "opt_hint", "opt_hint_list",
  "hint_list", "single_hint", "opt_qb_name", "opt_qb_name_single",
  "hint_table_list", "opt_split", "leading_hint_table",
  "leading_hint_table_list", "opt_distinct", "select_expr_list",
  "from_list", "opt_where", "opt_groupby", "opt_having", "opt_order_by",
  "order_by_list", "order_by", "opt_asc_desc", "opt_select_limit",
  "limit_expr", "projection", "basic_table_factor", "view_table_factor",
  "joined_table_factor", "sub_table_factor", "table_factor", "opt_alias",
  "simple_expr", "arith_expr", "arith_expr_list", "cmp_expr", "cmp_type",
  "sq_cmp_type", "query_ref_expr", "case_when_expr", "when_then_list1",
  "when_then_list2", "logical_expr", "column_ref", "expr_const",
  "func_expr", "distinct_or_all", "insert_stmt", "insert_value_list",
  "insert_value", "update_stmt", "update_asgn_list", "update_asgn_factor",
  "delete_stmt", "explain_stmt", "explainable_stmt", "create_stmt",
  "table_element_list", "column_definition", "data_type",
  "opt_time_precision", "opt_char_length", "opt_engine_def", "drop_stmt",
  "opt_if_exists", "show_stmt", "op_from_database", "use_stmt",
  "desc_stmt", "analyze_stmt", "opt_sample_size", "set_var_stmt",
  "kill_stmt", "relation_factor", "database_name", "relation_name",
  "column_name", "function_name", "column_label", "ident", "datetime",
  "string", "number", "int_value", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   351,   351,   359,   360,   361,   362,   363,   364,   365,
     374,   375,   376,   377,   378,   379,   380,   381,   385,   389,
     396,   401,   406,   411,   416,   421,   426,   431,   439,   443,
     450,   457,   478,   479,   485,   495,   498,   505,   512,   523,
     528,   533,   539,   547,   555,   563,   571,   578,   588,   597,
     598,   605,   606,   610,   617,   622,   630,   631,   635,   641,
     650,   655,   663,   664,   668,   673,   681,   686,   694,   695,
     699,   700,   704,   705,   709,   710,   714,   719,   727,   734,
     735,   736,   740,   741,   748,   758,   765,   771,   777,   786,
     794,   803,   807,   811,   815,   822,   826,   830,   837,   841,
     845,   852,   853,   857,   869,   873,   877,   881,   885,   892,
     896,   901,   906,   911,   916,   921,   926,   933,   939,   948,
     953,   958,   963,   968,   973,   984,   995,  1000,  1005,  1009,
    1014,  1022,  1026,  1030,  1034,  1038,  1042,  1046,  1050,  1057,
    1061,  1065,  1069,  1073,  1077,  1081,  1085,  1089,  1093,  1097,
    1101,  1108,  1117,  1124,  1133,  1139,  1148,  1154,  1163,  1167,
    1171,  1176,  1184,  1191,  1197,  1205,  1212,  1220,  1231,  1240,
    1253,  1263,  1273,  1283,  1292,  1304,  1313,  1321,  1328,  1332,
    1340,  1352,  1356,  1363,  1367,  1379,  1388,  1399,  1404,  1412,
    1417,  1430,  1441,  1455,  1461,  1470,  1485,  1496,  1507,  1525,
    1534,  1535,  1536,  1537,  1546,  1555,  1565,  1570,  1578,  1587,
    1589,  1591,  1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,
    1609,  1611,  1613,  1615,  1617,  1619,  1621,  1623,  1625,  1627,
    1632,  1633,  1644,  1645,  1657,  1661,  1677,  1685,  1696,  1697,
    1706,  1713,  1720,  1727,  1734,  1738,  1742,  1750,  1758,  1764,
    1773,  1778,  1784,  1799,  1806,  1820,  1829,  1838,  1850,  1851,
    1858,  1869,  1883,  1891,  1899,  1909,  1913,  1917,  1921,  1925,
    1929,  1933,  1934,  1938,  1942,  1946
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
#line 4400 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:1167
#line 1950 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
