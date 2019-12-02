// A Bison parser, made by GNU Bison 3.4.1.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file D:/CatDB/parser/lex_yacc/sql_parser.hpp
 ** Define the CatDB::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_D_CATDB_PARSER_LEX_YACC_SQL_PARSER_HPP_INCLUDED
# define YY_YY_D_CATDB_PARSER_LEX_YACC_SQL_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 12 "D:/CatDB/parser/lex_yacc/sql_parser.y"

	#define YYDEBUG 1
	#define SqlParser parser 
	#include "stmt.h"
	#include "type.h"
	/*避免包含头文件时冲突*/
	namespace CatDB {
		class SqlScanner;
		class SqlDriver;
	}
	using namespace CatDB::Parser;

#line 61 "D:/CatDB/parser/lex_yacc/sql_parser.hpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

#line 3 "D:/CatDB/parser/lex_yacc/sql_parser.y"
namespace CatDB {
#line 177 "D:/CatDB/parser/lex_yacc/sql_parser.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {
      YYASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
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
    self_type& operator= (const self_type&);
    semantic_type (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // sql_stmt
      // stmt
      // select_stmt
      // select_with_parens
      // select_no_parens
      // select_expr_list
      // from_list
      // opt_where
      // opt_groupby
      // opt_having
      // opt_order_by
      // opt_select_limit
      // order_by
      // projection
      // table_factor
      // expr_list
      // expr
      // in_expr
      // arith_expr
      // simple_expr
      // column_ref
      // expr_const
      // func_expr
      // insert_stmt
      // insert_vals_list
      // insert_vals
      // update_stmt
      // update_asgn_list
      // update_asgn_factor
      // delete_stmt
      // explain_stmt
      // explainable_stmt
      // create_stmt
      // table_element_list
      // table_element
      // column_definition
      // drop_stmt
      // show_stmt
      // use_stmt
      // desc_stmt
      // analyze_stmt
      // relation_factor
      char dummy1[sizeof (Stmt_s)];

      // opt_distinct
      // opt_asc_desc
      // distinct_or_all
      char dummy2[sizeof (bool)];

      // number
      char dummy3[sizeof (double)];

      // limit_expr
      // data_type
      char dummy4[sizeof (int)];

      // STRING
      // IDENT
      // NUMERIC
      // TIMESTAMP
      // simple_function_expr
      // op_from_database
      // database_name
      // relation_name
      // column_name
      // function_name
      // column_label
      // ident
      // datetime
      // string
      char dummy5[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

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
        TOKEN_COLUMNS = 298,
        TOKEN_CREATE = 299,
        TOKEN_DATETIME = 300,
        TOKEN_DATABASE = 301,
        TOKEN_DATABASES = 302,
        TOKEN_DELETE = 303,
        TOKEN_DESC = 304,
        TOKEN_DESCRIBE = 305,
        TOKEN_DISTINCT = 306,
        TOKEN_DOUBLE = 307,
        TOKEN_DROP = 308,
        TOKEN_EXISTS = 309,
        TOKEN_EXPLAIN = 310,
        TOKEN_FLOAT = 311,
        TOKEN_FROM = 312,
        TOKEN_GROUP = 313,
        TOKEN_HAVING = 314,
        TOKEN_INDEX = 315,
        TOKEN_INSERT = 316,
        TOKEN_INT = 317,
        TOKEN_INTO = 318,
        TOKEN_LIMIT = 319,
        TOKEN_NUMBER = 320,
        TOKEN_ORDER = 321,
        TOKEN_SELECT = 322,
        TOKEN_SET = 323,
        TOKEN_SHOW = 324,
        TOKEN_STATUS = 325,
        TOKEN_TABLE = 326,
        TOKEN_TABLES = 327,
        TOKEN_UPDATE = 328,
        TOKEN_USING = 329,
        TOKEN_VALUES = 330,
        TOKEN_VARCHAR = 331,
        TOKEN_WHERE = 332,
        TOKEN_TINYINT = 333,
        TOKEN_SMALLINT = 334,
        TOKEN_MEDIUMINT = 335,
        TOKEN_BIGINT = 336,
        TOKEN_DECIMAL = 337,
        TOKEN_NUMERIC_SYM = 338,
        TOKEN_REAL = 339,
        TOKEN_CHAR = 340,
        TOKEN_BINARY = 341,
        TOKEN_VARBINARY = 342,
        TOKEN_TIMESTAMP_SYM = 343,
        TOKEN_DATE = 344,
        TOKEN_TIME = 345
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
    /// via type_get ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Stmt_s&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Stmt_s& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, bool&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const bool& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, double&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const double& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
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
        value.template destroy< Stmt_s > ();
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        value.template destroy< bool > ();
        break;

      case 154: // number
        value.template destroy< double > ();
        break;

      case 107: // limit_expr
      case 134: // data_type
        value.template destroy< int > ();
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
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

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
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The token.
      token_type token () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {
      /// Superclass.
      typedef basic_symbol<by_type> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YYASSERT (tok == token::TOKEN_END || tok == token::TOKEN_UNION || tok == token::TOKEN_EXCEPT || tok == token::TOKEN_INTERSECT || tok == token::TOKEN_OR || tok == token::TOKEN_AND || tok == token::TOKEN_NOT || tok == token::TOKEN_CMP_LE || tok == token::TOKEN_CMP_LT || tok == token::TOKEN_CMP_EQ || tok == token::TOKEN_CMP_GT || tok == token::TOKEN_CMP_GE || tok == token::TOKEN_CMP_NE || tok == token::TOKEN_LIKE || tok == token::TOKEN_BETWEEN || tok == token::TOKEN_IN || tok == token::TOKEN_IS || tok == token::TOKEN_NULLX || tok == token::TOKEN_BOOL || tok == token::TOKEN_UMINUS || tok == token::TOKEN_PLUS || tok == token::TOKEN_MINUS || tok == token::TOKEN_MUL || tok == token::TOKEN_DIV || tok == token::TOKEN_LP || tok == token::TOKEN_RP || tok == token::TOKEN_COMMA || tok == token::TOKEN_PERIOD || tok == token::TOKEN_SEMICOLON || tok == token::TOKEN_FALSE || tok == token::TOKEN_TRUE || tok == token::TOKEN_ANALYZE || tok == token::TOKEN_ANY || tok == token::TOKEN_ALL || tok == token::TOKEN_AS || tok == token::TOKEN_ASC || tok == token::TOKEN_BY || tok == token::TOKEN_COLUMNS || tok == token::TOKEN_CREATE || tok == token::TOKEN_DATETIME || tok == token::TOKEN_DATABASE || tok == token::TOKEN_DATABASES || tok == token::TOKEN_DELETE || tok == token::TOKEN_DESC || tok == token::TOKEN_DESCRIBE || tok == token::TOKEN_DISTINCT || tok == token::TOKEN_DOUBLE || tok == token::TOKEN_DROP || tok == token::TOKEN_EXISTS || tok == token::TOKEN_EXPLAIN || tok == token::TOKEN_FLOAT || tok == token::TOKEN_FROM || tok == token::TOKEN_GROUP || tok == token::TOKEN_HAVING || tok == token::TOKEN_INDEX || tok == token::TOKEN_INSERT || tok == token::TOKEN_INT || tok == token::TOKEN_INTO || tok == token::TOKEN_LIMIT || tok == token::TOKEN_NUMBER || tok == token::TOKEN_ORDER || tok == token::TOKEN_SELECT || tok == token::TOKEN_SET || tok == token::TOKEN_SHOW || tok == token::TOKEN_STATUS || tok == token::TOKEN_TABLE || tok == token::TOKEN_TABLES || tok == token::TOKEN_UPDATE || tok == token::TOKEN_USING || tok == token::TOKEN_VALUES || tok == token::TOKEN_VARCHAR || tok == token::TOKEN_WHERE || tok == token::TOKEN_TINYINT || tok == token::TOKEN_SMALLINT || tok == token::TOKEN_MEDIUMINT || tok == token::TOKEN_BIGINT || tok == token::TOKEN_DECIMAL || tok == token::TOKEN_NUMERIC_SYM || tok == token::TOKEN_REAL || tok == token::TOKEN_CHAR || tok == token::TOKEN_BINARY || tok == token::TOKEN_VARBINARY || tok == token::TOKEN_TIMESTAMP_SYM || tok == token::TOKEN_DATE || tok == token::TOKEN_TIME || tok == 44);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YYASSERT (tok == token::TOKEN_END || tok == token::TOKEN_UNION || tok == token::TOKEN_EXCEPT || tok == token::TOKEN_INTERSECT || tok == token::TOKEN_OR || tok == token::TOKEN_AND || tok == token::TOKEN_NOT || tok == token::TOKEN_CMP_LE || tok == token::TOKEN_CMP_LT || tok == token::TOKEN_CMP_EQ || tok == token::TOKEN_CMP_GT || tok == token::TOKEN_CMP_GE || tok == token::TOKEN_CMP_NE || tok == token::TOKEN_LIKE || tok == token::TOKEN_BETWEEN || tok == token::TOKEN_IN || tok == token::TOKEN_IS || tok == token::TOKEN_NULLX || tok == token::TOKEN_BOOL || tok == token::TOKEN_UMINUS || tok == token::TOKEN_PLUS || tok == token::TOKEN_MINUS || tok == token::TOKEN_MUL || tok == token::TOKEN_DIV || tok == token::TOKEN_LP || tok == token::TOKEN_RP || tok == token::TOKEN_COMMA || tok == token::TOKEN_PERIOD || tok == token::TOKEN_SEMICOLON || tok == token::TOKEN_FALSE || tok == token::TOKEN_TRUE || tok == token::TOKEN_ANALYZE || tok == token::TOKEN_ANY || tok == token::TOKEN_ALL || tok == token::TOKEN_AS || tok == token::TOKEN_ASC || tok == token::TOKEN_BY || tok == token::TOKEN_COLUMNS || tok == token::TOKEN_CREATE || tok == token::TOKEN_DATETIME || tok == token::TOKEN_DATABASE || tok == token::TOKEN_DATABASES || tok == token::TOKEN_DELETE || tok == token::TOKEN_DESC || tok == token::TOKEN_DESCRIBE || tok == token::TOKEN_DISTINCT || tok == token::TOKEN_DOUBLE || tok == token::TOKEN_DROP || tok == token::TOKEN_EXISTS || tok == token::TOKEN_EXPLAIN || tok == token::TOKEN_FLOAT || tok == token::TOKEN_FROM || tok == token::TOKEN_GROUP || tok == token::TOKEN_HAVING || tok == token::TOKEN_INDEX || tok == token::TOKEN_INSERT || tok == token::TOKEN_INT || tok == token::TOKEN_INTO || tok == token::TOKEN_LIMIT || tok == token::TOKEN_NUMBER || tok == token::TOKEN_ORDER || tok == token::TOKEN_SELECT || tok == token::TOKEN_SET || tok == token::TOKEN_SHOW || tok == token::TOKEN_STATUS || tok == token::TOKEN_TABLE || tok == token::TOKEN_TABLES || tok == token::TOKEN_UPDATE || tok == token::TOKEN_USING || tok == token::TOKEN_VALUES || tok == token::TOKEN_VARCHAR || tok == token::TOKEN_WHERE || tok == token::TOKEN_TINYINT || tok == token::TOKEN_SMALLINT || tok == token::TOKEN_MEDIUMINT || tok == token::TOKEN_BIGINT || tok == token::TOKEN_DECIMAL || tok == token::TOKEN_NUMERIC_SYM || tok == token::TOKEN_REAL || tok == token::TOKEN_CHAR || tok == token::TOKEN_BINARY || tok == token::TOKEN_VARBINARY || tok == token::TOKEN_TIMESTAMP_SYM || tok == token::TOKEN_DATE || tok == token::TOKEN_TIME || tok == 44);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::TOKEN_STRING || tok == token::TOKEN_IDENT || tok == token::TOKEN_NUMERIC || tok == token::TOKEN_TIMESTAMP);
      }
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::TOKEN_STRING || tok == token::TOKEN_IDENT || tok == token::TOKEN_NUMERIC || tok == token::TOKEN_TIMESTAMP);
      }
#endif
    };

    /// Build a parser object.
    parser (CatDB::SqlScanner& scanner_yyarg, CatDB::SqlDriver& driver_yyarg);
    virtual ~parser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

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

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::TOKEN_END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::TOKEN_END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNION (location_type l)
      {
        return symbol_type (token::TOKEN_UNION, std::move (l));
      }
#else
      static
      symbol_type
      make_UNION (const location_type& l)
      {
        return symbol_type (token::TOKEN_UNION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXCEPT (location_type l)
      {
        return symbol_type (token::TOKEN_EXCEPT, std::move (l));
      }
#else
      static
      symbol_type
      make_EXCEPT (const location_type& l)
      {
        return symbol_type (token::TOKEN_EXCEPT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTERSECT (location_type l)
      {
        return symbol_type (token::TOKEN_INTERSECT, std::move (l));
      }
#else
      static
      symbol_type
      make_INTERSECT (const location_type& l)
      {
        return symbol_type (token::TOKEN_INTERSECT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OR (location_type l)
      {
        return symbol_type (token::TOKEN_OR, std::move (l));
      }
#else
      static
      symbol_type
      make_OR (const location_type& l)
      {
        return symbol_type (token::TOKEN_OR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AND (location_type l)
      {
        return symbol_type (token::TOKEN_AND, std::move (l));
      }
#else
      static
      symbol_type
      make_AND (const location_type& l)
      {
        return symbol_type (token::TOKEN_AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOT (location_type l)
      {
        return symbol_type (token::TOKEN_NOT, std::move (l));
      }
#else
      static
      symbol_type
      make_NOT (const location_type& l)
      {
        return symbol_type (token::TOKEN_NOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CMP_LE (location_type l)
      {
        return symbol_type (token::TOKEN_CMP_LE, std::move (l));
      }
#else
      static
      symbol_type
      make_CMP_LE (const location_type& l)
      {
        return symbol_type (token::TOKEN_CMP_LE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CMP_LT (location_type l)
      {
        return symbol_type (token::TOKEN_CMP_LT, std::move (l));
      }
#else
      static
      symbol_type
      make_CMP_LT (const location_type& l)
      {
        return symbol_type (token::TOKEN_CMP_LT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CMP_EQ (location_type l)
      {
        return symbol_type (token::TOKEN_CMP_EQ, std::move (l));
      }
#else
      static
      symbol_type
      make_CMP_EQ (const location_type& l)
      {
        return symbol_type (token::TOKEN_CMP_EQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CMP_GT (location_type l)
      {
        return symbol_type (token::TOKEN_CMP_GT, std::move (l));
      }
#else
      static
      symbol_type
      make_CMP_GT (const location_type& l)
      {
        return symbol_type (token::TOKEN_CMP_GT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CMP_GE (location_type l)
      {
        return symbol_type (token::TOKEN_CMP_GE, std::move (l));
      }
#else
      static
      symbol_type
      make_CMP_GE (const location_type& l)
      {
        return symbol_type (token::TOKEN_CMP_GE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CMP_NE (location_type l)
      {
        return symbol_type (token::TOKEN_CMP_NE, std::move (l));
      }
#else
      static
      symbol_type
      make_CMP_NE (const location_type& l)
      {
        return symbol_type (token::TOKEN_CMP_NE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LIKE (location_type l)
      {
        return symbol_type (token::TOKEN_LIKE, std::move (l));
      }
#else
      static
      symbol_type
      make_LIKE (const location_type& l)
      {
        return symbol_type (token::TOKEN_LIKE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BETWEEN (location_type l)
      {
        return symbol_type (token::TOKEN_BETWEEN, std::move (l));
      }
#else
      static
      symbol_type
      make_BETWEEN (const location_type& l)
      {
        return symbol_type (token::TOKEN_BETWEEN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IN (location_type l)
      {
        return symbol_type (token::TOKEN_IN, std::move (l));
      }
#else
      static
      symbol_type
      make_IN (const location_type& l)
      {
        return symbol_type (token::TOKEN_IN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IS (location_type l)
      {
        return symbol_type (token::TOKEN_IS, std::move (l));
      }
#else
      static
      symbol_type
      make_IS (const location_type& l)
      {
        return symbol_type (token::TOKEN_IS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NULLX (location_type l)
      {
        return symbol_type (token::TOKEN_NULLX, std::move (l));
      }
#else
      static
      symbol_type
      make_NULLX (const location_type& l)
      {
        return symbol_type (token::TOKEN_NULLX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL (location_type l)
      {
        return symbol_type (token::TOKEN_BOOL, std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL (const location_type& l)
      {
        return symbol_type (token::TOKEN_BOOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UMINUS (location_type l)
      {
        return symbol_type (token::TOKEN_UMINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_UMINUS (const location_type& l)
      {
        return symbol_type (token::TOKEN_UMINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::TOKEN_PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::TOKEN_PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (location_type l)
      {
        return symbol_type (token::TOKEN_MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::TOKEN_MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MUL (location_type l)
      {
        return symbol_type (token::TOKEN_MUL, std::move (l));
      }
#else
      static
      symbol_type
      make_MUL (const location_type& l)
      {
        return symbol_type (token::TOKEN_MUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIV (location_type l)
      {
        return symbol_type (token::TOKEN_DIV, std::move (l));
      }
#else
      static
      symbol_type
      make_DIV (const location_type& l)
      {
        return symbol_type (token::TOKEN_DIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LP (location_type l)
      {
        return symbol_type (token::TOKEN_LP, std::move (l));
      }
#else
      static
      symbol_type
      make_LP (const location_type& l)
      {
        return symbol_type (token::TOKEN_LP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RP (location_type l)
      {
        return symbol_type (token::TOKEN_RP, std::move (l));
      }
#else
      static
      symbol_type
      make_RP (const location_type& l)
      {
        return symbol_type (token::TOKEN_RP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::TOKEN_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::TOKEN_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERIOD (location_type l)
      {
        return symbol_type (token::TOKEN_PERIOD, std::move (l));
      }
#else
      static
      symbol_type
      make_PERIOD (const location_type& l)
      {
        return symbol_type (token::TOKEN_PERIOD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMICOLON (location_type l)
      {
        return symbol_type (token::TOKEN_SEMICOLON, std::move (l));
      }
#else
      static
      symbol_type
      make_SEMICOLON (const location_type& l)
      {
        return symbol_type (token::TOKEN_SEMICOLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRING (std::string v, location_type l)
      {
        return symbol_type (token::TOKEN_STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOKEN_STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENT (std::string v, location_type l)
      {
        return symbol_type (token::TOKEN_IDENT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOKEN_IDENT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NUMERIC (std::string v, location_type l)
      {
        return symbol_type (token::TOKEN_NUMERIC, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NUMERIC (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOKEN_NUMERIC, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TIMESTAMP (std::string v, location_type l)
      {
        return symbol_type (token::TOKEN_TIMESTAMP, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TIMESTAMP (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOKEN_TIMESTAMP, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FALSE (location_type l)
      {
        return symbol_type (token::TOKEN_FALSE, std::move (l));
      }
#else
      static
      symbol_type
      make_FALSE (const location_type& l)
      {
        return symbol_type (token::TOKEN_FALSE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TRUE (location_type l)
      {
        return symbol_type (token::TOKEN_TRUE, std::move (l));
      }
#else
      static
      symbol_type
      make_TRUE (const location_type& l)
      {
        return symbol_type (token::TOKEN_TRUE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ANALYZE (location_type l)
      {
        return symbol_type (token::TOKEN_ANALYZE, std::move (l));
      }
#else
      static
      symbol_type
      make_ANALYZE (const location_type& l)
      {
        return symbol_type (token::TOKEN_ANALYZE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ANY (location_type l)
      {
        return symbol_type (token::TOKEN_ANY, std::move (l));
      }
#else
      static
      symbol_type
      make_ANY (const location_type& l)
      {
        return symbol_type (token::TOKEN_ANY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ALL (location_type l)
      {
        return symbol_type (token::TOKEN_ALL, std::move (l));
      }
#else
      static
      symbol_type
      make_ALL (const location_type& l)
      {
        return symbol_type (token::TOKEN_ALL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AS (location_type l)
      {
        return symbol_type (token::TOKEN_AS, std::move (l));
      }
#else
      static
      symbol_type
      make_AS (const location_type& l)
      {
        return symbol_type (token::TOKEN_AS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASC (location_type l)
      {
        return symbol_type (token::TOKEN_ASC, std::move (l));
      }
#else
      static
      symbol_type
      make_ASC (const location_type& l)
      {
        return symbol_type (token::TOKEN_ASC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BY (location_type l)
      {
        return symbol_type (token::TOKEN_BY, std::move (l));
      }
#else
      static
      symbol_type
      make_BY (const location_type& l)
      {
        return symbol_type (token::TOKEN_BY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLUMNS (location_type l)
      {
        return symbol_type (token::TOKEN_COLUMNS, std::move (l));
      }
#else
      static
      symbol_type
      make_COLUMNS (const location_type& l)
      {
        return symbol_type (token::TOKEN_COLUMNS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CREATE (location_type l)
      {
        return symbol_type (token::TOKEN_CREATE, std::move (l));
      }
#else
      static
      symbol_type
      make_CREATE (const location_type& l)
      {
        return symbol_type (token::TOKEN_CREATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DATETIME (location_type l)
      {
        return symbol_type (token::TOKEN_DATETIME, std::move (l));
      }
#else
      static
      symbol_type
      make_DATETIME (const location_type& l)
      {
        return symbol_type (token::TOKEN_DATETIME, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DATABASE (location_type l)
      {
        return symbol_type (token::TOKEN_DATABASE, std::move (l));
      }
#else
      static
      symbol_type
      make_DATABASE (const location_type& l)
      {
        return symbol_type (token::TOKEN_DATABASE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DATABASES (location_type l)
      {
        return symbol_type (token::TOKEN_DATABASES, std::move (l));
      }
#else
      static
      symbol_type
      make_DATABASES (const location_type& l)
      {
        return symbol_type (token::TOKEN_DATABASES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DELETE (location_type l)
      {
        return symbol_type (token::TOKEN_DELETE, std::move (l));
      }
#else
      static
      symbol_type
      make_DELETE (const location_type& l)
      {
        return symbol_type (token::TOKEN_DELETE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DESC (location_type l)
      {
        return symbol_type (token::TOKEN_DESC, std::move (l));
      }
#else
      static
      symbol_type
      make_DESC (const location_type& l)
      {
        return symbol_type (token::TOKEN_DESC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DESCRIBE (location_type l)
      {
        return symbol_type (token::TOKEN_DESCRIBE, std::move (l));
      }
#else
      static
      symbol_type
      make_DESCRIBE (const location_type& l)
      {
        return symbol_type (token::TOKEN_DESCRIBE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DISTINCT (location_type l)
      {
        return symbol_type (token::TOKEN_DISTINCT, std::move (l));
      }
#else
      static
      symbol_type
      make_DISTINCT (const location_type& l)
      {
        return symbol_type (token::TOKEN_DISTINCT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOUBLE (location_type l)
      {
        return symbol_type (token::TOKEN_DOUBLE, std::move (l));
      }
#else
      static
      symbol_type
      make_DOUBLE (const location_type& l)
      {
        return symbol_type (token::TOKEN_DOUBLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DROP (location_type l)
      {
        return symbol_type (token::TOKEN_DROP, std::move (l));
      }
#else
      static
      symbol_type
      make_DROP (const location_type& l)
      {
        return symbol_type (token::TOKEN_DROP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXISTS (location_type l)
      {
        return symbol_type (token::TOKEN_EXISTS, std::move (l));
      }
#else
      static
      symbol_type
      make_EXISTS (const location_type& l)
      {
        return symbol_type (token::TOKEN_EXISTS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXPLAIN (location_type l)
      {
        return symbol_type (token::TOKEN_EXPLAIN, std::move (l));
      }
#else
      static
      symbol_type
      make_EXPLAIN (const location_type& l)
      {
        return symbol_type (token::TOKEN_EXPLAIN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT (location_type l)
      {
        return symbol_type (token::TOKEN_FLOAT, std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT (const location_type& l)
      {
        return symbol_type (token::TOKEN_FLOAT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FROM (location_type l)
      {
        return symbol_type (token::TOKEN_FROM, std::move (l));
      }
#else
      static
      symbol_type
      make_FROM (const location_type& l)
      {
        return symbol_type (token::TOKEN_FROM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GROUP (location_type l)
      {
        return symbol_type (token::TOKEN_GROUP, std::move (l));
      }
#else
      static
      symbol_type
      make_GROUP (const location_type& l)
      {
        return symbol_type (token::TOKEN_GROUP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HAVING (location_type l)
      {
        return symbol_type (token::TOKEN_HAVING, std::move (l));
      }
#else
      static
      symbol_type
      make_HAVING (const location_type& l)
      {
        return symbol_type (token::TOKEN_HAVING, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INDEX (location_type l)
      {
        return symbol_type (token::TOKEN_INDEX, std::move (l));
      }
#else
      static
      symbol_type
      make_INDEX (const location_type& l)
      {
        return symbol_type (token::TOKEN_INDEX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INSERT (location_type l)
      {
        return symbol_type (token::TOKEN_INSERT, std::move (l));
      }
#else
      static
      symbol_type
      make_INSERT (const location_type& l)
      {
        return symbol_type (token::TOKEN_INSERT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT (location_type l)
      {
        return symbol_type (token::TOKEN_INT, std::move (l));
      }
#else
      static
      symbol_type
      make_INT (const location_type& l)
      {
        return symbol_type (token::TOKEN_INT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTO (location_type l)
      {
        return symbol_type (token::TOKEN_INTO, std::move (l));
      }
#else
      static
      symbol_type
      make_INTO (const location_type& l)
      {
        return symbol_type (token::TOKEN_INTO, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LIMIT (location_type l)
      {
        return symbol_type (token::TOKEN_LIMIT, std::move (l));
      }
#else
      static
      symbol_type
      make_LIMIT (const location_type& l)
      {
        return symbol_type (token::TOKEN_LIMIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NUMBER (location_type l)
      {
        return symbol_type (token::TOKEN_NUMBER, std::move (l));
      }
#else
      static
      symbol_type
      make_NUMBER (const location_type& l)
      {
        return symbol_type (token::TOKEN_NUMBER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ORDER (location_type l)
      {
        return symbol_type (token::TOKEN_ORDER, std::move (l));
      }
#else
      static
      symbol_type
      make_ORDER (const location_type& l)
      {
        return symbol_type (token::TOKEN_ORDER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SELECT (location_type l)
      {
        return symbol_type (token::TOKEN_SELECT, std::move (l));
      }
#else
      static
      symbol_type
      make_SELECT (const location_type& l)
      {
        return symbol_type (token::TOKEN_SELECT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SET (location_type l)
      {
        return symbol_type (token::TOKEN_SET, std::move (l));
      }
#else
      static
      symbol_type
      make_SET (const location_type& l)
      {
        return symbol_type (token::TOKEN_SET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SHOW (location_type l)
      {
        return symbol_type (token::TOKEN_SHOW, std::move (l));
      }
#else
      static
      symbol_type
      make_SHOW (const location_type& l)
      {
        return symbol_type (token::TOKEN_SHOW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STATUS (location_type l)
      {
        return symbol_type (token::TOKEN_STATUS, std::move (l));
      }
#else
      static
      symbol_type
      make_STATUS (const location_type& l)
      {
        return symbol_type (token::TOKEN_STATUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TABLE (location_type l)
      {
        return symbol_type (token::TOKEN_TABLE, std::move (l));
      }
#else
      static
      symbol_type
      make_TABLE (const location_type& l)
      {
        return symbol_type (token::TOKEN_TABLE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TABLES (location_type l)
      {
        return symbol_type (token::TOKEN_TABLES, std::move (l));
      }
#else
      static
      symbol_type
      make_TABLES (const location_type& l)
      {
        return symbol_type (token::TOKEN_TABLES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UPDATE (location_type l)
      {
        return symbol_type (token::TOKEN_UPDATE, std::move (l));
      }
#else
      static
      symbol_type
      make_UPDATE (const location_type& l)
      {
        return symbol_type (token::TOKEN_UPDATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_USING (location_type l)
      {
        return symbol_type (token::TOKEN_USING, std::move (l));
      }
#else
      static
      symbol_type
      make_USING (const location_type& l)
      {
        return symbol_type (token::TOKEN_USING, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VALUES (location_type l)
      {
        return symbol_type (token::TOKEN_VALUES, std::move (l));
      }
#else
      static
      symbol_type
      make_VALUES (const location_type& l)
      {
        return symbol_type (token::TOKEN_VALUES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VARCHAR (location_type l)
      {
        return symbol_type (token::TOKEN_VARCHAR, std::move (l));
      }
#else
      static
      symbol_type
      make_VARCHAR (const location_type& l)
      {
        return symbol_type (token::TOKEN_VARCHAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WHERE (location_type l)
      {
        return symbol_type (token::TOKEN_WHERE, std::move (l));
      }
#else
      static
      symbol_type
      make_WHERE (const location_type& l)
      {
        return symbol_type (token::TOKEN_WHERE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TINYINT (location_type l)
      {
        return symbol_type (token::TOKEN_TINYINT, std::move (l));
      }
#else
      static
      symbol_type
      make_TINYINT (const location_type& l)
      {
        return symbol_type (token::TOKEN_TINYINT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SMALLINT (location_type l)
      {
        return symbol_type (token::TOKEN_SMALLINT, std::move (l));
      }
#else
      static
      symbol_type
      make_SMALLINT (const location_type& l)
      {
        return symbol_type (token::TOKEN_SMALLINT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MEDIUMINT (location_type l)
      {
        return symbol_type (token::TOKEN_MEDIUMINT, std::move (l));
      }
#else
      static
      symbol_type
      make_MEDIUMINT (const location_type& l)
      {
        return symbol_type (token::TOKEN_MEDIUMINT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIGINT (location_type l)
      {
        return symbol_type (token::TOKEN_BIGINT, std::move (l));
      }
#else
      static
      symbol_type
      make_BIGINT (const location_type& l)
      {
        return symbol_type (token::TOKEN_BIGINT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DECIMAL (location_type l)
      {
        return symbol_type (token::TOKEN_DECIMAL, std::move (l));
      }
#else
      static
      symbol_type
      make_DECIMAL (const location_type& l)
      {
        return symbol_type (token::TOKEN_DECIMAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NUMERIC_SYM (location_type l)
      {
        return symbol_type (token::TOKEN_NUMERIC_SYM, std::move (l));
      }
#else
      static
      symbol_type
      make_NUMERIC_SYM (const location_type& l)
      {
        return symbol_type (token::TOKEN_NUMERIC_SYM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REAL (location_type l)
      {
        return symbol_type (token::TOKEN_REAL, std::move (l));
      }
#else
      static
      symbol_type
      make_REAL (const location_type& l)
      {
        return symbol_type (token::TOKEN_REAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CHAR (location_type l)
      {
        return symbol_type (token::TOKEN_CHAR, std::move (l));
      }
#else
      static
      symbol_type
      make_CHAR (const location_type& l)
      {
        return symbol_type (token::TOKEN_CHAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BINARY (location_type l)
      {
        return symbol_type (token::TOKEN_BINARY, std::move (l));
      }
#else
      static
      symbol_type
      make_BINARY (const location_type& l)
      {
        return symbol_type (token::TOKEN_BINARY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VARBINARY (location_type l)
      {
        return symbol_type (token::TOKEN_VARBINARY, std::move (l));
      }
#else
      static
      symbol_type
      make_VARBINARY (const location_type& l)
      {
        return symbol_type (token::TOKEN_VARBINARY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TIMESTAMP_SYM (location_type l)
      {
        return symbol_type (token::TOKEN_TIMESTAMP_SYM, std::move (l));
      }
#else
      static
      symbol_type
      make_TIMESTAMP_SYM (const location_type& l)
      {
        return symbol_type (token::TOKEN_TIMESTAMP_SYM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DATE (location_type l)
      {
        return symbol_type (token::TOKEN_DATE, std::move (l));
      }
#else
      static
      symbol_type
      make_DATE (const location_type& l)
      {
        return symbol_type (token::TOKEN_DATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TIME (location_type l)
      {
        return symbol_type (token::TOKEN_TIME, std::move (l));
      }
#else
      static
      symbol_type
      make_TIME (const location_type& l)
      {
        return symbol_type (token::TOKEN_TIME, l);
      }
#endif


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

    static const short yypact_ninf_;
    static const short yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short yytable_[];

  static const short yycheck_[];

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
  static const unsigned short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
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
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

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
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (size_type i)
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (int i)
      {
        return operator[] (size_type (i));
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (size_type i) const
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (int i) const
      {
        return operator[] (size_type (i));
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (int n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      size_type
      size () const YY_NOEXCEPT
      {
        return seq_.size ();
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, int range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (int i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        int range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 615,     ///< Last index in yytable_.
      yynnts_ = 63,  ///< Number of nonterminal symbols.
      yyfinal_ = 60, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 92  ///< Number of tokens.
    };


    // User arguments.
    CatDB::SqlScanner& scanner;
    CatDB::SqlDriver& driver;
  };

  inline
  parser::token_number_type
  parser::yytranslate_ (token_type t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    91,     2,     2,     2,     2,     2,
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
      85,    86,    87,    88,    89,    90
    };
    const unsigned user_token_number_max_ = 345;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
    , location (std::move (that.location))
  {
    switch (this->type_get ())
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
        value.move< Stmt_s > (std::move (that.value));
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        value.move< bool > (std::move (that.value));
        break;

      case 154: // number
        value.move< double > (std::move (that.value));
        break;

      case 107: // limit_expr
      case 134: // data_type
        value.move< int > (std::move (that.value));
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
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->type_get ())
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
        value.copy< Stmt_s > (YY_MOVE (that.value));
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        value.copy< bool > (YY_MOVE (that.value));
        break;

      case 154: // number
        value.copy< double > (YY_MOVE (that.value));
        break;

      case 107: // limit_expr
      case 134: // data_type
        value.copy< int > (YY_MOVE (that.value));
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
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
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
        value.move< Stmt_s > (YY_MOVE (s.value));
        break;

      case 98: // opt_distinct
      case 105: // opt_asc_desc
      case 120: // distinct_or_all
        value.move< bool > (YY_MOVE (s.value));
        break;

      case 154: // number
        value.move< double > (YY_MOVE (s.value));
        break;

      case 107: // limit_expr
      case 134: // data_type
        value.move< int > (YY_MOVE (s.value));
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
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_type::by_type (const by_type& that)
    : type (that.type)
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
  parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }

  inline
  parser::token_type
  parser::by_type::token () const YY_NOEXCEPT
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short
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
     345,    44
    };
    return token_type (yytoken_number_[type]);
  }

#line 3 "D:/CatDB/parser/lex_yacc/sql_parser.y"
} // CatDB
#line 3014 "D:/CatDB/parser/lex_yacc/sql_parser.hpp"





#endif // !YY_YY_D_CATDB_PARSER_LEX_YACC_SQL_PARSER_HPP_INCLUDED
