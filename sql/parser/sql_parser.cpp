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
#line 33 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:397

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
	#include "object.h"
	
	/*注意：这里的参数由%parse-param决定*/
	static CatDB::SqlParser::symbol_type yylex(CatDB::SqlScanner& scanner,CatDB::SqlDriver &driver)
	{
		return scanner.get_next_token();
	}
	using namespace CatDB;
	using namespace CatDB::Parser;
	using namespace CatDB::Common;

#line 58 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:397


// First part of user declarations.
#line 57 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:404

	
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

#line 154 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sql_parser.hpp"

// User implementation prologue.

#line 168 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:412


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
#line 254 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:479

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
      case 161: // relation_factor
        value.move< BasicTableStmt_s > (that.value);
        break;

      case 149: // column_definition
        value.move< ColumnDefineStmt_s > (that.value);
        break;

      case 124: // projection
      case 129: // expr
      case 130: // in_expr
      case 131: // query_ref_expr
      case 132: // arith_expr
      case 133: // simple_expr
      case 134: // column_ref
      case 135: // expr_const
      case 136: // func_expr
      case 140: // insert_value
      case 143: // update_asgn_factor
        value.move< ExprStmt_s > (that.value);
        break;

      case 122: // opt_select_limit
        value.move< LimitStmt_s > (that.value);
        break;

      case 120: // order_by
        value.move< OrderStmt_s > (that.value);
        break;

      case 104: // sql_stmt
      case 105: // stmt
      case 106: // cmd_stmt
      case 107: // select_stmt
      case 108: // set_select
      case 109: // sub_set_select
      case 110: // select_with_parens
      case 111: // simple_select
      case 138: // insert_stmt
      case 141: // update_stmt
      case 144: // delete_stmt
      case 145: // explain_stmt
      case 146: // explainable_stmt
      case 147: // create_stmt
      case 155: // drop_stmt
      case 156: // show_stmt
      case 158: // use_stmt
      case 159: // desc_stmt
      case 160: // analyze_stmt
        value.move< Stmt_s > (that.value);
        break;

      case 125: // table_factor
      case 127: // sub_table_factor
        value.move< TableStmt_s > (that.value);
        break;

      case 148: // table_element_list
        value.move< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 113: // select_expr_list
      case 115: // opt_where
      case 116: // opt_groupby
      case 117: // opt_having
      case 128: // expr_list
      case 139: // insert_value_list
      case 142: // update_asgn_list
        value.move< Vector<ExprStmt_s> > (that.value);
        break;

      case 118: // opt_order_by
      case 119: // order_by_list
        value.move< Vector<OrderStmt_s> > (that.value);
        break;

      case 114: // from_list
        value.move< Vector<TableStmt_s> > (that.value);
        break;

      case 112: // opt_distinct
      case 121: // opt_asc_desc
      case 137: // distinct_or_all
        value.move< bool > (that.value);
        break;

      case 123: // limit_expr
      case 150: // data_type
        value.move< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 126: // opt_alias
      case 157: // op_from_database
      case 162: // database_name
      case 163: // relation_name
      case 164: // column_name
      case 165: // function_name
      case 166: // column_label
      case 167: // ident
      case 168: // datetime
      case 169: // string
      case 170: // number
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
      case 161: // relation_factor
        value.copy< BasicTableStmt_s > (that.value);
        break;

      case 149: // column_definition
        value.copy< ColumnDefineStmt_s > (that.value);
        break;

      case 124: // projection
      case 129: // expr
      case 130: // in_expr
      case 131: // query_ref_expr
      case 132: // arith_expr
      case 133: // simple_expr
      case 134: // column_ref
      case 135: // expr_const
      case 136: // func_expr
      case 140: // insert_value
      case 143: // update_asgn_factor
        value.copy< ExprStmt_s > (that.value);
        break;

      case 122: // opt_select_limit
        value.copy< LimitStmt_s > (that.value);
        break;

      case 120: // order_by
        value.copy< OrderStmt_s > (that.value);
        break;

      case 104: // sql_stmt
      case 105: // stmt
      case 106: // cmd_stmt
      case 107: // select_stmt
      case 108: // set_select
      case 109: // sub_set_select
      case 110: // select_with_parens
      case 111: // simple_select
      case 138: // insert_stmt
      case 141: // update_stmt
      case 144: // delete_stmt
      case 145: // explain_stmt
      case 146: // explainable_stmt
      case 147: // create_stmt
      case 155: // drop_stmt
      case 156: // show_stmt
      case 158: // use_stmt
      case 159: // desc_stmt
      case 160: // analyze_stmt
        value.copy< Stmt_s > (that.value);
        break;

      case 125: // table_factor
      case 127: // sub_table_factor
        value.copy< TableStmt_s > (that.value);
        break;

      case 148: // table_element_list
        value.copy< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 113: // select_expr_list
      case 115: // opt_where
      case 116: // opt_groupby
      case 117: // opt_having
      case 128: // expr_list
      case 139: // insert_value_list
      case 142: // update_asgn_list
        value.copy< Vector<ExprStmt_s> > (that.value);
        break;

      case 118: // opt_order_by
      case 119: // order_by_list
        value.copy< Vector<OrderStmt_s> > (that.value);
        break;

      case 114: // from_list
        value.copy< Vector<TableStmt_s> > (that.value);
        break;

      case 112: // opt_distinct
      case 121: // opt_asc_desc
      case 137: // distinct_or_all
        value.copy< bool > (that.value);
        break;

      case 123: // limit_expr
      case 150: // data_type
        value.copy< int > (that.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 126: // opt_alias
      case 157: // op_from_database
      case 162: // database_name
      case 163: // relation_name
      case 164: // column_name
      case 165: // function_name
      case 166: // column_label
      case 167: // ident
      case 168: // datetime
      case 169: // string
      case 170: // number
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
      case 161: // relation_factor
        yylhs.value.build< BasicTableStmt_s > ();
        break;

      case 149: // column_definition
        yylhs.value.build< ColumnDefineStmt_s > ();
        break;

      case 124: // projection
      case 129: // expr
      case 130: // in_expr
      case 131: // query_ref_expr
      case 132: // arith_expr
      case 133: // simple_expr
      case 134: // column_ref
      case 135: // expr_const
      case 136: // func_expr
      case 140: // insert_value
      case 143: // update_asgn_factor
        yylhs.value.build< ExprStmt_s > ();
        break;

      case 122: // opt_select_limit
        yylhs.value.build< LimitStmt_s > ();
        break;

      case 120: // order_by
        yylhs.value.build< OrderStmt_s > ();
        break;

      case 104: // sql_stmt
      case 105: // stmt
      case 106: // cmd_stmt
      case 107: // select_stmt
      case 108: // set_select
      case 109: // sub_set_select
      case 110: // select_with_parens
      case 111: // simple_select
      case 138: // insert_stmt
      case 141: // update_stmt
      case 144: // delete_stmt
      case 145: // explain_stmt
      case 146: // explainable_stmt
      case 147: // create_stmt
      case 155: // drop_stmt
      case 156: // show_stmt
      case 158: // use_stmt
      case 159: // desc_stmt
      case 160: // analyze_stmt
        yylhs.value.build< Stmt_s > ();
        break;

      case 125: // table_factor
      case 127: // sub_table_factor
        yylhs.value.build< TableStmt_s > ();
        break;

      case 148: // table_element_list
        yylhs.value.build< Vector<ColumnDefineStmt_s> > ();
        break;

      case 113: // select_expr_list
      case 115: // opt_where
      case 116: // opt_groupby
      case 117: // opt_having
      case 128: // expr_list
      case 139: // insert_value_list
      case 142: // update_asgn_list
        yylhs.value.build< Vector<ExprStmt_s> > ();
        break;

      case 118: // opt_order_by
      case 119: // order_by_list
        yylhs.value.build< Vector<OrderStmt_s> > ();
        break;

      case 114: // from_list
        yylhs.value.build< Vector<TableStmt_s> > ();
        break;

      case 112: // opt_distinct
      case 121: // opt_asc_desc
      case 137: // distinct_or_all
        yylhs.value.build< bool > ();
        break;

      case 123: // limit_expr
      case 150: // data_type
        yylhs.value.build< int > ();
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 126: // opt_alias
      case 157: // op_from_database
      case 162: // database_name
      case 163: // relation_name
      case 164: // column_name
      case 165: // function_name
      case 166: // column_label
      case 167: // ident
      case 168: // datetime
      case 169: // string
      case 170: // number
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
#line 300 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 960 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 307 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 966 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 308 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 972 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 309 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 978 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 310 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 984 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 311 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 990 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 312 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 996 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 313 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow stmt"); }
#line 1002 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 322 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1008 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 323 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1014 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 324 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1020 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 325 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1026 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 326 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1032 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 327 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1038 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 332 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1046 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 336 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1054 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 343 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1063 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 348 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1072 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 353 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1081 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 358 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1090 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 363 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1099 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 368 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1108 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 373 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1117 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 378 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1126 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 386 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1134 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 390 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1142 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 397 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1150 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 407 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建select stmt
		SelectStmt_s select_stmt = SelectStmt::make_select_stmt();
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
#line 1168 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 423 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1174 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 424 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1180 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 429 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1189 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 434 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yystack_[2].value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
    }
#line 1198 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 442 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = Vector<TableStmt_s>();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1207 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 447 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = yystack_[2].value.as< Vector<TableStmt_s> > ();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1216 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 454 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1222 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 455 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1228 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 459 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1234 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 460 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1240 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 464 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1246 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 465 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1252 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 469 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>(); }
#line 1258 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 470 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[0].value.as< Vector<OrderStmt_s> > (); }
#line 1264 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 475 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = Vector<OrderStmt_s>();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1273 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 480 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<OrderStmt_s> > () = yystack_[2].value.as< Vector<OrderStmt_s> > ();
		yylhs.value.as< Vector<OrderStmt_s> > ().push_back(yystack_[0].value.as< OrderStmt_s > ());
	}
#line 1282 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 488 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OrderStmt_s > () = OrderStmt::make_order_stmt(yystack_[1].value.as< ExprStmt_s > (), yystack_[0].value.as< bool > ());
    }
#line 1290 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 494 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1296 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 495 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1302 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 496 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1308 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 500 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< LimitStmt_s > () = NULL;}
#line 1314 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 502 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
    }
#line 1325 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 509 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
	}
#line 1336 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 519 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 1344 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 526 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< ExprStmt_s > ()->to_string();
    }
#line 1354 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 532 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1364 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 538 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[2].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1374 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 547 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 1382 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 551 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< BasicTableStmt_s > ();
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
    }
#line 1391 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 556 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), LeftOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1399 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 560 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), RightOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1407 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 564 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), FullOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1415 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 568 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), Inner, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1423 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 574 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1429 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 576 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1437 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 580 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1445 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 587 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		yylhs.value.as< TableStmt_s > () = ViewTableStmt::make_view_table(yystack_[2].value.as< Stmt_s > ());
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1455 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 593 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< TableStmt_s > ();
	}
#line 1463 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 605 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1473 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 611 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1483 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 620 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 1491 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 624 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_LE);
	}
#line 1500 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 629 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_LT);
	}
#line 1509 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 634 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EQ);
	}
#line 1518 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 639 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_GE);
	}
#line 1527 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 644 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_GT);
	}
#line 1536 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 649 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NE);
	}
#line 1545 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 654 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_LIKE);
	}
#line 1554 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 659 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_LIKE);
	}
#line 1563 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 664 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_AND);
    }
#line 1572 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 669 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_OR);
    }
#line 1581 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 674 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), OP_IS_NULL);
    }
#line 1590 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 679 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), OP_IS_NOT_NULL);
    }
#line 1599 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 684 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[4].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_BETWEEN);
    }
#line 1608 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 689 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_BETWEEN);
    }
#line 1617 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 694 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_IN);
    }
#line 1626 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 699 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_IN);
    }
#line 1635 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 707 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 1643 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 711 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		ListStmt_s list_stmt = ListStmt::make_list_stmt();
		Vector<ExprStmt_s> &exprs = yystack_[1].value.as< Vector<ExprStmt_s> > ();
		for (u32 i = 0; i < exprs.size(); ++i) {
			list_stmt->push_back(exprs[i]);
		}
		yylhs.value.as< ExprStmt_s > () = list_stmt;
	}
#line 1656 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 723 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< ExprStmt_s > () = query_expr;
    }
#line 1666 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 732 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 1674 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 736 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 1683 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 741 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MINUS);
    }
#line 1692 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 746 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_ADD);
	}
#line 1701 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 751 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_SUB);
	}
#line 1710 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 756 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MUL);
	}
#line 1719 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 761 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DIV);
	}
#line 1728 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 769 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 1736 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 773 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 1744 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 777 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 1752 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 781 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 1760 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 785 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 1768 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 789 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EXISTS);
    }
#line 1776 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 793 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_EXISTS);
    }
#line 1785 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 801 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 1796 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 808 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 1806 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 814 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 1817 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 822 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 1828 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 829 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 1840 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 837 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 1852 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 848 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 1865 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 857 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = DateTime::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 1878 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 866 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 1891 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 875 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Bool::make_object(false);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 1904 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 884 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Bool::make_object(true);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 1917 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 893 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 1930 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 905 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[3].value.as< std::string > ());
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		aggr->set_aggr_expr(col);
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 1943 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 914 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[4].value.as< std::string > ());
		aggr->distinct = yystack_[2].value.as< bool > ();
		aggr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 1955 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 922 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		AggrStmt_s aggr;
		make_aggr_stmt(aggr, yystack_[3].value.as< std::string > ());
		aggr->set_aggr_expr(yystack_[1].value.as< ExprStmt_s > ());
		yylhs.value.as< ExprStmt_s > () = aggr;
    }
#line 1966 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 932 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 1974 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 936 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 1982 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 947 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		insert_stmt->value_list = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 1995 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 956 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[1].value.as< BasicTableStmt_s > ();
		insert_stmt->query_values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 2007 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 966 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[1].value.as< ExprStmt_s > ());
    }
#line 2016 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 971 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2025 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 979 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建值列表
		make_list(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2034 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 984 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		list_push(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2043 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 997 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		UpdateStmt_s update_stmt = UpdateStmt::make_update_stmt();
		check(update_stmt);
		update_stmt->table = yystack_[4].value.as< BasicTableStmt_s > ();
		yystack_[4].value.as< BasicTableStmt_s > ()->set_alias_name(yystack_[3].value.as< std::string > ());
		update_stmt->from_stmts.push_back(yystack_[4].value.as< BasicTableStmt_s > ());
		update_stmt->update_assign_stmt = yystack_[1].value.as< Vector<ExprStmt_s> > ();
		update_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = update_stmt;
    }
#line 2058 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1008 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		UpdateStmt_s update_stmt = UpdateStmt::make_update_stmt();
		check(update_stmt);
		update_stmt->table = yystack_[5].value.as< BasicTableStmt_s > ();
		update_stmt->update_assign_stmt = yystack_[3].value.as< Vector<ExprStmt_s> > ();
		update_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		update_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = update_stmt;
    }
#line 2072 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1021 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2082 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1027 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2092 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1036 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), col, yystack_[0].value.as< ExprStmt_s > (), OP_EQ);
    }
#line 2103 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1051 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		yystack_[2].value.as< BasicTableStmt_s > ()->set_alias_name(yystack_[1].value.as< std::string > ());
		delete_stmt->from_stmts.push_back(yystack_[2].value.as< BasicTableStmt_s > ());
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 2117 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1061 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		yystack_[2].value.as< BasicTableStmt_s > ()->set_alias_name(yystack_[1].value.as< std::string > ()); 
		delete_stmt->from_stmts.push_back(yystack_[2].value.as< BasicTableStmt_s > ());
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 2131 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1071 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		delete_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 2144 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1088 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		DMLStmt_s dml = yylhs.value.as< Stmt_s > ();
		dml->is_explain = true;
    }
#line 2154 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1096 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2160 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1097 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2166 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1098 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2172 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1099 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 2178 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1109 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::CreateTable);
		check(cmd_stmt);
		cmd_stmt->params.create_table_params.table = yystack_[3].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.create_table_params.column_define_list = yystack_[1].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 2190 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1117 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2201 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1127 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 2210 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1132 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 2219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1140 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as< std::string > (), yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
    }
#line 2229 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1149 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2235 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1151 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2241 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1153 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2247 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1155 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2253 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1157 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2259 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1159 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2265 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1161 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2271 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1163 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2277 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1165 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2283 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1167 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2289 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1169 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::NUMBER; }
#line 2295 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1171 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2301 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1173 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2307 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1175 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2313 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1177 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::VARCHAR; }
#line 2319 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1179 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2325 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1181 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2331 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1183 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2337 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1185 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = ColumnDefineStmt::DATETIME; }
#line 2343 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1189 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2349 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1190 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2355 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1191 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2361 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1195 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2367 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1196 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2373 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1200 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2379 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1201 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2385 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1205 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2391 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1206 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { }
#line 2397 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1216 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.table = yystack_[0].value.as< TableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 2408 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1223 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2419 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1238 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
 	}
#line 2430 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1245 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2441 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1252 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2452 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1259 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2463 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1266 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 2471 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1270 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 2479 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1274 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2491 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1282 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2503 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1293 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2511 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1297 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 2519 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1304 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::UseDatabase);
		check(cmd_stmt);
		cmd_stmt->params.use_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2530 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1319 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 2541 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1326 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 2552 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1340 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 2564 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1348 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[2].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = "*";
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 2576 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1356 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CMDStmt::Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 2588 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 1372 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 2600 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 1380 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 2612 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 1388 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 2623 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 1397 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2629 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 1401 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2635 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 1405 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2641 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 1409 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2647 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 1413 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 2653 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 1417 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2659 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 1421 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2665 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 1422 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2671 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 1426 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2677 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 1430 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2683 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;


#line 2687 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -166;

  const short int parser::yytable_ninf_ = -198;

  const short int
  parser::yypact_[] =
  {
     480,    22,   -77,   -21,   149,     0,     0,    53,    -9,   -42,
      -6,   391,     0,     5,    50,    37,  -166,  -166,   228,   256,
    -166,   158,  -166,  -166,  -166,  -166,  -166,  -166,  -166,  -166,
    -166,  -166,    47,   104,     5,     0,    35,  -166,  -166,     0,
      39,   108,  -166,   145,  -166,  -166,     5,   150,  -166,  -166,
    -166,  -166,  -166,     0,  -166,   390,    66,    44,  -166,    71,
     124,  -166,   107,   134,    74,  -166,  -166,  -166,  -166,     8,
      22,    22,    13,    22,    22,  -166,   171,   181,  -166,   187,
       0,   102,   150,     5,  -166,   125,   300,   102,   -20,   159,
    -166,   390,   390,  -166,   364,  -166,  -166,  -166,  -166,   189,
     199,  -166,   189,  -166,    23,  -166,   573,  -166,   217,  -166,
    -166,  -166,  -166,   216,  -166,   227,   169,  -166,  -166,  -166,
       0,     0,   134,     0,     0,     5,  -166,     5,     5,   178,
    -166,  -166,    22,  -166,  -166,  -166,  -166,    22,  -166,  -166,
    -166,   250,   117,     5,   102,   190,   -15,   300,  -166,   125,
     253,   174,  -166,   222,   244,   252,   258,  -166,   302,  -166,
     199,  -166,  -166,   298,   608,  -166,  -166,  -166,   390,   150,
     390,   390,   268,   390,   390,   390,   390,   390,   390,   390,
     390,   309,    14,     5,  -166,  -166,   390,   390,   390,   390,
      -5,   315,  -166,  -166,  -166,  -166,  -166,  -166,  -166,   -39,
    -166,   334,  -166,     5,  -166,  -166,  -166,  -166,  -166,   136,
    -166,   404,   190,   390,  -166,   150,  -166,   325,   102,  -166,
     150,   150,   150,   150,   390,   331,  -166,  -166,  -166,   -15,
     686,   538,   390,   390,   309,   276,   276,   276,   276,   276,
     276,   340,   248,   364,  -166,  -166,   355,  -166,  -166,   198,
     198,  -166,  -166,  -166,  -166,  -166,   349,  -166,  -166,   630,
     390,   150,     5,   390,   -30,  -166,     5,  -166,  -166,   354,
     358,  -166,   363,  -166,  -166,  -166,  -166,   354,   354,  -166,
     363,   363,   363,   366,  -166,   366,  -166,  -166,   674,   300,
     247,  -166,   280,   369,   383,   407,   674,   351,   368,   346,
     340,   275,  -166,   390,   374,   674,  -166,  -166,  -166,   652,
     -15,  -166,   674,  -166,  -166,   375,  -166,   375,  -166,   375,
    -166,  -166,  -166,  -166,  -166,  -166,   375,  -166,  -166,   390,
     390,   390,   390,  -166,   390,   390,   377,   356,   390,   217,
    -166,   390,  -166,  -166,   376,   384,   400,   401,   674,   674,
     674,   674,   674,   378,   390,   390,   345,   217,   674,  -166,
     375,  -166,  -166,  -166,  -166,   402,   674,   389,   362,   410,
     390,   375,  -166,  -166,   417,  -166,   560,   420,  -166,   390,
    -166,  -166,  -166,   375,  -166,  -166
  };

  const unsigned char
  parser::yydefact_[] =
  {
       9,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,     0,     0,     0,     0,     3,     4,    17,     0,
      27,    26,     5,     6,     7,     8,    11,    12,    10,    14,
      13,    15,     0,     0,     0,     0,     0,   199,   193,     0,
       0,     0,   191,   195,   187,   186,     0,     0,   136,   138,
     139,   137,   135,     0,    31,     0,     0,     0,   175,     0,
       0,   180,     0,   184,     0,   185,   194,     1,     2,     0,
       0,     0,     0,     0,     0,    28,     0,     0,   141,     0,
       0,    63,     0,     0,   174,     0,   173,    63,     0,     0,
     115,     0,     0,   105,     0,   202,   203,   113,   114,     0,
       0,   108,     0,    89,     0,    32,    54,   101,    70,    90,
      97,    98,   100,     0,   104,     0,   196,   111,   110,   112,
       0,     0,   184,     0,     0,     0,   177,     0,     0,     0,
      64,   195,     0,    19,    26,    25,    23,     0,    18,    24,
      22,     0,     0,     0,    63,    37,    37,    34,   192,     0,
       0,     0,    57,     0,     0,     0,     0,    58,     0,   122,
       0,    91,    92,    89,     0,   200,   102,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,   198,     0,     0,     0,     0,
       0,     0,   182,   178,   176,   179,   181,   183,    65,     0,
     129,     0,   196,     0,    21,    20,   190,   189,   188,     0,
     142,     0,    37,     0,   132,     0,   134,     0,    63,    67,
       0,     0,     0,     0,     0,   121,   103,    99,    33,    37,
      80,    79,     0,     0,     0,    71,    72,    73,    75,    74,
      76,    77,     0,     0,    85,    87,     0,    81,    56,    93,
      94,    95,    96,   107,   109,   106,   105,   119,   120,     0,
       0,     0,     0,     0,    37,   140,     0,   152,   161,   166,
     168,   148,   172,   145,   146,   147,   149,   166,   166,   154,
     172,   172,   172,   170,   162,   170,   144,   133,    36,    35,
      63,    66,     0,     0,     0,     0,   125,     0,     0,    39,
      78,     0,    86,     0,     0,    68,    82,   116,   118,     0,
      37,   130,   131,   127,   143,     0,   155,     0,   153,     0,
     158,   150,   151,   156,   157,   159,     0,   160,   163,     0,
       0,     0,     0,   123,     0,     0,     0,    41,     0,    83,
      88,     0,   117,   128,     0,     0,     0,     0,    61,    62,
      59,    60,   126,     0,     0,     0,    42,    84,    69,   165,
       0,   167,   171,   169,   124,    38,    40,     0,    50,     0,
       0,     0,    29,   164,    43,    44,    47,    52,    53,     0,
      48,    49,    46,     0,    45,    51
  };

  const short int
  parser::yypgoto_[] =
  {
    -166,  -166,  -166,  -166,     7,  -166,    98,   236,   245,  -166,
    -166,  -165,  -132,  -166,  -166,  -166,  -166,    70,  -166,  -166,
      72,   285,   -31,   -43,  -166,   105,   -66,   226,   -38,   -27,
    -166,  -166,  -166,  -166,  -166,   453,  -166,   128,   457,   265,
     207,   467,  -166,  -166,  -166,  -166,   211,  -166,    65,  -166,
     193,    51,  -166,  -166,   359,  -166,  -166,  -166,   242,   -10,
       6,   -59,  -166,   301,    -4,  -166,   110,    10
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    14,    15,    16,    32,    18,    19,   103,    21,    55,
     104,   146,   214,   337,   356,   368,   374,   375,   382,   372,
     377,   105,   147,   291,   152,   304,   106,   244,   107,   108,
     109,   110,   111,   112,   260,    22,   225,   297,    23,   199,
     200,    24,    25,    52,    26,   209,   210,   286,   316,   318,
     327,   320,    27,    28,   126,    29,    30,    31,    87,    41,
     113,   114,   115,   184,   116,   117,   118,   119
  };

  const short int
  parser::yytable_[] =
  {
      43,    43,    43,    65,   229,    33,     1,    17,    43,    66,
      42,    42,    42,   215,   216,    48,    86,     1,    42,   253,
     261,   129,   246,    77,    78,    53,    34,    37,   164,    66,
      66,    43,    37,   247,     1,    43,    84,    37,   145,     1,
       4,    42,    66,    43,   157,    42,    54,   132,     1,    43,
      67,   168,   137,    42,   151,    38,     9,    10,   213,    42,
     131,    35,   166,   262,   161,   162,   158,    68,    10,   201,
     130,   254,   262,   213,    75,    12,    43,   131,    43,   131,
     287,    43,   169,   131,   211,    10,    42,   130,    42,   148,
      10,    42,   150,   130,    80,   159,   310,   299,    82,    10,
      46,   212,   185,   121,   230,   231,    37,   235,   236,   237,
     238,   239,   240,   241,   127,   197,    43,    43,   151,    43,
      43,    66,   226,   131,   202,   259,    42,    42,    76,    42,
      42,   255,   313,   198,    37,    47,    37,    83,   131,   202,
     131,   207,   127,   245,   201,    43,   120,   288,   208,    37,
     130,   149,   128,   242,   122,    42,   217,    37,   296,   249,
     250,   251,   252,   265,   266,    43,   300,   133,   135,   136,
     138,   139,   140,    36,  -194,    42,    85,   305,   343,   185,
      38,    37,    37,   123,   289,   -16,   202,   124,   -16,   292,
     293,   294,   295,   125,   309,  -197,   245,   312,  -195,   202,
     141,   219,    10,   201,    38,    38,   301,   211,    39,   165,
     142,    43,   167,   143,   131,   160,    43,    43,    43,    43,
      95,    42,   188,   189,   130,     1,    42,    42,    42,    42,
     204,    69,    70,    71,   153,   205,    20,    20,   154,   186,
     187,   188,   189,   155,    20,   190,    40,    44,    45,   156,
     -28,   -28,   -28,   191,    64,   303,   203,    43,   202,    72,
      73,    74,   202,   348,   349,   350,   351,    42,   352,   296,
     186,   187,   188,   189,   206,   358,   339,    79,   213,    37,
     218,    81,   338,   232,   233,   234,   131,   127,   305,   366,
     220,   179,   180,   181,   182,    88,   130,   186,   187,   188,
     189,   -27,   -27,   -27,   376,    20,    20,    20,    20,    20,
      20,   357,   221,   376,   134,   134,   134,   134,   134,   134,
     222,    20,   144,    89,    20,   344,   223,   345,   224,   346,
     163,   323,   324,   325,    90,   243,   347,    91,    92,   256,
     153,    94,   321,   322,   154,   263,    95,    37,    96,   155,
      97,    98,   290,   329,   257,   156,   180,   181,   182,   298,
     153,    99,   192,   193,   154,   195,   196,   258,    20,   155,
     369,   100,    89,    20,   306,   156,   307,   134,   333,   334,
     315,   378,   134,    90,   317,    20,    91,    92,    93,   319,
      94,   101,   326,   378,   335,    95,    37,    96,    89,    97,
      98,   340,   341,   359,   360,   364,   334,   336,    96,    90,
      99,   361,    91,    92,    93,   102,    94,   367,   355,   354,
     100,    95,    37,    96,   267,    97,    98,   362,   363,   153,
     341,   370,   371,   154,    56,    57,    99,   373,   155,    58,
     101,    10,   330,   153,   156,   379,   100,   154,   383,   384,
     268,    59,   155,   228,    60,   385,   331,   269,   156,   365,
     302,    49,   270,   353,   102,    50,   101,   153,   264,   311,
     271,   154,    61,    62,    63,    51,   155,   314,   328,   163,
     332,   194,   156,     0,   248,     0,     0,     0,     0,     0,
     102,   272,     0,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,     1,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     0,     0,
       0,     0,     0,     0,     0,     3,     0,     0,     0,     4,
       5,     6,     0,     0,     7,     0,     0,     8,     0,     0,
       0,     0,     0,     0,     0,     9,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,    10,     0,    11,
       0,     0,     0,     0,    12,    13,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,    37,     0,     0,     0,     0,
     381,     0,     0,   183,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,     0,     0,
       0,     0,     0,     0,     0,   227,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,     0,     0,     0,     0,     0,     0,     0,   342,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182
  };

  const short int
  parser::yycheck_[] =
  {
       4,     5,     6,    13,   169,    82,    26,     0,    12,    13,
       4,     5,     6,    28,   146,     8,    47,    26,    12,    24,
      59,    64,     8,    33,    34,    67,    47,    32,    94,    33,
      34,    35,    32,    19,    26,    39,    46,    32,    81,    26,
      49,    35,    46,    47,    87,    39,    52,    39,    26,    53,
       0,    28,    39,    47,    85,    55,    65,    77,    88,    53,
      64,    82,   100,   102,    91,    92,    86,    30,    77,   128,
      64,    76,   102,    88,    27,    84,    80,    81,    82,    83,
     212,    85,    59,    87,   143,    77,    80,    81,    82,    83,
      77,    85,    85,    87,    59,    88,   261,   229,    59,    77,
      47,   144,   106,    59,   170,   171,    32,   173,   174,   175,
     176,   177,   178,   179,    40,   125,   120,   121,   149,   123,
     124,   125,   160,   127,   128,   191,   120,   121,    24,   123,
     124,   190,   264,   127,    32,    82,    32,    29,   142,   143,
     144,    24,    40,   181,   203,   149,    80,   213,   142,    32,
     144,    26,    78,   180,    83,   149,   149,    32,   224,   186,
     187,   188,   189,    27,    28,   169,   232,    69,    70,    71,
      72,    73,    74,    24,    29,   169,    26,   243,   310,   183,
      55,    32,    32,    59,   215,    27,   190,    80,    30,   220,
     221,   222,   223,    59,   260,    26,   234,   263,    29,   203,
      29,    27,    77,   262,    55,    55,   233,   266,    59,    99,
      29,   215,   102,    26,   218,    56,   220,   221,   222,   223,
      31,   215,    24,    25,   218,    26,   220,   221,   222,   223,
     132,     3,     4,     5,    60,   137,     0,     1,    64,    22,
      23,    24,    25,    69,     8,    29,     4,     5,     6,    75,
       3,     4,     5,    26,    12,     7,    78,   261,   262,     3,
       4,     5,   266,   329,   330,   331,   332,   261,   334,   335,
      22,    23,    24,    25,    24,   341,   303,    35,    88,    32,
      27,    39,     7,    15,    16,    17,   290,    40,   354,   355,
      68,    15,    16,    17,    18,    53,   290,    22,    23,    24,
      25,     3,     4,     5,   370,    69,    70,    71,    72,    73,
      74,   338,    68,   379,    69,    70,    71,    72,    73,    74,
      68,    85,    80,     8,    88,   315,    68,   317,    26,   319,
      94,   280,   281,   282,    19,    26,   326,    22,    23,    24,
      60,    26,   277,   278,    64,    11,    31,    32,    33,    69,
      35,    36,    27,    73,    39,    75,    16,    17,    18,    28,
      60,    46,   120,   121,    64,   123,   124,    52,   132,    69,
     360,    56,     8,   137,    19,    75,    27,   132,    27,    28,
      26,   371,   137,    19,    26,   149,    22,    23,    24,    26,
      26,    76,    26,   383,    26,    31,    32,    33,     8,    35,
      36,    27,    28,    27,    28,    27,    28,    61,    33,    19,
      46,    27,    22,    23,    24,   100,    26,    72,    62,    42,
      56,    31,    32,    33,    20,    35,    36,    27,    27,    60,
      28,    42,    70,    64,    43,    44,    46,    27,    69,    48,
      76,    77,    73,    60,    75,    28,    56,    64,    28,   379,
      46,    60,    69,   168,    63,   383,    73,    53,    75,   354,
     234,     8,    58,   335,   100,     8,    76,    60,   203,   262,
      66,    64,    81,    82,    83,     8,    69,   266,   285,   243,
      73,   122,    75,    -1,   183,    -1,    -1,    -1,    -1,    -1,
     100,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      50,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    40,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    26,    37,    45,    49,    50,    51,    54,    57,    65,
      77,    79,    84,    85,   104,   105,   106,   107,   108,   109,
     110,   111,   138,   141,   144,   145,   147,   155,   156,   158,
     159,   160,   107,    82,    47,    82,    24,    32,    55,    59,
     161,   162,   163,   167,   161,   161,    47,    82,   107,   138,
     141,   144,   146,    67,    52,   112,    43,    44,    48,    60,
      63,    81,    82,    83,   161,   162,   167,     0,    30,     3,
       4,     5,     3,     4,     5,    27,    24,   162,   162,   161,
      59,   161,    59,    29,   162,    26,   125,   161,   161,     8,
      19,    22,    23,    24,    26,    31,    33,    35,    36,    46,
      56,    76,   100,   110,   113,   124,   129,   131,   132,   133,
     134,   135,   136,   163,   164,   165,   167,   168,   169,   170,
      80,    59,    83,    59,    80,    59,   157,    40,    78,   126,
     163,   167,    39,   109,   111,   109,   109,    39,   109,   109,
     109,    29,    29,    26,   161,   126,   114,   125,   163,    26,
     107,   125,   127,    60,    64,    69,    75,   126,    86,   107,
      56,   132,   132,   110,   129,   169,   131,   169,    28,    59,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    40,   166,   167,    22,    23,    24,    25,
      29,    26,   161,   161,   157,   161,   161,   162,   163,   142,
     143,   164,   167,    78,   109,   109,    24,    24,   163,   148,
     149,   164,   126,    88,   115,    28,   115,   107,    27,    27,
      68,    68,    68,    68,    26,   139,   131,    27,   124,   114,
     129,   129,    15,    16,    17,   129,   129,   129,   129,   129,
     129,   129,   132,    26,   130,   131,     8,    19,   166,   132,
     132,   132,   132,    24,    76,   164,    24,    39,    52,   129,
     137,    59,   102,    11,   142,    27,    28,    20,    46,    53,
      58,    66,    87,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   150,   115,   129,   125,
      27,   126,   125,   125,   125,   125,   129,   140,    28,   115,
     129,   132,   130,     7,   128,   129,    19,    27,    27,   129,
     114,   143,   129,   115,   149,    26,   151,    26,   152,    26,
     154,   151,   151,   154,   154,   154,    26,   153,   153,    73,
      73,    73,    73,    27,    28,    26,    61,   116,     7,   132,
      27,    28,    27,   115,   170,   170,   170,   170,   129,   129,
     129,   129,   129,   140,    42,    62,   117,   132,   129,    27,
      28,    27,    27,    27,    27,   128,   129,    72,   118,   170,
      42,    70,   122,    27,   119,   120,   129,   123,   170,    28,
      41,    50,   121,    28,   120,   123
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   103,   104,   105,   105,   105,   105,   105,   105,   105,
     106,   106,   106,   106,   106,   106,   107,   107,   108,   108,
     108,   108,   108,   108,   108,   108,   109,   109,   110,   111,
     112,   112,   113,   113,   114,   114,   115,   115,   116,   116,
     117,   117,   118,   118,   119,   119,   120,   121,   121,   121,
     122,   122,   122,   123,   124,   124,   124,   125,   125,   125,
     125,   125,   125,   126,   126,   126,   127,   127,   128,   128,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   130,   130,   131,
     132,   132,   132,   132,   132,   132,   132,   133,   133,   133,
     133,   133,   133,   133,   134,   134,   134,   134,   134,   134,
     135,   135,   135,   135,   135,   135,   136,   136,   136,   137,
     137,   138,   138,   139,   139,   140,   140,   141,   141,   142,
     142,   143,   144,   144,   144,   145,   146,   146,   146,   146,
     147,   147,   148,   148,   149,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   151,   151,   151,   152,   152,   153,
     153,   154,   154,   155,   155,   156,   156,   156,   156,   156,
     156,   156,   156,   157,   157,   158,   159,   159,   160,   160,
     160,   161,   161,   161,   162,   163,   164,   165,   166,   167,
     168,   168,   169,   170
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       4,     4,     3,     3,     3,     3,     1,     1,     3,    10,
       0,     1,     1,     3,     1,     3,     2,     0,     3,     0,
       2,     0,     0,     3,     1,     3,     2,     0,     1,     1,
       0,     4,     2,     1,     1,     2,     3,     2,     2,     6,
       6,     6,     6,     0,     1,     2,     3,     2,     1,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     4,     3,
       3,     3,     4,     5,     6,     3,     4,     1,     3,     1,
       1,     2,     2,     3,     3,     3,     3,     1,     1,     3,
       1,     1,     2,     3,     1,     1,     3,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     4,     5,     4,     1,
       1,     5,     4,     3,     5,     1,     3,     6,     7,     1,
       3,     3,     5,     6,     5,     2,     1,     1,     1,     1,
       6,     3,     1,     3,     2,     1,     1,     1,     1,     1,
       2,     2,     1,     2,     1,     2,     2,     2,     2,     2,
       2,     1,     1,     2,     5,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     3,     2,     4,     3,     4,     4,
       2,     4,     4,     2,     0,     2,     2,     2,     5,     5,
       5,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1
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
  "FROM", "FULL", "GROUP", "HAVING", "INDEX", "INNER", "INSERT", "INT",
  "INTO", "JOIN", "LEFT", "LIMIT", "NUMBER", "ORDER", "ON", "PARALLEL",
  "RIGHT", "ROWID", "SELECT", "SET", "SHOW", "STATIS", "STATUS", "TABLE",
  "TABLES", "UPDATE", "USING", "VALUES", "VARCHAR", "WHERE", "TINYINT",
  "SMALLINT", "MEDIUMINT", "BIGINT", "DECIMAL", "NUMERIC_SYM", "REAL",
  "CHAR", "BINARY", "VARBINARY", "TIMESTAMP_SYM", "DATE", "TIME", "','",
  "$accept", "sql_stmt", "stmt", "cmd_stmt", "select_stmt", "set_select",
  "sub_set_select", "select_with_parens", "simple_select", "opt_distinct",
  "select_expr_list", "from_list", "opt_where", "opt_groupby",
  "opt_having", "opt_order_by", "order_by_list", "order_by",
  "opt_asc_desc", "opt_select_limit", "limit_expr", "projection",
  "table_factor", "opt_alias", "sub_table_factor", "expr_list", "expr",
  "in_expr", "query_ref_expr", "arith_expr", "simple_expr", "column_ref",
  "expr_const", "func_expr", "distinct_or_all", "insert_stmt",
  "insert_value_list", "insert_value", "update_stmt", "update_asgn_list",
  "update_asgn_factor", "delete_stmt", "explain_stmt", "explainable_stmt",
  "create_stmt", "table_element_list", "column_definition", "data_type",
  "opt_decimal", "opt_float", "opt_time_precision", "opt_char_length",
  "drop_stmt", "show_stmt", "op_from_database", "use_stmt", "desc_stmt",
  "analyze_stmt", "relation_factor", "database_name", "relation_name",
  "column_name", "function_name", "column_label", "ident", "datetime",
  "string", "number", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   299,   299,   307,   308,   309,   310,   311,   312,   313,
     322,   323,   324,   325,   326,   327,   331,   335,   342,   347,
     352,   357,   362,   367,   372,   377,   385,   389,   396,   403,
     423,   424,   428,   433,   441,   446,   454,   455,   459,   460,
     464,   465,   469,   470,   474,   479,   487,   494,   495,   496,
     500,   501,   508,   518,   525,   531,   537,   546,   550,   555,
     559,   563,   567,   574,   575,   579,   586,   592,   604,   610,
     619,   623,   628,   633,   638,   643,   648,   653,   658,   663,
     668,   673,   678,   683,   688,   693,   698,   706,   710,   722,
     731,   735,   740,   745,   750,   755,   760,   768,   772,   776,
     780,   784,   788,   792,   800,   807,   813,   821,   828,   836,
     847,   856,   865,   874,   883,   892,   904,   913,   921,   931,
     935,   946,   955,   965,   970,   978,   983,   996,  1007,  1020,
    1026,  1035,  1050,  1060,  1070,  1087,  1096,  1097,  1098,  1099,
    1108,  1116,  1126,  1131,  1139,  1148,  1150,  1152,  1154,  1156,
    1158,  1160,  1162,  1164,  1166,  1168,  1170,  1172,  1174,  1176,
    1178,  1180,  1182,  1184,  1189,  1190,  1191,  1195,  1196,  1200,
    1201,  1205,  1206,  1215,  1222,  1237,  1244,  1251,  1258,  1265,
    1269,  1273,  1281,  1292,  1297,  1303,  1318,  1325,  1339,  1347,
    1355,  1371,  1379,  1387,  1397,  1401,  1405,  1409,  1413,  1417,
    1421,  1422,  1426,  1430
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
#line 3404 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:1167
#line 1433 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
