#include <gtest/gtest.h>

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试用例执行前的设置
    }

    void TearDown() override {
        // 每个测试用例执行后的清理
    }
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}