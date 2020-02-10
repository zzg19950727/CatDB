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
#line 26 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:397

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

#line 59 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:397


// First part of user declarations.
#line 51 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:404

	
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

#line 162 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sql_parser.hpp"

// User implementation prologue.

#line 176 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:412


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

#line 3 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:479
namespace CatDB {
#line 262 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:479

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
      case 97: // sql_stmt
      case 98: // stmt
      case 99: // select_stmt
      case 100: // select_with_parens
      case 101: // select_no_parens
      case 103: // select_expr_list
      case 104: // from_list
      case 105: // opt_where
      case 106: // opt_groupby
      case 107: // opt_having
      case 108: // opt_order_by
      case 110: // opt_select_limit
      case 112: // order_by
      case 113: // projection
      case 114: // table_factor
      case 115: // expr_list
      case 116: // expr
      case 117: // in_expr
      case 118: // arith_expr
      case 119: // simple_expr
      case 120: // column_ref
      case 121: // expr_const
      case 122: // func_expr
      case 125: // insert_stmt
      case 126: // insert_vals_list
      case 127: // insert_vals
      case 128: // update_stmt
      case 129: // update_asgn_list
      case 130: // update_asgn_factor
      case 131: // delete_stmt
      case 132: // explain_stmt
      case 133: // explainable_stmt
      case 134: // create_stmt
      case 135: // table_element_list
      case 136: // table_element
      case 137: // column_definition
      case 143: // drop_stmt
      case 144: // show_stmt
      case 146: // use_stmt
      case 147: // desc_stmt
      case 148: // analyze_stmt
      case 149: // relation_factor
        value.move< Stmt_s > (that.value);
        break;

      case 102: // opt_distinct
      case 109: // opt_asc_desc
      case 124: // distinct_or_all
        value.move< bool > (that.value);
        break;

      case 111: // limit_expr
      case 138: // data_type
        value.move< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 123: // simple_function_expr
      case 145: // op_from_database
      case 150: // database_name
      case 151: // relation_name
      case 152: // column_name
      case 153: // function_name
      case 154: // column_label
      case 155: // ident
      case 156: // datetime
      case 157: // string
      case 158: // number
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
      case 97: // sql_stmt
      case 98: // stmt
      case 99: // select_stmt
      case 100: // select_with_parens
      case 101: // select_no_parens
      case 103: // select_expr_list
      case 104: // from_list
      case 105: // opt_where
      case 106: // opt_groupby
      case 107: // opt_having
      case 108: // opt_order_by
      case 110: // opt_select_limit
      case 112: // order_by
      case 113: // projection
      case 114: // table_factor
      case 115: // expr_list
      case 116: // expr
      case 117: // in_expr
      case 118: // arith_expr
      case 119: // simple_expr
      case 120: // column_ref
      case 121: // expr_const
      case 122: // func_expr
      case 125: // insert_stmt
      case 126: // insert_vals_list
      case 127: // insert_vals
      case 128: // update_stmt
      case 129: // update_asgn_list
      case 130: // update_asgn_factor
      case 131: // delete_stmt
      case 132: // explain_stmt
      case 133: // explainable_stmt
      case 134: // create_stmt
      case 135: // table_element_list
      case 136: // table_element
      case 137: // column_definition
      case 143: // drop_stmt
      case 144: // show_stmt
      case 146: // use_stmt
      case 147: // desc_stmt
      case 148: // analyze_stmt
      case 149: // relation_factor
        value.copy< Stmt_s > (that.value);
        break;

      case 102: // opt_distinct
      case 109: // opt_asc_desc
      case 124: // distinct_or_all
        value.copy< bool > (that.value);
        break;

      case 111: // limit_expr
      case 138: // data_type
        value.copy< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 123: // simple_function_expr
      case 145: // op_from_database
      case 150: // database_name
      case 151: // relation_name
      case 152: // column_name
      case 153: // function_name
      case 154: // column_label
      case 155: // ident
      case 156: // datetime
      case 157: // string
      case 158: // number
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
      case 97: // sql_stmt
      case 98: // stmt
      case 99: // select_stmt
      case 100: // select_with_parens
      case 101: // select_no_parens
      case 103: // select_expr_list
      case 104: // from_list
      case 105: // opt_where
      case 106: // opt_groupby
      case 107: // opt_having
      case 108: // opt_order_by
      case 110: // opt_select_limit
      case 112: // order_by
      case 113: // projection
      case 114: // table_factor
      case 115: // expr_list
      case 116: // expr
      case 117: // in_expr
      case 118: // arith_expr
      case 119: // simple_expr
      case 120: // column_ref
      case 121: // expr_const
      case 122: // func_expr
      case 125: // insert_stmt
      case 126: // insert_vals_list
      case 127: // insert_vals
      case 128: // update_stmt
      case 129: // update_asgn_list
      case 130: // update_asgn_factor
      case 131: // delete_stmt
      case 132: // explain_stmt
      case 133: // explainable_stmt
      case 134: // create_stmt
      case 135: // table_element_list
      case 136: // table_element
      case 137: // column_definition
      case 143: // drop_stmt
      case 144: // show_stmt
      case 146: // use_stmt
      case 147: // desc_stmt
      case 148: // analyze_stmt
      case 149: // relation_factor
        yylhs.value.build< Stmt_s > ();
        break;

      case 102: // opt_distinct
      case 109: // opt_asc_desc
      case 124: // distinct_or_all
        yylhs.value.build< bool > ();
        break;

      case 111: // limit_expr
      case 138: // data_type
        yylhs.value.build< int > ();
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 123: // simple_function_expr
      case 145: // op_from_database
      case 150: // database_name
      case 151: // relation_name
      case 152: // column_name
      case 153: // function_name
      case 154: // column_label
      case 155: // ident
      case 156: // datetime
      case 157: // string
      case 158: // number
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
#line 290 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 863 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 297 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 869 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 298 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 875 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 299 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 881 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 300 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 887 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 301 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 893 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 302 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 899 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 303 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 905 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 304 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 911 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 305 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 917 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 306 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 923 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 307 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 929 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 308 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow stmt"); }
#line 935 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 318 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 943 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 325 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = QueryStmt::make_query_stmt();
		QueryStmt* query = dynamic_cast<QueryStmt*>(stmt.get());
		query->query_stmt = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 954 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 338 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建select stmt
		Stmt_s stmt = SelectStmt::make_select_stmt();
		SelectStmt* select_stmt = dynamic_cast<SelectStmt*>(stmt.get());
		select_stmt->is_distinct = yystack_[9].value.as< bool > ();
		select_stmt->select_expr_list = yystack_[8].value.as< Stmt_s > ();
		select_stmt->from_stmts = yystack_[6].value.as< Stmt_s > ();
		select_stmt->where_stmt = yystack_[5].value.as< Stmt_s > ();
		select_stmt->group_columns = yystack_[4].value.as< Stmt_s > ();
		select_stmt->having_stmt = yystack_[3].value.as< Stmt_s > ();
		select_stmt->order_columns = yystack_[2].value.as< Stmt_s > ();
		select_stmt->asc_desc = yystack_[1].value.as< bool > ();
		select_stmt->limit_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 974 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 354 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_UNION);
    }
#line 983 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 359 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_UNION_ALL);
    }
#line 992 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 364 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_INTERSECT);
    }
#line 1001 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 369 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EXCEPT);
    }
#line 1010 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 374 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(true);
	}
#line 1018 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 378 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建select stmt
		Stmt_s stmt = SelectStmt::make_select_stmt();
		SelectStmt* select_stmt = dynamic_cast<SelectStmt*>(stmt.get());
		select_stmt->is_distinct = false;
		select_stmt->select_expr_list = yystack_[2].value.as< Stmt_s > ();
		{
		make_list_stmt(select_stmt->from_stmts, TableStmt::make_table_stmt("system","sys_databases"));
		}
		select_stmt->limit_stmt = LimitStmt::make_limit_stmt(1);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1035 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 393 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1041 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 394 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1047 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 399 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建select list
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1056 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 404 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//把输出表达式加入到select list
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1065 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 412 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建from list 
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
	}
#line 1074 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 417 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//把表加入到from list
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
	}
#line 1083 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 424 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1089 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 425 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1095 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 429 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1101 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 430 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1107 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 434 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1113 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 435 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1119 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 439 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1125 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 440 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1131 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 444 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1137 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 445 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1143 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 446 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1149 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 450 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< Stmt_s > () = NULL;}
#line 1155 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 452 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1166 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 459 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1177 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 469 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 1185 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 476 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1193 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 483 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
	//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[0].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = stmt->to_string();
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1205 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 491 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
    }
#line 1217 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 499 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[2].value.as< Stmt_s > ();
    }
#line 1229 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 510 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1237 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 514 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[2].value.as< Stmt_s > ();
    }
#line 1249 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 522 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
    }
#line 1261 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 530 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[2].value.as< Stmt_s > ();
    }
#line 1273 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 538 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
    }
#line 1285 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 553 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1294 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 558 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1303 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 567 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1311 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 571 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建加法二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1320 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 576 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建减法二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1329 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 581 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建乘法二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1338 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 586 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建除法二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1347 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 591 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_LE);
	}
#line 1356 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 596 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_LT);
	}
#line 1365 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 601 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EQ);
	}
#line 1374 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 606 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_GE);
	}
#line 1383 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 611 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_GT);
	}
#line 1392 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 616 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NE);
	}
#line 1401 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 621 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_LIKE);
	}
#line 1410 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 626 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_LIKE);
	}
#line 1419 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 631 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_AND);
    }
#line 1428 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 636 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_OR);
    }
#line 1437 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 641 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), ExprStmt::OP_IS_NULL);
    }
#line 1446 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 646 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), ExprStmt::OP_IS_NOT_NULL);
    }
#line 1455 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 651 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< Stmt_s > (), yystack_[4].value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_BETWEEN);
    }
#line 1464 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 656 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< Stmt_s > (), yystack_[5].value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_BETWEEN);
    }
#line 1473 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 661 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_IN);
    }
#line 1482 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 666 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_IN);
    }
#line 1491 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 676 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1499 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 680 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1507 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 687 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1515 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 691 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1524 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 696 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_MINUS);
    }
#line 1533 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 701 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1542 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 706 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1551 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 711 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1560 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 716 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1569 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 724 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1577 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 728 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1585 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 732 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1593 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 736 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1601 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 740 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1609 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 744 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EXISTS);
    }
#line 1617 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 748 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_EXISTS);
    }
#line 1626 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 756 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< Stmt_s > () = col;
	}
#line 1637 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 763 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< Stmt_s > () = col;
	}
#line 1647 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 769 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< Stmt_s > () = col;
    }
#line 1658 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 777 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< Stmt_s > () = col;
    }
#line 1669 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 787 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1682 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 796 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = DateTime::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1695 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 805 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1708 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 814 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1721 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 823 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1734 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 832 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1747 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 844 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as< std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->aggr_expr = col;
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1761 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 854 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[4].value.as< std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->distinct = yystack_[2].value.as< bool > ();
		aggr->aggr_expr = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1774 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 863 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as< std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->aggr_expr = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1786 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 874 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "database";
    }
#line 1794 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 881 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 1802 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 885 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 1810 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 896 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		Stmt_s stmt = InsertStmt::make_insert_stmt();
		check(stmt);
		InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(stmt.get());
		insert_stmt->table = yystack_[2].value.as< Stmt_s > ();
		insert_stmt->values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1824 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 906 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yyerror("insert into table from query not support yet!");
		YYABORT;
    }
#line 1833 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 913 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[1].value.as< Stmt_s > ());
    }
#line 1841 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 917 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[4].value.as< Stmt_s > (), yystack_[1].value.as< Stmt_s > ());
	}
#line 1849 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 923 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		//构建值列表
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
	}
#line 1858 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 928 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1867 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 941 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = UpdateStmt::make_update_stmt();
		check(stmt);
		UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(stmt.get());
		update_stmt->table = yystack_[3].value.as< Stmt_s > ();
		update_stmt->update_asgn_stmt = yystack_[1].value.as< Stmt_s > ();
		update_stmt->where_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1881 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 954 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1890 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 959 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1899 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 967 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< Stmt_s > (), col, yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EQ);
    }
#line 1910 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 982 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DeleteStmt::make_delete_stmt();
		check(stmt);
		DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(stmt.get());
		delete_stmt->table = yystack_[1].value.as< Stmt_s > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1923 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 999 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > ()->is_explain = true;
    }
#line 1932 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 1006 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1938 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 1007 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1944 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 1008 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1950 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 1009 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1956 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 1019 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CreateTableStmt::make_create_table_stmt();
		check(stmt);
		CreateTableStmt* create_table_stmt = dynamic_cast<CreateTableStmt*>(stmt.get());
		create_table_stmt->table = yystack_[3].value.as< Stmt_s > ();
		create_table_stmt->column_define_list = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1969 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 1028 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CreateDatabaseStmt::make_create_database_stmt();
		check(stmt);
		CreateDatabaseStmt* create_database_stmt = dynamic_cast<CreateDatabaseStmt*>(stmt.get());
		create_database_stmt->database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1981 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1039 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1989 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1043 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1997 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1050 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 2005 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1057 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as< std::string > (), yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2015 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1066 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2021 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1068 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2027 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1070 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2033 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1072 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2039 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1074 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2045 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1076 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2051 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1078 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2057 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1080 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2063 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1082 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2069 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1084 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2075 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1086 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2081 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1088 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2087 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1090 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2093 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1092 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2099 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1094 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2105 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1096 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2111 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1098 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2117 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1100 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2123 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1102 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2129 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1106 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2135 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1107 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2141 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1108 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2147 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1112 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2153 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1113 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2159 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1117 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2165 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1118 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2171 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1122 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2177 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1123 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2183 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1133 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DropTableStmt::make_drop_table_stmt();
		check(stmt);
		DropTableStmt* drop_table_stmt = dynamic_cast<DropTableStmt*>(stmt.get());
		drop_table_stmt->table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2195 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1141 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DropDatabaseStmt::make_drop_database_stmt();
		check(stmt);
		DropDatabaseStmt* drop_database_stmt = dynamic_cast<DropDatabaseStmt*>(stmt.get());
		drop_database_stmt->database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2207 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1157 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = ShowDatabasesStmt::make_show_databases_stmt(false);
    }
#line 2215 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1161 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = ShowTablesStmt::make_show_tables_stmt(yystack_[0].value.as< std::string > ());
	}
#line 2223 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1165 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2235 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1173 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 2243 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1177 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 2251 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1181 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as< Stmt_s > ();
		desc_table_stmt->is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2264 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1190 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as< Stmt_s > ();
		desc_table_stmt->is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2277 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1202 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2285 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1206 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 2293 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1211 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = UseDatabaseStmt::make_use_database_stmt();
		check(stmt);
		UseDatabaseStmt* use_database_stmt = dynamic_cast<UseDatabaseStmt*>(stmt.get());
		use_database_stmt->database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2305 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1227 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2317 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1235 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DescTableStmt::make_desc_table_stmt();
		check(stmt);
		DescTableStmt* desc_table_stmt = dynamic_cast<DescTableStmt*>(stmt.get());
		desc_table_stmt->table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2329 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1250 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as< std::string > ();
		analyze_stmt->table = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2342 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1259 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = yystack_[2].value.as< std::string > ();
		analyze_stmt->table = "*";
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2355 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1268 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = AnalyzeStmt::make_analyze_stmt();
		check(stmt);
		AnalyzeStmt* analyze_stmt = dynamic_cast<AnalyzeStmt*>(stmt.get());
		analyze_stmt->database = "*";
		analyze_stmt->table = "*";
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2368 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1285 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		yylhs.value.as< Stmt_s > () = table;
	}
#line 2379 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1292 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		yylhs.value.as< Stmt_s > () = table;
	}
#line 2390 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1301 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2396 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1305 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2402 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1309 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2408 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1313 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2414 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1317 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 2420 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1321 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2426 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1325 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2432 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1326 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2438 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1330 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2444 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1334 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2450 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;


#line 2454 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -284;

  const short int parser::yytable_ninf_ = -182;

  const short int
  parser::yypact_[] =
  {
     396,   -19,   -44,   -31,    -9,    30,    30,   -26,   107,    14,
      61,   125,    30,    30,    83,    56,  -284,   171,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,    88,
      29,    30,    30,    30,  -284,  -284,    64,  -284,    99,  -284,
      30,    -4,  -284,  -284,  -284,  -284,  -284,    30,   106,  -284,
     336,  -284,    62,    94,  -284,    98,  -284,    91,   126,   121,
    -284,  -284,  -284,  -284,    51,   168,   168,  -284,   169,   181,
    -284,   186,   134,    30,  -284,    -6,  -284,    48,   -22,   189,
     161,  -284,   336,   336,  -284,   300,  -284,  -284,  -284,  -284,
     188,   168,   188,  -284,   -14,  -284,   469,   138,  -284,  -284,
    -284,  -284,   192,  -284,   194,    66,  -284,  -284,  -284,    30,
      30,    30,    30,    30,  -284,    30,   168,  -284,  -284,  -284,
     198,    57,    30,   336,  -284,  -284,  -284,    30,  -284,    30,
    -284,   199,  -284,  -284,   168,  -284,  -284,   171,   504,  -284,
    -284,  -284,   336,    -8,   336,   336,   163,   336,   336,   336,
     336,   336,   336,   336,   336,   200,    11,   336,   336,   336,
     336,    30,  -284,  -284,   336,   336,   336,   336,    82,   259,
    -284,  -284,  -284,  -284,  -284,     3,  -284,   223,  -284,  -284,
    -284,  -284,  -284,    28,  -284,  -284,   327,   570,  -284,  -284,
     336,   207,  -284,  -284,  -284,  -284,   -13,  -284,   589,   607,
     336,   336,   200,   372,   372,   372,   372,   372,   372,   414,
      47,   300,  -284,  -284,   217,  -284,   116,   116,  -284,  -284,
    -284,   142,   142,  -284,  -284,  -284,  -284,   210,  -284,  -284,
     526,   336,    30,  -284,   336,  -284,    30,  -284,  -284,   215,
     218,  -284,   220,  -284,  -284,  -284,  -284,   215,   215,  -284,
     220,   220,   220,   225,  -284,   225,  -284,   570,   144,   226,
      -4,   193,   414,   122,  -284,   336,   154,   570,  -284,  -284,
    -284,   548,  -284,   570,  -284,   216,  -284,   216,  -284,   216,
    -284,  -284,  -284,  -284,  -284,  -284,   216,  -284,  -284,  -284,
     336,   336,  -284,   212,   195,   336,   138,  -284,   336,  -284,
     177,   228,   230,   231,   570,   179,   336,   336,   197,   138,
     570,  -284,   216,  -284,  -284,  -284,  -284,   234,   570,   221,
      32,  -284,   242,   336,  -284,  -284,   204,  -284,   234,   216,
    -284,   243,  -284,   216,  -284
  };

  const unsigned char
  parser::yydefact_[] =
  {
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     3,     0,    15,     4,
       5,     6,     7,     9,    10,     8,    12,    11,    13,     0,
       0,     0,     0,     0,   183,   172,     0,   176,   179,   171,
       0,     0,   121,   123,   124,   122,   120,     0,     0,    25,
       0,    22,     0,     0,   161,     0,   165,     0,   169,     0,
     170,   178,     1,     2,     0,     0,     0,    16,     0,     0,
     126,     0,    31,     0,   160,     0,   159,    49,     0,     0,
       0,   102,     0,     0,    94,     0,   186,   187,   100,   101,
       0,     0,     0,    90,     0,    26,    46,    56,    79,    86,
      87,    89,     0,    93,     0,   180,    98,    97,    99,     0,
       0,     0,     0,     0,   162,     0,     0,    18,    21,    20,
       0,     0,     0,     0,   119,   177,   179,     0,    53,     0,
      51,     0,   110,   106,     0,    80,    81,    90,     0,   184,
      91,   185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,   182,     0,     0,     0,     0,     0,     0,
     167,   163,   164,   166,   168,    31,   116,     0,   180,    19,
     175,   174,   173,     0,   127,   129,     0,    30,    52,    50,
       0,   109,    92,    88,    27,    23,    31,    28,    70,    69,
       0,     0,     0,    61,    62,    63,    65,    64,    66,    67,
       0,     0,    77,    75,     0,    71,    57,    58,    59,    60,
      48,    82,    83,    84,    85,    96,    95,    94,   107,   108,
       0,     0,     0,   115,     0,   125,     0,   138,   147,   152,
     154,   134,   158,   131,   132,   133,   135,   152,   152,   140,
     158,   158,   158,   156,   148,   156,   130,   113,     0,     0,
       0,    33,    68,     0,    76,     0,     0,    54,    72,   103,
     105,     0,   117,   118,   128,     0,   141,     0,   139,     0,
     144,   136,   137,   142,   143,   145,     0,   146,   149,   111,
       0,     0,    29,     0,    35,     0,    73,    78,     0,   104,
       0,     0,     0,     0,   114,     0,     0,     0,    36,    74,
      55,   151,     0,   153,   157,   155,   112,    32,    34,     0,
      38,    37,     0,     0,    39,    40,    41,   150,    45,     0,
      17,    43,    44,     0,    42
  };

  const short int
  parser::yypgoto_[] =
  {
    -284,  -284,  -284,    -2,     0,     2,  -284,  -284,  -284,  -158,
    -284,  -284,  -284,  -284,  -284,   -61,  -284,   131,  -133,  -283,
     -48,    72,   -47,  -284,  -284,  -284,  -284,  -284,  -284,   267,
    -284,   -15,   269,  -284,    52,   272,  -284,  -284,  -284,  -284,
      53,  -284,  -284,   -39,  -284,    33,   -62,  -284,  -284,  -284,
    -284,  -284,  -284,    27,    12,   191,  -110,  -284,   132,   118,
    -284,   -63,  -266
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    14,    15,    16,    93,    18,    50,    94,   196,   124,
     294,   308,   320,   326,   330,   331,   321,    95,    76,   266,
     267,   213,    97,    98,    99,   100,   101,    51,   231,    19,
     191,   258,    20,   175,   176,    21,    22,    46,    23,   183,
     184,   185,   256,   276,   278,   287,   280,    24,    25,   114,
      26,    27,    28,    77,    36,   102,   103,   104,   162,   105,
     106,   107,   108
  };

  const short int
  parser::yytable_[] =
  {
      17,    17,    96,    29,     1,   177,    42,     1,    17,   300,
     197,   301,   186,   302,   142,   260,    31,   233,     1,   214,
     303,    40,     1,   317,    34,    60,    34,   139,    34,   141,
     215,    30,    35,    39,   127,   135,   136,   138,   261,    59,
     328,    75,    69,    70,    32,   143,   322,   195,    10,    41,
      33,    10,    74,    68,   265,   235,   236,   131,   226,    71,
      72,    34,    34,   332,   117,   118,   119,   332,   123,   164,
     165,   166,   167,   324,    78,   187,   132,     1,    17,    47,
      34,   181,   325,    62,   123,   137,    63,    29,   129,    34,
     116,   140,  -181,    73,    96,  -179,   198,   199,   232,   203,
     204,   205,   206,   207,   208,   209,   225,   210,    48,   216,
     217,   218,   219,    49,    34,    67,   179,   221,   222,   223,
     224,   230,   177,    38,    38,   174,   186,   292,  -178,   295,
      38,    61,    79,     1,   192,   109,   170,   171,   172,   173,
     159,   160,   257,    75,   164,   165,   166,   167,    61,    61,
      38,    38,   262,   110,   263,   212,     4,   111,    61,    38,
     164,   165,   166,   167,   112,    38,   166,   167,    52,    53,
       9,   289,   290,    54,    64,    65,    66,    10,   200,   201,
     202,   297,   298,   271,    12,   113,   273,    55,   283,   284,
     285,   126,   115,   126,     1,   126,    37,    37,   120,    56,
      57,    58,   212,    37,   311,   312,   316,   290,   281,   282,
     121,   137,   122,    29,   163,   123,   133,   134,   296,    86,
     169,   168,   180,    37,    37,   190,   211,    38,    38,    38,
      38,    61,    37,   178,   234,   259,   268,   269,    37,   126,
     178,   275,   304,   257,   277,   126,   279,   126,   309,    87,
     310,   286,   291,   293,   306,   313,   307,   314,   315,   318,
      75,    38,   298,   323,   125,   319,   128,    80,   130,   327,
     329,   333,   334,   194,   264,    43,   305,    44,    81,   163,
      45,    82,    83,   227,   272,    85,   178,     0,   288,   274,
      86,    34,    87,   220,    88,    89,     0,     0,   228,     0,
      37,    37,    37,    37,     0,    90,     0,     0,    80,     0,
       0,   229,   182,     0,     0,    91,     0,     0,   188,    81,
     189,     0,    82,    83,    84,     0,    85,     0,     0,     0,
       0,    86,    34,    87,    37,    88,    89,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    90,   237,     0,     0,
     178,     0,    92,     0,   178,    81,    91,     0,    82,    83,
      84,     0,    85,     0,     0,     0,     0,    86,    34,    87,
      10,    88,    89,   238,     0,     0,     0,     0,    38,     0,
     239,     0,    90,     0,     0,   240,     0,   153,   154,   155,
     156,   241,    91,    92,   157,   158,   159,   160,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,     1,     0,     0,     0,     0,     0,     0,    92,
     154,   155,   156,     2,     0,     0,   157,   158,   159,   160,
       0,     3,     0,     0,     0,     4,     5,     6,     0,     0,
       7,    37,     0,     8,     0,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     0,     0,    10,     0,    11,     0,
       0,     0,     0,    12,    13,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,     0,     0,
       0,   157,   158,   159,   160,     0,     0,     0,     0,     0,
       0,    34,     0,     0,     0,     0,     0,     0,     0,   161,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,     0,     0,     0,   157,   158,   159,   160,
       0,   193,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,     0,     0,     0,   157,   158,
     159,   160,     0,   270,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,     0,     0,     0,
     157,   158,   159,   160,     0,   299,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,     0,
       0,     0,   157,   158,   159,   160,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,     0,     0,
       0,   157,   158,   159,   160,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,     0,     0,     0,   157,
     158,   159,   160
  };

  const short int
  parser::yycheck_[] =
  {
       0,     1,    50,     1,    26,   115,     8,    26,     8,   275,
     143,   277,   122,   279,    28,    28,    47,   175,    26,     8,
     286,    47,    26,   306,    32,    13,    32,    90,    32,    92,
      19,    75,     5,     6,    40,    82,    83,    85,   196,    12,
     323,    41,    30,    31,    75,    59,   312,    55,    70,    75,
      59,    70,    40,    24,     7,    27,    28,    79,   168,    32,
      33,    32,    32,   329,    64,    65,    66,   333,    81,    22,
      23,    24,    25,    41,    47,   123,    78,    26,    78,    65,
      32,    24,    50,     0,    81,    85,    30,    85,    40,    32,
      39,    91,    26,    29,   142,    29,   144,   145,    95,   147,
     148,   149,   150,   151,   152,   153,    24,   154,    47,   157,
     158,   159,   160,    52,    32,    27,   116,   164,   165,   166,
     167,   169,   232,     5,     6,   113,   236,   260,    29,     7,
      12,    13,    26,    26,   134,    73,   109,   110,   111,   112,
      24,    25,   190,   143,    22,    23,    24,    25,    30,    31,
      32,    33,   200,    59,   201,   155,    49,    59,    40,    41,
      22,    23,    24,    25,    73,    47,    24,    25,    43,    44,
      63,    27,    28,    48,     3,     4,     5,    70,    15,    16,
      17,    27,    28,   231,    77,    59,   234,    62,   250,   251,
     252,    73,    71,    75,    26,    77,     5,     6,    29,    74,
      75,    76,   202,    12,    27,    28,    27,    28,   247,   248,
      29,   211,    26,   211,    96,    81,    27,    56,   265,    31,
      26,    29,    24,    32,    33,    26,    26,   109,   110,   111,
     112,   113,    41,   115,    11,    28,    19,    27,    47,   121,
     122,    26,   290,   291,    26,   127,    26,   129,   295,    33,
     298,    26,    26,    60,    42,    27,    61,    27,    27,   307,
     260,   143,    28,    42,    73,    68,    75,     8,    77,    27,
      66,    28,   333,   142,   202,     8,   291,     8,    19,   161,
       8,    22,    23,    24,   232,    26,   168,    -1,   255,   236,
      31,    32,    33,   161,    35,    36,    -1,    -1,    39,    -1,
     109,   110,   111,   112,    -1,    46,    -1,    -1,     8,    -1,
      -1,    52,   121,    -1,    -1,    56,    -1,    -1,   127,    19,
     129,    -1,    22,    23,    24,    -1,    26,    -1,    -1,    -1,
      -1,    31,    32,    33,   143,    35,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    46,    20,    -1,    -1,
     232,    -1,    93,    -1,   236,    19,    56,    -1,    22,    23,
      24,    -1,    26,    -1,    -1,    -1,    -1,    31,    32,    33,
      70,    35,    36,    46,    -1,    -1,    -1,    -1,   260,    -1,
      53,    -1,    46,    -1,    -1,    58,    -1,    15,    16,    17,
      18,    64,    56,    93,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    26,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      16,    17,    18,    37,    -1,    -1,    22,    23,    24,    25,
      -1,    45,    -1,    -1,    -1,    49,    50,    51,    -1,    -1,
      54,   260,    -1,    57,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    72,    -1,
      -1,    -1,    -1,    77,    78,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    22,    23,    24,    25,
      -1,    27,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    22,    23,
      24,    25,    -1,    27,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      22,    23,    24,    25,    -1,    27,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    22,    23,    24,    25,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    22,    23,    24,    25,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    22,
      23,    24,    25
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    26,    37,    45,    49,    50,    51,    54,    57,    63,
      70,    72,    77,    78,    97,    98,    99,   100,   101,   125,
     128,   131,   132,   134,   143,   144,   146,   147,   148,   101,
      75,    47,    75,    59,    32,   149,   150,   151,   155,   149,
      47,    75,    99,   125,   128,   131,   133,    65,    47,    52,
     102,   123,    43,    44,    48,    62,    74,    75,    76,   149,
     150,   155,     0,    30,     3,     4,     5,    27,    24,   150,
     150,   149,   149,    29,   150,   100,   114,   149,   149,    26,
       8,    19,    22,    23,    24,    26,    31,    33,    35,    36,
      46,    56,    93,   100,   103,   113,   116,   118,   119,   120,
     121,   122,   151,   152,   153,   155,   156,   157,   158,    73,
      59,    59,    73,    59,   145,    71,    39,   100,   100,   100,
      29,    29,    26,    81,   105,   151,   155,    40,   151,    40,
     151,    79,    99,    27,    56,   118,   118,   100,   116,   157,
     100,   157,    28,    59,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    22,    23,    24,
      25,    40,   154,   155,    22,    23,    24,    25,    29,    26,
     149,   149,   149,   149,   150,   129,   130,   152,   155,   100,
      24,    24,   151,   135,   136,   137,   152,   116,   151,   151,
      26,   126,   100,    27,   113,    55,   104,   114,   116,   116,
      15,    16,    17,   116,   116,   116,   116,   116,   116,   116,
     118,    26,   100,   117,     8,    19,   116,   116,   116,   116,
     154,   118,   118,   118,   118,    24,   152,    24,    39,    52,
     116,   124,    95,   105,    11,    27,    28,    20,    46,    53,
      58,    64,    80,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   138,   116,   127,    28,
      28,   105,   116,   118,   117,     7,   115,   116,    19,    27,
      27,   116,   130,   116,   136,    26,   139,    26,   140,    26,
     142,   139,   139,   142,   142,   142,    26,   141,   141,    27,
      28,    26,   114,    60,   106,     7,   118,    27,    28,    27,
     158,   158,   158,   158,   116,   127,    42,    61,   107,   118,
     116,    27,    28,    27,    27,    27,    27,   115,   116,    68,
     108,   112,   158,    42,    41,    50,   109,    27,   115,    66,
     110,   111,   158,    28,   111
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    96,    97,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    99,   100,   101,   101,   101,
     101,   101,   101,   101,   102,   102,   103,   103,   104,   104,
     105,   105,   106,   106,   107,   107,   108,   108,   109,   109,
     109,   110,   110,   110,   111,   112,   113,   113,   113,   114,
     114,   114,   114,   114,   115,   115,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   117,   117,   118,
     118,   118,   118,   118,   118,   118,   119,   119,   119,   119,
     119,   119,   119,   120,   120,   120,   120,   121,   121,   121,
     121,   121,   121,   122,   122,   122,   123,   124,   124,   125,
     125,   126,   126,   127,   127,   128,   129,   129,   130,   131,
     132,   133,   133,   133,   133,   134,   134,   135,   135,   136,
     137,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     139,   139,   139,   140,   140,   141,   141,   142,   142,   143,
     143,   144,   144,   144,   144,   144,   144,   144,   145,   145,
     146,   147,   147,   148,   148,   148,   149,   149,   150,   151,
     152,   153,   154,   155,   156,   156,   157,   158
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
       3,     2,     3,     4,     4,     2,     4,     4,     2,     0,
       2,     2,     2,     5,     5,     5,     1,     3,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1
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
  "select_with_parens", "select_no_parens", "opt_distinct",
  "select_expr_list", "from_list", "opt_where", "opt_groupby",
  "opt_having", "opt_order_by", "opt_asc_desc", "opt_select_limit",
  "limit_expr", "order_by", "projection", "table_factor", "expr_list",
  "expr", "in_expr", "arith_expr", "simple_expr", "column_ref",
  "expr_const", "func_expr", "simple_function_expr", "distinct_or_all",
  "insert_stmt", "insert_vals_list", "insert_vals", "update_stmt",
  "update_asgn_list", "update_asgn_factor", "delete_stmt", "explain_stmt",
  "explainable_stmt", "create_stmt", "table_element_list", "table_element",
  "column_definition", "data_type", "opt_decimal", "opt_float",
  "opt_time_precision", "opt_char_length", "drop_stmt", "show_stmt",
  "op_from_database", "use_stmt", "desc_stmt", "analyze_stmt",
  "relation_factor", "database_name", "relation_name", "column_name",
  "function_name", "column_label", "ident", "datetime", "string", "number", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   289,   289,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   317,   324,   334,   353,   358,
     363,   368,   373,   377,   393,   394,   398,   403,   411,   416,
     424,   425,   429,   430,   434,   435,   439,   440,   444,   445,
     446,   450,   451,   458,   468,   475,   482,   490,   498,   509,
     513,   521,   529,   537,   552,   557,   566,   570,   575,   580,
     585,   590,   595,   600,   605,   610,   615,   620,   625,   630,
     635,   640,   645,   650,   655,   660,   665,   675,   679,   686,
     690,   695,   700,   705,   710,   715,   723,   727,   731,   735,
     739,   743,   747,   755,   762,   768,   776,   786,   795,   804,
     813,   822,   831,   843,   853,   862,   873,   880,   884,   895,
     905,   912,   916,   922,   927,   940,   953,   958,   966,   981,
     998,  1006,  1007,  1008,  1009,  1018,  1027,  1038,  1042,  1049,
    1056,  1065,  1067,  1069,  1071,  1073,  1075,  1077,  1079,  1081,
    1083,  1085,  1087,  1089,  1091,  1093,  1095,  1097,  1099,  1101,
    1106,  1107,  1108,  1112,  1113,  1117,  1118,  1122,  1123,  1132,
    1140,  1156,  1160,  1164,  1172,  1176,  1180,  1189,  1201,  1206,
    1210,  1226,  1234,  1249,  1258,  1267,  1284,  1291,  1301,  1305,
    1309,  1313,  1317,  1321,  1325,  1326,  1330,  1334
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


#line 3 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:1167
} // CatDB
#line 3134 "/home/test/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:1167
#line 1336 "/home/test/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
