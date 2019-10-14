include_directories(
    . 
    ${PROJECT_SOURCE_DIR}/common 
    ${PROJECT_SOURCE_DIR}/storage 
    )
aux_source_directory(. DIR_SQL_SRCS)
add_library(sql ${DIR_SQL_SRCS})
target_link_libraries(sql common storage)