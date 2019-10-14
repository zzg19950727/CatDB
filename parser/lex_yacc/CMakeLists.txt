set(LEX_YACC_SRCS 
    sql_scanner.cpp
    sql_parser.cpp 
    sql_parser.hpp
    stack.hh
    position.hh
    location.hh
    )
add_custom_command(
    OUTPUT ${LEX_YACC_SRCS}
    PRE_BUILD 
    COMMAND D:/win_flex_bison-2.5.20/flex 
        --wincompat 
        -o sql_scanner.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l 
    COMMAND D:/win_flex_bison-2.5.20/bison 
        -o sql_parser.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y
    DEPENDS sql_parser.l sql_parser.y
    COMMENT "Now Generating lex and yacc file"
)
set(OTHER_SRCS
    sql_driver.cpp
    sql_driver.h
    )
include_directories(.
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/parser
    )

add_library(lex_yacc ${LEX_YACC_SRCS} ${OTHER_SRCS})
target_link_libraries(lex_yacc common parser)