RELEASE_BINARY_PATH=`pwd`"/../build_release/CatDB"
DEBUG_BINARY_PATH=`pwd`"/../build_debug/CatDB"
BINARY_PATH=$RELEASE_BINARY_PATH
CONF_FILE="catdb.conf"
DATA_DIR="."
RECYCLE_DIR="."
LOG_FILE="catdb.log"
IP="127.0.0.1"
PORT="1234"
PID=""

print_usage() {
    echo "./deploy.sh [init | start | restart | stop | sql | build]"
    echo "      init    : 初始化并启动数据库"
    echo "      start   : 启动数据库"
    echo "      restart : 重启数据库"
    echo "      stop    : 停止数据库"
    echo "      sql     : 启动数据库客户端"
    echo "      build [debug | release]   : 重新编译server"

}

get_CatDB() {
    if [ -f "$RELEASE_BINARY_PATH" ]
    then 
        BINARY_PATH=$RELEASE_BINARY_PATH
    elif [ -f "$DEBUG_BINARY_PATH" ]
    then
        BINARY_PATH=$DEBUG_BINARY_PATH
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
    N=`ps -aux | grep -c CatDB`
    if [ $N != 2 ]
    then
        return 0
    fi
    PID=`ps -aux | grep CatDB | awk -F ' ' '{print $2}' | awk 'NR==1'`
    return 1
}

init_server() {
    mkdir -p $DATA_DIR
    mkdir -p $RECYCLE_DIR
    mkdir -p $DATA_DIR"/system"
    touch $LOG_FILE
    start_server
    sleep 1
    echo "source init_sys_table.sql" | mysql -h $IP -P $PORT
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
    get_CatDB
    cp $BINARY_PATH .
    nohup ./CatDB > log.txt 2>&1 &
    echo "succeed to start CatDB"
}

stop_server() {
    find_pid
    if [ $? != 0 ]
    then
        echo $PID | xargs kill
    fi
    echo "succeed to stop CatDB"
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
    mysql -h $IP -P $PORT
}

build_server() {
    if [ "$1" == "debug" -o "$1" == "release" ]
    then
        cd ../
        ./build.sh $1
    else
        echo "./deploy.sh build [debug | release]"
    fi
}

if [ $# == 1 ]
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
        sleep 2
        start_server
    elif [ "$1" == "stop" ]
    then
        stop_server
    elif [ "$1" == "sql" ]
    then
        read_conf
        run_client
    else
        print_usage
    fi
elif [ $# == 2 ]
then
    if [ "$1" == "build" ]
    then
        build_server $2
    else
        print_usage
    fi
else
    print_usage
fi