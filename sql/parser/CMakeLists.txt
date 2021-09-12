set(PARSER_SRCS 
    sql_parser.hpp
    stack.hh
    position.hh
    location.hh
)

set(force_build false)
set(new_lex_file "sql_parser.l")
set (new_yacc_file "sql_parser.y")
set(old_lex_file "${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l")
set(old_yacc_file "${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y")

if(NOT EXISTS ${new_lex_file})
    execute_process(COMMAND cp ${old_lex_file} ${new_lex_file})
    set(force_build true)
endif()

if(NOT EXISTS ${new_yacc_file})
    execute_process(COMMAND cp ${old_yacc_file} ${new_yacc_file})
    set(force_build true)
endif()

if(${force_build} OR ${old_lex_file} IS_NEWER_THAN ${new_lex_file} OR ${old_yacc_file} IS_NEWER_THAN ${new_yacc_file})
    	execute_process(COMMAND cp ${old_lex_file} ${new_lex_file})
	execute_process(COMMAND cp ${old_yacc_file} ${new_yacc_file})
        message("-- Generate lex or yacc source file")
	IF (WIN32)
			execute_process(
			COMMAND flex 
				--wincompat 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_scanner.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l)
			execute_process(
			COMMAND bison -v --debug 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y)
	ELSE ()
			execute_process(
			COMMAND flex 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_scanner.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.l)
			execute_process(
			COMMAND bison -v --debug 
				-o ${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.cpp 
				${CMAKE_CURRENT_SOURCE_DIR}/sql_parser.y)
	ENDIF ()
endif()
aux_source_directory(. OTHER_SRCS)
include_directories(.
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/sql/resolver
)

add_library(parser ${OTHER_SRCS})
target_link_libraries(parser common resolver)
