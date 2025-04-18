# Lett

Lett是一个基于C++开发的简单编程语言项目，主要用于实验和教学目的。该项目旨在帮助学习者理解编程语言的基本概念和实现原理。

## 项目特点

- 简单直观的语法设计
- 基于C++实现，便于理解和扩展
- 适合教学和实验使用
- 包含基本的编程语言特性
- 完整的编译器实现
- 虚拟机执行引擎

## 项目结构

```
Lett/
├── bin/           # 编译后的二进制文件目录
├── build/         # CMake构建目录
├── docs/          # 项目文档目录
├── include/       # 公共头文件目录
├── samples/       # Lett语言示例程序
├── src/           # 源代码目录
│   ├── compiler/  # 编译器实现
│   ├── lib/       # 公共库实现
│   └── vm/        # 虚拟机实现
├── tests/         # 测试用例目录
│   ├── compiler/  # 编译器测试
│   ├── integration/ # 集成测试
│   └── vm/        # 虚拟机测试
├── CMakeLists.txt # 主CMake配置文件
└── README.md      # 项目说明文档
```

## 快速开始

### 环境要求

- C++编译器（支持C++17或更高版本）
- CMake 3.10或更高版本

### 构建项目

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 构建项目
cmake --build .
```

构建完成后，在`build/bin`目录下会生成以下可执行文件：
- `lett`: Lett语言编译器
- `lettc`: Lett语言虚拟机

### 运行示例程序

```bash
# 编译Lett源文件
./lett samples/hello.let

# 运行编译后的程序
./lettc hello.letc
```

## 示例代码

```lett
// 第一个Lett程序
fn main() {
    println("Hello world");
}
```

## 开发计划

- [x] 项目基础架构搭建
- [x] 实现基本的词法解析器
- [ ] 实现基本的语法解析器
- [ ] 添加变量声明和基本数据类型
- [ ] 实现控制流语句（if、while等）
- [ ] 添加函数定义和调用
- [ ] 完善错误处理机制
- [ ] 实现虚拟机执行引擎
- [ ] 添加标准库支持

## 测试

项目包含完整的测试套件，包括：
- 编译器单元测试
- 虚拟机单元测试
- 集成测试

运行测试：
```bash
cd build
ctest
```

## 设计文档

Lett项目的设计，参考[设计文档](docs/design.md)

## 贡献指南

欢迎提交Issue和Pull Request来帮助改进Lett项目。在提交代码前，请确保：

1. 代码符合项目的编码规范
2. 添加了必要的测试用例
3. 更新了相关文档
4. 所有测试都能通过

## 许可证

本项目采用MIT许可证。详见 [LICENSE](LICENSE) 文件。 