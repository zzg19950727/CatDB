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
#line 25 "sql_parser.y"

	#include "sql_parser.hpp"
	#include "sql_scanner.h"
	#include "sql_driver.h"
	#include "location.hh"
	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
	#include "create_table_stmt.h"
	#include "drop_table_stmt.h"
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

#line 62 "sql_parser.cpp"


// First part of user prologue.
#line 49 "sql_parser.y"

	
#define yyerror(fmt, ...) \
{ \
	char tmp[255]; \
	sprintf(tmp, fmt, __VA_ARGS__); \
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

#line 145 "sql_parser.cpp"


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

#line 3 "sql_parser.y"
namespace CatDB {
#line 245 "sql_parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   SqlParser ::yytnamerr_ (const char *yystr)
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
   SqlParser :: SqlParser  (CatDB::SqlScanner& scanner_yyarg, CatDB::SqlDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

   SqlParser ::~ SqlParser  ()
  {}

   SqlParser ::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
   SqlParser ::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

   SqlParser ::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
   SqlParser ::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
   SqlParser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

   SqlParser ::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

   SqlParser ::symbol_number_type
   SqlParser ::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

   SqlParser ::stack_symbol_type::stack_symbol_type ()
  {}

   SqlParser ::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 74: // sql_stmt
      case 75: // stmt
      case 76: // select_stmt
      case 77: // select_with_parens
      case 78: // select_no_parens
      case 80: // select_expr_list
      case 81: // from_list
      case 82: // opt_where
      case 83: // opt_groupby
      case 84: // opt_having
      case 85: // opt_order_by
      case 87: // opt_select_limit
      case 89: // order_by
      case 90: // projection
      case 91: // table_factor
      case 92: // expr_list
      case 93: // expr
      case 94: // in_expr
      case 95: // arith_expr
      case 96: // simple_expr
      case 97: // column_ref
      case 98: // expr_const
      case 99: // func_expr
      case 101: // insert_stmt
      case 102: // insert_vals_list
      case 103: // insert_vals
      case 104: // update_stmt
      case 105: // update_asgn_list
      case 106: // update_asgn_factor
      case 107: // delete_stmt
      case 108: // explain_stmt
      case 109: // explainable_stmt
      case 110: // create_table_stmt
      case 111: // table_element_list
      case 112: // table_element
      case 113: // column_definition
      case 115: // drop_table_stmt
      case 116: // relation_factor
        value.YY_MOVE_OR_COPY< Stmt_s > (YY_MOVE (that.value));
        break;

      case 79: // opt_distinct
      case 86: // opt_asc_desc
      case 100: // distinct_or_all
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 125: // number
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 88: // limit_expr
      case 114: // data_type
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 33: // STRING
      case 34: // NUMERIC
      case 35: // TIMESTAMP
      case 117: // database_name
      case 118: // relation_name
      case 119: // column_name
      case 120: // function_name
      case 121: // column_label
      case 122: // ident
      case 123: // string
      case 124: // datetime
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

   SqlParser ::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 74: // sql_stmt
      case 75: // stmt
      case 76: // select_stmt
      case 77: // select_with_parens
      case 78: // select_no_parens
      case 80: // select_expr_list
      case 81: // from_list
      case 82: // opt_where
      case 83: // opt_groupby
      case 84: // opt_having
      case 85: // opt_order_by
      case 87: // opt_select_limit
      case 89: // order_by
      case 90: // projection
      case 91: // table_factor
      case 92: // expr_list
      case 93: // expr
      case 94: // in_expr
      case 95: // arith_expr
      case 96: // simple_expr
      case 97: // column_ref
      case 98: // expr_const
      case 99: // func_expr
      case 101: // insert_stmt
      case 102: // insert_vals_list
      case 103: // insert_vals
      case 104: // update_stmt
      case 105: // update_asgn_list
      case 106: // update_asgn_factor
      case 107: // delete_stmt
      case 108: // explain_stmt
      case 109: // explainable_stmt
      case 110: // create_table_stmt
      case 111: // table_element_list
      case 112: // table_element
      case 113: // column_definition
      case 115: // drop_table_stmt
      case 116: // relation_factor
        value.move< Stmt_s > (YY_MOVE (that.value));
        break;

      case 79: // opt_distinct
      case 86: // opt_asc_desc
      case 100: // distinct_or_all
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 125: // number
        value.move< double > (YY_MOVE (that.value));
        break;

      case 88: // limit_expr
      case 114: // data_type
        value.move< int > (YY_MOVE (that.value));
        break;

      case 33: // STRING
      case 34: // NUMERIC
      case 35: // TIMESTAMP
      case 117: // database_name
      case 118: // relation_name
      case 119: // column_name
      case 120: // function_name
      case 121: // column_label
      case 122: // ident
      case 123: // string
      case 124: // datetime
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
   SqlParser ::stack_symbol_type&
   SqlParser ::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 74: // sql_stmt
      case 75: // stmt
      case 76: // select_stmt
      case 77: // select_with_parens
      case 78: // select_no_parens
      case 80: // select_expr_list
      case 81: // from_list
      case 82: // opt_where
      case 83: // opt_groupby
      case 84: // opt_having
      case 85: // opt_order_by
      case 87: // opt_select_limit
      case 89: // order_by
      case 90: // projection
      case 91: // table_factor
      case 92: // expr_list
      case 93: // expr
      case 94: // in_expr
      case 95: // arith_expr
      case 96: // simple_expr
      case 97: // column_ref
      case 98: // expr_const
      case 99: // func_expr
      case 101: // insert_stmt
      case 102: // insert_vals_list
      case 103: // insert_vals
      case 104: // update_stmt
      case 105: // update_asgn_list
      case 106: // update_asgn_factor
      case 107: // delete_stmt
      case 108: // explain_stmt
      case 109: // explainable_stmt
      case 110: // create_table_stmt
      case 111: // table_element_list
      case 112: // table_element
      case 113: // column_definition
      case 115: // drop_table_stmt
      case 116: // relation_factor
        value.move< Stmt_s > (that.value);
        break;

      case 79: // opt_distinct
      case 86: // opt_asc_desc
      case 100: // distinct_or_all
        value.move< bool > (that.value);
        break;

      case 125: // number
        value.move< double > (that.value);
        break;

      case 88: // limit_expr
      case 114: // data_type
        value.move< int > (that.value);
        break;

      case 33: // STRING
      case 34: // NUMERIC
      case 35: // TIMESTAMP
      case 117: // database_name
      case 118: // relation_name
      case 119: // column_name
      case 120: // function_name
      case 121: // column_label
      case 122: // ident
      case 123: // string
      case 124: // datetime
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
   SqlParser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   SqlParser ::yy_print_ (std::ostream& yyo,
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
   SqlParser ::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
   SqlParser ::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
   SqlParser ::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   SqlParser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   SqlParser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   SqlParser ::debug_level_type
   SqlParser ::debug_level () const
  {
    return yydebug_;
  }

  void
   SqlParser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

   SqlParser ::state_type
   SqlParser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
   SqlParser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
   SqlParser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   SqlParser ::operator() ()
  {
    return parse ();
  }

  int
   SqlParser ::parse ()
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
      case 74: // sql_stmt
      case 75: // stmt
      case 76: // select_stmt
      case 77: // select_with_parens
      case 78: // select_no_parens
      case 80: // select_expr_list
      case 81: // from_list
      case 82: // opt_where
      case 83: // opt_groupby
      case 84: // opt_having
      case 85: // opt_order_by
      case 87: // opt_select_limit
      case 89: // order_by
      case 90: // projection
      case 91: // table_factor
      case 92: // expr_list
      case 93: // expr
      case 94: // in_expr
      case 95: // arith_expr
      case 96: // simple_expr
      case 97: // column_ref
      case 98: // expr_const
      case 99: // func_expr
      case 101: // insert_stmt
      case 102: // insert_vals_list
      case 103: // insert_vals
      case 104: // update_stmt
      case 105: // update_asgn_list
      case 106: // update_asgn_factor
      case 107: // delete_stmt
      case 108: // explain_stmt
      case 109: // explainable_stmt
      case 110: // create_table_stmt
      case 111: // table_element_list
      case 112: // table_element
      case 113: // column_definition
      case 115: // drop_table_stmt
      case 116: // relation_factor
        yylhs.value.emplace< Stmt_s > ();
        break;

      case 79: // opt_distinct
      case 86: // opt_asc_desc
      case 100: // distinct_or_all
        yylhs.value.emplace< bool > ();
        break;

      case 125: // number
        yylhs.value.emplace< double > ();
        break;

      case 88: // limit_expr
      case 114: // data_type
        yylhs.value.emplace< int > ();
        break;

      case 33: // STRING
      case 34: // NUMERIC
      case 35: // TIMESTAMP
      case 117: // database_name
      case 118: // relation_name
      case 119: // column_name
      case 120: // function_name
      case 121: // column_label
      case 122: // ident
      case 123: // string
      case 124: // datetime
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
#line 247 "sql_parser.y"
    {
		driver.result = yystack_[0].value.as < Stmt_s > ();
		YYACCEPT;
    }
#line 940 "sql_parser.cpp"
    break;

  case 3:
#line 254 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 946 "sql_parser.cpp"
    break;

  case 4:
#line 255 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 952 "sql_parser.cpp"
    break;

  case 5:
#line 256 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 958 "sql_parser.cpp"
    break;

  case 6:
#line 257 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 964 "sql_parser.cpp"
    break;

  case 7:
#line 258 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 970 "sql_parser.cpp"
    break;

  case 8:
#line 259 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 976 "sql_parser.cpp"
    break;

  case 9:
#line 260 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 982 "sql_parser.cpp"
    break;

  case 10:
#line 262 "sql_parser.y"
    { yyerror("unknow stmt"); }
#line 988 "sql_parser.cpp"
    break;

  case 11:
#line 272 "sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 996 "sql_parser.cpp"
    break;

  case 12:
#line 279 "sql_parser.y"
    {
		Stmt_s stmt = QueryStmt::make_query_stmt();
		QueryStmt* query = dynamic_cast<QueryStmt*>(stmt.get());
		query->query_stmt = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1007 "sql_parser.cpp"
    break;

  case 13:
#line 292 "sql_parser.y"
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
#line 1027 "sql_parser.cpp"
    break;

  case 14:
#line 308 "sql_parser.y"
    {
		//构建union二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION);
    }
#line 1036 "sql_parser.cpp"
    break;

  case 15:
#line 313 "sql_parser.y"
    {
		//构建union all二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_UNION_ALL);
    }
#line 1045 "sql_parser.cpp"
    break;

  case 16:
#line 318 "sql_parser.y"
    {
		//构建intersect二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_INTERSECT);
    }
#line 1054 "sql_parser.cpp"
    break;

  case 17:
#line 323 "sql_parser.y"
    {
		//构建except二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXCEPT);
    }
#line 1063 "sql_parser.cpp"
    break;

  case 18:
#line 330 "sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1069 "sql_parser.cpp"
    break;

  case 19:
#line 331 "sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1075 "sql_parser.cpp"
    break;

  case 20:
#line 336 "sql_parser.y"
    {
		//构建select list
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1084 "sql_parser.cpp"
    break;

  case 21:
#line 341 "sql_parser.y"
    {
		//把输出表达式加入到select list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1093 "sql_parser.cpp"
    break;

  case 22:
#line 349 "sql_parser.y"
    {
		//构建from list 
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1102 "sql_parser.cpp"
    break;

  case 23:
#line 354 "sql_parser.y"
    {
		//把表加入到from list
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1111 "sql_parser.cpp"
    break;

  case 24:
#line 361 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1117 "sql_parser.cpp"
    break;

  case 25:
#line 362 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1123 "sql_parser.cpp"
    break;

  case 26:
#line 366 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1129 "sql_parser.cpp"
    break;

  case 27:
#line 367 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1135 "sql_parser.cpp"
    break;

  case 28:
#line 371 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1141 "sql_parser.cpp"
    break;

  case 29:
#line 372 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1147 "sql_parser.cpp"
    break;

  case 30:
#line 376 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = NULL; }
#line 1153 "sql_parser.cpp"
    break;

  case 31:
#line 377 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 1159 "sql_parser.cpp"
    break;

  case 32:
#line 381 "sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1165 "sql_parser.cpp"
    break;

  case 33:
#line 382 "sql_parser.y"
    { yylhs.value.as < bool > () = true; }
#line 1171 "sql_parser.cpp"
    break;

  case 34:
#line 383 "sql_parser.y"
    { yylhs.value.as < bool > () = false; }
#line 1177 "sql_parser.cpp"
    break;

  case 35:
#line 387 "sql_parser.y"
    {yylhs.value.as < Stmt_s > () = NULL;}
#line 1183 "sql_parser.cpp"
    break;

  case 36:
#line 389 "sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[2].value.as < int > (), yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1194 "sql_parser.cpp"
    break;

  case 37:
#line 396 "sql_parser.y"
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1205 "sql_parser.cpp"
    break;

  case 38:
#line 406 "sql_parser.y"
    {
		yylhs.value.as < int > () = yystack_[0].value.as < double > ();
	}
#line 1213 "sql_parser.cpp"
    break;

  case 39:
#line 413 "sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1221 "sql_parser.cpp"
    break;

  case 40:
#line 420 "sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1229 "sql_parser.cpp"
    break;

  case 41:
#line 424 "sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1241 "sql_parser.cpp"
    break;

  case 42:
#line 432 "sql_parser.y"
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1253 "sql_parser.cpp"
    break;

  case 43:
#line 440 "sql_parser.y"
    {
		//构建column表达式
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1264 "sql_parser.cpp"
    break;

  case 44:
#line 450 "sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1272 "sql_parser.cpp"
    break;

  case 45:
#line 454 "sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1284 "sql_parser.cpp"
    break;

  case 46:
#line 462 "sql_parser.y"
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1296 "sql_parser.cpp"
    break;

  case 47:
#line 470 "sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[2].value.as < Stmt_s > ();
    }
#line 1308 "sql_parser.cpp"
    break;

  case 48:
#line 478 "sql_parser.y"
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as < Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as < std::string > ();
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
    }
#line 1320 "sql_parser.cpp"
    break;

  case 49:
#line 493 "sql_parser.y"
    {
		//构建表达式列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1329 "sql_parser.cpp"
    break;

  case 50:
#line 498 "sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1338 "sql_parser.cpp"
    break;

  case 51:
#line 506 "sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1346 "sql_parser.cpp"
    break;

  case 52:
#line 510 "sql_parser.y"
    {
		//正数表达式
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1355 "sql_parser.cpp"
    break;

  case 53:
#line 515 "sql_parser.y"
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MINUS);
    }
#line 1364 "sql_parser.cpp"
    break;

  case 54:
#line 520 "sql_parser.y"
    {
		//构建加法二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1373 "sql_parser.cpp"
    break;

  case 55:
#line 525 "sql_parser.y"
    {
		//构建减法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1382 "sql_parser.cpp"
    break;

  case 56:
#line 530 "sql_parser.y"
    {
		//构建乘法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1391 "sql_parser.cpp"
    break;

  case 57:
#line 535 "sql_parser.y"
    {
		//构建除法二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1400 "sql_parser.cpp"
    break;

  case 58:
#line 540 "sql_parser.y"
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LE);
	}
#line 1409 "sql_parser.cpp"
    break;

  case 59:
#line 545 "sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LT);
	}
#line 1418 "sql_parser.cpp"
    break;

  case 60:
#line 550 "sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
	}
#line 1427 "sql_parser.cpp"
    break;

  case 61:
#line 555 "sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GE);
	}
#line 1436 "sql_parser.cpp"
    break;

  case 62:
#line 560 "sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_GT);
	}
#line 1445 "sql_parser.cpp"
    break;

  case 63:
#line 565 "sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NE);
	}
#line 1454 "sql_parser.cpp"
    break;

  case 64:
#line 570 "sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_LIKE);
	}
#line 1463 "sql_parser.cpp"
    break;

  case 65:
#line 575 "sql_parser.y"
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_LIKE);
	}
#line 1472 "sql_parser.cpp"
    break;

  case 66:
#line 580 "sql_parser.y"
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_AND);
    }
#line 1481 "sql_parser.cpp"
    break;

  case 67:
#line 585 "sql_parser.y"
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_OR);
    }
#line 1490 "sql_parser.cpp"
    break;

  case 68:
#line 590 "sql_parser.y"
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT);
    }
#line 1499 "sql_parser.cpp"
    break;

  case 69:
#line 595 "sql_parser.y"
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), ExprStmt::OP_IS_NULL);
    }
#line 1508 "sql_parser.cpp"
    break;

  case 70:
#line 600 "sql_parser.y"
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), ExprStmt::OP_IS_NOT_NULL);
    }
#line 1517 "sql_parser.cpp"
    break;

  case 71:
#line 605 "sql_parser.y"
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_BETWEEN);
    }
#line 1526 "sql_parser.cpp"
    break;

  case 72:
#line 610 "sql_parser.y"
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as < Stmt_s > (), yystack_[5].value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_BETWEEN);
    }
#line 1535 "sql_parser.cpp"
    break;

  case 73:
#line 615 "sql_parser.y"
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_IN);
    }
#line 1544 "sql_parser.cpp"
    break;

  case 74:
#line 620 "sql_parser.y"
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[3].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_IN);
    }
#line 1553 "sql_parser.cpp"
    break;

  case 75:
#line 628 "sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1561 "sql_parser.cpp"
    break;

  case 76:
#line 632 "sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1569 "sql_parser.cpp"
    break;

  case 77:
#line 639 "sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1577 "sql_parser.cpp"
    break;

  case 78:
#line 643 "sql_parser.y"
    {
		//正数表达式
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1586 "sql_parser.cpp"
    break;

  case 79:
#line 648 "sql_parser.y"
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MINUS);
    }
#line 1595 "sql_parser.cpp"
    break;

  case 80:
#line 653 "sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1604 "sql_parser.cpp"
    break;

  case 81:
#line 658 "sql_parser.y"
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1613 "sql_parser.cpp"
    break;

  case 82:
#line 663 "sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1622 "sql_parser.cpp"
    break;

  case 83:
#line 668 "sql_parser.y"
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1631 "sql_parser.cpp"
    break;

  case 84:
#line 676 "sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1639 "sql_parser.cpp"
    break;

  case 85:
#line 680 "sql_parser.y"
    { 
		 yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
	}
#line 1647 "sql_parser.cpp"
    break;

  case 86:
#line 684 "sql_parser.y"
    { 
		yylhs.value.as < Stmt_s > () = yystack_[1].value.as < Stmt_s > ();
	}
#line 1655 "sql_parser.cpp"
    break;

  case 87:
#line 688 "sql_parser.y"
    {
      	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1663 "sql_parser.cpp"
    break;

  case 88:
#line 692 "sql_parser.y"
    {
    	yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 1671 "sql_parser.cpp"
    break;

  case 89:
#line 696 "sql_parser.y"
    {
    	make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EXISTS);
    }
#line 1679 "sql_parser.cpp"
    break;

  case 90:
#line 700 "sql_parser.y"
    {
		make_unary_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_NOT_EXISTS);
    }
#line 1687 "sql_parser.cpp"
    break;

  case 91:
#line 707 "sql_parser.y"
    { 
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1698 "sql_parser.cpp"
    break;

  case 92:
#line 714 "sql_parser.y"
    {
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as < Stmt_s > () = col;
	}
#line 1708 "sql_parser.cpp"
    break;

  case 93:
#line 720 "sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1719 "sql_parser.cpp"
    break;

  case 94:
#line 728 "sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as < std::string > (), "*");
		check(col);
		yylhs.value.as < Stmt_s > () = col;
    }
#line 1730 "sql_parser.cpp"
    break;

  case 95:
#line 738 "sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1743 "sql_parser.cpp"
    break;

  case 96:
#line 747 "sql_parser.y"
    {
		//构建常量表达式
		Object_s value = DateTime::make_object(yystack_[0].value.as < std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1756 "sql_parser.cpp"
    break;

  case 97:
#line 756 "sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as < double > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1769 "sql_parser.cpp"
    break;

  case 98:
#line 765 "sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1782 "sql_parser.cpp"
    break;

  case 99:
#line 774 "sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1795 "sql_parser.cpp"
    break;

  case 100:
#line 783 "sql_parser.y"
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
	}
#line 1808 "sql_parser.cpp"
    break;

  case 101:
#line 795 "sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->aggr_expr = col;
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1822 "sql_parser.cpp"
    break;

  case 102:
#line 805 "sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[4].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->distinct = yystack_[2].value.as < bool > ();
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1835 "sql_parser.cpp"
    break;

  case 103:
#line 814 "sql_parser.y"
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as < std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->aggr_expr = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1847 "sql_parser.cpp"
    break;

  case 104:
#line 822 "sql_parser.y"
    {
		//sys function
		yyerror("system function %s not support yet", yystack_[2].value.as < std::string > ().c_str());
		YYABORT;
    }
#line 1857 "sql_parser.cpp"
    break;

  case 105:
#line 831 "sql_parser.y"
    {
		yylhs.value.as < bool > () = false;
    }
#line 1865 "sql_parser.cpp"
    break;

  case 106:
#line 835 "sql_parser.y"
    {
		yylhs.value.as < bool > () = true;
    }
#line 1873 "sql_parser.cpp"
    break;

  case 107:
#line 846 "sql_parser.y"
    {
		//构建insert stmt
		Stmt_s stmt = InsertStmt::make_insert_stmt();
		check(stmt);
		InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(stmt.get());
		insert_stmt->table = yystack_[2].value.as < Stmt_s > ();
		insert_stmt->values = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1887 "sql_parser.cpp"
    break;

  case 108:
#line 856 "sql_parser.y"
    {
		yyerror("insert into table from query not support yet!");
		YYABORT;
    }
#line 1896 "sql_parser.cpp"
    break;

  case 109:
#line 863 "sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
    }
#line 1904 "sql_parser.cpp"
    break;

  case 110:
#line 867 "sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[4].value.as < Stmt_s > (), yystack_[1].value.as < Stmt_s > ());
	}
#line 1912 "sql_parser.cpp"
    break;

  case 111:
#line 873 "sql_parser.y"
    { 
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
	}
#line 1921 "sql_parser.cpp"
    break;

  case 112:
#line 878 "sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1930 "sql_parser.cpp"
    break;

  case 113:
#line 891 "sql_parser.y"
    {
		Stmt_s stmt = UpdateStmt::make_update_stmt();
		check(stmt);
		UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(stmt.get());
		update_stmt->table = yystack_[3].value.as < Stmt_s > ();
		update_stmt->update_asgn_stmt = yystack_[1].value.as < Stmt_s > ();
		update_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1944 "sql_parser.cpp"
    break;

  case 114:
#line 904 "sql_parser.y"
    {
		//构建值列表
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1953 "sql_parser.cpp"
    break;

  case 115:
#line 909 "sql_parser.y"
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 1962 "sql_parser.cpp"
    break;

  case 116:
#line 917 "sql_parser.y"
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as < std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as < Stmt_s > (), col, yystack_[0].value.as < Stmt_s > (), ExprStmt::OP_EQ);
    }
#line 1973 "sql_parser.cpp"
    break;

  case 117:
#line 932 "sql_parser.y"
    {
		Stmt_s stmt = DeleteStmt::make_delete_stmt();
		check(stmt);
		DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(stmt.get());
		delete_stmt->table = yystack_[1].value.as < Stmt_s > ();
		delete_stmt->where_stmt = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 1986 "sql_parser.cpp"
    break;

  case 118:
#line 949 "sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
		yyerror("explain not support yet!");
		YYABORT;
    }
#line 1996 "sql_parser.cpp"
    break;

  case 119:
#line 957 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2002 "sql_parser.cpp"
    break;

  case 120:
#line 958 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2008 "sql_parser.cpp"
    break;

  case 121:
#line 959 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2014 "sql_parser.cpp"
    break;

  case 122:
#line 960 "sql_parser.y"
    { yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > (); }
#line 2020 "sql_parser.cpp"
    break;

  case 123:
#line 970 "sql_parser.y"
    {
		Stmt_s stmt = CreateTableStmt::make_create_table_stmt();
		check(stmt);
		CreateTableStmt* create_table_stmt = dynamic_cast<CreateTableStmt*>(stmt.get());
		create_table_stmt->table = yystack_[3].value.as < Stmt_s > ();
		create_table_stmt->column_define_list = yystack_[1].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2033 "sql_parser.cpp"
    break;

  case 124:
#line 982 "sql_parser.y"
    {
		make_list_stmt(yylhs.value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2041 "sql_parser.cpp"
    break;

  case 125:
#line 986 "sql_parser.y"
    {
		list_stmt_push(yylhs.value.as < Stmt_s > (), yystack_[2].value.as < Stmt_s > (), yystack_[0].value.as < Stmt_s > ());
    }
#line 2049 "sql_parser.cpp"
    break;

  case 126:
#line 993 "sql_parser.y"
    {
		yylhs.value.as < Stmt_s > () = yystack_[0].value.as < Stmt_s > ();
    }
#line 2057 "sql_parser.cpp"
    break;

  case 127:
#line 1000 "sql_parser.y"
    {
		Stmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as < std::string > (), yystack_[0].value.as < int > ());
		check(stmt);
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2067 "sql_parser.cpp"
    break;

  case 128:
#line 1009 "sql_parser.y"
    {
		yylhs.value.as < int > () = ColumnDefineStmt::NUMBER;
    }
#line 2075 "sql_parser.cpp"
    break;

  case 129:
#line 1013 "sql_parser.y"
    { 
		yylhs.value.as < int > () = ColumnDefineStmt::DATETIME;
	}
#line 2083 "sql_parser.cpp"
    break;

  case 130:
#line 1017 "sql_parser.y"
    {
		yylhs.value.as < int > () = ColumnDefineStmt::VARCHAR;
    }
#line 2091 "sql_parser.cpp"
    break;

  case 131:
#line 1029 "sql_parser.y"
    {
		Stmt_s stmt = DropTableStmt::make_drop_table_stmt();
		check(stmt);
		DropTableStmt* drop_table_stmt = dynamic_cast<DropTableStmt*>(stmt.get());
		drop_table_stmt->table = yystack_[0].value.as < Stmt_s > ();
		yylhs.value.as < Stmt_s > () = stmt;
    }
#line 2103 "sql_parser.cpp"
    break;

  case 132:
#line 1045 "sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2114 "sql_parser.cpp"
    break;

  case 133:
#line 1052 "sql_parser.y"
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
		check(table);
		yylhs.value.as < Stmt_s > () = table;
	}
#line 2125 "sql_parser.cpp"
    break;

  case 134:
#line 1061 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2131 "sql_parser.cpp"
    break;

  case 135:
#line 1065 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2137 "sql_parser.cpp"
    break;

  case 136:
#line 1069 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2143 "sql_parser.cpp"
    break;

  case 137:
#line 1073 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2149 "sql_parser.cpp"
    break;

  case 138:
#line 1077 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2155 "sql_parser.cpp"
    break;

  case 139:
#line 1081 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2161 "sql_parser.cpp"
    break;

  case 140:
#line 1085 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2167 "sql_parser.cpp"
    break;

  case 141:
#line 1086 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2173 "sql_parser.cpp"
    break;

  case 142:
#line 1090 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2179 "sql_parser.cpp"
    break;

  case 143:
#line 1091 "sql_parser.y"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2185 "sql_parser.cpp"
    break;

  case 144:
#line 1095 "sql_parser.y"
    { yylhs.value.as < double > () = std::stod(yystack_[0].value.as < std::string > ()); }
#line 2191 "sql_parser.cpp"
    break;


#line 2195 "sql_parser.cpp"

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
   SqlParser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
   SqlParser ::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
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


  const short  SqlParser ::yypact_ninf_ = -174;

  const short  SqlParser ::yytable_ninf_ = -138;

  const short
   SqlParser ::yypact_[] =
  {
      86,   -22,   -54,   -33,    20,    82,   -15,    49,    70,   115,
    -174,  -174,    30,  -174,  -174,  -174,  -174,  -174,  -174,  -174,
      94,    70,    70,    60,  -174,  -174,  -174,  -174,  -174,    70,
    -174,   253,  -174,    87,   108,  -174,   126,  -174,   -21,   135,
     135,  -174,   140,   105,    58,  -174,    61,   -12,   283,  -174,
     304,   304,    -5,     5,   123,   134,  -174,  -174,  -174,   135,
    -174,    -2,  -174,   357,  -174,  -174,  -174,  -174,   150,  -174,
     158,    35,  -174,  -174,  -174,    70,    70,   135,  -174,  -174,
    -174,    70,   304,  -174,    70,  -174,  -174,    70,  -174,   163,
    -174,  -174,   135,   495,  -174,  -174,    30,   392,   161,   173,
     175,   194,  -174,   253,    60,   304,   304,   205,   304,   304,
     304,   304,   304,   304,   304,   325,   201,    54,   304,   304,
     304,   304,    70,  -174,  -174,    -8,   206,    74,  -174,   223,
    -174,  -174,  -174,   154,  -174,  -174,    56,   458,  -174,  -174,
     304,   208,  -174,  -174,  -174,  -174,  -174,  -174,  -174,   -27,
    -174,   477,   495,   304,   325,   201,   369,   369,   369,   369,
     369,   369,   242,   190,   325,   325,    46,  -174,     5,  -174,
    -174,   225,  -174,   162,   162,  -174,  -174,  -174,  -174,  -174,
     226,  -174,  -174,  -174,   414,   304,    70,  -174,   304,  -174,
      70,  -174,  -174,  -174,  -174,   458,   171,   229,    60,   195,
     242,   224,  -174,   135,  -174,  -174,   325,   325,   325,   325,
     325,   174,   458,  -174,  -174,  -174,   436,  -174,   458,  -174,
    -174,   304,   304,  -174,   209,   200,   325,  -174,   110,   187,
     187,  -174,  -174,  -174,   304,  -174,   458,   196,   304,   304,
     191,   110,   458,  -174,   234,   458,   221,    17,  -174,   304,
    -174,  -174,   210,   234,   237,  -174,   240,  -174,   237,  -174
  };

  const unsigned char
   SqlParser ::yydefact_[] =
  {
      10,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       2,     3,     0,    11,     4,     5,     6,     9,     7,     8,
       0,     0,     0,     0,   119,   121,   122,   120,   118,     0,
      19,     0,   139,     0,     0,   132,   135,     1,     0,     0,
       0,    12,     0,    25,     0,   131,    44,     0,     0,   100,
       0,     0,    92,     0,     0,     0,   144,    98,    99,     0,
      88,     0,    20,    40,    51,    84,    85,    87,     0,    91,
       0,   136,    95,    96,    97,     0,     0,     0,    14,    17,
      16,     0,     0,   117,     0,    48,   135,     0,    46,     0,
     108,    92,     0,    68,    52,    53,    88,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,   138,     0,     0,    25,   114,     0,
     136,   133,    15,     0,   124,   126,     0,    24,    47,    45,
       0,   107,    89,    86,   141,   143,   140,   142,    21,    25,
      22,    67,    66,     0,     0,     0,    58,    59,    60,    62,
      61,    63,    64,     0,     0,     0,     0,    77,     0,    75,
      73,     0,    69,    54,    55,    56,    57,    42,    94,    93,
      92,   104,   105,   106,     0,     0,     0,   113,     0,   123,
       0,   129,   128,   130,   127,   111,     0,     0,     0,    27,
      65,     0,    74,     0,    78,    79,     0,     0,     0,     0,
       0,     0,    49,    70,   101,   103,     0,   115,   116,   125,
     109,     0,     0,    23,     0,    29,     0,    90,    71,    80,
      81,    82,    83,    76,     0,   102,   112,     0,     0,     0,
      30,    72,    50,   110,    26,    28,     0,    32,    31,     0,
      33,    34,    35,    39,     0,    13,    37,    38,     0,    36
  };

  const short
   SqlParser ::yypgoto_[] =
  {
    -174,  -174,  -174,     1,     7,     2,  -174,  -174,  -174,  -117,
    -174,  -174,  -174,  -174,  -174,    15,  -174,   177,  -102,  -173,
     -31,   128,     9,   -13,  -174,  -174,  -174,  -174,   273,  -174,
      59,   287,  -174,   102,   288,  -174,  -174,  -174,  -174,   106,
    -174,  -174,  -174,   156,  -174,    96,   -66,  -174,   176,    84,
    -174,  -174,  -144
  };

  const short
   SqlParser ::yydefgoto_[] =
  {
      -1,     9,    10,    11,    60,    13,    31,    61,   149,    83,
     225,   240,   247,   252,   255,   256,   248,    62,    45,   211,
     212,   170,   166,    64,    65,    66,    67,   185,    14,   141,
     196,    15,   127,   128,    16,    17,    28,    18,   133,   134,
     135,   194,    19,    46,    34,    68,    69,    70,   123,    71,
      72,    73,    74
  };

  const short
   SqlParser ::yytable_[] =
  {
      63,   198,   150,    20,     1,     1,    24,    12,    12,   129,
     187,    21,    12,    48,     1,   136,   178,    93,    77,    94,
      95,    22,    97,   -43,    49,    32,   103,    50,    51,    91,
      44,    53,   199,    38,    39,    40,    54,    55,    32,    56,
       7,    57,    58,    29,    82,    78,    79,    80,    90,   -43,
       7,   137,   104,   206,    12,    20,    59,    89,   250,   179,
      96,  -137,   171,   251,  -135,   244,   102,     7,   207,   208,
     209,   210,    63,   172,   151,   152,   253,   156,   157,   158,
     159,   160,   161,   162,   132,    23,     1,   173,   174,   175,
     176,    32,    36,    32,    32,   184,   223,    30,    84,   142,
     191,    87,   167,    32,    35,    36,    36,    36,     1,   195,
     257,    44,     1,    36,   257,    37,   192,    35,    35,    35,
     129,    41,   200,   169,   136,    35,   193,     3,    86,     2,
      86,     3,   207,   208,   209,   210,     4,    76,     5,     6,
      85,   167,    88,     6,     7,    82,   186,   124,     7,     8,
      75,   167,   167,     8,   216,  -134,    98,   218,    99,   130,
      86,     1,   169,   201,    33,   130,    81,   100,    86,   101,
      20,    86,   131,   204,   205,    96,    82,    42,    43,   125,
     138,   189,   190,   139,   126,    47,   120,   121,    36,   140,
     236,   195,   144,   167,   167,   167,   167,   167,   220,   221,
      35,   233,   234,   242,   145,    44,   124,   146,   245,   130,
     227,   209,   210,   167,    48,   228,   229,   230,   231,   232,
     153,   154,   155,   243,   221,    49,   147,   168,    50,    51,
     180,   226,    53,   181,   188,   241,   197,    54,    55,    32,
      56,   203,    57,    58,   213,   182,   207,   208,   209,   210,
     224,   238,   246,   214,   183,   222,   239,    59,   115,   116,
     117,    48,   234,   249,   118,   119,   120,   121,   258,   254,
     130,    56,    49,   259,   130,    50,    51,    52,    25,    53,
     148,   237,    36,   202,    54,    55,    32,    56,   217,    57,
      58,    48,    26,    27,    35,     0,   219,     0,   177,     0,
       0,     0,    49,     0,    59,    50,    51,    91,     0,    53,
       0,     0,    48,     0,    54,    55,    32,    56,     0,    57,
      58,     0,     0,    49,     0,     0,    50,    51,    91,     0,
      53,     0,     0,   163,    92,    54,    55,    32,    56,     0,
      57,    58,     0,     0,    49,     0,     0,   164,   165,    91,
       0,    53,     0,     0,     0,    59,    54,    55,    32,    56,
       0,    57,    58,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    59,     0,     0,   118,
     119,   120,   121,     0,   114,   115,   116,   117,     0,     0,
      32,   118,   119,   120,   121,     0,     0,   122,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     0,     0,     0,   118,   119,   120,   121,     0,   143,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,     0,     0,   118,   119,   120,   121,
       0,   215,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,     0,     0,   118,   119,
     120,   121,     0,   235,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,     0,     0,     0,
     118,   119,   120,   121,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,     0,     0,   118,
     119,   120,   121,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,     0,     0,     0,   118,   119,   120,
     121
  };

  const short
   SqlParser ::yycheck_[] =
  {
      31,    28,   104,     1,    26,    26,     5,     0,     1,    75,
     127,    65,     5,     8,    26,    81,    24,    48,    39,    50,
      51,    54,    53,    28,    19,    33,    28,    22,    23,    24,
      23,    26,   149,     3,     4,     5,    31,    32,    33,    34,
      62,    36,    37,    58,    71,    38,    39,    40,    47,    54,
      62,    82,    54,     7,    47,    53,    51,    69,    41,   125,
      53,    26,     8,    46,    29,   238,    59,    62,    22,    23,
      24,    25,   103,    19,   105,   106,   249,   108,   109,   110,
     111,   112,   113,   114,    77,    65,    26,   118,   119,   120,
     121,    33,     8,    33,    33,   126,   198,    48,    40,    92,
      44,    40,   115,    33,     8,    21,    22,    23,    26,   140,
     254,   104,    26,    29,   258,     0,    60,    21,    22,    23,
     186,    27,   153,   116,   190,    29,    70,    45,    44,    43,
      46,    45,    22,    23,    24,    25,    50,    29,    52,    57,
      44,   154,    46,    57,    62,    71,    72,    63,    62,    67,
      63,   164,   165,    67,   185,    29,    33,   188,    35,    75,
      76,    26,   155,   154,     8,    81,    26,    33,    84,    35,
     168,    87,    76,   164,   165,   168,    71,    21,    22,    29,
      84,    27,    28,    87,    26,    29,    24,    25,   104,    26,
     221,   222,    31,   206,   207,   208,   209,   210,    27,    28,
     104,    27,    28,   234,    31,   198,   122,    32,   239,   125,
     203,    24,    25,   226,     8,   206,   207,   208,   209,   210,
      15,    16,    17,    27,    28,    19,    32,    26,    22,    23,
      24,     7,    26,    27,    11,   226,    28,    31,    32,    33,
      34,    51,    36,    37,    19,    39,    22,    23,    24,    25,
      55,    42,    61,    27,    48,    26,    56,    51,    16,    17,
      18,     8,    28,    42,    22,    23,    24,    25,    28,    59,
     186,    34,    19,   258,   190,    22,    23,    24,     5,    26,
     103,   222,   198,   155,    31,    32,    33,    34,   186,    36,
      37,     8,     5,     5,   198,    -1,   190,    -1,   122,    -1,
      -1,    -1,    19,    -1,    51,    22,    23,    24,    -1,    26,
      -1,    -1,     8,    -1,    31,    32,    33,    34,    -1,    36,
      37,    -1,    -1,    19,    -1,    -1,    22,    23,    24,    -1,
      26,    -1,    -1,     8,    51,    31,    32,    33,    34,    -1,
      36,    37,    -1,    -1,    19,    -1,    -1,    22,    23,    24,
      -1,    26,    -1,    -1,    -1,    51,    31,    32,    33,    34,
      -1,    36,    37,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    51,    -1,    -1,    22,
      23,    24,    25,    -1,    15,    16,    17,    18,    -1,    -1,
      33,    22,    23,    24,    25,    -1,    -1,    40,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    -1,    27,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    22,    23,    24,    25,
      -1,    27,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    22,    23,
      24,    25,    -1,    27,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      22,    23,    24,    25,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    22,
      23,    24,    25,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    22,    23,    24,
      25
  };

  const unsigned char
   SqlParser ::yystos_[] =
  {
       0,    26,    43,    45,    50,    52,    57,    62,    67,    74,
      75,    76,    77,    78,   101,   104,   107,   108,   110,   115,
      78,    65,    54,    65,    76,   101,   104,   107,   109,    58,
      48,    79,    33,   116,   117,   118,   122,     0,     3,     4,
       5,    27,   116,   116,    77,    91,   116,   116,     8,    19,
      22,    23,    24,    26,    31,    32,    34,    36,    37,    51,
      77,    80,    90,    93,    96,    97,    98,    99,   118,   119,
     120,   122,   123,   124,   125,    63,    29,    39,    77,    77,
      77,    26,    71,    82,    40,   118,   122,    40,   118,    69,
      76,    24,    51,    93,    93,    93,    77,    93,    33,    35,
      33,    35,    77,    28,    54,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    22,    23,
      24,    25,    40,   121,   122,    29,    26,   105,   106,   119,
     122,   118,    77,   111,   112,   113,   119,    93,   118,   118,
      26,   102,    77,    27,    31,    31,    32,    32,    90,    81,
      91,    93,    93,    15,    16,    17,    93,    93,    93,    93,
      93,    93,    93,     8,    22,    23,    95,    96,    26,    77,
      94,     8,    19,    93,    93,    93,    93,   121,    24,   119,
      24,    27,    39,    48,    93,   100,    72,    82,    11,    27,
      28,    44,    60,    70,   114,    93,   103,    28,    28,    82,
      93,    95,    94,    51,    95,    95,     7,    22,    23,    24,
      25,    92,    93,    19,    27,    27,    93,   106,    93,   112,
      27,    28,    26,    91,    55,    83,     7,    77,    95,    95,
      95,    95,    95,    27,    28,    27,    93,   103,    42,    56,
      84,    95,    93,    27,    92,    93,    61,    85,    89,    42,
      41,    46,    86,    92,    59,    87,    88,   125,    28,    88
  };

  const unsigned char
   SqlParser ::yyr1_[] =
  {
       0,    73,    74,    75,    75,    75,    75,    75,    75,    75,
      75,    76,    77,    78,    78,    78,    78,    78,    79,    79,
      80,    80,    81,    81,    82,    82,    83,    83,    84,    84,
      85,    85,    86,    86,    86,    87,    87,    87,    88,    89,
      90,    90,    90,    90,    91,    91,    91,    91,    91,    92,
      92,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    94,    94,    95,    95,    95,
      95,    95,    95,    95,    96,    96,    96,    96,    96,    96,
      96,    97,    97,    97,    97,    98,    98,    98,    98,    98,
      98,    99,    99,    99,    99,   100,   100,   101,   101,   102,
     102,   103,   103,   104,   105,   105,   106,   107,   108,   109,
     109,   109,   109,   110,   111,   111,   112,   113,   114,   114,
     114,   115,   116,   116,   117,   118,   119,   120,   121,   122,
     123,   123,   124,   124,   125
  };

  const unsigned char
   SqlParser ::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     3,    11,     3,     4,     3,     3,     0,     1,
       1,     3,     1,     3,     2,     0,     3,     0,     2,     0,
       0,     1,     0,     1,     1,     0,     4,     2,     1,     3,
       1,     2,     3,     1,     1,     3,     2,     3,     2,     1,
       3,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     3,     3,     2,     3,
       4,     5,     6,     3,     4,     1,     3,     1,     2,     2,
       3,     3,     3,     3,     1,     1,     3,     1,     1,     2,
       3,     1,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     4,     5,     4,     3,     1,     1,     5,     4,     3,
       5,     1,     3,     5,     1,     3,     3,     4,     2,     1,
       1,     1,     1,     6,     1,     3,     1,     2,     1,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  SqlParser ::yytname_[] =
  {
  "END", "error", "$undefined", "UNION", "EXCEPT", "INTERSECT", "OR",
  "AND", "NOT", "CMP_LE", "CMP_LT", "CMP_EQ", "CMP_GT", "CMP_GE", "CMP_NE",
  "LIKE", "BETWEEN", "IN", "IS", "NULLX", "BOOL", "UMINUS", "\"+\"",
  "\"-\"", "\"*\"", "\"/\"", "\"(\"", "\")\"", "\",\"", "\".\"", "\";\"",
  "\"'\"", "\"\\\"\"", "STRING", "NUMERIC", "TIMESTAMP", "FALSE", "TRUE",
  "ANY", "ALL", "AS", "ASC", "BY", "CREATE", "DATETIME", "DELETE", "DESC",
  "DESCRIBE", "DISTINCT", "DOUBLE", "DROP", "EXISTS", "EXPLAIN", "FLOAT",
  "FROM", "GROUP", "HAVING", "INSERT", "INTO", "LIMIT", "NUMBER", "ORDER",
  "SELECT", "SET", "SHOW", "TABLE", "TABLES", "UPDATE", "USING", "VALUES",
  "VARCHAR", "WHERE", "','", "$accept", "sql_stmt", "stmt", "select_stmt",
  "select_with_parens", "select_no_parens", "opt_distinct",
  "select_expr_list", "from_list", "opt_where", "opt_groupby",
  "opt_having", "opt_order_by", "opt_asc_desc", "opt_select_limit",
  "limit_expr", "order_by", "projection", "table_factor", "expr_list",
  "expr", "in_expr", "arith_expr", "simple_expr", "column_ref",
  "expr_const", "func_expr", "distinct_or_all", "insert_stmt",
  "insert_vals_list", "insert_vals", "update_stmt", "update_asgn_list",
  "update_asgn_factor", "delete_stmt", "explain_stmt", "explainable_stmt",
  "create_table_stmt", "table_element_list", "table_element",
  "column_definition", "data_type", "drop_table_stmt", "relation_factor",
  "database_name", "relation_name", "column_name", "function_name",
  "column_label", "ident", "string", "datetime", "number", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
   SqlParser ::yyrline_[] =
  {
       0,   246,   246,   254,   255,   256,   257,   258,   259,   260,
     262,   271,   278,   288,   307,   312,   317,   322,   330,   331,
     335,   340,   348,   353,   361,   362,   366,   367,   371,   372,
     376,   377,   381,   382,   383,   387,   388,   395,   405,   412,
     419,   423,   431,   439,   449,   453,   461,   469,   477,   492,
     497,   505,   509,   514,   519,   524,   529,   534,   539,   544,
     549,   554,   559,   564,   569,   574,   579,   584,   589,   594,
     599,   604,   609,   614,   619,   627,   631,   638,   642,   647,
     652,   657,   662,   667,   675,   679,   683,   687,   691,   695,
     699,   706,   713,   719,   727,   737,   746,   755,   764,   773,
     782,   794,   804,   813,   821,   830,   834,   845,   855,   862,
     866,   872,   877,   890,   903,   908,   916,   931,   948,   957,
     958,   959,   960,   969,   981,   985,   992,   999,  1008,  1012,
    1016,  1028,  1044,  1051,  1061,  1065,  1069,  1073,  1077,  1081,
    1085,  1086,  1090,  1091,  1095
  };

  // Print the state stack on the debug stream.
  void
   SqlParser ::yystack_print_ ()
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
   SqlParser ::yy_reduce_print_ (int yyrule)
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


#line 3 "sql_parser.y"
} // CatDB
#line 2825 "sql_parser.cpp"

#line 1097 "sql_parser.y"


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
