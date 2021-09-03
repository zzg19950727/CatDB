BINARY_PATH=`pwd`"/../build/CatDB"
CONF_FILE="catdb.conf"
DATA_DIR="."
RECYCLE_DIR="."
LOG_FILE="catdb.log"
IP="127.0.0.1"
PORT="1234"

print_usage() {
    echo "./deploy.sh [init | start | restart | stop | sql | build]"
    echo "      init    : 初始化并启动数据库"
    echo "      start   : 启动数据库"
    echo "      restart : 重启数据库"
    echo "      stop    : 停止数据库"
    echo "      sql     : 启动数据库客户端"
    echo "      build   : 重新编译server"

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
    check_dir $DATA_DIR
    check_dir $RECYCLE_DIR
    check_dir $DATA_DIR"/system"
    check_file $LOG_FILE
    check_file $BINARY_PATH
    cp $BINARY_PATH .
    nohup ./CatDB > log.txt 2>&1 &
    echo "succeed to start CatDB"
}

stop_server() {
    ps -aux | grep CatDB | awk -F ' ' '{print $2}' | xargs kill
    echo "succeed to stop CatDB"
}

run_client() {
    CLIENT=`which mysql | grep "no mysql"`
    if [ "$CLIENT" != "" ]
    then
        echo "mysql client not install"
        exit
    fi
    mysql -h $IP -P $PORT
}

build_server() {
    cd ../
    ./build.sh
}

if [ $# != 1 ]
then
    print_usage
elif [ "$1" == "init" ]
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
elif [ "$1" == "build" ]
then
    build_server
else
    print_usage
fi