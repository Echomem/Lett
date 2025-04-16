#ifndef __LETT_LEXER_ANALYZER_H__
#define __LETT_LEXER_ANALYZER_H__

#include <vector>
#include <string>
#include <mutex>
#include <unordered_map>
#include "reader.h"
#include "token.h"

// 列出ASCII表中的可见字符
#define LEXER_USED_CHARS "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$@#?`.:,;()[]{}+-*/%&|!^~<>=\\'\"\t\n "

// 状态转换表中可接受字符的个数，Unicode字符统一虚拟为其他
#define LEXER_USED_CHAR_LEN (sizeof(LEXER_USED_CHARS) - 1)
#define UNICODE_CHAR_INDEX  (LEXER_USED_CHAR_LEN + 0)
#define LEXER_CHARSET_SIZE  (LEXER_USED_CHAR_LEN + 1)  

namespace Lett {
    // 词法分析器的状态机，定义了所有可能的状态
    #define TKTP_MEMBER(m, s) m,
    enum class LexerState {
        READY,
        ZERO,
        LETT_TKTP_BASIC
        LETT_TKTP_OPERATOR
        LETT_TKTP_SEPERATOR
        _HEX_S,
        _OCT,
        _BIN,
        _STRING,
        ESCSTRING,
        _CHAR_S,
        _CHAR,
        ESCCHAR,
        _SINGLINE_COMMENT   ,
        SINGLINE_COMMENT    ,
        _MUILTLINE_COMMENT  ,
        _MUILTLINE_COMMENT_E,
        MUILTLINE_COMMENT   ,
        ERROR
    };
    #undef TKTP_MEMBER

    // 词法分析器类，单例模式的类（加入了互斥锁，保证线程安全）
    class LexicalAnalyzer {
    private:
        // 构造函数私有化，防止外部实例化
        LexicalAnalyzer();
        LexicalAnalyzer(const LexicalAnalyzer&) = delete; 
        LexicalAnalyzer& operator=(const LexicalAnalyzer&) = delete;
        void _set_reader(Reader *rd);
        
        static LexicalAnalyzer *_instance;
        static std::mutex _mutex;
        
        Reader *_reader;
        std::vector<Token> _tokens;
        LexerState _state;

        std::string _lexer_used_chars;
        LexerState _state_table[static_cast<size_t>(LexerState::ERROR) + 1][LEXER_CHARSET_SIZE];
        void _setup_state_transform(LexerState startState, LexerState endState, const char *charList);
        void _setup_default_state_transform(LexerState initState, LexerState defaultState);
        void _init_state_table();          // 初始化状态转移表
        void _install_state_transition();  // 配置状态关系转换图
        LexerState _get_next_state(char ch);
        TokenType _get_token_type();       // 获取最终状态的TokeType
        void _handle_error(std::string &value, std::size_t line, std::size_t column);

        static const std::unordered_map<LexerState, TokenType> &_final_state_tktp_map;
        static LexerState _get_final_state(TokenType type);
    public:
        static LexicalAnalyzer& getInstance(Reader *rd=nullptr);
        void analyze();     // 词法分析
        void print();       // 打印词法分析的结果
        const std::vector<Token>& getTokens() const { return _tokens; } // 获取token列表
    };
} // namespace Lett

#endif // __LETT_LEXER_ANALYZER_H__