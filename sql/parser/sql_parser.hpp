// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file /home/lighthouse/CatDB/sql/parser/sql_parser.hpp
 ** Define the CatDB::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_HOME_LIGHTHOUSE_CATDB_SQL_PARSER_SQL_PARSER_HPP_INCLUDED
# define YY_YY_HOME_LIGHTHOUSE_CATDB_SQL_PARSER_SQL_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 12 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:377

	#define YYDEBUG 1
	#define SqlParser parser 
	#include "select_stmt.h"
	#include "insert_stmt.h"
	#include "update_stmt.h"
	#include "delete_stmt.h"
	#include "dml_stmt.h"
	#include "cmd_stmt.h"
	#include "expr_stmt.h"
	#include "stmt.h"
	#include "type.h"
	/*避免包含头文件时冲突*/
	namespace CatDB {
		class SqlScanner;
		class SqlDriver;
	}
	using namespace CatDB::Parser;

#line 64 "/home/lighthouse/CatDB/sql/parser/sql_parser.hpp" // lalr1.cc:377

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"
#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 3 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:377
namespace CatDB {
#line 141 "/home/lighthouse/CatDB/sql/parser/sql_parser.hpp" // lalr1.cc:377



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
      : yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {
      YYASSERT (!yytypeid_);
    }

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *other.yytypeid_);
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };


  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // relation_factor
      char dummy1[sizeof(BasicTableStmt_s)];

      // column_definition
      char dummy2[sizeof(ColumnDefineStmt_s)];

      // projection
      // expr
      // in_expr
      // query_ref_expr
      // arith_expr
      // simple_expr
      // column_ref
      // expr_const
      // func_expr
      // case_when_expr
      // insert_value
      // update_asgn_factor
      char dummy3[sizeof(ExprStmt_s)];

      // opt_select_limit
      char dummy4[sizeof(LimitStmt_s)];

      // order_by
      char dummy5[sizeof(OrderStmt_s)];

      // sql_stmt
      // stmt
      // cmd_stmt
      // select_stmt
      // set_select
      // sub_set_select
      // select_with_parens
      // simple_select
      // insert_stmt
      // update_stmt
      // delete_stmt
      // explain_stmt
      // explainable_stmt
      // create_stmt
      // drop_stmt
      // show_stmt
      // use_stmt
      // desc_stmt
      // analyze_stmt
      char dummy6[sizeof(Stmt_s)];

      // table_factor
      // sub_table_factor
      char dummy7[sizeof(TableStmt_s)];

      // table_element_list
      char dummy8[sizeof(Vector<ColumnDefineStmt_s>)];

      // select_expr_list
      // opt_where
      // opt_groupby
      // opt_having
      // expr_list
      // when_then_list
      // insert_value_list
      // update_asgn_list
      char dummy9[sizeof(Vector<ExprStmt_s>)];

      // opt_order_by
      // order_by_list
      char dummy10[sizeof(Vector<OrderStmt_s>)];

      // from_list
      char dummy11[sizeof(Vector<TableStmt_s>)];

      // opt_distinct
      // opt_asc_desc
      // distinct_or_all
      char dummy12[sizeof(bool)];

      // opt_sample_size
      char dummy13[sizeof(double)];

      // limit_expr
      // data_type
      char dummy14[sizeof(int)];

      // STRING
      // IDENT
      // NUMERIC
      // TIMESTAMP
      // opt_alias
      // op_from_database
      // database_name
      // relation_name
      // column_name
      // function_name
      // column_label
      // ident
      // datetime
      // string
      // number
      char dummy15[sizeof(std::string)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        TOKEN_END = 0,
        TOKEN_UNION = 258,
        TOKEN_EXCEPT = 259,
        TOKEN_INTERSECT = 260,
        TOKEN_OR = 261,
        TOKEN_AND = 262,
        TOKEN_NOT = 263,
        TOKEN_CMP_LE = 264,
        TOKEN_CMP_LT = 265,
        TOKEN_CMP_EQ = 266,
        TOKEN_CMP_GT = 267,
        TOKEN_CMP_GE = 268,
        TOKEN_CMP_NE = 269,
        TOKEN_LIKE = 270,
        TOKEN_BETWEEN = 271,
        TOKEN_IN = 272,
        TOKEN_IS = 273,
        TOKEN_NULLX = 274,
        TOKEN_BOOL = 275,
        TOKEN_UMINUS = 276,
        TOKEN_PLUS = 277,
        TOKEN_MINUS = 278,
        TOKEN_MUL = 279,
        TOKEN_DIV = 280,
        TOKEN_LP = 281,
        TOKEN_RP = 282,
        TOKEN_COMMA = 283,
        TOKEN_PERIOD = 284,
        TOKEN_SEMICOLON = 285,
        TOKEN_STRING = 286,
        TOKEN_IDENT = 287,
        TOKEN_NUMERIC = 288,
        TOKEN_TIMESTAMP = 289,
        TOKEN_FALSE = 290,
        TOKEN_TRUE = 291,
        TOKEN_ANALYZE = 292,
        TOKEN_ANY = 293,
        TOKEN_ALL = 294,
        TOKEN_AS = 295,
        TOKEN_ASC = 296,
        TOKEN_BY = 297,
        TOKEN_COLUMN = 298,
        TOKEN_COLUMNS = 299,
        TOKEN_CREATE = 300,
        TOKEN_DATETIME = 301,
        TOKEN_DATABASE = 302,
        TOKEN_DATABASES = 303,
        TOKEN_DELETE = 304,
        TOKEN_DESC = 305,
        TOKEN_DESCRIBE = 306,
        TOKEN_DISTINCT = 307,
        TOKEN_DOUBLE = 308,
        TOKEN_DROP = 309,
        TOKEN_DUAL = 310,
        TOKEN_EXISTS = 311,
        TOKEN_EXPLAIN = 312,
        TOKEN_FLOAT = 313,
        TOKEN_FROM = 314,
        TOKEN_FULL = 315,
        TOKEN_GROUP = 316,
        TOKEN_HAVING = 317,
        TOKEN_INDEX = 318,
        TOKEN_INNER = 319,
        TOKEN_INSERT = 320,
        TOKEN_INT = 321,
        TOKEN_INTO = 322,
        TOKEN_JOIN = 323,
        TOKEN_LEFT = 324,
        TOKEN_LIMIT = 325,
        TOKEN_NUMBER = 326,
        TOKEN_ORDER = 327,
        TOKEN_ON = 328,
        TOKEN_PARALLEL = 329,
        TOKEN_RIGHT = 330,
        TOKEN_ROWID = 331,
        TOKEN_SAMPLE = 332,
        TOKEN_SELECT = 333,
        TOKEN_SET = 334,
        TOKEN_SHOW = 335,
        TOKEN_SIZE = 336,
        TOKEN_STATIS = 337,
        TOKEN_STATUS = 338,
        TOKEN_TABLE = 339,
        TOKEN_TABLES = 340,
        TOKEN_UPDATE = 341,
        TOKEN_USING = 342,
        TOKEN_VALUES = 343,
        TOKEN_VARCHAR = 344,
        TOKEN_WHERE = 345,
        TOKEN_TINYINT = 346,
        TOKEN_SMALLINT = 347,
        TOKEN_MEDIUMINT = 348,
        TOKEN_BIGINT = 349,
        TOKEN_DECIMAL = 350,
        TOKEN_NUMERIC_SYM = 351,
        TOKEN_REAL = 352,
        TOKEN_CHAR = 353,
        TOKEN_BINARY = 354,
        TOKEN_VARBINARY = 355,
        TOKEN_TIMESTAMP_SYM = 356,
        TOKEN_DATE = 357,
        TOKEN_TIME = 358,
        TOKEN_CASE = 359,
        TOKEN_WHEN = 360,
        TOKEN_THEN = 361,
        TOKEN_ELSE = 362
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const BasicTableStmt_s v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ColumnDefineStmt_s v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ExprStmt_s v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const LimitStmt_s v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const OrderStmt_s v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Stmt_s v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const TableStmt_s v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Vector<ColumnDefineStmt_s> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Vector<ExprStmt_s> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Vector<OrderStmt_s> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Vector<TableStmt_s> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const double v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const int v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_UNION (const location_type& l);

    static inline
    symbol_type
    make_EXCEPT (const location_type& l);

    static inline
    symbol_type
    make_INTERSECT (const location_type& l);

    static inline
    symbol_type
    make_OR (const location_type& l);

    static inline
    symbol_type
    make_AND (const location_type& l);

    static inline
    symbol_type
    make_NOT (const location_type& l);

    static inline
    symbol_type
    make_CMP_LE (const location_type& l);

    static inline
    symbol_type
    make_CMP_LT (const location_type& l);

    static inline
    symbol_type
    make_CMP_EQ (const location_type& l);

    static inline
    symbol_type
    make_CMP_GT (const location_type& l);

    static inline
    symbol_type
    make_CMP_GE (const location_type& l);

    static inline
    symbol_type
    make_CMP_NE (const location_type& l);

    static inline
    symbol_type
    make_LIKE (const location_type& l);

    static inline
    symbol_type
    make_BETWEEN (const location_type& l);

    static inline
    symbol_type
    make_IN (const location_type& l);

    static inline
    symbol_type
    make_IS (const location_type& l);

    static inline
    symbol_type
    make_NULLX (const location_type& l);

    static inline
    symbol_type
    make_BOOL (const location_type& l);

    static inline
    symbol_type
    make_UMINUS (const location_type& l);

    static inline
    symbol_type
    make_PLUS (const location_type& l);

    static inline
    symbol_type
    make_MINUS (const location_type& l);

    static inline
    symbol_type
    make_MUL (const location_type& l);

    static inline
    symbol_type
    make_DIV (const location_type& l);

    static inline
    symbol_type
    make_LP (const location_type& l);

    static inline
    symbol_type
    make_RP (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_PERIOD (const location_type& l);

    static inline
    symbol_type
    make_SEMICOLON (const location_type& l);

    static inline
    symbol_type
    make_STRING (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_IDENT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_NUMERIC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TIMESTAMP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_FALSE (const location_type& l);

    static inline
    symbol_type
    make_TRUE (const location_type& l);

    static inline
    symbol_type
    make_ANALYZE (const location_type& l);

    static inline
    symbol_type
    make_ANY (const location_type& l);

    static inline
    symbol_type
    make_ALL (const location_type& l);

    static inline
    symbol_type
    make_AS (const location_type& l);

    static inline
    symbol_type
    make_ASC (const location_type& l);

    static inline
    symbol_type
    make_BY (const location_type& l);

    static inline
    symbol_type
    make_COLUMN (const location_type& l);

    static inline
    symbol_type
    make_COLUMNS (const location_type& l);

    static inline
    symbol_type
    make_CREATE (const location_type& l);

    static inline
    symbol_type
    make_DATETIME (const location_type& l);

    static inline
    symbol_type
    make_DATABASE (const location_type& l);

    static inline
    symbol_type
    make_DATABASES (const location_type& l);

    static inline
    symbol_type
    make_DELETE (const location_type& l);

    static inline
    symbol_type
    make_DESC (const location_type& l);

    static inline
    symbol_type
    make_DESCRIBE (const location_type& l);

    static inline
    symbol_type
    make_DISTINCT (const location_type& l);

    static inline
    symbol_type
    make_DOUBLE (const location_type& l);

    static inline
    symbol_type
    make_DROP (const location_type& l);

    static inline
    symbol_type
    make_DUAL (const location_type& l);

    static inline
    symbol_type
    make_EXISTS (const location_type& l);

    static inline
    symbol_type
    make_EXPLAIN (const location_type& l);

    static inline
    symbol_type
    make_FLOAT (const location_type& l);

    static inline
    symbol_type
    make_FROM (const location_type& l);

    static inline
    symbol_type
    make_FULL (const location_type& l);

    static inline
    symbol_type
    make_GROUP (const location_type& l);

    static inline
    symbol_type
    make_HAVING (const location_type& l);

    static inline
    symbol_type
    make_INDEX (const location_type& l);

    static inline
    symbol_type
    make_INNER (const location_type& l);

    static inline
    symbol_type
    make_INSERT (const location_type& l);

    static inline
    symbol_type
    make_INT (const location_type& l);

    static inline
    symbol_type
    make_INTO (const location_type& l);

    static inline
    symbol_type
    make_JOIN (const location_type& l);

    static inline
    symbol_type
    make_LEFT (const location_type& l);

    static inline
    symbol_type
    make_LIMIT (const location_type& l);

    static inline
    symbol_type
    make_NUMBER (const location_type& l);

    static inline
    symbol_type
    make_ORDER (const location_type& l);

    static inline
    symbol_type
    make_ON (const location_type& l);

    static inline
    symbol_type
    make_PARALLEL (const location_type& l);

    static inline
    symbol_type
    make_RIGHT (const location_type& l);

    static inline
    symbol_type
    make_ROWID (const location_type& l);

    static inline
    symbol_type
    make_SAMPLE (const location_type& l);

    static inline
    symbol_type
    make_SELECT (const location_type& l);

    static inline
    symbol_type
    make_SET (const location_type& l);

    static inline
    symbol_type
    make_SHOW (const location_type& l);

    static inline
    symbol_type
    make_SIZE (const location_type& l);

    static inline
    symbol_type
    make_STATIS (const location_type& l);

    static inline
    symbol_type
    make_STATUS (const location_type& l);

    static inline
    symbol_type
    make_TABLE (const location_type& l);

    static inline
    symbol_type
    make_TABLES (const location_type& l);

    static inline
    symbol_type
    make_UPDATE (const location_type& l);

    static inline
    symbol_type
    make_USING (const location_type& l);

    static inline
    symbol_type
    make_VALUES (const location_type& l);

    static inline
    symbol_type
    make_VARCHAR (const location_type& l);

    static inline
    symbol_type
    make_WHERE (const location_type& l);

    static inline
    symbol_type
    make_TINYINT (const location_type& l);

    static inline
    symbol_type
    make_SMALLINT (const location_type& l);

    static inline
    symbol_type
    make_MEDIUMINT (const location_type& l);

    static inline
    symbol_type
    make_BIGINT (const location_type& l);

    static inline
    symbol_type
    make_DECIMAL (const location_type& l);

    static inline
    symbol_type
    make_NUMERIC_SYM (const location_type& l);

    static inline
    symbol_type
    make_REAL (const location_type& l);

    static inline
    symbol_type
    make_CHAR (const location_type& l);

    static inline
    symbol_type
    make_BINARY (const location_type& l);

    static inline
    symbol_type
    make_VARBINARY (const location_type& l);

    static inline
    symbol_type
    make_TIMESTAMP_SYM (const location_type& l);

    static inline
    symbol_type
    make_DATE (const location_type& l);

    static inline
    symbol_type
    make_TIME (const location_type& l);

    static inline
    symbol_type
    make_CASE (const location_type& l);

    static inline
    symbol_type
    make_WHEN (const location_type& l);

    static inline
    symbol_type
    make_THEN (const location_type& l);

    static inline
    symbol_type
    make_ELSE (const location_type& l);


    /// Build a parser object.
    parser (CatDB::SqlScanner& scanner_yyarg, CatDB::SqlDriver& driver_yyarg);
    virtual ~parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    parser (const parser&);
    parser& operator= (const parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short int yypact_ninf_;
    static const short int yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short int yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 919,     ///< Last index in yytable_.
      yynnts_ = 71,  ///< Number of nonterminal symbols.
      yyfinal_ = 67, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 109  ///< Number of tokens.
    };


    // User arguments.
    CatDB::SqlScanner& scanner;
    CatDB::SqlDriver& driver;
  };

  // Symbol number corresponding to token number t.
  inline
  parser::token_number_type
  parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   108,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107
    };
    const unsigned int user_token_number_max_ = 362;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 170: // relation_factor
        value.copy< BasicTableStmt_s > (other.value);
        break;

      case 157: // column_definition
        value.copy< ColumnDefineStmt_s > (other.value);
        break;

      case 130: // projection
      case 135: // expr
      case 136: // in_expr
      case 137: // query_ref_expr
      case 138: // arith_expr
      case 139: // simple_expr
      case 140: // column_ref
      case 141: // expr_const
      case 142: // func_expr
      case 144: // case_when_expr
      case 148: // insert_value
      case 151: // update_asgn_factor
        value.copy< ExprStmt_s > (other.value);
        break;

      case 128: // opt_select_limit
        value.copy< LimitStmt_s > (other.value);
        break;

      case 126: // order_by
        value.copy< OrderStmt_s > (other.value);
        break;

      case 110: // sql_stmt
      case 111: // stmt
      case 112: // cmd_stmt
      case 113: // select_stmt
      case 114: // set_select
      case 115: // sub_set_select
      case 116: // select_with_parens
      case 117: // simple_select
      case 146: // insert_stmt
      case 149: // update_stmt
      case 152: // delete_stmt
      case 153: // explain_stmt
      case 154: // explainable_stmt
      case 155: // create_stmt
      case 163: // drop_stmt
      case 164: // show_stmt
      case 166: // use_stmt
      case 167: // desc_stmt
      case 168: // analyze_stmt
        value.copy< Stmt_s > (other.value);
        break;

      case 131: // table_factor
      case 133: // sub_table_factor
        value.copy< TableStmt_s > (other.value);
        break;

      case 156: // table_element_list
        value.copy< Vector<ColumnDefineStmt_s> > (other.value);
        break;

      case 119: // select_expr_list
      case 121: // opt_where
      case 122: // opt_groupby
      case 123: // opt_having
      case 134: // expr_list
      case 145: // when_then_list
      case 147: // insert_value_list
      case 150: // update_asgn_list
        value.copy< Vector<ExprStmt_s> > (other.value);
        break;

      case 124: // opt_order_by
      case 125: // order_by_list
        value.copy< Vector<OrderStmt_s> > (other.value);
        break;

      case 120: // from_list
        value.copy< Vector<TableStmt_s> > (other.value);
        break;

      case 118: // opt_distinct
      case 127: // opt_asc_desc
      case 143: // distinct_or_all
        value.copy< bool > (other.value);
        break;

      case 169: // opt_sample_size
        value.copy< double > (other.value);
        break;

      case 129: // limit_expr
      case 158: // data_type
        value.copy< int > (other.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 132: // opt_alias
      case 165: // op_from_database
      case 171: // database_name
      case 172: // relation_name
      case 173: // column_name
      case 174: // function_name
      case 175: // column_label
      case 176: // ident
      case 177: // datetime
      case 178: // string
      case 179: // number
        value.copy< std::string > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 170: // relation_factor
        value.copy< BasicTableStmt_s > (v);
        break;

      case 157: // column_definition
        value.copy< ColumnDefineStmt_s > (v);
        break;

      case 130: // projection
      case 135: // expr
      case 136: // in_expr
      case 137: // query_ref_expr
      case 138: // arith_expr
      case 139: // simple_expr
      case 140: // column_ref
      case 141: // expr_const
      case 142: // func_expr
      case 144: // case_when_expr
      case 148: // insert_value
      case 151: // update_asgn_factor
        value.copy< ExprStmt_s > (v);
        break;

      case 128: // opt_select_limit
        value.copy< LimitStmt_s > (v);
        break;

      case 126: // order_by
        value.copy< OrderStmt_s > (v);
        break;

      case 110: // sql_stmt
      case 111: // stmt
      case 112: // cmd_stmt
      case 113: // select_stmt
      case 114: // set_select
      case 115: // sub_set_select
      case 116: // select_with_parens
      case 117: // simple_select
      case 146: // insert_stmt
      case 149: // update_stmt
      case 152: // delete_stmt
      case 153: // explain_stmt
      case 154: // explainable_stmt
      case 155: // create_stmt
      case 163: // drop_stmt
      case 164: // show_stmt
      case 166: // use_stmt
      case 167: // desc_stmt
      case 168: // analyze_stmt
        value.copy< Stmt_s > (v);
        break;

      case 131: // table_factor
      case 133: // sub_table_factor
        value.copy< TableStmt_s > (v);
        break;

      case 156: // table_element_list
        value.copy< Vector<ColumnDefineStmt_s> > (v);
        break;

      case 119: // select_expr_list
      case 121: // opt_where
      case 122: // opt_groupby
      case 123: // opt_having
      case 134: // expr_list
      case 145: // when_then_list
      case 147: // insert_value_list
      case 150: // update_asgn_list
        value.copy< Vector<ExprStmt_s> > (v);
        break;

      case 124: // opt_order_by
      case 125: // order_by_list
        value.copy< Vector<OrderStmt_s> > (v);
        break;

      case 120: // from_list
        value.copy< Vector<TableStmt_s> > (v);
        break;

      case 118: // opt_distinct
      case 127: // opt_asc_desc
      case 143: // distinct_or_all
        value.copy< bool > (v);
        break;

      case 169: // opt_sample_size
        value.copy< double > (v);
        break;

      case 129: // limit_expr
      case 158: // data_type
        value.copy< int > (v);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 132: // opt_alias
      case 165: // op_from_database
      case 171: // database_name
      case 172: // relation_name
      case 173: // column_name
      case 174: // function_name
      case 175: // column_label
      case 176: // ident
      case 177: // datetime
      case 178: // string
      case 179: // number
        value.copy< std::string > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const BasicTableStmt_s v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ColumnDefineStmt_s v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ExprStmt_s v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const LimitStmt_s v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const OrderStmt_s v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Stmt_s v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const TableStmt_s v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Vector<ColumnDefineStmt_s> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Vector<ExprStmt_s> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Vector<OrderStmt_s> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Vector<TableStmt_s> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const bool v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const double v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const int v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 170: // relation_factor
        value.template destroy< BasicTableStmt_s > ();
        break;

      case 157: // column_definition
        value.template destroy< ColumnDefineStmt_s > ();
        break;

      case 130: // projection
      case 135: // expr
      case 136: // in_expr
      case 137: // query_ref_expr
      case 138: // arith_expr
      case 139: // simple_expr
      case 140: // column_ref
      case 141: // expr_const
      case 142: // func_expr
      case 144: // case_when_expr
      case 148: // insert_value
      case 151: // update_asgn_factor
        value.template destroy< ExprStmt_s > ();
        break;

      case 128: // opt_select_limit
        value.template destroy< LimitStmt_s > ();
        break;

      case 126: // order_by
        value.template destroy< OrderStmt_s > ();
        break;

      case 110: // sql_stmt
      case 111: // stmt
      case 112: // cmd_stmt
      case 113: // select_stmt
      case 114: // set_select
      case 115: // sub_set_select
      case 116: // select_with_parens
      case 117: // simple_select
      case 146: // insert_stmt
      case 149: // update_stmt
      case 152: // delete_stmt
      case 153: // explain_stmt
      case 154: // explainable_stmt
      case 155: // create_stmt
      case 163: // drop_stmt
      case 164: // show_stmt
      case 166: // use_stmt
      case 167: // desc_stmt
      case 168: // analyze_stmt
        value.template destroy< Stmt_s > ();
        break;

      case 131: // table_factor
      case 133: // sub_table_factor
        value.template destroy< TableStmt_s > ();
        break;

      case 156: // table_element_list
        value.template destroy< Vector<ColumnDefineStmt_s> > ();
        break;

      case 119: // select_expr_list
      case 121: // opt_where
      case 122: // opt_groupby
      case 123: // opt_having
      case 134: // expr_list
      case 145: // when_then_list
      case 147: // insert_value_list
      case 150: // update_asgn_list
        value.template destroy< Vector<ExprStmt_s> > ();
        break;

      case 124: // opt_order_by
      case 125: // order_by_list
        value.template destroy< Vector<OrderStmt_s> > ();
        break;

      case 120: // from_list
        value.template destroy< Vector<TableStmt_s> > ();
        break;

      case 118: // opt_distinct
      case 127: // opt_asc_desc
      case 143: // distinct_or_all
        value.template destroy< bool > ();
        break;

      case 169: // opt_sample_size
        value.template destroy< double > ();
        break;

      case 129: // limit_expr
      case 158: // data_type
        value.template destroy< int > ();
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 132: // opt_alias
      case 165: // op_from_database
      case 171: // database_name
      case 172: // relation_name
      case 173: // column_name
      case 174: // function_name
      case 175: // column_label
      case 176: // ident
      case 177: // datetime
      case 178: // string
      case 179: // number
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 170: // relation_factor
        value.move< BasicTableStmt_s > (s.value);
        break;

      case 157: // column_definition
        value.move< ColumnDefineStmt_s > (s.value);
        break;

      case 130: // projection
      case 135: // expr
      case 136: // in_expr
      case 137: // query_ref_expr
      case 138: // arith_expr
      case 139: // simple_expr
      case 140: // column_ref
      case 141: // expr_const
      case 142: // func_expr
      case 144: // case_when_expr
      case 148: // insert_value
      case 151: // update_asgn_factor
        value.move< ExprStmt_s > (s.value);
        break;

      case 128: // opt_select_limit
        value.move< LimitStmt_s > (s.value);
        break;

      case 126: // order_by
        value.move< OrderStmt_s > (s.value);
        break;

      case 110: // sql_stmt
      case 111: // stmt
      case 112: // cmd_stmt
      case 113: // select_stmt
      case 114: // set_select
      case 115: // sub_set_select
      case 116: // select_with_parens
      case 117: // simple_select
      case 146: // insert_stmt
      case 149: // update_stmt
      case 152: // delete_stmt
      case 153: // explain_stmt
      case 154: // explainable_stmt
      case 155: // create_stmt
      case 163: // drop_stmt
      case 164: // show_stmt
      case 166: // use_stmt
      case 167: // desc_stmt
      case 168: // analyze_stmt
        value.move< Stmt_s > (s.value);
        break;

      case 131: // table_factor
      case 133: // sub_table_factor
        value.move< TableStmt_s > (s.value);
        break;

      case 156: // table_element_list
        value.move< Vector<ColumnDefineStmt_s> > (s.value);
        break;

      case 119: // select_expr_list
      case 121: // opt_where
      case 122: // opt_groupby
      case 123: // opt_having
      case 134: // expr_list
      case 145: // when_then_list
      case 147: // insert_value_list
      case 150: // update_asgn_list
        value.move< Vector<ExprStmt_s> > (s.value);
        break;

      case 124: // opt_order_by
      case 125: // order_by_list
        value.move< Vector<OrderStmt_s> > (s.value);
        break;

      case 120: // from_list
        value.move< Vector<TableStmt_s> > (s.value);
        break;

      case 118: // opt_distinct
      case 127: // opt_asc_desc
      case 143: // distinct_or_all
        value.move< bool > (s.value);
        break;

      case 169: // opt_sample_size
        value.move< double > (s.value);
        break;

      case 129: // limit_expr
      case 158: // data_type
        value.move< int > (s.value);
        break;

      case 31: // STRING
      case 32: // IDENT
      case 33: // NUMERIC
      case 34: // TIMESTAMP
      case 132: // opt_alias
      case 165: // op_from_database
      case 171: // database_name
      case 172: // relation_name
      case 173: // column_name
      case 174: // function_name
      case 175: // column_label
      case 176: // ident
      case 177: // datetime
      case 178: // string
      case 179: // number
        value.move< std::string > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const
  {
    return type;
  }

  inline
  parser::token_type
  parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,    44
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  parser::symbol_type
  parser::make_END (const location_type& l)
  {
    return symbol_type (token::TOKEN_END, l);
  }

  parser::symbol_type
  parser::make_UNION (const location_type& l)
  {
    return symbol_type (token::TOKEN_UNION, l);
  }

  parser::symbol_type
  parser::make_EXCEPT (const location_type& l)
  {
    return symbol_type (token::TOKEN_EXCEPT, l);
  }

  parser::symbol_type
  parser::make_INTERSECT (const location_type& l)
  {
    return symbol_type (token::TOKEN_INTERSECT, l);
  }

  parser::symbol_type
  parser::make_OR (const location_type& l)
  {
    return symbol_type (token::TOKEN_OR, l);
  }

  parser::symbol_type
  parser::make_AND (const location_type& l)
  {
    return symbol_type (token::TOKEN_AND, l);
  }

  parser::symbol_type
  parser::make_NOT (const location_type& l)
  {
    return symbol_type (token::TOKEN_NOT, l);
  }

  parser::symbol_type
  parser::make_CMP_LE (const location_type& l)
  {
    return symbol_type (token::TOKEN_CMP_LE, l);
  }

  parser::symbol_type
  parser::make_CMP_LT (const location_type& l)
  {
    return symbol_type (token::TOKEN_CMP_LT, l);
  }

  parser::symbol_type
  parser::make_CMP_EQ (const location_type& l)
  {
    return symbol_type (token::TOKEN_CMP_EQ, l);
  }

  parser::symbol_type
  parser::make_CMP_GT (const location_type& l)
  {
    return symbol_type (token::TOKEN_CMP_GT, l);
  }

  parser::symbol_type
  parser::make_CMP_GE (const location_type& l)
  {
    return symbol_type (token::TOKEN_CMP_GE, l);
  }

  parser::symbol_type
  parser::make_CMP_NE (const location_type& l)
  {
    return symbol_type (token::TOKEN_CMP_NE, l);
  }

  parser::symbol_type
  parser::make_LIKE (const location_type& l)
  {
    return symbol_type (token::TOKEN_LIKE, l);
  }

  parser::symbol_type
  parser::make_BETWEEN (const location_type& l)
  {
    return symbol_type (token::TOKEN_BETWEEN, l);
  }

  parser::symbol_type
  parser::make_IN (const location_type& l)
  {
    return symbol_type (token::TOKEN_IN, l);
  }

  parser::symbol_type
  parser::make_IS (const location_type& l)
  {
    return symbol_type (token::TOKEN_IS, l);
  }

  parser::symbol_type
  parser::make_NULLX (const location_type& l)
  {
    return symbol_type (token::TOKEN_NULLX, l);
  }

  parser::symbol_type
  parser::make_BOOL (const location_type& l)
  {
    return symbol_type (token::TOKEN_BOOL, l);
  }

  parser::symbol_type
  parser::make_UMINUS (const location_type& l)
  {
    return symbol_type (token::TOKEN_UMINUS, l);
  }

  parser::symbol_type
  parser::make_PLUS (const location_type& l)
  {
    return symbol_type (token::TOKEN_PLUS, l);
  }

  parser::symbol_type
  parser::make_MINUS (const location_type& l)
  {
    return symbol_type (token::TOKEN_MINUS, l);
  }

  parser::symbol_type
  parser::make_MUL (const location_type& l)
  {
    return symbol_type (token::TOKEN_MUL, l);
  }

  parser::symbol_type
  parser::make_DIV (const location_type& l)
  {
    return symbol_type (token::TOKEN_DIV, l);
  }

  parser::symbol_type
  parser::make_LP (const location_type& l)
  {
    return symbol_type (token::TOKEN_LP, l);
  }

  parser::symbol_type
  parser::make_RP (const location_type& l)
  {
    return symbol_type (token::TOKEN_RP, l);
  }

  parser::symbol_type
  parser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::TOKEN_COMMA, l);
  }

  parser::symbol_type
  parser::make_PERIOD (const location_type& l)
  {
    return symbol_type (token::TOKEN_PERIOD, l);
  }

  parser::symbol_type
  parser::make_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::TOKEN_SEMICOLON, l);
  }

  parser::symbol_type
  parser::make_STRING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOKEN_STRING, v, l);
  }

  parser::symbol_type
  parser::make_IDENT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOKEN_IDENT, v, l);
  }

  parser::symbol_type
  parser::make_NUMERIC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOKEN_NUMERIC, v, l);
  }

  parser::symbol_type
  parser::make_TIMESTAMP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOKEN_TIMESTAMP, v, l);
  }

  parser::symbol_type
  parser::make_FALSE (const location_type& l)
  {
    return symbol_type (token::TOKEN_FALSE, l);
  }

  parser::symbol_type
  parser::make_TRUE (const location_type& l)
  {
    return symbol_type (token::TOKEN_TRUE, l);
  }

  parser::symbol_type
  parser::make_ANALYZE (const location_type& l)
  {
    return symbol_type (token::TOKEN_ANALYZE, l);
  }

  parser::symbol_type
  parser::make_ANY (const location_type& l)
  {
    return symbol_type (token::TOKEN_ANY, l);
  }

  parser::symbol_type
  parser::make_ALL (const location_type& l)
  {
    return symbol_type (token::TOKEN_ALL, l);
  }

  parser::symbol_type
  parser::make_AS (const location_type& l)
  {
    return symbol_type (token::TOKEN_AS, l);
  }

  parser::symbol_type
  parser::make_ASC (const location_type& l)
  {
    return symbol_type (token::TOKEN_ASC, l);
  }

  parser::symbol_type
  parser::make_BY (const location_type& l)
  {
    return symbol_type (token::TOKEN_BY, l);
  }

  parser::symbol_type
  parser::make_COLUMN (const location_type& l)
  {
    return symbol_type (token::TOKEN_COLUMN, l);
  }

  parser::symbol_type
  parser::make_COLUMNS (const location_type& l)
  {
    return symbol_type (token::TOKEN_COLUMNS, l);
  }

  parser::symbol_type
  parser::make_CREATE (const location_type& l)
  {
    return symbol_type (token::TOKEN_CREATE, l);
  }

  parser::symbol_type
  parser::make_DATETIME (const location_type& l)
  {
    return symbol_type (token::TOKEN_DATETIME, l);
  }

  parser::symbol_type
  parser::make_DATABASE (const location_type& l)
  {
    return symbol_type (token::TOKEN_DATABASE, l);
  }

  parser::symbol_type
  parser::make_DATABASES (const location_type& l)
  {
    return symbol_type (token::TOKEN_DATABASES, l);
  }

  parser::symbol_type
  parser::make_DELETE (const location_type& l)
  {
    return symbol_type (token::TOKEN_DELETE, l);
  }

  parser::symbol_type
  parser::make_DESC (const location_type& l)
  {
    return symbol_type (token::TOKEN_DESC, l);
  }

  parser::symbol_type
  parser::make_DESCRIBE (const location_type& l)
  {
    return symbol_type (token::TOKEN_DESCRIBE, l);
  }

  parser::symbol_type
  parser::make_DISTINCT (const location_type& l)
  {
    return symbol_type (token::TOKEN_DISTINCT, l);
  }

  parser::symbol_type
  parser::make_DOUBLE (const location_type& l)
  {
    return symbol_type (token::TOKEN_DOUBLE, l);
  }

  parser::symbol_type
  parser::make_DROP (const location_type& l)
  {
    return symbol_type (token::TOKEN_DROP, l);
  }

  parser::symbol_type
  parser::make_DUAL (const location_type& l)
  {
    return symbol_type (token::TOKEN_DUAL, l);
  }

  parser::symbol_type
  parser::make_EXISTS (const location_type& l)
  {
    return symbol_type (token::TOKEN_EXISTS, l);
  }

  parser::symbol_type
  parser::make_EXPLAIN (const location_type& l)
  {
    return symbol_type (token::TOKEN_EXPLAIN, l);
  }

  parser::symbol_type
  parser::make_FLOAT (const location_type& l)
  {
    return symbol_type (token::TOKEN_FLOAT, l);
  }

  parser::symbol_type
  parser::make_FROM (const location_type& l)
  {
    return symbol_type (token::TOKEN_FROM, l);
  }

  parser::symbol_type
  parser::make_FULL (const location_type& l)
  {
    return symbol_type (token::TOKEN_FULL, l);
  }

  parser::symbol_type
  parser::make_GROUP (const location_type& l)
  {
    return symbol_type (token::TOKEN_GROUP, l);
  }

  parser::symbol_type
  parser::make_HAVING (const location_type& l)
  {
    return symbol_type (token::TOKEN_HAVING, l);
  }

  parser::symbol_type
  parser::make_INDEX (const location_type& l)
  {
    return symbol_type (token::TOKEN_INDEX, l);
  }

  parser::symbol_type
  parser::make_INNER (const location_type& l)
  {
    return symbol_type (token::TOKEN_INNER, l);
  }

  parser::symbol_type
  parser::make_INSERT (const location_type& l)
  {
    return symbol_type (token::TOKEN_INSERT, l);
  }

  parser::symbol_type
  parser::make_INT (const location_type& l)
  {
    return symbol_type (token::TOKEN_INT, l);
  }

  parser::symbol_type
  parser::make_INTO (const location_type& l)
  {
    return symbol_type (token::TOKEN_INTO, l);
  }

  parser::symbol_type
  parser::make_JOIN (const location_type& l)
  {
    return symbol_type (token::TOKEN_JOIN, l);
  }

  parser::symbol_type
  parser::make_LEFT (const location_type& l)
  {
    return symbol_type (token::TOKEN_LEFT, l);
  }

  parser::symbol_type
  parser::make_LIMIT (const location_type& l)
  {
    return symbol_type (token::TOKEN_LIMIT, l);
  }

  parser::symbol_type
  parser::make_NUMBER (const location_type& l)
  {
    return symbol_type (token::TOKEN_NUMBER, l);
  }

  parser::symbol_type
  parser::make_ORDER (const location_type& l)
  {
    return symbol_type (token::TOKEN_ORDER, l);
  }

  parser::symbol_type
  parser::make_ON (const location_type& l)
  {
    return symbol_type (token::TOKEN_ON, l);
  }

  parser::symbol_type
  parser::make_PARALLEL (const location_type& l)
  {
    return symbol_type (token::TOKEN_PARALLEL, l);
  }

  parser::symbol_type
  parser::make_RIGHT (const location_type& l)
  {
    return symbol_type (token::TOKEN_RIGHT, l);
  }

  parser::symbol_type
  parser::make_ROWID (const location_type& l)
  {
    return symbol_type (token::TOKEN_ROWID, l);
  }

  parser::symbol_type
  parser::make_SAMPLE (const location_type& l)
  {
    return symbol_type (token::TOKEN_SAMPLE, l);
  }

  parser::symbol_type
  parser::make_SELECT (const location_type& l)
  {
    return symbol_type (token::TOKEN_SELECT, l);
  }

  parser::symbol_type
  parser::make_SET (const location_type& l)
  {
    return symbol_type (token::TOKEN_SET, l);
  }

  parser::symbol_type
  parser::make_SHOW (const location_type& l)
  {
    return symbol_type (token::TOKEN_SHOW, l);
  }

  parser::symbol_type
  parser::make_SIZE (const location_type& l)
  {
    return symbol_type (token::TOKEN_SIZE, l);
  }

  parser::symbol_type
  parser::make_STATIS (const location_type& l)
  {
    return symbol_type (token::TOKEN_STATIS, l);
  }

  parser::symbol_type
  parser::make_STATUS (const location_type& l)
  {
    return symbol_type (token::TOKEN_STATUS, l);
  }

  parser::symbol_type
  parser::make_TABLE (const location_type& l)
  {
    return symbol_type (token::TOKEN_TABLE, l);
  }

  parser::symbol_type
  parser::make_TABLES (const location_type& l)
  {
    return symbol_type (token::TOKEN_TABLES, l);
  }

  parser::symbol_type
  parser::make_UPDATE (const location_type& l)
  {
    return symbol_type (token::TOKEN_UPDATE, l);
  }

  parser::symbol_type
  parser::make_USING (const location_type& l)
  {
    return symbol_type (token::TOKEN_USING, l);
  }

  parser::symbol_type
  parser::make_VALUES (const location_type& l)
  {
    return symbol_type (token::TOKEN_VALUES, l);
  }

  parser::symbol_type
  parser::make_VARCHAR (const location_type& l)
  {
    return symbol_type (token::TOKEN_VARCHAR, l);
  }

  parser::symbol_type
  parser::make_WHERE (const location_type& l)
  {
    return symbol_type (token::TOKEN_WHERE, l);
  }

  parser::symbol_type
  parser::make_TINYINT (const location_type& l)
  {
    return symbol_type (token::TOKEN_TINYINT, l);
  }

  parser::symbol_type
  parser::make_SMALLINT (const location_type& l)
  {
    return symbol_type (token::TOKEN_SMALLINT, l);
  }

  parser::symbol_type
  parser::make_MEDIUMINT (const location_type& l)
  {
    return symbol_type (token::TOKEN_MEDIUMINT, l);
  }

  parser::symbol_type
  parser::make_BIGINT (const location_type& l)
  {
    return symbol_type (token::TOKEN_BIGINT, l);
  }

  parser::symbol_type
  parser::make_DECIMAL (const location_type& l)
  {
    return symbol_type (token::TOKEN_DECIMAL, l);
  }

  parser::symbol_type
  parser::make_NUMERIC_SYM (const location_type& l)
  {
    return symbol_type (token::TOKEN_NUMERIC_SYM, l);
  }

  parser::symbol_type
  parser::make_REAL (const location_type& l)
  {
    return symbol_type (token::TOKEN_REAL, l);
  }

  parser::symbol_type
  parser::make_CHAR (const location_type& l)
  {
    return symbol_type (token::TOKEN_CHAR, l);
  }

  parser::symbol_type
  parser::make_BINARY (const location_type& l)
  {
    return symbol_type (token::TOKEN_BINARY, l);
  }

  parser::symbol_type
  parser::make_VARBINARY (const location_type& l)
  {
    return symbol_type (token::TOKEN_VARBINARY, l);
  }

  parser::symbol_type
  parser::make_TIMESTAMP_SYM (const location_type& l)
  {
    return symbol_type (token::TOKEN_TIMESTAMP_SYM, l);
  }

  parser::symbol_type
  parser::make_DATE (const location_type& l)
  {
    return symbol_type (token::TOKEN_DATE, l);
  }

  parser::symbol_type
  parser::make_TIME (const location_type& l)
  {
    return symbol_type (token::TOKEN_TIME, l);
  }

  parser::symbol_type
  parser::make_CASE (const location_type& l)
  {
    return symbol_type (token::TOKEN_CASE, l);
  }

  parser::symbol_type
  parser::make_WHEN (const location_type& l)
  {
    return symbol_type (token::TOKEN_WHEN, l);
  }

  parser::symbol_type
  parser::make_THEN (const location_type& l)
  {
    return symbol_type (token::TOKEN_THEN, l);
  }

  parser::symbol_type
  parser::make_ELSE (const location_type& l)
  {
    return symbol_type (token::TOKEN_ELSE, l);
  }


#line 3 "/home/lighthouse/CatDB/sql/parser/sql_parser.y" // lalr1.cc:377
} // CatDB
#line 2747 "/home/lighthouse/CatDB/sql/parser/sql_parser.hpp" // lalr1.cc:377




#endif // !YY_YY_HOME_LIGHTHOUSE_CATDB_SQL_PARSER_SQL_PARSER_HPP_INCLUDED