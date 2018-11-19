#!/bin/sh
exe="cl_mysql" #存放你的可执行文件的名字
des="/root/exe/" #可执行文件的路径
deplist=$(ldd $exe | awk '{if(match($3,"/")){printf("%s "),$3}}')
cp $deplist $des
