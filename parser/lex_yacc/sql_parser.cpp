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
#line 26 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:397

	#include "sql_parser.hpp"
	#include "sql_scanner.h"
	#include "sql_driver.h"
	#include "location.hh"
	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
	#include "cmd_stmt.h"
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

#line 57 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:397


// First part of user declarations.
#line 49 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:404

	
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

#line 160 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sql_parser.hpp"

// User implementation prologue.

#line 174 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:412


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

#line 3 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:479
namespace CatDB {
#line 260 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:479

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
      case 98: // sql_stmt
      case 99: // stmt
      case 100: // select_stmt
      case 101: // select_no_parens
      case 102: // select_with_parens
      case 103: // simple_select
      case 105: // select_expr_list
      case 106: // from_list
      case 107: // opt_where
      case 108: // opt_groupby
      case 109: // opt_having
      case 110: // opt_order_by
      case 112: // opt_select_limit
      case 114: // order_by
      case 115: // projection
      case 116: // table_factor
      case 117: // expr_list
      case 118: // expr
      case 119: // in_expr
      case 120: // arith_expr
      case 121: // simple_expr
      case 122: // column_ref
      case 123: // expr_const
      case 124: // func_expr
      case 127: // insert_stmt
      case 128: // insert_vals_list
      case 129: // insert_vals
      case 130: // update_stmt
      case 131: // update_asgn_list
      case 132: // update_asgn_factor
      case 133: // delete_stmt
      case 134: // explain_stmt
      case 135: // explainable_stmt
      case 136: // create_stmt
      case 137: // table_element_list
      case 138: // table_element
      case 139: // column_definition
      case 145: // drop_stmt
      case 146: // show_stmt
      case 148: // use_stmt
      case 149: // desc_stmt
      case 150: // analyze_stmt
      case 151: // relation_factor
        value.move< Stmt_s > (that.value);
        break;

      case 104: // opt_distinct
      case 111: // opt_asc_desc
      case 126: // distinct_or_all
        value.move< bool > (that.value);
        break;

      case 113: // limit_expr
      case 140: // data_type
        value.move< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 125: // simple_function_expr
      case 147: // op_from_database
      case 152: // database_name
      case 153: // relation_name
      case 154: // column_name
      case 155: // function_name
      case 156: // column_label
      case 157: // ident
      case 158: // datetime
      case 159: // string
      case 160: // number
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
      case 98: // sql_stmt
      case 99: // stmt
      case 100: // select_stmt
      case 101: // select_no_parens
      case 102: // select_with_parens
      case 103: // simple_select
      case 105: // select_expr_list
      case 106: // from_list
      case 107: // opt_where
      case 108: // opt_groupby
      case 109: // opt_having
      case 110: // opt_order_by
      case 112: // opt_select_limit
      case 114: // order_by
      case 115: // projection
      case 116: // table_factor
      case 117: // expr_list
      case 118: // expr
      case 119: // in_expr
      case 120: // arith_expr
      case 121: // simple_expr
      case 122: // column_ref
      case 123: // expr_const
      case 124: // func_expr
      case 127: // insert_stmt
      case 128: // insert_vals_list
      case 129: // insert_vals
      case 130: // update_stmt
      case 131: // update_asgn_list
      case 132: // update_asgn_factor
      case 133: // delete_stmt
      case 134: // explain_stmt
      case 135: // explainable_stmt
      case 136: // create_stmt
      case 137: // table_element_list
      case 138: // table_element
      case 139: // column_definition
      case 145: // drop_stmt
      case 146: // show_stmt
      case 148: // use_stmt
      case 149: // desc_stmt
      case 150: // analyze_stmt
      case 151: // relation_factor
        value.copy< Stmt_s > (that.value);
        break;

      case 104: // opt_distinct
      case 111: // opt_asc_desc
      case 126: // distinct_or_all
        value.copy< bool > (that.value);
        break;

      case 113: // limit_expr
      case 140: // data_type
        value.copy< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 125: // simple_function_expr
      case 147: // op_from_database
      case 152: // database_name
      case 153: // relation_name
      case 154: // column_name
      case 155: // function_name
      case 156: // column_label
      case 157: // ident
      case 158: // datetime
      case 159: // string
      case 160: // number
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
      case 98: // sql_stmt
      case 99: // stmt
      case 100: // select_stmt
      case 101: // select_no_parens
      case 102: // select_with_parens
      case 103: // simple_select
      case 105: // select_expr_list
      case 106: // from_list
      case 107: // opt_where
      case 108: // opt_groupby
      case 109: // opt_having
      case 110: // opt_order_by
      case 112: // opt_select_limit
      case 114: // order_by
      case 115: // projection
      case 116: // table_factor
      case 117: // expr_list
      case 118: // expr
      case 119: // in_expr
      case 120: // arith_expr
      case 121: // simple_expr
      case 122: // column_ref
      case 123: // expr_const
      case 124: // func_expr
      case 127: // insert_stmt
      case 128: // insert_vals_list
      case 129: // insert_vals
      case 130: // update_stmt
      case 131: // update_asgn_list
      case 132: // update_asgn_factor
      case 133: // delete_stmt
      case 134: // explain_stmt
      case 135: // explainable_stmt
      case 136: // create_stmt
      case 137: // table_element_list
      case 138: // table_element
      case 139: // column_definition
      case 145: // drop_stmt
      case 146: // show_stmt
      case 148: // use_stmt
      case 149: // desc_stmt
      case 150: // analyze_stmt
      case 151: // relation_factor
        yylhs.value.build< Stmt_s > ();
        break;

      case 104: // opt_distinct
      case 111: // opt_asc_desc
      case 126: // distinct_or_all
        yylhs.value.build< bool > ();
        break;

      case 113: // limit_expr
      case 140: // data_type
        yylhs.value.build< int > ();
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 125: // simple_function_expr
      case 147: // op_from_database
      case 152: // database_name
      case 153: // relation_name
      case 154: // column_name
      case 155: // function_name
      case 156: // column_label
      case 157: // ident
      case 158: // datetime
      case 159: // string
      case 160: // number
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
#line 289 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 864 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 296 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 870 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 297 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 876 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 298 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 882 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 299 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 888 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 300 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 894 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 301 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 900 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 302 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 906 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 303 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 912 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 304 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 918 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 305 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 924 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 306 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 930 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 307 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow stmt"); }
#line 936 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 317 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 944 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 321 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 952 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 328 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 960 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 332 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_UNION);
    }
#line 969 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 337 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_UNION_ALL);
    }
#line 978 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 342 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_INTERSECT);
    }
#line 987 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 347 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EXCEPT);
    }
#line 996 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 355 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = QueryStmt::make_query_stmt();
		QueryStmt* query = dynamic_cast<QueryStmt*>(stmt.get());
		query->query_stmt = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1007 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 368 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
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
#line 1027 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 384 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowDatabases);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.show_databases_params.is_select_current_database = true;
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1039 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 392 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
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
#line 1056 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 407 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1062 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 408 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1068 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 413 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建select list
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1077 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 418 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//把输出表达式加入到select list
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1086 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 426 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建from list 
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
	}
#line 1095 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 431 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//把表加入到from list
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
	}
#line 1104 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 438 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1110 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 439 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1116 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 443 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1122 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 444 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1128 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 448 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1134 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 449 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1140 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 453 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = NULL; }
#line 1146 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 454 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1152 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 458 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1158 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 459 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1164 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 460 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1170 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 464 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< Stmt_s > () = NULL;}
#line 1176 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 466 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1187 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 473 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		Stmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1198 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 483 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 1206 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 490 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1214 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 497 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
	//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[0].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = stmt->to_string();
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1226 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 505 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
    }
#line 1238 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 513 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[2].value.as< Stmt_s > ();
    }
#line 1250 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 524 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1258 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 528 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[2].value.as< Stmt_s > ();
    }
#line 1270 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 536 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
    }
#line 1282 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 544 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[2].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[2].value.as< Stmt_s > ();
    }
#line 1294 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 552 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	//设置表的别名
		ExprStmt* stmt = dynamic_cast<ExprStmt*>(yystack_[1].value.as< Stmt_s > ().get());
		check(stmt);
		stmt->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
    }
#line 1306 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 567 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1315 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 572 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1324 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 581 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1332 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 585 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建加法二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1341 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 590 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建减法二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1350 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 595 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建乘法二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1359 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 600 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建除法二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1368 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 605 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_LE);
	}
#line 1377 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 610 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_LT);
	}
#line 1386 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 615 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EQ);
	}
#line 1395 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 620 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_GE);
	}
#line 1404 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 625 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_GT);
	}
#line 1413 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 630 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NE);
	}
#line 1422 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 635 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_LIKE);
	}
#line 1431 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 640 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_LIKE);
	}
#line 1440 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 645 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_AND);
    }
#line 1449 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 650 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_OR);
    }
#line 1458 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 655 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), ExprStmt::OP_IS_NULL);
    }
#line 1467 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 660 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), ExprStmt::OP_IS_NOT_NULL);
    }
#line 1476 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 665 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< Stmt_s > (), yystack_[4].value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_BETWEEN);
    }
#line 1485 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 670 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< Stmt_s > (), yystack_[5].value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_BETWEEN);
    }
#line 1494 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 675 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_IN);
    }
#line 1503 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 680 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_IN);
    }
#line 1512 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 690 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1520 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 694 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1528 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 701 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1536 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 705 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1545 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 710 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_MINUS);
    }
#line 1554 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 715 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_ADD);
	}
#line 1563 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 720 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_SUB);
	}
#line 1572 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 725 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_MUL);
	}
#line 1581 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 730 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_DIV);
	}
#line 1590 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 738 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1598 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 742 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1606 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 746 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1614 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 750 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1622 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 754 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 1630 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 758 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EXISTS);
    }
#line 1638 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 762 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_NOT_EXISTS);
    }
#line 1647 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 770 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< Stmt_s > () = col;
	}
#line 1658 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 777 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< Stmt_s > () = col;
	}
#line 1668 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 783 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< Stmt_s > () = col;
    }
#line 1679 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 791 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< Stmt_s > () = col;
    }
#line 1690 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 801 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1703 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 810 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = DateTime::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1716 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 819 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1729 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 828 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1742 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 837 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1755 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 846 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		Stmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 1768 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 858 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as< std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		Stmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->aggr_expr = col;
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1782 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 868 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[4].value.as< std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->distinct = yystack_[2].value.as< bool > ();
		aggr->aggr_expr = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1795 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 877 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt;
		make_aggr_stmt(stmt, yystack_[3].value.as< std::string > ());
		AggrStmt* aggr = dynamic_cast<AggrStmt*>(stmt.get());
		aggr->aggr_expr = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1807 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 888 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "database";
    }
#line 1815 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 895 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 1823 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 899 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 1831 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 910 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		Stmt_s stmt = InsertStmt::make_insert_stmt();
		check(stmt);
		InsertStmt* insert_stmt = dynamic_cast<InsertStmt*>(stmt.get());
		insert_stmt->table = yystack_[2].value.as< Stmt_s > ();
		insert_stmt->values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1845 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 920 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yyerror("insert into table from query not support yet!");
		YYABORT;
    }
#line 1854 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 927 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[1].value.as< Stmt_s > ());
    }
#line 1862 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 931 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[4].value.as< Stmt_s > (), yystack_[1].value.as< Stmt_s > ());
	}
#line 1870 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 937 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { 
		//构建值列表
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
	}
#line 1879 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 942 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1888 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 955 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = UpdateStmt::make_update_stmt();
		check(stmt);
		UpdateStmt* update_stmt = dynamic_cast<UpdateStmt*>(stmt.get());
		update_stmt->table = yystack_[3].value.as< Stmt_s > ();
		update_stmt->update_asgn_stmt = yystack_[1].value.as< Stmt_s > ();
		update_stmt->where_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1902 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 968 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1911 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 973 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 1920 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 981 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		Stmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< Stmt_s > (), col, yystack_[0].value.as< Stmt_s > (), ExprStmt::OP_EQ);
    }
#line 1931 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 996 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = DeleteStmt::make_delete_stmt();
		check(stmt);
		DeleteStmt* delete_stmt = dynamic_cast<DeleteStmt*>(stmt.get());
		delete_stmt->table = yystack_[1].value.as< Stmt_s > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1944 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 1013 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > ()->is_explain = true;
    }
#line 1953 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 1020 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1959 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 1021 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1965 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 1022 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1971 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 1023 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1977 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1033 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::CreateTable);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.create_table_params.table = yystack_[3].value.as< Stmt_s > ();
		cmd_stmt->params.create_table_params.column_define_list = yystack_[1].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 1990 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1042 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::CreateDatabase);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.create_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2002 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1053 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		make_list_stmt(yylhs.value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 2010 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1057 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		list_stmt_push(yylhs.value.as< Stmt_s > (), yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > ());
    }
#line 2018 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1064 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
    }
#line 2026 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1071 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as< std::string > (), yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2036 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1080 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2042 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1082 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2048 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1084 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2054 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1086 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2060 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1088 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2066 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1090 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2072 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1092 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2078 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1094 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2084 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1096 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2090 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1098 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2096 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1100 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2102 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1102 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2108 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1104 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2114 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1106 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2120 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1108 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2126 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1110 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2132 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1112 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2138 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1114 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2144 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1116 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2150 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1120 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2156 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1121 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2162 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1122 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2168 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1126 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2174 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1127 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2180 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1131 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2186 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1132 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2192 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1136 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2198 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1137 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { }
#line 2204 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1147 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::DropTable);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.drop_table_params.table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2216 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1155 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::DropDatabase);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.drop_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2228 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1171 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowDatabases);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		yylhs.value.as< Stmt_s > () = stmt;
 	}
#line 2240 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1179 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowTables);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2252 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1187 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowTables);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2264 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1195 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2276 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1203 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 2284 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1207 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 2292 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1211 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< Stmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2305 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1220 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< Stmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2318 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1232 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2326 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1236 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 2334 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1241 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::UseDatabase);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.use_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2346 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1257 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2358 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1265 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = stmt;
	}
#line 2370 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1280 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.analyze_params.database = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2383 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1289 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.analyze_params.database = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = "*";
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2396 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1298 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		Stmt_s stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(stmt);
		CMDStmt* cmd_stmt = dynamic_cast<CMDStmt*>(stmt.get());
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		yylhs.value.as< Stmt_s > () = stmt;
    }
#line 2409 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1315 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		yylhs.value.as< Stmt_s > () = table;
	}
#line 2420 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1322 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		Stmt_s table = TableStmt::make_table_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		yylhs.value.as< Stmt_s > () = table;
	}
#line 2431 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1331 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2437 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1335 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2443 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1339 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2449 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1343 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2455 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1347 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 2461 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1351 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2467 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1355 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2473 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1356 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2479 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1360 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2485 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1364 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2491 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
    break;


#line 2495 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -272;

  const short int parser::yytable_ninf_ = -185;

  const short int
  parser::yypact_[] =
  {
     150,   -22,   -52,   -35,    -9,     8,     8,   -25,   -16,    28,
      45,   211,     8,     8,    74,    79,   170,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
      42,    96,     8,     8,     8,  -272,  -272,    98,  -272,   137,
    -272,     8,    47,   170,  -272,  -272,  -272,  -272,     8,   145,
    -272,   146,  -272,   106,   129,  -272,   114,   144,  -272,   123,
     157,   133,  -272,  -272,  -272,  -272,   -12,   -22,   -22,  -272,
     193,   203,  -272,   210,   165,     8,  -272,    90,  -272,   108,
     -10,   223,   196,  -272,   146,   146,  -272,   308,  -272,  -272,
    -272,  -272,   227,   237,   227,  -272,   -15,  -272,   422,   202,
    -272,  -272,  -272,  -272,   246,  -272,   250,   118,  -272,  -272,
    -272,     8,     8,   157,     8,     8,     8,  -272,     8,   -22,
     272,   272,  -272,   255,   117,     8,   146,  -272,  -272,  -272,
       8,  -272,     8,  -272,   254,   170,  -272,   237,  -272,  -272,
      81,   457,  -272,  -272,  -272,   146,     5,   146,   146,   168,
     146,   146,   146,   146,   146,   146,   146,   146,   256,    57,
     146,   146,   146,   146,     8,  -272,  -272,   146,   146,   146,
     146,   132,   273,  -272,  -272,  -272,  -272,  -272,  -272,   -62,
    -272,   274,  -272,   170,  -272,  -272,  -272,   115,  -272,  -272,
     332,   523,  -272,  -272,   146,   261,  -272,  -272,  -272,  -272,
     -19,  -272,   542,   560,   146,   146,   256,   371,   371,   371,
     371,   371,   371,   382,    93,   308,  -272,  -272,   275,  -272,
     138,   138,  -272,  -272,  -272,   188,   188,  -272,  -272,  -272,
    -272,   264,  -272,  -272,   479,   146,     8,  -272,   146,  -272,
       8,  -272,  -272,   267,   276,  -272,   277,  -272,  -272,  -272,
    -272,   267,   267,  -272,   277,   277,   277,   281,  -272,   281,
    -272,   523,   191,   284,    47,   239,   382,   186,  -272,   146,
     229,   523,  -272,  -272,  -272,   501,  -272,   523,  -272,   265,
    -272,   265,  -272,   265,  -272,  -272,  -272,  -272,  -272,  -272,
     265,  -272,  -272,  -272,   146,   146,  -272,   259,   252,   146,
     202,  -272,   146,  -272,   233,   290,   291,   293,   523,   245,
     146,   146,   253,   202,   523,  -272,   265,  -272,  -272,  -272,
    -272,   295,   523,   279,     2,  -272,   297,   146,  -272,  -272,
     266,  -272,   295,   265,  -272,   298,  -272,   265,  -272
  };

  const unsigned char
  parser::yydefact_[] =
  {
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,     3,    15,    16,    17,
       4,     5,     6,     7,     9,    10,     8,    12,    11,    13,
       0,     0,     0,     0,     0,   186,   175,     0,   179,   182,
     174,     0,     0,   123,   125,   126,   124,   122,     0,     0,
      27,     0,    24,     0,     0,   163,     0,     0,   168,     0,
     172,     0,   173,   181,     1,     2,     0,     0,     0,    22,
       0,     0,   128,     0,    33,     0,   162,     0,   161,    51,
       0,     0,     0,   104,     0,     0,    96,     0,   189,   190,
     102,   103,     0,     0,     0,    92,     0,    28,    48,    58,
      81,    88,    89,    91,     0,    95,     0,   183,   100,    99,
     101,     0,     0,   172,     0,     0,     0,   165,     0,     0,
      18,    21,    20,     0,     0,     0,     0,   121,   180,   182,
       0,    55,     0,    53,     0,   112,   108,     0,    82,    83,
      92,     0,   187,    93,   188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,   185,     0,     0,     0,
       0,     0,     0,   170,   166,   164,   167,   169,   171,    33,
     118,     0,   183,    19,   178,   177,   176,     0,   129,   131,
       0,    32,    54,    52,     0,   111,    94,    90,    29,    25,
      33,    30,    72,    71,     0,     0,     0,    63,    64,    65,
      67,    66,    68,    69,     0,     0,    79,    77,     0,    73,
      59,    60,    61,    62,    50,    84,    85,    86,    87,    98,
      97,    96,   109,   110,     0,     0,     0,   117,     0,   127,
       0,   140,   149,   154,   156,   136,   160,   133,   134,   135,
     137,   154,   154,   142,   160,   160,   160,   158,   150,   158,
     132,   115,     0,     0,     0,    35,    70,     0,    78,     0,
       0,    56,    74,   105,   107,     0,   119,   120,   130,     0,
     143,     0,   141,     0,   146,   138,   139,   144,   145,   147,
       0,   148,   151,   113,     0,     0,    31,     0,    37,     0,
      75,    80,     0,   106,     0,     0,     0,     0,   116,     0,
       0,     0,    38,    76,    57,   153,     0,   155,   159,   157,
     114,    34,    36,     0,    40,    39,     0,     0,    41,    42,
      43,   152,    47,     0,    23,    45,    46,     0,    44
  };

  const short int
  parser::yypgoto_[] =
  {
    -272,  -272,  -272,    15,  -272,     0,  -272,  -272,  -272,  -272,
    -147,  -272,  -272,  -272,  -272,  -272,    -2,  -272,   183,  -143,
    -271,   -49,   130,   -79,  -272,  -272,  -272,  -272,  -272,  -272,
     329,  -272,    43,   334,  -272,   109,   338,  -272,  -272,  -272,
    -272,   113,  -272,  -272,   -13,  -272,    97,    12,  -272,  -272,
     244,  -272,  -272,  -272,    24,    -6,   236,  -107,  -272,   185,
     119,  -272,    65,  -262
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    14,    15,    30,    17,    95,    19,    51,    96,   200,
     127,   298,   312,   324,   330,   334,   335,   325,    97,    78,
     270,   271,   217,    99,   100,   101,   102,   103,    52,   235,
      20,   195,   262,    21,   179,   180,    22,    23,    47,    24,
     187,   188,   189,   260,   280,   282,   291,   284,    25,    26,
     117,    27,    28,    29,    79,    37,   104,   105,   106,   165,
     107,   108,   109,   110
  };

  const short int
  parser::yytable_[] =
  {
      18,    18,    98,   201,     1,   138,   139,    62,    18,   264,
       1,   181,    32,   145,     1,    16,     1,   304,   190,   305,
     126,   306,    41,    43,    31,    71,    72,   119,   307,    36,
      40,     1,   237,     4,   236,    76,    61,    35,   141,   321,
      35,    33,    77,   328,   146,    66,    67,    68,     9,    10,
      34,    42,   329,   265,   326,    10,   332,    73,    74,    10,
     199,    10,    12,   126,   230,   218,    18,    18,    18,    69,
     134,   336,    80,     1,    64,   336,   219,   191,   214,    35,
      18,   120,   121,   122,   -16,   -16,   -16,   140,   225,   226,
     227,   228,    49,   143,    48,   135,    98,    50,   202,   203,
     269,   207,   208,   209,   210,   211,   212,   213,   -16,    65,
     178,   220,   221,   222,   223,   167,   168,   169,   170,    18,
      70,   296,    35,   234,    39,    39,   267,    75,    35,   181,
     130,    39,    63,   190,   183,   173,   174,   196,   176,   177,
      35,   185,   239,   240,  -184,   261,    77,  -182,   132,    35,
      63,    63,    39,    39,    82,   266,   229,   142,   216,   144,
      63,    39,   162,   163,    35,    83,  -181,    39,    84,    85,
      86,    81,    87,    66,    67,    68,     1,    88,    35,    89,
     111,    90,    91,   204,   205,   206,   275,     2,   112,   277,
     300,   113,    92,   299,   129,     3,   129,   115,   129,     4,
       5,     6,    93,   114,     7,   118,   216,     8,   167,   168,
     169,   170,   169,   170,     9,   140,   116,   166,   293,   294,
     313,    10,   123,    11,   167,   168,   169,   170,    12,    13,
      39,    39,   124,    39,    39,    63,   125,   182,   285,   286,
      94,    38,    38,   129,   182,   308,   261,   126,    38,   129,
     136,   129,   137,   314,    53,    54,   301,   302,    88,    55,
     315,   316,   322,     1,    77,    39,   287,   288,   289,    38,
      38,    56,   320,   294,    57,   171,   172,    68,    38,   184,
     194,    82,   215,   166,    38,   238,    58,    59,    60,   263,
     182,   273,    83,   279,   272,    84,    85,   231,    89,    87,
     297,   310,   281,   283,    88,    35,    89,   290,    90,    91,
     295,   128,   232,   131,   311,   133,    82,   317,   318,    92,
     319,   327,   323,   302,   331,   233,   337,    83,   198,    93,
      84,    85,    86,   333,    87,   338,   268,    44,   309,    88,
      35,    89,    45,    90,    91,   276,    46,    38,    38,   224,
      38,    38,   241,   278,    92,   182,   292,   175,     0,   182,
     186,     0,     0,     0,    93,     0,   192,    94,   193,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   242,    10,
       0,     0,    38,    39,     0,   243,   156,   157,   158,   159,
     244,     0,     0,   160,   161,   162,   163,   245,   157,   158,
     159,     0,    94,     0,   160,   161,   162,   163,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,     0,     0,     0,   160,   161,   162,   163,     0,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,     0,
       0,     0,   164,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,     0,     0,     0,   160,
     161,   162,   163,     0,   197,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,     0,     0,
      38,   160,   161,   162,   163,     0,   274,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
       0,     0,     0,   160,   161,   162,   163,     0,   303,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,     0,     0,     0,   160,   161,   162,   163,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,     0,     0,     0,   160,   161,   162,   163,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,     0,
       0,     0,   160,   161,   162,   163
  };

  const short int
  parser::yycheck_[] =
  {
       0,     1,    51,   146,    26,    84,    85,    13,     8,    28,
      26,   118,    47,    28,    26,     0,    26,   279,   125,   281,
      82,   283,    47,     8,    76,    31,    32,    39,   290,     5,
       6,    26,   179,    49,    96,    41,    12,    32,    87,   310,
      32,    76,    42,    41,    59,     3,     4,     5,    64,    71,
      59,    76,    50,   200,   316,    71,   327,    33,    34,    71,
      55,    71,    78,    82,   171,     8,    66,    67,    68,    27,
      80,   333,    48,    26,     0,   337,    19,   126,   157,    32,
      80,    66,    67,    68,     3,     4,     5,    87,   167,   168,
     169,   170,    47,    93,    66,    80,   145,    52,   147,   148,
       7,   150,   151,   152,   153,   154,   155,   156,    27,    30,
     116,   160,   161,   162,   163,    22,    23,    24,    25,   119,
      24,   264,    32,   172,     5,     6,   205,    29,    32,   236,
      40,    12,    13,   240,   119,   111,   112,   137,   114,   115,
      32,    24,    27,    28,    26,   194,   146,    29,    40,    32,
      31,    32,    33,    34,     8,   204,    24,    92,   158,    94,
      41,    42,    24,    25,    32,    19,    29,    48,    22,    23,
      24,    26,    26,     3,     4,     5,    26,    31,    32,    33,
      74,    35,    36,    15,    16,    17,   235,    37,    59,   238,
     269,    77,    46,     7,    75,    45,    77,    74,    79,    49,
      50,    51,    56,    59,    54,    72,   206,    57,    22,    23,
      24,    25,    24,    25,    64,   215,    59,    98,    27,    28,
     299,    71,    29,    73,    22,    23,    24,    25,    78,    79,
     111,   112,    29,   114,   115,   116,    26,   118,   251,   252,
      94,     5,     6,   124,   125,   294,   295,    82,    12,   130,
      27,   132,    56,   302,    43,    44,    27,    28,    31,    48,
      27,    28,   311,    26,   264,   146,   254,   255,   256,    33,
      34,    60,    27,    28,    63,    29,    26,     5,    42,    24,
      26,     8,    26,   164,    48,    11,    75,    76,    77,    28,
     171,    27,    19,    26,    19,    22,    23,    24,    33,    26,
      61,    42,    26,    26,    31,    32,    33,    26,    35,    36,
      26,    75,    39,    77,    62,    79,     8,    27,    27,    46,
      27,    42,    69,    28,    27,    52,    28,    19,   145,    56,
      22,    23,    24,    67,    26,   337,   206,     8,   295,    31,
      32,    33,     8,    35,    36,   236,     8,   111,   112,   164,
     114,   115,    20,   240,    46,   236,   259,   113,    -1,   240,
     124,    -1,    -1,    -1,    56,    -1,   130,    94,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    71,
      -1,    -1,   146,   264,    -1,    53,    15,    16,    17,    18,
      58,    -1,    -1,    22,    23,    24,    25,    65,    16,    17,
      18,    -1,    94,    -1,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    -1,    27,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
     264,    22,    23,    24,    25,    -1,    27,     6,     7,     8,
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
       0,    26,    37,    45,    49,    50,    51,    54,    57,    64,
      71,    73,    78,    79,    98,    99,   100,   101,   102,   103,
     127,   130,   133,   134,   136,   145,   146,   148,   149,   150,
     100,    76,    47,    76,    59,    32,   151,   152,   153,   157,
     151,    47,    76,   100,   127,   130,   133,   135,    66,    47,
      52,   104,   125,    43,    44,    48,    60,    63,    75,    76,
      77,   151,   152,   157,     0,    30,     3,     4,     5,    27,
      24,   152,   152,   151,   151,    29,   152,   102,   116,   151,
     151,    26,     8,    19,    22,    23,    24,    26,    31,    33,
      35,    36,    46,    56,    94,   102,   105,   115,   118,   120,
     121,   122,   123,   124,   153,   154,   155,   157,   158,   159,
     160,    74,    59,    77,    59,    74,    59,   147,    72,    39,
     100,   100,   100,    29,    29,    26,    82,   107,   153,   157,
      40,   153,    40,   153,    80,   100,    27,    56,   120,   120,
     102,   118,   159,   102,   159,    28,    59,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      22,    23,    24,    25,    40,   156,   157,    22,    23,    24,
      25,    29,    26,   151,   151,   147,   151,   151,   152,   131,
     132,   154,   157,   100,    24,    24,   153,   137,   138,   139,
     154,   118,   153,   153,    26,   128,   102,    27,   115,    55,
     106,   116,   118,   118,    15,    16,    17,   118,   118,   118,
     118,   118,   118,   118,   120,    26,   102,   119,     8,    19,
     118,   118,   118,   118,   156,   120,   120,   120,   120,    24,
     154,    24,    39,    52,   118,   126,    96,   107,    11,    27,
      28,    20,    46,    53,    58,    65,    81,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
     140,   118,   129,    28,    28,   107,   118,   120,   119,     7,
     117,   118,    19,    27,    27,   118,   132,   118,   138,    26,
     141,    26,   142,    26,   144,   141,   141,   144,   144,   144,
      26,   143,   143,    27,    28,    26,   116,    61,   108,     7,
     120,    27,    28,    27,   160,   160,   160,   160,   118,   129,
      42,    62,   109,   120,   118,    27,    28,    27,    27,    27,
      27,   117,   118,    69,   110,   114,   160,    42,    41,    50,
     111,    27,   117,    67,   112,   113,   160,    28,   113
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    97,    98,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,   100,   100,   101,   101,   101,
     101,   101,   102,   103,   103,   103,   104,   104,   105,   105,
     106,   106,   107,   107,   108,   108,   109,   109,   110,   110,
     111,   111,   111,   112,   112,   112,   113,   114,   115,   115,
     115,   116,   116,   116,   116,   116,   117,   117,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   119,
     119,   120,   120,   120,   120,   120,   120,   120,   121,   121,
     121,   121,   121,   121,   121,   122,   122,   122,   122,   123,
     123,   123,   123,   123,   123,   124,   124,   124,   125,   126,
     126,   127,   127,   128,   128,   129,   129,   130,   131,   131,
     132,   133,   134,   135,   135,   135,   135,   136,   136,   137,
     137,   138,   139,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   141,   141,   141,   142,   142,   143,   143,   144,
     144,   145,   145,   146,   146,   146,   146,   146,   146,   146,
     146,   147,   147,   148,   149,   149,   150,   150,   150,   151,
     151,   152,   153,   154,   155,   156,   157,   158,   158,   159,
     160
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     1,     3,     4,
       3,     3,     3,    11,     2,     5,     0,     1,     1,     3,
       1,     3,     2,     0,     3,     0,     2,     0,     0,     1,
       0,     1,     1,     0,     4,     2,     1,     3,     1,     2,
       3,     1,     3,     2,     3,     2,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     3,     3,     4,     5,     6,     3,     4,     1,
       3,     1,     2,     2,     3,     3,     3,     3,     1,     1,
       3,     1,     1,     2,     3,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     4,     5,     4,     3,     1,
       1,     5,     4,     3,     5,     1,     3,     5,     1,     3,
       3,     4,     2,     1,     1,     1,     1,     6,     3,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     2,     2,
       1,     2,     1,     2,     2,     2,     2,     2,     2,     1,
       1,     2,     5,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     3,     2,     4,     3,     4,     4,     2,     4,
       4,     2,     0,     2,     2,     2,     5,     5,     5,     1,
       3,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1
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
  "FROM", "FULL", "GROUP", "HAVING", "INDEX", "INSERT", "INT", "INTO",
  "LIMIT", "NUMBER", "ORDER", "PARALLEL", "SELECT", "SET", "SHOW",
  "STATIS", "STATUS", "TABLE", "TABLES", "UPDATE", "USING", "VALUES",
  "VARCHAR", "WHERE", "TINYINT", "SMALLINT", "MEDIUMINT", "BIGINT",
  "DECIMAL", "NUMERIC_SYM", "REAL", "CHAR", "BINARY", "VARBINARY",
  "TIMESTAMP_SYM", "DATE", "TIME", "','", "$accept", "sql_stmt", "stmt",
  "select_stmt", "select_no_parens", "select_with_parens", "simple_select",
  "opt_distinct", "select_expr_list", "from_list", "opt_where",
  "opt_groupby", "opt_having", "opt_order_by", "opt_asc_desc",
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
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   288,   288,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   316,   320,   327,   331,   336,
     341,   346,   354,   364,   383,   391,   407,   408,   412,   417,
     425,   430,   438,   439,   443,   444,   448,   449,   453,   454,
     458,   459,   460,   464,   465,   472,   482,   489,   496,   504,
     512,   523,   527,   535,   543,   551,   566,   571,   580,   584,
     589,   594,   599,   604,   609,   614,   619,   624,   629,   634,
     639,   644,   649,   654,   659,   664,   669,   674,   679,   689,
     693,   700,   704,   709,   714,   719,   724,   729,   737,   741,
     745,   749,   753,   757,   761,   769,   776,   782,   790,   800,
     809,   818,   827,   836,   845,   857,   867,   876,   887,   894,
     898,   909,   919,   926,   930,   936,   941,   954,   967,   972,
     980,   995,  1012,  1020,  1021,  1022,  1023,  1032,  1041,  1052,
    1056,  1063,  1070,  1079,  1081,  1083,  1085,  1087,  1089,  1091,
    1093,  1095,  1097,  1099,  1101,  1103,  1105,  1107,  1109,  1111,
    1113,  1115,  1120,  1121,  1122,  1126,  1127,  1131,  1132,  1136,
    1137,  1146,  1154,  1170,  1178,  1186,  1194,  1202,  1206,  1210,
    1219,  1231,  1236,  1240,  1256,  1264,  1279,  1288,  1297,  1314,
    1321,  1331,  1335,  1339,  1343,  1347,  1351,  1355,  1356,  1360,
    1364
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


#line 3 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:1167
} // CatDB
#line 3169 "/root/CatDB/parser/lex_yacc/sql_parser.cpp" // lalr1.cc:1167
#line 1366 "/root/CatDB/parser/lex_yacc/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
