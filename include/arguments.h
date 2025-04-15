#ifndef __LETTER_ARGUMENTS_H__
#define __LETTER_ARGUMENTS_H__

#include <string>
#include <vector>

namespace Lett {

    class ArgumentOption {
    private:
        std::string _name;
        std::string _short_name;
        std::string _description;
        bool _required;
        std::string _value_name;
        bool _is_set;
        std::string _value;
    public:
        ArgumentOption(
                const std::string &name, 
                const std::string &short_name, 
                const std::string &description,
                bool required = false, 
                const std::string &value_name=""
        );

        std::string getName() const { return _name; }
        std::string getShortName() const { return _short_name; }
        std::string getDescription() const { return _description; }
        bool isRequired() const { return _required; }
        std::string getValueName() const { return _value_name; }
        bool isSet() const { return _is_set; }
        std::string getValue() const { return _value; }
        void setFlag() { if(!_required) _is_set = true; }
        void setValue(const std::string &value) { _value = value; _is_set = true; }
        void setValue(const char *value) { _value = value; _is_set = true; }
    };

    // 命令行解析封装
    class ArgumentParser {
    private:
        std::string _program_name;
        std::vector<ArgumentOption> _options;
    public:
        ArgumentParser(const std::string &name);

        void addOption(
                const std::string &name,            // 选项的长名称
                const std::string &short_name,      // 选项的短名称，一般一个字符
                const std::string &description,     // 选项的描述
                bool required = false,              // 该选项是否提供值
                const std::string &value_name=""    // 值的别名
        );

        void parse(int argc, char *argv[]);
        bool givend(const std::string &option) const; // option选项是否在命令行中给出
        // 如果该选项是提供值的选项，则返回该选项的值
        std::string getValue(const std::string &option) const; 
        void printHelp() const;
    };  // class ArgumentParser
}   // namespace Lett

#endif // __LETTER_ARGUMENTS_H__