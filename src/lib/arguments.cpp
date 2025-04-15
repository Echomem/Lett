#include <iostream>
#include "arguments.h"
#include "common.h"

namespace Lett {

    ArgumentOption::ArgumentOption(
        const std::string &name, 
        const std::string &short_name, 
        const std::string &description,
        bool required, 
        const std::string &value_name
    ):  _name(name), 
        _short_name(short_name), 
        _description(description), 
        _required(required), 
        _value_name(value_name), 
        _is_set(false) {
    }

    ArgumentParser::ArgumentParser(const std::string &name)
        : _program_name(name),
          _options() {
    }

    void ArgumentParser::addOption(
        const std::string &name, 
        const std::string &short_name, 
        const std::string &description,
        bool required, 
        const std::string &value_name
    ) {
        //ArgumentOption option(name, short_name, description, required, value_name);
        _options.emplace_back(name, short_name, description, required, value_name);
    }

    void ArgumentParser::parse(int argc, char *argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (arg == "-h" || arg == "--help") {
                printHelp();
                exit(0);
            } else if (arg == "-v" || arg == "--version") {
                std::cout << _program_name << " "<< PROJECT_VERSION << std::endl;
                exit(0);
            } else {
                for (auto &opt : _options) {
                    std::string short_name = "-" + opt.getShortName();
                    std::string name = "--" + opt.getName();
                    if (arg==name || arg==short_name) {
                        opt.setFlag();
                        if (opt.isRequired()) {
                            if (i + 1 < argc) {
                                opt.setValue(argv[++i]);
                            } else {
                                throw InvalidOption(arg, "requires a value.\n");
                            }
                        }
                    }
                }
            }
        }
    }

    bool ArgumentParser::givend(const std::string &option) const {
        for (const auto &opt : _options) {
            if (opt.getName() == option || opt.getShortName() == option) {
                if (opt.isSet()) {
                    return true;
                }
            }
        }
        return false;
    }

    std::string ArgumentParser::getValue(const std::string &option) const {
        for (const auto &opt : _options) {
            if (opt.getName() == option || opt.getShortName() == option) {
                if (opt.isSet() && opt.isRequired()) {
                    return opt.getValue();
                }
            }
        }
        return "";
    }

    void ArgumentParser::printHelp() const {
        std::cout << "Usage: " << _program_name << " [options]\n"
                  << "Options:\n"
                  << "  -h, --help\t\tShow this help message and exit\n"
                  << "  -v, --version\t\tShow version information and exit\n";
        for (const auto &opt : _options) {
            if (opt.isRequired()) {
                std::cout << "  -" << opt.getShortName() << ", --"<< opt.getName() << " " <<opt.getValueName()<<"\t"<<opt.getDescription()<<std::endl; 
            } else {
                std::cout << "  -" << opt.getShortName() << ", --"<< opt.getName() <<"\t\t"<<opt.getDescription()<<std::endl; 
            }
        }
    }

} // namespace Lett