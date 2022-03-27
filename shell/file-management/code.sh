#!/bin/sh

command=$1

if [ $command = "" ]
	then echo "chua nhap chuc nang"
fi

# file management
if [ $command = "list" ]
	then
		ls -la
else if [ $command = "create-file" ]
	then
		if [ $2 = "" ]
			then echo "chua co ten file"
			else touch $2
		fi

else if [ $command = "remove-file" ]
	then
		if [ $2 = "" ]
			then echo "chua co ten file"
			else rm -rf $2
		fi

else if [ $command = "show-file" ]
	then
		if [ $2 = "" ]
			then echo "chua co ten file"
			else less $2
		fi
else if [ $command = "edit-file" ]
	then
		if [ $2 = "" ]
			then echo "chua co ten file"
			else vim $2
		fi

else if [ $command = "rename-file" ]
	then 
		if [ $2 = "" -o $3 = "" ]
			then echo "chua nhap ten file va ten file moi"
			else mv $2 $3
		fi
else if [ $command = "copy-file" ]
	then
		if [ $2 = "" -o $3 = "" ]
			then echo "chua nhap ten file va file dich"
			else cp $2 $3
		fi
fi