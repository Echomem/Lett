# 添加测试可执行文件
add_executable(lexer_test lexer_test.cpp)

# 添加include目录
target_include_directories(lexer_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/compiler/lexer
)

# 链接Google Test库和项目库
target_link_libraries(lexer_test
    PRIVATE
    gtest
    gtest_main
    ltparser
    ltlexer
    ltcomm
)

# 添加测试到CMake测试系统
add_test(NAME lexer_test COMMAND lexer_test)