#!/bin/bash

function ASSERT_SUCCESS {
	if [ $? -ne 0 ]
	then
		echo $1
		exit 1
	fi
}

if [ "$1" = "" ]
then
	echo "Usage: $0 <project_name>"
	exit 1
fi

PROJECT_NAME=$1
CREATION_DATE=`date`

mkdir $PROJECT_NAME
ASSERT_SUCCESS "Could not create project directory $PROJECT_NAME"

FILE_CONTENT=`cat CMakeLists_Template.txt`

touch $PROJECT_NAME/CMakeLists.txt

function CreateFile {
	while read line; do    
		echo "$line" | sed -e "s/\$PROJECT_NAME/$PROJECT_NAME/" | sed -e "s/\$CREATION_DATE/$CREATION_DATE/" >> $PROJECT_NAME/$2
	done < $1

}

CreateFile CMakeLists_Template.txt CMakeLists.txt
CreateFile main_template.cpp main.cpp

