include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/storage 
    ${PROJECT_SOURCE_DIR}/sql
)
aux_source_directory(. DIR_EXECUTOR_SRCS)
add_library(executor ${DIR_EXECUTOR_SRCS})
target_link_libraries(executor common storage)