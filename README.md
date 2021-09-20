# CatDB简介
以Volcano模型实现SQL引擎，存储引擎以行为存储格式，不支持锁和缓存。

SQL引擎支持基本的insert、delete、update、select，支持多层嵌套子查询（包括相关子查询和非相关子查询），
优化器部分支持N、A、J、JA四种子查询改写，提高子查询的效率。

接口驱动使用MySQL5.0的TEXT协议，各种MySQL客户端可以登录使用。

# Unix平台编译
依赖flex、bison、g++、pthread、cmake [apt-get install flex bison g++ cmake]

预先删除parser/lex_yacc/FlexLexer.h文件，避免linux下bison版本冲突

下载源码：git clone https://github.com/zzg19950727/CatDB.git

编译：./build.sh [ debug | release ]

cd tools

配置数据库：catdb.conf

初始化：./deploy.sh init

使用前确保已经安装mysql客户端

登陆：./deploy.sh sql

测试: ./deploy.sh test all

enjoy it!!!

# Windows平台编译
依赖flex、bison、cmake
使用cmake生成VS工程后编译，
注意点：需要配置FlexLexer.h路径，工程里面包含了已经编译好的windows平台的flex、bison和FlexLexer.h头文件
需要在server工程下配置NOMINMAX，解决bison生成std::max无法通过编译的BUG

# 内存优化
编译时使用google的tcmalloc管理内存，可至少提高20%的查询性能。

