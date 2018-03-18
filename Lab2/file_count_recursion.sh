#！/bin/bash

# prompt
echo "Welcome to File Counting -- Author Yilin Zheng"
echo "This script will count the files and directories in home directory"

# exam whether the file is exist
file=./file.info
if [ ! -e $file ];
then
    echo "file.info not found, creating a new one..."
    touch ./file.info # create a new file
    echo "Created."
else 
    echo "file.info exists and data in file.info will be wiped..."
    > file.info  # empty the contain
    echo "Fine."
fi


# function
dir_counter=0
file_counter=0
function count_file
{
    echo [ ${1#"$(dirname $HOME)"} ] >> $file #output directory name
    
    for f in "$1"/*;
    do
        echo ${f#"$(dirname $HOME)"} >> $file #output items in this directory
    done
    for f in "$1"/*;
    do
        if [ -d "$f" ];
        then
            let dir_counter++ #count directories
            echo >> $file #output a new line
            count_file "$f" #recursively call this function
        elif [ -f "$f" ];
        then
            let file_counter++ #count files
        fi
    done
}

#first function call
count_file $HOME

echo >> $file
#output counting results
echo [ Directories Count ] : $dir_counter >> $file
echo [ File Count ] : $file_counter >> $file

