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
#line 58 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:397

	#include "sql_parser.hpp"
	#include "sql_scanner.h"
	#include "sql_driver.h"
	#include "location.hh"

	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
    #include "table_stmt.h"
	#include "expr_stmt.h"
	#include "dml_stmt.h"
	#include "cmd_stmt.h"
	#include "object.h"
	
	/*注意：这里的参数由%parse-param决定*/
	static CatDB::SqlParser::symbol_type yylex(CatDB::SqlScanner& scanner,
											   CatDB::SqlDriver &driver)
	{
		return scanner.get_next_token();
	}
	using namespace CatDB;
	using namespace CatDB::Parser;
	using namespace CatDB::Common;

#line 61 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:397


// First part of user declarations.
#line 85 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:404

	
#define yyerror(fmt, ...) 				\
{ 										\
	char tmp[255]; 						\
	sprintf(tmp, fmt, ##__VA_ARGS__); 	\
	driver.set_sys_error(tmp); 			\
}
 
#define check(stmt) 				\
{ 									\
	if(!stmt) 						\
	{ 								\
		yyerror("make stmt error"); \
		YYABORT; 					\
	} 								\
}

#define make_unary_stmt(stmt, stmt1, op) 		\
{ 												\
	stmt = OpExprStmt::make_op_expr_stmt(op); 	\
	check(stmt); 								\
	stmt->params.push_back(stmt1); 				\
}

#define make_binary_stmt(stmt, stmt1, stmt2, op) 	\
{					 								\
	stmt = OpExprStmt::make_op_expr_stmt(op); 		\
	check(stmt); 									\
	stmt->params.push_back(stmt1); 					\
	stmt->params.push_back(stmt2); 					\
}

#define make_ternary_stmt(stmt, stmt1, stmt2, stmt3, op) 	\
{ 															\
	stmt = OpExprStmt::make_op_expr_stmt(op); 				\
	check(stmt); 											\
	stmt->params.push_back(stmt1); 							\
	stmt->params.push_back(stmt2); 							\
	stmt->params.push_back(stmt3); 							\
}
	
#define str_to_lower(str) 					\
{											\
	for(u32 i = 0; i<str.size(); ++i){		\
		if(str[i] >= 'A' && str[i] <= 'Z'){	\
			str[i] -= 'A';					\
			str[i] += 'a';					\
		}									\
	}										\
}


#line 119 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sql_parser.hpp"

// User implementation prologue.

#line 133 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:412


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

#line 16 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:479
namespace CatDB {
#line 219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:479

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
      case 267: // relation_factor
        value.move< BasicTableStmt_s > (that.value);
        break;

      case 236: // column_definition
      case 259: // param
        value.move< ColumnDefineStmt_s > (that.value);
        break;

      case 238: // data_type
        value.move< DataType > (that.value);
        break;

      case 191: // order_by
      case 195: // projection
      case 203: // simple_expr
      case 204: // seconds_expr
      case 205: // arith_expr
      case 207: // cmp_expr
      case 210: // query_ref_expr
      case 211: // case_when_expr
      case 214: // logical_expr
      case 215: // column_ref
      case 216: // expr_const
      case 217: // func_expr
      case 220: // opt_arith_expr
      case 227: // update_asgn_factor
        value.move< ExprStmt_s > (that.value);
        break;

      case 253: // pro_or_func
      case 254: // procedure_define
      case 255: // function_define
        value.move< FunctionDefinition_s > (that.value);
        break;

      case 173: // opt_hint
        value.move< Hint > (that.value);
        break;

      case 176: // single_hint
        value.move< HintStmt_s > (that.value);
        break;

      case 181: // leading_hint_table
        value.move< LeadingTable_s > (that.value);
        break;

      case 193: // opt_select_limit
        value.move< LimitStmt_s > (that.value);
        break;

      case 208: // cmp_type
      case 209: // sq_cmp_type
        value.move< OperationType > (that.value);
        break;

      case 165: // sql_stmt
      case 166: // stmt
      case 167: // cmd_stmt
      case 168: // select_stmt
      case 169: // set_select
      case 170: // sub_set_select
      case 171: // select_with_parens
      case 172: // simple_select
      case 222: // insert_stmt
      case 225: // update_stmt
      case 228: // delete_stmt
      case 229: // explain_stmt
      case 230: // explainable_stmt
      case 231: // create_stmt
      case 242: // drop_stmt
      case 244: // show_stmt
      case 246: // use_stmt
      case 247: // desc_stmt
      case 248: // analyze_stmt
      case 250: // kill_stmt
      case 251: // create_package_stmt
      case 265: // exec_package_stmt
        value.move< Stmt_s > (that.value);
        break;

      case 196: // basic_table_factor
      case 197: // view_table_factor
      case 198: // joined_table_factor
      case 200: // sub_table_factor
      case 201: // table_factor
        value.move< TableStmt_s > (that.value);
        break;

      case 235: // table_element_list
      case 256: // table_type
      case 257: // opt_param_list
      case 258: // param_list
        value.move< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 184: // select_expr_list
      case 186: // opt_where
      case 187: // opt_groupby
      case 188: // opt_having
      case 189: // opt_order_by
      case 190: // order_by_list
      case 206: // arith_expr_list
      case 212: // when_then_list1
      case 213: // when_then_list2
      case 221: // opt_partition_by
      case 224: // insert_value
      case 226: // update_asgn_list
      case 266: // opt_arith_expr_list
        value.move< Vector<ExprStmt_s> > (that.value);
        break;

      case 252: // pro_or_func_list
        value.move< Vector<FunctionDefinition_s> > (that.value);
        break;

      case 174: // opt_hint_list
      case 175: // hint_list
        value.move< Vector<HintStmt_s> > (that.value);
        break;

      case 182: // leading_hint_table_list
        value.move< Vector<LeadingTable_s> > (that.value);
        break;

      case 179: // hint_table_list
      case 232: // opt_view_column_define
      case 233: // view_column_define
      case 241: // opt_engine_def
        value.move< Vector<String> > (that.value);
        break;

      case 185: // from_list
        value.move< Vector<TableStmt_s> > (that.value);
        break;

      case 223: // insert_value_list
        value.move< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 219: // win_type
        value.move< WinType > (that.value);
        break;

      case 180: // opt_split
      case 183: // opt_distinct
      case 192: // opt_asc_desc
      case 199: // opt_outer
      case 218: // distinct_or_all
      case 237: // opt_not_null
      case 243: // opt_if_exists
      case 260: // opt_replace
        value.move< bool > (that.value);
        break;

      case 249: // opt_sample_size
        value.move< double > (that.value);
        break;

      case 194: // limit_expr
      case 239: // opt_time_precision
      case 240: // opt_char_length
      case 276: // int_value
        value.move< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 177: // opt_qb_name
      case 178: // opt_qb_name_single
      case 202: // opt_alias
      case 234: // beg_view_define
      case 245: // op_from_database
      case 261: // package_name
      case 262: // procedure_name
      case 263: // function_name
      case 264: // param_name
      case 268: // database_name
      case 269: // relation_name
      case 270: // column_name
      case 271: // column_label
      case 272: // ident
      case 273: // datetime
      case 274: // string
      case 275: // number
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
      case 267: // relation_factor
        value.copy< BasicTableStmt_s > (that.value);
        break;

      case 236: // column_definition
      case 259: // param
        value.copy< ColumnDefineStmt_s > (that.value);
        break;

      case 238: // data_type
        value.copy< DataType > (that.value);
        break;

      case 191: // order_by
      case 195: // projection
      case 203: // simple_expr
      case 204: // seconds_expr
      case 205: // arith_expr
      case 207: // cmp_expr
      case 210: // query_ref_expr
      case 211: // case_when_expr
      case 214: // logical_expr
      case 215: // column_ref
      case 216: // expr_const
      case 217: // func_expr
      case 220: // opt_arith_expr
      case 227: // update_asgn_factor
        value.copy< ExprStmt_s > (that.value);
        break;

      case 253: // pro_or_func
      case 254: // procedure_define
      case 255: // function_define
        value.copy< FunctionDefinition_s > (that.value);
        break;

      case 173: // opt_hint
        value.copy< Hint > (that.value);
        break;

      case 176: // single_hint
        value.copy< HintStmt_s > (that.value);
        break;

      case 181: // leading_hint_table
        value.copy< LeadingTable_s > (that.value);
        break;

      case 193: // opt_select_limit
        value.copy< LimitStmt_s > (that.value);
        break;

      case 208: // cmp_type
      case 209: // sq_cmp_type
        value.copy< OperationType > (that.value);
        break;

      case 165: // sql_stmt
      case 166: // stmt
      case 167: // cmd_stmt
      case 168: // select_stmt
      case 169: // set_select
      case 170: // sub_set_select
      case 171: // select_with_parens
      case 172: // simple_select
      case 222: // insert_stmt
      case 225: // update_stmt
      case 228: // delete_stmt
      case 229: // explain_stmt
      case 230: // explainable_stmt
      case 231: // create_stmt
      case 242: // drop_stmt
      case 244: // show_stmt
      case 246: // use_stmt
      case 247: // desc_stmt
      case 248: // analyze_stmt
      case 250: // kill_stmt
      case 251: // create_package_stmt
      case 265: // exec_package_stmt
        value.copy< Stmt_s > (that.value);
        break;

      case 196: // basic_table_factor
      case 197: // view_table_factor
      case 198: // joined_table_factor
      case 200: // sub_table_factor
      case 201: // table_factor
        value.copy< TableStmt_s > (that.value);
        break;

      case 235: // table_element_list
      case 256: // table_type
      case 257: // opt_param_list
      case 258: // param_list
        value.copy< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 184: // select_expr_list
      case 186: // opt_where
      case 187: // opt_groupby
      case 188: // opt_having
      case 189: // opt_order_by
      case 190: // order_by_list
      case 206: // arith_expr_list
      case 212: // when_then_list1
      case 213: // when_then_list2
      case 221: // opt_partition_by
      case 224: // insert_value
      case 226: // update_asgn_list
      case 266: // opt_arith_expr_list
        value.copy< Vector<ExprStmt_s> > (that.value);
        break;

      case 252: // pro_or_func_list
        value.copy< Vector<FunctionDefinition_s> > (that.value);
        break;

      case 174: // opt_hint_list
      case 175: // hint_list
        value.copy< Vector<HintStmt_s> > (that.value);
        break;

      case 182: // leading_hint_table_list
        value.copy< Vector<LeadingTable_s> > (that.value);
        break;

      case 179: // hint_table_list
      case 232: // opt_view_column_define
      case 233: // view_column_define
      case 241: // opt_engine_def
        value.copy< Vector<String> > (that.value);
        break;

      case 185: // from_list
        value.copy< Vector<TableStmt_s> > (that.value);
        break;

      case 223: // insert_value_list
        value.copy< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 219: // win_type
        value.copy< WinType > (that.value);
        break;

      case 180: // opt_split
      case 183: // opt_distinct
      case 192: // opt_asc_desc
      case 199: // opt_outer
      case 218: // distinct_or_all
      case 237: // opt_not_null
      case 243: // opt_if_exists
      case 260: // opt_replace
        value.copy< bool > (that.value);
        break;

      case 249: // opt_sample_size
        value.copy< double > (that.value);
        break;

      case 194: // limit_expr
      case 239: // opt_time_precision
      case 240: // opt_char_length
      case 276: // int_value
        value.copy< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 177: // opt_qb_name
      case 178: // opt_qb_name_single
      case 202: // opt_alias
      case 234: // beg_view_define
      case 245: // op_from_database
      case 261: // package_name
      case 262: // procedure_name
      case 263: // function_name
      case 264: // param_name
      case 268: // database_name
      case 269: // relation_name
      case 270: // column_name
      case 271: // column_label
      case 272: // ident
      case 273: // datetime
      case 274: // string
      case 275: // number
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
      case 267: // relation_factor
        yylhs.value.build< BasicTableStmt_s > ();
        break;

      case 236: // column_definition
      case 259: // param
        yylhs.value.build< ColumnDefineStmt_s > ();
        break;

      case 238: // data_type
        yylhs.value.build< DataType > ();
        break;

      case 191: // order_by
      case 195: // projection
      case 203: // simple_expr
      case 204: // seconds_expr
      case 205: // arith_expr
      case 207: // cmp_expr
      case 210: // query_ref_expr
      case 211: // case_when_expr
      case 214: // logical_expr
      case 215: // column_ref
      case 216: // expr_const
      case 217: // func_expr
      case 220: // opt_arith_expr
      case 227: // update_asgn_factor
        yylhs.value.build< ExprStmt_s > ();
        break;

      case 253: // pro_or_func
      case 254: // procedure_define
      case 255: // function_define
        yylhs.value.build< FunctionDefinition_s > ();
        break;

      case 173: // opt_hint
        yylhs.value.build< Hint > ();
        break;

      case 176: // single_hint
        yylhs.value.build< HintStmt_s > ();
        break;

      case 181: // leading_hint_table
        yylhs.value.build< LeadingTable_s > ();
        break;

      case 193: // opt_select_limit
        yylhs.value.build< LimitStmt_s > ();
        break;

      case 208: // cmp_type
      case 209: // sq_cmp_type
        yylhs.value.build< OperationType > ();
        break;

      case 165: // sql_stmt
      case 166: // stmt
      case 167: // cmd_stmt
      case 168: // select_stmt
      case 169: // set_select
      case 170: // sub_set_select
      case 171: // select_with_parens
      case 172: // simple_select
      case 222: // insert_stmt
      case 225: // update_stmt
      case 228: // delete_stmt
      case 229: // explain_stmt
      case 230: // explainable_stmt
      case 231: // create_stmt
      case 242: // drop_stmt
      case 244: // show_stmt
      case 246: // use_stmt
      case 247: // desc_stmt
      case 248: // analyze_stmt
      case 250: // kill_stmt
      case 251: // create_package_stmt
      case 265: // exec_package_stmt
        yylhs.value.build< Stmt_s > ();
        break;

      case 196: // basic_table_factor
      case 197: // view_table_factor
      case 198: // joined_table_factor
      case 200: // sub_table_factor
      case 201: // table_factor
        yylhs.value.build< TableStmt_s > ();
        break;

      case 235: // table_element_list
      case 256: // table_type
      case 257: // opt_param_list
      case 258: // param_list
        yylhs.value.build< Vector<ColumnDefineStmt_s> > ();
        break;

      case 184: // select_expr_list
      case 186: // opt_where
      case 187: // opt_groupby
      case 188: // opt_having
      case 189: // opt_order_by
      case 190: // order_by_list
      case 206: // arith_expr_list
      case 212: // when_then_list1
      case 213: // when_then_list2
      case 221: // opt_partition_by
      case 224: // insert_value
      case 226: // update_asgn_list
      case 266: // opt_arith_expr_list
        yylhs.value.build< Vector<ExprStmt_s> > ();
        break;

      case 252: // pro_or_func_list
        yylhs.value.build< Vector<FunctionDefinition_s> > ();
        break;

      case 174: // opt_hint_list
      case 175: // hint_list
        yylhs.value.build< Vector<HintStmt_s> > ();
        break;

      case 182: // leading_hint_table_list
        yylhs.value.build< Vector<LeadingTable_s> > ();
        break;

      case 179: // hint_table_list
      case 232: // opt_view_column_define
      case 233: // view_column_define
      case 241: // opt_engine_def
        yylhs.value.build< Vector<String> > ();
        break;

      case 185: // from_list
        yylhs.value.build< Vector<TableStmt_s> > ();
        break;

      case 223: // insert_value_list
        yylhs.value.build< Vector<Vector<ExprStmt_s>> > ();
        break;

      case 219: // win_type
        yylhs.value.build< WinType > ();
        break;

      case 180: // opt_split
      case 183: // opt_distinct
      case 192: // opt_asc_desc
      case 199: // opt_outer
      case 218: // distinct_or_all
      case 237: // opt_not_null
      case 243: // opt_if_exists
      case 260: // opt_replace
        yylhs.value.build< bool > ();
        break;

      case 249: // opt_sample_size
        yylhs.value.build< double > ();
        break;

      case 194: // limit_expr
      case 239: // opt_time_precision
      case 240: // opt_char_length
      case 276: // int_value
        yylhs.value.build< int > ();
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 177: // opt_qb_name
      case 178: // opt_qb_name_single
      case 202: // opt_alias
      case 234: // beg_view_define
      case 245: // op_from_database
      case 261: // package_name
      case 262: // procedure_name
      case 263: // function_name
      case 264: // param_name
      case 268: // database_name
      case 269: // relation_name
      case 270: // column_name
      case 271: // column_label
      case 272: // ident
      case 273: // datetime
      case 274: // string
      case 275: // number
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
#line 364 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		driver.result = yystack_[1].value.as< Stmt_s > ();
		YYACCEPT;
    }
#line 1171 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 371 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1177 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 372 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1183 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 373 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1189 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 374 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1195 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 375 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1201 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 376 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1207 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 377 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow SQL"); }
#line 1213 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 386 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 387 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1225 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 388 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1231 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 389 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1237 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 390 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1243 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 391 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1249 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 392 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1255 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 393 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1261 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 394 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1267 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 399 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1275 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 403 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1283 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 410 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1292 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 415 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1301 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 420 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1310 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 425 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1319 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 430 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1328 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 435 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1337 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 440 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1346 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 445 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1355 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 453 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1363 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 457 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1371 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 464 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1379 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 474 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 1398 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 491 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Hint > () = Hint(); }
#line 1404 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 493 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[1].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = false;
	}
#line 1414 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 499 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[2].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = true;
	}
#line 1424 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 508 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
	}
#line 1432 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 512 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[0].value.as< Vector<HintStmt_s> > ();
	}
#line 1440 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 519 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1451 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 526 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[1].value.as< Vector<HintStmt_s> > ();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1462 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 537 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(NO_REWRITE);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1471 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 542 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1480 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 547 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1489 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 552 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1498 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 557 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1507 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 562 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1516 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 567 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1525 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 572 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1534 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 577 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1543 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 582 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(WIN_MAGIC, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[3].value.as< std::string > ());
		WinMagicHintStmt_s win_magic_hint = yylhs.value.as< HintStmt_s > ();
		win_magic_hint->dst_qb_name = yystack_[1].value.as< std::string > ();
	}
#line 1554 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 589 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(WIN_MAGIC, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1563 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 595 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1575 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 603 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1587 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 611 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1599 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 619 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1611 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 627 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->set_is_ordered();
	}
#line 1622 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 634 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1635 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 644 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(PARALLEL);
		ParallelHintStmt_s hint = yylhs.value.as< HintStmt_s > ();
		hint->parallel = yystack_[1].value.as< int > ();
	}
#line 1645 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 652 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1651 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 654 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1659 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 660 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1665 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 662 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "";
	}
#line 1673 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 666 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > ();
	}
#line 1681 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 673 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1690 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 678 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1699 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 685 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1705 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 686 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1711 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 691 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = true;
		yylhs.value.as< LeadingTable_s > ()->table_name = yystack_[0].value.as< std::string > ();
	}
#line 1721 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 697 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = false;
		yylhs.value.as< LeadingTable_s > ()->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1731 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 706 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = Vector<LeadingTable_s>();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1740 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 711 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = yystack_[2].value.as< Vector<LeadingTable_s> > ();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1749 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 718 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1755 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 719 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1761 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 724 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1770 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 729 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yystack_[2].value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
    }
#line 1779 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 737 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = Vector<TableStmt_s>();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1788 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 742 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = yystack_[2].value.as< Vector<TableStmt_s> > ();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1797 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 749 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1803 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 750 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1809 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 754 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1815 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 755 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1821 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 759 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1827 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 760 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1833 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 764 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1839 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 765 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1845 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 770 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 1854 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 775 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 1863 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 783 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OrderStmt::make_order_stmt(yystack_[1].value.as< ExprStmt_s > (), yystack_[0].value.as< bool > ());
    }
#line 1871 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 789 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1877 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 790 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1883 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 791 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1889 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 795 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< LimitStmt_s > () = NULL;}
#line 1895 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 797 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
    }
#line 1906 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 804 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
	}
#line 1917 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 814 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = yystack_[0].value.as< int > ();
	}
#line 1925 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 821 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< ExprStmt_s > ()->to_string();
    }
#line 1935 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 827 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1945 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 833 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[2].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1955 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 842 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< BasicTableStmt_s > ();
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
    }
#line 1964 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 847 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		TableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		yylhs.value.as< TableStmt_s > () = table;
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1976 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 858 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		yylhs.value.as< TableStmt_s > () = ViewTableStmt::make_view_table(yystack_[2].value.as< Stmt_s > ());
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1986 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 867 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), LeftOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1994 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 871 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), RightOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 2002 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 875 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), FullOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 2010 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 879 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), Inner, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 2018 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 886 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 2026 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 890 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 2034 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 897 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 2042 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 901 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 2050 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 905 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< TableStmt_s > ();
	}
#line 2058 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 912 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2066 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 916 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2074 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 920 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2082 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 926 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 2088 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 928 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2096 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 932 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2104 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 944 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2112 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 948 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2120 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 952 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2128 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 956 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2136 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 960 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2144 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 967 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_day(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2158 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 977 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_month(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2172 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 987 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_year(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2186 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 1000 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2194 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 1004 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2203 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 1009 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MINUS);
    }
#line 2212 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1014 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_ADD);
	}
#line 2221 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1019 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_SUB);
	}
#line 2230 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1024 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MUL);
	}
#line 2239 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1029 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DIV);
	}
#line 2248 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1034 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_ADD);
  }
#line 2257 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1039 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_SUB);
  }
#line 2266 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1044 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2274 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1052 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2284 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1058 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2294 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1067 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2303 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1072 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), OP_IS_NULL);
    }
#line 2312 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1077 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), OP_IS_NOT_NULL);
    }
#line 2321 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1082 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[4].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_BETWEEN);
    }
#line 2330 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1087 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_BETWEEN);
    }
#line 2339 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1092 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2354 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1103 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2369 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1114 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_IN);
    }
#line 2378 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1119 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_IN);
    }
#line 2387 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1124 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EXISTS);
    }
#line 2395 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1128 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_EXISTS);
    }
#line 2404 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1133 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2413 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1141 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE;
	}
#line 2421 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1145 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT;
	}
#line 2429 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1149 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ;
	}
#line 2437 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1153 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE;
	}
#line 2445 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1157 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT;
	}
#line 2453 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1161 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE;
	}
#line 2461 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1165 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LIKE;
	}
#line 2469 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1169 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NOT_LIKE;
	}
#line 2477 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1176 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ANY;
	}
#line 2485 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1180 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ANY;
	}
#line 2493 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1184 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ANY;
	}
#line 2501 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1188 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ANY;
	}
#line 2509 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1192 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ANY;
	}
#line 2517 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1196 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ANY;
	}
#line 2525 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1200 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ALL;
	}
#line 2533 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1204 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ALL;
	}
#line 2541 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1208 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ALL;
	}
#line 2549 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1212 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ALL;
	}
#line 2557 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1216 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ALL;
	}
#line 2565 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1220 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ALL;
	}
#line 2573 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1227 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< ExprStmt_s > () = query_expr;
    }
#line 2583 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1236 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[4].value.as< ExprStmt_s > ());
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2594 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1243 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2604 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1252 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2614 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1258 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2624 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1267 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2634 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1273 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2644 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1282 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2652 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1286 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2660 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1290 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_AND);
    }
#line 2669 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1295 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_OR);
    }
#line 2678 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1303 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2689 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1310 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2699 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1316 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2710 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1324 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2721 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1331 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2733 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1339 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2745 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1350 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2758 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1359 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2775 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1372 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2788 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1381 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2801 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1393 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2830 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1418 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2846 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 1430 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_CHAR);
  }
#line 2854 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 1434 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	Object_s value = Varchar::make_object(yystack_[1].value.as< std::string > ());
	check(value);
	ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
	check(stmt);
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), stmt, OP_TO_CHAR);
  }
#line 2866 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 1442 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2882 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 1454 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_IFNULL);
  }
#line 2890 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 1458 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_NUMBER);
  }
#line 2898 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 1466 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
	}
#line 2906 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 1470 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 2914 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 1474 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 2922 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 1481 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_COUNT;
	}
#line 2930 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 1485 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_SUM;
	}
#line 2938 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 1489 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_AVG;
	}
#line 2946 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 1493 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_MIN;
	}
#line 2954 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 1497 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_MAX;
	}
#line 2962 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 204:
#line 1501 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_RANK;
	}
#line 2970 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 205:
#line 1505 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_DENSE_RANK;
	}
#line 2978 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 206:
#line 1509 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_ROW_NUMBER;
	}
#line 2986 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 207:
#line 1516 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = ExprStmt_s();
	}
#line 2994 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 208:
#line 1520 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 3002 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 209:
#line 1527 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
	}
#line 3010 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 210:
#line 1531 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > ();
	}
#line 3018 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 211:
#line 1543 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		insert_stmt->value_list = yystack_[0].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 3031 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 212:
#line 1552 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[1].value.as< BasicTableStmt_s > ();
		insert_stmt->query_values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 3043 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 213:
#line 1563 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = Vector<Vector<ExprStmt_s>>();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
    }
#line 3052 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 214:
#line 1568 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = yystack_[4].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
	}
#line 3061 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 215:
#line 1576 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 3070 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 216:
#line 1581 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3079 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 217:
#line 1594 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3094 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 218:
#line 1605 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3109 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 219:
#line 1619 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3119 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 220:
#line 1625 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3129 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 221:
#line 1634 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), col, yystack_[0].value.as< ExprStmt_s > (), OP_ASSIGN);
    }
#line 3140 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 222:
#line 1649 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3155 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 223:
#line 1660 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3170 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 224:
#line 1671 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		delete_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		delete_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 3184 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 225:
#line 1689 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		DMLStmt_s dml = yylhs.value.as< Stmt_s > ();
		dml->is_explain = true;
    }
#line 3194 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 226:
#line 1697 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3200 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 227:
#line 1698 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3206 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 228:
#line 1699 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3212 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 229:
#line 1700 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3218 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 230:
#line 1710 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateTable);
		check(cmd_stmt);
		cmd_stmt->params.create_table_params.table = yystack_[4].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.create_table_params.column_define_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		cmd_stmt->params.create_table_params.engine_args = yystack_[0].value.as< Vector<String> > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3231 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 231:
#line 1719 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		cmd_stmt->params.create_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3242 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 232:
#line 1726 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3257 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 233:
#line 1740 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
	}
#line 3265 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 234:
#line 1744 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[1].value.as< Vector<String> > ();
	}
#line 3273 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 235:
#line 1751 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3282 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 236:
#line 1756 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3291 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 237:
#line 1764 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.left_string();
	}
#line 3299 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 238:
#line 1771 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3308 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 239:
#line 1776 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3317 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 240:
#line 1784 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[2].value.as< std::string > (), yystack_[1].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
    }
#line 3327 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 241:
#line 1793 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 3335 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 242:
#line 1797 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		
	}
#line 3343 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 243:
#line 1804 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3349 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 244:
#line 1806 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3355 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 245:
#line 1808 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_MEDIUMINT_PREC, 0); }
#line 3361 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 246:
#line 1810 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3367 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 247:
#line 1812 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3373 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 248:
#line 1814 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
#line 3379 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 249:
#line 1816 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3385 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 250:
#line 1818 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3391 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 251:
#line 1820 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3397 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 252:
#line 1822 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3403 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 253:
#line 1824 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3409 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 254:
#line 1826 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3415 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 255:
#line 1828 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3421 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 256:
#line 1830 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3427 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 257:
#line 1832 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3433 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 258:
#line 1834 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3439 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 259:
#line 1836 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3445 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 260:
#line 1838 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3451 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 261:
#line 1840 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIMESTAMP, yystack_[0].value.as< int > ()); }
#line 3457 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 262:
#line 1842 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATETIME); }
#line 3463 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 263:
#line 1844 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATE); }
#line 3469 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 264:
#line 1846 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIME); }
#line 3475 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 265:
#line 1850 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_TIME_SCALE; }
#line 3481 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 266:
#line 1852 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		if (yystack_[1].value.as< int > () > MAX_TIME_SCALE) {
			yyerror("max time prec:%d", MAX_TIME_SCALE);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	}
#line 3493 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 267:
#line 1862 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_STR_LENGTH; }
#line 3499 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 268:
#line 1864 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		  if (yystack_[1].value.as< int > () > MAX_STR_LENGTH) {
			yyerror("max char length:%d", MAX_STR_LENGTH);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	  }
#line 3511 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 269:
#line 1875 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<String> > () = Vector<String>(); 
		yylhs.value.as< Vector<String> > ().push_back("CAT");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
	}
#line 3523 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 270:
#line 1883 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back("CSV");
		yylhs.value.as< Vector<String> > ().push_back(yystack_[10].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back(yystack_[5].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back("new line");
	}
#line 3535 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 271:
#line 1899 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		cmd_stmt->params.drop_table_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_table_params.table = yystack_[0].value.as< TableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3547 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 272:
#line 1907 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		cmd_stmt->params.drop_database_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3559 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 273:
#line 1915 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropView);
		check(cmd_stmt);
		cmd_stmt->params.drop_view_params.database = driver.get_global_database();
		cmd_stmt->params.drop_view_params.ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->params.drop_view_params.view_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3572 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 274:
#line 1926 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3578 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 275:
#line 1927 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3584 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 276:
#line 1937 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3595 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 277:
#line 1944 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		cmd_stmt->params.show_databases_params.is_select_current_database = false;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
 	}
#line 3606 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 278:
#line 1951 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3617 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 279:
#line 1958 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		cmd_stmt->params.show_tables_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3628 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 280:
#line 1965 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3639 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 281:
#line 1972 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3647 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 282:
#line 1976 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = NULL;
	}
#line 3655 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 283:
#line 1980 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_table_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3667 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 284:
#line 1988 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		cmd_stmt->params.desc_table_params.is_show_column_statis = true;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3679 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 285:
#line 1996 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowProcesslist);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3689 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 286:
#line 2002 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowMemory);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3699 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 287:
#line 2011 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 3707 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 288:
#line 2015 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 3715 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 289:
#line 2022 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(UseDatabase);
		check(cmd_stmt);
		cmd_stmt->params.use_database_params.database = yystack_[0].value.as< std::string > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3726 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 290:
#line 2037 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3737 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 291:
#line 2044 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		cmd_stmt->params.desc_table_params.table = yystack_[0].value.as< BasicTableStmt_s > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3748 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 292:
#line 2058 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = driver.get_global_database();
		cmd_stmt->params.analyze_params.table = yystack_[1].value.as< std::string > ();
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3761 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 293:
#line 2067 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = yystack_[1].value.as< std::string > ();
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3774 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 294:
#line 2076 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = yystack_[3].value.as< std::string > ();
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3787 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 295:
#line 2085 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Analyze);
		check(cmd_stmt);
		cmd_stmt->params.analyze_params.database = "*";
		cmd_stmt->params.analyze_params.table = "*";
		cmd_stmt->params.analyze_params.sample_size = yystack_[0].value.as< double > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3800 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 296:
#line 2096 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {	yylhs.value.as< double > () = 0.1; }
#line 3806 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 297:
#line 2098 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< double > () = std::stod(yystack_[0].value.as< std::string > ());
	}
#line 3814 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 298:
#line 2105 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		cmd_stmt->params.kill_params.session_id = yystack_[0].value.as< int > ();
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3825 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 299:
#line 2122 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreatePackage);
		check(cmd_stmt);
		CreatePackageParam_s param = CreatePackageParam::make_create_package_param();
		param->is_replace = yystack_[5].value.as< bool > ();
		param->name = yystack_[3].value.as< std::string > ();
		param->functions = yystack_[1].value.as< Vector<FunctionDefinition_s> > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3840 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 300:
#line 2136 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<FunctionDefinition_s> > () = Vector<FunctionDefinition_s>();
		yylhs.value.as< Vector<FunctionDefinition_s> > ().push_back(yystack_[0].value.as< FunctionDefinition_s > ());
	}
#line 3849 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 301:
#line 2141 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<FunctionDefinition_s> > () = yystack_[1].value.as< Vector<FunctionDefinition_s> > ();
		yylhs.value.as< Vector<FunctionDefinition_s> > ().push_back(yystack_[0].value.as< FunctionDefinition_s > ());
	}
#line 3858 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 302:
#line 2149 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = yystack_[0].value.as< FunctionDefinition_s > ();
	}
#line 3866 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 303:
#line 2153 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = yystack_[0].value.as< FunctionDefinition_s > ();
	}
#line 3874 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 304:
#line 2160 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = FunctionDefinition::make_func_define();
		yylhs.value.as< FunctionDefinition_s > ()->name = yystack_[4].value.as< std::string > ();
		yylhs.value.as< FunctionDefinition_s > ()->param_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3884 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 305:
#line 2170 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = FunctionDefinition::make_func_define();
		yylhs.value.as< FunctionDefinition_s > ()->name = yystack_[6].value.as< std::string > ();
		yylhs.value.as< FunctionDefinition_s > ()->param_list = yystack_[4].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< FunctionDefinition_s > ()->return_type_list = yystack_[1].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3895 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 306:
#line 2178 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = FunctionDefinition::make_func_define();
		yylhs.value.as< FunctionDefinition_s > ()->name = yystack_[6].value.as< std::string > ();
		yylhs.value.as< FunctionDefinition_s > ()->param_list = yystack_[4].value.as< Vector<ColumnDefineStmt_s> > ();
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[6].value.as< std::string > (), yystack_[1].value.as< DataType > ());
		yylhs.value.as< FunctionDefinition_s > ()->return_type_list.push_back(stmt);
	}
#line 3907 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 307:
#line 2189 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[1].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3915 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 308:
#line 2196 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
	}
#line 3923 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 309:
#line 2200 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[0].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3931 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 310:
#line 2207 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
	}
#line 3940 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 311:
#line 2212 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
	}
#line 3949 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 312:
#line 2220 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as< std::string > (), yystack_[0].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
	}
#line 3959 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 313:
#line 2228 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3965 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 314:
#line 2229 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3971 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 315:
#line 2233 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3977 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 316:
#line 2237 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3983 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 317:
#line 2241 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3989 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 318:
#line 2245 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3995 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 319:
#line 2250 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ExecFunction);
		check(cmd_stmt);
		ExecFunctionParam_s param = ExecFunctionParam::make_exec_func_param();
		param->package_name = yystack_[5].value.as< std::string > ();
		param->function_name = yystack_[3].value.as< std::string > ();
		param->param_list = yystack_[1].value.as< Vector<ExprStmt_s> > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 4010 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 320:
#line 2264 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
	}
#line 4018 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 321:
#line 2268 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > ();
	}
#line 4026 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 322:
#line 2280 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 4038 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 323:
#line 2288 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 4050 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 324:
#line 2298 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4056 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 325:
#line 2302 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4062 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 326:
#line 2306 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4068 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 327:
#line 2310 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 4074 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 328:
#line 2314 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4080 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 329:
#line 2318 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4086 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 330:
#line 2319 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4092 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 331:
#line 2323 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4098 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 332:
#line 2327 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4104 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 333:
#line 2332 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 4112 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;


#line 4116 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -534;

  const short int parser::yytable_ninf_ = -326;

  const short int
  parser::yypact_[] =
  {
     295,  -112,     6,    32,    52,    52,   -28,    52,    15,   -26,
      81,    85,    24,   148,    32,    52,    92,    97,  -534,  -534,
     422,   449,  -534,   -83,  -534,  -534,  -534,  -534,  -534,  -534,
    -534,  -534,  -534,  -534,  -534,  -534,  -534,   190,   110,    52,
      52,    52,   136,  1037,   166,  -534,  -534,   138,  -534,   153,
    -534,   211,   211,   211,   188,  -534,    32,  -534,  -534,  -534,
    -534,  -534,    52,  -534,  -534,  -534,   184,   247,   287,   232,
     290,  -534,   231,   322,  -534,  -534,  -534,   264,   338,    52,
    -534,  -534,  -534,  -534,   -11,    85,    85,    14,    85,    85,
     301,   323,   313,  -534,  -534,   353,   354,    52,  1099,   356,
     357,   356,   356,   356,   356,   356,   356,   370,   371,   356,
     356,   376,   356,   356,   391,   392,   394,   427,  1099,  -534,
      52,   429,   430,    52,   436,    52,    25,    52,    52,   146,
    -534,   381,  -534,  1029,    52,    52,   338,    52,    52,    52,
    -534,   372,    85,  -534,  -534,  -534,  -534,    85,  -534,  -534,
    -534,   415,   194,   390,  -534,    52,    52,   500,   511,   473,
      11,  -534,   512,  -534,  -534,  -534,  -534,  -534,  -534,   512,
     512,  -534,  -534,    81,  -534,  -534,   512,   512,   513,  -534,
    -534,   365,    52,    25,  -534,  -534,  -534,  -534,   365,    85,
    -534,  -534,  -534,   209,   365,  -534,   448,  -534,   451,  -534,
    -534,  -534,  -534,  -534,   499,  -534,   522,   522,  -534,   457,
     914,  -534,  -534,  1029,  -534,  1029,  -534,  -534,  -534,   458,
    -534,   460,   464,  -534,    27,  -534,  -534,   185,  -534,  -534,
    -534,  -534,  -534,   465,   442,  -534,   444,  -534,  -534,  -534,
    -534,  -534,  -534,  -534,  -534,  -534,    52,  -534,  -534,   313,
     313,   313,    81,    -7,  -534,   639,  -534,    68,  -534,  -534,
     -16,   487,   441,  -534,  -534,     7,    52,    52,   443,    52,
      52,   533,    52,   417,  -534,   365,   -33,   209,  -534,   446,
     466,   494,   466,   466,  -534,  1029,  1029,   543,   831,   112,
     -31,  -534,  -534,  1029,   456,   299,   414,   414,  1029,  1029,
    1029,  1029,    25,    52,  1029,   946,  1029,   946,  -534,  -534,
     202,    71,     9,  -534,   583,  -534,  -534,  -534,  -534,    52,
     525,  -534,  -534,   501,   501,  -534,  -534,   502,  -534,  -534,
    -534,  -534,  -534,   503,  -534,  -534,   506,   506,  -534,   501,
     501,   587,    52,  -534,    85,    52,    52,   -18,  -534,  -534,
    -534,  -534,  -534,     7,  -534,    78,  -534,   101,  -534,   133,
    -534,   150,   178,  -534,   579,  -534,   831,  -534,   417,    25,
    -534,   365,  -534,   519,   227,   520,   524,   414,   567,   482,
     414,   181,   521,   546,   528,   784,  1094,  -534,    12,  1029,
      22,  1029,   831,   420,  -534,   463,   325,   308,  -534,   -33,
    -534,  -534,   522,  -534,    16,  -534,  -534,    16,  -534,  -534,
    1029,  -534,  -534,  -534,    52,    25,  -534,  1029,  -534,   609,
    -534,    81,  -534,  -534,    81,    81,    81,  -534,  -534,  -534,
    -534,   599,  -534,  -534,  -534,   530,   532,  -534,  -534,  -534,
     182,  -534,     7,  -534,    52,  -534,  -534,  -534,   496,   169,
    -534,   209,  -534,   227,    20,  -534,  -534,   523,   227,   227,
    1029,  -534,  1029,  -534,  1029,   528,  -534,   694,    37,   254,
     271,   324,   331,   342,   363,  -534,   316,    53,  1029,   540,
    1029,   528,   831,   831,  1029,   228,  1029,  1029,   468,    17,
    1029,    81,   522,  -534,  -534,   560,   -25,   414,   507,  -534,
     -33,   414,   591,   508,   598,   601,   514,  -534,    52,    52,
    -534,  -534,  -534,  -534,   534,    85,   516,   209,   831,   545,
     547,   414,   414,   183,  -534,  -534,  -534,  -534,  -534,  -534,
    -534,  -534,  -534,  -534,  -534,  -534,  -534,  -534,  -534,  1029,
     558,   635,  -534,   205,   914,  -534,   414,  -534,   644,  -534,
     414,  1029,   471,   285,  -534,  1029,   413,   620,   536,   626,
     592,  -534,  -534,  -534,   552,  -534,   627,  -534,    81,    81,
    -534,   542,   628,  -534,   639,  -534,   551,   831,   554,  -534,
     169,   831,   831,  -534,   258,   914,  -534,  -534,  1029,   186,
     414,  -534,  1029,   414,  -534,    81,  -534,  1029,   831,   570,
     593,   605,   559,   562,   568,    52,  -534,   563,   169,   544,
     169,   169,  1029,   191,   414,  -534,   414,   565,   567,   169,
     656,   613,   588,   701,  -534,  -534,   581,  -534,  -534,   414,
    -534,  -534,  1029,    81,  -534,   674,   570,   522,   621,   580,
     584,   676,  -534,   265,   678,  -534,  1029,   594,   679,    52,
    -534,  -534,  1029,  -534,  -534,  -534,    81,   567,  -534,   657,
     198,  -534,  -534,   596,  -534,   689,   522,   691,   646,   597,
     704,   522,  -534
  };

  const unsigned short int
  parser::yydefact_[] =
  {
       9,     0,   313,    33,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,    33,     0,     0,     0,     3,     4,
      20,     0,    30,    29,     5,     6,     7,     8,    11,    12,
      10,    14,    13,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,    36,     0,   328,   291,     0,   322,   325,
     290,   274,   274,   274,     0,   315,    33,   226,   228,   229,
     227,   225,     0,   332,   333,   298,     0,     0,    71,     0,
       0,   277,     0,     0,   286,   285,   282,     0,   288,     0,
     289,   324,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     0,   296,   314,   231,     0,   233,     0,    36,    60,
       0,    60,    60,    60,    60,    60,    60,     0,     0,    60,
      60,     0,    60,    60,     0,     0,     0,     0,    37,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,     0,    72,     0,     0,     0,   288,     0,     0,     0,
     279,     0,     0,    22,    29,    28,    26,     0,    21,    27,
      25,     0,     0,     0,   292,     0,     0,     0,     0,     0,
       0,    47,    58,    43,    48,    44,    40,    46,    42,    58,
      58,    50,    55,     0,    45,    41,    58,    58,     0,    34,
      39,   113,     0,     0,   323,   325,   275,   272,   113,     0,
     110,   111,   112,   271,   113,   273,     0,   317,     0,   212,
     276,   188,   331,   201,     0,   199,     0,     0,   205,     0,
       0,   203,   202,     0,   180,     0,   204,   183,   206,     0,
     200,     0,     0,   168,     0,    73,   124,    95,   120,   133,
     116,   117,   119,     0,     0,   179,   326,   186,   185,   187,
     284,   280,   278,   281,   283,   287,     0,    24,    23,   296,
     296,   296,     0,     0,   238,     0,   326,     0,   235,   237,
       0,     0,     0,    61,    59,     0,     0,     0,     0,     0,
       0,    65,     0,    78,   114,   113,    78,    75,    99,     0,
     105,     0,   105,   105,    98,   320,     0,   211,     0,     0,
       0,   330,   329,     0,   168,     0,   126,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,   327,
     196,     0,    78,   219,     0,   295,   294,   293,   297,     0,
     269,   253,   248,   267,   267,   263,   262,   249,   256,   254,
     246,   247,   245,   250,   255,   244,   265,   265,   243,   267,
     267,   241,     0,   234,     0,     0,     0,     0,   300,   302,
     303,    35,    62,     0,    69,    65,    67,    65,    63,    65,
      57,    65,    65,    66,     0,   115,     0,   222,    78,     0,
     224,   113,   106,     0,     0,     0,     0,   134,   321,     0,
     215,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,     0,     0,   118,     0,     0,     0,    74,    78,
      97,   130,     0,   132,   128,   129,   131,   127,   197,   198,
     207,   182,   184,   181,     0,     0,   217,     0,   239,     0,
     230,     0,   258,   257,     0,     0,     0,   264,   261,   260,
     259,     0,   240,   236,   232,     0,     0,   316,   299,   301,
      65,    56,     0,    52,     0,    54,    51,    53,     0,    77,
     223,    76,   100,     0,     0,   107,   108,     0,     0,     0,
       0,   319,     0,   213,     0,     0,   145,     0,     0,   148,
     149,   150,   152,   151,   153,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,   195,    80,     0,   208,     0,   220,
      78,   221,     0,     0,     0,     0,     0,   242,   308,   308,
      68,    70,    64,    49,     0,     0,   112,     0,     0,     0,
       0,   135,   216,     0,   146,   176,   162,   156,   163,   157,
     164,   158,   166,   160,   165,   159,   167,   161,   155,     0,
       0,     0,   137,     0,     0,   143,   136,   147,   178,   177,
     173,     0,     0,     0,   170,     0,     0,     0,     0,     0,
      82,   121,   122,   123,   189,   218,     0,   268,     0,     0,
     266,     0,   309,   310,     0,   318,     0,     0,     0,   109,
     104,     0,     0,   214,     0,     0,   144,   138,     0,     0,
     171,   169,     0,   174,   194,     0,   192,     0,     0,    83,
       0,     0,     0,     0,     0,     0,   312,     0,   103,   113,
     101,   102,     0,     0,   139,   141,   172,     0,    79,    81,
       0,    91,   209,     0,   251,   252,     0,   311,   304,   140,
     142,   193,     0,     0,    32,     0,    83,     0,     0,     0,
       0,    84,    85,    88,    93,    94,     0,     0,     0,     0,
     306,   305,     0,    89,    90,    87,     0,   210,   190,     0,
       0,    86,    92,     0,   307,     0,     0,     0,     0,     0,
       0,     0,   270
  };

  const short int
  parser::yypgoto_[] =
  {
    -534,  -534,  -534,  -534,    10,  -534,   667,     1,   680,    59,
     632,  -534,   623,    76,   669,   131,  -264,   300,   393,  -534,
    -534,  -288,  -265,  -534,  -534,   108,  -534,    93,  -534,  -534,
     102,   459,    36,    50,   303,   152,   -79,  -122,   -80,  -534,
     452,   192,  -392,  -534,  -534,  -534,  -368,  -534,  -534,  -534,
    -322,  -534,  -534,  -534,  -534,  -534,  -534,  -534,   741,  -534,
     297,   755,  -534,   369,   780,  -534,  -534,  -534,  -534,  -534,
    -534,  -534,   472,  -534,  -533,   453,   -91,  -534,  -534,   386,
    -534,   659,  -534,  -534,  -534,   222,  -534,  -534,  -534,   454,
    -534,  -534,  -534,   284,   151,   200,  -534,   702,  -534,   467,
    -534,  -534,  -534,   196,    39,    21,  -136,   515,    -2,  -534,
    -184,   -10,    -4
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    16,    17,    18,    66,    20,    21,   223,    23,    68,
     117,   118,   119,   265,   161,   357,   444,   354,   355,   133,
     224,   276,   367,   560,   599,   621,   641,   642,   655,   634,
     644,   225,   190,   191,   192,   373,   457,   277,   452,   226,
     403,   386,   378,   387,   480,   481,   228,   229,   390,   290,
     388,   230,   231,   232,   410,   233,   498,   636,    24,   287,
     381,    25,   312,   313,    26,    27,    61,    28,   157,   257,
     344,   253,   254,   432,   341,   427,   422,   420,    29,   125,
      30,   140,    31,    32,    33,   154,    34,    35,   347,   348,
     349,   350,   640,   571,   572,   573,    42,    54,   436,   196,
     574,    36,   379,   194,    47,   234,   235,   308,   236,   237,
     238,   239,   645
  };

  const short int
  parser::yytable_[] =
  {
      64,    22,    49,    49,   193,    55,    65,   364,   369,    22,
      19,   370,    22,    81,   399,   142,   466,    51,    57,   255,
     258,    38,   291,   292,   561,    48,    48,   482,   483,   391,
      45,    37,   482,   483,   319,    49,   262,    81,    49,    96,
     147,   606,    49,    45,   449,   438,   -19,   416,    45,   -19,
     414,    39,   482,   483,    80,   345,    43,   345,    92,    62,
      49,    48,    44,   468,    43,    48,     3,   541,   301,    67,
     489,   542,   304,    79,   562,    45,    91,    49,    94,   188,
     415,    11,   486,    48,   188,    22,    22,    22,    22,    22,
      22,   442,    82,   639,    45,    55,     9,   524,   302,   353,
      48,   273,   346,   450,   346,    63,    11,    11,   278,   342,
     314,   545,   515,   547,   284,    52,   306,   189,    49,   363,
      56,   185,   320,    81,    49,   195,   197,   500,   366,   392,
      22,    53,    49,    49,   495,    49,    49,    81,   563,   199,
     263,    48,   363,    22,   184,    56,    56,    48,    22,    40,
     185,    56,   589,   256,   256,    48,    48,   484,    48,    48,
     548,   549,   555,    64,   187,    41,   525,    14,   304,   268,
     366,   411,   586,   251,   363,   413,   442,    11,   245,   185,
      49,    49,   487,   255,   482,   483,   185,    69,    70,    45,
      22,   363,   185,   613,    71,   368,   580,   343,   412,   279,
      46,    50,   274,    48,    48,   618,   433,   441,    45,   274,
     305,   294,   306,    45,   303,   274,    56,    45,   496,   363,
      72,   588,   462,   363,   462,   309,    73,   460,   408,    83,
     443,   307,   460,   423,    93,   565,    95,   120,    11,   605,
     122,   304,   318,    74,   256,   266,   267,   451,   429,   430,
      45,    97,   269,   270,   657,   608,   123,   409,   129,   610,
     611,   304,   445,   356,   358,   358,   121,   358,   358,    75,
     185,  -324,   389,   185,   612,   141,   619,    56,   314,   446,
     526,   280,   527,   305,   304,   306,   188,   124,    76,   281,
      90,    77,    78,   365,   250,   653,   274,   528,   282,   529,
      49,   309,   198,   305,   307,   306,   128,   447,   558,   256,
     463,   510,   583,   130,   304,   615,   181,   256,   654,   454,
     630,   304,     1,    48,   307,   227,   305,   664,   306,   538,
     240,   241,   517,   243,   244,   283,   539,   540,     2,   131,
     256,   304,   132,   197,   437,    22,     3,   307,     4,     5,
     530,   356,   531,     6,   434,   304,   305,   532,   306,   533,
       7,   135,     8,   305,   304,   306,   492,    49,   534,   185,
     535,   134,    49,   551,   514,   136,     9,   307,   275,   519,
     520,   304,    10,   305,   307,   306,   294,    11,    45,   536,
      48,   537,   274,   137,   272,    48,   289,   305,   359,   306,
     361,   362,   295,   138,   307,   296,   305,   297,   306,   139,
     455,    64,   256,    49,    64,    64,    64,   503,   307,   151,
     504,   505,   506,   305,   456,   306,    12,   307,   394,    13,
     592,    84,    85,    86,   375,   376,    48,   494,   126,   127,
     356,   152,   512,   153,   307,   155,   156,    14,   160,   162,
      15,    49,    49,   648,   493,    22,    49,    49,    87,    88,
      89,   490,   169,   170,   279,   -30,   -30,   -30,   173,   304,
     304,   315,   316,   317,    48,    48,   304,   377,   380,    48,
      48,   557,   667,   176,   177,   393,   178,   672,   179,   455,
     395,   396,   397,   227,   455,   455,   401,   404,   405,   407,
     182,   183,   186,   456,   491,   246,   575,   575,   456,   456,
     200,   305,   305,   306,   306,   249,    22,   201,   305,   304,
     306,   202,    45,    63,   304,   578,   252,   304,   260,   259,
     203,   554,   307,   307,   591,   261,   204,   264,   271,   307,
     285,   205,   594,   286,   202,   294,   206,   207,   351,   293,
     298,   208,   299,   -31,   -31,   -31,   300,   310,    64,    64,
     311,   305,  -325,   306,   602,   603,   305,    45,   306,   305,
     352,   306,   360,   272,   363,   371,   209,   467,   366,   372,
     374,   485,   307,   488,   382,   617,   294,   307,   417,   419,
     307,   210,   211,   421,   424,   425,   212,   213,   426,   214,
     321,   431,   497,   575,   448,   453,   458,   185,   460,   501,
     459,   461,   465,   464,   502,   322,   323,   507,   215,   324,
      11,   216,   508,    64,   509,   513,   217,   218,   325,   326,
     274,   327,   544,   518,   559,   566,   564,   567,   328,   568,
     219,   220,   569,   570,   577,   579,    64,   575,   221,   222,
     585,   329,   521,   587,   522,   581,   380,   582,   321,   288,
     483,   595,   597,   330,   331,   596,   600,   598,   601,   605,
     543,   604,   546,   322,   323,   332,   550,   324,   552,   553,
     607,   620,   556,   609,   623,   622,   325,   326,   624,   327,
     333,   625,   632,   626,   631,   628,   328,   469,   470,   471,
     472,   473,   474,   633,   334,   635,   637,   475,   476,   329,
     646,   477,   650,   649,   478,   479,   651,   652,   335,   656,
     659,   330,   331,   658,   638,   666,   663,   336,   337,   338,
     159,   584,   668,   332,   665,   670,   377,   669,   339,   340,
     671,   180,   511,   590,   647,   661,   440,   593,   333,    58,
     304,   143,   145,   146,   148,   149,   150,   516,   662,   406,
     398,   523,   334,    59,   144,   144,   144,   144,   144,   144,
     163,   164,   165,   166,   167,   168,   335,   377,   171,   172,
     614,   174,   175,   499,   616,   336,   337,   338,    60,   377,
     428,   418,   305,   576,   306,   242,   339,   340,   383,   158,
     660,   439,   201,     0,   629,   627,   202,    45,    63,   247,
       0,     0,   435,   307,   248,   203,     0,     0,   400,     0,
       0,   204,   144,   394,   643,     0,   205,   144,     0,     0,
       0,   206,   207,     0,     0,     0,   208,     0,   377,     0,
       0,     0,     0,     0,   643,   383,     0,     0,     0,   201,
     384,     0,     0,   202,    45,    63,     0,     0,     0,     0,
       0,   209,   203,     0,     0,     0,     0,     0,   204,     0,
       0,     0,     0,   205,     0,     0,   385,   211,   206,   207,
       0,   212,   213,   208,   214,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   384,     0,     0,
       0,     0,     0,   215,     0,     0,   216,     0,   209,     0,
       0,   217,   218,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,   385,   211,   219,   220,     0,   212,   213,
       0,   214,   201,   221,   222,     0,   202,    45,    63,     0,
       0,     0,     0,     0,     0,   203,     0,     0,     0,     0,
     215,   204,     0,   216,     0,     0,   205,     0,   217,   218,
       0,   206,   207,     0,   201,     0,   208,     0,   202,    45,
      63,     0,   219,   220,     0,     0,     0,   203,     0,     0,
     221,   222,     0,   204,     0,     0,     0,     0,   205,     0,
       0,   209,     0,   206,   207,     0,     0,     0,   208,     0,
       0,     0,     0,     0,     0,     0,   210,   211,     0,     0,
       0,   212,   213,     0,   214,     0,     0,     0,     0,     0,
       0,     0,     0,   209,     0,     0,     0,     0,     0,     0,
     402,     0,     0,   215,     0,     0,   216,     0,   210,   211,
       0,   217,   218,   212,   213,    56,   214,   201,     0,     0,
       0,   202,    45,    63,     0,   219,   220,     0,     0,     0,
     203,     0,     0,   221,   222,   215,   204,     0,   216,     0,
      98,   205,     0,   217,   218,     0,   206,   207,     0,     0,
       0,   208,     0,     0,     0,     0,     0,   219,   220,     0,
       0,     0,     0,     0,     0,   221,   222,   469,   470,   471,
     472,   473,   474,     0,     0,    99,   209,   475,   476,     0,
       0,   477,     0,     0,   478,   479,     0,     0,     0,     0,
       0,   210,   211,     0,     0,   100,   212,   213,     0,   214,
       0,     0,     0,   101,     0,     0,     0,     0,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,   215,   110,
     304,   216,     0,   111,     0,     0,   217,   218,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     219,   220,   112,     0,     0,     0,     0,     0,   221,   222,
       0,     0,     0,     0,     0,     0,     0,   100,   113,     0,
     114,   115,   305,     0,   306,   101,     0,     0,     0,   116,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,   110,     0,   307,     0,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,     0,   114,   115,     0,     0,     0,     0,     0,     0,
       0,   116
  };

  const short int
  parser::yycheck_[] =
  {
      10,     0,     4,     5,   126,     7,    10,   271,    41,     8,
       0,   276,    11,    15,   302,    26,   384,    45,     8,   155,
     156,    15,   206,   207,    49,     4,     5,    15,    16,    60,
      23,   143,    15,    16,    41,    37,    25,    39,    40,    41,
      26,   574,    44,    23,   366,    63,   129,   312,    23,   132,
      41,    45,    15,    16,    15,    73,    32,    73,    37,    85,
      62,    40,     3,   385,    32,    44,    51,    14,    41,    45,
     392,    18,    56,    14,    99,    23,    37,    79,    39,    59,
      71,    92,    60,    62,    59,    84,    85,    86,    87,    88,
      89,   355,     0,   626,    23,    97,    81,   465,    71,    92,
      79,   181,   120,   368,   120,    24,    92,    92,   188,    41,
     246,   479,    92,   481,   194,   143,   100,    92,   120,    41,
     131,   123,   129,   125,   126,   127,   128,   415,   161,   160,
     129,   159,   134,   135,   399,   137,   138,   139,   163,   129,
     129,   120,    41,   142,   123,   131,   131,   126,   147,   143,
     152,   131,   544,   155,   156,   134,   135,   145,   137,   138,
     482,   483,   145,   173,   125,   159,   129,   152,    56,   173,
     161,   100,   540,   152,    41,   311,   440,    92,   139,   181,
     182,   183,   160,   319,    15,    16,   188,    39,    40,    23,
     189,    41,   194,   585,    46,   275,   518,   129,   127,   189,
       4,     5,   181,   182,   183,   597,   342,   129,    23,   188,
      98,   210,   100,    23,    29,   194,   131,    23,   402,    41,
      72,    16,    41,    41,    41,   227,    78,    41,    26,   132,
     129,   119,    41,   324,   124,   500,    40,    71,    92,    41,
      44,    56,   252,    95,   246,   169,   170,   369,   339,   340,
      23,   115,   176,   177,   646,   577,   118,    55,    62,   581,
     582,    56,   129,   265,   266,   267,   100,   269,   270,   121,
     272,   118,   160,   275,    16,    79,   598,   131,   414,   129,
      26,    72,    28,    98,    56,   100,    59,    76,   140,    80,
     100,   143,   144,   272,   100,    30,   275,    26,    89,    28,
     302,   303,   156,    98,   119,   100,   118,   129,   492,   311,
     129,   129,   129,   129,    56,   129,   120,   319,    53,    92,
     129,    56,    27,   302,   119,   133,    98,   129,   100,    13,
     134,   135,   454,   137,   138,   126,    20,    21,    43,    92,
     342,    56,    55,   345,   346,   344,    51,   119,    53,    54,
      26,   353,    28,    58,   344,    56,    98,    26,   100,    28,
      65,    71,    67,    98,    56,   100,    41,   369,    26,   371,
      28,   139,   374,   145,   453,   144,    81,   119,   182,   458,
     459,    56,    87,    98,   119,   100,   385,    92,    23,    26,
     369,    28,   371,    71,    29,   374,   204,    98,   267,   100,
     269,   270,   210,   139,   119,   213,    98,   215,   100,    71,
     374,   421,   414,   415,   424,   425,   426,   421,   119,   118,
     424,   425,   426,    98,   374,   100,   131,   119,   129,   134,
     145,     9,    10,    11,   282,   283,   415,   129,    52,    53,
     442,   118,   444,   130,   119,    92,    92,   152,    92,    92,
     155,   453,   454,   637,   129,   454,   458,   459,     9,    10,
      11,    41,    92,    92,   454,     9,    10,    11,    92,    56,
      56,   249,   250,   251,   453,   454,    56,   285,   286,   458,
     459,   491,   666,    92,    92,   293,    92,   671,    61,   453,
     298,   299,   300,   301,   458,   459,   304,   305,   306,   307,
      71,    71,    66,   453,    41,   133,   508,   509,   458,   459,
     129,    98,    98,   100,   100,   100,   515,    18,    98,    56,
     100,    22,    23,    24,    56,   515,   136,    56,    17,    29,
      31,    63,   119,   119,    63,    62,    37,    25,    25,   119,
      92,    42,   129,    92,    22,   544,    47,    48,    61,    92,
      92,    52,    92,     9,    10,    11,    92,    92,   568,   569,
     118,    98,   118,   100,   568,   569,    98,    23,   100,    98,
     129,   100,   129,    29,    41,   129,    77,   385,   161,   113,
      86,   389,   119,   391,    41,   595,   585,   119,     5,    64,
     119,    92,    93,    92,    92,    92,    97,    98,    92,   100,
      19,    14,   410,   605,    25,    86,    86,   609,    41,   417,
      86,   129,    66,    92,     5,    34,    35,    18,   119,    38,
      92,   122,    92,   633,    92,   129,   127,   128,    47,    48,
     609,    50,    92,   110,    74,    44,   129,   129,    57,    41,
     141,   142,    41,   129,   110,   129,   656,   649,   149,   150,
      92,    70,   460,    18,   462,   110,   464,   110,    19,   160,
      16,    41,    36,    82,    83,   129,   114,    75,    41,    41,
     478,   129,   480,    34,    35,    94,   484,    38,   486,   487,
     129,   111,   490,   129,    79,    92,    47,    48,   129,    50,
     109,   129,    36,   125,   129,   132,    57,     3,     4,     5,
       6,     7,     8,    90,   123,   117,     5,    13,    14,    70,
      36,    17,   132,    92,    20,    21,   132,    41,   137,    41,
      41,    82,    83,   129,   143,    36,    69,   146,   147,   148,
      98,   539,    41,    94,   138,   138,   544,    91,   157,   158,
      36,   118,   442,   551,   636,   652,   353,   555,   109,     8,
      56,    84,    85,    86,    87,    88,    89,   454,   656,   307,
     301,   464,   123,     8,    84,    85,    86,    87,    88,    89,
     101,   102,   103,   104,   105,   106,   137,   585,   109,   110,
     588,   112,   113,   414,   592,   146,   147,   148,     8,   597,
     337,   319,    98,   509,   100,   136,   157,   158,    14,    97,
     649,   347,    18,    -1,   612,   605,    22,    23,    24,   142,
      -1,    -1,   345,   119,   147,    31,    -1,    -1,   303,    -1,
      -1,    37,   142,   129,   632,    -1,    42,   147,    -1,    -1,
      -1,    47,    48,    -1,    -1,    -1,    52,    -1,   646,    -1,
      -1,    -1,    -1,    -1,   652,    14,    -1,    -1,    -1,    18,
      66,    -1,    -1,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    77,    31,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    92,    93,    47,    48,
      -1,    97,    98,    52,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    -1,   122,    -1,    77,    -1,
      -1,   127,   128,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,   141,   142,    -1,    97,    98,
      -1,   100,    18,   149,   150,    -1,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
     119,    37,    -1,   122,    -1,    -1,    42,    -1,   127,   128,
      -1,    47,    48,    -1,    18,    -1,    52,    -1,    22,    23,
      24,    -1,   141,   142,    -1,    -1,    -1,    31,    -1,    -1,
     149,   150,    -1,    37,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    77,    -1,    47,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,   119,    -1,    -1,   122,    -1,    92,    93,
      -1,   127,   128,    97,    98,   131,   100,    18,    -1,    -1,
      -1,    22,    23,    24,    -1,   141,   142,    -1,    -1,    -1,
      31,    -1,    -1,   149,   150,   119,    37,    -1,   122,    -1,
      33,    42,    -1,   127,   128,    -1,    47,    48,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,     3,     4,     5,
       6,     7,     8,    -1,    -1,    68,    77,    13,    14,    -1,
      -1,    17,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    88,    97,    98,    -1,   100,
      -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,   108,    -1,    -1,   119,   112,
      56,   122,    -1,   116,    -1,    -1,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
     141,   142,   135,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,   151,    -1,
     153,   154,    98,    -1,   100,    96,    -1,    -1,    -1,   162,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,    -1,
      -1,   112,    -1,   119,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,    -1,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162
  };

  const unsigned short int
  parser::yystos_[] =
  {
       0,    27,    43,    51,    53,    54,    58,    65,    67,    81,
      87,    92,   131,   134,   152,   155,   165,   166,   167,   168,
     169,   170,   171,   172,   222,   225,   228,   229,   231,   242,
     244,   246,   247,   248,   250,   251,   265,   143,    15,    45,
     143,   159,   260,    32,   173,    23,   267,   268,   269,   272,
     267,    45,   143,   159,   261,   272,   131,   168,   222,   225,
     228,   230,    85,    24,   275,   276,   168,    45,   173,    39,
      40,    46,    72,    78,    95,   121,   140,   143,   144,   173,
     268,   272,     0,   132,     9,    10,    11,     9,    10,    11,
     100,   268,   269,   124,   268,   267,   272,   115,    33,    68,
      88,    96,   101,   102,   103,   104,   105,   106,   107,   108,
     112,   116,   135,   151,   153,   154,   162,   174,   175,   176,
      71,   100,   267,   118,    76,   243,   243,   243,   118,   267,
     129,    92,    55,   183,   139,    71,   144,    71,   139,    71,
     245,   267,    26,   170,   172,   170,   170,    26,   170,   170,
     170,   118,   118,   130,   249,    92,    92,   232,   261,   174,
      92,   178,    92,   178,   178,   178,   178,   178,   178,    92,
      92,   178,   178,    92,   178,   178,    92,    92,    92,    61,
     176,   267,    71,    71,   269,   272,    66,   268,    59,    92,
     196,   197,   198,   201,   267,   272,   263,   272,   156,   168,
     129,    18,    22,    31,    37,    42,    47,    48,    52,    77,
      92,    93,    97,    98,   100,   119,   122,   127,   128,   141,
     142,   149,   150,   171,   184,   195,   203,   205,   210,   211,
     215,   216,   217,   219,   269,   270,   272,   273,   274,   275,
     267,   267,   245,   267,   267,   268,   133,   170,   170,   100,
     100,   269,   136,   235,   236,   270,   272,   233,   270,    29,
      17,    62,    25,   129,    25,   177,   177,   177,   276,   177,
     177,    25,    29,   202,   269,   267,   185,   201,   202,   168,
      72,    80,    89,   126,   202,    92,    92,   223,   160,   205,
     213,   274,   274,    92,   171,   205,   205,   205,    92,    92,
      92,    41,    71,    29,    56,    98,   100,   119,   271,   272,
      92,   118,   226,   227,   270,   249,   249,   249,   275,    41,
     129,    19,    34,    35,    38,    47,    48,    50,    57,    70,
      82,    83,    94,   109,   123,   137,   146,   147,   148,   157,
     158,   238,    41,   129,   234,    73,   120,   252,   253,   254,
     255,    61,   129,    92,   181,   182,   272,   179,   272,   179,
     129,   179,   179,    41,   180,   269,   161,   186,   202,    41,
     186,   129,   113,   199,    86,   199,   199,   205,   206,   266,
     205,   224,    41,    14,    66,    92,   205,   207,   214,   160,
     212,    60,   160,   205,   129,   205,   205,   205,   195,   185,
     271,   205,    84,   204,   205,   205,   204,   205,    26,    55,
     218,   100,   127,   270,    41,    71,   186,     5,   236,    64,
     241,    92,   240,   240,    92,    92,    92,   239,   239,   240,
     240,    14,   237,   270,   168,   263,   262,   272,    63,   253,
     182,   129,   180,   129,   180,   129,   129,   129,    25,   214,
     186,   201,   202,    86,    92,   196,   197,   200,    86,    86,
      41,   129,    41,   129,    92,    66,   210,   205,   214,     3,
       4,     5,     6,     7,     8,    13,    14,    17,    20,    21,
     208,   209,    15,    16,   145,   205,    60,   160,   205,   214,
      41,    41,    41,   129,   129,   186,   274,   205,   220,   227,
     185,   205,     5,   276,   276,   276,   276,    18,    92,    92,
     129,   181,   272,   129,   200,    92,   198,   201,   110,   200,
     200,   205,   205,   224,   210,   129,    26,    28,    26,    28,
      26,    28,    26,    28,    26,    28,    26,    28,    13,    20,
      21,    14,    18,   205,    92,   210,   205,   210,   214,   214,
     205,   145,   205,   205,    63,   145,   205,   275,   274,    74,
     187,    49,    99,   163,   129,   186,    44,   129,    41,    41,
     129,   257,   258,   259,   264,   272,   257,   110,   168,   129,
     214,   110,   110,   129,   205,    92,   210,    18,    16,   206,
     205,    63,   145,   205,   129,    41,   129,    36,    75,   188,
     114,    41,   276,   276,   129,    41,   238,   129,   214,   129,
     214,   214,    16,   206,   205,   129,   205,   275,   206,   214,
     111,   189,    92,    79,   129,   129,   125,   259,   132,   205,
     129,   129,    36,    90,   193,   117,   221,     5,   143,   238,
     256,   190,   191,   205,   194,   276,    36,   189,   274,    92,
     132,   132,    41,    30,    53,   192,    41,   206,   129,    41,
     258,   191,   194,    69,   129,   138,    36,   274,    41,    91,
     138,    36,   274
  };

  const unsigned short int
  parser::yyr1_[] =
  {
       0,   164,   165,   166,   166,   166,   166,   166,   166,   166,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   168,
     168,   169,   169,   169,   169,   169,   169,   169,   169,   170,
     170,   171,   172,   173,   173,   173,   174,   174,   175,   175,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   177,   177,
     178,   178,   178,   179,   179,   180,   180,   181,   181,   182,
     182,   183,   183,   184,   184,   185,   185,   186,   186,   187,
     187,   188,   188,   189,   189,   190,   190,   191,   192,   192,
     192,   193,   193,   193,   194,   195,   195,   195,   196,   196,
     197,   198,   198,   198,   198,   199,   199,   200,   200,   200,
     201,   201,   201,   202,   202,   202,   203,   203,   203,   203,
     203,   204,   204,   204,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   206,   206,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   208,   208,
     208,   208,   208,   208,   208,   208,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   210,   211,
     211,   212,   212,   213,   213,   214,   214,   214,   214,   215,
     215,   215,   215,   215,   215,   216,   216,   216,   216,   217,
     217,   217,   217,   217,   217,   217,   218,   218,   218,   219,
     219,   219,   219,   219,   219,   219,   219,   220,   220,   221,
     221,   222,   222,   223,   223,   224,   224,   225,   225,   226,
     226,   227,   228,   228,   228,   229,   230,   230,   230,   230,
     231,   231,   231,   232,   232,   233,   233,   234,   235,   235,
     236,   237,   237,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   239,   239,   240,   240,   241,
     241,   242,   242,   242,   243,   243,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   245,   245,   246,
     247,   247,   248,   248,   248,   248,   249,   249,   250,   251,
     252,   252,   253,   253,   254,   255,   255,   256,   257,   257,
     258,   258,   259,   260,   260,   261,   262,   263,   264,   265,
     266,   266,   267,   267,   268,   269,   270,   271,   272,   273,
     273,   274,   275,   276
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     4,     4,     3,     3,     3,     3,     1,
       1,     3,    11,     0,     3,     5,     0,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       2,     5,     5,     5,     5,     2,     5,     4,     0,     1,
       0,     2,     3,     1,     3,     0,     1,     1,     3,     1,
       3,     0,     1,     1,     3,     1,     3,     2,     0,     3,
       0,     2,     0,     0,     3,     1,     3,     2,     0,     1,
       1,     0,     4,     2,     1,     1,     2,     3,     2,     2,
       4,     7,     7,     7,     6,     0,     1,     1,     1,     3,
       1,     1,     1,     0,     1,     2,     1,     1,     3,     1,
       1,     3,     3,     3,     1,     2,     2,     3,     3,     3,
       3,     3,     3,     1,     1,     3,     3,     3,     4,     5,
       6,     5,     6,     3,     4,     2,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     6,
       5,     4,     5,     4,     5,     1,     3,     3,     3,     1,
       1,     3,     3,     1,     3,     1,     1,     1,     1,     5,
      10,     4,     6,     8,     6,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     0,
       3,     5,     4,     3,     5,     1,     3,     6,     8,     1,
       3,     3,     6,     7,     6,     2,     1,     1,     1,     1,
       7,     3,     7,     0,     3,     1,     3,     0,     1,     3,
       3,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     6,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     1,     1,     2,     0,     3,     0,     3,     0,
      17,     4,     4,     4,     0,     2,     4,     2,     4,     3,
       4,     4,     2,     4,     4,     2,     2,     2,     0,     2,
       2,     2,     4,     6,     6,     6,     0,     3,     2,     7,
       1,     2,     1,     1,     6,     8,     8,     4,     0,     1,
       1,     3,     2,     0,     2,     1,     1,     1,     1,     7,
       0,     1,     1,     3,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END", "error", "$undefined", "CMP_LE", "CMP_LT", "CMP_EQ", "CMP_GT",
  "CMP_GE", "CMP_NE", "UNION", "EXCEPT", "INTERSECT", "UMINUS", "LIKE",
  "NOT", "OR", "AND", "IS", "NULLX", "BOOL", "BETWEEN", "IN", "STRING",
  "IDENT", "NUMERIC", "QB_NAME_IDENT", "ALL", "ANALYZE", "ANY", "AS",
  "ASC", "AVG", "BEGIN_HINT", "BEGIN_OUTLINE_DATA", "BIGINT", "BINARY",
  "BY", "CASE", "CHAR", "COLUMN", "COLUMNS", "\",\"", "COUNT", "CREATE",
  "CSV", "DATABASE", "DATABASES", "DATE", "DATETIME", "DAY", "DECIMAL",
  "DELETE", "DENSE_RANK", "DESC", "DESCRIBE", "DISTINCT", "\"/\"",
  "DOUBLE", "DROP", "DUAL", "ELSE", "END_HINT", "END_OUTLINE_DATA",
  "END_SYM", "ENGINE", "EXEC", "EXISTS", "EXPLAIN", "EXPR_NORMALIZE",
  "FIELD", "FLOAT", "FROM", "FULL", "FUNCTION", "GROUP", "HAVING", "IF",
  "IFNULL", "INDEX", "INFILE", "INNER", "INSERT", "INT", "INTEGER",
  "INTERVAL", "INTO", "JOIN", "KILL", "LEADING", "LEFT", "LIMIT", "LINE",
  "\"(\"", "MAX", "MEDIUMINT", "MEMORY", "MERGE", "MIN", "\"-\"", "MONTH",
  "\"*\"", "NO_EXPR_NORMALIZE", "NO_MERGE", "NO_REWRITE", "NO_SIMPLIFY_SQ",
  "NO_UNNEST", "NO_USE_HASH", "NO_USE_NL", "NO_WIN_MAGIC", "NUMERIC_SYM",
  "ON", "ORDER", "ORDERED", "OUTER", "OVER", "PACKAGE", "PARALLEL",
  "PARTITION", "\".\"", "\"+\"", "PROCEDURE", "PROCESSLIST", "RANK",
  "REAL", "REPLACE", "RETURN", "RIGHT", "ROWID", "ROW_NUMBER", "\")\"",
  "SAMPLE", "SELECT", "\";\"", "SET", "SHOW", "SIMPLIFY_SQ", "SIZE",
  "SMALLINT", "SPLIT", "STATIS", "STATUS", "SUBSTR", "SUM", "TABLE",
  "TABLES", "THEN", "TIME", "TIMESTAMP_SYM", "TINYINT", "TO_CHAR",
  "TO_NUMBER", "UNNEST", "UPDATE", "USE_HASH", "USE_NL", "USING", "VALUES",
  "VARBINARY", "VARCHAR", "VIEW", "WHEN", "WHERE", "WIN_MAGIC", "YEAR",
  "$accept", "sql_stmt", "stmt", "cmd_stmt", "select_stmt", "set_select",
  "sub_set_select", "select_with_parens", "simple_select", "opt_hint",
  "opt_hint_list", "hint_list", "single_hint", "opt_qb_name",
  "opt_qb_name_single", "hint_table_list", "opt_split",
  "leading_hint_table", "leading_hint_table_list", "opt_distinct",
  "select_expr_list", "from_list", "opt_where", "opt_groupby",
  "opt_having", "opt_order_by", "order_by_list", "order_by",
  "opt_asc_desc", "opt_select_limit", "limit_expr", "projection",
  "basic_table_factor", "view_table_factor", "joined_table_factor",
  "opt_outer", "sub_table_factor", "table_factor", "opt_alias",
  "simple_expr", "seconds_expr", "arith_expr", "arith_expr_list",
  "cmp_expr", "cmp_type", "sq_cmp_type", "query_ref_expr",
  "case_when_expr", "when_then_list1", "when_then_list2", "logical_expr",
  "column_ref", "expr_const", "func_expr", "distinct_or_all", "win_type",
  "opt_arith_expr", "opt_partition_by", "insert_stmt", "insert_value_list",
  "insert_value", "update_stmt", "update_asgn_list", "update_asgn_factor",
  "delete_stmt", "explain_stmt", "explainable_stmt", "create_stmt",
  "opt_view_column_define", "view_column_define", "beg_view_define",
  "table_element_list", "column_definition", "opt_not_null", "data_type",
  "opt_time_precision", "opt_char_length", "opt_engine_def", "drop_stmt",
  "opt_if_exists", "show_stmt", "op_from_database", "use_stmt",
  "desc_stmt", "analyze_stmt", "opt_sample_size", "kill_stmt",
  "create_package_stmt", "pro_or_func_list", "pro_or_func",
  "procedure_define", "function_define", "table_type", "opt_param_list",
  "param_list", "param", "opt_replace", "package_name", "procedure_name",
  "function_name", "param_name", "exec_package_stmt",
  "opt_arith_expr_list", "relation_factor", "database_name",
  "relation_name", "column_name", "column_label", "ident", "datetime",
  "string", "number", "int_value", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   363,   363,   371,   372,   373,   374,   375,   376,   377,
     386,   387,   388,   389,   390,   391,   392,   393,   394,   398,
     402,   409,   414,   419,   424,   429,   434,   439,   444,   452,
     456,   463,   470,   491,   492,   498,   508,   511,   518,   525,
     536,   541,   546,   551,   556,   561,   566,   571,   576,   581,
     588,   594,   602,   610,   618,   626,   633,   643,   652,   653,
     660,   661,   665,   672,   677,   685,   686,   690,   696,   705,
     710,   718,   719,   723,   728,   736,   741,   749,   750,   754,
     755,   759,   760,   764,   765,   769,   774,   782,   789,   790,
     791,   795,   796,   803,   813,   820,   826,   832,   841,   846,
     857,   866,   870,   874,   878,   886,   889,   896,   900,   904,
     911,   915,   919,   926,   927,   931,   943,   947,   951,   955,
     959,   966,   976,   986,   999,  1003,  1008,  1013,  1018,  1023,
    1028,  1033,  1038,  1043,  1051,  1057,  1066,  1071,  1076,  1081,
    1086,  1091,  1102,  1113,  1118,  1123,  1127,  1132,  1140,  1144,
    1148,  1152,  1156,  1160,  1164,  1168,  1175,  1179,  1183,  1187,
    1191,  1195,  1199,  1203,  1207,  1211,  1215,  1219,  1226,  1235,
    1242,  1251,  1257,  1266,  1272,  1281,  1285,  1289,  1294,  1302,
    1309,  1315,  1323,  1330,  1338,  1349,  1358,  1371,  1380,  1392,
    1417,  1429,  1433,  1441,  1453,  1457,  1466,  1469,  1473,  1480,
    1484,  1488,  1492,  1496,  1500,  1504,  1508,  1516,  1519,  1527,
    1530,  1542,  1551,  1562,  1567,  1575,  1580,  1593,  1604,  1618,
    1624,  1633,  1648,  1659,  1670,  1688,  1697,  1698,  1699,  1700,
    1709,  1718,  1725,  1740,  1743,  1750,  1755,  1764,  1770,  1775,
    1783,  1793,  1796,  1803,  1805,  1807,  1809,  1811,  1813,  1815,
    1817,  1819,  1821,  1823,  1825,  1827,  1829,  1831,  1833,  1835,
    1837,  1839,  1841,  1843,  1845,  1850,  1851,  1862,  1863,  1875,
    1882,  1898,  1906,  1914,  1926,  1927,  1936,  1943,  1950,  1957,
    1964,  1971,  1975,  1979,  1987,  1995,  2001,  2010,  2015,  2021,
    2036,  2043,  2057,  2066,  2075,  2084,  2096,  2097,  2104,  2119,
    2135,  2140,  2148,  2152,  2159,  2168,  2176,  2188,  2196,  2199,
    2206,  2211,  2219,  2228,  2229,  2233,  2237,  2241,  2245,  2249,
    2264,  2267,  2279,  2287,  2298,  2302,  2306,  2310,  2314,  2318,
    2319,  2323,  2327,  2331
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


#line 16 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1167
} // CatDB
#line 5104 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:1167
#line 2335 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
