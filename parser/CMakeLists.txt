aux_source_directory(. DIR_PARSER_SRCS)
add_subdirectory(lex_yacc)
add_library(parser ${DIR_PARSER_SRCS})
target_link_libraries(parser lex_yacc)