# easy_copy_file_tree

# 中文

## 项目简介

`easy_copy_file_tree`是一个Windows平台下的命令行工具。
它能够根据指定文件夹路径自动生成目录树结构，并将结果直接复制到剪贴板，方便开发者在编写项目文档、README、设计文档或技术报告时快速插入项目结构。
例如：

```text
easy_copy_file_tree/
├──inc/
│  ├──file_tree.h
│  └──tree.h
├──src/
│  ├──file_tree.c
│  ├──main.c
│  └──tree.c
├──Makefile
├──README.md
└──run.exe
```

生成后自动复制到系统剪贴板，可直接粘贴到Markdown、Word、博客或技术文档中。

## 项目功能

### 已实现

- [x] 支持Unicode路径参数（中文、日文、韩文等非ASCII字符）
- [x] 自动生成目录树结构
- [x] 自动复制结果到系统剪贴板

### 计划实现

- [ ] Windows文件夹右键菜单集成
  - [ ] 直接生成目录树
- [ ] 支持配置文件
  - [ ] 自定义输出编码
  - [ ] 自定义树形符号样式
  - [ ] 屏蔽指定文件或目录（如.git .vscode）
  - [ ] 管理Windows右键菜单
- [ ] 可视化配置界面
- [ ] 多语言支持
- [ ] 支持Linux平台

## 使用方法

暂时不开放

# English

## Project Overview

`easy_copy_file_tree` is a Windows command-line utility that automatically generates a directory tree from a specified folder and copies the result directly to the system clipboard.

It is designed to help developers quickly insert project structures into documentation, README files, design documents, technical reports, and other written materials.

Example output:

```text
easy_copy_file_tree/
├──inc/
│  ├──file_tree.h
│  └──tree.h
├──src/
│  ├──file_tree.c
│  ├──main.c
│  └──tree.c
├──Makefile
├──README.md
└──run.exe
```

The generated tree is automatically copied to the system clipboard and can be pasted directly into Markdown documents, Word files, blogs, or other technical documentation.

## Features

### Implemented

- [x] Support for Unicode path arguments (Chinese, Japanese, Korean, and other non-ASCII characters)
- [x] Automatic directory tree generation
- [x] Automatic copying of generated results to the system clipboard

### Planned Features

- [ ] Windows Explorer context menu integration
  - [ ] Generate directory trees directly from the folder right-click menu
- [ ] Configuration file support
  - [ ] Custom output encoding
  - [ ] Custom tree-style symbols
  - [ ] Exclude specified files or directories (e.g. `.git`, `.vscode`)
  - [ ] Manage Windows context menu integration
- [ ] Graphical configuration interface
- [ ] Multi-language support
- [ ] Linux support

## Usage

Not available at this stage.
