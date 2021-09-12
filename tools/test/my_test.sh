#!/bin/bash
IP="127.0.0.1"
PORT=1234

CASE_COUNT=0
CASE_INDEX=0
CASES=()
TYPE="all"

print_usage() {
	echo "my_test IP PORT [option]"
	echo "option:"
	echo "	all"
	echo "	suite=suite_name,suite_name,..."
	echo "	testset=suite_name.case_name,suite_name.case_name,..."
}

run_test() {
	if [ $# != 1 ]
	then
		print_usage
		return 0
	fi
	parse_test_type $1
	if [ "$TYPE" == "all" ]
	then
		get_all_suite
	elif [ "$TYPE" == "testset" ]
	then
		get_testset_list $1
	elif [ "$TYPE" == "suite" ]
	then
		get_suite_list $1
	else
		echo "unknown case type ""$1"
		exit
	fi
	CASE_COUNT=${#CASES[@]}
	echo "[======] "`date`
	for case in ${CASES[@]}
        do
                test_one_case $case
        done	
	echo "[======] "`date`
}

parse_test_type() {
	if [ $# == 1 ]
	then
		if [ "$1" == "all" ]
		then
			TYPE="all"
			return 1
		fi
		type=`echo $1 | awk -F '=' '{print $1}'`
		if [ "$type" == "testset" ]
		then
			TYPE="testset"
			return 2
		elif [ "$type" == "suite" ]
		then
			TYPE="suite"
			return 3
		fi
	else
		return 0
	fi
}

get_all_suite() {
        all_suite=`ls suite/`
        suites=($all_suite)
	for suite in ${suites[@]}
	do
		get_all_case $suite
	done
}

get_all_case() {
    	if [ $# != 1 ]
    	then
                return 0
        fi
        all_case=`ls suite/$1/t/`
        cases=($all_case)
        index=0
        for case in ${cases[@]}
        do
                cases[$index]="$1""."`echo $case | awk -F '.' '{print $1}'`
                ((index=index+1))
        done
	CASES=(${CASES[@]}  ${cases[@]})
}

get_testset_list() {
	OLD_IFS="$IFS"
	IFS=","
	cases=(`echo $1 | awk -F '=' '{print $2}'`)
	CASES=(${CASES[@]}  ${cases[@]})
	IFS="$OLD_IFS"
}

get_suite_list() {
        OLD_IFS="$IFS"
        IFS=","
        suites=(`echo $1 | awk -F '=' '{print $2}'`)
	for suite in ${suites[@]}
        do
                get_all_case $suite
        done
        IFS="$OLD_IFS"
}

test_one_case() {
	if [ $# != 1 ]
	then
		return 0
	fi
	case_name=`echo $1 | awk -F '.' '{print $2}'`
	suite_name=`echo $1 | awk -F '.' '{print $1}'`
	((CASE_INDEX=CASE_INDEX+1))
	echo "[ RUN  ] ""$1"".""$2"" [""$CASE_INDEX""/""$CASE_COUNT""] "
	test_file="suite/"$suite_name"/t/"$case_name".test"
	result_file="suite/"$suite_name"/r/"$case_name".result"
	reject_file="suite/"$suite_name"/r/"$case_name".reject"
	echo "source "$test_file"" | mysql -h127.0.0.1 -P12345 -c -vvv 1> $reject_file 2> $reject_file
	sed -i 's/(.* sec)//g' $reject_file
	check_case $result_file $reject_file
	if [ $? == 1 ]
	then
		echo "[ PASS ]"
	else
		echo "[ FAIL ] ""$reject_file"" ""$result_file"
	fi
}

check_case() {
	if [ $# != 2 ]
	then
		return 0
	fi
	if [ ! -f "$1" -o ! -f "$2" ]
	then
		return 0
	fi
	cat $1 $2 | sort | uniq -d > tmp.txt
	cat $1 tmp.txt | sort | uniq -u > tmp2.txt
	result=`cat tmp2.txt`
	rm -f tmp.txt tmp2.txt
	if [ "$result" == "" ]
	then
		return 1
	else
		return 0
	fi
}

main() {
	if [ $# != 3 ]
	then
		print_usage
		exit
	fi
	IP=$1
	PORT=$2
	run_test $3
}

main $@
