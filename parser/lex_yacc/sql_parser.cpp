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
      case 94: // sql_stmt
      case 95: // stmt
      case 96: // select_stmt
      case 97: // select_with_parens
      case 98: // select_no_parens
      case 100: // select_expr_list
      case 101: // from_list
      case 102: // opt_where
      case 103: // opt_groupby
      case 104: // opt_having
      case 105: // opt_order_by
      case 107: // opt_select_limit
      case 109: // order_by
      case 110: // projection
      case 111: // table_factor
      case 112: // expr_list
      case 113: // expr
      case 114: // in_expr
      case 115: // arith_expr
      case 116: // simple_expr
      case 117: // column_ref
      case 118: // expr_const
      case 119: // func_expr
      case 122: // insert_stmt
      case 123: // insert_vals_list
      case 124: // insert_vals
      case 125: // update_stmt
      case 126: // update_asgn_list
      case 127: // update_asgn_factor
      case 128: // delete_stmt
      case 129: // explain_stmt
      case 130: // explainable_stmt
      case 131: // create_stmt
      case 132: // table_element_list
      case 133: // table_element
      case 134: // column_definition
      case 140: // drop_stmt
      case 141: // show_stmt
      case 143: // use_stmt
      case 144: // desc_stmt
      case 145: // analyze_stmt
      case 146: // relation_factor
        value.YY_MOVE_OR_COPY< Stmt_s > (YY_MOVE (that.value));
        break;

      case 99: // opt_distinct
      case 106: // opt_asc_desc
      case 121: // distinct_or_all
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 108: // limit_expr
      case 135: // data_type
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 120: // simple_function_expr
      case 142: // op_from_database
      case 147: // database_name
      case 148: // relation_name
      case 149: // column_name
      case 150: // function_name
      case 151: // column_label
      case 152: // ident
      case 153: // datetime
      case 154: // string
      case 155: // number
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
      case 94: // sql_stmt
      case 95: // stmt
      case 96: // select_stmt
      case 97: // select_with_parens
      case 98: // select_no_parens
      case 100: // select_expr_list
      case 101: // from_list
      case 102: // opt_where
      case 103: // opt_groupby
      case 104: // opt_having
      case 105: // opt_order_by
      case 107: // opt_select_limit
      case 109: // order_by
      case 110: // projection
      case 111: // table_factor
      case 112: // expr_list
      case 113: // expr
      case 114: // in_expr
      case 115: // arith_expr
      case 116: // simple_expr
      case 117: // column_ref
      case 118: // expr_const
      case 119: // func_expr
      case 122: // insert_stmt
      case 123: // insert_vals_list
      case 124: // insert_vals
      case 125: // update_stmt
      case 126: // update_asgn_list
      case 127: // update_asgn_factor
      case 128: // delete_stmt
      case 129: // explain_stmt
      case 130: // explainable_stmt
      case 131: // create_stmt
      case 132: // table_element_list
      case 133: // table_element
      case 134: // column_definition
      case 140: // drop_stmt
      case 141: // show_stmt
      case 143: // use_stmt
      case 144: // desc_stmt
      case 145: // analyze_stmt
      case 146: // relation_factor
        value.move< Stmt_s > (YY_MOVE (that.value));
        break;

      case 99: // opt_distinct
      case 106: // opt_asc_desc
      case 121: // distinct_or_all
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 108: // limit_expr
      case 135: // data_type
        value.move< int > (YY_MOVE (that.value));
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 120: // simple_function_expr
      case 142: // op_from_database
      case 147: // database_name
      case 148: // relation_name
      case 149: // column_name
      case 150: // function_name
      case 151: // column_label
      case 152: // ident
      case 153: // datetime
      case 154: // string
      case 155: // number
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
      case 94: // sql_stmt
      case 95: // stmt
      case 96: // select_stmt
      case 97: // select_with_parens
      case 98: // select_no_parens
      case 100: // select_expr_list
      case 101: // from_list
      case 102: // opt_where
      case 103: // opt_groupby
      case 104: // opt_having
      case 105: // opt_order_by
      case 107: // opt_select_limit
      case 109: // order_by
      case 110: // projection
      case 111: // table_factor
      case 112: // expr_list
      case 113: // expr
      case 114: // in_expr
      case 115: // arith_expr
      case 116: // simple_expr
      case 117: // column_ref
      case 118: // expr_const
      case 119: // func_expr
      case 122: // insert_stmt
      case 123: // insert_vals_list
      case 124: // insert_vals
      case 125: // update_stmt
      case 126: // update_asgn_list
      case 127: // update_asgn_factor
      case 128: // delete_stmt
      case 129: // explain_stmt
      case 130: // explainable_stmt
      case 131: // create_stmt
      case 132: // table_element_list
      case 133: // table_element
      case 134: // column_definition
      case 140: // drop_stmt
      case 141: // show_stmt
      case 143: // use_stmt
      case 144: // desc_stmt
      case 145: // analyze_stmt
      case 146: // relation_factor
        value.move< Stmt_s > (that.value);
        break;

      case 99: // opt_distinct
      case 106: // opt_asc_desc
      case 121: // distinct_or_all
        value.move< bool > (that.value);
        break;

      case 108: // limit_expr
      case 135: // data_type
        value.move< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 120: // simple_function_expr
      case 142: // op_from_database
      case 147: // database_name
      case 148: // relation_name
      case 149: // column_name
      case 150: // function_name
      case 151: // column_label
      case 152: // ident
      case 153: // datetime
      case 154: // string
      case 155: // number
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
      case 94: // sql_stmt
      case 95: // stmt
      case 96: // select_stmt
      case 97: // select_with_parens
      case 98: // select_no_parens
      case 100: // select_expr_list
      case 101: // from_list
      case 102: // opt_where
      case 103: // opt_groupby
      case 104: // opt_having
      case 105: // opt_order_by
      case 107: // opt_select_limit
      case 109: // order_by
      case 110: // projection
      case 111: // table_factor
      case 112: // expr_list
      case 113: // expr
      case 114: // in_expr
      case 115: // arith_expr
      case 116: // simple_expr
      case 117: // column_ref
      case 118: // expr_const
      case 119: // func_expr
      case 122: // insert_stmt
      case 123: // insert_vals_list
      case 124: // insert_vals
      case 125: // update_stmt
      case 126: // update_asgn_list
      case 127: // update_asgn_factor
      case 128: // delete_stmt
      case 129: // explain_stmt
      case 130: // explainable_stmt
      case 131: // create_stmt
      case 132: // table_element_list
      case 133: // table_element
      case 134: // column_definition
      case 140: // drop_stmt
      case 141: // show_stmt
      case 143: // use_stmt
      case 144: // desc_stmt
      case 145: // analyze_stmt
      case 146: // relation_factor
        yylhs.value.emplace< Stmt_s > ();
        break;

      case 99: // opt_distinct
      case 106: // opt_asc_desc
      case 121: // distinct_or_all
        yylhs.value.emplace< bool > ();
        break;

      case 108: // limit_expr
      case 135: // data_type
        yylhs.value.emplace< int > ();
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 120: // simple_function_expr
      case 142: // op_from_database
      case 147: // database_name
      case 148: // relation_name
      case 149: // column_name
      case 150: // function_name
      case 151: // column_label
      case 152: // ident
      case 153: // datetime
      case 154: // string
      case 155: // number
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
#line 287 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		driver.result = yystack_[1].value.as < Stmt_s > ();
		YYACCEPT;
    }
#line 977 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 3:
#line 294 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 983 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 4:
#line 295 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 989 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 5:
#line 296 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 995 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 6:
#line 297 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1001 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 7:
#line 298 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1007 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 8:
#line 299 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1013 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 9:
#line 300 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1019 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 10:
#line 301 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1025 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 11:
#line 302 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1031 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 12:
#line 303 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1037 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 13:
#line 304 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1043 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 14:
#line 305 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yyerror("unknow stmt"); }
#line 1049 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 15:
#line 315 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1057 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 16:
#line 322 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = QueryStmt::make_query_stmt();
		QueryStmt* query = dynamic_cast<QueryStmt*>(stmt.get());
		query->query_stmt = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1068 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 17:
#line 335 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建select stmt
		Stmt_s stmt = SelectStmt::make_select_stmt();
		SelectStmt* select_stmt = dynamic_cast<SelectStmt*>(stmt.get());
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
#line 1088 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 18:
#line 351 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建union二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION);
    }
#line 1097 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 19:
#line 356 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建union all二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION_ALL);
    }
#line 1106 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 20:
#line 361 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建intersect二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_INTERSECT);
    }
#line 1115 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 21:
#line 366 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建except二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXCEPT);
    }
#line 1124 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 22:
#line 371 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(true);
	}
#line 1132 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 23:
#line 375 "D:/CatDB/parser/lex_yacc/sql_parser.y"
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
#line 1149 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 24:
#line 390 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1155 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 25:
#line 391 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1161 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 26:
#line 396 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建select list
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1170 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 27:
#line 401 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//把输出表达式加入到select list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1179 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 28:
#line 409 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建from list 
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1188 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 29:
#line 414 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//把表加入到from list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1197 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 30:
#line 421 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1203 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 31:
#line 422 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1209 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 32:
#line 426 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1215 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 33:
#line 427 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1221 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 34:
#line 431 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1227 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 35:
#line 432 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1233 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 36:
#line 436 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1239 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 37:
#line 437 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1245 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 38:
#line 441 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1251 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 39:
#line 442 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1257 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 40:
#line 443 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1263 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 41:
#line 447 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {yylhs.value.as < Stmt_s > () = NULL;}
#line 1269 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 42:
#line 449 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as < int > (), yystack_[2].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1280 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 43:
#line 456 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1291 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 44:
#line 466 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < int > () = std::stoi(yystack_[0].value.as < std::string > ());
	}
#line 1299 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 45:
#line 473 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1307 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 46:
#line 480 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
	//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[0].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = stmt->to_string();
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1319 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 47:
#line 488 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1331 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 48:
#line 496 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1343 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 49:
#line 507 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1351 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 50:
#line 511 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1363 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 51:
#line 519 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1375 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 52:
#line 527 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1387 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 53:
#line 535 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1399 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 54:
#line 550 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表达式列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1408 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 55:
#line 555 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1417 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 56:
#line 564 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1425 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 57:
#line 568 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建加法二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1434 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 58:
#line 573 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建减法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1443 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 59:
#line 578 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建乘法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1452 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 60:
#line 583 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建除法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1461 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 61:
#line 588 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LE);
	}
#line 1470 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 62:
#line 593 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LT);
	}
#line 1479 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 63:
#line 598 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
	}
#line 1488 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 64:
#line 603 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GE);
	}
#line 1497 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 65:
#line 608 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GT);
	}
#line 1506 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 66:
#line 613 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NE);
	}
#line 1515 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 67:
#line 618 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LIKE);
	}
#line 1524 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 68:
#line 623 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_LIKE);
	}
#line 1533 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 69:
#line 628 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_AND);
    }
#line 1542 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 70:
#line 633 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_OR);
    }
#line 1551 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 71:
#line 638 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), ExprStmt::OP_IS_NULL);
    }
#line 1560 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 72:
#line 643 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), ExprStmt::OP_IS_NOT_NULL);
    }
#line 1569 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 73:
#line 648 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_BETWEEN);
    }
#line 1578 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 74:
#line 653 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[5].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_BETWEEN);
    }
#line 1587 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 75:
#line 658 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_IN);
    }
#line 1596 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 76:
#line 663 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_IN);
    }
#line 1605 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 77:
#line 673 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1613 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 78:
#line 677 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1621 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 79:
#line 684 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1629 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 80:
#line 688 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//正数表达式
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1638 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 81:
#line 693 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MINUS);
    }
#line 1647 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 82:
#line 698 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1656 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 83:
#line 703 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1665 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 84:
#line 708 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1674 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 85:
#line 713 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1683 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 86:
#line 721 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1691 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 87:
#line 725 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		 yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1699 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 88:
#line 729 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1707 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 89:
#line 733 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
      	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1715 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 90:
#line 737 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1723 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 91:
#line 741 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXISTS);
    }
#line 1731 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 92:
#line 745 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_EXISTS);
    }
#line 1740 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 93:
#line 753 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1751 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 94:
#line 760 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1761 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 95:
#line 766 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1772 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 96:
#line 774 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), "*");
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1783 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 97:
#line 784 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1796 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 98:
#line 793 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = DateTime::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1809 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 99:
#line 802 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1822 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 100:
#line 811 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1835 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 101:
#line 820 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1848 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 102:
#line 829 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1861 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 103:
#line 841 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->aggr_expr = col;
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1875 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 104:
#line 851 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[4].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->distinct = yystack_[2].value.as < bool > ();
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1888 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 105:
#line 860 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1900 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 106:
#line 871 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = "database";
    }
#line 1908 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 107:
#line 878 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < bool > () = false;
    }
#line 1916 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 108:
#line 882 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < bool > () = true;
    }
#line 1924 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 109:
#line 893 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建insert stmt
		Stmt_s stmt = InsertStmt::make_insert_stmt();
		check(stmt);
		InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(stmt.get());
		insert_stmt->table = yystack_[2].value.as < Stmt_s > ();
		insert_stmt->values = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1938 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 110:
#line 903 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yyerror("insert into table from query not support yet!");
		YYABORT;
    }
#line 1947 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 111:
#line 910 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
    }
#line 1955 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 112:
#line 914 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
	}
#line 1963 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 113:
#line 920 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1972 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 114:
#line 925 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1981 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 115:
#line 938 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = UpdateStmt::make_update_stmt();
		check(stmt);
		UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(stmt.get());
		update_stmt->table = yystack_[3].value.as < Stmt_s > ();
		update_stmt->update_asgn_stmt = yystack_[1].value.as < Stmt_s > ();
		update_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1995 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 116:
#line 951 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2004 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 117:
#line 956 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2013 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 118:
#line 964 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as < std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as < Stmt_s > (), col, yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
    }
#line 2024 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 119:
#line 979 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DeleteStmt::make_delete_stmt();
		check(stmt);
		DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(stmt.get());
		delete_stmt->table = yystack_[1].value.as < Stmt_s > ();
		delete_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2037 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 120:
#line 996 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
		yyerror("explain not support yet!");
		YYABORT;
    }
#line 2047 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 121:
#line 1004 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2053 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 122:
#line 1005 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2059 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 123:
#line 1006 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2065 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 124:
#line 1007 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2071 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 125:
#line 1017 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = CreateTableStmt::make_create_table_stmt();
		check(stmt);
		CreateTableStmt* create_table_stmt = dynamic_cast<CreateTableStmt*>(stmt.get());
		create_table_stmt->table = yystack_[3].value.as < Stmt_s > ();
		create_table_stmt->column_define_list = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2084 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 126:
#line 1026 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = CreateDatabaseStmt::make_create_database_stmt();
		check(stmt);
		CreateDatabaseStmt* create_database_stmt = dynamic_cast<CreateDatabaseStmt*>(stmt.get());
		create_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2096 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 127:
#line 1037 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2104 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 128:
#line 1041 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2112 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 129:
#line 1048 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 2120 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 130:
#line 1055 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as < std::string > (), yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2130 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 131:
#line 1064 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2136 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 132:
#line 1066 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2142 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 133:
#line 1068 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2148 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 134:
#line 1070 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2154 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 135:
#line 1072 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2160 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 136:
#line 1074 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2166 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 137:
#line 1076 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2172 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 138:
#line 1078 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2178 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 139:
#line 1080 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2184 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 140:
#line 1082 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2190 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 141:
#line 1084 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2196 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 142:
#line 1086 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2202 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 143:
#line 1088 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2208 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 144:
#line 1090 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2214 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 145:
#line 1092 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2220 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 146:
#line 1094 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2226 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 147:
#line 1096 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2232 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 148:
#line 1098 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2238 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 149:
#line 1100 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2244 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 150:
#line 1104 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2250 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 151:
#line 1105 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2256 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 152:
#line 1106 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2262 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 153:
#line 1110 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2268 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 154:
#line 1111 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2274 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 155:
#line 1115 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2280 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 156:
#line 1116 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2286 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 157:
#line 1120 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2292 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 158:
#line 1121 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2298 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 159:
#line 1131 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DropTableStmt::make_drop_table_stmt();
		check(stmt);
		DropTableStmt* drop_table_stmt = dynamic_cast<DropTableStmt*>(stmt.get());
		drop_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2310 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 160:
#line 1139 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DropDatabaseStmt::make_drop_database_stmt();
		check(stmt);
		DropDatabaseStmt* drop_database_stmt = dynamic_cast<DropDatabaseStmt*>(stmt.get());
		drop_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2322 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 161:
#line 1155 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(false);
    }
#line 2330 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 162:
#line 1159 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowTablesStmt::make_show_tables_stmt(yystack_[0].value.as < std::string > ());
	}
#line 2338 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 163:
#line 1163 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2350 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 164:
#line 1171 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = NULL;
	}
#line 2358 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 165:
#line 1175 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = NULL;
	}
#line 2366 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 166:
#line 1182 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
	}
#line 2374 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 167:
#line 1186 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = driver.get_global_database();
	}
#line 2382 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 168:
#line 1191 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = UseDatabaseStmt::make_use_database_stmt();
		check(stmt);
		UseDatabaseStmt* use_database_stmt = dynamic_cast<UseDatabaseStmt*>(stmt.get());
		use_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2394 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 169:
#line 1207 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2406 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 170:
#line 1215 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2418 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 171:
#line 1230 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as < std::string > ();
		analyze_stmt->table = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2431 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 172:
#line 1239 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as < std::string > ();
		analyze_stmt->table = "*";
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2444 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 173:
#line 1248 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = "*";
		analyze_stmt->table = "*";
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2457 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 174:
#line 1265 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(driver.get_global_database(), yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2468 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 175:
#line 1272 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2479 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 176:
#line 1281 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2485 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 177:
#line 1285 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2491 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 178:
#line 1289 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2497 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 179:
#line 1293 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2503 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 180:
#line 1297 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 2509 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 181:
#line 1301 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { str_to_lower(yystack_[0].value.as < std::string > ());yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2515 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 182:
#line 1305 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2521 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 183:
#line 1306 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2527 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 184:
#line 1310 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2533 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 185:
#line 1314 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2539 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;


#line 2543 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"

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


  const short parser::yypact_ninf_ = -289;

  const short parser::yytable_ninf_ = -180;

  const short
  parser::yypact_[] =
  {
     364,   -19,   -50,   -28,   -32,    43,    43,   -25,    85,    22,
      -8,   -34,    43,    43,   104,    61,  -289,   158,  -289,  -289,
    -289,  -289,  -289,  -289,  -289,  -289,  -289,  -289,  -289,    82,
      24,    43,    43,    43,  -289,  -289,    99,  -289,   102,  -289,
      43,    -1,  -289,  -289,  -289,  -289,  -289,    43,   112,  -289,
     312,  -289,   113,  -289,   132,  -289,   134,    88,  -289,  -289,
    -289,  -289,    -7,   152,   152,  -289,   165,   168,  -289,   180,
     130,    43,  -289,    42,  -289,    70,   -10,   176,   162,  -289,
     312,   312,  -289,   243,  -289,  -289,  -289,  -289,   192,   152,
     192,  -289,   -18,  -289,   434,   197,  -289,  -289,  -289,  -289,
     206,  -289,   215,    55,  -289,  -289,  -289,    43,    43,    43,
    -289,    43,   152,  -289,  -289,  -289,   233,   105,    43,   312,
    -289,  -289,  -289,    43,  -289,    43,  -289,   217,  -289,  -289,
     152,  -289,  -289,   158,   469,  -289,  -289,  -289,   312,   -12,
     312,   312,   184,   312,   312,   312,   312,   312,   312,   312,
     312,   232,    53,   312,   312,   312,   312,    43,  -289,  -289,
     312,   312,   312,   312,   108,   273,  -289,  -289,  -289,   -24,
    -289,   248,  -289,  -289,  -289,  -289,  -289,   107,  -289,  -289,
     297,   535,  -289,  -289,   312,   235,  -289,  -289,  -289,  -289,
     -11,  -289,   554,   572,   312,   312,   232,   151,   151,   151,
     151,   151,   151,   209,   120,   243,  -289,  -289,   241,  -289,
      45,    45,  -289,  -289,  -289,   125,   125,  -289,  -289,  -289,
    -289,   234,  -289,  -289,   491,   312,    43,  -289,   312,  -289,
      43,  -289,  -289,   238,   242,  -289,   244,  -289,  -289,  -289,
    -289,   238,   238,  -289,   244,   244,   244,   245,  -289,   245,
    -289,   535,   157,   246,    -1,   221,   209,   191,  -289,   312,
     160,   535,  -289,  -289,  -289,   513,  -289,   535,  -289,   249,
    -289,   249,  -289,   249,  -289,  -289,  -289,  -289,  -289,  -289,
     249,  -289,  -289,  -289,   312,   312,  -289,   247,   223,   312,
     197,  -289,   312,  -289,   202,   259,   260,   263,   535,   219,
     312,   312,   224,   197,   535,  -289,   249,  -289,  -289,  -289,
    -289,   265,   535,   261,   111,  -289,   280,   312,  -289,  -289,
     250,  -289,   265,   249,  -289,   282,  -289,   249,  -289
  };

  const unsigned char
  parser::yydefact_[] =
  {
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     3,     0,    15,     4,
       5,     6,     7,     9,    10,     8,    12,    11,    13,     0,
       0,     0,     0,     0,   181,   170,     0,   174,   177,   169,
       0,     0,   121,   123,   124,   122,   120,     0,     0,    25,
       0,    22,     0,   161,     0,   165,   167,     0,   168,   176,
       1,     2,     0,     0,     0,    16,     0,     0,   126,     0,
      31,     0,   160,     0,   159,    49,     0,     0,     0,   102,
       0,     0,    94,     0,   184,   185,   100,   101,     0,     0,
       0,    90,     0,    26,    46,    56,    79,    86,    87,    89,
       0,    93,     0,   178,    98,    97,    99,     0,     0,     0,
     162,     0,     0,    18,    21,    20,     0,     0,     0,     0,
     119,   175,   177,     0,    53,     0,    51,     0,   110,   106,
       0,    80,    81,    90,     0,   182,    91,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,   180,
       0,     0,     0,     0,     0,     0,   163,   164,   166,    31,
     116,     0,   178,    19,   173,   172,   171,     0,   127,   129,
       0,    30,    52,    50,     0,   109,    92,    88,    27,    23,
      31,    28,    70,    69,     0,     0,     0,    61,    62,    63,
      65,    64,    66,    67,     0,     0,    77,    75,     0,    71,
      57,    58,    59,    60,    48,    82,    83,    84,    85,    96,
      95,    94,   107,   108,     0,     0,     0,   115,     0,   125,
       0,   138,   147,   152,   154,   134,   158,   131,   132,   133,
     135,   152,   152,   140,   158,   158,   158,   156,   148,   156,
     130,   113,     0,     0,     0,    33,    68,     0,    76,     0,
       0,    54,    72,   103,   105,     0,   117,   118,   128,     0,
     141,     0,   139,     0,   144,   136,   137,   142,   143,   145,
       0,   146,   149,   111,     0,     0,    29,     0,    35,     0,
      73,    78,     0,   104,     0,     0,     0,     0,   114,     0,
       0,     0,    36,    74,    55,   151,     0,   153,   157,   155,
     112,    32,    34,     0,    38,    37,     0,     0,    39,    40,
      41,   150,    45,     0,    17,    43,    44,     0,    42
  };

  const short
  parser::yypgoto_[] =
  {
    -289,  -289,  -289,    -3,     0,     2,  -289,  -289,  -289,  -154,
    -289,  -289,  -289,  -289,  -289,   -14,  -289,   181,  -133,  -288,
     -48,   118,   -47,  -289,  -289,  -289,  -289,  -289,  -289,   313,
    -289,    37,   315,  -289,   100,   317,  -289,  -289,  -289,  -289,
      97,  -289,  -289,    14,  -289,    80,    -6,  -289,  -289,  -289,
    -289,  -289,  -289,    18,    15,   177,  -107,  -289,   173,    47,
    -289,    91,   -78
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,    14,    15,    16,    91,    18,    50,    92,   190,   120,
     288,   302,   314,   320,   324,   325,   315,    93,    74,   260,
     261,   207,    95,    96,    97,    98,    99,    51,   225,    19,
     185,   252,    20,   169,   170,    21,    22,    46,    23,   177,
     178,   179,   250,   270,   272,   281,   274,    24,    25,   110,
      26,    27,    28,    75,    36,   100,   101,   102,   158,   103,
     104,   105,   106
  };

  const short
  parser::yytable_[] =
  {
      17,    17,    94,    29,   171,    42,   191,     1,    17,    52,
     138,   180,   311,    53,     1,   227,     1,   254,    31,     1,
      34,    40,    30,    35,    39,     1,    33,    54,    58,   322,
      57,    34,   112,   131,   132,   134,   255,    55,    48,    56,
     139,    73,   189,    49,    32,    67,    68,    41,    66,    10,
      69,    70,    38,    38,   119,    72,    34,   220,    10,    38,
      59,   208,   113,   114,   115,    76,   127,   119,   226,   155,
     156,   181,   209,   128,    34,    34,    17,    59,    59,    38,
      38,  -179,   123,   133,  -177,    29,    47,    59,    38,   136,
      94,    61,   192,   193,    38,   197,   198,   199,   200,   201,
     202,   203,    34,   204,    60,   210,   211,   212,   213,    65,
     125,     1,   173,   215,   216,   217,   218,   224,   122,   171,
     122,   286,   122,   180,   168,   166,   167,   259,    71,   175,
     186,  -176,   219,     4,   229,   230,   251,    34,    77,    73,
      34,   159,   160,   161,   162,   163,   256,     9,   257,   162,
     163,   206,   318,    10,    38,    38,    59,   111,   172,    12,
     319,    62,    63,    64,   122,   172,   149,   150,   151,   152,
     122,   107,   122,   153,   154,   155,   156,   265,     1,   135,
     267,   137,    37,    37,   283,   284,    38,   291,   292,    37,
     108,   294,   109,   295,   116,   296,   206,   117,   289,   194,
     195,   196,   297,   129,   159,   133,   118,    29,   119,    37,
      37,   172,   290,   160,   161,   162,   163,   130,    37,   160,
     161,   162,   163,    84,    37,   150,   151,   152,   316,   305,
     306,   153,   154,   155,   156,   164,   298,   251,   277,   278,
     279,   165,   303,   184,   304,   326,   310,   284,   121,   326,
     124,    78,   126,   312,    73,   275,   276,   174,   205,   228,
     262,   263,    79,   253,   269,    80,    81,    82,   271,    83,
     273,   280,   285,   172,    84,    34,    85,   172,    86,    87,
     287,    78,    85,   301,    37,    37,   307,   308,    88,   300,
     309,   313,    79,   292,   176,    80,    81,   221,    89,    83,
     182,    38,   183,   317,    84,    34,    85,   321,    86,    87,
     327,    10,   222,   328,   258,   323,    37,   231,    88,   188,
      78,    43,   299,    44,   223,    45,   266,   268,    89,   282,
     214,    79,     0,    90,    80,    81,    82,     0,    83,     0,
       0,     0,   232,    84,    34,    85,     0,    86,    87,   233,
       0,     0,     0,     0,   234,     0,     0,    88,     0,     0,
     235,     0,     0,    90,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,     0,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,    90,     0,     0,     0,     0,     0,     3,     0,
       0,     0,     4,     5,     6,     0,     0,     7,     0,     0,
       8,     0,     0,     0,     0,     0,     9,     0,     0,     0,
       0,    37,    10,     0,    11,     0,     0,     0,    12,    13,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,     0,     0,     0,   153,   154,   155,   156,
       0,     0,     0,     0,     0,     0,    34,     0,     0,     0,
       0,     0,     0,     0,   157,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,     0,     0,
       0,   153,   154,   155,   156,     0,   187,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
       0,     0,     0,   153,   154,   155,   156,     0,   264,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,     0,     0,     0,   153,   154,   155,   156,     0,
     293,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,     0,     0,     0,   153,   154,   155,
     156,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,     0,     0,     0,   153,   154,   155,   156,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,     0,     0,     0,   153,   154,   155,   156
  };

  const short
  parser::yycheck_[] =
  {
       0,     1,    50,     1,   111,     8,   139,    26,     8,    43,
      28,   118,   300,    47,    26,   169,    26,    28,    46,    26,
      32,    46,    72,     5,     6,    26,    58,    61,    13,   317,
      12,    32,    39,    80,    81,    83,   190,    71,    46,    73,
      58,    41,    54,    51,    72,    30,    31,    72,    24,    68,
      32,    33,     5,     6,    78,    40,    32,   164,    68,    12,
      13,     8,    62,    63,    64,    47,    76,    78,    92,    24,
      25,   119,    19,    76,    32,    32,    76,    30,    31,    32,
      33,    26,    40,    83,    29,    83,    64,    40,    41,    89,
     138,    30,   140,   141,    47,   143,   144,   145,   146,   147,
     148,   149,    32,   150,     0,   153,   154,   155,   156,    27,
      40,    26,   112,   160,   161,   162,   163,   165,    71,   226,
      73,   254,    75,   230,   109,   107,   108,     7,    29,    24,
     130,    29,    24,    48,    27,    28,   184,    32,    26,   139,
      32,    94,    22,    23,    24,    25,   194,    62,   195,    24,
      25,   151,    41,    68,   107,   108,   109,    69,   111,    74,
      49,     3,     4,     5,   117,   118,    15,    16,    17,    18,
     123,    58,   125,    22,    23,    24,    25,   225,    26,    88,
     228,    90,     5,     6,    27,    28,   139,    27,    28,    12,
      58,   269,    58,   271,    29,   273,   196,    29,     7,    15,
      16,    17,   280,    27,   157,   205,    26,   205,    78,    32,
      33,   164,   259,    22,    23,    24,    25,    55,    41,    22,
      23,    24,    25,    31,    47,    16,    17,    18,   306,    27,
      28,    22,    23,    24,    25,    29,   284,   285,   244,   245,
     246,    26,   289,    26,   292,   323,    27,    28,    71,   327,
      73,     8,    75,   301,   254,   241,   242,    24,    26,    11,
      19,    27,    19,    28,    26,    22,    23,    24,    26,    26,
      26,    26,    26,   226,    31,    32,    33,   230,    35,    36,
      59,     8,    33,    60,   107,   108,    27,    27,    45,    42,
      27,    67,    19,    28,   117,    22,    23,    24,    55,    26,
     123,   254,   125,    42,    31,    32,    33,    27,    35,    36,
      28,    68,    39,   327,   196,    65,   139,    20,    45,   138,
       8,     8,   285,     8,    51,     8,   226,   230,    55,   249,
     157,    19,    -1,    90,    22,    23,    24,    -1,    26,    -1,
      -1,    -1,    45,    31,    32,    33,    -1,    35,    36,    52,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    45,    -1,    -1,
      63,    -1,    -1,    90,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    90,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    48,    49,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,   254,    68,    -1,    70,    -1,    -1,    -1,    74,    75,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    22,    23,    24,    25,    -1,    27,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    22,    23,    24,    25,    -1,    27,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    22,    23,    24,    25,    -1,
      27,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    22,    23,    24,
      25,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    22,    23,    24,    25,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    23,    24,    25
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    26,    37,    44,    48,    49,    50,    53,    56,    62,
      68,    70,    74,    75,    94,    95,    96,    97,    98,   122,
     125,   128,   129,   131,   140,   141,   143,   144,   145,    98,
      72,    46,    72,    58,    32,   146,   147,   148,   152,   146,
      46,    72,    96,   122,   125,   128,   130,    64,    46,    51,
      99,   120,    43,    47,    61,    71,    73,   146,   147,   152,
       0,    30,     3,     4,     5,    27,    24,   147,   147,   146,
     146,    29,   147,    97,   111,   146,   146,    26,     8,    19,
      22,    23,    24,    26,    31,    33,    35,    36,    45,    55,
      90,    97,   100,   110,   113,   115,   116,   117,   118,   119,
     148,   149,   150,   152,   153,   154,   155,    58,    58,    58,
     142,    69,    39,    97,    97,    97,    29,    29,    26,    78,
     102,   148,   152,    40,   148,    40,   148,    76,    96,    27,
      55,   115,   115,    97,   113,   154,    97,   154,    28,    58,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    22,    23,    24,    25,    40,   151,   152,
      22,    23,    24,    25,    29,    26,   146,   146,   147,   126,
     127,   149,   152,    97,    24,    24,   148,   132,   133,   134,
     149,   113,   148,   148,    26,   123,    97,    27,   110,    54,
     101,   111,   113,   113,    15,    16,    17,   113,   113,   113,
     113,   113,   113,   113,   115,    26,    97,   114,     8,    19,
     113,   113,   113,   113,   151,   115,   115,   115,   115,    24,
     149,    24,    39,    51,   113,   121,    92,   102,    11,    27,
      28,    20,    45,    52,    57,    63,    77,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     135,   113,   124,    28,    28,   102,   113,   115,   114,     7,
     112,   113,    19,    27,    27,   113,   127,   113,   133,    26,
     136,    26,   137,    26,   139,   136,   136,   139,   139,   139,
      26,   138,   138,    27,    28,    26,   111,    59,   103,     7,
     115,    27,    28,    27,   155,   155,   155,   155,   113,   124,
      42,    60,   104,   115,   113,    27,    28,    27,    27,    27,
      27,   112,   113,    67,   105,   109,   155,    42,    41,    49,
     106,    27,   112,    65,   107,   108,   155,    28,   108
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    93,    94,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    96,    97,    98,    98,    98,
      98,    98,    98,    98,    99,    99,   100,   100,   101,   101,
     102,   102,   103,   103,   104,   104,   105,   105,   106,   106,
     106,   107,   107,   107,   108,   109,   110,   110,   110,   111,
     111,   111,   111,   111,   112,   112,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   114,   114,   115,
     115,   115,   115,   115,   115,   115,   116,   116,   116,   116,
     116,   116,   116,   117,   117,   117,   117,   118,   118,   118,
     118,   118,   118,   119,   119,   119,   120,   121,   121,   122,
     122,   123,   123,   124,   124,   125,   126,   126,   127,   128,
     129,   130,   130,   130,   130,   131,   131,   132,   132,   133,
     134,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     136,   136,   136,   137,   137,   138,   138,   139,   139,   140,
     140,   141,   141,   141,   141,   141,   142,   142,   143,   144,
     144,   145,   145,   145,   146,   146,   147,   148,   149,   150,
     151,   152,   153,   153,   154,   155
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     3,    11,     3,     4,
       3,     3,     2,     5,     0,     1,     1,     3,     1,     3,
       2,     0,     3,     0,     2,     0,     0,     1,     0,     1,
       1,     0,     4,     2,     1,     3,     1,     2,     3,     1,
       3,     2,     3,     2,     1,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     3,
       3,     3,     4,     5,     6,     3,     4,     1,     3,     1,
       2,     2,     3,     3,     3,     3,     1,     1,     3,     1,
       1,     2,     3,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     4,     5,     4,     3,     1,     1,     5,
       4,     3,     5,     1,     3,     5,     1,     3,     3,     4,
       2,     1,     1,     1,     1,     6,     3,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     2,     2,     1,     2,
       1,     2,     2,     2,     2,     2,     2,     1,     1,     2,
       5,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       3,     2,     3,     4,     4,     2,     2,     0,     2,     2,
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
  "ANY", "ALL", "AS", "ASC", "BY", "COLUMNS", "CREATE", "DATETIME",
  "DATABASE", "DATABASES", "DELETE", "DESC", "DESCRIBE", "DISTINCT",
  "DOUBLE", "DROP", "DUAL", "EXISTS", "EXPLAIN", "FLOAT", "FROM", "GROUP",
  "HAVING", "INDEX", "INSERT", "INT", "INTO", "LIMIT", "NUMBER", "ORDER",
  "SELECT", "SET", "SHOW", "STATUS", "TABLE", "TABLES", "UPDATE", "USING",
  "VALUES", "VARCHAR", "WHERE", "TINYINT", "SMALLINT", "MEDIUMINT",
  "BIGINT", "DECIMAL", "NUMERIC_SYM", "REAL", "CHAR", "BINARY",
  "VARBINARY", "TIMESTAMP_SYM", "DATE", "TIME", "','", "$accept",
  "sql_stmt", "stmt", "select_stmt", "select_with_parens",
  "select_no_parens", "opt_distinct", "select_expr_list", "from_list",
  "opt_where", "opt_groupby", "opt_having", "opt_order_by", "opt_asc_desc",
  "opt_select_limit", "limit_expr", "order_by", "projection",
  "table_factor", "expr_list", "expr", "in_expr", "arith_expr",
  "simple_expr", "column_ref", "expr_const", "func_expr",
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
       0,   286,   286,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   314,   321,   331,   350,   355,
     360,   365,   370,   374,   390,   391,   395,   400,   408,   413,
     421,   422,   426,   427,   431,   432,   436,   437,   441,   442,
     443,   447,   448,   455,   465,   472,   479,   487,   495,   506,
     510,   518,   526,   534,   549,   554,   563,   567,   572,   577,
     582,   587,   592,   597,   602,   607,   612,   617,   622,   627,
     632,   637,   642,   647,   652,   657,   662,   672,   676,   683,
     687,   692,   697,   702,   707,   712,   720,   724,   728,   732,
     736,   740,   744,   752,   759,   765,   773,   783,   792,   801,
     810,   819,   828,   840,   850,   859,   870,   877,   881,   892,
     902,   909,   913,   919,   924,   937,   950,   955,   963,   978,
     995,  1004,  1005,  1006,  1007,  1016,  1025,  1036,  1040,  1047,
    1054,  1063,  1065,  1067,  1069,  1071,  1073,  1075,  1077,  1079,
    1081,  1083,  1085,  1087,  1089,  1091,  1093,  1095,  1097,  1099,
    1104,  1105,  1106,  1110,  1111,  1115,  1116,  1120,  1121,  1130,
    1138,  1154,  1158,  1162,  1170,  1174,  1181,  1186,  1190,  1206,
    1214,  1229,  1238,  1247,  1264,  1271,  1281,  1285,  1289,  1293,
    1297,  1301,  1305,  1306,  1310,  1314
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
#line 3228 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"

#line 1316 "D:/CatDB/parser/lex_yacc/sql_parser.y"


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
