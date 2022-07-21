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
      case 245: // relation_factor
        value.move< BasicTableStmt_s > (that.value);
        break;

      case 229: // column_definition
        value.move< ColumnDefineStmt_s > (that.value);
        break;

      case 231: // data_type
        value.move< DataType > (that.value);
        break;

      case 184: // order_by
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
      case 213: // opt_arith_expr
      case 220: // update_asgn_factor
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

      case 158: // sql_stmt
      case 159: // stmt
      case 160: // cmd_stmt
      case 161: // select_stmt
      case 162: // set_select
      case 163: // sub_set_select
      case 164: // select_with_parens
      case 165: // simple_select
      case 215: // insert_stmt
      case 218: // update_stmt
      case 221: // delete_stmt
      case 222: // explain_stmt
      case 223: // explainable_stmt
      case 224: // create_stmt
      case 235: // drop_stmt
      case 237: // show_stmt
      case 239: // use_stmt
      case 240: // desc_stmt
      case 241: // analyze_stmt
      case 243: // set_var_stmt
      case 244: // kill_stmt
        value.move< Stmt_s > (that.value);
        break;

      case 189: // basic_table_factor
      case 190: // view_table_factor
      case 191: // joined_table_factor
      case 193: // sub_table_factor
      case 194: // table_factor
        value.move< TableStmt_s > (that.value);
        break;

      case 228: // table_element_list
        value.move< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 177: // select_expr_list
      case 179: // opt_where
      case 180: // opt_groupby
      case 181: // opt_having
      case 182: // opt_order_by
      case 183: // order_by_list
      case 199: // arith_expr_list
      case 205: // when_then_list1
      case 206: // when_then_list2
      case 214: // opt_partition_by
      case 217: // insert_value
      case 219: // update_asgn_list
        value.move< Vector<ExprStmt_s> > (that.value);
        break;

      case 167: // opt_hint_list
      case 168: // hint_list
        value.move< Vector<HintStmt_s> > (that.value);
        break;

      case 175: // leading_hint_table_list
        value.move< Vector<LeadingTable_s> > (that.value);
        break;

      case 172: // hint_table_list
      case 225: // opt_view_column_define
      case 226: // view_column_define
      case 234: // opt_engine_def
        value.move< Vector<String> > (that.value);
        break;

      case 178: // from_list
        value.move< Vector<TableStmt_s> > (that.value);
        break;

      case 216: // insert_value_list
        value.move< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 212: // win_type
        value.move< WinType > (that.value);
        break;

      case 173: // opt_split
      case 176: // opt_distinct
      case 185: // opt_asc_desc
      case 192: // opt_outer
      case 211: // distinct_or_all
      case 230: // opt_not_null
      case 236: // opt_if_exists
        value.move< bool > (that.value);
        break;

      case 242: // opt_sample_size
        value.move< double > (that.value);
        break;

      case 187: // limit_expr
      case 232: // opt_time_precision
      case 233: // opt_char_length
      case 254: // int_value
        value.move< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 170: // opt_qb_name
      case 171: // opt_qb_name_single
      case 195: // opt_alias
      case 227: // beg_view_define
      case 238: // op_from_database
      case 246: // database_name
      case 247: // relation_name
      case 248: // column_name
      case 249: // column_label
      case 250: // ident
      case 251: // datetime
      case 252: // string
      case 253: // number
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
      case 245: // relation_factor
        value.copy< BasicTableStmt_s > (that.value);
        break;

      case 229: // column_definition
        value.copy< ColumnDefineStmt_s > (that.value);
        break;

      case 231: // data_type
        value.copy< DataType > (that.value);
        break;

      case 184: // order_by
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
      case 213: // opt_arith_expr
      case 220: // update_asgn_factor
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

      case 158: // sql_stmt
      case 159: // stmt
      case 160: // cmd_stmt
      case 161: // select_stmt
      case 162: // set_select
      case 163: // sub_set_select
      case 164: // select_with_parens
      case 165: // simple_select
      case 215: // insert_stmt
      case 218: // update_stmt
      case 221: // delete_stmt
      case 222: // explain_stmt
      case 223: // explainable_stmt
      case 224: // create_stmt
      case 235: // drop_stmt
      case 237: // show_stmt
      case 239: // use_stmt
      case 240: // desc_stmt
      case 241: // analyze_stmt
      case 243: // set_var_stmt
      case 244: // kill_stmt
        value.copy< Stmt_s > (that.value);
        break;

      case 189: // basic_table_factor
      case 190: // view_table_factor
      case 191: // joined_table_factor
      case 193: // sub_table_factor
      case 194: // table_factor
        value.copy< TableStmt_s > (that.value);
        break;

      case 228: // table_element_list
        value.copy< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 177: // select_expr_list
      case 179: // opt_where
      case 180: // opt_groupby
      case 181: // opt_having
      case 182: // opt_order_by
      case 183: // order_by_list
      case 199: // arith_expr_list
      case 205: // when_then_list1
      case 206: // when_then_list2
      case 214: // opt_partition_by
      case 217: // insert_value
      case 219: // update_asgn_list
        value.copy< Vector<ExprStmt_s> > (that.value);
        break;

      case 167: // opt_hint_list
      case 168: // hint_list
        value.copy< Vector<HintStmt_s> > (that.value);
        break;

      case 175: // leading_hint_table_list
        value.copy< Vector<LeadingTable_s> > (that.value);
        break;

      case 172: // hint_table_list
      case 225: // opt_view_column_define
      case 226: // view_column_define
      case 234: // opt_engine_def
        value.copy< Vector<String> > (that.value);
        break;

      case 178: // from_list
        value.copy< Vector<TableStmt_s> > (that.value);
        break;

      case 216: // insert_value_list
        value.copy< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 212: // win_type
        value.copy< WinType > (that.value);
        break;

      case 173: // opt_split
      case 176: // opt_distinct
      case 185: // opt_asc_desc
      case 192: // opt_outer
      case 211: // distinct_or_all
      case 230: // opt_not_null
      case 236: // opt_if_exists
        value.copy< bool > (that.value);
        break;

      case 242: // opt_sample_size
        value.copy< double > (that.value);
        break;

      case 187: // limit_expr
      case 232: // opt_time_precision
      case 233: // opt_char_length
      case 254: // int_value
        value.copy< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 170: // opt_qb_name
      case 171: // opt_qb_name_single
      case 195: // opt_alias
      case 227: // beg_view_define
      case 238: // op_from_database
      case 246: // database_name
      case 247: // relation_name
      case 248: // column_name
      case 249: // column_label
      case 250: // ident
      case 251: // datetime
      case 252: // string
      case 253: // number
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
      case 245: // relation_factor
        yylhs.value.build< BasicTableStmt_s > ();
        break;

      case 229: // column_definition
        yylhs.value.build< ColumnDefineStmt_s > ();
        break;

      case 231: // data_type
        yylhs.value.build< DataType > ();
        break;

      case 184: // order_by
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
      case 213: // opt_arith_expr
      case 220: // update_asgn_factor
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

      case 158: // sql_stmt
      case 159: // stmt
      case 160: // cmd_stmt
      case 161: // select_stmt
      case 162: // set_select
      case 163: // sub_set_select
      case 164: // select_with_parens
      case 165: // simple_select
      case 215: // insert_stmt
      case 218: // update_stmt
      case 221: // delete_stmt
      case 222: // explain_stmt
      case 223: // explainable_stmt
      case 224: // create_stmt
      case 235: // drop_stmt
      case 237: // show_stmt
      case 239: // use_stmt
      case 240: // desc_stmt
      case 241: // analyze_stmt
      case 243: // set_var_stmt
      case 244: // kill_stmt
        yylhs.value.build< Stmt_s > ();
        break;

      case 189: // basic_table_factor
      case 190: // view_table_factor
      case 191: // joined_table_factor
      case 193: // sub_table_factor
      case 194: // table_factor
        yylhs.value.build< TableStmt_s > ();
        break;

      case 228: // table_element_list
        yylhs.value.build< Vector<ColumnDefineStmt_s> > ();
        break;

      case 177: // select_expr_list
      case 179: // opt_where
      case 180: // opt_groupby
      case 181: // opt_having
      case 182: // opt_order_by
      case 183: // order_by_list
      case 199: // arith_expr_list
      case 205: // when_then_list1
      case 206: // when_then_list2
      case 214: // opt_partition_by
      case 217: // insert_value
      case 219: // update_asgn_list
        yylhs.value.build< Vector<ExprStmt_s> > ();
        break;

      case 167: // opt_hint_list
      case 168: // hint_list
        yylhs.value.build< Vector<HintStmt_s> > ();
        break;

      case 175: // leading_hint_table_list
        yylhs.value.build< Vector<LeadingTable_s> > ();
        break;

      case 172: // hint_table_list
      case 225: // opt_view_column_define
      case 226: // view_column_define
      case 234: // opt_engine_def
        yylhs.value.build< Vector<String> > ();
        break;

      case 178: // from_list
        yylhs.value.build< Vector<TableStmt_s> > ();
        break;

      case 216: // insert_value_list
        yylhs.value.build< Vector<Vector<ExprStmt_s>> > ();
        break;

      case 212: // win_type
        yylhs.value.build< WinType > ();
        break;

      case 173: // opt_split
      case 176: // opt_distinct
      case 185: // opt_asc_desc
      case 192: // opt_outer
      case 211: // distinct_or_all
      case 230: // opt_not_null
      case 236: // opt_if_exists
        yylhs.value.build< bool > ();
        break;

      case 242: // opt_sample_size
        yylhs.value.build< double > ();
        break;

      case 187: // limit_expr
      case 232: // opt_time_precision
      case 233: // opt_char_length
      case 254: // int_value
        yylhs.value.build< int > ();
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 170: // opt_qb_name
      case 171: // opt_qb_name_single
      case 195: // opt_alias
      case 227: // beg_view_define
      case 238: // op_from_database
      case 246: // database_name
      case 247: // relation_name
      case 248: // column_name
      case 249: // column_label
      case 250: // ident
      case 251: // datetime
      case 252: // string
      case 253: // number
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
#line 354 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 1121 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 361 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1127 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 362 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1133 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 363 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1139 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 364 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1145 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 365 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1151 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 366 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1157 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 367 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow stmt"); }
#line 1163 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 376 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1169 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 377 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1175 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 378 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1181 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 379 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1187 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 380 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1193 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 381 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1199 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 382 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1205 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 383 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1211 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 388 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 392 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1227 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 399 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1236 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 404 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1245 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 409 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1254 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 414 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1263 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 419 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1272 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 424 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1281 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 429 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1290 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 434 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1299 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 442 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1307 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 446 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1315 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 453 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1323 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 463 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
		select_stmt->order_exprs = yystack_[1].value.as< Vector<ExprStmt_s> > ();
		select_stmt->limit_stmt = yystack_[0].value.as< LimitStmt_s > ();
		yylhs.value.as< Stmt_s > () = select_stmt;
    }
#line 1342 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 480 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Hint > () = Hint(); }
#line 1348 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 482 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[1].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = false;
	}
#line 1358 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 488 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[2].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = true;
	}
#line 1368 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 497 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
	}
#line 1376 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 501 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[0].value.as< Vector<HintStmt_s> > ();
	}
#line 1384 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 508 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1395 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 515 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[1].value.as< Vector<HintStmt_s> > ();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1406 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 526 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(NO_REWRITE);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1415 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 531 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1424 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 536 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1433 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 541 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1442 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 546 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1451 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 551 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1460 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 556 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1469 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 561 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1478 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 566 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1487 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 572 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1499 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 580 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1511 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 588 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1523 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 596 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1535 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 604 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->set_is_ordered();
	}
#line 1546 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 611 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1559 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 621 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(PARALLEL);
		ParallelHintStmt_s hint = yylhs.value.as< HintStmt_s > ();
		hint->parallel = yystack_[1].value.as< int > ();
	}
#line 1569 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 629 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1575 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 631 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1583 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 637 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1589 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 639 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "";
	}
#line 1597 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 643 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > ();
	}
#line 1605 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 650 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1614 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 655 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1623 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 662 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1629 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 663 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1635 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 668 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = true;
		yylhs.value.as< LeadingTable_s > ()->table_name = yystack_[0].value.as< std::string > ();
	}
#line 1645 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 674 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = false;
		yylhs.value.as< LeadingTable_s > ()->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1655 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 683 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = Vector<LeadingTable_s>();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1664 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 688 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = yystack_[2].value.as< Vector<LeadingTable_s> > ();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1673 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 695 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1679 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 696 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1685 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 701 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1694 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 706 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yystack_[2].value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
    }
#line 1703 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 714 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = Vector<TableStmt_s>();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1712 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 719 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = yystack_[2].value.as< Vector<TableStmt_s> > ();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1721 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 726 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1727 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 727 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1733 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 731 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1739 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 732 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1745 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 736 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1751 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 737 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1757 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 741 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1763 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 742 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1769 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 747 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 1778 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 752 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 1787 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 760 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OrderStmt::make_order_stmt(yystack_[1].value.as< ExprStmt_s > (), yystack_[0].value.as< bool > ());
    }
#line 1795 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 766 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1801 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 767 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1807 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 768 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1813 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 772 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< LimitStmt_s > () = NULL;}
#line 1819 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 774 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
    }
#line 1830 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 781 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
	}
#line 1841 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 791 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = yystack_[0].value.as< int > ();
	}
#line 1849 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 798 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< ExprStmt_s > ()->to_string();
    }
#line 1859 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 804 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1869 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 810 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[2].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1879 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 819 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< BasicTableStmt_s > ();
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
    }
#line 1888 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 824 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		TableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		yylhs.value.as< TableStmt_s > () = table;
	}
#line 1899 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 834 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		yylhs.value.as< TableStmt_s > () = ViewTableStmt::make_view_table(yystack_[2].value.as< Stmt_s > ());
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1909 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 843 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), LeftOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1917 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 847 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), RightOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1925 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 851 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), FullOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1933 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 855 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), Inner, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1941 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 862 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 1949 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 866 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 1957 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 873 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1965 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 877 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 1973 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 881 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< TableStmt_s > ();
	}
#line 1981 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 888 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1989 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 892 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1997 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 896 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2005 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 902 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 2011 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 904 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2019 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 908 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2027 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 920 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2035 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 924 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2043 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 928 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2051 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 932 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2059 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 936 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2067 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 943 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_day(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2081 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 953 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_month(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2095 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 963 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_year(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2109 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 976 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2117 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 980 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2126 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 985 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MINUS);
    }
#line 2135 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 990 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_ADD);
	}
#line 2144 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 995 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_SUB);
	}
#line 2153 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 1000 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MUL);
	}
#line 2162 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1005 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DIV);
	}
#line 2171 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1010 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_ADD);
  }
#line 2180 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1015 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_SUB);
  }
#line 2189 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1020 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2197 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1028 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2207 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1034 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2217 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1043 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2226 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1048 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), OP_IS_NULL);
    }
#line 2235 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1053 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), OP_IS_NOT_NULL);
    }
#line 2244 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1058 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[4].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_BETWEEN);
    }
#line 2253 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1063 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_BETWEEN);
    }
#line 2262 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1068 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2277 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1079 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2292 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1090 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_IN);
    }
#line 2301 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1095 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_IN);
    }
#line 2310 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1100 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EXISTS);
    }
#line 2318 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1104 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_EXISTS);
    }
#line 2327 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1109 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2336 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1117 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE;
	}
#line 2344 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1121 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT;
	}
#line 2352 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1125 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ;
	}
#line 2360 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1129 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE;
	}
#line 2368 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1133 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT;
	}
#line 2376 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1137 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE;
	}
#line 2384 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1141 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LIKE;
	}
#line 2392 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1145 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NOT_LIKE;
	}
#line 2400 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1152 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ANY;
	}
#line 2408 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1156 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ANY;
	}
#line 2416 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1160 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ANY;
	}
#line 2424 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1164 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ANY;
	}
#line 2432 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1168 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ANY;
	}
#line 2440 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1172 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ANY;
	}
#line 2448 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1176 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ALL;
	}
#line 2456 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1180 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ALL;
	}
#line 2464 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1184 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ALL;
	}
#line 2472 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1188 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ALL;
	}
#line 2480 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1192 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ALL;
	}
#line 2488 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1196 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ALL;
	}
#line 2496 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1203 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< ExprStmt_s > () = query_expr;
    }
#line 2506 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1212 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[4].value.as< ExprStmt_s > ());
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2517 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1219 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2527 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1228 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2537 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1234 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2547 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1243 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2557 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1249 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2567 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1258 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2575 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1262 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2583 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1266 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_AND);
    }
#line 2592 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1271 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_OR);
    }
#line 2601 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1279 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2612 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1286 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2622 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1292 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2633 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1300 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2644 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1307 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2656 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1315 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2668 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1326 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2681 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1335 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2698 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1348 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2711 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1357 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2724 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1369 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	if (!yystack_[1].value.as< ExprStmt_s > ()) {
		yyerror("expect aggr expr");
		YYABORT;
	}
	AggrType type;
	if (WIN_COUNT == yystack_[4].value.as< WinType > ()) {
		type = COUNT;
	} else if (WIN_SUM == yystack_[4].value.as< WinType > ()) {
		type = SUM;
	} else if (WIN_AVG == yystack_[4].value.as< WinType > ()) {
		type = AVG;
	} else if (WIN_MIN == yystack_[4].value.as< WinType > ()) {
		type = MIN;
	} else if (WIN_MAX == yystack_[4].value.as< WinType > ()) {
		type = MAX;
	}
	AggrStmt_s expr = AggrStmt::make_aggr_stmt();
	check(expr);
	expr->aggr_func = type;
	expr->distinct = yystack_[2].value.as< bool > ();
	expr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
	yylhs.value.as< ExprStmt_s > () = expr;
  }
#line 2753 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1394 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	WinExprStmt_s expr = WinExprStmt::make_win_expr_stmt(yystack_[9].value.as< WinType > ());
	check(expr);
	expr->is_distinct = yystack_[7].value.as< bool > ();
	if (yystack_[6].value.as< ExprStmt_s > ()) {
		expr->set_win_func_expr(yystack_[6].value.as< ExprStmt_s > ());
	}
	expr->set_win_part_by_exprs(yystack_[2].value.as< Vector<ExprStmt_s> > ());
	expr->set_win_order_by_exprs(yystack_[1].value.as< Vector<ExprStmt_s> > ());
	yylhs.value.as< ExprStmt_s > () = expr;
  }
#line 2769 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1406 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_CHAR);
  }
#line 2777 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1410 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	Object_s value = Varchar::make_object(yystack_[1].value.as< std::string > ());
	check(value);
	ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
	check(stmt);
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), stmt, OP_TO_CHAR);
  }
#line 2789 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1418 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2805 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 1430 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_IFNULL);
  }
#line 2813 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 1434 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_NUMBER);
  }
#line 2821 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 1442 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
	}
#line 2829 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 1446 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 2837 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 1450 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 2845 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 1457 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_COUNT;
	}
#line 2853 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 1461 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_SUM;
	}
#line 2861 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 1465 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_AVG;
	}
#line 2869 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 1469 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_MIN;
	}
#line 2877 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 1473 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_MAX;
	}
#line 2885 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 1477 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_RANK;
	}
#line 2893 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 1481 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_DENSE_RANK;
	}
#line 2901 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 1485 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_ROW_NUMBER;
	}
#line 2909 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 204:
#line 1492 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = ExprStmt_s();
	}
#line 2917 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 205:
#line 1496 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2925 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 206:
#line 1503 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
	}
#line 2933 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 207:
#line 1507 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > ();
	}
#line 2941 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 208:
#line 1519 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		insert_stmt->value_list = yystack_[0].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2954 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 209:
#line 1528 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[1].value.as< BasicTableStmt_s > ();
		insert_stmt->query_values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2966 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 210:
#line 1539 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = Vector<Vector<ExprStmt_s>>();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
    }
#line 2975 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 211:
#line 1544 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = yystack_[4].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
	}
#line 2984 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 212:
#line 1552 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2993 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 213:
#line 1557 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3002 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 214:
#line 1570 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3017 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 215:
#line 1581 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3032 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 216:
#line 1595 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3042 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 217:
#line 1601 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3052 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 218:
#line 1610 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), col, yystack_[0].value.as< ExprStmt_s > (), OP_ASSIGN);
    }
#line 3063 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 219:
#line 1625 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3078 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 220:
#line 1636 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3093 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 221:
#line 1647 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		delete_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		delete_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 3107 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 222:
#line 1665 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		DMLStmt_s dml = yylhs.value.as< Stmt_s > ();
		dml->is_explain = true;
    }
#line 3117 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 223:
#line 1673 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3123 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 224:
#line 1674 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3129 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 225:
#line 1675 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3135 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 226:
#line 1676 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3141 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 227:
#line 1686 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateTable);
		check(cmd_stmt);
		cmd_stmt->params.create_table_params.table = yystack_[4].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.create_table_params.column_define_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		cmd_stmt->params.create_table_params.engine_args = yystack_[0].value.as< Vector<String> > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3154 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 228:
#line 1695 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3165 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 229:
#line 1702 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3180 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 230:
#line 1716 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
	}
#line 3188 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 231:
#line 1720 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[1].value.as< Vector<String> > ();
	}
#line 3196 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 232:
#line 1727 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3205 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 233:
#line 1732 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3214 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 234:
#line 1740 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.left_string();
	}
#line 3222 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 235:
#line 1747 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3231 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 236:
#line 1752 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3240 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 237:
#line 1760 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[2].value.as< std::string > (), yystack_[1].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
    }
#line 3250 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 238:
#line 1769 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 3258 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 239:
#line 1773 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		
	}
#line 3266 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 240:
#line 1780 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3272 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 241:
#line 1782 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3278 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 242:
#line 1784 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_MEDIUMINT_PREC, 0); }
#line 3284 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 243:
#line 1786 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3290 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 244:
#line 1788 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3296 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 245:
#line 1790 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
#line 3302 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 246:
#line 1792 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3308 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 247:
#line 1794 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3314 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 248:
#line 1796 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3320 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 249:
#line 1798 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3326 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 250:
#line 1800 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3332 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 251:
#line 1802 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3338 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 252:
#line 1804 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3344 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 253:
#line 1806 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3350 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 254:
#line 1808 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3356 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 255:
#line 1810 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3362 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 256:
#line 1812 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3368 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 257:
#line 1814 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3374 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 258:
#line 1816 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIMESTAMP, yystack_[0].value.as< int > ()); }
#line 3380 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 259:
#line 1818 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATETIME); }
#line 3386 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 260:
#line 1820 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATE); }
#line 3392 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 261:
#line 1822 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIME); }
#line 3398 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 262:
#line 1826 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_TIME_SCALE; }
#line 3404 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 263:
#line 1828 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		if (yystack_[1].value.as< int > () > MAX_TIME_SCALE) {
			yyerror("max time prec:%d", MAX_TIME_SCALE);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	}
#line 3416 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 264:
#line 1838 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_STR_LENGTH; }
#line 3422 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 265:
#line 1840 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		  if (yystack_[1].value.as< int > () > MAX_STR_LENGTH) {
			yyerror("max char length:%d", MAX_STR_LENGTH);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	  }
#line 3434 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 266:
#line 1851 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<String> > () = Vector<String>(); 
		yylhs.value.as< Vector<String> > ().push_back("CAT");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
	}
#line 3446 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 267:
#line 1859 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back("CSV");
		yylhs.value.as< Vector<String> > ().push_back(yystack_[10].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back(yystack_[5].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back("new line");
	}
#line 3458 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 268:
#line 1875 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_table_params.table = yystack_[0].value.as< TableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3470 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 269:
#line 1883 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3482 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 270:
#line 1891 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropView);
		check(cmd_stmt);
		cmd_stmt->params.drop_view_params.database = driver.get_global_database();
		cmd_stmt->params.drop_view_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_view_params.view_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3495 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 271:
#line 1902 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3501 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 272:
#line 1903 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3507 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 273:
#line 1913 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3518 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 274:
#line 1920 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
 	}
#line 3529 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 275:
#line 1927 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3540 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 276:
#line 1934 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3551 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 277:
#line 1941 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3562 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 278:
#line 1948 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3570 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 279:
#line 1952 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3578 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 280:
#line 1956 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3590 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 281:
#line 1964 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3602 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 282:
#line 1972 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowProcesslist);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3612 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 283:
#line 1978 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowMemory);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3622 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 284:
#line 1987 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 3630 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 285:
#line 1991 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 3638 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 286:
#line 1998 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(UseDatabase);
		check(cmd_stmt);
		cmd_stmt->params.use_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3649 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 287:
#line 2013 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3660 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 288:
#line 2020 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3671 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 289:
#line 2034 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = yystack_[1].value.as< std::string > ();
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3684 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 290:
#line 2043 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3697 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 291:
#line 2052 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3710 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 292:
#line 2063 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {	yylhs.value.as< double > () = 0.1; }
#line 3716 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 293:
#line 2065 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< double > () = std::stod(yystack_[0].value.as< std::string > ());
	}
#line 3724 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 294:
#line 2072 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(SetVar);
		check(cmd_stmt);
		cmd_stmt->params.set_var_params.var_name = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.set_var_params.var_value = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3736 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 295:
#line 2083 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		cmd_stmt->params.kill_params.session_id = yystack_[0].value.as< int > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3747 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 296:
#line 2097 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3759 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 297:
#line 2105 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3771 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 298:
#line 2115 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3777 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 299:
#line 2119 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3783 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 300:
#line 2123 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3789 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 301:
#line 2127 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 3795 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 302:
#line 2131 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3801 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 303:
#line 2135 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3807 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 304:
#line 2136 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3813 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 305:
#line 2140 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3819 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 306:
#line 2144 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3825 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 307:
#line 2149 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 3833 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;


#line 3837 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -359;

  const short int parser::yytable_ninf_ = -300;

  const short int
  parser::yypact_[] =
  {
     362,   -80,    -6,    32,    16,    16,    30,   217,    10,   139,
     -37,    43,    16,   229,    32,    16,    87,    67,  -359,  -359,
     451,   487,  -359,   -85,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,   194,    16,    16,    16,
     231,   253,  -359,  -359,    72,  -359,    85,  -359,   126,   126,
     126,    32,  -359,  -359,  -359,  -359,  -359,    16,  -359,  -359,
    -359,   107,   145,   174,   227,   124,   198,  -359,   158,   222,
    -359,  -359,  -359,   168,   243,    16,  -359,  -359,  -359,  -359,
      63,   -37,   -37,    91,   -37,   -37,   207,   237,  -359,   247,
     268,   999,   276,   301,   276,   276,   276,   276,   322,   356,
     276,   276,   276,   359,   276,   276,   386,   387,   346,   999,
    -359,    16,   412,   415,    16,   421,    16,    -5,    16,   182,
    -359,   373,  -359,   901,   466,    16,    16,   243,    16,    16,
      16,  -359,   381,   -37,  -359,  -359,  -359,  -359,   -37,  -359,
    -359,  -359,   422,   290,    16,    16,   496,   457,   -15,  -359,
     506,  -359,  -359,  -359,  -359,   506,   506,  -359,  -359,  -359,
     139,  -359,  -359,   506,   506,  -359,  -359,   177,    16,    -5,
    -359,  -359,  -359,  -359,  -359,   -37,  -359,  -359,  -359,   248,
     177,  -359,   449,  -359,  -359,  -359,  -359,   670,   466,   466,
     453,   806,   901,  -359,   901,  -359,   473,   475,   481,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,    13,  -359,
    -359,   208,  -359,  -359,  -359,  -359,  -359,   486,   430,  -359,
     436,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,    16,  -359,  -359,   462,   462,   462,   -23,  -359,   991,
    -359,   -16,  -359,  -359,   518,   476,  -359,  -359,    58,    16,
      16,   478,    16,    16,    16,   435,  -359,   177,   -36,   248,
     479,   482,   500,   482,   482,  -359,   901,   549,   659,   165,
     -14,  -359,  -359,   901,   562,   339,  -359,  -359,   901,   901,
     901,   901,    -5,    16,   901,   817,   901,   817,  -359,  -359,
      31,    69,   125,  -359,   583,   480,  -359,  -359,  -359,    16,
     534,  -359,  -359,   509,   509,  -359,  -359,   511,  -359,  -359,
    -359,  -359,  -359,   512,  -359,  -359,   515,   515,  -359,   509,
     509,   597,    16,  -359,   -37,  -359,  -359,    58,  -359,     6,
    -359,   143,  -359,   148,  -359,   150,   163,  -359,   659,  -359,
     435,    -5,  -359,   177,  -359,   525,   149,   526,   527,   -25,
     164,   524,   558,   535,   510,   967,  -359,     7,   901,     5,
     901,   659,   331,  -359,   366,   309,   433,  -359,   -36,  -359,
    -359,   466,  -359,   -28,  -359,  -359,   -28,  -359,  -359,   901,
    -359,  -359,  -359,    -5,    16,  -359,   901,   139,  -359,   610,
    -359,   139,  -359,  -359,   139,   139,   139,  -359,  -359,  -359,
    -359,   605,  -359,  -359,  -359,   169,  -359,  -359,    58,  -359,
      16,  -359,  -359,  -359,   335,  -359,   248,  -359,   149,     8,
    -359,  -359,   528,   149,   149,   901,  -359,   901,   535,  -359,
     495,    54,   568,   347,   371,   407,   424,   467,   474,  -359,
     901,   540,    59,   901,   535,   659,   659,   901,   305,   901,
     901,   461,     9,   901,   139,   466,  -359,  -359,   560,   -40,
     -25,   517,   -36,  -359,   -25,  -359,   591,   523,   599,   600,
     537,  -359,  -359,  -359,  -359,   539,   -37,   538,   248,   659,
     551,   552,   -25,   180,  -359,  -359,  -359,   901,   555,  -359,
    -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,  -359,
    -359,   121,   806,  -359,   636,  -359,   -25,  -359,   649,  -359,
     -25,   901,   470,   338,  -359,   901,   444,   625,   554,   634,
     603,  -359,  -359,  -359,   519,  -359,   630,  -359,   139,   139,
    -359,   659,   561,  -359,   335,   659,   659,  -359,   162,   806,
    -359,   901,   -25,   189,  -359,   -25,  -359,   901,   -25,  -359,
     139,  -359,   901,   659,   573,   590,   606,   570,   571,   335,
     546,   335,   335,   901,   193,   -25,   901,  -359,   -25,   572,
     648,   335,   644,   612,   584,   679,  -359,  -359,   -25,  -359,
     -25,  -359,   901,   139,  -359,   664,   573,   466,   661,  -359,
     317,   662,  -359,   901,   593,   668,   901,  -359,  -359,  -359,
     139,   648,  -359,   645,  -359,  -359,   586,   676,   466,   672,
     627,   594,   683,   466,  -359
  };

  const unsigned short int
  parser::yydefact_[] =
  {
       9,     0,     0,    32,     0,     0,     0,     0,     0,     0,
       0,    32,     0,     0,    32,     0,     0,     0,     3,     4,
      19,     0,    29,    28,     5,     6,     7,     8,    11,    12,
      10,    14,    13,    15,    16,    17,     0,     0,     0,     0,
      35,     0,   302,   288,     0,   296,   299,   287,   271,   271,
     271,    32,   223,   225,   226,   224,   222,     0,   306,   307,
     295,     0,     0,    68,     0,     0,     0,   274,     0,     0,
     283,   282,   279,     0,   285,     0,   286,   298,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,   228,     0,
     230,    35,    57,     0,    57,    57,    57,    57,     0,     0,
      57,    57,    57,     0,    57,    57,     0,     0,     0,    36,
      37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,    69,     0,     0,     0,     0,   285,     0,     0,
       0,   276,     0,     0,    21,    28,    27,    25,     0,    20,
      26,    24,     0,     0,     0,     0,     0,     0,     0,    46,
      55,    42,    39,    41,    43,    55,    55,    45,    47,    52,
       0,    44,    40,    55,    55,    33,    38,   110,     0,     0,
     297,   299,   272,   269,    96,     0,   107,   108,   109,   268,
     110,   270,     0,   209,   273,   185,   305,     0,     0,     0,
       0,     0,     0,   177,     0,   180,     0,     0,     0,   197,
     196,   198,   199,   200,   201,   202,   203,   165,     0,    70,
     121,    92,   117,   130,   113,   114,   116,     0,     0,   176,
     300,   183,   182,   184,   294,   281,   277,   275,   278,   280,
     284,     0,    23,    22,   292,   292,   292,     0,   235,     0,
     300,     0,   232,   234,     0,     0,    58,    56,     0,     0,
       0,     0,     0,     0,     0,    75,   111,   110,    75,    72,
       0,   102,     0,   102,   102,    95,     0,   208,     0,     0,
       0,   304,   303,     0,   165,     0,   123,   122,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,   301,
     193,     0,    75,   216,     0,     0,   291,   290,   289,     0,
     266,   250,   245,   264,   264,   260,   259,   246,   253,   251,
     243,   244,   242,   247,   252,   241,   262,   262,   240,   264,
     264,   238,     0,   231,     0,    34,    59,     0,    66,    62,
      64,    62,    60,    62,    54,    62,    62,   112,     0,   219,
      75,     0,   221,   110,   103,     0,     0,     0,     0,   212,
       0,     0,     0,     0,     0,     0,   172,     0,     0,     0,
       0,     0,     0,   115,     0,     0,     0,    71,    75,    94,
     127,     0,   129,   125,   126,   128,   124,   194,   195,   204,
     179,   181,   178,     0,     0,   214,     0,     0,   236,     0,
     227,     0,   255,   254,     0,     0,     0,   261,   258,   257,
     256,     0,   237,   233,   229,    62,    63,    53,     0,    49,
       0,    51,    48,    50,    74,   220,    73,    97,     0,     0,
     104,   105,     0,     0,     0,     0,   210,     0,     0,   142,
       0,     0,     0,   145,   146,   147,   149,   148,   150,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,   192,    77,     0,
     205,     0,    75,   217,   218,   293,     0,     0,     0,     0,
       0,   239,    65,    67,    61,     0,     0,   109,     0,     0,
       0,     0,   213,     0,   143,   173,   152,     0,     0,   159,
     153,   160,   154,   161,   155,   163,   157,   162,   156,   164,
     158,     0,     0,   140,     0,   134,   133,   144,   175,   174,
     170,     0,     0,     0,   167,     0,     0,     0,     0,     0,
      79,   118,   119,   120,   186,   215,     0,   265,     0,     0,
     263,     0,     0,   106,   101,     0,     0,   211,     0,     0,
     141,     0,   131,     0,   135,   168,   166,     0,   171,   191,
       0,   189,     0,     0,    80,     0,     0,     0,     0,   100,
     110,    98,    99,     0,     0,   136,     0,   138,   169,     0,
      76,    78,     0,    88,   206,     0,   248,   249,   137,   139,
     132,   190,     0,     0,    31,     0,    80,     0,    81,    82,
      85,    90,    91,     0,     0,     0,     0,    86,    87,    84,
       0,   207,   187,     0,    83,    89,     0,     0,     0,     0,
       0,     0,     0,     0,   267
  };

  const short int
  parser::yypgoto_[] =
  {
    -359,  -359,  -359,  -359,    41,  -359,   533,    20,   565,    82,
     623,  -359,   611,   321,   639,    98,  -308,   311,   395,  -359,
    -359,  -271,  -252,  -359,  -359,   137,  -359,   129,  -359,  -359,
     127,   447,  -133,  -112,   310,   -88,  -222,  -114,  -109,  -359,
     443,   185,  -358,  -359,  -359,  -359,  -334,  -359,  -359,  -359,
    -312,  -359,  -359,  -359,  -359,  -359,  -359,  -359,   731,  -359,
     318,   740,  -359,   365,   746,  -359,  -359,  -359,  -359,  -359,
    -359,  -359,   455,  -359,  -359,   438,   -60,  -359,  -359,   378,
    -359,   629,  -359,  -359,  -359,   226,  -359,  -359,   114,    40,
      18,  -100,   477,    -3,  -359,  -119,    -9,    -1
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    16,    17,    18,    61,    20,    21,   207,    23,    63,
     108,   109,   110,   248,   149,   331,   410,   328,   329,   123,
     208,   258,   339,   520,   554,   573,   588,   589,   599,   584,
     591,   209,   176,   177,   178,   345,   422,   259,   417,   210,
     372,   355,   543,   356,   443,   444,   212,   213,   359,   270,
     357,   214,   215,   216,   379,   217,   461,   586,    24,   267,
     350,    25,   292,   293,    26,    27,    56,    28,   146,   241,
     324,   237,   238,   402,   321,   397,   392,   390,    29,   116,
      30,   131,    31,    32,    33,   296,    34,    35,   180,    44,
     218,   219,   288,   220,   221,   222,   223,   592
  };

  const short int
  parser::yytable_[] =
  {
      59,    46,    46,   179,   341,   224,   342,   521,    60,    64,
     245,   368,    77,   445,   446,   445,   446,   299,    42,   429,
      22,   408,    45,    45,   322,   284,   414,    22,   284,   -18,
      22,    42,   -18,    77,    77,    46,    90,    37,    46,    42,
     385,    19,   431,   360,   239,   242,   406,    36,    52,   452,
      10,   174,   522,   281,    46,    76,    45,   377,   255,    45,
     445,   446,   449,    40,   174,   286,   285,   504,   286,   271,
     272,   265,    46,    48,    40,    45,    87,    88,   505,    51,
     282,    42,   175,   378,   287,    41,    62,    78,   415,   133,
      57,   300,    42,    45,   484,   476,    75,   408,   323,   246,
      22,    22,    22,    22,    22,    22,   523,   503,    46,   338,
     507,   171,   462,    77,    46,   181,   458,   138,    43,    47,
     407,    38,    46,    46,    51,    46,    46,    77,   541,    45,
     361,   294,   170,   508,   509,    45,   447,    39,   515,    22,
     171,   240,   240,    45,    45,   327,    45,    45,   340,   450,
      10,    59,    89,    22,   540,   113,   173,    49,    22,   251,
     183,   236,   380,    58,   171,    46,    46,   534,   485,   563,
     230,   119,    42,    50,   284,   347,   348,   171,    10,    51,
     114,   564,   381,   406,    79,   256,    45,    45,   406,   132,
     406,   382,   383,  -298,   570,    22,   475,   115,   256,   239,
      42,   480,   481,   406,   425,   174,   254,    51,   289,   406,
     525,   274,   285,   420,   286,   284,   260,    42,   284,   559,
     425,   120,   403,   561,   562,   167,   122,   416,   240,   566,
     287,    42,   121,   566,   421,   601,   419,   283,   124,   225,
     226,   571,   228,   229,   393,   330,   332,   332,   125,   332,
     332,   171,   459,   285,   171,   286,   285,   409,   286,   399,
     400,   284,   411,    91,   412,   126,     3,    65,    66,    10,
     338,   287,   337,    67,   287,   256,    42,   413,   426,    46,
     289,   384,   257,   472,   294,   420,   127,    86,   240,   128,
     420,   420,   129,     8,   537,    92,   240,    68,    51,   285,
      45,   286,    69,   567,    10,   478,   421,   579,   211,   358,
     130,   421,   421,    42,    93,   142,   261,   287,    70,   240,
     111,    94,   182,   262,   330,    95,    96,    97,    98,    99,
     100,   101,   263,    51,   144,   102,   518,   103,    46,    71,
     171,   445,   446,    46,    22,   143,   112,   597,   333,   455,
     335,   336,   104,    14,    72,   145,    73,    74,   284,    45,
     264,   256,   284,   148,    45,   404,   105,   598,   106,   107,
     284,   453,   269,   489,   274,   490,   275,   276,   465,   277,
      46,   240,    59,   235,   284,    59,    59,    59,   150,     1,
     467,   284,   284,   468,   469,   470,   285,   491,   286,   492,
     285,    45,   286,     2,   165,   330,   454,   474,   285,   155,
     286,     3,     4,     5,   287,    46,    46,     6,   287,   284,
      46,    46,   285,   456,   286,     7,   287,   117,   118,   285,
     285,   286,   286,   493,   511,   494,    45,    45,     8,    22,
     287,    45,    45,   156,     9,   517,   160,   287,   287,    10,
     495,   349,   496,   363,    80,    81,    82,   285,   362,   286,
     260,   297,   298,   364,   365,   366,   211,   547,   595,   370,
     373,   374,   376,   163,   164,   287,   249,   250,    11,   168,
      12,    13,   169,   172,   252,   253,   284,   184,   186,   609,
      83,    84,    85,   497,   614,   498,    22,   284,    14,   231,
     499,    15,   500,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   284,   234,   244,   532,   352,    59,
      59,   514,   274,   284,   285,   243,   286,   557,   558,   185,
     546,   247,   186,    42,    58,   285,   266,   286,   291,   430,
     273,   569,   287,   448,  -299,   451,   187,   457,   284,   -30,
     -30,   -30,   285,   287,   286,   188,   189,   171,   549,   274,
     278,   285,   279,   286,   460,   -29,   -29,   -29,   280,    42,
     287,   464,   353,   290,    59,   254,   325,   295,   256,   287,
     338,   346,   190,   486,   487,   488,   285,   344,   286,   351,
     326,    59,   334,   343,   386,   389,   391,   354,   394,   395,
     387,   192,   396,   193,   287,   401,   418,   423,   424,   363,
     482,   427,   349,   134,   136,   137,   139,   140,   141,   194,
     428,   466,    10,   195,   471,   501,    51,   502,   506,   519,
     479,   524,   510,   526,   512,   513,   196,   527,   516,   528,
     529,   531,   539,   197,   198,   135,   135,   135,   135,   135,
     135,   530,   533,   535,   536,   544,   446,   199,   200,   201,
     202,   203,   204,   205,   206,   550,   232,   352,   551,   552,
     556,   233,   538,   553,   555,   560,   572,   574,   185,   582,
     575,   186,    42,    58,   576,   577,   581,   542,   566,   185,
     587,   585,   186,    42,    58,   187,   545,   583,   135,   593,
     548,   596,   600,   135,   188,   189,   187,   602,   603,   607,
     606,   608,   610,   611,   147,   188,   189,   612,   613,   473,
     166,   353,   405,   594,   542,   604,   565,   605,   367,   477,
     375,   190,   568,   151,   152,   153,   154,   542,    53,   157,
     158,   159,   190,   161,   162,   483,   354,    54,   578,   463,
     192,   580,   193,    55,   388,   398,   227,   191,     0,     0,
     369,   192,     0,   193,     0,     0,     0,   590,   194,     0,
       0,     0,   195,     0,     0,     0,     0,     0,   542,   194,
       0,   590,     0,   195,     0,   196,     0,     0,     0,     0,
       0,     0,   197,   198,     0,     0,   196,     0,     0,     0,
       0,     0,     0,   197,   198,     0,   199,   200,   201,   202,
     203,   204,   205,   206,   268,     0,     0,   199,   200,   201,
     202,   203,   204,   205,   206,   185,     0,     0,   186,    42,
      58,     0,     0,     0,     0,     0,   185,     0,     0,   186,
      42,    58,   187,     0,     0,     0,     0,     0,     0,     0,
       0,   188,   189,   187,     0,     0,     0,     0,     0,     0,
       0,     0,   188,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,     0,   191,     0,   371,     0,   192,     0,   193,
       0,     0,     0,     0,   191,     0,     0,     0,   192,     0,
     193,     0,     0,     0,     0,   194,     0,     0,     0,   195,
     185,     0,    51,   186,    42,    58,   194,     0,     0,     0,
     195,     0,   196,     0,     0,     0,     0,   187,     0,   197,
     198,     0,     0,   196,     0,     0,   188,   189,     0,     0,
     197,   198,     0,   199,   200,   201,   202,   203,   204,   205,
     206,     0,     0,     0,   199,   200,   201,   202,   203,   204,
     205,   206,     0,   190,     0,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,     0,     0,   191,     0,
       0,     0,   192,     0,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     194,   301,     0,     0,   195,     0,     0,     0,     0,     0,
     284,     0,     0,     0,   302,   303,     0,   196,   304,     0,
       0,     0,     0,     0,   197,   198,   305,   306,     0,   307,
       0,     0,     0,     0,     0,   308,     0,     0,   199,   200,
     201,   202,   203,   204,   205,   206,     0,   309,   285,     0,
     286,     0,     0,    92,     0,     0,     0,     0,   310,     0,
     311,     0,     0,     0,     0,     0,   287,     0,     0,   312,
       0,     0,    93,     0,     0,     0,     0,     0,     0,    94,
       0,     0,   313,    95,    96,    97,    98,    99,   100,   101,
       0,     0,   314,   102,     0,   103,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,     0,     0,     0,
     104,   316,   317,   318,     0,     0,     0,     0,     0,     0,
       0,     0,   319,   320,   105,     0,   106,   107
  };

  const short int
  parser::yycheck_[] =
  {
       9,     4,     5,   117,    40,   124,   258,    47,     9,    12,
      25,   282,    15,     6,     7,     6,     7,    40,    23,   353,
       0,   329,     4,     5,    40,    53,   338,     7,    53,   114,
      10,    23,   117,    36,    37,    38,    39,    43,    41,    23,
     292,     0,   354,    57,   144,   145,    40,   127,     7,   361,
      87,    56,    92,    40,    57,    15,    38,    26,   167,    41,
       6,     7,    57,    31,    56,    93,    91,     8,    93,   188,
     189,   180,    75,    43,    31,    57,    36,    37,    19,   116,
      67,    23,    87,    52,   109,     3,    43,     0,   340,    26,
      80,   114,    23,    75,   428,    87,    14,   405,   114,   114,
      80,    81,    82,    83,    84,    85,   146,   441,   111,   145,
     444,   114,   383,   116,   117,   118,   368,    26,     4,     5,
     114,   127,   125,   126,   116,   128,   129,   130,     7,   111,
     144,   231,   114,   445,   446,   117,   129,   143,   129,   119,
     143,   144,   145,   125,   126,    87,   128,   129,   257,   144,
      87,   160,    38,   133,   488,    41,   116,   127,   138,   160,
     119,   143,    93,    24,   167,   168,   169,   479,   114,     7,
     130,    57,    23,   143,    53,   263,   264,   180,    87,   116,
     108,   539,   113,    40,   117,   167,   168,   169,    40,    75,
      40,   291,    67,   108,   552,   175,   418,    71,   180,   299,
      23,   423,   424,    40,    40,    56,    29,   116,   211,    40,
     462,   191,    91,   346,    93,    53,   175,    23,    53,   531,
      40,   114,   322,   535,   536,   111,    52,   341,   231,    40,
     109,    23,    87,    40,   346,   593,    87,    29,    11,   125,
     126,   553,   128,   129,   304,   248,   249,   250,   124,   252,
     253,   254,   371,    91,   257,    93,    91,   114,    93,   319,
     320,    53,   114,    32,   114,    67,    49,    38,    39,    87,
     145,   109,   254,    44,   109,   257,    23,   114,   114,   282,
     283,   156,   168,   114,   384,   418,   128,    93,   291,    67,
     423,   424,   124,    76,   114,    64,   299,    68,   116,    91,
     282,    93,    73,   114,    87,   419,   418,   114,   123,   144,
      67,   423,   424,    23,    83,   108,    68,   109,    89,   322,
      67,    90,   140,    75,   327,    94,    95,    96,    97,    98,
      99,   100,    84,   116,    87,   104,   455,   106,   341,   110,
     343,     6,     7,   346,   324,   108,    93,    30,   250,    40,
     252,   253,   121,   136,   125,    87,   127,   128,    53,   341,
     112,   343,    53,    87,   346,   324,   135,    50,   137,   138,
      53,    40,   187,    26,   354,    28,   191,   192,   387,   194,
     383,   384,   391,    93,    53,   394,   395,   396,    87,    27,
     391,    53,    53,   394,   395,   396,    91,    26,    93,    28,
      91,   383,    93,    41,    58,   408,    40,   410,    91,    87,
      93,    49,    50,    51,   109,   418,   419,    55,   109,    53,
     423,   424,    91,   114,    93,    63,   109,    49,    50,    91,
      91,    93,    93,    26,   129,    28,   418,   419,    76,   419,
     109,   423,   424,    87,    82,   454,    87,   109,   109,    87,
      26,   266,    28,   114,     3,     4,     5,    91,   273,    93,
     419,   235,   236,   278,   279,   280,   281,   129,   587,   284,
     285,   286,   287,    87,    87,   109,   155,   156,   116,    67,
     118,   119,    67,    62,   163,   164,    53,   114,    22,   608,
       3,     4,     5,    26,   613,    28,   476,    53,   136,   118,
      26,   139,    28,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    53,    93,    59,   476,     8,   528,
     529,    60,   502,    53,    91,    29,    93,   528,   529,    19,
      60,    25,    22,    23,    24,    91,    87,    93,   108,   354,
      87,   550,   109,   358,   108,   360,    36,   114,    53,     3,
       4,     5,    91,   109,    93,    45,    46,   560,   114,   539,
      87,    91,    87,    93,   379,     3,     4,     5,    87,    23,
     109,   386,    62,    87,   583,    29,    58,   115,   560,   109,
     145,    81,    72,    15,    16,    17,    91,   105,    93,    40,
     114,   600,   114,   114,    11,    61,    87,    87,    87,    87,
     120,    91,    87,    93,   109,     8,    81,    81,    81,   114,
     425,    87,   427,    80,    81,    82,    83,    84,    85,   109,
      62,    11,    87,   113,    19,   440,   116,    87,   443,    69,
     102,   114,   447,    42,   449,   450,   126,   114,   453,    40,
      40,   102,    87,   133,   134,    80,    81,    82,    83,    84,
      85,   114,   114,   102,   102,    19,     7,   147,   148,   149,
     150,   151,   152,   153,   154,    40,   133,     8,   114,    35,
      40,   138,   487,    70,   155,   114,   103,    87,    19,    35,
      74,    22,    23,    24,   114,   114,   114,   502,    40,    19,
      11,   107,    22,    23,    24,    36,   511,    85,   133,    35,
     515,    40,    40,   138,    45,    46,    36,   114,    40,   123,
      65,    35,    40,    86,    91,    45,    46,   123,    35,   408,
     109,    62,   327,   586,   539,   596,   541,   600,   281,   419,
     287,    72,   547,    94,    95,    96,    97,   552,     7,   100,
     101,   102,    72,   104,   105,   427,    87,     7,   563,   384,
      91,   566,    93,     7,   299,   317,   127,    87,    -1,    -1,
     283,    91,    -1,    93,    -1,    -1,    -1,   582,   109,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   593,   109,
      -1,   596,    -1,   113,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   147,   148,   149,   150,
     151,   152,   153,   154,   144,    -1,    -1,   147,   148,   149,
     150,   151,   152,   153,   154,    19,    -1,    -1,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    22,
      23,    24,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    46,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    87,    -1,    78,    -1,    91,    -1,    93,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,    -1,
      93,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,
      19,    -1,   116,    22,    23,    24,   109,    -1,    -1,    -1,
     113,    -1,   126,    -1,    -1,    -1,    -1,    36,    -1,   133,
     134,    -1,    -1,   126,    -1,    -1,    45,    46,    -1,    -1,
     133,   134,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,    -1,    -1,    -1,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,    72,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    20,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    33,    34,    -1,   126,    37,    -1,
      -1,    -1,    -1,    -1,   133,   134,    45,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,    -1,    66,    91,    -1,
      93,    -1,    -1,    64,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    88,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,   101,    94,    95,    96,    97,    98,    99,   100,
      -1,    -1,   111,   104,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
     121,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   135,    -1,   137,   138
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    27,    41,    49,    50,    51,    55,    63,    76,    82,
      87,   116,   118,   119,   136,   139,   158,   159,   160,   161,
     162,   163,   164,   165,   215,   218,   221,   222,   224,   235,
     237,   239,   240,   241,   243,   244,   127,    43,   127,   143,
      31,   166,    23,   245,   246,   247,   250,   245,    43,   127,
     143,   116,   161,   215,   218,   221,   223,    80,    24,   253,
     254,   161,    43,   166,   250,    38,    39,    44,    68,    73,
      89,   110,   125,   127,   128,   166,   246,   250,     0,   117,
       3,     4,     5,     3,     4,     5,    93,   246,   246,   245,
     250,    32,    64,    83,    90,    94,    95,    96,    97,    98,
      99,   100,   104,   106,   121,   135,   137,   138,   167,   168,
     169,    67,    93,   245,   108,    71,   236,   236,   236,   245,
     114,    87,    52,   176,    11,   124,    67,   128,    67,   124,
      67,   238,   245,    26,   163,   165,   163,   163,    26,   163,
     163,   163,   108,   108,    87,    87,   225,   167,    87,   171,
      87,   171,   171,   171,   171,    87,    87,   171,   171,   171,
      87,   171,   171,    87,    87,    58,   169,   245,    67,    67,
     247,   250,    62,   246,    56,    87,   189,   190,   191,   194,
     245,   250,   140,   161,   114,    19,    22,    36,    45,    46,
      72,    87,    91,    93,   109,   113,   126,   133,   134,   147,
     148,   149,   150,   151,   152,   153,   154,   164,   177,   188,
     196,   198,   203,   204,   208,   209,   210,   212,   247,   248,
     250,   251,   252,   253,   252,   245,   245,   238,   245,   245,
     246,   118,   163,   163,    93,    93,   247,   228,   229,   248,
     250,   226,   248,    29,    59,    25,   114,    25,   170,   170,
     170,   254,   170,   170,    29,   195,   247,   245,   178,   194,
     161,    68,    75,    84,   112,   195,    87,   216,   144,   198,
     206,   252,   252,    87,   164,   198,   198,   198,    87,    87,
      87,    40,    67,    29,    53,    91,    93,   109,   249,   250,
      87,   108,   219,   220,   248,   115,   242,   242,   242,    40,
     114,    20,    33,    34,    37,    45,    46,    48,    54,    66,
      77,    79,    88,   101,   111,   122,   130,   131,   132,   141,
     142,   231,    40,   114,   227,    58,   114,    87,   174,   175,
     250,   172,   250,   172,   114,   172,   172,   247,   145,   179,
     195,    40,   179,   114,   105,   192,    81,   192,   192,   198,
     217,    40,     8,    62,    87,   198,   200,   207,   144,   205,
      57,   144,   198,   114,   198,   198,   198,   188,   178,   249,
     198,    78,   197,   198,   198,   197,   198,    26,    52,   211,
      93,   113,   248,    67,   156,   179,    11,   120,   229,    61,
     234,    87,   233,   233,    87,    87,    87,   232,   232,   233,
     233,     8,   230,   248,   161,   175,    40,   114,   173,   114,
     173,   114,   114,   114,   207,   179,   194,   195,    81,    87,
     189,   190,   193,    81,    81,    40,   114,    87,    62,   203,
     198,   207,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,   201,   202,     6,     7,   129,   198,    57,
     144,   198,   207,    40,    40,    40,   114,   114,   179,   252,
     198,   213,   178,   220,   198,   253,    11,   254,   254,   254,
     254,    19,   114,   174,   250,   193,    87,   191,   194,   102,
     193,   193,   198,   217,   203,   114,    15,    16,    17,    26,
      28,    26,    28,    26,    28,    26,    28,    26,    28,    26,
      28,   198,    87,   203,     8,    19,   198,   203,   207,   207,
     198,   129,   198,   198,    60,   129,   198,   253,   252,    69,
     180,    47,    92,   146,   114,   179,    42,   114,    40,    40,
     114,   102,   161,   114,   207,   102,   102,   114,   198,    87,
     203,     7,   198,   199,    19,   198,    60,   129,   198,   114,
      40,   114,    35,    70,   181,   155,    40,   254,   254,   207,
     114,   207,   207,     7,   199,   198,    40,   114,   198,   253,
     199,   207,   103,   182,    87,    74,   114,   114,   198,   114,
     198,   114,    35,    85,   186,   107,   214,    11,   183,   184,
     198,   187,   254,    35,   182,   252,    40,    30,    50,   185,
      40,   199,   114,    40,   184,   187,    65,   123,    35,   252,
      40,    86,   123,    35,   252
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
     186,   187,   188,   188,   188,   189,   189,   190,   191,   191,
     191,   191,   192,   192,   193,   193,   193,   194,   194,   194,
     195,   195,   195,   196,   196,   196,   196,   196,   197,   197,
     197,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   199,   199,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   201,   201,   201,   201,   201,
     201,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   203,   204,   204,   205,   205,
     206,   206,   207,   207,   207,   207,   208,   208,   208,   208,
     208,   208,   209,   209,   209,   209,   210,   210,   210,   210,
     210,   210,   210,   211,   211,   211,   212,   212,   212,   212,
     212,   212,   212,   212,   213,   213,   214,   214,   215,   215,
     216,   216,   217,   217,   218,   218,   219,   219,   220,   221,
     221,   221,   222,   223,   223,   223,   223,   224,   224,   224,
     225,   225,   226,   226,   227,   228,   228,   229,   230,   230,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   232,   232,   233,   233,   234,   234,   235,   235,
     235,   236,   236,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   238,   238,   239,   240,   240,   241,
     241,   241,   242,   242,   243,   244,   245,   245,   246,   247,
     248,   249,   250,   251,   251,   252,   253,   254
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
       2,     1,     1,     2,     3,     2,     1,     4,     7,     7,
       7,     6,     0,     1,     1,     1,     3,     1,     1,     1,
       0,     1,     2,     1,     1,     3,     1,     1,     3,     3,
       3,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     3,     3,     4,     5,     6,     5,     6,
       3,     4,     2,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     6,     5,     4,     5,
       4,     5,     1,     3,     3,     3,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     1,     5,    10,     4,     6,
       8,     6,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     0,     3,     5,     4,
       3,     5,     1,     3,     6,     8,     1,     3,     3,     6,
       7,     6,     2,     1,     1,     1,     1,     7,     3,     7,
       0,     3,     1,     3,     0,     1,     3,     3,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     6,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     1,
       1,     2,     0,     3,     0,     3,     0,    17,     4,     4,
       4,     0,     2,     4,     2,     4,     3,     4,     4,     2,
       4,     4,     2,     2,     2,     0,     2,     2,     2,     6,
       6,     6,     0,     3,     4,     2,     1,     3,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1
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
  "logical_expr", "column_ref", "expr_const", "func_expr",
  "distinct_or_all", "win_type", "opt_arith_expr", "opt_partition_by",
  "insert_stmt", "insert_value_list", "insert_value", "update_stmt",
  "update_asgn_list", "update_asgn_factor", "delete_stmt", "explain_stmt",
  "explainable_stmt", "create_stmt", "opt_view_column_define",
  "view_column_define", "beg_view_define", "table_element_list",
  "column_definition", "opt_not_null", "data_type", "opt_time_precision",
  "opt_char_length", "opt_engine_def", "drop_stmt", "opt_if_exists",
  "show_stmt", "op_from_database", "use_stmt", "desc_stmt", "analyze_stmt",
  "opt_sample_size", "set_var_stmt", "kill_stmt", "relation_factor",
  "database_name", "relation_name", "column_name", "column_label", "ident",
  "datetime", "string", "number", "int_value", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   353,   353,   361,   362,   363,   364,   365,   366,   367,
     376,   377,   378,   379,   380,   381,   382,   383,   387,   391,
     398,   403,   408,   413,   418,   423,   428,   433,   441,   445,
     452,   459,   480,   481,   487,   497,   500,   507,   514,   525,
     530,   535,   540,   545,   550,   555,   560,   565,   571,   579,
     587,   595,   603,   610,   620,   629,   630,   637,   638,   642,
     649,   654,   662,   663,   667,   673,   682,   687,   695,   696,
     700,   705,   713,   718,   726,   727,   731,   732,   736,   737,
     741,   742,   746,   751,   759,   766,   767,   768,   772,   773,
     780,   790,   797,   803,   809,   818,   823,   833,   842,   846,
     850,   854,   862,   865,   872,   876,   880,   887,   891,   895,
     902,   903,   907,   919,   923,   927,   931,   935,   942,   952,
     962,   975,   979,   984,   989,   994,   999,  1004,  1009,  1014,
    1019,  1027,  1033,  1042,  1047,  1052,  1057,  1062,  1067,  1078,
    1089,  1094,  1099,  1103,  1108,  1116,  1120,  1124,  1128,  1132,
    1136,  1140,  1144,  1151,  1155,  1159,  1163,  1167,  1171,  1175,
    1179,  1183,  1187,  1191,  1195,  1202,  1211,  1218,  1227,  1233,
    1242,  1248,  1257,  1261,  1265,  1270,  1278,  1285,  1291,  1299,
    1306,  1314,  1325,  1334,  1347,  1356,  1368,  1393,  1405,  1409,
    1417,  1429,  1433,  1442,  1445,  1449,  1456,  1460,  1464,  1468,
    1472,  1476,  1480,  1484,  1492,  1495,  1503,  1506,  1518,  1527,
    1538,  1543,  1551,  1556,  1569,  1580,  1594,  1600,  1609,  1624,
    1635,  1646,  1664,  1673,  1674,  1675,  1676,  1685,  1694,  1701,
    1716,  1719,  1726,  1731,  1740,  1746,  1751,  1759,  1769,  1772,
    1779,  1781,  1783,  1785,  1787,  1789,  1791,  1793,  1795,  1797,
    1799,  1801,  1803,  1805,  1807,  1809,  1811,  1813,  1815,  1817,
    1819,  1821,  1826,  1827,  1838,  1839,  1851,  1858,  1874,  1882,
    1890,  1902,  1903,  1912,  1919,  1926,  1933,  1940,  1947,  1951,
    1955,  1963,  1971,  1977,  1986,  1991,  1997,  2012,  2019,  2033,
    2042,  2051,  2063,  2064,  2071,  2082,  2096,  2104,  2115,  2119,
    2123,  2127,  2131,  2135,  2136,  2140,  2144,  2148
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
#line 4762 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:1167
#line 2152 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
