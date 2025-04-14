# 添加测试可执行文件
add_executable(integration_test integration_test.cpp)

# 添加include目录
target_include_directories(integration_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/compiler/lexer
    ${CMAKE_SOURCE_DIR}/src/compiler/parser
)

# 链接Google Test库和项目库
target_link_libraries(integration_test
    PRIVATE
    gtest
    gtest_main
    ltparser
    ltlexer
    ltcomm
)

# 添加测试到CMake测试系统
add_test(NAME integration_test COMMAND integration_test)