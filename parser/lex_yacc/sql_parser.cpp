// A Bison parser, made by GNU Bison 3.4.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

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

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

// //                    "%code top" blocks.
#line 26 "D:/CatDB/parser/lex_yacc/sql_parser.y"

	#include "sql_parser.hpp"
	#include "sql_scanner.h"
	#include "sql_driver.h"
	#include "location.hh"
	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
	#include "create_stmt.h"
	#include "drop_stmt.h"
	#include "show_stmt.h"
	#include "expr_stmt.h"
	#include "object.h"
	
	/*注意：这里的参数由%parse-param决定*/
	static CatDB::SqlParser::symbol_type yylex(CatDB::SqlScanner& scanner,CatDB::SqlDriver &driver)
	{
		return scanner.get_next_token();
	}
	using namespace CatDB;
	using namespace CatDB::Parser;
	using namespace CatDB::Common;

#line 63 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"


// First part of user prologue.
#line 51 "D:/CatDB/parser/lex_yacc/sql_parser.y"

	
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

#define make_list_stmt(ret, from) \
{ \
	Stmt_s stmt = ListStmt::make_list_stmt(); \
	check(stmt); \
	ListStmt* list = dynamic_cast<ListStmt*>(stmt.get()); \
	check(list); \
	list->stmt_list.push_back(from); \
	ret = stmt; \
}

#define list_stmt_push(ret, list, from) \
{ \
	ListStmt* stmt = dynamic_cast<ListStmt*>(list.get()); \
	check(stmt); \
	stmt->stmt_list.push_back(from); \
	ret = list; \
}

#define make_aggr_stmt(stmt, function_name) \
{ \
	stmt = AggrStmt::make_aggr_stmt(); \
	check(stmt); \
	AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get()); \
	if(function_name == "sum") \
		aggr->aggr_func = AggrStmt::SUM; \
	else if(function_name == "avg") \
		aggr->aggr_func = AggrStmt::AVG; \
	else if(function_name == "count") \
		aggr->aggr_func = AggrStmt::COUNT; \
	else if(function_name == "min") \
		aggr->aggr_func = AggrStmt::MIN; \
	else if(function_name == "max") \
		aggr->aggr_func = AggrStmt::MAX; \
	else \
	{ \
		yyerror("unknow aggregate function %s", function_name.c_str()); \
		YYABORT; \
	} \
}

#define make_unary_stmt(stmt, stmt1, op) \
{ \
	stmt = UnaryExprStmt::make_unary_stmt(); \
	check(stmt); \
	UnaryExprStmt* unary = dynamic_cast<UnaryExprStmt*>(stmt.get()); \
	unary->expr_stmt = stmt1; \
	unary->op_type = op; \
}

#define make_binary_stmt(stmt, stmt1, stmt2, op) \
{ \
	stmt = BinaryExprStmt::make_binary_stmt(); \
	check(stmt); \
	BinaryExprStmt* binary = dynamic_cast<BinaryExprStmt*>(stmt.get()); \
	binary->first_expr_stmt = stmt1; \
	binary->second_expr_stmt = stmt2; \
	binary->op_type = op; \
}

#define make_ternary_stmt(stmt, stmt1, stmt2, stmt3, op) \
{ \
	stmt = TernaryExprStmt::make_ternary_stmt(); \
	check(stmt); \
	TernaryExprStmt* ternary = dynamic_cast<TernaryExprStmt*>(stmt.get()); \
	ternary->first_expr_stmt = stmt1; \
	ternary->second_expr_stmt = stmt2; \
	ternary->third_expr_stmt = stmt3; \
	ternary->op_type = op; \
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

#line 166 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"


#include "sql_parser.hpp"




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

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
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
    while (false)
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
      *yycdebug_ << '\n';                       \
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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 3 "D:/CatDB/parser/lex_yacc/sql_parser.y"
namespace CatDB {
#line 266 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"


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
        std::string yyr;
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
              else
                goto append;

            append:
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

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 97: // sql_stmt
      case 98: // stmt
      case 99: // select_stmt
      case 100: // select_with_parens
      case 101: // select_no_parens
      case 102: // opt_hint
      case 103: // opt_hint_body
      case 104: // opt_hint_list
      case 105: // hint_item
      case 107: // select_expr_list
      case 108: // from_list
      case 109: // opt_where
      case 110: // opt_groupby
      case 111: // opt_having
      case 112: // opt_order_by
      case 114: // opt_select_limit
      case 116: // order_by
      case 117: // projection
      case 118: // table_factor
      case 119: // expr_list
      case 120: // expr
      case 121: // in_expr
      case 122: // arith_expr
      case 123: // simple_expr
      case 124: // column_ref
      case 125: // expr_const
      case 126: // func_expr
      case 129: // insert_stmt
      case 130: // insert_vals_list
      case 131: // insert_vals
      case 132: // update_stmt
      case 133: // update_asgn_list
      case 134: // update_asgn_factor
      case 135: // delete_stmt
      case 136: // explain_stmt
      case 137: // explainable_stmt
      case 138: // create_stmt
      case 139: // table_element_list
      case 140: // table_element
      case 141: // column_definition
      case 147: // drop_stmt
      case 148: // show_stmt
      case 150: // use_stmt
      case 151: // desc_stmt
      case 152: // analyze_stmt
      case 153: // relation_factor
        value.YY_MOVE_OR_COPY< Stmt_s > (YY_MOVE (that.value));
        break;

      case 106: // opt_distinct
      case 113: // opt_asc_desc
      case 128: // distinct_or_all
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 115: // limit_expr
      case 142: // data_type
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 127: // simple_function_expr
      case 149: // op_from_database
      case 154: // database_name
      case 155: // relation_name
      case 156: // column_name
      case 157: // function_name
      case 158: // column_label
      case 159: // ident
      case 160: // datetime
      case 161: // string
      case 162: // number
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 97: // sql_stmt
      case 98: // stmt
      case 99: // select_stmt
      case 100: // select_with_parens
      case 101: // select_no_parens
      case 102: // opt_hint
      case 103: // opt_hint_body
      case 104: // opt_hint_list
      case 105: // hint_item
      case 107: // select_expr_list
      case 108: // from_list
      case 109: // opt_where
      case 110: // opt_groupby
      case 111: // opt_having
      case 112: // opt_order_by
      case 114: // opt_select_limit
      case 116: // order_by
      case 117: // projection
      case 118: // table_factor
      case 119: // expr_list
      case 120: // expr
      case 121: // in_expr
      case 122: // arith_expr
      case 123: // simple_expr
      case 124: // column_ref
      case 125: // expr_const
      case 126: // func_expr
      case 129: // insert_stmt
      case 130: // insert_vals_list
      case 131: // insert_vals
      case 132: // update_stmt
      case 133: // update_asgn_list
      case 134: // update_asgn_factor
      case 135: // delete_stmt
      case 136: // explain_stmt
      case 137: // explainable_stmt
      case 138: // create_stmt
      case 139: // table_element_list
      case 140: // table_element
      case 141: // column_definition
      case 147: // drop_stmt
      case 148: // show_stmt
      case 150: // use_stmt
      case 151: // desc_stmt
      case 152: // analyze_stmt
      case 153: // relation_factor
        value.move< Stmt_s > (YY_MOVE (that.value));
        break;

      case 106: // opt_distinct
      case 113: // opt_asc_desc
      case 128: // distinct_or_all
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 115: // limit_expr
      case 142: // data_type
        value.move< int > (YY_MOVE (that.value));
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 127: // simple_function_expr
      case 149: // op_from_database
      case 154: // database_name
      case 155: // relation_name
      case 156: // column_name
      case 157: // function_name
      case 158: // column_label
      case 159: // ident
      case 160: // datetime
      case 161: // string
      case 162: // number
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 97: // sql_stmt
      case 98: // stmt
      case 99: // select_stmt
      case 100: // select_with_parens
      case 101: // select_no_parens
      case 102: // opt_hint
      case 103: // opt_hint_body
      case 104: // opt_hint_list
      case 105: // hint_item
      case 107: // select_expr_list
      case 108: // from_list
      case 109: // opt_where
      case 110: // opt_groupby
      case 111: // opt_having
      case 112: // opt_order_by
      case 114: // opt_select_limit
      case 116: // order_by
      case 117: // projection
      case 118: // table_factor
      case 119: // expr_list
      case 120: // expr
      case 121: // in_expr
      case 122: // arith_expr
      case 123: // simple_expr
      case 124: // column_ref
      case 125: // expr_const
      case 126: // func_expr
      case 129: // insert_stmt
      case 130: // insert_vals_list
      case 131: // insert_vals
      case 132: // update_stmt
      case 133: // update_asgn_list
      case 134: // update_asgn_factor
      case 135: // delete_stmt
      case 136: // explain_stmt
      case 137: // explainable_stmt
      case 138: // create_stmt
      case 139: // table_element_list
      case 140: // table_element
      case 141: // column_definition
      case 147: // drop_stmt
      case 148: // show_stmt
      case 150: // use_stmt
      case 151: // desc_stmt
      case 152: // analyze_stmt
      case 153: // relation_factor
        value.move< Stmt_s > (that.value);
        break;

      case 106: // opt_distinct
      case 113: // opt_asc_desc
      case 128: // distinct_or_all
        value.move< bool > (that.value);
        break;

      case 115: // limit_expr
      case 142: // data_type
        value.move< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 127: // simple_function_expr
      case 149: // op_from_database
      case 154: // database_name
      case 155: // relation_name
      case 156: // column_name
      case 157: // function_name
      case 158: // column_label
      case 159: // ident
      case 160: // datetime
      case 161: // string
      case 162: // number
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
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
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
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

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
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

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
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
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
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
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 97: // sql_stmt
      case 98: // stmt
      case 99: // select_stmt
      case 100: // select_with_parens
      case 101: // select_no_parens
      case 102: // opt_hint
      case 103: // opt_hint_body
      case 104: // opt_hint_list
      case 105: // hint_item
      case 107: // select_expr_list
      case 108: // from_list
      case 109: // opt_where
      case 110: // opt_groupby
      case 111: // opt_having
      case 112: // opt_order_by
      case 114: // opt_select_limit
      case 116: // order_by
      case 117: // projection
      case 118: // table_factor
      case 119: // expr_list
      case 120: // expr
      case 121: // in_expr
      case 122: // arith_expr
      case 123: // simple_expr
      case 124: // column_ref
      case 125: // expr_const
      case 126: // func_expr
      case 129: // insert_stmt
      case 130: // insert_vals_list
      case 131: // insert_vals
      case 132: // update_stmt
      case 133: // update_asgn_list
      case 134: // update_asgn_factor
      case 135: // delete_stmt
      case 136: // explain_stmt
      case 137: // explainable_stmt
      case 138: // create_stmt
      case 139: // table_element_list
      case 140: // table_element
      case 141: // column_definition
      case 147: // drop_stmt
      case 148: // show_stmt
      case 150: // use_stmt
      case 151: // desc_stmt
      case 152: // analyze_stmt
      case 153: // relation_factor
        yylhs.value.emplace< Stmt_s > ();
        break;

      case 106: // opt_distinct
      case 113: // opt_asc_desc
      case 128: // distinct_or_all
        yylhs.value.emplace< bool > ();
        break;

      case 115: // limit_expr
      case 142: // data_type
        yylhs.value.emplace< int > ();
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 127: // simple_function_expr
      case 149: // op_from_database
      case 154: // database_name
      case 155: // relation_name
      case 156: // column_name
      case 157: // function_name
      case 158: // column_label
      case 159: // ident
      case 160: // datetime
      case 161: // string
      case 162: // number
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 290 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		driver.result = yystack_[1].value.as < Stmt_s > ();
		YYACCEPT;
    }
#line 993 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 3:
#line 297 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 999 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 4:
#line 298 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1005 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 5:
#line 299 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1011 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 6:
#line 300 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1017 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 7:
#line 301 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1023 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 8:
#line 302 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1029 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 9:
#line 303 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1035 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 10:
#line 304 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1041 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 11:
#line 305 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1047 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 12:
#line 306 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1053 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 13:
#line 307 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1059 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 14:
#line 308 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yyerror("unknow stmt"); }
#line 1065 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 15:
#line 318 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1073 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 16:
#line 325 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = QueryStmt::make_query_stmt();
		QueryStmt* query = dynamic_cast<QueryStmt*>(stmt.get());
		query->query_stmt = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1084 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 17:
#line 338 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建select stmt
		Stmt_s stmt = SelectStmt::make_select_stmt();
		SelectStmt* select_stmt = dynamic_cast<SelectStmt*>(stmt.get());
		select_stmt->hint_list = yystack_[10].value.as < Stmt_s > ();
		select_stmt->is_distinct = yystack_[9].value.as < bool > ();
		select_stmt->select_expr_list = yystack_[8].value.as < Stmt_s > ();
		select_stmt->from_stmts = yystack_[6].value.as < Stmt_s > ();
		select_stmt->where_stmt = yystack_[5].value.as < Stmt_s > ();
		select_stmt->group_columns = yystack_[4].value.as < Stmt_s > ();
		select_stmt->having_stmt = yystack_[3].value.as < Stmt_s > ();
		select_stmt->order_columns = yystack_[2].value.as < Stmt_s > ();
		select_stmt->asc_desc = yystack_[1].value.as < bool > ();
		select_stmt->limit_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1105 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 18:
#line 355 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建union二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION);
    }
#line 1114 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 19:
#line 360 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建union all二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION_ALL);
    }
#line 1123 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 20:
#line 365 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建intersect二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_INTERSECT);
    }
#line 1132 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 21:
#line 370 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建except二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXCEPT);
    }
#line 1141 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 22:
#line 375 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(true);
	}
#line 1149 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 23:
#line 379 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建select stmt
		Stmt_s stmt = SelectStmt::make_select_stmt();
		SelectStmt* select_stmt = dynamic_cast<SelectStmt*>(stmt.get());
		select_stmt->is_distinct = false;
		select_stmt->select_expr_list = yystack_[2].value.as < Stmt_s > ();
		{
		make_list_stmt(select_stmt->from_stmts, TableStmt::make_table_stmt("system","sys_databases"));
		}
		select_stmt->limit_stmt = LimitStmt::make_limit_stmt(1);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1166 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 24:
#line 394 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = nullptr; }
#line 1172 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 25:
#line 396 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
	}
#line 1180 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 26:
#line 401 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = nullptr; }
#line 1186 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 27:
#line 403 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1194 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 28:
#line 408 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = nullptr; }
#line 1200 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 29:
#line 410 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1208 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 30:
#line 414 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1216 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 31:
#line 420 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		HintStmt::HintBody hint;
		hint.dop = std::stoi(yystack_[1].value.as < std::string > ());
		yylhs.value.as < Stmt_s > () = HintStmt::make_hint_stmt(HintStmt::Parallel, hint);
	}
#line 1226 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 32:
#line 427 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1232 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 33:
#line 428 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1238 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 34:
#line 433 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建select list
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1247 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 35:
#line 438 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//把输出表达式加入到select list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1256 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 36:
#line 446 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建from list 
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1265 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 37:
#line 451 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//把表加入到from list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1274 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 38:
#line 458 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1280 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 39:
#line 459 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1286 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 40:
#line 463 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1292 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 41:
#line 464 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1298 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 42:
#line 468 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1304 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 43:
#line 469 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1310 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 44:
#line 473 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1316 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 45:
#line 474 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1322 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 46:
#line 478 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1328 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 47:
#line 479 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1334 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 48:
#line 480 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1340 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 49:
#line 484 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {yylhs.value.as < Stmt_s > () = NULL;}
#line 1346 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 50:
#line 486 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as < int > (), yystack_[2].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1357 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 51:
#line 493 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1368 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 52:
#line 503 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < int > () = std::stoi(yystack_[0].value.as < std::string > ());
	}
#line 1376 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 53:
#line 510 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1384 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 54:
#line 517 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
	//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[0].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = stmt->to_string();
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1396 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 55:
#line 525 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1408 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 56:
#line 533 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1420 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 57:
#line 544 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1428 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 58:
#line 548 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1440 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 59:
#line 556 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1452 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 60:
#line 564 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1464 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 61:
#line 572 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1476 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 62:
#line 587 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表达式列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1485 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 63:
#line 592 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1494 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 64:
#line 601 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1502 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 65:
#line 605 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建加法二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1511 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 66:
#line 610 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建减法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1520 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 67:
#line 615 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建乘法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1529 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 68:
#line 620 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建除法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1538 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 69:
#line 625 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LE);
	}
#line 1547 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 70:
#line 630 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LT);
	}
#line 1556 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 71:
#line 635 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
	}
#line 1565 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 72:
#line 640 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GE);
	}
#line 1574 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 73:
#line 645 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GT);
	}
#line 1583 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 74:
#line 650 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NE);
	}
#line 1592 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 75:
#line 655 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LIKE);
	}
#line 1601 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 76:
#line 660 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_LIKE);
	}
#line 1610 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 77:
#line 665 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_AND);
    }
#line 1619 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 78:
#line 670 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_OR);
    }
#line 1628 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 79:
#line 675 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), ExprStmt::OP_IS_NULL);
    }
#line 1637 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 80:
#line 680 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), ExprStmt::OP_IS_NOT_NULL);
    }
#line 1646 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 81:
#line 685 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_BETWEEN);
    }
#line 1655 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 82:
#line 690 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[5].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_BETWEEN);
    }
#line 1664 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 83:
#line 695 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_IN);
    }
#line 1673 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 84:
#line 700 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_IN);
    }
#line 1682 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 85:
#line 710 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1690 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 86:
#line 714 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1698 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 87:
#line 721 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1706 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 88:
#line 725 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//正数表达式
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1715 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 89:
#line 730 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MINUS);
    }
#line 1724 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 90:
#line 735 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1733 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 91:
#line 740 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1742 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 92:
#line 745 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1751 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 93:
#line 750 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1760 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 94:
#line 758 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1768 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 95:
#line 762 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		 yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1776 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 96:
#line 766 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1784 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 97:
#line 770 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
      	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1792 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 98:
#line 774 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1800 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 99:
#line 778 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXISTS);
    }
#line 1808 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 100:
#line 782 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_EXISTS);
    }
#line 1817 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 101:
#line 790 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1828 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 102:
#line 797 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1838 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 103:
#line 803 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1849 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 104:
#line 811 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), "*");
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1860 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 105:
#line 821 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1873 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 106:
#line 830 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = DateTime::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1886 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 107:
#line 839 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1899 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 108:
#line 848 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1912 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 109:
#line 857 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1925 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 110:
#line 866 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1938 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 111:
#line 878 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->aggr_expr = col;
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1952 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 112:
#line 888 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[4].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->distinct = yystack_[2].value.as < bool > ();
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1965 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 113:
#line 897 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1977 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 114:
#line 908 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = "database";
    }
#line 1985 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 115:
#line 915 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < bool > () = false;
    }
#line 1993 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 116:
#line 919 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < bool > () = true;
    }
#line 2001 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 117:
#line 930 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建insert stmt
		Stmt_s stmt = InsertStmt::make_insert_stmt();
		check(stmt);
		InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(stmt.get());
		insert_stmt->table = yystack_[2].value.as < Stmt_s > ();
		insert_stmt->values = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2015 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 118:
#line 940 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yyerror("insert into table from query not support yet!");
		YYABORT;
    }
#line 2024 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 119:
#line 947 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
    }
#line 2032 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 120:
#line 951 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
	}
#line 2040 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 121:
#line 957 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 2049 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 122:
#line 962 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2058 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 123:
#line 975 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = UpdateStmt::make_update_stmt();
		check(stmt);
		UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(stmt.get());
		update_stmt->table = yystack_[3].value.as < Stmt_s > ();
		update_stmt->update_asgn_stmt = yystack_[1].value.as < Stmt_s > ();
		update_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2072 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 124:
#line 988 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2081 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 125:
#line 993 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2090 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 126:
#line 1001 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as < std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as < Stmt_s > (), col, yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
    }
#line 2101 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 127:
#line 1016 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DeleteStmt::make_delete_stmt();
		check(stmt);
		DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(stmt.get());
		delete_stmt->table = yystack_[1].value.as < Stmt_s > ();
		delete_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2114 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 128:
#line 1033 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > ()->is_explain = true;
    }
#line 2123 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 129:
#line 1040 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2129 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 130:
#line 1041 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2135 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 131:
#line 1042 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2141 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 132:
#line 1043 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2147 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 133:
#line 1053 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = CreateTableStmt::make_create_table_stmt();
		check(stmt);
		CreateTableStmt* create_table_stmt = dynamic_cast<CreateTableStmt*>(stmt.get());
		create_table_stmt->table = yystack_[3].value.as < Stmt_s > ();
		create_table_stmt->column_define_list = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2160 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 134:
#line 1062 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = CreateDatabaseStmt::make_create_database_stmt();
		check(stmt);
		CreateDatabaseStmt* create_database_stmt = dynamic_cast<CreateDatabaseStmt*>(stmt.get());
		create_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2172 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 135:
#line 1073 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2180 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 136:
#line 1077 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2188 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 137:
#line 1084 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 2196 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 138:
#line 1091 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as < std::string > (), yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2206 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 139:
#line 1100 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2212 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 140:
#line 1102 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2218 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 141:
#line 1104 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2224 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 142:
#line 1106 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2230 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 143:
#line 1108 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2236 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 144:
#line 1110 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2242 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 145:
#line 1112 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2248 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 146:
#line 1114 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2254 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 147:
#line 1116 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2260 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 148:
#line 1118 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2266 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 149:
#line 1120 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2272 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 150:
#line 1122 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2278 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 151:
#line 1124 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2284 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 152:
#line 1126 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2290 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 153:
#line 1128 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2296 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 154:
#line 1130 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2302 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 155:
#line 1132 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2308 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 156:
#line 1134 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2314 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 157:
#line 1136 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2320 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 158:
#line 1140 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2326 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 159:
#line 1141 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2332 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 160:
#line 1142 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2338 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 161:
#line 1146 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2344 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 162:
#line 1147 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2350 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 163:
#line 1151 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2356 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 164:
#line 1152 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2362 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 165:
#line 1156 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2368 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 166:
#line 1157 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2374 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 167:
#line 1167 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DropTableStmt::make_drop_table_stmt();
		check(stmt);
		DropTableStmt* drop_table_stmt = dynamic_cast<DropTableStmt*>(stmt.get());
		drop_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2386 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 168:
#line 1175 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DropDatabaseStmt::make_drop_database_stmt();
		check(stmt);
		DropDatabaseStmt* drop_database_stmt = dynamic_cast<DropDatabaseStmt*>(stmt.get());
		drop_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2398 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 169:
#line 1191 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(false);
    }
#line 2406 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 170:
#line 1195 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowTablesStmt::make_show_tables_stmt(yystack_[0].value.as < std::string > ());
	}
#line 2414 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 171:
#line 1199 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2426 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 172:
#line 1207 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = NULL;
	}
#line 2434 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 173:
#line 1211 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = NULL;
	}
#line 2442 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 174:
#line 1215 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		desc_table_stmt->is_show_table_statis = true;
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2455 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 175:
#line 1224 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		desc_table_stmt->is_show_column_statis = true;
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2468 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 176:
#line 1236 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
	}
#line 2476 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 177:
#line 1240 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = driver.get_global_database();
	}
#line 2484 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 178:
#line 1245 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = UseDatabaseStmt::make_use_database_stmt();
		check(stmt);
		UseDatabaseStmt* use_database_stmt = dynamic_cast<UseDatabaseStmt*>(stmt.get());
		use_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2496 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 179:
#line 1261 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2508 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 180:
#line 1269 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2520 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 181:
#line 1284 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as < std::string > ();
		analyze_stmt->table = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2533 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 182:
#line 1293 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as < std::string > ();
		analyze_stmt->table = "*";
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2546 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 183:
#line 1302 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = "*";
		analyze_stmt->table = "*";
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2559 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 184:
#line 1319 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(driver.get_global_database(), yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2570 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 185:
#line 1326 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2581 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 186:
#line 1335 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2587 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 187:
#line 1339 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2593 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 188:
#line 1343 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2599 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 189:
#line 1347 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2605 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 190:
#line 1351 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 2611 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 191:
#line 1355 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { str_to_lower(yystack_[0].value.as < std::string > ());yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2617 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 192:
#line 1359 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2623 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 193:
#line 1360 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2629 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 194:
#line 1364 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2635 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 195:
#line 1368 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2641 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;


#line 2645 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
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
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

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
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
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
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
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
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
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


  const short parser::yypact_ninf_ = -293;

  const short parser::yytable_ninf_ = -190;

  const short
  parser::yypact_[] =
  {
     294,   -10,   -56,   -24,    14,    24,    24,   -14,    72,    18,
       1,   151,    24,    24,    90,    73,  -293,   170,  -293,  -293,
    -293,  -293,  -293,  -293,  -293,  -293,  -293,  -293,  -293,   127,
      25,    24,    24,    24,  -293,  -293,   131,  -293,   135,  -293,
      24,   107,  -293,  -293,  -293,  -293,  -293,    24,   105,   150,
    -293,   172,   286,  -293,   106,   173,  -293,   181,  -293,   155,
     183,   185,  -293,  -293,  -293,  -293,    13,   226,   226,  -293,
     230,   231,  -293,   240,   188,    24,  -293,    85,  -293,    87,
       8,   249,   245,   286,   217,  -293,   286,   286,  -293,   257,
    -293,  -293,  -293,  -293,   244,   226,   244,  -293,   -13,  -293,
     392,   186,  -293,  -293,  -293,  -293,   248,  -293,   256,    75,
    -293,  -293,  -293,    24,    24,    24,    24,    24,  -293,    24,
     226,  -293,  -293,  -293,   260,   100,    24,   286,  -293,  -293,
    -293,    24,  -293,    24,  -293,   259,  -293,   218,   262,  -293,
      -4,   226,  -293,  -293,   170,   427,  -293,  -293,  -293,   286,
     236,   286,   286,   234,   286,   286,   286,   286,   286,   286,
     286,   286,   269,    56,   286,   286,   286,   286,    24,  -293,
    -293,   286,   286,   286,   286,   102,   222,  -293,  -293,  -293,
    -293,  -293,   -63,  -293,   285,  -293,  -293,  -293,  -293,  -293,
     110,  -293,  -293,   303,   493,  -293,  -293,   286,   270,   271,
     272,  -293,   274,   107,  -293,  -293,  -293,  -293,   512,   530,
     286,   286,   269,   403,   403,   403,   403,   403,   403,   134,
     182,   257,  -293,  -293,   282,  -293,   123,   123,  -293,  -293,
    -293,   146,   146,  -293,  -293,  -293,  -293,   275,  -293,  -293,
     449,   286,    24,  -293,   286,  -293,    24,  -293,  -293,   280,
     288,  -293,   290,  -293,  -293,  -293,  -293,   280,   280,  -293,
     290,   290,   290,   298,  -293,   298,  -293,   493,   160,   299,
     278,   218,  -293,   -11,  -293,   134,   194,  -293,   286,   169,
     493,  -293,  -293,  -293,   471,  -293,   493,  -293,   278,  -293,
     278,  -293,   278,  -293,  -293,  -293,  -293,  -293,  -293,   278,
    -293,  -293,  -293,   286,   286,   301,  -293,   107,   266,   286,
     186,  -293,   286,  -293,   187,   306,   307,   308,   493,   207,
    -293,  -293,   304,   279,   186,   493,  -293,   278,  -293,  -293,
    -293,  -293,   286,   286,   268,   314,   319,   493,   310,    38,
    -293,  -293,   286,  -293,  -293,   287,   319,   278,  -293,   326,
    -293,   278,  -293
  };

  const unsigned char
  parser::yydefact_[] =
  {
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     3,     0,    15,     4,
       5,     6,     7,     9,    10,     8,    12,    11,    13,     0,
       0,     0,     0,     0,   191,   180,     0,   184,   187,   179,
       0,     0,   129,   131,   132,   130,   128,     0,     0,     0,
      33,    32,     0,    22,     0,     0,   169,     0,   173,     0,
     177,     0,   178,   186,     1,     2,     0,     0,     0,    16,
       0,     0,   134,     0,    39,     0,   168,     0,   167,    57,
       0,    26,     0,     0,     0,   110,     0,     0,   102,     0,
     194,   195,   108,   109,     0,     0,     0,    98,     0,    34,
      54,    64,    87,    94,    95,    97,     0,   101,     0,   188,
     106,   105,   107,     0,     0,     0,     0,     0,   170,     0,
       0,    18,    21,    20,     0,     0,     0,     0,   127,   185,
     187,     0,    61,     0,    59,     0,   118,    28,     0,   114,
       0,     0,    88,    89,    98,     0,   192,    99,   193,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
     190,     0,     0,     0,     0,     0,     0,   175,   171,   172,
     174,   176,    39,   124,     0,   188,    19,   183,   182,   181,
       0,   135,   137,     0,    38,    60,    58,     0,   117,     0,
      27,    29,     0,     0,   100,    96,    35,    23,    78,    77,
       0,     0,     0,    69,    70,    71,    73,    72,    74,    75,
       0,     0,    85,    83,     0,    79,    65,    66,    67,    68,
      56,    90,    91,    92,    93,   104,   103,   102,   115,   116,
       0,     0,     0,   123,     0,   133,     0,   146,   155,   160,
     162,   142,   166,   139,   140,   141,   143,   160,   160,   148,
     166,   166,   166,   164,   156,   164,   138,   121,     0,     0,
       0,     0,    25,    39,    36,    76,     0,    84,     0,     0,
      62,    80,   111,   113,     0,   125,   126,   136,     0,   149,
       0,   147,     0,   152,   144,   145,   150,   151,   153,     0,
     154,   157,   119,     0,     0,     0,    30,     0,    41,     0,
      81,    86,     0,   112,     0,     0,     0,     0,   122,     0,
      31,    37,     0,    43,    82,    63,   159,     0,   161,   165,
     163,   120,     0,     0,    44,     0,    40,    42,     0,    46,
      45,   158,     0,    47,    48,    49,    53,     0,    17,    51,
      52,     0,    50
  };

  const short
  parser::yypgoto_[] =
  {
    -293,  -293,  -293,     4,     0,    10,  -293,  -293,  -293,    84,
     309,   291,  -293,  -177,  -293,  -293,  -293,  -293,  -293,    11,
    -293,   214,  -189,  -292,    26,   153,   -80,  -293,  -293,  -293,
    -293,  -293,  -293,   361,  -293,    66,   365,  -293,   133,   368,
    -293,  -293,  -293,  -293,   132,  -293,  -293,   -20,  -293,   112,
       2,  -293,  -293,  -293,  -293,  -293,  -293,    53,    -9,    30,
    -106,  -293,   212,    -3,  -293,   126,  -245
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,    14,    15,    16,    97,    18,    51,   138,   200,   201,
      52,    98,   273,   128,   323,   334,   339,   345,   348,   349,
     340,    99,    78,   279,   100,   223,   101,   102,   103,   104,
     105,    53,   241,    19,   198,   268,    20,   182,   183,    21,
      22,    46,    23,   190,   191,   192,   266,   289,   291,   300,
     293,    24,    25,   118,    26,    27,    28,    79,    36,   106,
     107,   108,   169,   109,   110,   111,   112
  };

  const short
  parser::yytable_[] =
  {
      17,    17,    38,    38,    62,   243,   142,   143,    17,    38,
      63,    29,    42,   184,   274,   149,     1,   307,   127,    30,
     193,    71,    72,    31,   149,   305,    48,    63,    63,    38,
      38,    76,   242,    40,     1,    37,    37,    63,    38,     1,
     336,    77,    37,   314,    38,   315,   150,   316,    49,    70,
     346,    32,   120,    50,   317,   203,    34,    34,    35,    39,
      10,    41,    37,    37,   224,    61,   121,   122,   123,   236,
     127,    37,   130,    33,   130,   225,   130,    37,    10,   343,
      17,   220,   335,    47,   136,    73,    74,   135,   344,   144,
      64,   231,   232,   233,   234,   147,   308,   170,     1,    29,
      80,  -189,   350,    65,  -187,   129,   350,   132,   181,   134,
      38,    38,    38,    38,    63,   145,   185,    34,   321,    34,
     186,     4,   130,   185,   188,   131,   235,   133,   130,    81,
     130,   276,    34,     1,    34,     9,   184,   245,   246,    34,
     193,   204,    10,    37,    37,    37,    37,   166,   167,    12,
     161,   162,   163,   194,    69,   189,   164,   165,   166,   167,
      75,   195,   222,   196,  -186,   170,   177,   178,   179,   180,
     173,   174,   185,    66,    67,    68,    82,   208,   209,   113,
     213,   214,   215,   216,   217,   218,   219,   302,   303,   278,
     226,   227,   228,   229,    54,    55,   311,   312,   310,    56,
      38,   309,   240,    77,   171,   172,   173,   174,   171,   172,
     173,   174,   222,    57,   326,   327,   171,   172,   173,   174,
     146,   144,   148,   267,    50,    58,    59,    60,   116,   324,
      84,    29,   114,    37,   331,   303,   275,   294,   295,   185,
     115,    85,   117,   185,    86,    87,   237,   280,    89,   210,
     211,   212,     1,    90,    34,    91,   119,    92,    93,   124,
     125,   238,   296,   297,   298,    84,   126,   284,    94,   127,
     286,   137,   139,   141,   239,    90,    85,   175,    95,    86,
      87,    88,   176,    89,   187,   197,   202,   199,    90,    34,
      91,   207,    92,    93,    84,   221,   244,   270,   269,   272,
     271,   281,   282,    94,    38,    85,   288,    77,    86,    87,
      88,    91,    89,    95,   290,    96,   292,    90,    34,    91,
       1,    92,    93,   247,   299,   304,   322,    10,   320,   318,
     267,     2,    94,   328,   329,   330,   338,    37,   325,     3,
     333,   341,    95,     4,     5,     6,   332,   312,     7,   248,
      96,     8,   342,   347,   351,   306,   249,     9,   280,   337,
      83,   250,   352,   206,    10,   277,    11,   251,   280,    43,
     319,    12,    13,    44,   140,   285,    45,   301,   287,    96,
     230,     0,     0,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,     0,     0,   164,   165,   166,   167,   160,   161,
     162,   163,     0,     0,    34,   164,   165,   166,   167,     0,
       0,     0,   168,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,     0,     0,     0,   164,
     165,   166,   167,     0,   205,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,     0,     0,
       0,   164,   165,   166,   167,     0,   283,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
       0,     0,     0,   164,   165,   166,   167,     0,   313,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,     0,     0,     0,   164,   165,   166,   167,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,     0,     0,   164,   165,   166,   167,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
       0,     0,   164,   165,   166,   167
  };

  const short
  parser::yycheck_[] =
  {
       0,     1,     5,     6,    13,   182,    86,    87,     8,    12,
      13,     1,     8,   119,   203,    28,    26,    28,    81,    75,
     126,    30,    31,    47,    28,   270,    25,    30,    31,    32,
      33,    40,    95,    47,    26,     5,     6,    40,    41,    26,
     332,    41,    12,   288,    47,   290,    59,   292,    47,    24,
     342,    75,    39,    52,   299,    59,    32,    32,     5,     6,
      70,    75,    32,    33,     8,    12,    66,    67,    68,   175,
      81,    41,    75,    59,    77,    19,    79,    47,    70,    41,
      80,   161,   327,    65,    80,    32,    33,    79,    50,    89,
       0,   171,   172,   173,   174,    95,   273,   100,    26,    89,
      47,    26,   347,    30,    29,    75,   351,    77,   117,    79,
     113,   114,   115,   116,   117,    89,   119,    32,   307,    32,
     120,    49,   125,   126,    24,    40,    24,    40,   131,    24,
     133,   211,    32,    26,    32,    63,   242,    27,    28,    32,
     246,   141,    70,   113,   114,   115,   116,    24,    25,    77,
      16,    17,    18,   127,    27,   125,    22,    23,    24,    25,
      29,   131,   162,   133,    29,   168,   113,   114,   115,   116,
      24,    25,   175,     3,     4,     5,    26,   151,   152,    73,
     154,   155,   156,   157,   158,   159,   160,    27,    28,     7,
     164,   165,   166,   167,    43,    44,    27,    28,   278,    48,
     203,     7,   176,   203,    22,    23,    24,    25,    22,    23,
      24,    25,   212,    62,    27,    28,    22,    23,    24,    25,
      94,   221,    96,   197,    52,    74,    75,    76,    73,   309,
       8,   221,    59,   203,    27,    28,   210,   257,   258,   242,
      59,    19,    59,   246,    22,    23,    24,   221,    26,    15,
      16,    17,    26,    31,    32,    33,    71,    35,    36,    29,
      29,    39,   260,   261,   262,     8,    26,   241,    46,    81,
     244,    22,    27,    56,    52,    31,    19,    29,    56,    22,
      23,    24,    26,    26,    24,    26,    24,    69,    31,    32,
      33,    55,    35,    36,     8,    26,    11,    26,    28,    25,
      28,    19,    27,    46,   307,    19,    26,   307,    22,    23,
      24,    33,    26,    56,    26,    93,    26,    31,    32,    33,
      26,    35,    36,    20,    26,    26,    60,    70,    27,   303,
     304,    37,    46,    27,    27,    27,    68,   307,   312,    45,
      61,    27,    56,    49,    50,    51,    42,    28,    54,    46,
      93,    57,    42,    66,    28,   271,    53,    63,   332,   333,
      51,    58,   351,   149,    70,   212,    72,    64,   342,     8,
     304,    77,    78,     8,    83,   242,     8,   265,   246,    93,
     168,    -1,    -1,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    15,    16,
      17,    18,    -1,    -1,    32,    22,    23,    24,    25,    -1,
      -1,    -1,    40,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    -1,    27,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    22,    23,    24,    25,    -1,    27,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    22,    23,    24,    25,    -1,    27,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    22,    23,    24,    25,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    23,    24,    25,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    22,    23,    24,    25
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    26,    37,    45,    49,    50,    51,    54,    57,    63,
      70,    72,    77,    78,    97,    98,    99,   100,   101,   129,
     132,   135,   136,   138,   147,   148,   150,   151,   152,   101,
      75,    47,    75,    59,    32,   153,   154,   155,   159,   153,
      47,    75,    99,   129,   132,   135,   137,    65,    25,    47,
      52,   102,   106,   127,    43,    44,    48,    62,    74,    75,
      76,   153,   154,   159,     0,    30,     3,     4,     5,    27,
      24,   154,   154,   153,   153,    29,   154,   100,   118,   153,
     153,    24,    26,   106,     8,    19,    22,    23,    24,    26,
      31,    33,    35,    36,    46,    56,    93,   100,   107,   117,
     120,   122,   123,   124,   125,   126,   155,   156,   157,   159,
     160,   161,   162,    73,    59,    59,    73,    59,   149,    71,
      39,   100,   100,   100,    29,    29,    26,    81,   109,   155,
     159,    40,   155,    40,   155,    79,    99,    22,   103,    27,
     107,    56,   122,   122,   100,   120,   161,   100,   161,    28,
      59,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    22,    23,    24,    25,    40,   158,
     159,    22,    23,    24,    25,    29,    26,   153,   153,   153,
     153,   154,   133,   134,   156,   159,   100,    24,    24,   155,
     139,   140,   141,   156,   120,   155,   155,    26,   130,    69,
     104,   105,    24,    59,   100,    27,   117,    55,   120,   120,
      15,    16,    17,   120,   120,   120,   120,   120,   120,   120,
     122,    26,   100,   121,     8,    19,   120,   120,   120,   120,
     158,   122,   122,   122,   122,    24,   156,    24,    39,    52,
     120,   128,    95,   109,    11,    27,    28,    20,    46,    53,
      58,    64,    80,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   142,   120,   131,    28,
      26,    28,    25,   108,   118,   120,   122,   121,     7,   119,
     120,    19,    27,    27,   120,   134,   120,   140,    26,   143,
      26,   144,    26,   146,   143,   143,   146,   146,   146,    26,
     145,   145,    27,    28,    26,   162,   105,    28,   109,     7,
     122,    27,    28,    27,   162,   162,   162,   162,   120,   131,
      27,   118,    60,   110,   122,   120,    27,    28,    27,    27,
      27,    27,    42,    61,   111,   162,   119,   120,    68,   112,
     116,    27,    42,    41,    50,   113,   119,    66,   114,   115,
     162,    28,   115
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    96,    97,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    99,   100,   101,   101,   101,
     101,   101,   101,   101,   102,   102,   103,   103,   104,   104,
     104,   105,   106,   106,   107,   107,   108,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   113,   113,   113,   114,
     114,   114,   115,   116,   117,   117,   117,   118,   118,   118,
     118,   118,   119,   119,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   121,   121,   122,   122,   122,
     122,   122,   122,   122,   123,   123,   123,   123,   123,   123,
     123,   124,   124,   124,   124,   125,   125,   125,   125,   125,
     125,   126,   126,   126,   127,   128,   128,   129,   129,   130,
     130,   131,   131,   132,   133,   133,   134,   135,   136,   137,
     137,   137,   137,   138,   138,   139,   139,   140,   141,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   143,   143,
     143,   144,   144,   145,   145,   146,   146,   147,   147,   148,
     148,   148,   148,   148,   148,   148,   149,   149,   150,   151,
     151,   152,   152,   152,   153,   153,   154,   155,   156,   157,
     158,   159,   160,   160,   161,   162
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     3,    12,     3,     4,
       3,     3,     2,     5,     0,     5,     0,     2,     0,     1,
       3,     4,     0,     1,     1,     3,     1,     3,     2,     0,
       3,     0,     2,     0,     0,     1,     0,     1,     1,     0,
       4,     2,     1,     3,     1,     2,     3,     1,     3,     2,
       3,     2,     1,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     3,     3,     3,
       4,     5,     6,     3,     4,     1,     3,     1,     2,     2,
       3,     3,     3,     3,     1,     1,     3,     1,     1,     2,
       3,     1,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     4,     5,     4,     3,     1,     1,     5,     4,     3,
       5,     1,     3,     5,     1,     3,     3,     4,     2,     1,
       1,     1,     1,     6,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     1,     2,
       2,     2,     2,     2,     2,     1,     1,     2,     5,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     3,     2,
       3,     4,     4,     2,     4,     4,     2,     0,     2,     2,
       2,     5,     5,     5,     1,     3,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END", "error", "$undefined", "UNION", "EXCEPT", "INTERSECT", "OR",
  "AND", "NOT", "CMP_LE", "CMP_LT", "CMP_EQ", "CMP_GT", "CMP_GE", "CMP_NE",
  "LIKE", "BETWEEN", "IN", "IS", "NULLX", "BOOL", "UMINUS", "\"+\"",
  "\"-\"", "\"*\"", "\"/\"", "\"(\"", "\")\"", "\",\"", "\".\"", "\";\"",
  "STRING", "IDENT", "NUMERIC", "TIMESTAMP", "FALSE", "TRUE", "ANALYZE",
  "ANY", "ALL", "AS", "ASC", "BY", "COLUMN", "COLUMNS", "CREATE",
  "DATETIME", "DATABASE", "DATABASES", "DELETE", "DESC", "DESCRIBE",
  "DISTINCT", "DOUBLE", "DROP", "DUAL", "EXISTS", "EXPLAIN", "FLOAT",
  "FROM", "GROUP", "HAVING", "INDEX", "INSERT", "INT", "INTO", "LIMIT",
  "NUMBER", "ORDER", "PARALLEL", "SELECT", "SET", "SHOW", "STATIS",
  "STATUS", "TABLE", "TABLES", "UPDATE", "USING", "VALUES", "VARCHAR",
  "WHERE", "TINYINT", "SMALLINT", "MEDIUMINT", "BIGINT", "DECIMAL",
  "NUMERIC_SYM", "REAL", "CHAR", "BINARY", "VARBINARY", "TIMESTAMP_SYM",
  "DATE", "TIME", "','", "$accept", "sql_stmt", "stmt", "select_stmt",
  "select_with_parens", "select_no_parens", "opt_hint", "opt_hint_body",
  "opt_hint_list", "hint_item", "opt_distinct", "select_expr_list",
  "from_list", "opt_where", "opt_groupby", "opt_having", "opt_order_by",
  "opt_asc_desc", "opt_select_limit", "limit_expr", "order_by",
  "projection", "table_factor", "expr_list", "expr", "in_expr",
  "arith_expr", "simple_expr", "column_ref", "expr_const", "func_expr",
  "simple_function_expr", "distinct_or_all", "insert_stmt",
  "insert_vals_list", "insert_vals", "update_stmt", "update_asgn_list",
  "update_asgn_factor", "delete_stmt", "explain_stmt", "explainable_stmt",
  "create_stmt", "table_element_list", "table_element",
  "column_definition", "data_type", "opt_decimal", "opt_float",
  "opt_time_precision", "opt_char_length", "drop_stmt", "show_stmt",
  "op_from_database", "use_stmt", "desc_stmt", "analyze_stmt",
  "relation_factor", "database_name", "relation_name", "column_name",
  "function_name", "column_label", "ident", "datetime", "string", "number", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
  parser::yyrline_[] =
  {
       0,   289,   289,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   317,   324,   334,   354,   359,
     364,   369,   374,   378,   394,   395,   401,   402,   408,   409,
     413,   419,   427,   428,   432,   437,   445,   450,   458,   459,
     463,   464,   468,   469,   473,   474,   478,   479,   480,   484,
     485,   492,   502,   509,   516,   524,   532,   543,   547,   555,
     563,   571,   586,   591,   600,   604,   609,   614,   619,   624,
     629,   634,   639,   644,   649,   654,   659,   664,   669,   674,
     679,   684,   689,   694,   699,   709,   713,   720,   724,   729,
     734,   739,   744,   749,   757,   761,   765,   769,   773,   777,
     781,   789,   796,   802,   810,   820,   829,   838,   847,   856,
     865,   877,   887,   896,   907,   914,   918,   929,   939,   946,
     950,   956,   961,   974,   987,   992,  1000,  1015,  1032,  1040,
    1041,  1042,  1043,  1052,  1061,  1072,  1076,  1083,  1090,  1099,
    1101,  1103,  1105,  1107,  1109,  1111,  1113,  1115,  1117,  1119,
    1121,  1123,  1125,  1127,  1129,  1131,  1133,  1135,  1140,  1141,
    1142,  1146,  1147,  1151,  1152,  1156,  1157,  1166,  1174,  1190,
    1194,  1198,  1206,  1210,  1214,  1223,  1235,  1240,  1244,  1260,
    1268,  1283,  1292,  1301,  1318,  1325,  1335,  1339,  1343,  1347,
    1351,  1355,  1359,  1360,  1364,  1368
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
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 3 "D:/CatDB/parser/lex_yacc/sql_parser.y"
} // CatDB
#line 3335 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"

#line 1370 "D:/CatDB/parser/lex_yacc/sql_parser.y"


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
