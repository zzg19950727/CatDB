use system;
create table user(name varchar,host varchar,auth_string varchar);
create table table_statis(tid int,row_count int, space_size int, analyze_time datetime);
create table column_statis(tid int, cid int, ndv int, null_count int, max_value int, min_value int, analyze_time datetime);
create table sys_vars(name varchar(64), value varchar(64));
create table charset(charset varchar(20),description varchar(64),default_collation varchar(64),maxlen int);
insert into charset values('utf8mb4','UTF-8 Unicode','utf8mb4_general_ci',4);
create table `engine`(`engine` varchar(20),support bool,comment varchar(64),transactions bool, XA bool,savepoints bool);
insert into `engine` values('catdb',1,'test db',0,0,0);
create table collation(collation varchar(20),charset varchar(20), id int, default bool,complied bool, sortlen int);
insert into collation values('utf8mb4_bin','utf8mb4',46,0,0,1);
create table memory_use(mode varchar(102), use_count int);
create table memory_trace(mode varchar(102), ptr varchar(64), release bool, trace_info varchar(102));
create database test;
