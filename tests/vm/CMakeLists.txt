# 添加测试可执行文件
add_executable(vm_test vm_test.cpp)

# 添加include目录
target_include_directories(vm_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/vm
)

# 链接Google Test库和项目库
target_link_libraries(vm_test
    PRIVATE
    gtest
    gtest_main
    ltparser
    ltlexer
    ltcomm
)

# 添加测试到CMake测试系统
add_test(NAME vm_test COMMAND vm_test)