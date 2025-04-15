/*
 * 编译器主程序
 * 生成编译器lett
 */
#include <iostream>
#include "common.h"
#include "lexer/reader.h"
#include "lexer/lexer.h"

int main(int argc, char* argv[]) {
    Lett::ArgumentParser arg_parser("lettc");
    arg_parser.addOption("file", "f", "compile with file.", true, "filename");
    arg_parser.addOption("string", "s", "compile with string", true, "str");

    try {
        arg_parser.parse(argc, argv);
        if (arg_parser.givend("file")) {
            std::string filename = arg_parser.getValue("file");
            std::string file(filename);
            Lett::FileReader reader(file);
            Lett::LexicalAnalyzer& analyzer = Lett::LexicalAnalyzer::getInstance(&reader);
            analyzer.analyze();
            analyzer.print();

        } else if (arg_parser.givend("string")) {
            std::string str = arg_parser.getValue("string");
            Lett::StringReader reader(str);
            Lett::LexicalAnalyzer& analyzer = Lett::LexicalAnalyzer::getInstance(&reader);
            analyzer.analyze();
            analyzer.print();
        } else {
            arg_parser.printHelp();
        }
    } catch (const Lett::FileNotExsit &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (const Lett::InvalidOption &e) {
        std::cerr << e.what() << std::endl;
        arg_parser.printHelp();
        return -1;
    } catch (const Lett::LettException &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}