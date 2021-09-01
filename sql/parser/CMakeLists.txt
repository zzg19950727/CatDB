set(PARSER_SRCS 
    sql_parser.hpp
    stack.hh
    position.hh
    location.hh
)

set(new_lex_file "sql_parser.l")
set (new_yacc_file "sql_parser.y")
set(old_lex_file "${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l")
set(old_yacc_file "${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y")

if(NOT EXISTS ${new_lex_file})
    execute_process(COMMAND cp ${old_lex_file} ${new_lex_file})
    message(" lex file copied.")
endif()

if(NOT EXISTS ${new_yacc_file})
    execute_process(COMMAND cp ${old_yacc_file} ${new_yacc_file})
    message(" yacc copied.")
endif()

if(${old_lex_file} IS_NEWER_THAN ${new_lex_file} OR ${old_yacc_file} IS_NEWER_THAN ${new_yacc_file})
    execute_process(COMMAND cp ${old_lex_file} ${new_lex_file})
	execute_process(COMMAND cp ${old_yacc_file} ${new_yacc_file})
    message("lex or yacc file changed")
	IF (WIN32)
		add_custom_command(
			OUTPUT ${PARSER_SRCS}
			PRE_BUILD 
			COMMAND flex 
				--wincompat 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_scanner.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l 
			COMMAND bison -v --debug 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y
			DEPENDS sql_parser.l sql_parser.y
			COMMENT "Now Generating lex and yacc file"
		)
	ELSE ()
			add_custom_command(
			OUTPUT ${PARSER_SRCS}
			PRE_BUILD 
			COMMAND flex 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_scanner.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l 
			COMMAND bison -v --debug 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y
			DEPENDS sql_parser.l sql_parser.y
			COMMENT "Now Generating lex and yacc file"
		)
	ENDIF ()
endif()

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
    ${PROJECT_SOURCE_DIR}/sql/resolver
)

add_library(parser ${OTHER_SRCS})
target_link_libraries(parser common resolver)