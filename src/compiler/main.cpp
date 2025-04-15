/*
 * 编译器主程序
 * 生成编译器lett
 */
#include <iostream>
#include "common.h"
#include "lexer/reader.h"
#include "lexer/lexer.h"

int main(int argc, char* argv[]) {
    Lett::ArgumentParser parser("lettc");
    parser.addOption("file", "f", "compile with file.", true, "filename");
    parser.addOption("string", "s", "compile with string", true, "str");

    try {
        parser.parse(argc, argv);
        if (parser.givend("file")) {
            std::string filename = parser.getValue("file");
            std::string file(filename);
            Lett::FileReader reader(file);
            Lett::LexicalAnalyzer& analyzer = Lett::LexicalAnalyzer::getInstance(&reader);
            analyzer.analyze();
            analyzer.print();

        } else if (parser.givend("string")) {
            std::string str = parser.getValue("string");
            Lett::StringReader reader(str);
            Lett::LexicalAnalyzer& analyzer = Lett::LexicalAnalyzer::getInstance(&reader);
            analyzer.analyze();
            analyzer.print();
        } else {
            parser.printHelp();
        }
    } catch (const Lett::FileNotExsit &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (const Lett::LettException &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}