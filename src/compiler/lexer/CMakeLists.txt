# 收集源文件
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp" "*.h")

# 创建库
add_library(ltlexer STATIC ${SOURCES} ${HEADERS})

# 设置包含目录
target_include_directories(ltlexer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置库的属性
set_target_properties(ltlexer PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
) 