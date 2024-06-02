# cpplog

#### 介绍
自己实现的一个简易的log库

#### 软件架构
软件架构说明


#### 安装教程

1.  一个头文件 Log.h
2.  一个源文件 Log.cpp
3.  直接包含使用即可

#### 使用说明

使用示例：

//use demo：
#include <iostream>
using namespace std;
#include "Log.h"

int main()
{
    /*
     init log for use;
     log file name "log.txt" ; 
     output log to file only( console is also supported);
     log format is datetime+level+fun_name+log_content+source_file_name+line_number
    */
	cpp_log_init("main-log.txt", Log::LOG_LEVEL::DEBUG_LEVEL, Log::LOG_TARGET::ALL, "datetime_level_func_log_source_line");
	cpp_log_debug("Hello World!");
	cpp_log_error("I done this work for " << 32 << " hours.");
	return 0;
}

#### 示例输出

./test_main
cpp_log_init entered.
2023-03-06 14:49:48:936016 DEBUG main() Hello World! /mnt/c/cpplog/test_main.cpp :8
2023-03-06 14:49:48:936355 ERROR main() I done this work for 32 hours. /mnt/c/cpplog/test_main.cpp :9


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
