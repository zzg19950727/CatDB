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

#line 132 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sql_parser.hpp"

// User implementation prologue.

#line 146 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:412


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
#line 232 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:479

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
      case 212: // aggr_type
        value.move< AggrType > (that.value);
        break;

      case 244: // relation_factor
        value.move< BasicTableStmt_s > (that.value);
        break;

      case 228: // column_definition
        value.move< ColumnDefineStmt_s > (that.value);
        break;

      case 230: // data_type
        value.move< DataType > (that.value);
        break;

      case 188: // projection
      case 196: // simple_expr
      case 197: // seconds_expr
      case 198: // arith_expr
      case 200: // cmp_expr
      case 203: // query_ref_expr
      case 204: // case_when_expr
      case 207: // logical_expr
      case 208: // column_ref
      case 209: // expr_const
      case 210: // func_expr
      case 211: // aggr_expr
      case 219: // update_asgn_factor
        value.move< ExprStmt_s > (that.value);
        break;

      case 166: // opt_hint
        value.move< Hint > (that.value);
        break;

      case 169: // single_hint
        value.move< HintStmt_s > (that.value);
        break;

      case 174: // leading_hint_table
        value.move< LeadingTable_s > (that.value);
        break;

      case 186: // opt_select_limit
        value.move< LimitStmt_s > (that.value);
        break;

      case 201: // cmp_type
      case 202: // sq_cmp_type
        value.move< OperationType > (that.value);
        break;

      case 184: // order_by
        value.move< OrderStmt_s > (that.value);
        break;

      case 158: // sql_stmt
      case 159: // stmt
      case 160: // cmd_stmt
      case 161: // select_stmt
      case 162: // set_select
      case 163: // sub_set_select
      case 164: // select_with_parens
      case 165: // simple_select
      case 214: // insert_stmt
      case 217: // update_stmt
      case 220: // delete_stmt
      case 221: // explain_stmt
      case 222: // explainable_stmt
      case 223: // create_stmt
      case 234: // drop_stmt
      case 236: // show_stmt
      case 238: // use_stmt
      case 239: // desc_stmt
      case 240: // analyze_stmt
      case 242: // set_var_stmt
      case 243: // kill_stmt
        value.move< Stmt_s > (that.value);
        break;

      case 189: // basic_table_factor
      case 190: // view_table_factor
      case 191: // joined_table_factor
      case 193: // sub_table_factor
      case 194: // table_factor
        value.move< TableStmt_s > (that.value);
        break;

      case 227: // table_element_list
        value.move< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 177: // select_expr_list
      case 179: // opt_where
      case 180: // opt_groupby
      case 181: // opt_having
      case 199: // arith_expr_list
      case 205: // when_then_list1
      case 206: // when_then_list2
      case 216: // insert_value
      case 218: // update_asgn_list
        value.move< Vector<ExprStmt_s> > (that.value);
        break;

      case 167: // opt_hint_list
      case 168: // hint_list
        value.move< Vector<HintStmt_s> > (that.value);
        break;

      case 175: // leading_hint_table_list
        value.move< Vector<LeadingTable_s> > (that.value);
        break;

      case 182: // opt_order_by
      case 183: // order_by_list
        value.move< Vector<OrderStmt_s> > (that.value);
        break;

      case 172: // hint_table_list
      case 224: // opt_view_column_define
      case 225: // view_column_define
      case 233: // opt_engine_def
        value.move< Vector<String> > (that.value);
        break;

      case 178: // from_list
        value.move< Vector<TableStmt_s> > (that.value);
        break;

      case 215: // insert_value_list
        value.move< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 173: // opt_split
      case 176: // opt_distinct
      case 185: // opt_asc_desc
      case 192: // opt_outer
      case 213: // distinct_or_all
      case 229: // opt_not_null
      case 235: // opt_if_exists
        value.move< bool > (that.value);
        break;

      case 241: // opt_sample_size
        value.move< double > (that.value);
        break;

      case 187: // limit_expr
      case 231: // opt_time_precision
      case 232: // opt_char_length
      case 253: // int_value
        value.move< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 170: // opt_qb_name
      case 171: // opt_qb_name_single
      case 195: // opt_alias
      case 226: // beg_view_define
      case 237: // op_from_database
      case 245: // database_name
      case 246: // relation_name
      case 247: // column_name
      case 248: // column_label
      case 249: // ident
      case 250: // datetime
      case 251: // string
      case 252: // number
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
      case 212: // aggr_type
        value.copy< AggrType > (that.value);
        break;

      case 244: // relation_factor
        value.copy< BasicTableStmt_s > (that.value);
        break;

      case 228: // column_definition
        value.copy< ColumnDefineStmt_s > (that.value);
        break;

      case 230: // data_type
        value.copy< DataType > (that.value);
        break;

      case 188: // projection
      case 196: // simple_expr
      case 197: // seconds_expr
      case 198: // arith_expr
      case 200: // cmp_expr
      case 203: // query_ref_expr
      case 204: // case_when_expr
      case 207: // logical_expr
      case 208: // column_ref
      case 209: // expr_const
      case 210: // func_expr
      case 211: // aggr_expr
      case 219: // update_asgn_factor
        value.copy< ExprStmt_s > (that.value);
        break;

      case 166: // opt_hint
        value.copy< Hint > (that.value);
        break;

      case 169: // single_hint
        value.copy< HintStmt_s > (that.value);
        break;

      case 174: // leading_hint_table
        value.copy< LeadingTable_s > (that.value);
        break;

      case 186: // opt_select_limit
        value.copy< LimitStmt_s > (that.value);
        break;

      case 201: // cmp_type
      case 202: // sq_cmp_type
        value.copy< OperationType > (that.value);
        break;

      case 184: // order_by
        value.copy< OrderStmt_s > (that.value);
        break;

      case 158: // sql_stmt
      case 159: // stmt
      case 160: // cmd_stmt
      case 161: // select_stmt
      case 162: // set_select
      case 163: // sub_set_select
      case 164: // select_with_parens
      case 165: // simple_select
      case 214: // insert_stmt
      case 217: // update_stmt
      case 220: // delete_stmt
      case 221: // explain_stmt
      case 222: // explainable_stmt
      case 223: // create_stmt
      case 234: // drop_stmt
      case 236: // show_stmt
      case 238: // use_stmt
      case 239: // desc_stmt
      case 240: // analyze_stmt
      case 242: // set_var_stmt
      case 243: // kill_stmt
        value.copy< Stmt_s > (that.value);
        break;

      case 189: // basic_table_factor
      case 190: // view_table_factor
      case 191: // joined_table_factor
      case 193: // sub_table_factor
      case 194: // table_factor
        value.copy< TableStmt_s > (that.value);
        break;

      case 227: // table_element_list
        value.copy< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 177: // select_expr_list
      case 179: // opt_where
      case 180: // opt_groupby
      case 181: // opt_having
      case 199: // arith_expr_list
      case 205: // when_then_list1
      case 206: // when_then_list2
      case 216: // insert_value
      case 218: // update_asgn_list
        value.copy< Vector<ExprStmt_s> > (that.value);
        break;

      case 167: // opt_hint_list
      case 168: // hint_list
        value.copy< Vector<HintStmt_s> > (that.value);
        break;

      case 175: // leading_hint_table_list
        value.copy< Vector<LeadingTable_s> > (that.value);
        break;

      case 182: // opt_order_by
      case 183: // order_by_list
        value.copy< Vector<OrderStmt_s> > (that.value);
        break;

      case 172: // hint_table_list
      case 224: // opt_view_column_define
      case 225: // view_column_define
      case 233: // opt_engine_def
        value.copy< Vector<String> > (that.value);
        break;

      case 178: // from_list
        value.copy< Vector<TableStmt_s> > (that.value);
        break;

      case 215: // insert_value_list
        value.copy< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 173: // opt_split
      case 176: // opt_distinct
      case 185: // opt_asc_desc
      case 192: // opt_outer
      case 213: // distinct_or_all
      case 229: // opt_not_null
      case 235: // opt_if_exists
        value.copy< bool > (that.value);
        break;

      case 241: // opt_sample_size
        value.copy< double > (that.value);
        break;

      case 187: // limit_expr
      case 231: // opt_time_precision
      case 232: // opt_char_length
      case 253: // int_value
        value.copy< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 170: // opt_qb_name
      case 171: // opt_qb_name_single
      case 195: // opt_alias
      case 226: // beg_view_define
      case 237: // op_from_database
      case 245: // database_name
      case 246: // relation_name
      case 247: // column_name
      case 248: // column_label
      case 249: // ident
      case 250: // datetime
      case 251: // string
      case 252: // number
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
      case 212: // aggr_type
        yylhs.value.build< AggrType > ();
        break;

      case 244: // relation_factor
        yylhs.value.build< BasicTableStmt_s > ();
        break;

      case 228: // column_definition
        yylhs.value.build< ColumnDefineStmt_s > ();
        break;

      case 230: // data_type
        yylhs.value.build< DataType > ();
        break;

      case 188: // projection
      case 196: // simple_expr
      case 197: // seconds_expr
      case 198: // arith_expr
      case 200: // cmp_expr
      case 203: // query_ref_expr
      case 204: // case_when_expr
      case 207: // logical_expr
      case 208: // column_ref
      case 209: // expr_const
      case 210: // func_expr
      case 211: // aggr_expr
      case 219: // update_asgn_factor
        yylhs.value.build< ExprStmt_s > ();
        break;

      case 166: // opt_hint
        yylhs.value.build< Hint > ();
        break;

      case 169: // single_hint
        yylhs.value.build< HintStmt_s > ();
        break;

      case 174: // leading_hint_table
        yylhs.value.build< LeadingTable_s > ();
        break;

      case 186: // opt_select_limit
        yylhs.value.build< LimitStmt_s > ();
        break;

      case 201: // cmp_type
      case 202: // sq_cmp_type
        yylhs.value.build< OperationType > ();
        break;

      case 184: // order_by
        yylhs.value.build< OrderStmt_s > ();
        break;

      case 158: // sql_stmt
      case 159: // stmt
      case 160: // cmd_stmt
      case 161: // select_stmt
      case 162: // set_select
      case 163: // sub_set_select
      case 164: // select_with_parens
      case 165: // simple_select
      case 214: // insert_stmt
      case 217: // update_stmt
      case 220: // delete_stmt
      case 221: // explain_stmt
      case 222: // explainable_stmt
      case 223: // create_stmt
      case 234: // drop_stmt
      case 236: // show_stmt
      case 238: // use_stmt
      case 239: // desc_stmt
      case 240: // analyze_stmt
      case 242: // set_var_stmt
      case 243: // kill_stmt
        yylhs.value.build< Stmt_s > ();
        break;

      case 189: // basic_table_factor
      case 190: // view_table_factor
      case 191: // joined_table_factor
      case 193: // sub_table_factor
      case 194: // table_factor
        yylhs.value.build< TableStmt_s > ();
        break;

      case 227: // table_element_list
        yylhs.value.build< Vector<ColumnDefineStmt_s> > ();
        break;

      case 177: // select_expr_list
      case 179: // opt_where
      case 180: // opt_groupby
      case 181: // opt_having
      case 199: // arith_expr_list
      case 205: // when_then_list1
      case 206: // when_then_list2
      case 216: // insert_value
      case 218: // update_asgn_list
        yylhs.value.build< Vector<ExprStmt_s> > ();
        break;

      case 167: // opt_hint_list
      case 168: // hint_list
        yylhs.value.build< Vector<HintStmt_s> > ();
        break;

      case 175: // leading_hint_table_list
        yylhs.value.build< Vector<LeadingTable_s> > ();
        break;

      case 182: // opt_order_by
      case 183: // order_by_list
        yylhs.value.build< Vector<OrderStmt_s> > ();
        break;

      case 172: // hint_table_list
      case 224: // opt_view_column_define
      case 225: // view_column_define
      case 233: // opt_engine_def
        yylhs.value.build< Vector<String> > ();
        break;

      case 178: // from_list
        yylhs.value.build< Vector<TableStmt_s> > ();
        break;

      case 215: // insert_value_list
        yylhs.value.build< Vector<Vector<ExprStmt_s>> > ();
        break;

      case 173: // opt_split
      case 176: // opt_distinct
      case 185: // opt_asc_desc
      case 192: // opt_outer
      case 213: // distinct_or_all
      case 229: // opt_not_null
      case 235: // opt_if_exists
        yylhs.value.build< bool > ();
        break;

      case 241: // opt_sample_size
        yylhs.value.build< double > ();
        break;

      case 187: // limit_expr
      case 231: // opt_time_precision
      case 232: // opt_char_length
      case 253: // int_value
        yylhs.value.build< int > ();
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 170: // opt_qb_name
      case 171: // opt_qb_name_single
      case 195: // opt_alias
      case 226: // beg_view_define
      case 237: // op_from_database
      case 245: // database_name
      case 246: // relation_name
      case 247: // column_name
      case 248: // column_label
      case 249: // ident
      case 250: // datetime
      case 251: // string
      case 252: // number
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
#line 356 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 1136 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 363 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1142 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 364 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1148 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 365 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1154 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 366 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1160 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 367 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1166 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 368 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1172 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 369 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow stmt"); }
#line 1178 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 378 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1184 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 379 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1190 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 380 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1196 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 381 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1202 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 382 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1208 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 383 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1214 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 384 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1220 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 385 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1226 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 390 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1234 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 394 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1242 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 401 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1251 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 406 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1260 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 411 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1269 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 416 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1278 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 421 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1287 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 426 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1296 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 431 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1305 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 436 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1314 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 444 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1322 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 448 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1330 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 455 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1338 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 465 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 1357 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 482 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Hint > () = Hint(); }
#line 1363 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 484 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[1].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = false;
	}
#line 1373 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 490 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[2].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = true;
	}
#line 1383 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 499 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
	}
#line 1391 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 503 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[0].value.as< Vector<HintStmt_s> > ();
	}
#line 1399 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 510 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1410 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 517 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[1].value.as< Vector<HintStmt_s> > ();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1421 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 528 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(NO_REWRITE);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1430 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 533 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1439 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 538 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1448 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 543 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1457 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 548 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1466 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 553 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1475 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 558 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1484 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 563 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1493 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 568 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1502 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 574 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1514 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 582 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1526 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 590 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1538 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 598 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1550 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 606 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->set_is_ordered();
	}
#line 1561 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 613 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1574 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 623 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(PARALLEL);
		ParallelHintStmt_s hint = yylhs.value.as< HintStmt_s > ();
		hint->parallel = yystack_[1].value.as< int > ();
	}
#line 1584 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 631 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1590 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 633 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1598 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 639 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1604 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 641 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "";
	}
#line 1612 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 645 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > ();
	}
#line 1620 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 652 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1629 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 657 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1638 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 664 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1644 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 665 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1650 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 670 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = true;
		yylhs.value.as< LeadingTable_s > ()->table_name = yystack_[0].value.as< std::string > ();
	}
#line 1660 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 676 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = false;
		yylhs.value.as< LeadingTable_s > ()->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1670 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 685 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = Vector<LeadingTable_s>();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1679 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 690 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = yystack_[2].value.as< Vector<LeadingTable_s> > ();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1688 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 697 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1694 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 698 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1700 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 703 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1709 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 708 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yystack_[2].value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
    }
#line 1718 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 716 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = Vector<TableStmt_s>();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1727 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 721 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = yystack_[2].value.as< Vector<TableStmt_s> > ();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1736 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 728 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1742 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 729 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1748 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 733 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1754 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 734 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1760 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 738 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1766 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 739 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1772 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 743 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>(); }
#line 1778 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 744 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[0].value.as< Vector<OrderStmt_s> > (); }
#line 1784 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 749 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1793 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 754 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[2].value.as< Vector<OrderStmt_s> > ();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1802 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 762 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OrderStmt_s > () = OrderStmt::make_order_stmt(yystack_[1].value.as< ExprStmt_s > (), yystack_[0].value.as< bool > ());
    }
#line 1810 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 768 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1816 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 769 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1822 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 770 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1828 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 774 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< LimitStmt_s > () = NULL;}
#line 1834 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 776 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
    }
#line 1845 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 783 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
	}
#line 1856 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 793 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = yystack_[0].value.as< int > ();
	}
#line 1864 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 800 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< ExprStmt_s > ()->to_string();
    }
#line 1874 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 806 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1884 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 812 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[2].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1894 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 821 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< BasicTableStmt_s > ();
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
    }
#line 1903 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 829 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		yylhs.value.as< TableStmt_s > () = ViewTableStmt::make_view_table(yystack_[2].value.as< Stmt_s > ());
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1913 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 838 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), LeftOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1921 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 842 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), RightOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1929 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 846 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), FullOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1937 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 850 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), Inner, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1945 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 857 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 1953 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 861 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 1961 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 868 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1969 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 872 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1977 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 876 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< TableStmt_s > ();
	}
#line 1985 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 883 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1993 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 887 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2001 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 891 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2009 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 897 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 2015 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 899 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2023 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 903 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2031 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 915 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2039 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 919 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2047 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 923 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2055 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 927 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2063 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 931 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2071 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 938 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_day(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2085 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 948 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_month(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2099 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 958 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_year(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2113 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 971 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2121 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 975 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2130 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 980 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MINUS);
    }
#line 2139 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 985 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_ADD);
	}
#line 2148 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 990 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_SUB);
	}
#line 2157 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 995 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MUL);
	}
#line 2166 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 1000 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DIV);
	}
#line 2175 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1005 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_ADD);
  }
#line 2184 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1010 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_SUB);
  }
#line 2193 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1015 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2201 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1023 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2211 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1029 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2221 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1038 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2230 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1043 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), OP_IS_NULL);
    }
#line 2239 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1048 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), OP_IS_NOT_NULL);
    }
#line 2248 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1053 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[4].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_BETWEEN);
    }
#line 2257 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1058 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_BETWEEN);
    }
#line 2266 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1063 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2281 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1074 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2296 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1085 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_IN);
    }
#line 2305 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1090 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_IN);
    }
#line 2314 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1095 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EXISTS);
    }
#line 2322 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1099 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_EXISTS);
    }
#line 2331 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1104 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2340 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1112 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE;
	}
#line 2348 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1116 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT;
	}
#line 2356 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1120 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ;
	}
#line 2364 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1124 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE;
	}
#line 2372 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1128 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT;
	}
#line 2380 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1132 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE;
	}
#line 2388 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1136 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LIKE;
	}
#line 2396 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1140 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NOT_LIKE;
	}
#line 2404 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1147 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ANY;
	}
#line 2412 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1151 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ANY;
	}
#line 2420 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1155 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ANY;
	}
#line 2428 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1159 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ANY;
	}
#line 2436 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1163 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ANY;
	}
#line 2444 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1167 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ANY;
	}
#line 2452 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1171 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ALL;
	}
#line 2460 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1175 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ALL;
	}
#line 2468 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1179 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ALL;
	}
#line 2476 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1183 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ALL;
	}
#line 2484 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1187 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ALL;
	}
#line 2492 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1191 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ALL;
	}
#line 2500 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1198 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< ExprStmt_s > () = query_expr;
    }
#line 2510 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1207 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[4].value.as< ExprStmt_s > ());
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2521 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1214 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2531 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1223 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2541 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1229 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2551 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1238 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2561 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1244 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2571 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1253 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2579 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1257 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2587 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1261 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_AND);
    }
#line 2596 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1266 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_OR);
    }
#line 2605 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1274 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2616 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1281 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2626 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1287 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2637 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1295 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2648 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1302 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2660 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1310 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2672 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1321 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2685 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1330 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2702 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1343 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2715 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1352 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2728 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1364 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
  }
#line 2736 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1368 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_CHAR);
  }
#line 2744 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1372 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	Object_s value = Varchar::make_object(yystack_[1].value.as< std::string > ());
	check(value);
	ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
	check(stmt);
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), stmt, OP_TO_CHAR);
  }
#line 2756 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1380 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2772 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1392 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_IFNULL);
  }
#line 2780 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1396 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_NUMBER);
  }
#line 2788 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 1403 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s expr = AggrStmt::make_aggr_stmt();
		check(expr);
		expr->aggr_func = yystack_[3].value.as< AggrType > ();
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		expr->set_aggr_expr(col);
		yylhs.value.as< ExprStmt_s > () = expr;
	}
#line 2802 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 1413 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s expr = AggrStmt::make_aggr_stmt();
		check(expr);
		expr->aggr_func = yystack_[4].value.as< AggrType > ();
		expr->distinct = yystack_[2].value.as< bool > ();
		expr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
		yylhs.value.as< ExprStmt_s > () = expr;
    }
#line 2815 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 1425 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< AggrType > () = COUNT;
	}
#line 2823 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 1429 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< AggrType > () = SUM;
	}
#line 2831 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 1433 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< AggrType > () = AVG;
	}
#line 2839 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 1437 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< AggrType > () = MIN;
	}
#line 2847 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 1441 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< AggrType > () = MAX;
	}
#line 2855 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 1448 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
	}
#line 2863 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 1452 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 2871 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 1456 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 2879 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 1468 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		insert_stmt->value_list = yystack_[0].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2892 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 1477 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[1].value.as< BasicTableStmt_s > ();
		insert_stmt->query_values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2904 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 1488 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = Vector<Vector<ExprStmt_s>>();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
    }
#line 2913 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 204:
#line 1493 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = yystack_[4].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
	}
#line 2922 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 205:
#line 1501 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2931 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 206:
#line 1506 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2940 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 207:
#line 1519 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2955 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 208:
#line 1530 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2970 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 209:
#line 1544 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2980 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 210:
#line 1550 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2990 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 211:
#line 1559 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), col, yystack_[0].value.as< ExprStmt_s > (), OP_ASSIGN);
    }
#line 3001 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 212:
#line 1574 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3016 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 213:
#line 1585 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3031 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 214:
#line 1596 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		delete_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		delete_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 3045 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 215:
#line 1614 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		DMLStmt_s dml = yylhs.value.as< Stmt_s > ();
		dml->is_explain = true;
    }
#line 3055 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 216:
#line 1622 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3061 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 217:
#line 1623 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3067 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 218:
#line 1624 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3073 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 219:
#line 1625 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3079 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 220:
#line 1635 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateTable);
		check(cmd_stmt);
		cmd_stmt->params.create_table_params.table = yystack_[4].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.create_table_params.column_define_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		cmd_stmt->params.create_table_params.engine_args = yystack_[0].value.as< Vector<String> > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3092 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 221:
#line 1644 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3103 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 222:
#line 1651 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3118 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 223:
#line 1665 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
	}
#line 3126 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 224:
#line 1669 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[1].value.as< Vector<String> > ();
	}
#line 3134 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 225:
#line 1676 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3143 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 226:
#line 1681 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3152 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 227:
#line 1689 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.left_string();
	}
#line 3160 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 228:
#line 1696 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3169 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 229:
#line 1701 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3178 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 230:
#line 1709 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[2].value.as< std::string > (), yystack_[1].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
    }
#line 3188 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 231:
#line 1718 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 3196 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 232:
#line 1722 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		
	}
#line 3204 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 233:
#line 1729 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3210 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 234:
#line 1731 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3216 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 235:
#line 1733 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_MEDIUMINT_PREC, 0); }
#line 3222 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 236:
#line 1735 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3228 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 237:
#line 1737 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3234 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 238:
#line 1739 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
#line 3240 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 239:
#line 1741 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3246 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 240:
#line 1743 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3252 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 241:
#line 1745 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3258 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 242:
#line 1747 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3264 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 243:
#line 1749 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3270 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 244:
#line 1751 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3276 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 245:
#line 1753 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3282 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 246:
#line 1755 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3288 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 247:
#line 1757 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3294 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 248:
#line 1759 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3300 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 249:
#line 1761 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3306 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 250:
#line 1763 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3312 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 251:
#line 1765 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIMESTAMP, yystack_[0].value.as< int > ()); }
#line 3318 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 252:
#line 1767 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATETIME); }
#line 3324 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 253:
#line 1769 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATE); }
#line 3330 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 254:
#line 1771 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIME); }
#line 3336 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 255:
#line 1775 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_TIME_SCALE; }
#line 3342 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 256:
#line 1777 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		if (yystack_[1].value.as< int > () > MAX_TIME_SCALE) {
			yyerror("max time prec:%d", MAX_TIME_SCALE);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	}
#line 3354 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 257:
#line 1787 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_STR_LENGTH; }
#line 3360 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 258:
#line 1789 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		  if (yystack_[1].value.as< int > () > MAX_STR_LENGTH) {
			yyerror("max char length:%d", MAX_STR_LENGTH);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	  }
#line 3372 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 259:
#line 1800 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<String> > () = Vector<String>(); 
		yylhs.value.as< Vector<String> > ().push_back("CAT");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
	}
#line 3384 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 260:
#line 1808 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back("CSV");
		yylhs.value.as< Vector<String> > ().push_back(yystack_[10].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back(yystack_[5].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back("new line");
	}
#line 3396 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 261:
#line 1824 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_table_params.table = yystack_[0].value.as< TableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3408 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 262:
#line 1832 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3420 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 263:
#line 1840 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropView);
		check(cmd_stmt);
		cmd_stmt->params.drop_view_params.database = driver.get_global_database();
		cmd_stmt->params.drop_view_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_view_params.view_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3433 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 264:
#line 1851 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3439 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 265:
#line 1852 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3445 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 266:
#line 1862 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3456 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 267:
#line 1869 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
 	}
#line 3467 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 268:
#line 1876 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3478 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 269:
#line 1883 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3489 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 270:
#line 1890 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3500 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 271:
#line 1897 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3508 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 272:
#line 1901 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3516 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 273:
#line 1905 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3528 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 274:
#line 1913 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3540 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 275:
#line 1921 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowProcesslist);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3550 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 276:
#line 1927 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowMemory);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3560 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 277:
#line 1936 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 3568 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 278:
#line 1940 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 3576 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 279:
#line 1947 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(UseDatabase);
		check(cmd_stmt);
		cmd_stmt->params.use_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3587 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 280:
#line 1962 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3598 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 281:
#line 1969 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3609 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 282:
#line 1983 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = yystack_[1].value.as< std::string > ();
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3622 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 283:
#line 1992 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3635 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 284:
#line 2001 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3648 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 285:
#line 2012 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {	yylhs.value.as< double > () = 0.1; }
#line 3654 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 286:
#line 2014 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< double > () = std::stod(yystack_[0].value.as< std::string > ());
	}
#line 3662 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 287:
#line 2021 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(SetVar);
		check(cmd_stmt);
		cmd_stmt->params.set_var_params.var_name = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.set_var_params.var_value = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3674 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 288:
#line 2032 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		cmd_stmt->params.kill_params.thread_id = yystack_[0].value.as< int > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3685 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 289:
#line 2046 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3697 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 290:
#line 2054 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3709 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 291:
#line 2062 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3720 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 292:
#line 2071 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3726 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 293:
#line 2075 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3732 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 294:
#line 2079 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3738 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 295:
#line 2083 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 3744 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 296:
#line 2087 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3750 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 297:
#line 2091 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3756 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 298:
#line 2092 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3762 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 299:
#line 2096 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3768 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 300:
#line 2100 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3774 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 301:
#line 2105 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 3782 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;


#line 3786 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -502;

  const short int parser::yytable_ninf_ = -294;

  const short int
  parser::yypact_[] =
  {
     406,   -70,   -32,    31,    16,    16,    36,   121,   -51,    40,
     -38,     0,    57,   162,    31,    57,    99,    54,  -502,  -502,
     469,   472,  -502,   282,  -502,  -502,  -502,  -502,  -502,  -502,
    -502,  -502,  -502,  -502,  -502,  -502,     3,    57,    16,    57,
     210,   372,  -502,  -502,  -502,    84,  -502,    88,  -502,   132,
     132,   132,    31,  -502,  -502,  -502,  -502,  -502,    16,  -502,
    -502,  -502,   166,   126,   240,   318,   197,   267,  -502,   218,
     284,  -502,  -502,  -502,   277,   338,    16,  -502,  -502,  -502,
    -502,     6,   -38,   -38,    59,   -38,   -38,   300,   304,  -502,
     326,   336,   553,   339,   344,   339,   339,   339,   339,   345,
     350,   339,   339,   339,   355,   339,   339,   381,   400,   387,
     553,  -502,    16,   422,   425,    57,   441,    57,   241,    57,
      98,  -502,   386,  -502,   848,   483,    16,    16,   338,    16,
      16,    57,  -502,   392,   -38,  -502,  -502,  -502,  -502,   -38,
    -502,  -502,  -502,   418,   160,    57,    57,   488,   461,    42,
    -502,   498,  -502,  -502,  -502,  -502,   498,   498,  -502,  -502,
    -502,    40,  -502,  -502,   498,   498,  -502,  -502,   202,    16,
     241,  -502,  -502,  -502,  -502,   -38,  -502,  -502,  -502,   523,
     202,  -502,   454,  -502,  -502,  -502,  -502,   684,   483,   483,
     456,   766,   848,  -502,   848,  -502,   457,   466,   467,  -502,
    -502,  -502,  -502,  -502,  -502,    14,  -502,  -502,   259,  -502,
    -502,  -502,  -502,  -502,  -502,   470,   430,  -502,   448,  -502,
    -502,  -502,  -502,  -502,  -502,  -502,  -502,  -502,  -502,    57,
    -502,  -502,   445,   445,   445,    21,  -502,   925,  -502,    50,
    -502,  -502,   509,   455,  -502,  -502,   198,    57,    57,   458,
      57,    57,    57,   423,  -502,   202,   -27,   523,   464,   475,
     500,   475,   475,  -502,   848,   543,   673,   -25,    68,  -502,
    -502,   848,   493,   390,  -502,  -502,   848,   848,   848,   848,
     241,    57,   848,   818,   848,   818,  -502,  -502,   203,   109,
      -7,  -502,   578,   473,  -502,  -502,  -502,    57,   531,  -502,
    -502,   507,   507,  -502,  -502,   512,  -502,  -502,  -502,  -502,
    -502,   514,  -502,  -502,   515,   515,  -502,   507,   507,   595,
      57,  -502,   -38,  -502,  -502,   198,  -502,   118,  -502,   142,
    -502,   153,  -502,   154,   167,  -502,   673,  -502,   423,   241,
    -502,   202,  -502,   524,   288,   525,   528,   325,   169,   517,
     548,   529,   551,  1060,  -502,     8,   848,    74,   848,   673,
     357,  -502,   358,   170,   437,  -502,   -27,  -502,  -502,   483,
    -502,    -6,  -502,  -502,    -6,  -502,  -502,   497,   848,  -502,
    -502,  -502,   241,    57,  -502,   848,    40,  -502,   601,  -502,
      40,  -502,  -502,    40,    40,    40,  -502,  -502,  -502,  -502,
     599,  -502,  -502,  -502,   177,  -502,  -502,   198,  -502,    57,
    -502,  -502,  -502,   269,  -502,   523,  -502,   288,   -16,  -502,
    -502,   518,   288,   288,   848,  -502,   848,   529,  -502,  1006,
      39,   533,   243,   327,   401,   426,   432,   436,  -502,   848,
     534,   172,   848,   529,   673,   673,   848,   273,   848,   848,
     316,    10,   848,    40,   483,  -502,  -502,   555,   -29,  -502,
     438,   -27,  -502,   325,  -502,   580,   511,   586,   587,   516,
    -502,  -502,  -502,  -502,   526,   -38,   519,   523,   673,   527,
     530,   325,   187,  -502,  -502,  -502,   848,   544,  -502,  -502,
    -502,  -502,  -502,  -502,  -502,  -502,  -502,  -502,  -502,  -502,
      -2,   766,  -502,   615,  -502,   325,  -502,   630,  -502,   325,
     848,   393,   274,  -502,   848,   486,   600,   532,   604,   571,
    -502,  -502,  -502,  -502,  -502,   605,  -502,    40,    40,  -502,
     673,   541,  -502,   269,   673,   673,  -502,    81,   766,  -502,
     848,   325,   199,  -502,   325,  -502,   848,   325,  -502,    40,
    -502,   848,   673,   558,   584,   549,   552,   269,   320,   269,
     269,   848,   206,   325,   848,  -502,   325,   554,   622,   269,
     635,   588,   654,  -502,  -502,   325,  -502,   325,  -502,   848,
      40,  -502,   483,   631,  -502,   265,   640,  -502,   643,   848,
    -502,  -502,  -502,    40,   610,  -502,  -502,   563,   658,   483,
     647,   603,   581,   659,   483,  -502
  };

  const unsigned short int
  parser::yydefact_[] =
  {
       9,     0,     0,    32,     0,     0,     0,     0,     0,     0,
       0,    32,     0,     0,    32,     0,     0,     0,     3,     4,
      19,     0,    29,    28,     5,     6,     7,     8,    11,    12,
      10,    14,    13,    15,    16,    17,     0,     0,     0,     0,
      35,     0,   296,   291,   281,     0,   289,   293,   280,   264,
     264,   264,    32,   216,   218,   219,   217,   215,     0,   300,
     301,   288,     0,     0,    68,     0,     0,     0,   267,     0,
       0,   276,   275,   272,     0,   278,     0,   279,   292,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,   223,    35,    57,     0,    57,    57,    57,    57,     0,
       0,    57,    57,    57,     0,    57,    57,     0,     0,     0,
      36,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,     0,    69,     0,     0,     0,     0,   278,     0,
       0,     0,   269,     0,     0,    21,    28,    27,    25,     0,
      20,    26,    24,     0,     0,     0,     0,     0,     0,     0,
      46,    55,    42,    39,    41,    43,    55,    55,    45,    47,
      52,     0,    44,    40,    55,    55,    33,    38,   109,     0,
       0,   290,   293,   265,   262,     0,   106,   107,   108,   261,
     109,   263,     0,   202,   266,   184,   299,     0,     0,     0,
       0,     0,     0,   176,     0,   179,     0,     0,     0,   194,
     193,   195,   196,   197,   164,     0,    70,   120,    92,   116,
     129,   112,   113,   115,   185,     0,     0,   175,   294,   182,
     181,   183,   287,   274,   270,   268,   271,   273,   277,     0,
      23,    22,   285,   285,   285,     0,   228,     0,   294,     0,
     225,   227,     0,     0,    58,    56,     0,     0,     0,     0,
       0,     0,     0,    75,   110,   109,    75,    72,     0,   101,
       0,   101,   101,    95,     0,   201,     0,     0,     0,   298,
     297,     0,   164,     0,   122,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,   295,   198,     0,
      75,   209,     0,     0,   284,   283,   282,     0,   259,   243,
     238,   257,   257,   253,   252,   239,   246,   244,   236,   237,
     235,   240,   245,   234,   255,   255,   233,   257,   257,   231,
       0,   224,     0,    34,    59,     0,    66,    62,    64,    62,
      60,    62,    54,    62,    62,   111,     0,   212,    75,     0,
     214,   109,   102,     0,     0,     0,     0,   205,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,   114,     0,     0,     0,    71,    75,    94,   126,     0,
     128,   124,   125,   127,   123,   199,   200,     0,     0,   178,
     180,   177,     0,     0,   207,     0,     0,   229,     0,   220,
       0,   248,   247,     0,     0,     0,   254,   251,   250,   249,
       0,   230,   226,   222,    62,    63,    53,     0,    49,     0,
      51,    48,    50,    74,   213,    73,    96,     0,     0,   103,
     104,     0,     0,     0,     0,   203,     0,     0,   141,     0,
       0,     0,   144,   145,   146,   148,   147,   149,   150,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   186,   190,    77,     0,   191,
       0,    75,   210,   211,   286,     0,     0,     0,     0,     0,
     232,    65,    67,    61,     0,     0,   108,     0,     0,     0,
       0,   206,     0,   142,   172,   151,     0,     0,   158,   152,
     159,   153,   160,   154,   162,   156,   161,   155,   163,   157,
       0,     0,   139,     0,   133,   132,   143,   174,   173,   169,
       0,     0,     0,   166,     0,     0,     0,     0,     0,    79,
     117,   118,   119,   192,   208,     0,   258,     0,     0,   256,
       0,     0,   105,   100,     0,     0,   204,     0,     0,   140,
       0,   130,     0,   134,   167,   165,     0,   170,   189,     0,
     187,     0,     0,    80,     0,     0,     0,    99,   109,    97,
      98,     0,     0,   135,     0,   137,   168,     0,    76,    78,
       0,    88,     0,   241,   242,   136,   138,   131,   188,     0,
       0,    31,     0,    81,    82,    85,    90,    91,     0,     0,
      86,    87,    84,     0,     0,    83,    89,     0,     0,     0,
       0,     0,     0,     0,     0,   260
  };

  const short int
  parser::yypgoto_[] =
  {
    -502,  -502,  -502,  -502,    41,  -502,   451,    20,   480,   175,
     613,  -502,   602,   314,   652,    82,  -275,   303,   388,  -502,
    -502,  -261,  -246,  -502,  -502,  -502,  -502,   122,  -502,  -502,
     124,   435,  -340,  -163,   297,    78,  -189,  -115,   -82,  -502,
     439,   230,  -501,  -502,  -502,  -502,  -330,  -502,  -502,  -502,
    -294,  -502,  -502,  -502,  -502,  -502,  -502,   714,  -502,   296,
     716,  -502,   342,   719,  -502,  -502,  -502,  -502,  -502,  -502,
    -502,   431,  -502,  -502,   412,   -98,  -502,  -502,   310,  -502,
     606,  -502,  -502,  -502,   137,  -502,  -502,   261,    38,    18,
    -121,   452,    -3,  -502,  -119,    -9,    -1
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    16,    17,    18,    62,    20,    21,   204,    23,    64,
     109,   110,   111,   246,   150,   329,   409,   326,   327,   124,
     205,   256,   337,   519,   553,   571,   583,   584,   592,   581,
     586,   206,   176,   177,   178,   343,   421,   257,   416,   207,
     370,   353,   542,   354,   442,   443,   209,   210,   357,   268,
     355,   211,   212,   213,   214,   215,   378,    24,   265,   348,
      25,   290,   291,    26,    27,    57,    28,   147,   239,   322,
     235,   236,   401,   319,   396,   391,   389,    29,   117,    30,
     132,    31,    32,    33,   294,    34,    35,   180,    45,   216,
     217,   286,   218,   219,   220,   221,   587
  };

  const short int
  parser::yytable_[] =
  {
      60,    47,    47,   179,   419,   540,   222,    42,    61,    65,
     340,    37,    78,   339,   444,   445,   444,   445,   520,   366,
      22,   428,    46,    46,   237,   240,    42,    22,   282,    58,
      22,    40,   134,    78,    78,    47,    91,   562,    47,    42,
      43,    19,   413,    63,   384,   444,   445,   282,    53,    10,
     568,   282,   407,    77,   279,    47,    46,    36,   430,    46,
     382,   297,    40,   521,    59,   451,   283,   243,   284,   269,
     270,   475,    43,    47,    88,    89,    46,   419,    52,    49,
      42,   280,   419,   419,   285,   139,   253,   284,   561,   283,
     320,   284,   414,    10,    46,    38,    87,   483,   263,    79,
      52,    22,    22,    22,    22,    22,    22,   285,   292,    47,
     502,    39,   172,   506,    78,    47,   181,   522,   336,   356,
     457,   461,    52,    47,    47,   358,    47,    47,    78,   407,
      46,   448,    42,   171,   282,   298,    46,   446,   336,   514,
      22,   172,   238,   238,    46,    46,    10,    46,    46,   383,
     507,   508,    60,   484,    22,   174,   244,   539,   405,    22,
     249,   183,   234,    50,   321,   172,    47,    47,   381,   228,
       3,    80,   283,   338,   284,    52,   237,   172,    41,    51,
     503,   420,   405,    42,   533,    10,   254,    46,    46,    76,
     285,   504,   115,   405,   405,    22,  -292,     8,   254,   402,
      66,    67,   379,   116,   392,   287,    68,   405,    10,   424,
     454,   272,   359,   122,    52,   524,   258,   405,   449,   398,
     399,    42,   380,   282,   415,    42,   238,   424,   474,   375,
      69,   252,   406,   479,   480,    70,   557,    52,   182,   564,
     559,   560,    92,   328,   330,   330,   564,   330,   330,   172,
     458,    71,   172,   233,   420,   376,   408,    14,   569,   420,
     420,   283,   292,   284,    42,    44,    48,   410,   411,   488,
     335,   489,    72,   254,    93,   444,   445,    47,   287,   285,
     121,   412,    42,   425,   455,   325,   238,    73,   281,    74,
      75,   471,   123,    94,   238,   590,   377,    43,    46,    90,
      95,   536,   114,   477,    96,    97,    98,    99,   100,   101,
     102,    42,   282,   565,   103,   591,   104,   238,   282,   120,
     576,   126,   328,   -30,   -30,   -30,   282,   282,   175,   125,
     331,   105,   333,   334,   127,   517,    47,   133,   172,   345,
     346,    47,    22,    42,    43,   106,   128,   107,   108,   252,
     283,   129,   284,   490,   208,   491,   283,    46,   284,   254,
     118,   119,    46,   403,   283,   283,   284,   284,   285,   282,
     295,   296,   272,   168,   285,   418,   513,   464,   282,    47,
     238,    60,   285,   285,    60,    60,    60,   223,   224,   466,
     226,   227,   467,   468,   469,    42,   -18,   452,   453,   -18,
      46,   130,   510,   546,   328,   131,   473,   283,   143,   284,
     282,   282,   144,   145,    47,    47,   283,   267,   284,    47,
      47,   273,   274,   146,   275,   285,   149,   492,    43,   493,
     255,   151,   156,     1,   285,    46,    46,   157,    22,   112,
      46,    46,   161,   282,   516,   166,   282,     2,   283,   283,
     284,   284,   494,   545,   495,     3,     4,     5,   496,   258,
     497,     6,   498,   588,   499,   113,   285,   285,   164,     7,
     247,   248,    81,    82,    83,    84,    85,    86,   250,   251,
     600,   283,     8,   284,   283,   605,   284,   165,     9,   169,
     282,   282,   170,    10,   347,    22,   -29,   -29,   -29,   285,
     184,   360,   285,   173,   361,   186,   362,   363,   364,   208,
     229,   232,   368,   371,   372,   374,   531,   241,    60,    60,
     242,   272,    11,   245,    12,    13,   555,   556,   283,   283,
     284,   284,   135,   137,   138,   140,   141,   142,   289,   282,
     567,   264,    14,   271,   276,    15,   285,   285,   485,   486,
     487,   456,   523,   277,   278,   172,  -293,   288,   272,   350,
     293,   136,   136,   136,   136,   136,   136,   323,   336,   324,
     185,    60,   332,   186,    42,    59,   254,   283,   341,   284,
     342,   344,   429,   349,    60,   230,   447,   187,   450,   385,
     231,   259,   388,   386,   390,   285,   188,   189,   260,   393,
     548,   394,   395,   400,   426,   417,   422,   261,   460,   423,
     427,   459,   465,   351,   136,   463,    10,    93,   470,   136,
     478,   501,   525,   190,   518,   526,   527,   528,   530,   534,
     529,   538,   535,   532,   543,   262,    94,   445,   352,   551,
     549,   552,   192,    95,   193,   554,   550,    96,    97,    98,
      99,   100,   101,   102,   481,   558,   347,   103,   572,   104,
     194,   570,   564,   573,   195,   582,   574,    52,   578,   500,
     579,   589,   505,   580,   105,   597,   509,   196,   511,   512,
     593,   350,   515,   594,   197,   198,   598,   601,   106,   602,
     107,   108,   185,   599,   604,   186,    42,    59,   199,   200,
     201,   202,   203,   185,   603,   148,   186,    42,    59,   187,
     472,   595,   167,   404,   365,   476,   537,   596,   188,   189,
     187,    54,   482,    55,   373,   462,    56,   397,   387,   188,
     189,   541,     0,   367,   225,   351,     0,     0,     0,     0,
     544,     0,     0,     0,   547,   190,     0,   152,   153,   154,
     155,     0,     0,   158,   159,   160,   190,   162,   163,     0,
     352,     0,     0,     0,   192,     0,   193,     0,   541,     0,
     563,   191,     0,     0,     0,   192,   566,   193,     0,     0,
       0,   541,   194,     0,     0,   185,   195,     0,   186,    42,
      59,   575,     0,   194,   577,     0,     0,   195,     0,   196,
       0,     0,   187,     0,     0,     0,   197,   198,     0,   585,
     196,   188,   189,     0,     0,     0,     0,   197,   198,   585,
     199,   200,   201,   202,   203,     0,     0,     0,   266,     0,
       0,   199,   200,   201,   202,   203,     0,   185,   190,     0,
     186,    42,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,   187,     0,     0,   192,     0,   193,
       0,     0,     0,   188,   189,     0,     0,   185,     0,     0,
     186,    42,    59,     0,     0,   194,     0,     0,     0,   195,
       0,     0,    52,     0,   187,     0,     0,     0,     0,     0,
     190,     0,   196,   188,   189,     0,   369,     0,     0,   197,
     198,     0,     0,     0,     0,   191,     0,     0,     0,   192,
       0,   193,     0,   199,   200,   201,   202,   203,     0,     0,
     190,     0,     0,     0,     0,     0,     0,   194,     0,     0,
       0,   195,     0,     0,     0,   191,     0,     0,     0,   192,
       0,   193,     0,     0,   196,   299,     0,     0,     0,     0,
       0,   197,   198,     0,     0,     0,     0,   194,   300,   301,
       0,   195,   302,     0,     0,   199,   200,   201,   202,   203,
     303,   304,     0,   305,   196,     0,     0,     0,     0,   306,
       0,   197,   198,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,   199,   200,   201,   202,   203,
       0,     0,   308,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,   310,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,     0,   314,   315,   316,     0,   282,
       0,     0,     0,     0,     0,     0,   317,   318,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   283,     0,   284,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   282,     0,   285,     0,     0,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   283,     0,   284,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   285
  };

  const short int
  parser::yycheck_[] =
  {
       9,     4,     5,   118,   344,     7,   125,    23,     9,    12,
     256,    43,    15,    40,     6,     7,     6,     7,    47,   280,
       0,   351,     4,     5,   145,   146,    23,     7,    53,    80,
      10,    31,    26,    36,    37,    38,    39,   538,    41,    23,
      56,     0,   336,    43,   290,     6,     7,    53,     7,    87,
     551,    53,   327,    15,    40,    58,    38,   127,   352,    41,
      67,    40,    31,    92,    24,   359,    91,    25,    93,   188,
     189,    87,    56,    76,    36,    37,    58,   417,   116,    43,
      23,    67,   422,   423,   109,    26,   168,    93,     7,    91,
      40,    93,   338,    87,    76,   127,    93,   427,   180,     0,
     116,    81,    82,    83,    84,    85,    86,   109,   229,   112,
     440,   143,   115,   443,   117,   118,   119,   146,   145,   144,
     366,   382,   116,   126,   127,    57,   129,   130,   131,   404,
     112,    57,    23,   115,    53,   114,   118,   129,   145,   129,
     120,   144,   145,   146,   126,   127,    87,   129,   130,   156,
     444,   445,   161,   114,   134,   117,   114,   487,    40,   139,
     161,   120,   144,   127,   114,   168,   169,   170,   289,   131,
      49,   117,    91,   255,    93,   116,   297,   180,     3,   143,
       8,   344,    40,    23,   478,    87,   168,   169,   170,    14,
     109,    19,   108,    40,    40,   175,   108,    76,   180,   320,
      38,    39,    93,    71,   302,   208,    44,    40,    87,    40,
      40,   191,   144,    87,   116,   461,   175,    40,   144,   317,
     318,    23,   113,    53,   339,    23,   229,    40,   417,    26,
      68,    29,   114,   422,   423,    73,   530,   116,   140,    40,
     534,   535,    32,   246,   247,   248,    40,   250,   251,   252,
     369,    89,   255,    93,   417,    52,   114,   136,   552,   422,
     423,    91,   383,    93,    23,     4,     5,   114,   114,    26,
     252,    28,   110,   255,    64,     6,     7,   280,   281,   109,
     114,   114,    23,   114,   114,    87,   289,   125,    29,   127,
     128,   114,    52,    83,   297,    30,    93,    56,   280,    38,
      90,   114,    41,   418,    94,    95,    96,    97,    98,    99,
     100,    23,    53,   114,   104,    50,   106,   320,    53,    58,
     114,   124,   325,     3,     4,     5,    53,    53,    87,    11,
     248,   121,   250,   251,    67,   454,   339,    76,   341,   261,
     262,   344,   322,    23,    56,   135,   128,   137,   138,    29,
      91,    67,    93,    26,   124,    28,    91,   339,    93,   341,
      50,    51,   344,   322,    91,    91,    93,    93,   109,    53,
     233,   234,   352,   112,   109,    87,    60,   386,    53,   382,
     383,   390,   109,   109,   393,   394,   395,   126,   127,   390,
     129,   130,   393,   394,   395,    23,   114,    40,    40,   117,
     382,   124,   129,   129,   407,    67,   409,    91,   108,    93,
      53,    53,   108,    87,   417,   418,    91,   187,    93,   422,
     423,   191,   192,    87,   194,   109,    87,    26,    56,    28,
     169,    87,    87,    27,   109,   417,   418,    87,   418,    67,
     422,   423,    87,    53,   453,    58,    53,    41,    91,    91,
      93,    93,    26,    60,    28,    49,    50,    51,    26,   418,
      28,    55,    26,   582,    28,    93,   109,   109,    87,    63,
     156,   157,     3,     4,     5,     3,     4,     5,   164,   165,
     599,    91,    76,    93,    91,   604,    93,    87,    82,    67,
      53,    53,    67,    87,   264,   475,     3,     4,     5,   109,
     114,   271,   109,    62,   114,    22,   276,   277,   278,   279,
     118,    93,   282,   283,   284,   285,   475,    29,   527,   528,
      59,   501,   116,    25,   118,   119,   527,   528,    91,    91,
      93,    93,    81,    82,    83,    84,    85,    86,   108,    53,
     549,    87,   136,    87,    87,   139,   109,   109,    15,    16,
      17,   114,   114,    87,    87,   558,   108,    87,   538,     8,
     115,    81,    82,    83,    84,    85,    86,    58,   145,   114,
      19,   580,   114,    22,    23,    24,   558,    91,   114,    93,
     105,    81,   352,    40,   593,   134,   356,    36,   358,    11,
     139,    68,    61,   120,    87,   109,    45,    46,    75,    87,
     114,    87,    87,     8,    87,    81,    81,    84,   378,    81,
      62,   114,    11,    62,   134,   385,    87,    64,    19,   139,
     102,    87,    42,    72,    69,   114,    40,    40,   102,   102,
     114,    87,   102,   114,    19,   112,    83,     7,    87,    35,
      40,    70,    91,    90,    93,    40,   114,    94,    95,    96,
      97,    98,    99,   100,   424,   114,   426,   104,    74,   106,
     109,   103,    40,   114,   113,    11,   114,   116,   114,   439,
      35,    40,   442,    85,   121,    65,   446,   126,   448,   449,
      40,     8,   452,    40,   133,   134,   123,    40,   135,    86,
     137,   138,    19,    35,    35,    22,    23,    24,   147,   148,
     149,   150,   151,    19,   123,    92,    22,    23,    24,    36,
     407,   589,   110,   325,   279,   418,   486,   593,    45,    46,
      36,     7,   426,     7,   285,   383,     7,   315,   297,    45,
      46,   501,    -1,   281,   128,    62,    -1,    -1,    -1,    -1,
     510,    -1,    -1,    -1,   514,    72,    -1,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,    72,   105,   106,    -1,
      87,    -1,    -1,    -1,    91,    -1,    93,    -1,   538,    -1,
     540,    87,    -1,    -1,    -1,    91,   546,    93,    -1,    -1,
      -1,   551,   109,    -1,    -1,    19,   113,    -1,    22,    23,
      24,   561,    -1,   109,   564,    -1,    -1,   113,    -1,   126,
      -1,    -1,    36,    -1,    -1,    -1,   133,   134,    -1,   579,
     126,    45,    46,    -1,    -1,    -1,    -1,   133,   134,   589,
     147,   148,   149,   150,   151,    -1,    -1,    -1,   144,    -1,
      -1,   147,   148,   149,   150,   151,    -1,    19,    72,    -1,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    36,    -1,    -1,    91,    -1,    93,
      -1,    -1,    -1,    45,    46,    -1,    -1,    19,    -1,    -1,
      22,    23,    24,    -1,    -1,   109,    -1,    -1,    -1,   113,
      -1,    -1,   116,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      72,    -1,   126,    45,    46,    -1,    78,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    93,    -1,   147,   148,   149,   150,   151,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    93,    -1,    -1,   126,    20,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,    -1,    -1,    -1,   109,    33,    34,
      -1,   113,    37,    -1,    -1,   147,   148,   149,   150,   151,
      45,    46,    -1,    48,   126,    -1,    -1,    -1,    -1,    54,
      -1,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,   147,   148,   149,   150,   151,
      -1,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,   109,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    27,    41,    49,    50,    51,    55,    63,    76,    82,
      87,   116,   118,   119,   136,   139,   158,   159,   160,   161,
     162,   163,   164,   165,   214,   217,   220,   221,   223,   234,
     236,   238,   239,   240,   242,   243,   127,    43,   127,   143,
      31,   166,    23,    56,   244,   245,   246,   249,   244,    43,
     127,   143,   116,   161,   214,   217,   220,   222,    80,    24,
     252,   253,   161,    43,   166,   249,    38,    39,    44,    68,
      73,    89,   110,   125,   127,   128,   166,   245,   249,     0,
     117,     3,     4,     5,     3,     4,     5,    93,   245,   245,
     244,   249,    32,    64,    83,    90,    94,    95,    96,    97,
      98,    99,   100,   104,   106,   121,   135,   137,   138,   167,
     168,   169,    67,    93,   244,   108,    71,   235,   235,   235,
     244,   114,    87,    52,   176,    11,   124,    67,   128,    67,
     124,    67,   237,   244,    26,   163,   165,   163,   163,    26,
     163,   163,   163,   108,   108,    87,    87,   224,   167,    87,
     171,    87,   171,   171,   171,   171,    87,    87,   171,   171,
     171,    87,   171,   171,    87,    87,    58,   169,   244,    67,
      67,   246,   249,    62,   245,    87,   189,   190,   191,   194,
     244,   249,   140,   161,   114,    19,    22,    36,    45,    46,
      72,    87,    91,    93,   109,   113,   126,   133,   134,   147,
     148,   149,   150,   151,   164,   177,   188,   196,   198,   203,
     204,   208,   209,   210,   211,   212,   246,   247,   249,   250,
     251,   252,   251,   244,   244,   237,   244,   244,   245,   118,
     163,   163,    93,    93,   246,   227,   228,   247,   249,   225,
     247,    29,    59,    25,   114,    25,   170,   170,   170,   253,
     170,   170,    29,   195,   246,   244,   178,   194,   161,    68,
      75,    84,   112,   195,    87,   215,   144,   198,   206,   251,
     251,    87,   164,   198,   198,   198,    87,    87,    87,    40,
      67,    29,    53,    91,    93,   109,   248,   249,    87,   108,
     218,   219,   247,   115,   241,   241,   241,    40,   114,    20,
      33,    34,    37,    45,    46,    48,    54,    66,    77,    79,
      88,   101,   111,   122,   130,   131,   132,   141,   142,   230,
      40,   114,   226,    58,   114,    87,   174,   175,   249,   172,
     249,   172,   114,   172,   172,   246,   145,   179,   195,    40,
     179,   114,   105,   192,    81,   192,   192,   198,   216,    40,
       8,    62,    87,   198,   200,   207,   144,   205,    57,   144,
     198,   114,   198,   198,   198,   188,   178,   248,   198,    78,
     197,   198,   198,   197,   198,    26,    52,    93,   213,    93,
     113,   247,    67,   156,   179,    11,   120,   228,    61,   233,
      87,   232,   232,    87,    87,    87,   231,   231,   232,   232,
       8,   229,   247,   161,   175,    40,   114,   173,   114,   173,
     114,   114,   114,   207,   179,   194,   195,    81,    87,   189,
     190,   193,    81,    81,    40,   114,    87,    62,   203,   198,
     207,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,   201,   202,     6,     7,   129,   198,    57,   144,
     198,   207,    40,    40,    40,   114,   114,   179,   251,   114,
     198,   178,   219,   198,   252,    11,   253,   253,   253,   253,
      19,   114,   174,   249,   193,    87,   191,   194,   102,   193,
     193,   198,   216,   203,   114,    15,    16,    17,    26,    28,
      26,    28,    26,    28,    26,    28,    26,    28,    26,    28,
     198,    87,   203,     8,    19,   198,   203,   207,   207,   198,
     129,   198,   198,    60,   129,   198,   252,   251,    69,   180,
      47,    92,   146,   114,   179,    42,   114,    40,    40,   114,
     102,   161,   114,   207,   102,   102,   114,   198,    87,   203,
       7,   198,   199,    19,   198,    60,   129,   198,   114,    40,
     114,    35,    70,   181,    40,   253,   253,   207,   114,   207,
     207,     7,   199,   198,    40,   114,   198,   252,   199,   207,
     103,   182,    74,   114,   114,   198,   114,   198,   114,    35,
      85,   186,    11,   183,   184,   198,   187,   253,   251,    40,
      30,    50,   185,    40,    40,   184,   187,    65,   123,    35,
     251,    40,    86,   123,    35,   251
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   157,   158,   159,   159,   159,   159,   159,   159,   159,
     160,   160,   160,   160,   160,   160,   160,   160,   161,   161,
     162,   162,   162,   162,   162,   162,   162,   162,   163,   163,
     164,   165,   166,   166,   166,   167,   167,   168,   168,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   170,   170,   171,   171,   171,
     172,   172,   173,   173,   174,   174,   175,   175,   176,   176,
     177,   177,   178,   178,   179,   179,   180,   180,   181,   181,
     182,   182,   183,   183,   184,   185,   185,   185,   186,   186,
     186,   187,   188,   188,   188,   189,   190,   191,   191,   191,
     191,   192,   192,   193,   193,   193,   194,   194,   194,   195,
     195,   195,   196,   196,   196,   196,   196,   197,   197,   197,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     199,   199,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   201,   201,   201,   201,   201,   201,
     201,   201,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   203,   204,   204,   205,   205,   206,
     206,   207,   207,   207,   207,   208,   208,   208,   208,   208,
     208,   209,   209,   209,   209,   210,   210,   210,   210,   210,
     210,   211,   211,   212,   212,   212,   212,   212,   213,   213,
     213,   214,   214,   215,   215,   216,   216,   217,   217,   218,
     218,   219,   220,   220,   220,   221,   222,   222,   222,   222,
     223,   223,   223,   224,   224,   225,   225,   226,   227,   227,
     228,   229,   229,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   231,   231,   232,   232,   233,
     233,   234,   234,   234,   235,   235,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   237,   237,   238,
     239,   239,   240,   240,   240,   241,   241,   242,   243,   244,
     244,   244,   245,   246,   247,   248,   249,   250,   250,   251,
     252,   253
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     4,     4,     3,     3,     3,     3,     1,     1,
       3,    11,     0,     3,     5,     0,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     5,     5,
       5,     5,     2,     5,     4,     0,     1,     0,     2,     3,
       1,     3,     0,     1,     1,     3,     1,     3,     0,     1,
       1,     3,     1,     3,     2,     0,     3,     0,     2,     0,
       0,     3,     1,     3,     2,     0,     1,     1,     0,     4,
       2,     1,     1,     2,     3,     2,     4,     7,     7,     7,
       6,     0,     1,     1,     1,     3,     1,     1,     1,     0,
       1,     2,     1,     1,     3,     1,     1,     3,     3,     3,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     1,
       1,     3,     3,     3,     4,     5,     6,     5,     6,     3,
       4,     2,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     6,     5,     4,     5,     4,
       5,     1,     3,     3,     3,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     1,     1,     4,     6,     8,     6,
       4,     4,     5,     1,     1,     1,     1,     1,     0,     1,
       1,     5,     4,     3,     5,     1,     3,     6,     8,     1,
       3,     3,     6,     7,     6,     2,     1,     1,     1,     1,
       7,     3,     7,     0,     3,     1,     3,     0,     1,     3,
       3,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     6,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     1,     1,     2,     0,     3,     0,     3,     0,
      17,     4,     4,     4,     0,     2,     4,     2,     4,     3,
       4,     4,     2,     4,     4,     2,     2,     2,     0,     2,
       2,     2,     6,     6,     6,     0,     3,     4,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     2,     2,     1,
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
  "EXPLAIN", "EXPR_NORMALIZE", "FIELD", "FLOAT", "FROM", "FULL", "GROUP",
  "HAVING", "IF", "IFNULL", "INDEX", "INFILE", "INNER", "INSERT", "INT",
  "INTERVAL", "INTEGER", "INTO", "JOIN", "KILL", "LEADING", "LEFT",
  "LIMIT", "LINE", "\"(\"", "MEDIUMINT", "MEMORY", "MERGE", "\"-\"",
  "MONTH", "\"*\"", "NO_REWRITE", "NO_UNNEST", "NO_MERGE", "NO_USE_HASH",
  "NO_USE_NL", "NO_SIMPLIFY_SQ", "NO_EXPR_NORMALIZE", "NUMERIC_SYM", "ON",
  "ORDER", "ORDERED", "OUTER", "PARALLEL", "PARTITION", "\".\"", "\"+\"",
  "PROCESSLIST", "REAL", "RIGHT", "ROWID", "\")\"", "SAMPLE", "SELECT",
  "\";\"", "SET", "SHOW", "SIZE", "SIMPLIFY_SQ", "SMALLINT", "SPLIT",
  "STATIS", "STATUS", "SUBSTR", "TABLE", "TABLES", "THEN", "TIME",
  "TIMESTAMP_SYM", "TINYINT", "TO_CHAR", "TO_NUMBER", "UNNEST", "UPDATE",
  "USE_HASH", "USE_NL", "USING", "VALUES", "VARBINARY", "VARCHAR", "VIEW",
  "WHEN", "WHERE", "YEAR", "SUM", "COUNT", "AVG", "MIN", "MAX", "RANK",
  "DENSE_RANK", "ROW_NUMBER", "OVER", "','", "$accept", "sql_stmt", "stmt",
  "cmd_stmt", "select_stmt", "set_select", "sub_set_select",
  "select_with_parens", "simple_select", "opt_hint", "opt_hint_list",
  "hint_list", "single_hint", "opt_qb_name", "opt_qb_name_single",
  "hint_table_list", "opt_split", "leading_hint_table",
  "leading_hint_table_list", "opt_distinct", "select_expr_list",
  "from_list", "opt_where", "opt_groupby", "opt_having", "opt_order_by",
  "order_by_list", "order_by", "opt_asc_desc", "opt_select_limit",
  "limit_expr", "projection", "basic_table_factor", "view_table_factor",
  "joined_table_factor", "opt_outer", "sub_table_factor", "table_factor",
  "opt_alias", "simple_expr", "seconds_expr", "arith_expr",
  "arith_expr_list", "cmp_expr", "cmp_type", "sq_cmp_type",
  "query_ref_expr", "case_when_expr", "when_then_list1", "when_then_list2",
  "logical_expr", "column_ref", "expr_const", "func_expr", "aggr_expr",
  "aggr_type", "distinct_or_all", "insert_stmt", "insert_value_list",
  "insert_value", "update_stmt", "update_asgn_list", "update_asgn_factor",
  "delete_stmt", "explain_stmt", "explainable_stmt", "create_stmt",
  "opt_view_column_define", "view_column_define", "beg_view_define",
  "table_element_list", "column_definition", "opt_not_null", "data_type",
  "opt_time_precision", "opt_char_length", "opt_engine_def", "drop_stmt",
  "opt_if_exists", "show_stmt", "op_from_database", "use_stmt",
  "desc_stmt", "analyze_stmt", "opt_sample_size", "set_var_stmt",
  "kill_stmt", "relation_factor", "database_name", "relation_name",
  "column_name", "column_label", "ident", "datetime", "string", "number",
  "int_value", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   355,   355,   363,   364,   365,   366,   367,   368,   369,
     378,   379,   380,   381,   382,   383,   384,   385,   389,   393,
     400,   405,   410,   415,   420,   425,   430,   435,   443,   447,
     454,   461,   482,   483,   489,   499,   502,   509,   516,   527,
     532,   537,   542,   547,   552,   557,   562,   567,   573,   581,
     589,   597,   605,   612,   622,   631,   632,   639,   640,   644,
     651,   656,   664,   665,   669,   675,   684,   689,   697,   698,
     702,   707,   715,   720,   728,   729,   733,   734,   738,   739,
     743,   744,   748,   753,   761,   768,   769,   770,   774,   775,
     782,   792,   799,   805,   811,   820,   828,   837,   841,   845,
     849,   857,   860,   867,   871,   875,   882,   886,   890,   897,
     898,   902,   914,   918,   922,   926,   930,   937,   947,   957,
     970,   974,   979,   984,   989,   994,   999,  1004,  1009,  1014,
    1022,  1028,  1037,  1042,  1047,  1052,  1057,  1062,  1073,  1084,
    1089,  1094,  1098,  1103,  1111,  1115,  1119,  1123,  1127,  1131,
    1135,  1139,  1146,  1150,  1154,  1158,  1162,  1166,  1170,  1174,
    1178,  1182,  1186,  1190,  1197,  1206,  1213,  1222,  1228,  1237,
    1243,  1252,  1256,  1260,  1265,  1273,  1280,  1286,  1294,  1301,
    1309,  1320,  1329,  1342,  1351,  1363,  1367,  1371,  1379,  1391,
    1395,  1402,  1412,  1424,  1428,  1432,  1436,  1440,  1448,  1451,
    1455,  1467,  1476,  1487,  1492,  1500,  1505,  1518,  1529,  1543,
    1549,  1558,  1573,  1584,  1595,  1613,  1622,  1623,  1624,  1625,
    1634,  1643,  1650,  1665,  1668,  1675,  1680,  1689,  1695,  1700,
    1708,  1718,  1721,  1728,  1730,  1732,  1734,  1736,  1738,  1740,
    1742,  1744,  1746,  1748,  1750,  1752,  1754,  1756,  1758,  1760,
    1762,  1764,  1766,  1768,  1770,  1775,  1776,  1787,  1788,  1800,
    1807,  1823,  1831,  1839,  1851,  1852,  1861,  1868,  1875,  1882,
    1889,  1896,  1900,  1904,  1912,  1920,  1926,  1935,  1940,  1946,
    1961,  1968,  1982,  1991,  2000,  2012,  2013,  2020,  2031,  2045,
    2053,  2061,  2071,  2075,  2079,  2083,  2087,  2091,  2092,  2096,
    2100,  2104
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
#line 4714 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:1167
#line 2108 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
