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

      case 237: // column_definition
      case 258: // param
        value.move< ColumnDefineStmt_s > (that.value);
        break;

      case 239: // data_type
        value.move< DataType > (that.value);
        break;

      case 192: // order_by
      case 196: // projection
      case 204: // simple_expr
      case 205: // seconds_expr
      case 206: // arith_expr
      case 208: // cmp_expr
      case 211: // query_ref_expr
      case 212: // case_when_expr
      case 215: // logical_expr
      case 216: // column_ref
      case 217: // expr_const
      case 218: // func_expr
      case 221: // opt_arith_expr
      case 228: // update_asgn_factor
        value.move< ExprStmt_s > (that.value);
        break;

      case 252: // pro_or_func
      case 253: // procedure_define
      case 254: // function_define
        value.move< FunctionDefinition_s > (that.value);
        break;

      case 174: // opt_hint
        value.move< Hint > (that.value);
        break;

      case 177: // single_hint
        value.move< HintStmt_s > (that.value);
        break;

      case 182: // leading_hint_table
        value.move< LeadingTable_s > (that.value);
        break;

      case 194: // opt_select_limit
        value.move< LimitStmt_s > (that.value);
        break;

      case 209: // cmp_type
      case 210: // sq_cmp_type
        value.move< OperationType > (that.value);
        break;

      case 166: // sql_stmt
      case 167: // stmt
      case 168: // cmd_stmt
      case 169: // select_stmt
      case 170: // set_select
      case 171: // sub_set_select
      case 172: // select_with_parens
      case 173: // simple_select
      case 223: // insert_stmt
      case 226: // update_stmt
      case 229: // delete_stmt
      case 230: // explain_stmt
      case 231: // explainable_stmt
      case 232: // create_stmt
      case 243: // drop_stmt
      case 245: // show_stmt
      case 247: // use_stmt
      case 248: // desc_stmt
      case 249: // kill_stmt
      case 250: // create_package_stmt
      case 264: // exec_package_stmt
        value.move< Stmt_s > (that.value);
        break;

      case 197: // basic_table_factor
      case 198: // view_table_factor
      case 199: // joined_table_factor
      case 201: // sub_table_factor
      case 202: // table_factor
        value.move< TableStmt_s > (that.value);
        break;

      case 236: // table_element_list
      case 255: // table_type
      case 256: // opt_param_list
      case 257: // param_list
        value.move< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 185: // select_expr_list
      case 187: // opt_where
      case 188: // opt_groupby
      case 189: // opt_having
      case 190: // opt_order_by
      case 191: // order_by_list
      case 207: // arith_expr_list
      case 213: // when_then_list1
      case 214: // when_then_list2
      case 222: // opt_partition_by
      case 225: // insert_value
      case 227: // update_asgn_list
      case 266: // opt_arith_expr_list
        value.move< Vector<ExprStmt_s> > (that.value);
        break;

      case 251: // pro_or_func_list
        value.move< Vector<FunctionDefinition_s> > (that.value);
        break;

      case 175: // opt_hint_list
      case 176: // hint_list
        value.move< Vector<HintStmt_s> > (that.value);
        break;

      case 183: // leading_hint_table_list
        value.move< Vector<LeadingTable_s> > (that.value);
        break;

      case 180: // hint_table_list
      case 233: // opt_view_column_define
      case 234: // view_column_define
      case 242: // opt_engine_def
        value.move< Vector<String> > (that.value);
        break;

      case 186: // from_list
        value.move< Vector<TableStmt_s> > (that.value);
        break;

      case 224: // insert_value_list
        value.move< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 220: // win_type
        value.move< WinType > (that.value);
        break;

      case 181: // opt_split
      case 184: // opt_distinct
      case 193: // opt_asc_desc
      case 200: // opt_outer
      case 219: // distinct_or_all
      case 238: // opt_not_null
      case 244: // opt_if_exists
      case 259: // opt_replace
      case 265: // start_exec_package
        value.move< bool > (that.value);
        break;

      case 195: // limit_expr
      case 240: // opt_time_precision
      case 241: // opt_char_length
      case 276: // int_value
        value.move< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 178: // opt_qb_name
      case 179: // opt_qb_name_single
      case 203: // opt_alias
      case 235: // beg_view_define
      case 246: // op_from_database
      case 260: // package_name
      case 261: // procedure_name
      case 262: // function_name
      case 263: // param_name
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

      case 237: // column_definition
      case 258: // param
        value.copy< ColumnDefineStmt_s > (that.value);
        break;

      case 239: // data_type
        value.copy< DataType > (that.value);
        break;

      case 192: // order_by
      case 196: // projection
      case 204: // simple_expr
      case 205: // seconds_expr
      case 206: // arith_expr
      case 208: // cmp_expr
      case 211: // query_ref_expr
      case 212: // case_when_expr
      case 215: // logical_expr
      case 216: // column_ref
      case 217: // expr_const
      case 218: // func_expr
      case 221: // opt_arith_expr
      case 228: // update_asgn_factor
        value.copy< ExprStmt_s > (that.value);
        break;

      case 252: // pro_or_func
      case 253: // procedure_define
      case 254: // function_define
        value.copy< FunctionDefinition_s > (that.value);
        break;

      case 174: // opt_hint
        value.copy< Hint > (that.value);
        break;

      case 177: // single_hint
        value.copy< HintStmt_s > (that.value);
        break;

      case 182: // leading_hint_table
        value.copy< LeadingTable_s > (that.value);
        break;

      case 194: // opt_select_limit
        value.copy< LimitStmt_s > (that.value);
        break;

      case 209: // cmp_type
      case 210: // sq_cmp_type
        value.copy< OperationType > (that.value);
        break;

      case 166: // sql_stmt
      case 167: // stmt
      case 168: // cmd_stmt
      case 169: // select_stmt
      case 170: // set_select
      case 171: // sub_set_select
      case 172: // select_with_parens
      case 173: // simple_select
      case 223: // insert_stmt
      case 226: // update_stmt
      case 229: // delete_stmt
      case 230: // explain_stmt
      case 231: // explainable_stmt
      case 232: // create_stmt
      case 243: // drop_stmt
      case 245: // show_stmt
      case 247: // use_stmt
      case 248: // desc_stmt
      case 249: // kill_stmt
      case 250: // create_package_stmt
      case 264: // exec_package_stmt
        value.copy< Stmt_s > (that.value);
        break;

      case 197: // basic_table_factor
      case 198: // view_table_factor
      case 199: // joined_table_factor
      case 201: // sub_table_factor
      case 202: // table_factor
        value.copy< TableStmt_s > (that.value);
        break;

      case 236: // table_element_list
      case 255: // table_type
      case 256: // opt_param_list
      case 257: // param_list
        value.copy< Vector<ColumnDefineStmt_s> > (that.value);
        break;

      case 185: // select_expr_list
      case 187: // opt_where
      case 188: // opt_groupby
      case 189: // opt_having
      case 190: // opt_order_by
      case 191: // order_by_list
      case 207: // arith_expr_list
      case 213: // when_then_list1
      case 214: // when_then_list2
      case 222: // opt_partition_by
      case 225: // insert_value
      case 227: // update_asgn_list
      case 266: // opt_arith_expr_list
        value.copy< Vector<ExprStmt_s> > (that.value);
        break;

      case 251: // pro_or_func_list
        value.copy< Vector<FunctionDefinition_s> > (that.value);
        break;

      case 175: // opt_hint_list
      case 176: // hint_list
        value.copy< Vector<HintStmt_s> > (that.value);
        break;

      case 183: // leading_hint_table_list
        value.copy< Vector<LeadingTable_s> > (that.value);
        break;

      case 180: // hint_table_list
      case 233: // opt_view_column_define
      case 234: // view_column_define
      case 242: // opt_engine_def
        value.copy< Vector<String> > (that.value);
        break;

      case 186: // from_list
        value.copy< Vector<TableStmt_s> > (that.value);
        break;

      case 224: // insert_value_list
        value.copy< Vector<Vector<ExprStmt_s>> > (that.value);
        break;

      case 220: // win_type
        value.copy< WinType > (that.value);
        break;

      case 181: // opt_split
      case 184: // opt_distinct
      case 193: // opt_asc_desc
      case 200: // opt_outer
      case 219: // distinct_or_all
      case 238: // opt_not_null
      case 244: // opt_if_exists
      case 259: // opt_replace
      case 265: // start_exec_package
        value.copy< bool > (that.value);
        break;

      case 195: // limit_expr
      case 240: // opt_time_precision
      case 241: // opt_char_length
      case 276: // int_value
        value.copy< int > (that.value);
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 178: // opt_qb_name
      case 179: // opt_qb_name_single
      case 203: // opt_alias
      case 235: // beg_view_define
      case 246: // op_from_database
      case 260: // package_name
      case 261: // procedure_name
      case 262: // function_name
      case 263: // param_name
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

      case 237: // column_definition
      case 258: // param
        yylhs.value.build< ColumnDefineStmt_s > ();
        break;

      case 239: // data_type
        yylhs.value.build< DataType > ();
        break;

      case 192: // order_by
      case 196: // projection
      case 204: // simple_expr
      case 205: // seconds_expr
      case 206: // arith_expr
      case 208: // cmp_expr
      case 211: // query_ref_expr
      case 212: // case_when_expr
      case 215: // logical_expr
      case 216: // column_ref
      case 217: // expr_const
      case 218: // func_expr
      case 221: // opt_arith_expr
      case 228: // update_asgn_factor
        yylhs.value.build< ExprStmt_s > ();
        break;

      case 252: // pro_or_func
      case 253: // procedure_define
      case 254: // function_define
        yylhs.value.build< FunctionDefinition_s > ();
        break;

      case 174: // opt_hint
        yylhs.value.build< Hint > ();
        break;

      case 177: // single_hint
        yylhs.value.build< HintStmt_s > ();
        break;

      case 182: // leading_hint_table
        yylhs.value.build< LeadingTable_s > ();
        break;

      case 194: // opt_select_limit
        yylhs.value.build< LimitStmt_s > ();
        break;

      case 209: // cmp_type
      case 210: // sq_cmp_type
        yylhs.value.build< OperationType > ();
        break;

      case 166: // sql_stmt
      case 167: // stmt
      case 168: // cmd_stmt
      case 169: // select_stmt
      case 170: // set_select
      case 171: // sub_set_select
      case 172: // select_with_parens
      case 173: // simple_select
      case 223: // insert_stmt
      case 226: // update_stmt
      case 229: // delete_stmt
      case 230: // explain_stmt
      case 231: // explainable_stmt
      case 232: // create_stmt
      case 243: // drop_stmt
      case 245: // show_stmt
      case 247: // use_stmt
      case 248: // desc_stmt
      case 249: // kill_stmt
      case 250: // create_package_stmt
      case 264: // exec_package_stmt
        yylhs.value.build< Stmt_s > ();
        break;

      case 197: // basic_table_factor
      case 198: // view_table_factor
      case 199: // joined_table_factor
      case 201: // sub_table_factor
      case 202: // table_factor
        yylhs.value.build< TableStmt_s > ();
        break;

      case 236: // table_element_list
      case 255: // table_type
      case 256: // opt_param_list
      case 257: // param_list
        yylhs.value.build< Vector<ColumnDefineStmt_s> > ();
        break;

      case 185: // select_expr_list
      case 187: // opt_where
      case 188: // opt_groupby
      case 189: // opt_having
      case 190: // opt_order_by
      case 191: // order_by_list
      case 207: // arith_expr_list
      case 213: // when_then_list1
      case 214: // when_then_list2
      case 222: // opt_partition_by
      case 225: // insert_value
      case 227: // update_asgn_list
      case 266: // opt_arith_expr_list
        yylhs.value.build< Vector<ExprStmt_s> > ();
        break;

      case 251: // pro_or_func_list
        yylhs.value.build< Vector<FunctionDefinition_s> > ();
        break;

      case 175: // opt_hint_list
      case 176: // hint_list
        yylhs.value.build< Vector<HintStmt_s> > ();
        break;

      case 183: // leading_hint_table_list
        yylhs.value.build< Vector<LeadingTable_s> > ();
        break;

      case 180: // hint_table_list
      case 233: // opt_view_column_define
      case 234: // view_column_define
      case 242: // opt_engine_def
        yylhs.value.build< Vector<String> > ();
        break;

      case 186: // from_list
        yylhs.value.build< Vector<TableStmt_s> > ();
        break;

      case 224: // insert_value_list
        yylhs.value.build< Vector<Vector<ExprStmt_s>> > ();
        break;

      case 220: // win_type
        yylhs.value.build< WinType > ();
        break;

      case 181: // opt_split
      case 184: // opt_distinct
      case 193: // opt_asc_desc
      case 200: // opt_outer
      case 219: // distinct_or_all
      case 238: // opt_not_null
      case 244: // opt_if_exists
      case 259: // opt_replace
      case 265: // start_exec_package
        yylhs.value.build< bool > ();
        break;

      case 195: // limit_expr
      case 240: // opt_time_precision
      case 241: // opt_char_length
      case 276: // int_value
        yylhs.value.build< int > ();
        break;

      case 22: // STRING
      case 23: // IDENT
      case 24: // NUMERIC
      case 25: // QB_NAME_IDENT
      case 178: // opt_qb_name
      case 179: // opt_qb_name_single
      case 203: // opt_alias
      case 235: // beg_view_define
      case 246: // op_from_database
      case 260: // package_name
      case 261: // procedure_name
      case 262: // function_name
      case 263: // param_name
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
#line 1159 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 371 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1165 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 372 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1171 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 373 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1177 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 374 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1183 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 375 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1189 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 376 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1195 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 377 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yyerror("unknow SQL"); }
#line 1201 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 386 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1207 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 387 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1213 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 388 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1219 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 389 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1225 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 390 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1231 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 391 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1237 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 392 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1243 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 393 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 1249 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 398 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1257 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 402 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1265 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 409 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1274 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 414 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION);
    }
#line 1283 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 419 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1292 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 424 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建union all二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[3].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), UNION_ALL);
    }
#line 1301 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 429 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1310 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 434 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建intersect二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), INTERSECT);
    }
#line 1319 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 439 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1328 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 444 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建except二元表达式
		yylhs.value.as< Stmt_s > () = SetStmt::make_set_stmt(yystack_[2].value.as< Stmt_s > (), yystack_[0].value.as< Stmt_s > (), EXCEPT);
    }
#line 1337 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 452 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1345 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 456 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
	}
#line 1353 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 463 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[1].value.as< Stmt_s > ();
	}
#line 1361 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 473 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 1380 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 490 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Hint > () = Hint(); }
#line 1386 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 492 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[1].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = false;
	}
#line 1396 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 498 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Hint > () = Hint();
		yylhs.value.as< Hint > ().all_hints = yystack_[2].value.as< Vector<HintStmt_s> > ();
		yylhs.value.as< Hint > ().is_outline = true;
	}
#line 1406 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 507 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
	}
#line 1414 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 511 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[0].value.as< Vector<HintStmt_s> > ();
	}
#line 1422 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 518 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = Vector<HintStmt_s>();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1433 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 525 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<HintStmt_s> > () = yystack_[1].value.as< Vector<HintStmt_s> > ();
		if (yystack_[0].value.as< HintStmt_s > ()) {
			yylhs.value.as< Vector<HintStmt_s> > ().push_back(yystack_[0].value.as< HintStmt_s > ());
		}
	}
#line 1444 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 536 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(NO_REWRITE);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1453 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 541 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1462 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 546 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(UNNEST, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1471 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 551 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1480 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 556 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(MERGE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1489 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 561 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1498 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 566 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(SIMPLIFY_SQ, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1507 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 571 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1516 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 576 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(EXPR_NORMALIZE, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1525 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 581 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(WIN_MAGIC, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[3].value.as< std::string > ());
		WinMagicHintStmt_s win_magic_hint = yylhs.value.as< HintStmt_s > ();
		win_magic_hint->dst_qb_name = yystack_[1].value.as< std::string > ();
	}
#line 1536 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 588 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(WIN_MAGIC, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
	}
#line 1545 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 594 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1557 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 602 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(HASH_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1569 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 610 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, true);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1581 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 618 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(USE_JOIN, false);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		JoinHintStmt_s join_hint = yylhs.value.as< HintStmt_s > ();
		join_hint->set_join_algo(NL_JOIN);
		join_hint->table_names = yystack_[1].value.as< Vector<String> > ();
	}
#line 1593 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 626 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[0].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->set_is_ordered();
	}
#line 1604 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 633 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(LEADING);
		yylhs.value.as< HintStmt_s > ()->set_qb_name(yystack_[2].value.as< std::string > ());
		LeadingHintStmt_s leading = yylhs.value.as< HintStmt_s > ();
		leading->tables = LeadingTable::make_leading_table();
		leading->tables->is_base_table = false;
		leading->tables->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1617 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 643 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< HintStmt_s > () = HintStmt::make_hint_stmt(PARALLEL);
		ParallelHintStmt_s hint = yylhs.value.as< HintStmt_s > ();
		hint->parallel = yystack_[1].value.as< int > ();
	}
#line 1627 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 651 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1633 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 653 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 1641 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 659 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1647 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 661 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = "";
	}
#line 1655 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 665 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > ();
	}
#line 1663 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 672 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1672 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 677 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 1681 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 684 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1687 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 685 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {}
#line 1693 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 690 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = true;
		yylhs.value.as< LeadingTable_s > ()->table_name = yystack_[0].value.as< std::string > ();
	}
#line 1703 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 696 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< LeadingTable_s > () = LeadingTable::make_leading_table();
		yylhs.value.as< LeadingTable_s > ()->is_base_table = false;
		yylhs.value.as< LeadingTable_s > ()->table_list = yystack_[1].value.as< Vector<LeadingTable_s> > ();
	}
#line 1713 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 705 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = Vector<LeadingTable_s>();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1722 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 710 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<LeadingTable_s> > () = yystack_[2].value.as< Vector<LeadingTable_s> > ();
		yylhs.value.as< Vector<LeadingTable_s> > ().push_back(yystack_[0].value.as< LeadingTable_s > ());
	}
#line 1731 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 717 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1737 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 718 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1743 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 723 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 1752 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 728 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yystack_[2].value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
    }
#line 1761 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 736 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = Vector<TableStmt_s>();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1770 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 741 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<TableStmt_s> > () = yystack_[2].value.as< Vector<TableStmt_s> > ();
		yylhs.value.as< Vector<TableStmt_s> > ().push_back(yystack_[0].value.as< TableStmt_s > ());
	}
#line 1779 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 748 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1785 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 749 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1791 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 753 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1797 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 754 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1803 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 758 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ()); }
#line 1809 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 759 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1815 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 763 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>(); }
#line 1821 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 83:
#line 764 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > (); }
#line 1827 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 84:
#line 769 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 1836 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 85:
#line 774 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 1845 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 86:
#line 782 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OrderStmt::make_order_stmt(yystack_[1].value.as< ExprStmt_s > (), yystack_[0].value.as< bool > ());
    }
#line 1853 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 87:
#line 788 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1859 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 88:
#line 789 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1865 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 89:
#line 790 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1871 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 90:
#line 794 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {yylhs.value.as< LimitStmt_s > () = NULL;}
#line 1877 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 91:
#line 796 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > (), yystack_[2].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
    }
#line 1888 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 92:
#line 803 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建limit表达式
		LimitStmt_s stmt = LimitStmt::make_limit_stmt(yystack_[0].value.as< int > ());
		check(stmt);
		yylhs.value.as< LimitStmt_s > () = stmt;
	}
#line 1899 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 93:
#line 813 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = yystack_[0].value.as< int > ();
	}
#line 1907 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 94:
#line 820 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< ExprStmt_s > ()->to_string();
    }
#line 1917 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 95:
#line 826 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1927 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 96:
#line 832 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表达式别名
		yylhs.value.as< ExprStmt_s > () = yystack_[2].value.as< ExprStmt_s > ();
		yylhs.value.as< ExprStmt_s > ()->alias_name = yystack_[0].value.as< std::string > ();
    }
#line 1937 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 97:
#line 841 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< BasicTableStmt_s > ();
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
    }
#line 1946 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 98:
#line 846 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		TableStmt_s table = BasicTableStmt::make_dual_table();
		check(table);
		yylhs.value.as< TableStmt_s > () = table;
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1958 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 99:
#line 857 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//设置表的别名
		yylhs.value.as< TableStmt_s > () = ViewTableStmt::make_view_table(yystack_[2].value.as< Stmt_s > ());
		yylhs.value.as< TableStmt_s > ()->set_alias_name(yystack_[0].value.as< std::string > ());
	}
#line 1968 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 100:
#line 866 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), LeftOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1976 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 101:
#line 870 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), RightOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1984 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 102:
#line 874 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[6].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), FullOuter, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 1992 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 103:
#line 878 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	   yylhs.value.as< TableStmt_s > () = JoinedTableStmt::make_joined_table(yystack_[5].value.as< TableStmt_s > (), yystack_[2].value.as< TableStmt_s > (), Inner, yystack_[0].value.as< ExprStmt_s > ());
   }
#line 2000 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 104:
#line 885 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 2008 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 105:
#line 889 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 2016 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 106:
#line 896 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 2024 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 107:
#line 900 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
	}
#line 2032 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 108:
#line 904 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< TableStmt_s > () = yystack_[1].value.as< TableStmt_s > ();
	}
#line 2040 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 109:
#line 911 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2048 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 110:
#line 915 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2056 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 111:
#line 919 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  yylhs.value.as< TableStmt_s > () = yystack_[0].value.as< TableStmt_s > ();
  }
#line 2064 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 112:
#line 925 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 2070 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 113:
#line 927 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2078 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 114:
#line 931 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 2086 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 115:
#line 943 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2094 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 116:
#line 947 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		 yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2102 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 117:
#line 951 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2110 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 118:
#line 955 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
      	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2118 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 119:
#line 959 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2126 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 120:
#line 966 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_day(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2140 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 121:
#line 976 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_month(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2154 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 122:
#line 986 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  //构建常量表达式
		Number_s value;
		DateTime::make_second_from_year(std::stoi(yystack_[1].value.as< std::string > ()), value);
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
  }
#line 2168 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 123:
#line 999 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2176 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 124:
#line 1003 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//正数表达式
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
    }
#line 2185 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 125:
#line 1008 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//负数表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MINUS);
    }
#line 2194 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 126:
#line 1013 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_ADD);
	}
#line 2203 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 127:
#line 1018 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_SUB);
	}
#line 2212 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 128:
#line 1023 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_MUL);
	}
#line 2221 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 129:
#line 1028 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DIV);
	}
#line 2230 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 130:
#line 1033 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_ADD);
  }
#line 2239 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 131:
#line 1038 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	  	//构建二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_DATE_SUB);
  }
#line 2248 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 132:
#line 1043 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2256 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 133:
#line 1051 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2266 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 134:
#line 1057 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 2276 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 135:
#line 1066 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2285 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 136:
#line 1071 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), OP_IS_NULL);
    }
#line 2294 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 137:
#line 1076 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建is not null表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), OP_IS_NOT_NULL);
    }
#line 2303 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 138:
#line 1081 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[4].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_BETWEEN);
    }
#line 2312 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 139:
#line 1086 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not between and三元表达式
		make_ternary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[5].value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_BETWEEN);
    }
#line 2321 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 140:
#line 1091 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2336 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 141:
#line 1102 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2351 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 142:
#line 1113 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_IN);
    }
#line 2360 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 143:
#line 1118 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not in表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_IN);
    }
#line 2369 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 144:
#line 1123 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
    	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_EXISTS);
    }
#line 2377 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 145:
#line 1127 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建not一元表达式
		make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_NOT_EXISTS);
    }
#line 2386 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 146:
#line 1132 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建比较二元表达式 
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), yystack_[1].value.as< OperationType > ());
	}
#line 2395 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 147:
#line 1140 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE;
	}
#line 2403 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 148:
#line 1144 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT;
	}
#line 2411 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 149:
#line 1148 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ;
	}
#line 2419 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 150:
#line 1152 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE;
	}
#line 2427 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 151:
#line 1156 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT;
	}
#line 2435 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 152:
#line 1160 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE;
	}
#line 2443 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 153:
#line 1164 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LIKE;
	}
#line 2451 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 154:
#line 1168 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NOT_LIKE;
	}
#line 2459 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 155:
#line 1175 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ANY;
	}
#line 2467 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 156:
#line 1179 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ANY;
	}
#line 2475 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 157:
#line 1183 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ANY;
	}
#line 2483 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 158:
#line 1187 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ANY;
	}
#line 2491 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 159:
#line 1191 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ANY;
	}
#line 2499 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 160:
#line 1195 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ANY;
	}
#line 2507 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 161:
#line 1199 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LE_ALL;
	}
#line 2515 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 162:
#line 1203 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_LT_ALL;
	}
#line 2523 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 163:
#line 1207 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_EQ_ALL;
	}
#line 2531 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 164:
#line 1211 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GE_ALL;
	}
#line 2539 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 165:
#line 1215 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_GT_ALL;
	}
#line 2547 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 166:
#line 1219 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< OperationType > () = OP_NE_ALL;
	}
#line 2555 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 167:
#line 1226 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		SubQueryStmt_s query_expr = SubQueryStmt::make_query_stmt();
		query_expr->query_stmt = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< ExprStmt_s > () = query_expr;
    }
#line 2565 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 168:
#line 1235 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[4].value.as< ExprStmt_s > ());
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2576 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 169:
#line 1242 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = OpExprStmt::make_op_expr_stmt(OP_CASE_WHEN);
		append(yylhs.value.as< ExprStmt_s > ()->params, yystack_[3].value.as< Vector<ExprStmt_s> > ());
		yylhs.value.as< ExprStmt_s > ()->params.push_back(yystack_[1].value.as< ExprStmt_s > ());
	}
#line 2586 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 170:
#line 1251 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2596 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 171:
#line 1257 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2606 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 172:
#line 1266 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2616 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 173:
#line 1272 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[4].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[2].value.as< ExprStmt_s > ());
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 2626 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 174:
#line 1281 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2634 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 175:
#line 1285 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[1].value.as< ExprStmt_s > ();
	}
#line 2642 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 176:
#line 1289 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建and二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_AND);
    }
#line 2651 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 177:
#line 1294 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建or二元表达式
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[2].value.as< ExprStmt_s > (), yystack_[0].value.as< ExprStmt_s > (), OP_OR);
    }
#line 2660 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 178:
#line 1302 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2671 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 179:
#line 1309 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ExprStmt_s col = ColumnStmt::make_all_column_stmt();
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2681 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 180:
#line 1315 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2692 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 181:
#line 1323 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "*");
		check(col);
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2703 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 182:
#line 1330 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt("", "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
	}
#line 2715 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 183:
#line 1338 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ColumnStmt_s col = ColumnStmt::make_column_stmt(yystack_[2].value.as< std::string > (), "ROWID");
		check(col);
		col->is_row_id = true;
		yylhs.value.as< ExprStmt_s > () = col;
    }
#line 2727 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 184:
#line 1349 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Varchar::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2740 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 185:
#line 1358 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2757 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 186:
#line 1371 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Number::make_object(yystack_[0].value.as< std::string > ());
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
    }
#line 2770 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 187:
#line 1380 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建常量表达式
		Object_s value = Object::make_null_object();
		check(value);
		ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
		check(stmt);
		yylhs.value.as< ExprStmt_s > () = stmt;
	}
#line 2783 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 188:
#line 1392 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2812 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 189:
#line 1417 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2828 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 190:
#line 1429 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_CHAR);
  }
#line 2836 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 191:
#line 1433 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	Object_s value = Varchar::make_object(yystack_[1].value.as< std::string > ());
	check(value);
	ExprStmt_s stmt = ConstStmt::make_const_stmt(value);
	check(stmt);
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), stmt, OP_TO_CHAR);
  }
#line 2848 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 192:
#line 1441 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 2864 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 193:
#line 1453 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_binary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[3].value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_IFNULL);
  }
#line 2872 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 194:
#line 1457 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
	make_unary_stmt(yylhs.value.as< ExprStmt_s > (), yystack_[1].value.as< ExprStmt_s > (), OP_TO_NUMBER);
  }
#line 2880 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 195:
#line 1465 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
	}
#line 2888 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 196:
#line 1469 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = false;
    }
#line 2896 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 197:
#line 1473 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< bool > () = true;
    }
#line 2904 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 198:
#line 1480 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_COUNT;
	}
#line 2912 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 199:
#line 1484 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_SUM;
	}
#line 2920 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 200:
#line 1488 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_AVG;
	}
#line 2928 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 201:
#line 1492 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_MIN;
	}
#line 2936 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 202:
#line 1496 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_MAX;
	}
#line 2944 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 203:
#line 1500 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_RANK;
	}
#line 2952 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 204:
#line 1504 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_DENSE_RANK;
	}
#line 2960 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 205:
#line 1508 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< WinType > () = WIN_ROW_NUMBER;
	}
#line 2968 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 206:
#line 1515 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = ExprStmt_s();
	}
#line 2976 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 207:
#line 1519 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< ExprStmt_s > () = yystack_[0].value.as< ExprStmt_s > ();
	}
#line 2984 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 208:
#line 1526 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
	}
#line 2992 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 209:
#line 1530 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > ();
	}
#line 3000 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 210:
#line 1542 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建insert stmt
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[2].value.as< BasicTableStmt_s > ();
		insert_stmt->value_list = yystack_[0].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 3013 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 211:
#line 1551 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		InsertStmt_s insert_stmt = InsertStmt::make_insert_stmt();
		check(insert_stmt);
		insert_stmt->table = yystack_[1].value.as< BasicTableStmt_s > ();
		insert_stmt->query_values = yystack_[0].value.as< Stmt_s > ();
		yylhs.value.as< Stmt_s > () = insert_stmt;
    }
#line 3025 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 212:
#line 1562 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = Vector<Vector<ExprStmt_s>>();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
    }
#line 3034 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 213:
#line 1567 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > () = yystack_[4].value.as< Vector<Vector<ExprStmt_s>> > ();
		yylhs.value.as< Vector<Vector<ExprStmt_s>> > ().push_back(yystack_[1].value.as< Vector<ExprStmt_s> > ());
	}
#line 3043 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 214:
#line 1575 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
	}
#line 3052 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 215:
#line 1580 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3061 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 216:
#line 1593 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3076 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 217:
#line 1604 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3091 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 218:
#line 1618 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建值列表
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3101 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 219:
#line 1624 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//将新的表达式加入到表达式列表
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[2].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Vector<ExprStmt_s> > ().push_back(yystack_[0].value.as< ExprStmt_s > ());
    }
#line 3111 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 220:
#line 1633 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建列引用表达式
		ExprStmt_s col = ColumnStmt::make_column_stmt("", yystack_[2].value.as< std::string > ());
		check(col);
		make_binary_stmt(yylhs.value.as< ExprStmt_s > (), col, yystack_[0].value.as< ExprStmt_s > (), OP_ASSIGN);
    }
#line 3122 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 221:
#line 1648 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3137 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 222:
#line 1659 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3152 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 223:
#line 1670 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		DeleteStmt_s delete_stmt = DeleteStmt::make_delete_stmt();
		check(delete_stmt);
		delete_stmt->stmt_hint = yystack_[4].value.as< Hint > ();
		delete_stmt->table = yystack_[3].value.as< BasicTableStmt_s > ();
		delete_stmt->from_stmts = yystack_[1].value.as< Vector<TableStmt_s> > ();
		delete_stmt->where_stmt = yystack_[0].value.as< Vector<ExprStmt_s> > ();
		yylhs.value.as< Stmt_s > () = delete_stmt;
    }
#line 3166 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 224:
#line 1688 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > ();
		DMLStmt_s dml = yylhs.value.as< Stmt_s > ();
		dml->is_explain = true;
    }
#line 3176 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 225:
#line 1696 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3182 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 226:
#line 1697 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3188 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 227:
#line 1698 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3194 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 228:
#line 1699 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< Stmt_s > () = yystack_[0].value.as< Stmt_s > (); }
#line 3200 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 229:
#line 1709 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateTable);
		check(cmd_stmt);
		CreateTableParam_s param = CreateTableParam::make_create_table_param();
		param->database_name = yystack_[4].value.as< BasicTableStmt_s > ()->database;
		param->table_name = yystack_[4].value.as< BasicTableStmt_s > ()->table_name;
		param->column_define_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		param->engine_args = yystack_[0].value.as< Vector<String> > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3216 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 230:
#line 1721 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateDatabase);
		check(cmd_stmt);
		CreateDBParam_s param = CreateDBParam::make_create_DB_param();
		param->database = yystack_[0].value.as< std::string > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3229 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 231:
#line 1730 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(CreateView);
		check(cmd_stmt);
		CreateViewParam_s param = CreateViewParam::make_create_view_param();
		param->database = driver.get_global_database();
		param->view_name = yystack_[4].value.as< std::string > ();
		param->column_define = yystack_[3].value.as< Vector<String> > ();
		param->view_define_sql = yystack_[1].value.as< std::string > ();
		param->ref_query = yystack_[0].value.as< Stmt_s > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3246 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 232:
#line 1746 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
	}
#line 3254 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 233:
#line 1750 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[1].value.as< Vector<String> > ();
	}
#line 3262 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 234:
#line 1757 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3271 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 235:
#line 1762 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = yystack_[2].value.as< Vector<String> > ();
		yylhs.value.as< Vector<String> > ().push_back(yystack_[0].value.as< std::string > ());
	}
#line 3280 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 236:
#line 1770 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.left_string();
	}
#line 3288 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 237:
#line 1777 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3297 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 238:
#line 1782 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
    }
#line 3306 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 239:
#line 1790 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[2].value.as< std::string > (), yystack_[1].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
    }
#line 3316 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 240:
#line 1799 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {

	}
#line 3324 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 241:
#line 1803 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		
	}
#line 3332 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 242:
#line 1810 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3338 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 243:
#line 1812 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3344 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 244:
#line 1814 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_MEDIUMINT_PREC, 0); }
#line 3350 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 245:
#line 1816 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3356 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 246:
#line 1818 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_INT_PREC, 0); }
#line 3362 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 247:
#line 1820 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_BIGINT_PREC, 0); }
#line 3368 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 248:
#line 1822 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3374 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 249:
#line 1824 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3380 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 250:
#line 1826 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3386 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 251:
#line 1828 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3392 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 252:
#line 1830 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3398 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 253:
#line 1832 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, yystack_[3].value.as< int > (), yystack_[1].value.as< int > ()); }
#line 3404 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 254:
#line 1834 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_SMALLINT_PREC, 0); }
#line 3410 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 255:
#line 1836 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3416 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 256:
#line 1838 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3422 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 257:
#line 1840 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_NUMBER, MAX_PREC, MAX_NUM_SCALE); }
#line 3428 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 258:
#line 1842 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3434 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 259:
#line 1844 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3440 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 260:
#line 1846 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3446 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 261:
#line 1848 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_VARCHAR, yystack_[0].value.as< int > ()); }
#line 3452 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 262:
#line 1850 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIMESTAMP, yystack_[0].value.as< int > ()); }
#line 3458 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 263:
#line 1852 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATETIME); }
#line 3464 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 264:
#line 1854 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, DATE); }
#line 3470 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 265:
#line 1856 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< DataType > () = DataType(T_DATETIME, TIME); }
#line 3476 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 266:
#line 1860 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_TIME_SCALE; }
#line 3482 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 267:
#line 1862 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		if (yystack_[1].value.as< int > () > MAX_TIME_SCALE) {
			yyerror("max time prec:%d", MAX_TIME_SCALE);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	}
#line 3494 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 268:
#line 1872 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< int > () = MAX_STR_LENGTH; }
#line 3500 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 269:
#line 1874 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		  if (yystack_[1].value.as< int > () > MAX_STR_LENGTH) {
			yyerror("max char length:%d", MAX_STR_LENGTH);
			YYABORT;
		}
		yylhs.value.as< int > () = yystack_[1].value.as< int > (); 
	  }
#line 3512 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 270:
#line 1885 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { 
		yylhs.value.as< Vector<String> > () = Vector<String>(); 
		yylhs.value.as< Vector<String> > ().push_back("CAT");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
		yylhs.value.as< Vector<String> > ().push_back(" ");
	}
#line 3524 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 271:
#line 1893 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<String> > () = Vector<String>();
		yylhs.value.as< Vector<String> > ().push_back("CSV");
		yylhs.value.as< Vector<String> > ().push_back(yystack_[10].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back(yystack_[5].value.as< std::string > ());
		yylhs.value.as< Vector<String> > ().push_back("new line");
	}
#line 3536 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 272:
#line 1909 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropTable);
		check(cmd_stmt);
		DropTableParam_s param = DropTableParam::make_drop_table_param();
		param->database_name = yystack_[0].value.as< BasicTableStmt_s > ()->database;
		param->table_name = yystack_[0].value.as< BasicTableStmt_s > ()->table_name;
		param->ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3551 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 273:
#line 1920 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropDatabase);
		check(cmd_stmt);
		DropDBParam_s param = DropDBParam::make_drop_DB_param();
		param->database = yystack_[0].value.as< std::string > ();
		param->ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3565 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 274:
#line 1930 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DropView);
		check(cmd_stmt);
		DropViewParam_s param = DropViewParam::make_drop_view_param();
		param->database = driver.get_global_database();
		param->view_name = yystack_[0].value.as< std::string > ();
		param->ignore_not_exists = yystack_[1].value.as< bool > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3580 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 275:
#line 1943 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3586 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 276:
#line 1944 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3592 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 277:
#line 1954 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowDatabases);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
 	}
#line 3602 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 278:
#line 1960 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		ShowTablesParam_s param = ShowTablesParam::make_show_tables_param();
		param->database = yystack_[0].value.as< std::string > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3615 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 279:
#line 1969 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowTables);
		check(cmd_stmt);
		ShowTablesParam_s param = ShowTablesParam::make_show_tables_param();
		param->database = yystack_[0].value.as< std::string > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3628 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 280:
#line 1978 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		DescTableParam_s param = DescTableParam::make_desc_table_param();
		param->database_name = yystack_[0].value.as< BasicTableStmt_s > ()->database;
		param->table_name = yystack_[0].value.as< BasicTableStmt_s > ()->table_name;
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3642 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 281:
#line 1988 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowProcesslist);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3652 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 282:
#line 1994 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(ShowMemory);
		check(cmd_stmt);
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3662 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 283:
#line 2003 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > ();
	}
#line 3670 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 284:
#line 2007 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< std::string > () = driver.get_global_database();
	}
#line 3678 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 285:
#line 2014 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(UseDatabase);
		check(cmd_stmt);
		UseDBParam_s param = UseDBParam::make_use_DB_param();
		param->database = yystack_[0].value.as< std::string > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3691 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 286:
#line 2031 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		DescTableParam_s param = DescTableParam::make_desc_table_param();
		param->database_name = yystack_[0].value.as< BasicTableStmt_s > ()->database;
		param->table_name = yystack_[0].value.as< BasicTableStmt_s > ()->table_name;
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
    }
#line 3705 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 287:
#line 2041 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(DescTable);
		check(cmd_stmt);
		DescTableParam_s param = DescTableParam::make_desc_table_param();
		param->database_name = yystack_[0].value.as< BasicTableStmt_s > ()->database;
		param->table_name = yystack_[0].value.as< BasicTableStmt_s > ()->table_name;
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3719 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 288:
#line 2060 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		CMDStmt_s cmd_stmt = CMDStmt::make_cmd_stmt(Kill);
		check(cmd_stmt);
		KillSessionParam_s param = KillSessionParam::make_kill_session_param();
		param->session_id = yystack_[0].value.as< int > ();
		cmd_stmt->param = param;
		yylhs.value.as< Stmt_s > () = cmd_stmt;
	}
#line 3732 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 289:
#line 2079 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3747 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 290:
#line 2093 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<FunctionDefinition_s> > () = Vector<FunctionDefinition_s>();
		yylhs.value.as< Vector<FunctionDefinition_s> > ().push_back(yystack_[0].value.as< FunctionDefinition_s > ());
	}
#line 3756 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 291:
#line 2098 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<FunctionDefinition_s> > () = yystack_[1].value.as< Vector<FunctionDefinition_s> > ();
		yylhs.value.as< Vector<FunctionDefinition_s> > ().push_back(yystack_[0].value.as< FunctionDefinition_s > ());
	}
#line 3765 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 292:
#line 2106 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = yystack_[0].value.as< FunctionDefinition_s > ();
	}
#line 3773 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 293:
#line 2110 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = yystack_[0].value.as< FunctionDefinition_s > ();
	}
#line 3781 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 294:
#line 2117 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = FunctionDefinition::make_func_define();
		yylhs.value.as< FunctionDefinition_s > ()->name = yystack_[4].value.as< std::string > ();
		yylhs.value.as< FunctionDefinition_s > ()->param_list = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3791 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 295:
#line 2127 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = FunctionDefinition::make_func_define();
		yylhs.value.as< FunctionDefinition_s > ()->name = yystack_[6].value.as< std::string > ();
		yylhs.value.as< FunctionDefinition_s > ()->param_list = yystack_[4].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< FunctionDefinition_s > ()->return_type_list = yystack_[1].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3802 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 296:
#line 2135 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< FunctionDefinition_s > () = FunctionDefinition::make_func_define();
		yylhs.value.as< FunctionDefinition_s > ()->name = yystack_[6].value.as< std::string > ();
		yylhs.value.as< FunctionDefinition_s > ()->param_list = yystack_[4].value.as< Vector<ColumnDefineStmt_s> > ();
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[6].value.as< std::string > (), yystack_[1].value.as< DataType > ());
		yylhs.value.as< FunctionDefinition_s > ()->return_type_list.push_back(stmt);
	}
#line 3814 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 297:
#line 2146 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[1].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3822 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 298:
#line 2153 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
	}
#line 3830 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 299:
#line 2157 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[0].value.as< Vector<ColumnDefineStmt_s> > ();
	}
#line 3838 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 300:
#line 2164 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = Vector<ColumnDefineStmt_s>();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
	}
#line 3847 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 301:
#line 2169 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ColumnDefineStmt_s> > () = yystack_[2].value.as< Vector<ColumnDefineStmt_s> > ();
		yylhs.value.as< Vector<ColumnDefineStmt_s> > ().push_back(yystack_[0].value.as< ColumnDefineStmt_s > ());
	}
#line 3856 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 302:
#line 2177 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		ColumnDefineStmt_s stmt = ColumnDefineStmt::make_column_define_stmt(yystack_[1].value.as< std::string > (), yystack_[0].value.as< DataType > ());
		check(stmt);
		yylhs.value.as< ColumnDefineStmt_s > () = stmt;
	}
#line 3866 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 303:
#line 2185 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 3872 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 304:
#line 2186 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3878 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 305:
#line 2190 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3884 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 306:
#line 2194 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3890 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 307:
#line 2198 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3896 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 308:
#line 2202 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3902 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 309:
#line 2207 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
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
#line 3917 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 310:
#line 2221 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3923 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 311:
#line 2223 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 3929 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 312:
#line 2228 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = Vector<ExprStmt_s>();
	}
#line 3937 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 313:
#line 2232 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< Vector<ExprStmt_s> > () = yystack_[0].value.as< Vector<ExprStmt_s> > ();
	}
#line 3945 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 314:
#line 2244 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(driver.get_global_database(), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3957 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 315:
#line 2252 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		//构建表表达式
		BasicTableStmt_s table = BasicTableStmt::make_basic_table(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
		check(table);
		table->alias_name = yystack_[0].value.as< std::string > ();
		yylhs.value.as< BasicTableStmt_s > () = table;
	}
#line 3969 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 316:
#line 2262 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3975 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 317:
#line 2266 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3981 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 318:
#line 2270 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3987 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 319:
#line 2274 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > ()=yystack_[0].value.as< std::string > (); }
#line 3993 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 320:
#line 2278 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { str_to_lower(yystack_[0].value.as< std::string > ());yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 3999 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 321:
#line 2282 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4005 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 322:
#line 2283 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4011 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 323:
#line 2287 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4017 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 324:
#line 2291 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 4023 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;

  case 325:
#line 2296 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:859
    {
		yylhs.value.as< int > () = std::stoi(yystack_[0].value.as< std::string > ());
	}
#line 4031 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
    break;


#line 4035 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:859
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


  const short int parser::yypact_ninf_ = -496;

  const short int parser::yytable_ninf_ = -318;

  const short int
  parser::yypact_[] =
  {
    1027,  -496,    10,    -7,    55,    55,   -13,  -496,    14,    -5,
      58,    -9,    -7,   219,    -7,    55,   166,    71,  -496,  -496,
     391,   425,  -496,    96,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,    55,    95,    55,    55,
      55,   118,   921,   192,  -496,  -496,   120,  -496,   154,  -496,
     200,   200,   200,  -496,  -496,  -496,  -496,  -496,    55,  -496,
    -496,  -496,   151,   229,   226,  -496,   161,  -496,  -496,   228,
      55,  -496,  -496,  -496,  -496,    37,    -9,    -9,    50,    -9,
      -9,   184,  -496,  -496,  -496,   220,   232,    55,  1019,   245,
     252,   245,   245,   245,   245,   245,   245,   260,   265,   245,
     245,   270,   245,   245,   276,   279,   295,   338,  1019,  -496,
      55,   250,   345,    55,   343,    55,    55,    55,    86,  -496,
    -496,   954,    55,   228,    55,  -496,   297,    -9,  -496,  -496,
    -496,  -496,    -9,  -496,  -496,  -496,    55,    55,    55,   426,
     428,   393,    28,  -496,   433,  -496,  -496,  -496,  -496,  -496,
    -496,   433,   433,  -496,  -496,    58,  -496,  -496,   433,   433,
     436,  -496,  -496,    47,    55,     7,  -496,  -496,  -496,  -496,
    -496,  -496,   370,  -496,  -496,  -496,  -496,   532,  -496,   448,
     448,  -496,   383,   837,  -496,  -496,   954,  -496,   954,  -496,
    -496,  -496,   384,  -496,   388,   395,  -496,   136,  -496,  -496,
     349,  -496,  -496,  -496,  -496,  -496,   407,   386,  -496,   394,
    -496,  -496,  -496,  -496,  -496,  -496,    55,  -496,  -496,   410,
    -496,   -20,  -496,   615,  -496,    38,  -496,  -496,   -28,   451,
     379,  -496,  -496,   105,    55,    55,   389,    55,    55,   473,
      55,   354,  -496,    47,    47,    -9,   -37,  -496,  -496,  -496,
     368,    47,   954,   477,   753,   103,   -29,  -496,  -496,   954,
     457,   408,   483,   483,   954,   954,   954,   954,     7,    55,
     954,   870,   954,   870,  -496,  -496,    41,    89,   -22,  -496,
     522,   954,    55,   467,  -496,  -496,   438,   438,  -496,  -496,
     440,  -496,  -496,  -496,  -496,  -496,   449,   450,  -496,  -496,
     452,   452,  -496,   438,   438,   526,    55,  -496,    -9,    55,
      55,   -16,  -496,  -496,  -496,  -496,  -496,   105,  -496,    60,
    -496,    75,  -496,   100,  -496,   155,   158,  -496,   520,  -496,
     753,  -496,   354,  -496,   418,     7,  -496,   437,   461,   437,
     437,  -496,   483,   168,   468,   493,   469,   689,   390,  -496,
      -8,   954,     4,   954,   753,   466,  -496,   517,   403,   496,
    -496,   -37,  -496,  -496,   448,  -496,   -23,  -496,  -496,   -23,
    -496,  -496,   954,  -496,  -496,  -496,    55,     7,  -496,   954,
     483,   527,   441,  -496,   562,  -496,    58,  -496,  -496,    58,
      58,    58,    58,  -496,  -496,  -496,  -496,   552,  -496,  -496,
    -496,   485,   486,  -496,  -496,  -496,   172,  -496,   105,  -496,
      55,  -496,  -496,  -496,   445,    45,  -496,    47,   368,  -496,
     499,   160,   506,   507,   954,  -496,   954,   469,  -496,   322,
      13,   268,   339,   357,   387,   397,   424,  -496,   227,    44,
     954,   490,   954,   469,   753,   753,   954,   365,   954,   954,
     579,     2,   954,    58,   448,  -496,  -496,   523,   -27,   483,
     472,  -496,   -37,   483,   954,  -496,   554,   478,   569,   570,
     577,   489,  -496,    55,    55,  -496,  -496,  -496,  -496,  -496,
     160,    25,  -496,  -496,   509,   160,   160,   483,   175,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,   954,   529,   610,  -496,   234,   837,
    -496,   483,  -496,   617,  -496,   483,   954,   580,   417,  -496,
     954,   556,   590,   508,   606,   571,  -496,  -496,  -496,   530,
    -496,   483,   603,  -496,    58,    58,    58,  -496,   528,   609,
    -496,   615,  -496,   534,   536,    -9,   538,   368,   753,   546,
     548,  -496,   240,   837,  -496,  -496,   954,   180,   483,  -496,
     954,   483,  -496,    58,  -496,   954,   753,   558,   581,   592,
     539,   559,   560,   561,    55,  -496,   551,   753,   564,  -496,
      45,   753,   753,   954,   181,   483,  -496,   483,   566,   527,
      45,   653,   601,   574,   697,  -496,  -496,  -496,   557,  -496,
    -496,    45,   578,    45,    45,   483,  -496,  -496,   954,    58,
    -496,   673,   558,   448,   618,   584,   585,   679,  -496,   400,
     680,  -496,   954,   593,   686,    55,  -496,  -496,   954,  -496,
    -496,  -496,    58,   527,  -496,   645,   183,  -496,  -496,   583,
    -496,   693,   448,   688,   639,   594,   699,   448,  -496
  };

  const unsigned short int
  parser::yydefact_[] =
  {
       9,   311,   303,    32,     0,     0,     0,   310,     0,     0,
       0,     0,    32,     0,    32,     0,     0,     0,     3,     4,
      19,     0,    29,    28,     5,     6,     7,     8,    11,    12,
      10,    14,    13,    15,    16,    17,     0,     0,     0,     0,
       0,     0,    35,     0,   320,   287,     0,   314,   317,   286,
     275,   275,   275,   225,   227,   228,   226,   224,     0,   324,
     325,   288,     0,    70,     0,   277,     0,   282,   281,   284,
       0,   285,   316,     1,     2,     0,     0,     0,     0,     0,
       0,     0,   305,   304,   230,     0,   232,     0,    35,    59,
       0,    59,    59,    59,    59,    59,    59,     0,     0,    59,
      59,     0,    59,    59,     0,     0,     0,     0,    36,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
      71,     0,     0,   284,     0,   279,     0,     0,    21,    28,
      27,    25,     0,    20,    26,    24,     0,     0,     0,     0,
       0,     0,     0,    46,    57,    42,    47,    43,    39,    45,
      41,    57,    57,    49,    54,     0,    44,    40,    57,    57,
       0,    33,    38,   112,     0,     0,   315,   317,   276,   273,
     272,   274,     0,   211,   187,   323,   200,     0,   198,     0,
       0,   204,     0,     0,   202,   201,     0,   179,     0,   203,
     182,   205,     0,   199,     0,     0,   167,     0,    72,   123,
      94,   119,   132,   115,   116,   118,     0,     0,   178,   318,
     185,   184,   186,   280,   278,   283,     0,    23,    22,     0,
     307,     0,   237,     0,   318,     0,   234,   236,     0,     0,
       0,    60,    58,     0,     0,     0,     0,     0,     0,    64,
       0,    77,   113,   112,   112,     0,    77,   109,   110,   111,
      74,   112,     0,   210,     0,     0,     0,   322,   321,     0,
     167,     0,   125,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,   319,   195,     0,    77,   218,
       0,   312,     0,   270,   254,   247,   268,   268,   264,   263,
     248,   257,   255,   245,   246,   244,   251,   249,   256,   243,
     266,   266,   242,   268,   268,   240,     0,   233,     0,     0,
       0,     0,   290,   292,   293,    34,    61,     0,    68,    64,
      66,    64,    62,    64,    56,    64,    64,    65,     0,   114,
       0,   221,    77,    98,     0,     0,   223,   104,     0,   104,
     104,    97,   214,     0,     0,     0,     0,     0,     0,   174,
       0,     0,     0,     0,     0,     0,   117,     0,     0,     0,
      73,    77,    96,   129,     0,   131,   127,   128,   130,   126,
     196,   197,   206,   181,   183,   180,     0,     0,   216,     0,
     133,   313,     0,   238,     0,   229,     0,   259,   258,     0,
       0,     0,     0,   265,   262,   261,   260,     0,   239,   235,
     231,     0,     0,   306,   289,   291,    64,    55,     0,    51,
       0,    53,    50,    52,     0,    76,   222,   112,    75,   105,
       0,     0,     0,     0,     0,   212,     0,     0,   144,     0,
       0,   147,   148,   149,   151,   150,   152,   153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   190,   194,    79,     0,   207,
       0,   219,    77,   220,     0,   309,     0,     0,     0,     0,
       0,     0,   241,   298,   298,    67,    69,    63,    48,    99,
       0,     0,   106,   107,     0,     0,     0,   215,     0,   145,
     175,   161,   155,   162,   156,   163,   157,   165,   159,   164,
     158,   166,   160,   154,     0,     0,     0,   136,     0,     0,
     142,   135,   146,   177,   176,   172,     0,     0,     0,   169,
       0,     0,     0,     0,     0,    81,   120,   121,   122,   188,
     217,   134,     0,   269,     0,     0,     0,   267,     0,   299,
     300,     0,   308,     0,     0,     0,   111,     0,     0,     0,
       0,   213,     0,     0,   143,   137,     0,     0,   170,   168,
       0,   173,   193,     0,   191,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,   302,     0,     0,     0,   108,
     103,     0,     0,     0,     0,   138,   140,   171,     0,    78,
      80,     0,    90,   208,     0,   250,   252,   253,     0,   301,
     294,   102,   112,   100,   101,   139,   141,   192,     0,     0,
      31,     0,    82,     0,     0,     0,     0,    83,    84,    87,
      92,    93,     0,     0,     0,     0,   296,   295,     0,    88,
      89,    86,     0,   209,   189,     0,     0,    85,    91,     0,
     297,     0,     0,     0,     0,     0,     0,     0,   271
  };

  const short int
  parser::yypgoto_[] =
  {
    -496,  -496,  -496,  -496,     1,  -496,   667,    12,   681,    59,
     644,  -496,   627,   197,   849,    81,  -225,   330,   423,  -496,
    -496,  -224,  -235,  -496,  -496,   137,  -496,   122,  -496,  -496,
     116,   484,  -163,   -68,   271,    87,   -47,  -329,   107,  -496,
     481,   -79,  -353,  -496,  -496,  -496,  -341,  -496,  -496,  -496,
    -295,  -496,  -496,  -496,  -496,  -496,  -496,  -496,   757,  -496,
     342,   761,  -496,   396,   762,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,   497,  -496,  -495,   479,   -59,  -496,  -496,   209,
    -496,   655,  -496,  -496,  -496,  -496,  -496,   474,  -496,  -496,
    -496,   310,   163,   217,  -496,   705,  -496,   487,  -496,  -496,
    -496,  -496,   167,    36,    11,   -98,   524,    -2,  -496,  -153,
     -10,     0
  };

  const short int
  parser::yydefgoto_[] =
  {
      -1,    16,    17,    18,    62,    20,    21,   196,    23,    43,
     107,   108,   109,   233,   143,   321,   410,   318,   319,   121,
     197,   246,   331,   525,   567,   592,   617,   618,   631,   610,
     620,   198,   247,   248,   249,   420,   484,   250,   479,   199,
     365,   348,   381,   349,   442,   443,   201,   202,   352,   256,
     350,   203,   204,   205,   372,   206,   460,   612,    24,   253,
     343,    25,   278,   279,    26,    27,    57,    28,   139,   225,
     308,   221,   222,   398,   305,   393,   387,   385,    29,   115,
      30,   125,    31,    32,    33,    34,   311,   312,   313,   314,
     616,   538,   539,   540,    41,    81,   402,   219,   541,    35,
      36,   382,   251,    46,   207,   208,   274,   209,   210,   211,
     212,   621
  };

  const short int
  parser::yytable_[] =
  {
      60,    19,    48,    48,   335,   428,   418,   444,   445,    53,
      61,   336,    22,    72,   328,    47,    47,   444,   445,   376,
      22,   282,   526,    22,    42,    37,   257,   258,   444,   445,
      44,   353,    50,   270,    82,   415,    72,    48,    86,   223,
     226,    48,   200,   378,   361,   309,   575,   404,    44,   377,
      47,    71,   430,   230,    47,    38,    48,   309,   506,   451,
     444,   445,   507,   127,   448,     3,   244,   370,    48,    47,
      44,    63,   527,    70,    84,   240,   132,   272,    44,   306,
      58,    47,    59,    11,   244,    82,   489,    22,    22,    22,
      22,    22,    22,   310,   408,     9,   371,   416,   255,   245,
     510,   327,   512,   615,   261,   310,    11,   262,    48,   263,
     283,   167,    44,    72,    48,   171,   327,   545,   280,   173,
      48,    47,    72,    12,   166,   330,   457,    47,    44,    11,
      22,    51,   354,    47,   220,   224,   224,   528,   446,    22,
     330,   327,    11,   490,    22,    60,    12,    52,   520,   513,
     514,   169,   547,   462,    39,   236,   557,    12,   231,   270,
     215,   167,    48,    48,   554,   449,    73,    14,   307,    12,
      40,    45,    49,   342,   242,    47,    47,   267,    11,   375,
     355,   408,    12,    44,   223,   357,   358,   359,   200,   373,
     407,   363,   366,   367,   369,   260,   327,   317,   275,   327,
     584,   271,   380,   272,    74,   409,    85,   268,   399,   424,
     112,   458,   589,   327,   224,    44,   424,   374,    12,   244,
      83,   464,   464,   273,   574,   118,   -18,   530,   388,   -18,
     411,   320,   322,   322,    87,   322,   322,   126,   167,   113,
     503,   167,   167,   172,   395,   396,   334,   504,   505,   167,
     556,   329,   481,   580,   242,   242,   583,    22,   482,    64,
     116,   117,   242,   110,   351,    65,    48,   275,   429,   633,
     241,   590,   447,  -316,   450,   224,   114,   163,   280,    47,
     224,   119,   601,   170,   120,   412,   603,   604,   413,   213,
     270,    66,   111,   459,   491,   492,   270,   122,   425,   124,
     463,   523,   475,   136,   224,   551,   123,   220,   403,   400,
     586,   606,   137,   640,    67,   320,   323,   482,   325,   326,
      22,   164,   482,   482,   138,   431,   432,   433,   434,   435,
     436,   243,   271,    48,   272,   437,   438,   142,   271,   439,
     272,    68,   440,   441,   144,   487,    47,   342,   234,   235,
     332,   333,   151,   483,   273,   237,   238,   152,   341,   260,
     273,   508,   155,   511,    69,   493,   494,   515,   158,   517,
     518,   159,    44,   521,   224,    48,    60,   269,   270,    60,
      60,    60,    60,   495,   496,   531,   467,   160,    47,   468,
     469,   470,   471,   431,   432,   433,   434,   435,   436,   161,
      75,    76,    77,   437,   438,   270,   320,   439,   477,   168,
     440,   441,   483,   497,   498,   167,   165,   483,   483,    48,
     271,   270,   272,   499,   500,   552,   422,   423,   242,   629,
     380,   216,    47,   544,    78,    79,    80,   558,   549,   550,
     337,   561,   273,   522,   454,   228,   270,   271,   338,   272,
     501,   502,   356,   630,   227,   229,   270,   339,   232,   270,
     624,   239,   252,   271,   270,   272,   -29,   -29,   -29,   273,
     175,   542,   542,   270,   380,   259,   264,   585,    48,    48,
     265,   587,   334,    48,    48,   273,   380,   266,   271,   643,
     272,    47,    47,    22,   648,   340,    47,    47,   271,   276,
     272,   271,   281,   272,   605,   277,   271,   452,   272,   316,
     273,   516,   315,  -317,   327,   271,   330,   272,   344,   324,
     273,   260,   270,   273,    60,    60,    60,   379,   273,   619,
     386,   384,   389,   455,   570,   571,   572,   273,   356,   270,
     397,   390,   391,   380,   392,   414,   578,   421,   417,   619,
     174,   419,   270,   588,   175,    44,    59,    22,   453,   427,
     426,    11,   176,   560,   271,   260,   272,   466,   464,   177,
     472,   465,   542,   270,   178,   478,   284,   473,   474,   179,
     180,   271,   509,   272,   181,   480,   273,   -30,   -30,   -30,
     285,   286,   485,   486,   271,   287,   272,   524,   532,    60,
     167,    44,   529,   273,   288,   289,   240,   290,   533,   182,
     534,   535,   270,   242,   291,   271,   273,   272,   536,   537,
     548,   553,    60,   542,   183,   184,   456,   292,   555,   185,
     186,   563,   187,   445,   284,   270,   270,   273,   564,   293,
     294,   565,   519,   559,   569,   568,   566,   577,   285,   286,
     574,   295,   188,   287,   271,   189,   272,   581,   573,   582,
     190,   191,   288,   289,   576,   290,   296,   297,   579,   595,
     591,   594,   291,   593,   192,   193,   273,   271,   271,   272,
     272,   298,   194,   195,   600,   292,   562,   598,   608,   596,
     597,   609,   611,   254,   602,   299,   607,   293,   294,   273,
     273,   614,   613,   345,   300,   301,   302,   174,   622,   295,
     625,   175,    44,    59,   639,   303,   304,   626,   627,   176,
     628,   632,   641,   634,   296,   297,   177,   635,   642,   644,
     645,   178,   141,   646,   647,   162,   179,   180,   476,   298,
     406,   181,   128,   130,   131,   133,   134,   135,   638,   623,
     637,   360,   546,   299,   368,   346,   129,   129,   129,   129,
     129,   129,   300,   301,   302,    54,   182,   345,   488,    55,
      56,   174,   461,   303,   304,   175,    44,    59,   214,   383,
     394,   347,   184,   176,   543,   405,   185,   186,   636,   187,
     177,   599,   140,   362,   217,   178,   401,     0,     0,   218,
     179,   180,     0,     0,     0,   181,     0,     0,   129,   188,
       0,     0,   189,   129,     0,     0,     0,   190,   191,   346,
       0,    12,     0,     0,     0,     0,     0,     0,     0,     0,
     182,   192,   193,     0,     0,     0,     0,     0,     0,   194,
     195,     0,     0,     0,     0,   347,   184,     0,     0,     0,
     185,   186,     0,   187,     0,   174,     0,     0,     0,   175,
      44,    59,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,   188,   177,     0,   189,     0,     0,   178,
       0,   190,   191,     0,   179,   180,     0,     0,   174,   181,
       0,     0,   175,    44,    59,   192,   193,     0,     0,     0,
     176,     0,     0,   194,   195,     0,     0,   177,     0,     0,
       0,     0,   178,     0,   182,     0,     0,   179,   180,     0,
       0,     0,   181,     0,     0,     0,     0,     0,     0,   183,
     184,     0,     0,     0,   185,   186,     0,   187,     0,     0,
     145,   146,   147,   148,   149,   150,     0,   182,   153,   154,
       0,   156,   157,    88,   364,     0,     0,   188,     0,     0,
     189,     0,   183,   184,     0,   190,   191,   185,   186,    12,
     187,     0,   174,     0,     0,     0,   175,    44,    59,   192,
     193,     0,     0,     0,   176,     0,     0,   194,   195,    89,
     188,   177,     0,   189,     0,     0,   178,     0,   190,   191,
       0,   179,   180,     0,     0,     0,   181,     0,     0,    90,
       0,     0,   192,   193,     0,     0,     0,    91,     0,     0,
     194,   195,    92,    93,    94,    95,    96,    97,    98,    99,
       0,   182,     0,     0,   100,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,   183,   184,     0,     0,
       0,   185,   186,     0,   187,     0,     0,   102,     0,     0,
       0,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       2,     0,     0,   103,   188,   104,   105,   189,     3,     0,
       4,     5,   190,   191,   106,     6,     0,    89,     0,     0,
       0,     0,     7,     0,     8,     0,   192,   193,     0,     0,
       0,     0,     0,     0,   194,   195,     0,    90,     9,     0,
       0,     0,     0,     0,    10,    91,     0,     0,     0,    11,
      92,    93,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,    12,
       0,     0,    13,     0,     0,     0,     0,     0,     0,     0,
       0,   103,     0,   104,   105,     0,     0,     0,     0,     0,
      14,     0,   106,    15
  };

  const short int
  parser::yycheck_[] =
  {
      10,     0,     4,     5,    41,   346,   335,    15,    16,     8,
      10,   246,     0,    15,   239,     4,     5,    15,    16,    41,
       8,    41,    49,    11,    31,    15,   179,   180,    15,    16,
      23,    60,    45,    56,    36,   330,    38,    39,    40,   137,
     138,    43,   121,   278,   268,    73,   541,    63,    23,    71,
      39,    15,   347,    25,    43,    45,    58,    73,    14,   354,
      15,    16,    18,    26,    60,    51,    59,    26,    70,    58,
      23,    12,    99,    14,    38,    28,    26,   100,    23,    41,
      85,    70,    24,    92,    59,    87,   427,    75,    76,    77,
      78,    79,    80,   121,   319,    81,    55,   332,   177,    92,
     441,    41,   443,   598,   183,   121,    92,   186,   110,   188,
     130,   113,    23,   115,   116,   117,    41,    92,   216,   118,
     122,   110,   124,   132,   113,   162,   361,   116,    23,    92,
     118,   144,   161,   122,   136,   137,   138,   164,   146,   127,
     162,    41,    92,   130,   132,   155,   132,   160,   146,   444,
     445,   115,   481,   377,   144,   155,   509,   132,   130,    56,
     124,   163,   164,   165,   505,   161,     0,   153,   130,   132,
     160,     4,     5,   252,   163,   164,   165,    41,    92,   277,
     259,   406,   132,    23,   282,   264,   265,   266,   267,   100,
     130,   270,   271,   272,   273,   183,    41,    92,   200,    41,
     553,    98,   281,   100,   133,   130,    39,    71,   306,    41,
      43,   364,   565,    41,   216,    23,    41,   128,   132,    59,
     125,    41,    41,   120,    41,    58,   130,   462,   287,   133,
     130,   233,   234,   235,   116,   237,   238,    70,   240,   119,
      13,   243,   244,   157,   303,   304,   245,    20,    21,   251,
      16,   240,    92,   548,   243,   244,    16,   245,   421,    40,
      51,    52,   251,    71,   161,    46,   268,   269,   347,   622,
     163,   566,   351,   119,   353,   277,    76,   110,   376,   268,
     282,   130,   577,   116,    55,   130,   581,   582,   130,   122,
      56,    72,   100,   372,    26,    27,    56,    71,   130,    71,
     379,   454,   130,   119,   306,   130,   145,   309,   310,   308,
     130,   130,    92,   130,    95,   317,   235,   480,   237,   238,
     308,    71,   485,   486,    92,     3,     4,     5,     6,     7,
       8,   164,    98,   335,   100,    13,    14,    92,    98,    17,
     100,   122,    20,    21,    92,   424,   335,   426,   151,   152,
     243,   244,    92,   421,   120,   158,   159,    92,   251,   347,
     120,   440,    92,   442,   145,    26,    27,   446,    92,   448,
     449,    92,    23,   452,   376,   377,   386,    28,    56,   389,
     390,   391,   392,    26,    27,   464,   386,    92,   377,   389,
     390,   391,   392,     3,     4,     5,     6,     7,     8,    61,
       9,    10,    11,    13,    14,    56,   408,    17,   410,    66,
      20,    21,   480,    26,    27,   417,    71,   485,   486,   421,
      98,    56,   100,    26,    27,   504,   339,   340,   417,    29,
     509,   134,   421,   480,     9,    10,    11,   516,   485,   486,
      72,   520,   120,   453,    41,    17,    56,    98,    80,   100,
      26,    27,   130,    53,    28,    62,    56,    89,    25,    56,
     613,    25,    92,    98,    56,   100,     9,    10,    11,   120,
      22,   473,   474,    56,   553,    92,    92,   556,   480,   481,
      92,   560,   481,   485,   486,   120,   565,    92,    98,   642,
     100,   480,   481,   481,   647,   127,   485,   486,    98,    92,
     100,    98,    92,   100,   583,   119,    98,    41,   100,   130,
     120,   146,    61,   119,    41,    98,   162,   100,    41,   130,
     120,   509,    56,   120,   534,   535,   536,     5,   120,   608,
      92,    64,    92,   130,   534,   535,   536,   120,   130,    56,
      14,    92,    92,   622,    92,    25,   545,    86,   130,   628,
      18,   114,    56,   563,    22,    23,    24,   545,    41,    66,
      92,    92,    30,   146,    98,   553,   100,     5,    41,    37,
      18,   130,   574,    56,    42,   130,    19,    92,    92,    47,
      48,    98,    92,   100,    52,    86,   120,     9,    10,    11,
      33,    34,    86,    86,    98,    38,   100,    74,    44,   609,
     602,    23,   130,   120,    47,    48,    28,    50,   130,    77,
      41,    41,    56,   602,    57,    98,   120,   100,    41,   130,
     111,    92,   632,   625,    92,    93,   130,    70,    18,    97,
      98,    41,   100,    16,    19,    56,    56,   120,   130,    82,
      83,    35,    63,    63,    41,   115,    75,   111,    33,    34,
      41,    94,   120,    38,    98,   123,   100,   111,   130,   111,
     128,   129,    47,    48,   130,    50,   109,   110,   130,   130,
     112,    79,    57,    92,   142,   143,   120,    98,    98,   100,
     100,   124,   150,   151,   133,    70,   130,   126,    35,   130,
     130,    90,   118,   161,   130,   138,   130,    82,    83,   120,
     120,   144,     5,    14,   147,   148,   149,    18,    35,    94,
      92,    22,    23,    24,    69,   158,   159,   133,   133,    30,
      41,    41,   139,   130,   109,   110,    37,    41,    35,    41,
      91,    42,    88,   139,    35,   108,    47,    48,   408,   124,
     317,    52,    75,    76,    77,    78,    79,    80,   632,   612,
     628,   267,   481,   138,   273,    66,    75,    76,    77,    78,
      79,    80,   147,   148,   149,     8,    77,    14,   426,     8,
       8,    18,   376,   158,   159,    22,    23,    24,   123,   282,
     301,    92,    93,    30,   474,   311,    97,    98,   625,   100,
      37,   574,    87,   269,   127,    42,   309,    -1,    -1,   132,
      47,    48,    -1,    -1,    -1,    52,    -1,    -1,   127,   120,
      -1,    -1,   123,   132,    -1,    -1,    -1,   128,   129,    66,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,   150,
     151,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,   120,    37,    -1,   123,    -1,    -1,    42,
      -1,   128,   129,    -1,    47,    48,    -1,    -1,    18,    52,
      -1,    -1,    22,    23,    24,   142,   143,    -1,    -1,    -1,
      30,    -1,    -1,   150,   151,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    42,    -1,    77,    -1,    -1,    47,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,    -1,
      91,    92,    93,    94,    95,    96,    -1,    77,    99,   100,
      -1,   102,   103,    32,    84,    -1,    -1,   120,    -1,    -1,
     123,    -1,    92,    93,    -1,   128,   129,    97,    98,   132,
     100,    -1,    18,    -1,    -1,    -1,    22,    23,    24,   142,
     143,    -1,    -1,    -1,    30,    -1,    -1,   150,   151,    68,
     120,    37,    -1,   123,    -1,    -1,    42,    -1,   128,   129,
      -1,    47,    48,    -1,    -1,    -1,    52,    -1,    -1,    88,
      -1,    -1,   142,   143,    -1,    -1,    -1,    96,    -1,    -1,
     150,   151,   101,   102,   103,   104,   105,   106,   107,   108,
      -1,    77,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    97,    98,    -1,   100,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,   152,   120,   154,   155,   123,    51,    -1,
      53,    54,   128,   129,   163,    58,    -1,    68,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,   142,   143,    -1,    -1,
      -1,    -1,    -1,    -1,   150,   151,    -1,    88,    81,    -1,
      -1,    -1,    -1,    -1,    87,    96,    -1,    -1,    -1,    92,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,   132,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
     153,    -1,   163,   156
  };

  const unsigned short int
  parser::yystos_[] =
  {
       0,    36,    43,    51,    53,    54,    58,    65,    67,    81,
      87,    92,   132,   135,   153,   156,   166,   167,   168,   169,
     170,   171,   172,   173,   223,   226,   229,   230,   232,   243,
     245,   247,   248,   249,   250,   264,   265,    15,    45,   144,
     160,   259,    31,   174,    23,   267,   268,   269,   272,   267,
      45,   144,   160,   169,   223,   226,   229,   231,    85,    24,
     275,   276,   169,   174,    40,    46,    72,    95,   122,   145,
     174,   268,   272,     0,   133,     9,    10,    11,     9,    10,
      11,   260,   272,   125,   268,   267,   272,   116,    32,    68,
      88,    96,   101,   102,   103,   104,   105,   106,   107,   108,
     113,   117,   136,   152,   154,   155,   163,   175,   176,   177,
      71,   100,   267,   119,    76,   244,   244,   244,   267,   130,
      55,   184,    71,   145,    71,   246,   267,    26,   171,   173,
     171,   171,    26,   171,   171,   171,   119,    92,    92,   233,
     260,   175,    92,   179,    92,   179,   179,   179,   179,   179,
     179,    92,    92,   179,   179,    92,   179,   179,    92,    92,
      92,    61,   177,   267,    71,    71,   269,   272,    66,   268,
     267,   272,   157,   169,    18,    22,    30,    37,    42,    47,
      48,    52,    77,    92,    93,    97,    98,   100,   120,   123,
     128,   129,   142,   143,   150,   151,   172,   185,   196,   204,
     206,   211,   212,   216,   217,   218,   220,   269,   270,   272,
     273,   274,   275,   267,   246,   268,   134,   171,   171,   262,
     272,   236,   237,   270,   272,   234,   270,    28,    17,    62,
      25,   130,    25,   178,   178,   178,   276,   178,   178,    25,
      28,   203,   269,   267,    59,    92,   186,   197,   198,   199,
     202,   267,    92,   224,   161,   206,   214,   274,   274,    92,
     172,   206,   206,   206,    92,    92,    92,    41,    71,    28,
      56,    98,   100,   120,   271,   272,    92,   119,   227,   228,
     270,    92,    41,   130,    19,    33,    34,    38,    47,    48,
      50,    57,    70,    82,    83,    94,   109,   110,   124,   138,
     147,   148,   149,   158,   159,   239,    41,   130,   235,    73,
     121,   251,   252,   253,   254,    61,   130,    92,   182,   183,
     272,   180,   272,   180,   130,   180,   180,    41,   181,   269,
     162,   187,   203,   203,   169,    41,   187,    72,    80,    89,
     127,   203,   206,   225,    41,    14,    66,    92,   206,   208,
     215,   161,   213,    60,   161,   206,   130,   206,   206,   206,
     196,   186,   271,   206,    84,   205,   206,   206,   205,   206,
      26,    55,   219,   100,   128,   270,    41,    71,   187,     5,
     206,   207,   266,   237,    64,   242,    92,   241,   241,    92,
      92,    92,    92,   240,   240,   241,   241,    14,   238,   270,
     169,   262,   261,   272,    63,   252,   183,   130,   181,   130,
     181,   130,   130,   130,    25,   215,   187,   130,   202,   114,
     200,    86,   200,   200,    41,   130,    92,    66,   211,   206,
     215,     3,     4,     5,     6,     7,     8,    13,    14,    17,
      20,    21,   209,   210,    15,    16,   146,   206,    60,   161,
     206,   215,    41,    41,    41,   130,   130,   187,   274,   206,
     221,   228,   186,   206,    41,   130,     5,   276,   276,   276,
     276,   276,    18,    92,    92,   130,   182,   272,   130,   203,
      86,    92,   197,   198,   201,    86,    86,   206,   225,   211,
     130,    26,    27,    26,    27,    26,    27,    26,    27,    26,
      27,    26,    27,    13,    20,    21,    14,    18,   206,    92,
     211,   206,   211,   215,   215,   206,   146,   206,   206,    63,
     146,   206,   275,   274,    74,   188,    49,    99,   164,   130,
     187,   206,    44,   130,    41,    41,    41,   130,   256,   257,
     258,   263,   272,   256,   201,    92,   199,   202,   111,   201,
     201,   130,   206,    92,   211,    18,    16,   207,   206,    63,
     146,   206,   130,    41,   130,    35,    75,   189,   115,    41,
     276,   276,   276,   130,    41,   239,   130,   111,   169,   130,
     215,   111,   111,    16,   207,   206,   130,   206,   275,   207,
     215,   112,   190,    92,    79,   130,   130,   130,   126,   258,
     133,   215,   130,   215,   215,   206,   130,   130,    35,    90,
     194,   118,   222,     5,   144,   239,   255,   191,   192,   206,
     195,   276,    35,   190,   274,    92,   133,   133,    41,    29,
      53,   193,    41,   207,   130,    41,   257,   192,   195,    69,
     130,   139,    35,   274,    41,    91,   139,    35,   274
  };

  const unsigned short int
  parser::yyr1_[] =
  {
       0,   165,   166,   167,   167,   167,   167,   167,   167,   167,
     168,   168,   168,   168,   168,   168,   168,   168,   169,   169,
     170,   170,   170,   170,   170,   170,   170,   170,   171,   171,
     172,   173,   174,   174,   174,   175,   175,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   178,   178,   179,
     179,   179,   180,   180,   181,   181,   182,   182,   183,   183,
     184,   184,   185,   185,   186,   186,   187,   187,   188,   188,
     189,   189,   190,   190,   191,   191,   192,   193,   193,   193,
     194,   194,   194,   195,   196,   196,   196,   197,   197,   198,
     199,   199,   199,   199,   200,   200,   201,   201,   201,   202,
     202,   202,   203,   203,   203,   204,   204,   204,   204,   204,
     205,   205,   205,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   207,   207,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   209,   209,   209,
     209,   209,   209,   209,   209,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   211,   212,   212,
     213,   213,   214,   214,   215,   215,   215,   215,   216,   216,
     216,   216,   216,   216,   217,   217,   217,   217,   218,   218,
     218,   218,   218,   218,   218,   219,   219,   219,   220,   220,
     220,   220,   220,   220,   220,   220,   221,   221,   222,   222,
     223,   223,   224,   224,   225,   225,   226,   226,   227,   227,
     228,   229,   229,   229,   230,   231,   231,   231,   231,   232,
     232,   232,   233,   233,   234,   234,   235,   236,   236,   237,
     238,   238,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   240,   241,   241,
     242,   242,   243,   243,   243,   244,   244,   245,   245,   245,
     245,   245,   245,   246,   246,   247,   248,   248,   249,   250,
     251,   251,   252,   252,   253,   254,   254,   255,   256,   256,
     257,   257,   258,   259,   259,   260,   261,   262,   263,   264,
     265,   265,   266,   266,   267,   267,   268,   269,   270,   271,
     272,   273,   273,   274,   275,   276
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     4,     4,     3,     3,     3,     3,     1,     1,
       3,    11,     0,     3,     5,     0,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     2,
       5,     5,     5,     5,     2,     5,     4,     0,     1,     0,
       2,     3,     1,     3,     0,     1,     1,     3,     1,     3,
       0,     1,     1,     3,     1,     3,     2,     0,     3,     0,
       2,     0,     0,     3,     1,     3,     2,     0,     1,     1,
       0,     4,     2,     1,     1,     2,     3,     2,     2,     4,
       7,     7,     7,     6,     0,     1,     1,     1,     3,     1,
       1,     1,     0,     1,     2,     1,     1,     3,     1,     1,
       3,     3,     3,     1,     2,     2,     3,     3,     3,     3,
       3,     3,     1,     1,     3,     3,     3,     4,     5,     6,
       5,     6,     3,     4,     2,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     6,     5,
       4,     5,     4,     5,     1,     3,     3,     3,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     1,     5,    10,
       4,     6,     8,     6,     4,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     3,
       5,     4,     3,     5,     1,     3,     6,     8,     1,     3,
       3,     6,     7,     6,     2,     1,     1,     1,     1,     7,
       3,     7,     0,     3,     1,     3,     0,     1,     3,     3,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       6,     1,     6,     6,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     1,     1,     2,     0,     3,     0,     3,
       0,    17,     4,     4,     4,     0,     2,     2,     4,     3,
       4,     2,     2,     2,     0,     2,     2,     2,     2,     7,
       1,     2,     1,     1,     6,     8,     8,     4,     0,     1,
       1,     3,     2,     0,     2,     1,     1,     1,     1,     7,
       1,     1,     0,     1,     1,     3,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END", "error", "$undefined", "CMP_LE", "CMP_LT", "CMP_EQ", "CMP_GT",
  "CMP_GE", "CMP_NE", "UNION", "EXCEPT", "INTERSECT", "UMINUS", "LIKE",
  "NOT", "OR", "AND", "IS", "NULLX", "BOOL", "BETWEEN", "IN", "STRING",
  "IDENT", "NUMERIC", "QB_NAME_IDENT", "ALL", "ANY", "AS", "ASC", "AVG",
  "BEGIN_HINT", "BEGIN_OUTLINE_DATA", "BIGINT", "BINARY", "BY", "CALL",
  "CASE", "CHAR", "COLUMN", "COLUMNS", "\",\"", "COUNT", "CREATE", "CSV",
  "DATABASE", "DATABASES", "DATE", "DATETIME", "DAY", "DECIMAL", "DELETE",
  "DENSE_RANK", "DESC", "DESCRIBE", "DISTINCT", "\"/\"", "DOUBLE", "DROP",
  "DUAL", "ELSE", "END_HINT", "END_OUTLINE_DATA", "END_SYM", "ENGINE",
  "EXEC", "EXISTS", "EXPLAIN", "EXPR_NORMALIZE", "FIELD", "FLOAT", "FROM",
  "FULL", "FUNCTION", "GROUP", "HAVING", "IF", "IFNULL", "INDEX", "INFILE",
  "INNER", "INSERT", "INT", "INTEGER", "INTERVAL", "INTO", "JOIN", "KILL",
  "LEADING", "LEFT", "LIMIT", "LINE", "\"(\"", "MAX", "MEDIUMINT",
  "MEMORY", "MERGE", "MIN", "\"-\"", "MONTH", "\"*\"", "NO_EXPR_NORMALIZE",
  "NO_MERGE", "NO_REWRITE", "NO_SIMPLIFY_SQ", "NO_UNNEST", "NO_USE_HASH",
  "NO_USE_NL", "NO_WIN_MAGIC", "NUMBER", "NUMERIC_SYM", "ON", "ORDER",
  "ORDERED", "OUTER", "OVER", "PACKAGE", "PARALLEL", "PARTITION", "\".\"",
  "\"+\"", "PROCEDURE", "PROCESSLIST", "RANK", "REAL", "REPLACE", "RETURN",
  "RIGHT", "ROWID", "ROW_NUMBER", "\")\"", "SAMPLE", "SELECT", "\";\"",
  "SET", "SHOW", "SIMPLIFY_SQ", "SIZE", "SMALLINT", "SPLIT", "STATIS",
  "STATUS", "SUBSTR", "SUM", "TABLE", "TABLES", "THEN", "TIME",
  "TIMESTAMP_SYM", "TINYINT", "TO_CHAR", "TO_NUMBER", "UNNEST", "UPDATE",
  "USE_HASH", "USE_NL", "USING", "VALUES", "VARBINARY", "VARCHAR", "VIEW",
  "WHEN", "WHERE", "WIN_MAGIC", "YEAR", "$accept", "sql_stmt", "stmt",
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
  "show_stmt", "op_from_database", "use_stmt", "desc_stmt", "kill_stmt",
  "create_package_stmt", "pro_or_func_list", "pro_or_func",
  "procedure_define", "function_define", "table_type", "opt_param_list",
  "param_list", "param", "opt_replace", "package_name", "procedure_name",
  "function_name", "param_name", "exec_package_stmt", "start_exec_package",
  "opt_arith_expr_list", "relation_factor", "database_name",
  "relation_name", "column_name", "column_label", "ident", "datetime",
  "string", "number", "int_value", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   363,   363,   371,   372,   373,   374,   375,   376,   377,
     386,   387,   388,   389,   390,   391,   392,   393,   397,   401,
     408,   413,   418,   423,   428,   433,   438,   443,   451,   455,
     462,   469,   490,   491,   497,   507,   510,   517,   524,   535,
     540,   545,   550,   555,   560,   565,   570,   575,   580,   587,
     593,   601,   609,   617,   625,   632,   642,   651,   652,   659,
     660,   664,   671,   676,   684,   685,   689,   695,   704,   709,
     717,   718,   722,   727,   735,   740,   748,   749,   753,   754,
     758,   759,   763,   764,   768,   773,   781,   788,   789,   790,
     794,   795,   802,   812,   819,   825,   831,   840,   845,   856,
     865,   869,   873,   877,   885,   888,   895,   899,   903,   910,
     914,   918,   925,   926,   930,   942,   946,   950,   954,   958,
     965,   975,   985,   998,  1002,  1007,  1012,  1017,  1022,  1027,
    1032,  1037,  1042,  1050,  1056,  1065,  1070,  1075,  1080,  1085,
    1090,  1101,  1112,  1117,  1122,  1126,  1131,  1139,  1143,  1147,
    1151,  1155,  1159,  1163,  1167,  1174,  1178,  1182,  1186,  1190,
    1194,  1198,  1202,  1206,  1210,  1214,  1218,  1225,  1234,  1241,
    1250,  1256,  1265,  1271,  1280,  1284,  1288,  1293,  1301,  1308,
    1314,  1322,  1329,  1337,  1348,  1357,  1370,  1379,  1391,  1416,
    1428,  1432,  1440,  1452,  1456,  1465,  1468,  1472,  1479,  1483,
    1487,  1491,  1495,  1499,  1503,  1507,  1515,  1518,  1526,  1529,
    1541,  1550,  1561,  1566,  1574,  1579,  1592,  1603,  1617,  1623,
    1632,  1647,  1658,  1669,  1687,  1696,  1697,  1698,  1699,  1708,
    1720,  1729,  1746,  1749,  1756,  1761,  1770,  1776,  1781,  1789,
    1799,  1802,  1809,  1811,  1813,  1815,  1817,  1819,  1821,  1823,
    1825,  1827,  1829,  1831,  1833,  1835,  1837,  1839,  1841,  1843,
    1845,  1847,  1849,  1851,  1853,  1855,  1860,  1861,  1872,  1873,
    1885,  1892,  1908,  1919,  1929,  1943,  1944,  1953,  1959,  1968,
    1977,  1987,  1993,  2002,  2007,  2013,  2030,  2040,  2059,  2076,
    2092,  2097,  2105,  2109,  2116,  2125,  2133,  2145,  2153,  2156,
    2163,  2168,  2176,  2185,  2186,  2190,  2194,  2198,  2202,  2206,
    2220,  2222,  2228,  2231,  2243,  2251,  2262,  2266,  2270,  2274,
    2278,  2282,  2283,  2287,  2291,  2295
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
#line 4994 "/home/lighthouse/CatDB/sql/parser/sql_parser.cpp" // lalr1.cc:1167
#line 2299 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:1168


void CatDB::SqlParser::error(const CatDB::location& location, const std::string& message)
{
	driver.set_syntax_error(message);
}
