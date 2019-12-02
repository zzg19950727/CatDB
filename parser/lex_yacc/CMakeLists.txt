set(LEX_YACC_SRCS 
    sql_parser.hpp
    stack.hh
    position.hh
    location.hh
    )

IF (WIN32)
	add_custom_command(
		OUTPUT ${LEX_YACC_SRCS}
		PRE_BUILD 
		COMMAND flex 
			--wincompat 
			-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_scanner.cpp 
			${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l 
		COMMAND bison 
			-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.cpp 
			${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y
		DEPENDS sql_parser.l sql_parser.y
		COMMENT "Now Generating lex and yacc file"
	)
ELSE ()
		add_custom_command(
		OUTPUT ${LEX_YACC_SRCS}
		PRE_BUILD 
		COMMAND flex 
			-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_scanner.cpp 
			${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l 
		COMMAND bison 
			-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.cpp 
			${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y
		DEPENDS sql_parser.l sql_parser.y
		COMMENT "Now Generating lex and yacc file"
	)
ENDIF ()

set(OTHER_SRCS
    sql_driver.cpp
    sql_driver.h
	sql_scanner.cpp
	sql_parser.cpp 	
	sql_parser.hpp
    stack.hh
    position.hh
    location.hh
    )
include_directories(.
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/parser
    )

add_library(lex_yacc ${OTHER_SRCS})
target_link_libraries(lex_yacc common parser)