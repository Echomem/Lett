# Lett

Lett是一个基于C++开发的简单编程语言项目，主要用于实验和教学目的。该项目旨在帮助学习者理解编程语言的基本概念和实现原理。

## 项目特点

- 简单直观的语法设计
- 基于C++实现，便于理解和扩展
- 适合教学和实验使用
- 包含基本的编程语言特性

## 项目结构

```
Lett/
├── src/           # 源代码目录
├── tests/         # 测试用例目录
│   └── lett_program/  # Lett语言示例程序
├── docs/          # 文档目录
└── README.md      # 项目说明文档
```

## 快速开始

### 环境要求

- C++编译器（支持C++17或更高版本）
- CMake 3.10或更高版本

### 构建项目

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 运行示例程序

```bash
./lett tests/lett_program/1-Helloworld.let
```

## 示例代码

```lett
// 第一个Lett程序
fn main() {
    println("Hello world");
}
```

## 开发计划

- [ ] 实现基本的语法解析器
- [ ] 添加变量声明和基本数据类型
- [ ] 实现控制流语句（if、while等）
- [ ] 添加函数定义和调用
- [ ] 完善错误处理机制

## 贡献指南

欢迎提交Issue和Pull Request来帮助改进Lett项目。在提交代码前，请确保：

1. 代码符合项目的编码规范
2. 添加了必要的测试用例
3. 更新了相关文档

## 许可证

本项目采用MIT许可证。详见 [LICENSE](LICENSE) 文件。 