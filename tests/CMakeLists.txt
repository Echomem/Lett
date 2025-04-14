# 添加Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# 启用测试
enable_testing()

# 添加子目录
add_subdirectory(compiler)
add_subdirectory(integration) 
add_subdirectory(vm)