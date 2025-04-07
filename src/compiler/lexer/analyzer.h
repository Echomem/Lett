#ifndef __LETT_LEXER_ANALYZER_H__
#define __LETT_LEXER_ANALYZER_H__

#include <vector>
#include <string>
#include <mutex>
#include "reader.h"
#include "token.h"

// 列出ASCII表中的可见字符
#define LEXER_VISIBLE_CHARS  "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_@#$?`,;(){}[]:+-*/%&|!^~<>=\\'\""

// 状态转换表中可接受字符的个数，包含上述的可见字符，还有下面定义的逻辑虚拟字符
#define LEXER_VISIBLE_CHAR_LEN 93
#define BLANK_CHAR_INDEX    (LEXER_VISIBLE_CHAR_LEN + 0)
#define NEWLINE_CHAR_INDEX  (LEXER_VISIBLE_CHAR_LEN + 1) 
#define LEXER_CHARSET_SIZE  (LEXER_VISIBLE_CHAR_LEN + 2)  

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
        Error              // 错误状态，作为最后一项，不要移动该位置。
    };

    // 词法分析器类，单例模式的类（加入了互斥锁，保证线程安全）
    class LexicalAnalyzer {
    private:
        // 构造函数私有化，防止外部实例化
        LexicalAnalyzer(Reader *rd);

        // 拷贝构造函数私有化，防止拷贝
        LexicalAnalyzer(const LexicalAnalyzer&) = delete; 
        
        // 赋值运算符私有化，防止赋值
        LexicalAnalyzer& operator=(const LexicalAnalyzer&) = delete;
        
        // 静态成员变量，保存单例实例
        static LexicalAnalyzer *_instance;
        
        // 静态互斥锁，用于线程安全
        static std::mutex _mutex;
    protected:
        // 将LEXER_VISIBLE_CHARS定义的可见字符转换成字符串
        std::string lexerVisibleChars;
        
        // 状态转移表，对于当前所处的状态及输入的字符定义了下一个状态。
        // 构造函数中需手工对该状态表进行写入。
        LexerState stateTable[static_cast<size_t>(LexerState::Error) + 1][LEXER_CHARSET_SIZE];
        
        // 初始化状态转移表
        void initStateTable(); 
        
        // 根据输入的字符，查找状态转移表，获取下一个状态
        // 如果状态表中未查找到，则返回LexerState::Error状态
        LexerState getNextState(char ch);
    public:
        // 静态成员函数，用于获取单例实例
        static LexicalAnalyzer& getInstance(Reader *rd);

        // 解析词法分析，生成词法单元
        void analyze();

        // 打印解析后的token列表用于测试
        void print();
    private:
        Reader *_reader;
        std::vector<Token> _tokens; // 词法单元列表
        LexerState _state; // 当前状态
    };
} // namespace Lett

#endif // __LETT_LEXER_ANALYZER_H__