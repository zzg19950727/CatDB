build_release() {
    mkdir -p build_release
    cd build_release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
}

build_debug() {
    mkdir -p build_debug
    cd build_debug
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make
}

print_usage() {
    echo "./build.sh [debug | release]"
}

if [ $# != 1 ]
then
    print_usage
elif [ "$1" == "debug" ]
then
    build_debug
elif [ "$1" == "release" ]
then
    build_release
else
    print_usage
fi
