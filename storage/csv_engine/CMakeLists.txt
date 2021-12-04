include_directories(
    ${PROJECT_SOURCE_DIR}/storage
)
aux_source_directory(. DIR_CSV_ENGINE_SRCS)
add_library(csv_engine ${DIR_CSV_ENGINE_SRCS})