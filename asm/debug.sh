./build.sh
gdb build/$1
break $1_long_long
