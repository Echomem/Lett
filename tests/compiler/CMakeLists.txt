# 收集测试源文件
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
file(GLOB_RECURSE TEST_HEADERS "*.hpp" "*.h")

# 创建测试可执行文件
add_executable(lett_compiler_tests ${TEST_SOURCES} ${TEST_HEADERS})

# 链接liblett库
target_link_libraries(lett_compiler_tests PRIVATE liblett)

# 设置包含目录
target_include_directories(lett_compiler_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 添加测试
add_test(NAME lett_compiler_tests COMMAND lett_compiler_tests) 