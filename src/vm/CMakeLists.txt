# 收集源文件
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp" "*.h")

# 创建可执行文件
add_executable(lett ${SOURCES} ${HEADERS})

# 链接ltcomm库
target_link_libraries(lett PRIVATE ltcomm)

# 设置包含目录
target_include_directories(lett
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
) 