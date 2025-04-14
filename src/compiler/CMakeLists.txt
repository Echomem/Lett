add_subdirectory(lexer)
add_subdirectory(parser)

# 创建可执行文件
add_executable(lett main.cpp)

# 链接lettcomm库
target_link_libraries(lett PRIVATE ltparser ltlexer ltcomm)

# 设置包含目录
target_include_directories(lett
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
) 

