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

# 源码解读
## 目录
1. 数据库系统架构
2. 服务端模块
3. 存储引擎模块
4. SQL引擎
4.1. 语法解析
4.2. 语义解析
4.3. 查询重写
4.4. 逻辑计划生成及优化
4.4.1. join reorder算法
4.4.2. 统计信息
4.4.3. 代价模型
4.5. 物理计划生成
4.6. 物理计划执行
5. 自动化测试框架
5.1. 随机查询生成器
5.2. 测试集维护
5.3. deploy脚本

## 1.数据库系统架构

        ###############  SQL   ###############        ###############        ###############
        #             # =====> #             #  SQL   #             #  stmt  #             #
        # MySQL 客户端 #        #    服务端    # =====> #  SQL解析器   # =====> #   查询重写   #
        #             # <===== #             #        #             #        #             #
        ###############  DATA  ###############        ###############        ###############
                                      /\                                              ||   
                                      ||                                              || stmt    
                                      || result                                       ||  
                                      ||                                              \/   
        ###############        ###############        ###############        ###############
        #             # 行记录  #             # 物理计划 #             # 逻辑计划#             #
        #   存储引擎   # =====> #   执行引擎    # <===== #    编译器    # <===== # 逻辑计划优化  #
        #             #        #             #        #             #        #             #
        ###############        ###############        ###############        ###############

## 2.服务端模块
    服务端的源码位于project_dir/server，主要由mysql客户端协议包解析、网络监听、数据收发、线程池、内存池等模块组成。
    提供的系统服务：响应客户端请求。
    project_dir/server/packet目录下封装了mysql客户端各种协议包的解析工具。
## 3.存储引擎模块
    存储引擎的源码位于project_dir/storage，主要由IO服务、页管理、表空间管理模块组成。
    存储引擎提供了行存系统，与INNODB类似，采用页管理方式，每一页存在固定大小的记录（默认16K），页也是IO的基本单位。
    每张表对应一个表空间，表空间由连续的页组成。
    存储引擎不提供主键约束，采用堆表设计，即所有的记录插入均追加在表空间的最后一个页。
	更新采用的是删除-插入策略，即删除原来的行，再追加一行。注意这里的删除并不会移动数据页，只是标记删除。这个策略优点在于更新也能使用堆表的优点，避免数据页的移动，缺点也很明显，在大量的更新、删除操作之后，表空间会存在大量空隙，导致查询性能下降。目前的策略是定期整理表空间，此期间表空间不可写。
## 4.SQL引擎模块
	SQL引擎的源码位于project_dir/sql，主要由parser、resolver、transformer、optimizer、executor五个模块组成。
	SQL引擎的核心功能是解析并优化用户的查询，生成最优的物理计划，执行并返回查询结果，下面会依次介绍每个子模块。
### 4.1语法解析
	语法解析模块的源码为project_dir/sql/parser，parser采用lex作为词法解析器，bison作为语法解析器，支持基本的ddl、dml语句。
	增加或修改SQL语法时只需要修改parser.l、parser.y文件，然后重新配置cmake。
### 4.2语义解析
	语义解析模块的源码位于project_dir/sql/resolver，resolver主要是解析查询语句是否合法，例如引用的表是否存在，引用的列是否在表中，聚合函数是否只在select、having中使用等等。
	除基础的语义检查外，resolver还有一个非常重要的作用，用statement结构描述用户的查询。
	CmdStmt用于描述用户的ddl命令，如创建、删除表。
	SelectStmt用于描述select语句，查询的输出用一个表达式数组描述，from引用的表使用TableStmt数组描述，where条件用表达式数组描述，group by用表达式数据描述，having用表达式数组描述，order by用OrderbyItem描述，limit用limitStmt描述。
	其中TableStmt还有三个派生类，用于分别描述基表、视图、显示连接表（如A full join B on A.c1 = B.c1）。
	update、insert、delete分别用UpdateStmt、InsertStmt、DeleteStmt描述，另外from和where字段用公共基类DmlStmt描述。
	ExprStmt是一个挺有趣的设计，详细部分大家可以阅读代码。
	Resolver相关类用于解析各个stmt。
### 4.3查询重写
	查询重写模块的源码位于project_dir/sql/transformer，查询重写是SQL引擎一个非常重要的模块，主要作用是对用户的查询作等价变换，使之查询效率变高，例如exists子查询改成semi join执行。
	查询重写框架采用的是规则式的迭代框架，即每种重写用一个TransformRule描述，每一轮对查询依次应用规则集中重写规则，当且仅当任意一个重写规则生效时才会触发下一轮重写，直至不再有任何规则生效或者达到迭代次数上限。
### 4.4逻辑计划生成及优化
	逻辑计划优化的源码位于project_dir/sql/optimizer，log前缀相关文件定义了各种逻辑算子，plan后缀相关文件定义了各种计划生成函数，opt_est前缀文件定义各种估算函数（行数估算、代价估算）。
#### 4.4.1 join reorder
	代码入口：dml_plan.cpp:generate_join_order，逻辑计划优化最重要的是选择一个最优连接顺序，不同的连接顺序代价之间的差异是非常大的。
	本系统实现了一个动态规划算法用于枚举所有可能的连接顺序，同时实现了一种CDC冲突检测算法，用于检查枚举过程中的无效计划。具体算法原理详见论文：<on-the-correct-and-complete-enumeration-of-the-core-search-space>。这个算法的优点在于可以枚举所有连接类型，包括内连接、外连接、半连接及及其他自己扩展的连接算法，其次把枚举过程与无效计划剪枝、代价竞争解耦。开发者可以任意增加其他枚举算法如贪心、遗传、随机算法，无需关心无效计划的检查原理及计划选择，只需要关注如何高效枚举的过程。
	算法第一步是下推所有的过滤条件，使之尽可能的靠近基表扫描，减少数据量；第二步，使用初始计划树生成冲突规则集，用于无效计划检查；第三步，连接顺序枚举，目前采用动态规划算法，算法每次选择左右表集，CDC冲突检测器自动选择可用的连接条件，枚举框架自动选用最佳连接算法（目前只支持hash join和nestloop join）生成连接树并计算相应的成本，同时完成剪枝。
	生成最优的连接计划树后，dml生成剩下的逻辑计划，例如查询可能需要生成group by算子、order by算子等。
#### 4.4.2统计信息
	源码文件：sql/optimizer/statis_manager.cpp，数据库系统维护了表的基础统计信息，用户通过analyze table命令收集指定表的统计信息。基础统计信息主要包含数值、时间类型列的最大最小值、不同值的数量、null值数量、表的行数与数据页的数量。优化器根据统计信息可以估算计划树的输出行数。
#### 4.4.3代价模型
	源码文件：sql/optimizer/opt_est_sel.cpp，代价模型用于评估每个计划的成本，依据此模型选择执行效率最高的计划。因此代价模型影响着优化器的选择权，准确的代价模型能够帮助优化器选择好的计划，错误的代价模型会误导优化器的选择。
	代价模型是基于每个物理算子的实现设计的，主要考虑的因素有IO成本和CPU成本，以执行时间（微秒）作为成本单位。
#### 4.4.4物理计划生成
	物理计划的生成过程分为两步，第一步，表达式分配，为每个物理算子分配输入、计算、输出的表达式。其他作表达式分配的好处在于每个算子在执行期只需要关注自己的逻辑计算，无需关注如何与其他算子交互。第二步，逻辑算子翻译成对应的物理算子，同时需要把表达式翻译成执行态表达式。
	说明一下为何要同时引入逻辑算子和物理算子，咋一看两种算子的实现几乎相同，但是由于优化和执行的需要，在优化期和执行期算子所需的信息不同，为了避免代码混乱，增加代码的可读性和可维护性，因为逻辑算子和物理算子，拆分两段代码逻辑。
#### 4.4.5物理计划执行
	代码目录：sql/executor，物理算子采用火山模型设计，数据在物理算子树中自底向上流动，每个算子不向上面的算子保证数据内存不被释放，即每个算子收到层算子的数据时如果需要使用多次数据，需要自己拷贝一份数据，否则会访问未知内存。
	每个物理算子都需要实现四个接口：open准备计划执行前的工作，约定open接口不会有任何数据流动；inner_get_next_row，计算输出一行数据，注意filter的计算及输出数据的计算由基类的get_next_row实现；reset重制算子状态以准备重新执行，例如nestloop join的右子计划每次都需要reset；close计划执行结束后的清理工作。
	算子之间的交互数据只有row（一行数据）。
## 5.测试
	为了更好的测试新功能，本系统维护了一套测试工具，方面测试新功能是否引起老功能的回退，或者发现新bug。
### 5.1随机查询生成器
	源码目录在project_dir/tools/random_test，开发者可自由配置随机参数，生成指定模式的查询，并放入回归集合中。
### 5.2测试集维护
	实现新功能后需要编写对应的测试（主要是通过查询测试），并放入project_dir/tools/test/suite，根据功能分类放入对应的测试集中。编写完成之后通过deploy脚本启动测试。
### 5.3 deploy脚本
	为了方便开发，deploy可一键编译、部署、重启server，同时还能启动测试，查询当前系统日志。
	Usage: ./deploy.sh [option]
	Options:
		init                        init and start database service
		start                       start database service
		restart                     restart database service
		stop                        stop database service
		sql                         connect to database with mysql clinet
		build                       rebuild server
		random_test                 generate random test case
		test                        run test