#include <gtest/gtest.h>
#include "reader.h"
#include "lexer.h"

using namespace Lett;

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试用例执行前的设置
    }

    void TearDown() override {
        // 每个测试用例执行后的清理
    }

    // 辅助函数：验证token的类型和值
    void verifyToken(const Token& token, TokenType expectedType, const std::string& expectedValue) {
        EXPECT_EQ(token.type(), expectedType);
        EXPECT_STREQ(token.value(), expectedValue.c_str());
    }
};

// 测试基本词法单元
TEST_F(LexerTest, BasicTokens) {
    StringReader reader("var x = 10;");
    LexicalAnalyzer& analyzer = LexicalAnalyzer::getInstance(&reader);
    analyzer.analyze();
    
    const auto& tokens = analyzer.getTokens();
    ASSERT_EQ(tokens.size(), 5);
    
    verifyToken(tokens[0], TokenType::KEYWORD, "var");
    verifyToken(tokens[1], TokenType::IDENTIFIER, "x");
    verifyToken(tokens[2], TokenType::OP_ASSIGN, "=");
    verifyToken(tokens[3], TokenType::DEC_INTEGER, "10");
    verifyToken(tokens[4], TokenType::SEMI_COLON, ";");
}

// 测试数字字面量
TEST_F(LexerTest, NumberLiterals) {
    StringReader reader("123 0x1F 0o77 0b101 3.14");
    LexicalAnalyzer& analyzer = LexicalAnalyzer::getInstance(&reader);
    analyzer.analyze();
    
    const auto& tokens = analyzer.getTokens();
    ASSERT_EQ(tokens.size(), 5);
    
    verifyToken(tokens[0], TokenType::DEC_INTEGER, "123");
    verifyToken(tokens[1], TokenType::HEX_INTEGER, "0x1F");
    verifyToken(tokens[2], TokenType::OCT_INTEGER, "0o77");
    verifyToken(tokens[3], TokenType::BIN_INTEGER, "0b101");
    verifyToken(tokens[4], TokenType::FLOAT, "3.14");
}

// 测试字符串和字符字面量
TEST_F(LexerTest, StringAndCharLiterals) {
    StringReader reader("\"hello\" 'a' \"escaped\\nstring\"");
    LexicalAnalyzer& analyzer = LexicalAnalyzer::getInstance(&reader);
    analyzer.analyze();
    
    const auto& tokens = analyzer.getTokens();
    ASSERT_EQ(tokens.size(), 3);
    
    verifyToken(tokens[0], TokenType::STRING, "\"hello\"");
    verifyToken(tokens[1], TokenType::CHAR, "'a'");
    verifyToken(tokens[2], TokenType::STRING, "\"escaped\\nstring\"");
}

// 测试运算符和分隔符
TEST_F(LexerTest, OperatorsAndSeparators) {
    StringReader reader("+ - * / % ++ -- && || == !=");
    LexicalAnalyzer& analyzer = LexicalAnalyzer::getInstance(&reader);
    analyzer.analyze();
    
    const auto& tokens = analyzer.getTokens();
    ASSERT_EQ(tokens.size(), 11);
    
    verifyToken(tokens[0], TokenType::OP_ADD, "+");
    verifyToken(tokens[1], TokenType::OP_SUB, "-");
    verifyToken(tokens[2], TokenType::OP_MUL, "*");
    verifyToken(tokens[3], TokenType::OP_DIV, "/");
    verifyToken(tokens[4], TokenType::OP_MOD, "%");
    verifyToken(tokens[5], TokenType::OP_INC, "++");
    verifyToken(tokens[6], TokenType::OP_DEC, "--");
    verifyToken(tokens[7], TokenType::OP_AND, "&&");
    verifyToken(tokens[8], TokenType::OP_OR, "||");
    verifyToken(tokens[9], TokenType::OP_EQUAL, "==");
    verifyToken(tokens[10], TokenType::OP_NOT_EQUAL, "!=");
}

// 测试注释
TEST_F(LexerTest, Comments) {
    StringReader reader("// single-line comment.\n/* muilt-line\ncomment. */");
    LexicalAnalyzer& analyzer = LexicalAnalyzer::getInstance(&reader);
    analyzer.analyze();
    
    const auto& tokens = analyzer.getTokens();
    // 注释应该被完全跳过，不产生任何token
    EXPECT_TRUE(tokens.empty());
}

// 测试错误处理
TEST_F(LexerTest, ErrorHandling) {
    StringReader reader("123abc 0xGH 0o89 'unclosed");
    LexicalAnalyzer& analyzer = LexicalAnalyzer::getInstance(&reader);
    analyzer.analyze();
    
    const auto& tokens = analyzer.getTokens();
    ASSERT_EQ(tokens.size(), 4);
    
    // 验证错误token
    verifyToken(tokens[0], TokenType::UNKNOWN, "123abc");
    verifyToken(tokens[1], TokenType::UNKNOWN, "0xGH");
    verifyToken(tokens[2], TokenType::UNKNOWN, "0o89");
    verifyToken(tokens[3], TokenType::UNKNOWN, "'unclosed");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 