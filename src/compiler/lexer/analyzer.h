#ifndef __LETT_LEXER_ANALYZER_H__
#define __LETT_LEXER_ANALYZER_H__

#include <vector>
#include <string>
#include <mutex>
#include "reader.h"
#include "token.h"

// 列出ASCII表中的可见字符
#define LEXER_VISIBLE_CHARS  "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$@#?`.:,;()[]{}+-*/%&|!^~<>=\\'\"\t\n "

// 状态转换表中可接受字符的个数，Unicode字符统一虚拟为其他
#define LEXER_VISIBLE_CHAR_LEN (sizeof(LEXER_VISIBLE_CHARS) - 1)
#define UNICODE_CHAR_INDEX  (LEXER_VISIBLE_CHAR_LEN + 0)
#define LEXER_CHARSET_SIZE  (LEXER_VISIBLE_CHAR_LEN + 1)  

namespace Lett {

    enum class LexerState {
        READY,             // 准备状态
        // 数字字面量识别状态
        ZERO,
        DEC_INTEGER,
        HEX_INTEGER,
        _HEX_S,
        OCT_INTEGER,
        _OCT,
        BIN_INTEGER,
        _BIN ,
        FLOAT,
        // 字符串字面量识别状态
        _STRING,
        STRING,
        ESCSTRING,
        // 字符字面量识别状态
        _CHAR_S,
        _CHAR,
        CHAR,
        ESCCHAR,
        // 标识符字面量识别状态
        IDENTIFIER,
        // 符号是被状态
        OP_ADD              ,
        OP_INC              ,
        OP_ADD_ASSIGN       ,
        OP_SUB              ,
        OP_SUB_ASSIGN       ,
        OP_DEC              ,
        OP_MUL              ,
        OP_MUL_ASSIGN       ,
        OP_DIV              ,
        OP_DIV_ASSIGN       ,
        _SINGLINE_COMMENT   ,
        SINGLINE_COMMENT    ,
        _MUILTLINE_COMMENT  ,
        _MUILTLINE_COMMENT_E,
        MUILTLINE_COMMENT   ,
        OP_NOT_EQUAL        ,
        OP_MOD              ,
        OP_MOD_ASSIGN       ,
        OP_ASSIGN           ,
        OP_EQUAL            ,
        OP_BIT_AND          ,
        OP_BIT_AND_ASSIGN   ,
        OP_AND              ,
        OP_BIT_OR           ,
        OP_BIT_OR_ASSIGN    ,
        OP_OR               ,
        OP_NOT              ,
        OP_BIT_NOT          ,
        OP_BIT_XOR          ,
        OP_GREAT            ,
        OP_GREAT_EQUAL      ,
        OP_LESS             ,
        OP_LESS_EQUAL       ,
        OP_BIT_SHIFT_LEFT   ,
        OP_BIT_SHIFT_RIGHT  ,
        LEFT_PARENT         ,
        RIGHT_PARENT        ,
        LEFT_BRACKET        ,
        RIGHT_BRACKET       ,
        LEFT_BRACE          ,
        RIGHT_BRACE         ,
        COMMA               ,
        SEMI_COLON          ,
        DOT                 ,
        COLON               ,
        DOUBLE_COLON        ,
        // 错误状态，作为最后一项，不要移动该位置
        ERROR              
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
        
        // 源代码读取器
        Reader *_reader;

        // 词法单元列表
        std::vector<Token> _tokens;

        // 词法分析器的当前状态
        LexerState _state;
    protected:
        // 将LEXER_VISIBLE_CHARS定义的可见字符转换成字符串
        std::string lexerVisibleChars;
        
        // 状态转移表，对于当前所处的状态及输入的字符定义了下一个状态。
        // 构造函数中需手工对该状态表进行写入。
        LexerState stateTable[static_cast<size_t>(LexerState::ERROR) + 1][LEXER_CHARSET_SIZE];
        
        // 设置状态转换表中的转换关系
        // 当在statrtState状态时，将输入符合charList的字符，全部转换为endState
        void setupStateTransform(LexerState startState, LexerState endState, const char *charList);

        // 设置默认状态转移
        // 当在initState时，将其它未设置的状态全部设置为defaultState
        void setupDefaultStateTransform(LexerState initState, LexerState defaultState);
        
        // 初始化状态转移表
        void initStateTable(); 
        // 配置状态关系转换图
        void installStateTransition();
        void installSymbolTransition();
        
        // 根据输入的字符，查找状态转移表，获取下一个状态
        // 如果状态表中未查找到，则返回LexerState::ERROR状态
        LexerState getNextState(char ch);

        // 根据当前态，确定Token类型
        TokenType getTokenType();
    public:
        // 静态成员函数，用于获取单例实例
        static LexicalAnalyzer& getInstance(Reader *rd);

        // 解析词法分析，生成词法单元
        void analyze();

        // 打印解析后的token列表用于测试
        void print();
    };
} // namespace Lett

#endif // __LETT_LEXER_ANALYZER_H__