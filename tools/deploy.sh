#!/bin/bash
BUILD_PATH=`pwd`"/../build_"
CATDB_PATH=""
GENERATOR_PATH=""
CONF_FILE="catdb.conf"
DATA_DIR="test"
RECYCLE_DIR="test"
LOG_FILE="catdb.log"
IP="127.0.0.1"
PORT="1234"
PID=""

print_usage() {
    echo "Usage: ./deploy.sh [option]"
    echo "Options:"
    echo "      init                        init and start database service"
    echo "      start                       start database service"
    echo "      restart                     restart database service"
    echo "      stop                        stop database service"
    echo "      sql                         connect to database with mysql clinet"
    echo "      build                       rebuild server"
    echo "      random_test                 generate random test case"
    echo "      test                        run test"
    echo "      log                         get trace log"

}

get_CatDB() {
    if [ $# != 1 ]
    then
        exit
    fi
    CATDB_PATH="$BUILD_PATH""$1""/CatDB"
    if [ -f "$CATDB_PATH" ]
    then 
        GENERATOR_PATH="$BUILD_PATH""$1""/tools/random_test/SqlGenerator"
    else
        echo "CatDB not build"
    fi
}

read_conf() {
    if [ ! -f "$CONF_FILE" ]
    then
        echo "$CONF_FILE not exists"
        exit
    fi
    DATA_DIR=`cat $CONF_FILE | grep data_dir | awk -F '=' '{print $2}'`
    RECYCLE_DIR=`cat $CONF_FILE | grep recycle_dir | awk -F '=' '{print $2}'`
    LOG_FILE=`cat $CONF_FILE | grep log_file | awk -F '=' '{print $2}'`
    IP=`cat $CONF_FILE | grep ip | awk -F '=' '{print $2}'`
    PORT=`cat $CONF_FILE | grep port | awk -F '=' '{print $2}'`
}

check_file() {
    if [ $# != 1 ]
    then
        exit
    fi
    if [ ! -f "$1" ]
    then
        echo "$1"" not exists"
        exit
    fi
}

check_dir() {
    if [ $# != 1 ]
    then
        exit
    fi
    if [ ! -d "$1" ]
    then
        echo "$1"" not exists"
        exit
    fi
}

find_pid() {
    N=`ps -ax | grep CatDB | grep -v "grep" | grep -c CatDB`
    if [ $N -eq 1 ]
    then
        PID=`ps -ax | grep CatDB | grep -v "grep" | awk -F ' ' '{print $1}'`
        return 1
    fi
    return 0
}

run_sql() {
    echo "$1" | mysql -h $IP -P $PORT -c -vvv
}

init_server() {
    mkdir -p $DATA_DIR
    mkdir -p $RECYCLE_DIR
    mkdir -p $DATA_DIR"/system"
	touch $DATA_DIR"/system/sys_databases"
	touch $DATA_DIR"/system/sys_tables"
	touch $DATA_DIR"/system/sys_columns"
    touch $LOG_FILE
    start_server
    sleep 1
    run_sql "source init_sys_table.sql"
    echo "succeed to init CatDB"
}

start_server() {
    find_pid
    if [ $? != 0 ]
    then
        echo "CatDB is running"
        exit
    fi
    check_dir $DATA_DIR
    check_dir $RECYCLE_DIR
    check_dir $DATA_DIR"/system"
    check_file $LOG_FILE
    check_file "./CatDB"
    nohup ./CatDB > log.txt 2>&1 &
    sleep 1
    find_pid
    if [ $? != 0 ]
    then
        echo "succeed to start CatDB"
    else
		tail -n 10 $LOG_FILE
        echo "failed to start CatDB"
    fi
}

stop_server() {
    find_pid
    if [ $? == 0 ]
    then
        echo "CatDB not start"
    else
        #echo $PID | xargs kill
		killall CatDB
        echo "succeed to stop CatDB"
    fi
}

run_client() {
    find_pid
    if [ $? == 0 ]
    then
        echo "CatDB not start"
        exit
    fi
    CLIENT=`which mysql | grep "no mysql"`
    if [ "$CLIENT" != "" ]
    then
        echo "mysql client not install"
        exit
    fi
    mysql -h $IP -P $PORT -c
}

build_server() {
    CUR_PATH=`pwd`
    cd ../
    ./build.sh $@
    get_CatDB $@
    cp $CATDB_PATH $CUR_PATH
    cp $GENERATOR_PATH $CUR_PATH
	cd $CUR_PATH
}

print_random_test_usage() {
    echo "random_test suite case_name"
}

random_test() {
    if [ $# != 2 ]
    then
        print_random_test_usage
        exit
    elif [ ! -f "./SqlGenerator" ]
    then
	print_random_test_usage
	exit
    else
			out_file="test/suite/$1/t/$2.test"
			config_file="random_test.conf"
            mkdir -p test/suite/$1/t
            mkdir -p test/suite/$1/r
            ./SqlGenerator $config_file $out_file
    fi
}

run_tpch_test() {
    echo "start load tpch data..."
    #run_sql "source tpch_load.sql;"
    echo "end load tpch data"
    echo "start gather statis"
    #run_sql "exec dbms_stats.gather_database_stats('tpch');"
    echo "end gather statis"
    all_query=`ls tpch-queries/`
	queries=($all_query)
	for query in ${queries[@]}
	do
        start_time=`date +%s`
        echo "start run query:$query"
        sql=`cat tpch-queries/$query`
        sql="use tpch;explain ""$sql"
		run_sql "$sql" >> log.txt
        end_time=`date +%s`
        time=`echo $start_time $end_time | awk '{print $2-$1}'`
        echo "$query cost $time s" >> tpch.log
        echo "end run query, cost $time s"
	done 
    echo "finish tpch test!"
}

run_test() {
    if [ "$1" == "tpch" ]
    then
        run_tpch_test
    else
        cd test
        ./my_test.sh $IP $PORT $@
    fi
}

watching_log() {
    grep $1 $LOG_FILE | vim -
}

if [ $# != 0 ]
then
    if [ "$1" == "init" ]
    then
        read_conf
        init_server
    elif [ "$1" == "start" ]
    then
        read_conf
        start_server
    elif [ "$1" == "restart" ]
    then
        read_conf
        stop_server
        sleep 1
		build_server "$2"
		sleep 1
        start_server
    elif [ "$1" == "stop" ]
    then
        stop_server
    elif [ "$1" == "sql" ]
    then
        read_conf
        run_client
    elif [ "$1" == "log" ]
    then
        read_conf
        watching_log $2
    elif [ "$1" == "build" ]
    then
        build_server $2
    elif [ "$1" == "random_test" ]
    then
        random_test $2 $3
    elif [ "$1" == "test" ]
    then
        read_conf
        run_test $2
    else
        print_usage
    fi
else
    print_usage
fi
