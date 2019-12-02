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
	Stmt_s stmt = ListStmt::make_list_stmt(); \
	check(stmt); \
	ListStmt* list = dynamic_cast<ListStmt*>(stmt.get()); \
	check(list); \
	list->stmt_list.push_back(from); \
	ret = stmt;

#define list_stmt_push(ret, list, from) \
	ListStmt* stmt = dynamic_cast<ListStmt*>(list.get()); \
	check(stmt); \
	stmt->stmt_list.push_back(from); \
	ret = list;

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
	stmt = UnaryExprStmt::make_unary_stmt(); \
	check(stmt); \
	UnaryExprStmt* unary = dynamic_cast<UnaryExprStmt*>(stmt.get()); \
	unary->expr_stmt = stmt1; \
	unary->op_type = op;

#define make_binary_stmt(stmt, stmt1, stmt2, op) \
	stmt = BinaryExprStmt::make_binary_stmt(); \
	check(stmt); \
	BinaryExprStmt* binary = dynamic_cast<BinaryExprStmt*>(stmt.get()); \
	binary->first_expr_stmt = stmt1; \
	binary->second_expr_stmt = stmt2; \
	binary->op_type = op;

#define make_ternary_stmt(stmt, stmt1, stmt2, stmt3, op) \
	stmt = TernaryExprStmt::make_ternary_stmt(); \
	check(stmt); \
	TernaryExprStmt* ternary = dynamic_cast<TernaryExprStmt*>(stmt.get()); \
	ternary->first_expr_stmt = stmt1; \
	ternary->second_expr_stmt = stmt2; \
	ternary->third_expr_stmt = stmt3; \
	ternary->op_type = op;
	
#define str_to_lower(str) \
	{\
		for(u32 i = 0; i<str.size(); ++i){\
			if(str[i] >= 'A' && str[i] <= 'Z'){\
				str[i] -= 'A';\
				str[i] += 'a';\
			}\
		}\
	}

#line 156 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"


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
#line 256 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"


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
      case 93: // sql_stmt
      case 94: // stmt
      case 95: // select_stmt
      case 96: // select_with_parens
      case 97: // select_no_parens
      case 99: // select_expr_list
      case 100: // from_list
      case 101: // opt_where
      case 102: // opt_groupby
      case 103: // opt_having
      case 104: // opt_order_by
      case 106: // opt_select_limit
      case 108: // order_by
      case 109: // projection
      case 110: // table_factor
      case 111: // expr_list
      case 112: // expr
      case 113: // in_expr
      case 114: // arith_expr
      case 115: // simple_expr
      case 116: // column_ref
      case 117: // expr_const
      case 118: // func_expr
      case 121: // insert_stmt
      case 122: // insert_vals_list
      case 123: // insert_vals
      case 124: // update_stmt
      case 125: // update_asgn_list
      case 126: // update_asgn_factor
      case 127: // delete_stmt
      case 128: // explain_stmt
      case 129: // explainable_stmt
      case 130: // create_stmt
      case 131: // table_element_list
      case 132: // table_element
      case 133: // column_definition
      case 139: // drop_stmt
      case 140: // show_stmt
      case 142: // use_stmt
      case 143: // desc_stmt
      case 144: // analyze_stmt
      case 145: // relation_factor
        value.YY_MOVE_OR_COPY< Stmt_s > (YY_MOVE (that.value));
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 154: // number
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 107: // limit_expr
      case 134: // data_type
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 119: // simple_function_expr
      case 141: // op_from_database
      case 146: // database_name
      case 147: // relation_name
      case 148: // column_name
      case 149: // function_name
      case 150: // column_label
      case 151: // ident
      case 152: // datetime
      case 153: // string
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
      case 93: // sql_stmt
      case 94: // stmt
      case 95: // select_stmt
      case 96: // select_with_parens
      case 97: // select_no_parens
      case 99: // select_expr_list
      case 100: // from_list
      case 101: // opt_where
      case 102: // opt_groupby
      case 103: // opt_having
      case 104: // opt_order_by
      case 106: // opt_select_limit
      case 108: // order_by
      case 109: // projection
      case 110: // table_factor
      case 111: // expr_list
      case 112: // expr
      case 113: // in_expr
      case 114: // arith_expr
      case 115: // simple_expr
      case 116: // column_ref
      case 117: // expr_const
      case 118: // func_expr
      case 121: // insert_stmt
      case 122: // insert_vals_list
      case 123: // insert_vals
      case 124: // update_stmt
      case 125: // update_asgn_list
      case 126: // update_asgn_factor
      case 127: // delete_stmt
      case 128: // explain_stmt
      case 129: // explainable_stmt
      case 130: // create_stmt
      case 131: // table_element_list
      case 132: // table_element
      case 133: // column_definition
      case 139: // drop_stmt
      case 140: // show_stmt
      case 142: // use_stmt
      case 143: // desc_stmt
      case 144: // analyze_stmt
      case 145: // relation_factor
        value.move< Stmt_s > (YY_MOVE (that.value));
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 154: // number
        value.move< double > (YY_MOVE (that.value));
        break;

      case 107: // limit_expr
      case 134: // data_type
        value.move< int > (YY_MOVE (that.value));
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 119: // simple_function_expr
      case 141: // op_from_database
      case 146: // database_name
      case 147: // relation_name
      case 148: // column_name
      case 149: // function_name
      case 150: // column_label
      case 151: // ident
      case 152: // datetime
      case 153: // string
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
      case 93: // sql_stmt
      case 94: // stmt
      case 95: // select_stmt
      case 96: // select_with_parens
      case 97: // select_no_parens
      case 99: // select_expr_list
      case 100: // from_list
      case 101: // opt_where
      case 102: // opt_groupby
      case 103: // opt_having
      case 104: // opt_order_by
      case 106: // opt_select_limit
      case 108: // order_by
      case 109: // projection
      case 110: // table_factor
      case 111: // expr_list
      case 112: // expr
      case 113: // in_expr
      case 114: // arith_expr
      case 115: // simple_expr
      case 116: // column_ref
      case 117: // expr_const
      case 118: // func_expr
      case 121: // insert_stmt
      case 122: // insert_vals_list
      case 123: // insert_vals
      case 124: // update_stmt
      case 125: // update_asgn_list
      case 126: // update_asgn_factor
      case 127: // delete_stmt
      case 128: // explain_stmt
      case 129: // explainable_stmt
      case 130: // create_stmt
      case 131: // table_element_list
      case 132: // table_element
      case 133: // column_definition
      case 139: // drop_stmt
      case 140: // show_stmt
      case 142: // use_stmt
      case 143: // desc_stmt
      case 144: // analyze_stmt
      case 145: // relation_factor
        value.move< Stmt_s > (that.value);
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        value.move< bool > (that.value);
        break;

      case 154: // number
        value.move< double > (that.value);
        break;

      case 107: // limit_expr
      case 134: // data_type
        value.move< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 119: // simple_function_expr
      case 141: // op_from_database
      case 146: // database_name
      case 147: // relation_name
      case 148: // column_name
      case 149: // function_name
      case 150: // column_label
      case 151: // ident
      case 152: // datetime
      case 153: // string
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
      case 93: // sql_stmt
      case 94: // stmt
      case 95: // select_stmt
      case 96: // select_with_parens
      case 97: // select_no_parens
      case 99: // select_expr_list
      case 100: // from_list
      case 101: // opt_where
      case 102: // opt_groupby
      case 103: // opt_having
      case 104: // opt_order_by
      case 106: // opt_select_limit
      case 108: // order_by
      case 109: // projection
      case 110: // table_factor
      case 111: // expr_list
      case 112: // expr
      case 113: // in_expr
      case 114: // arith_expr
      case 115: // simple_expr
      case 116: // column_ref
      case 117: // expr_const
      case 118: // func_expr
      case 121: // insert_stmt
      case 122: // insert_vals_list
      case 123: // insert_vals
      case 124: // update_stmt
      case 125: // update_asgn_list
      case 126: // update_asgn_factor
      case 127: // delete_stmt
      case 128: // explain_stmt
      case 129: // explainable_stmt
      case 130: // create_stmt
      case 131: // table_element_list
      case 132: // table_element
      case 133: // column_definition
      case 139: // drop_stmt
      case 140: // show_stmt
      case 142: // use_stmt
      case 143: // desc_stmt
      case 144: // analyze_stmt
      case 145: // relation_factor
        yylhs.value.emplace< Stmt_s > ();
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        yylhs.value.emplace< bool > ();
        break;

      case 154: // number
        yylhs.value.emplace< double > ();
        break;

      case 107: // limit_expr
      case 134: // data_type
        yylhs.value.emplace< int > ();
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 119: // simple_function_expr
      case 141: // op_from_database
      case 146: // database_name
      case 147: // relation_name
      case 148: // column_name
      case 149: // function_name
      case 150: // column_label
      case 151: // ident
      case 152: // datetime
      case 153: // string
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
#line 278 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		driver.result = yystack_[1].value.as < Stmt_s > ();
		YYACCEPT;
    }
#line 979 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 3:
#line 285 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 985 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 4:
#line 286 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 991 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 5:
#line 287 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 997 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 6:
#line 288 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1003 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 7:
#line 289 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1009 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 8:
#line 290 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1015 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 9:
#line 291 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1021 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 10:
#line 292 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1027 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 11:
#line 293 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1033 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 12:
#line 294 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1039 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 13:
#line 295 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1045 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 14:
#line 296 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yyerror("unknow stmt"); }
#line 1051 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 15:
#line 306 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1059 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 16:
#line 313 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = QueryStmt::make_query_stmt();
		QueryStmt* query = dynamic_cast<QueryStmt*>(stmt.get());
		query->query_stmt = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1070 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 17:
#line 326 "D:/CatDB/parser/lex_yacc/sql_parser.y"
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
#line 1090 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 18:
#line 342 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建union二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION);
    }
#line 1099 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 19:
#line 347 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建union all二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION_ALL);
    }
#line 1108 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 20:
#line 352 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建intersect二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_INTERSECT);
    }
#line 1117 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 21:
#line 357 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建except二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXCEPT);
    }
#line 1126 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 22:
#line 362 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(true);
	}
#line 1134 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 23:
#line 368 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1140 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 24:
#line 369 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1146 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 25:
#line 374 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建select list
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1155 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 26:
#line 379 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//把输出表达式加入到select list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1164 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 27:
#line 387 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建from list 
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1173 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 28:
#line 392 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//把表加入到from list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1182 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 29:
#line 399 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1188 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 30:
#line 400 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1194 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 31:
#line 404 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1200 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 32:
#line 405 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1206 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 33:
#line 409 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1212 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 34:
#line 410 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1218 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 35:
#line 414 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1224 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 36:
#line 415 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1230 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 37:
#line 419 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1236 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 38:
#line 420 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1242 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 39:
#line 421 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1248 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 40:
#line 425 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {yylhs.value.as < Stmt_s > () = NULL;}
#line 1254 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 41:
#line 427 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as < int > (), yystack_[2].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1265 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 42:
#line 434 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1276 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 43:
#line 444 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < int > () = yystack_[0].value.as < double > ();
	}
#line 1284 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 44:
#line 451 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1292 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 45:
#line 458 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
	//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[0].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = stmt->to_string();
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1304 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 46:
#line 466 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1316 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 47:
#line 474 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1328 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 48:
#line 485 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1336 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 49:
#line 489 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1348 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 50:
#line 497 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1360 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 51:
#line 505 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1372 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 52:
#line 513 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1384 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 53:
#line 528 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表达式列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1393 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 54:
#line 533 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1402 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 55:
#line 542 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1410 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 56:
#line 546 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建加法二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1419 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 57:
#line 551 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建减法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1428 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 58:
#line 556 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建乘法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1437 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 59:
#line 561 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建除法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1446 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 60:
#line 566 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LE);
	}
#line 1455 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 61:
#line 571 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LT);
	}
#line 1464 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 62:
#line 576 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
	}
#line 1473 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 63:
#line 581 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GE);
	}
#line 1482 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 64:
#line 586 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GT);
	}
#line 1491 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 65:
#line 591 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NE);
	}
#line 1500 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 66:
#line 596 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LIKE);
	}
#line 1509 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 67:
#line 601 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_LIKE);
	}
#line 1518 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 68:
#line 606 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_AND);
    }
#line 1527 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 69:
#line 611 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_OR);
    }
#line 1536 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 70:
#line 616 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), ExprStmt::OP_IS_NULL);
    }
#line 1545 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 71:
#line 621 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), ExprStmt::OP_IS_NOT_NULL);
    }
#line 1554 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 72:
#line 626 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_BETWEEN);
    }
#line 1563 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 73:
#line 631 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[5].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_BETWEEN);
    }
#line 1572 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 74:
#line 636 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_IN);
    }
#line 1581 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 75:
#line 641 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_IN);
    }
#line 1590 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 76:
#line 651 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1598 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 77:
#line 655 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1606 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 78:
#line 662 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1614 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 79:
#line 666 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//正数表达式
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1623 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 80:
#line 671 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MINUS);
    }
#line 1632 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 81:
#line 676 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1641 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 82:
#line 681 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1650 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 83:
#line 686 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1659 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 84:
#line 691 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1668 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 85:
#line 699 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1676 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 86:
#line 703 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		 yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1684 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 87:
#line 707 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1692 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 88:
#line 711 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
      	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1700 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 89:
#line 715 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1708 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 90:
#line 719 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
    	make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXISTS);
    }
#line 1716 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 91:
#line 723 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_EXISTS);
    }
#line 1725 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 92:
#line 731 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1736 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 93:
#line 738 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1746 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 94:
#line 744 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1757 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 95:
#line 752 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), "*");
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1768 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 96:
#line 762 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1781 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 97:
#line 771 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = DateTime::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1794 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 98:
#line 780 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as < double > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1807 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 99:
#line 789 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1820 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 100:
#line 798 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1833 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 101:
#line 807 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1846 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 102:
#line 819 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->aggr_expr = col;
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1860 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 103:
#line 829 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[4].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->distinct = yystack_[2].value.as < bool > ();
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1873 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 104:
#line 838 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1885 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 105:
#line 849 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = "database";
    }
#line 1893 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 106:
#line 856 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < bool > () = false;
    }
#line 1901 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 107:
#line 860 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < bool > () = true;
    }
#line 1909 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 108:
#line 871 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建insert stmt
		Stmt_s stmt = InsertStmt::make_insert_stmt();
		check(stmt);
		InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(stmt.get());
		insert_stmt->table = yystack_[2].value.as < Stmt_s > ();
		insert_stmt->values = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1923 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 109:
#line 881 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yyerror("insert into table from query not support yet!");
		YYABORT;
    }
#line 1932 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 110:
#line 888 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
    }
#line 1940 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 111:
#line 892 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
	}
#line 1948 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 112:
#line 898 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { 
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1957 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 113:
#line 903 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1966 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 114:
#line 916 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = UpdateStmt::make_update_stmt();
		check(stmt);
		UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(stmt.get());
		update_stmt->table = yystack_[3].value.as < Stmt_s > ();
		update_stmt->update_asgn_stmt = yystack_[1].value.as < Stmt_s > ();
		update_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1980 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 115:
#line 929 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1989 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 116:
#line 934 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1998 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 117:
#line 942 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as < std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as < Stmt_s > (), col, yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
    }
#line 2009 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 118:
#line 957 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DeleteStmt::make_delete_stmt();
		check(stmt);
		DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(stmt.get());
		delete_stmt->table = yystack_[1].value.as < Stmt_s > ();
		delete_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2022 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 119:
#line 974 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
		yyerror("explain not support yet!");
		YYABORT;
    }
#line 2032 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 120:
#line 982 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2038 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 121:
#line 983 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2044 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 122:
#line 984 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2050 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 123:
#line 985 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2056 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 124:
#line 995 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = CreateTableStmt::make_create_table_stmt();
		check(stmt);
		CreateTableStmt* create_table_stmt = dynamic_cast<CreateTableStmt*>(stmt.get());
		create_table_stmt->table = yystack_[3].value.as < Stmt_s > ();
		create_table_stmt->column_define_list = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2069 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 125:
#line 1004 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = CreateDatabaseStmt::make_create_database_stmt();
		check(stmt);
		CreateDatabaseStmt* create_database_stmt = dynamic_cast<CreateDatabaseStmt*>(stmt.get());
		create_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2081 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 126:
#line 1015 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2089 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 127:
#line 1019 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2097 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 128:
#line 1026 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 2105 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 129:
#line 1033 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as < std::string > (), yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2115 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 130:
#line 1042 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2121 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 131:
#line 1044 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2127 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 132:
#line 1046 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2133 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 133:
#line 1048 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2139 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 134:
#line 1050 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2145 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 135:
#line 1052 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2151 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 136:
#line 1054 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2157 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 137:
#line 1056 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2163 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 138:
#line 1058 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2169 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 139:
#line 1060 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2175 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 140:
#line 1062 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::NUMBER; }
#line 2181 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 141:
#line 1064 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2187 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 142:
#line 1066 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2193 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 143:
#line 1068 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2199 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 144:
#line 1070 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR; }
#line 2205 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 145:
#line 1072 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2211 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 146:
#line 1074 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2217 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 147:
#line 1076 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2223 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 148:
#line 1078 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < int > () = ColumnDefineStmt::DATETIME; }
#line 2229 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 149:
#line 1082 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2235 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 150:
#line 1083 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2241 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 151:
#line 1084 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2247 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 152:
#line 1088 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2253 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 153:
#line 1089 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2259 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 154:
#line 1093 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2265 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 155:
#line 1094 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2271 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 156:
#line 1098 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2277 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 157:
#line 1099 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { }
#line 2283 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 158:
#line 1109 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DropTableStmt::make_drop_table_stmt();
		check(stmt);
		DropTableStmt* drop_table_stmt = dynamic_cast<DropTableStmt*>(stmt.get());
		drop_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2295 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 159:
#line 1117 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DropDatabaseStmt::make_drop_database_stmt();
		check(stmt);
		DropDatabaseStmt* drop_database_stmt = dynamic_cast<DropDatabaseStmt*>(stmt.get());
		drop_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2307 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 160:
#line 1133 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(false);
    }
#line 2315 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 161:
#line 1137 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = ShowTablesStmt::make_show_tables_stmt(yystack_[0].value.as < std::string > ());
	}
#line 2323 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 162:
#line 1141 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2335 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 163:
#line 1149 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = NULL;
	}
#line 2343 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 164:
#line 1153 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = NULL;
	}
#line 2351 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 165:
#line 1160 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
	}
#line 2359 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 166:
#line 1164 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		yylhs.value.as < std::string > () = driver.get_global_database();
	}
#line 2367 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 167:
#line 1169 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = UseDatabaseStmt::make_use_database_stmt();
		check(stmt);
		UseDatabaseStmt* use_database_stmt = dynamic_cast<UseDatabaseStmt*>(stmt.get());
		use_database_stmt->database = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2379 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 168:
#line 1185 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2391 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 169:
#line 1193 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 2403 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 170:
#line 1208 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as < std::string > ();
		analyze_stmt->table = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2416 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 171:
#line 1217 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as < std::string > ();
		analyze_stmt->table = "*";
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2429 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 172:
#line 1226 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = "*";
		analyze_stmt->table = "*";
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2442 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 173:
#line 1243 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(driver.get_global_database(), yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2453 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 174:
#line 1250 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2464 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 175:
#line 1259 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2470 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 176:
#line 1263 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2476 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 177:
#line 1267 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2482 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 178:
#line 1271 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2488 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 179:
#line 1275 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > ()=yystack_[0].value.as < std::string > (); }
#line 2494 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 180:
#line 1279 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { str_to_lower(yystack_[0].value.as < std::string > ());yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2500 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 181:
#line 1283 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2506 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 182:
#line 1287 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2512 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;

  case 183:
#line 1291 "D:/CatDB/parser/lex_yacc/sql_parser.y"
    { yylhs.value.as < double > () = std::stod(yystack_[0].value.as < std::string > ()); }
#line 2518 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"
    break;


#line 2522 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"

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


  const short parser::yypact_ninf_ = -240;

  const short parser::yytable_ninf_ = -179;

  const short
  parser::yypact_[] =
  {
     298,   -15,   -51,   -23,   -28,    32,    32,   -20,    -1,    17,
      31,   -30,    32,    32,   105,    82,  -240,   214,  -240,  -240,
    -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,  -240,   106,
     -10,    32,    32,    32,  -240,  -240,   107,  -240,   110,  -240,
      32,    36,  -240,  -240,  -240,  -240,  -240,    32,   115,  -240,
     403,  -240,    86,  -240,    95,  -240,   101,    77,  -240,  -240,
    -240,  -240,    28,   135,   135,  -240,   142,   159,  -240,   168,
     138,    32,  -240,    23,  -240,    29,   -22,   172,   169,  -240,
    -240,   287,  -240,  -240,  -240,  -240,   190,   135,  -240,   -16,
    -240,   452,  -240,  -240,  -240,  -240,   209,  -240,   210,    -2,
    -240,  -240,  -240,    32,    32,    32,  -240,    32,   135,  -240,
    -240,  -240,   216,    71,    32,   403,  -240,  -240,  -240,    32,
    -240,    32,  -240,   227,  -240,  -240,   135,   214,   487,  -240,
    -240,   403,    36,   403,   403,   230,   403,   403,   403,   403,
     403,   403,   403,   354,   229,    13,   403,   403,   403,   403,
      32,  -240,  -240,    94,   374,  -240,  -240,  -240,   -18,  -240,
     222,  -240,  -240,  -240,  -240,  -240,   103,  -240,  -240,   204,
     553,  -240,  -240,   403,   231,  -240,  -240,  -240,   -21,  -240,
     572,   590,   403,   354,   229,   246,   246,   246,   246,   246,
     246,   167,   354,   354,   150,  -240,   287,  -240,  -240,   223,
    -240,   125,   125,  -240,  -240,  -240,  -240,  -240,   238,  -240,
    -240,   509,   403,    32,  -240,   403,  -240,    32,  -240,  -240,
     241,   252,  -240,   255,  -240,  -240,  -240,  -240,   241,   241,
    -240,   255,   255,   255,   272,  -240,   272,  -240,   553,   140,
     273,    36,   242,   167,   189,  -240,  -240,  -240,   354,   354,
     354,   354,   354,   181,   553,  -240,  -240,  -240,   531,  -240,
     553,  -240,   268,  -240,   268,  -240,   268,  -240,  -240,  -240,
    -240,  -240,  -240,   268,  -240,  -240,  -240,   403,   403,  -240,
     260,   244,   354,   207,   248,   248,  -240,  -240,  -240,   403,
    -240,   247,   277,   278,   280,   553,   249,   403,   403,   243,
     207,   553,  -240,   268,  -240,  -240,  -240,  -240,   282,   553,
     266,    88,  -240,   285,   403,  -240,  -240,   250,  -240,   282,
     268,  -240,   288,  -240,   268,  -240
  };

  const unsigned char
  parser::yydefact_[] =
  {
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,     3,     0,    15,     4,
       5,     6,     7,     9,    10,     8,    12,    11,    13,     0,
       0,     0,     0,     0,   180,   169,     0,   173,   176,   168,
       0,     0,   120,   122,   123,   121,   119,     0,     0,    24,
       0,    22,     0,   160,     0,   164,   166,     0,   167,   175,
       1,     2,     0,     0,     0,    16,     0,     0,   125,     0,
      30,     0,   159,     0,   158,    48,     0,     0,     0,   101,
      93,     0,   182,   183,    99,   100,     0,     0,    89,     0,
      25,    45,    55,    85,    86,    88,     0,    92,     0,   177,
      97,    96,    98,     0,     0,     0,   161,     0,     0,    18,
      21,    20,     0,     0,     0,     0,   118,   174,   176,     0,
      52,     0,    50,     0,   109,   105,     0,    89,     0,   181,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,   179,     0,     0,   162,   163,   165,    30,   115,
       0,   177,    19,   172,   171,   170,     0,   126,   128,     0,
      29,    51,    49,     0,   108,    91,    87,    26,    30,    27,
      69,    68,     0,     0,     0,    60,    61,    62,    64,    63,
      65,    66,     0,     0,     0,    78,     0,    76,    74,     0,
      70,    56,    57,    58,    59,    47,    95,    94,    93,   106,
     107,     0,     0,     0,   114,     0,   124,     0,   137,   146,
     151,   153,   133,   157,   130,   131,   132,   134,   151,   151,
     139,   157,   157,   157,   155,   147,   155,   129,   112,     0,
       0,     0,    32,    67,     0,    75,    79,    80,     0,     0,
       0,     0,     0,     0,    53,    71,   102,   104,     0,   116,
     117,   127,     0,   140,     0,   138,     0,   143,   135,   136,
     141,   142,   144,     0,   145,   148,   110,     0,     0,    28,
       0,    34,     0,    72,    81,    82,    83,    84,    77,     0,
     103,     0,     0,     0,     0,   113,     0,     0,     0,    35,
      73,    54,   150,     0,   152,   156,   154,   111,    31,    33,
       0,    37,    36,     0,     0,    38,    39,    40,   149,    44,
       0,    17,    42,    43,     0,    41
  };

  const short
  parser::yypgoto_[] =
  {
    -240,  -240,  -240,    -6,    38,     4,  -240,  -240,  -240,  -121,
    -240,  -240,  -240,  -240,  -240,    -9,  -240,   186,  -126,  -239,
     -50,   137,   -45,   -72,  -240,  -240,  -240,  -240,  -240,   317,
    -240,    48,   319,  -240,   116,   320,  -240,  -240,  -240,  -240,
     113,  -240,  -240,    68,  -240,    97,    19,  -240,  -240,  -240,
    -240,  -240,  -240,   154,   -12,   122,  -104,  -240,   184,     3,
    -240,   245,  -122
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,    14,    15,    16,    88,    18,    50,    89,   178,   116,
     281,   299,   311,   317,   321,   322,   312,    90,    74,   253,
     254,   198,   194,    92,    93,    94,    95,    51,   212,    19,
     174,   239,    20,   158,   159,    21,    22,    46,    23,   166,
     167,   168,   237,   263,   265,   274,   267,    24,    25,   106,
      26,    27,    28,    75,    36,    96,    97,    98,   151,    99,
     100,   101,   102
  };

  const short
  parser::yytable_[] =
  {
      91,    58,    42,   160,     1,    29,   179,   241,    38,    38,
     169,     1,   131,    52,    66,    38,    59,    53,    67,    68,
      30,   199,    34,    31,  -178,     1,    40,  -176,    72,    33,
      54,   128,   200,    59,    59,    38,    38,   214,    17,    17,
      55,   132,    56,    59,    38,    10,    17,     4,    32,   207,
      38,    41,    10,   123,     1,    34,   115,   242,   308,   115,
       9,    34,     1,   119,    34,   170,    10,   108,    34,   121,
     124,   195,    12,   213,   118,   319,   118,    48,   118,    73,
      47,    91,    49,   180,   181,    29,   185,   186,   187,   188,
     189,   190,   191,   157,   152,   164,   201,   202,   203,   204,
     109,   110,   111,    34,   211,    60,    38,    38,    59,   160,
     161,   195,    61,   169,    17,   279,   118,   161,   206,   127,
     195,   195,   118,   238,   118,   130,    34,    37,    37,   315,
     216,   217,   243,    65,    37,    38,    71,   316,   244,  -175,
     291,    77,   292,   103,   293,   107,   162,   246,   247,   148,
     149,   294,   104,   152,    37,    37,   161,   248,   105,    35,
      39,     1,   258,    37,   175,   260,    57,   276,   277,    37,
      73,   112,   249,   250,   251,   252,   195,   195,   195,   195,
     195,   313,   197,   143,   144,   145,    69,    70,   113,   146,
     147,   148,   149,   117,   114,   120,   282,   122,   323,   125,
      29,    76,   323,   283,   284,   285,   286,   287,   288,   289,
     195,   249,   250,   251,   252,   115,   161,    62,    63,    64,
     161,    82,   197,   126,   218,    37,    37,   295,   238,   249,
     250,   251,   252,   215,   127,   165,   154,   300,   153,   301,
     163,   171,   255,   172,    38,   182,   183,   184,   309,   219,
     270,   271,   272,   173,    37,   196,   220,   155,   156,   240,
     221,   142,   143,   144,   145,   256,   222,   262,   146,   147,
     148,   149,   251,   252,   302,   303,   307,   277,   264,    73,
     223,   266,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    78,   268,   269,   273,   278,
     280,    83,   297,   298,   304,   305,    79,   306,   314,   310,
     289,    80,   318,    81,   320,   325,   324,   177,    82,    34,
      83,   245,    84,    85,     1,    43,   296,    44,    45,   259,
     261,   129,    86,   275,   205,     2,     0,     0,     0,     0,
       0,    87,     3,     0,     0,     0,     4,     5,     6,     0,
       0,     7,     0,     8,    10,     0,     0,     0,     0,     9,
       0,     0,    78,    37,     0,    10,     0,    11,     0,     0,
       0,    12,    13,    79,     0,     0,   192,   193,    80,     0,
      81,     0,    78,     0,     0,    82,    34,    83,     0,    84,
      85,     0,     0,    79,     0,     0,     0,     0,   208,    86,
      81,     0,     0,     0,     0,    82,    34,    83,    87,    84,
      85,    78,     0,   209,     0,     0,     0,     0,     0,    86,
       0,     0,    79,     0,     0,   210,     0,    80,    87,    81,
       0,     0,     0,     0,    82,    34,    83,     0,    84,    85,
       0,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,    87,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,     0,   146,   147,   148,   149,     0,     0,
       0,     0,     0,     0,    34,     0,     0,     0,     0,     0,
       0,     0,   150,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,     0,   146,
     147,   148,   149,     0,   176,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,     0,     0,
       0,   146,   147,   148,   149,     0,   257,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
       0,     0,     0,   146,   147,   148,   149,     0,   290,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,   146,   147,   148,   149,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,     0,   146,   147,   148,   149,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,     0,
       0,     0,   146,   147,   148,   149
  };

  const short
  parser::yycheck_[] =
  {
      50,    13,     8,   107,    26,     1,   132,    28,     5,     6,
     114,    26,    28,    43,    24,    12,    13,    47,    30,    31,
      71,     8,    32,    46,    26,    26,    46,    29,    40,    57,
      60,    81,    19,    30,    31,    32,    33,   158,     0,     1,
      70,    57,    72,    40,    41,    67,     8,    48,    71,   153,
      47,    71,    67,    75,    26,    32,    77,   178,   297,    77,
      61,    32,    26,    40,    32,   115,    67,    39,    32,    40,
      76,   143,    73,    91,    71,   314,    73,    46,    75,    41,
      63,   131,    51,   133,   134,    81,   136,   137,   138,   139,
     140,   141,   142,   105,    91,    24,   146,   147,   148,   149,
      62,    63,    64,    32,   154,     0,   103,   104,   105,   213,
     107,   183,    30,   217,    76,   241,   113,   114,    24,    81,
     192,   193,   119,   173,   121,    87,    32,     5,     6,    41,
      27,    28,   182,    27,    12,   132,    29,    49,   183,    29,
     262,    26,   264,    57,   266,    68,   108,   192,   193,    24,
      25,   273,    57,   150,    32,    33,   153,     7,    57,     5,
       6,    26,   212,    41,   126,   215,    12,    27,    28,    47,
     132,    29,    22,    23,    24,    25,   248,   249,   250,   251,
     252,   303,   144,    16,    17,    18,    32,    33,    29,    22,
      23,    24,    25,    71,    26,    73,     7,    75,   320,    27,
     196,    47,   324,   248,   249,   250,   251,   252,    27,    28,
     282,    22,    23,    24,    25,    77,   213,     3,     4,     5,
     217,    31,   184,    54,    20,   103,   104,   277,   278,    22,
      23,    24,    25,    11,   196,   113,    26,   282,    29,   289,
      24,   119,    19,   121,   241,    15,    16,    17,   298,    45,
     231,   232,   233,    26,   132,    26,    52,   103,   104,    28,
      56,    15,    16,    17,    18,    27,    62,    26,    22,    23,
      24,    25,    24,    25,    27,    28,    27,    28,    26,   241,
      76,    26,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,     8,   228,   229,    26,    26,
      58,    33,    42,    59,    27,    27,    19,    27,    42,    66,
      28,    24,    27,    26,    64,   324,    28,   131,    31,    32,
      33,   184,    35,    36,    26,     8,   278,     8,     8,   213,
     217,    86,    45,   236,   150,    37,    -1,    -1,    -1,    -1,
      -1,    54,    44,    -1,    -1,    -1,    48,    49,    50,    -1,
      -1,    53,    -1,    55,    67,    -1,    -1,    -1,    -1,    61,
      -1,    -1,     8,   241,    -1,    67,    -1,    69,    -1,    -1,
      -1,    73,    74,    19,    -1,    -1,    22,    23,    24,    -1,
      26,    -1,     8,    -1,    -1,    31,    32,    33,    -1,    35,
      36,    -1,    -1,    19,    -1,    -1,    -1,    -1,    24,    45,
      26,    -1,    -1,    -1,    -1,    31,    32,    33,    54,    35,
      36,     8,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    19,    -1,    -1,    51,    -1,    24,    54,    26,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
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
       0,    26,    37,    44,    48,    49,    50,    53,    55,    61,
      67,    69,    73,    74,    93,    94,    95,    96,    97,   121,
     124,   127,   128,   130,   139,   140,   142,   143,   144,    97,
      71,    46,    71,    57,    32,   145,   146,   147,   151,   145,
      46,    71,    95,   121,   124,   127,   129,    63,    46,    51,
      98,   119,    43,    47,    60,    70,    72,   145,   146,   151,
       0,    30,     3,     4,     5,    27,    24,   146,   146,   145,
     145,    29,   146,    96,   110,   145,   145,    26,     8,    19,
      24,    26,    31,    33,    35,    36,    45,    54,    96,    99,
     109,   112,   115,   116,   117,   118,   147,   148,   149,   151,
     152,   153,   154,    57,    57,    57,   141,    68,    39,    96,
      96,    96,    29,    29,    26,    77,   101,   147,   151,    40,
     147,    40,   147,    75,    95,    27,    54,    96,   112,   153,
      96,    28,    57,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    22,    23,    24,    25,
      40,   150,   151,    29,    26,   145,   145,   146,   125,   126,
     148,   151,    96,    24,    24,   147,   131,   132,   133,   148,
     112,   147,   147,    26,   122,    96,    27,   109,   100,   110,
     112,   112,    15,    16,    17,   112,   112,   112,   112,   112,
     112,   112,    22,    23,   114,   115,    26,    96,   113,     8,
      19,   112,   112,   112,   112,   150,    24,   148,    24,    39,
      51,   112,   120,    91,   101,    11,    27,    28,    20,    45,
      52,    56,    62,    76,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,   134,   112,   123,
      28,    28,   101,   112,   114,   113,   114,   114,     7,    22,
      23,    24,    25,   111,   112,    19,    27,    27,   112,   126,
     112,   132,    26,   135,    26,   136,    26,   138,   135,   135,
     138,   138,   138,    26,   137,   137,    27,    28,    26,   110,
      58,   102,     7,   114,   114,   114,   114,   114,    27,    28,
      27,   154,   154,   154,   154,   112,   123,    42,    59,   103,
     114,   112,    27,    28,    27,    27,    27,    27,   111,   112,
      66,   104,   108,   154,    42,    41,    49,   105,    27,   111,
      64,   106,   107,   154,    28,   107
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    92,    93,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    95,    96,    97,    97,    97,
      97,    97,    97,    98,    98,    99,    99,   100,   100,   101,
     101,   102,   102,   103,   103,   104,   104,   105,   105,   105,
     106,   106,   106,   107,   108,   109,   109,   109,   110,   110,
     110,   110,   110,   111,   111,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   113,   113,   114,   114,
     114,   114,   114,   114,   114,   115,   115,   115,   115,   115,
     115,   115,   116,   116,   116,   116,   117,   117,   117,   117,
     117,   117,   118,   118,   118,   119,   120,   120,   121,   121,
     122,   122,   123,   123,   124,   125,   125,   126,   127,   128,
     129,   129,   129,   129,   130,   130,   131,   131,   132,   133,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   135,
     135,   135,   136,   136,   137,   137,   138,   138,   139,   139,
     140,   140,   140,   140,   140,   141,   141,   142,   143,   143,
     144,   144,   144,   145,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     3,    11,     3,     4,
       3,     3,     2,     0,     1,     1,     3,     1,     3,     2,
       0,     3,     0,     2,     0,     0,     1,     0,     1,     1,
       0,     4,     2,     1,     3,     1,     2,     3,     1,     3,
       2,     3,     2,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     3,     3,
       3,     4,     5,     6,     3,     4,     1,     3,     1,     2,
       2,     3,     3,     3,     3,     1,     1,     3,     1,     1,
       2,     3,     1,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     4,     5,     4,     3,     1,     1,     5,     4,
       3,     5,     1,     3,     5,     1,     3,     3,     4,     2,
       1,     1,     1,     1,     6,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     2,     2,     1,     2,     1,
       2,     2,     2,     2,     2,     2,     1,     1,     2,     5,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     3,
       2,     3,     4,     4,     2,     2,     0,     2,     2,     2,
       5,     5,     5,     1,     3,     1,     1,     1,     1,     1,
       1,     2,     1,     1
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
  "DOUBLE", "DROP", "EXISTS", "EXPLAIN", "FLOAT", "FROM", "GROUP",
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
       0,   277,   277,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   305,   312,   322,   341,   346,
     351,   356,   361,   368,   369,   373,   378,   386,   391,   399,
     400,   404,   405,   409,   410,   414,   415,   419,   420,   421,
     425,   426,   433,   443,   450,   457,   465,   473,   484,   488,
     496,   504,   512,   527,   532,   541,   545,   550,   555,   560,
     565,   570,   575,   580,   585,   590,   595,   600,   605,   610,
     615,   620,   625,   630,   635,   640,   650,   654,   661,   665,
     670,   675,   680,   685,   690,   698,   702,   706,   710,   714,
     718,   722,   730,   737,   743,   751,   761,   770,   779,   788,
     797,   806,   818,   828,   837,   848,   855,   859,   870,   880,
     887,   891,   897,   902,   915,   928,   933,   941,   956,   973,
     982,   983,   984,   985,   994,  1003,  1014,  1018,  1025,  1032,
    1041,  1043,  1045,  1047,  1049,  1051,  1053,  1055,  1057,  1059,
    1061,  1063,  1065,  1067,  1069,  1071,  1073,  1075,  1077,  1082,
    1083,  1084,  1088,  1089,  1093,  1094,  1098,  1099,  1108,  1116,
    1132,  1136,  1140,  1148,  1152,  1159,  1164,  1168,  1184,  1192,
    1207,  1216,  1225,  1242,  1249,  1259,  1263,  1267,  1271,  1275,
    1279,  1283,  1287,  1291
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
#line 3211 "D:/CatDB/parser/lex_yacc/sql_parser.cpp"

#line 1293 "D:/CatDB/parser/lex_yacc/sql_parser.y"


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
