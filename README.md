# CatDB简介
借鉴OceanBase0.4开源代码，以Volcano模型实现SQL引擎，存储引擎以行为存储格式，不支持锁和缓存。
SQL引擎支持基本的insert、delete、update、select，支持多层嵌套子查询（包括相关子查询和非相关子查询），
优化器部分支持N、A、J、JA四种子查询改写，提高子查询的效率。
接口驱动使用MySQL5.0的TEXT协议，各种MySQL客户端可以登录使用。

# Unix平台编译
依赖flex、bison、g++、pthread、cmake
预先删除parser/lex_yacc/FlexLexer.h文件，避免linux下bison版本冲突
使用cmake编译

# Windows平台编译
依赖flex、bison、cmake
使用cmake生成VS工程后编译，
注意点：需要配置FlexLexer.h路径，工程里面包含了已经编译好的windows平台的flex、bison和FlexLexer.h头文件
需要在server工程下配置NOMINMAX，解决bison生成std::max无法通过编译的BUG

# 安装使用
拷贝script/catdb.conf到可执行文件目录下，填写相应的配置
然后直接运行数据库服务，使用第三方MySQL客户端可直接登录。
第一次需要执行script/install_db.sql来初始化系统库。

# 内存优化
编译时使用google的tcmalloc管理内存，可至少提高20%的查询性能。