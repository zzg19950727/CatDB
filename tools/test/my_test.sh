IP="127.0.0.1"
PORT=1234

CASE_COUNT=0
CASE_INDEX=0
PASS_COUNT=0
FAIL_COUNT=0
CASES=()
TYPE="unknown"

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
	echo "[======] Running $CASE_COUNT tests"
	echo "[------] "`date`
	for case in ${CASES[@]}
        do
                test_one_case $case
        done	
	echo "[------] $CASE_COUNT tests done "`date`
	if [ $PASS_COUNT -gt 0 ]
	then
		echo -e "\033[32m[ PASS ]\033[0m $PASS_COUNT tests"
	fi
	if [ $FAIL_COUNT -gt 0 ]
        then
                echo -e "\033[31m[ FAIL ]\033[0m $FAIL_COUNT tests"
        fi
}

parse_test_type() {
	if [ $# == 1 ]
	then
		if [ "$1" == "all" ]
		then
			TYPE="all"
			return 1
		fi
		type=`echo "$1" | awk -F '=' '{print $1}'`
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
	for file in `ls suite/$1/t/ | sed 's/.test//g'`
	do
		CASES[${#CASES[*]}]="$1.$file"
	done
}

get_testset_list() {
	OLD_IFS="$IFS"
	IFS=","
	new_cases=(`echo "$1" | awk -F '=' '{print $2}'`)
	CASES=(${CASES[@]}  ${new_cases[@]})
	IFS="$OLD_IFS"
}

get_suite_list() {
        OLD_IFS="$IFS"
        IFS=","
        suites=(`echo "$1" | awk -F '=' '{print $2}'`)
	IFS="$OLD_IFS"
	for suite in ${suites[@]}
        do
                get_all_case $suite
        done
}

PASS=0
test_one_case() {
	if [ $# != 1 ]
	then
		return 0
	fi
	case_name=`echo "$1" | awk -F '.' '{print $2}'`
	suite_name=`echo "$1" | awk -F '.' '{print $1}'`
	((CASE_INDEX=CASE_INDEX+1))
	echo "[ RUN  ] $1 [ $CASE_INDEX / $CASE_COUNT ]"
	test_file="suite/"$suite_name"/t/"$case_name".test"
	result_file="suite/"$suite_name"/r/"$case_name".result"
	reject_file="suite/"$suite_name"/r/"$case_name".reject"
	start_time=`date +%s`
	echo "source "$test_file"" | mysql -h $IP -P $PORT -c -vvv > $reject_file
	if [ $? != 0 ]
	then
		exit
	fi
	format_result $reject_file
	format_result $reject_file
	check_case $result_file $reject_file
	end_time=`date +%s`
	CUR_PATH=`pwd`
	if [ $PASS == 1 ]
	then
		((PASS_COUNT=PASS_COUNT+1))
		time=`echo $start_time $end_time | awk '{print $2-$1}'`
		echo -e "\033[32m[ PASS ]\033[0m cost $time s"
	else
		((FAIL_COUNT=FAIL_COUNT+1))
		echo -e "\033[31m[ FAIL ]\033[0m vimdiff ""$CUR_PATH/$reject_file"" ""$CUR_PATH/$result_file"
	fi
}

format_result() {
	if [ $# != 1 ]
	then
		return 0
	fi
	sed -i 's/(.*sec)//g' $1
	new_line=0
	tmp_file="tmp.txt"
	echo "" > $tmp_file
	while read line
	do
		if [ "$line" == "" ]
		then
			if [ $new_line == 0 ]
			then
				new_line=1
			else
				continue
			fi
		else
			new_line=0
		fi
		is_remove=`echo "$line" | grep -E "Query OK.*|INSERT INTO.*VALUES.*|^--------------$"`
		if [ "$is_remove" == "" ]
		then
			echo "$line" >> $tmp_file
		fi
	done < $1
	mv $tmp_file $1
}

check_case() {
	PASS=0
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
		PASS=1
		return 1
	else
		PASS=0
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
