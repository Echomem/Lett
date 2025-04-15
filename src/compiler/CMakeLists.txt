add_subdirectory(lexer)
add_subdirectory(parser)

# 创建可执行文件
add_executable(lettc main.cpp)

# 链接lettcomm库
target_link_libraries(lettc PRIVATE ltparser ltlexer ltcomm)

# 设置包含目录
target_include_directories(lettc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
) 

