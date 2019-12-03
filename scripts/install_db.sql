create database system;
use system;
create table sys_databases(id int, name varchar);
create table sys_tables(id int, name varchar, db_id int);
create table sys_columns(id int, name varchar, table_id int, type varchar);
create table user(name varchar,host varchar,auth_string varchar);
create table table_statis(tid int,row_count int, space_size int, analyze_time datetime);
create table column_statis(tid int, cid int, ndv int, null_count int, max_value int, min_value int, analyze_time datetime);
create database test;