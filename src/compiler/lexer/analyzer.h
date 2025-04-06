#ifndef __LETT_LEXER_ANALYZER_H__
#define __LETT_LEXER_ANALYZER_H__

#include <map>
#include <vector>
#include <string>
#include "reader.h"
#include "token.h"

namespace Lett {

    enum class LexerState {
        Ready,             // 准备状态
        Ident,             // 标识符状态
        Number,            // 十进制数字状态
        HexNumber,         // 十六进制数字状态
        OctNumber,         // 八进制数字状态
        BinNumber,         // 二进制数字状态
        Float,             // 浮点数字状态
        String,            // 字符串状态
        Char,              // 字符状态
        MuiltiComment,     // 多行注释状态
        SingleComment,     // 单行注释状态
        Operator,          // 运算符状态
        Separator,         // 分隔符状态
        Error              // 错误状态
    };

    enum class CharType {
        Letter,            // 字母
        Digit,             // 数字
        HexDigitStart,     // 十六进制数字起始字符
        OctDigitStart,     // 八进制数字起始字符
        BinDigitStart,     // 二进制数字起始字符
        StringStart,       // 字符串起始字符
        CharStart,         // 字符起始字符
        MultiCommentStart, // 多行注释起始字符
        SingleCommentStart,// 单行注释起始字符
        Whitespace,        // 空白字符
        Newline,           // 换行符
        Operator,          // 运算符
        Separator,         // 分隔符
        Other              // 其他字符
    };

    class LexerStateOperator {
    protected:
        static bool isLetter(char ch);      // 是否字母
        static bool isDigit(char ch);       // 是否十进制数字
        static bool isHexDigit(char ch);    // 是否十六进制数字
        static bool isOctDigit(char ch);    // 是否八进制数字
        static bool isBinDigit(char ch);    // 是否二进制数字
        static bool isWhitespace(char ch);  // 是否空白字符
        static bool isNewline(char ch);     // 是否换行符
    public:
        static CharType getCharType(char ch, char nextch); // 获取字符类型
    };

    // 词法分析器类
    class LexicalAnalyzer {
    protected:
        /* 状态转移表 */
        static LexerState stateTable[static_cast<int>(LexerState::Error) + 1][static_cast<int>(CharType::Other) + 1];
    public:
        LexicalAnalyzer(Reader *rd);
        
        void analyze();     // 扫描源代码流，生成词法单元
        void print();       // 打印解析后的tokens
    private:
        Reader *_reader;
        std::vector<Token> _tokens; // 词法单元列表
        LexerState _state; // 当前状态
    };
} // namespace Lett

#endif // __LETT_LEXER_ANALYZER_H__