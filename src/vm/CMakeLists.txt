# 收集源文件
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp" "*.h")

# 创建可执行文件
add_executable(lettc ${SOURCES} ${HEADERS})

# 链接liblett库
target_link_libraries(lettc PRIVATE lettcomm)

# 设置包含目录
target_include_directories(lettc
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
) 