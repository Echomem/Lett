/*
 * 编译器主程序
 * 生成编译器lett
 */
#include <iostream>
#include "common.h"
#include "lexer/reader.h"
#include "lexer/lexer.h"

int main(int argc, char* argv[]) {
    // 检查命令行参数是否正确
    // 如果没有提供源文件名，则输出用法提示并返回错误代码
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return -1;
    }

    try {
        // 创建文件读取器
        Lett::FileReader reader(argv[1]);
        // 创建词法分析器
        Lett::LexicalAnalyzer& analyzer = Lett::LexicalAnalyzer::getInstance(&reader);
        // 扫描文件，生成词法单元
        analyzer.analyze();

        // 打印扫描结果
        analyzer.print();

    } catch (const Lett::FileNotExsit &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (const Lett::LettException &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}