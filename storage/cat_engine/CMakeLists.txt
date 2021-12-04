include_directories(
    ${PROJECT_SOURCE_DIR}/storage
)
aux_source_directory(. DIR_CAT_ENGINE_SRCS)
add_library(cat_engine ${DIR_CAT_ENGINE_SRCS})