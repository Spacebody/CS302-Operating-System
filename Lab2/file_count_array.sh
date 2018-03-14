#！/bin/bash

# prompt
echo "Welcome to File Counting -- Author Yilin Zheng."
echo "This script will count the files and directories in home directory."

# exam whether the file is exist
file=./file.info
if [ ! -e $file ];
then
    echo "file.info not found, creating a new one..."
    # mkdir -p ~/lab1/xxxxxxxx
    touch $file # create a new file
    echo "Created."
else 
    echo "file.info exists and data in file.info will be wiped..."
    > $file  # empty the contain
    echo "Fine."
fi
 
 echo "Counting..."

# function
dir_counter=0
file_counter=0
dir_queue=()
i=0
function count_file
{
    # loop the array
    while [ $i -lt ${#dir_queue[@]} ];
    do
        echo [ ${dir_queue[$i]#"$(dirname $HOME)"} ] >> $file
        for f in "${dir_queue[$i]}"/*;
        do
            if [ -d "$f" ];
            then
                let dir_counter++
                dir_queue+=("$f")
            else
                let file_counter++
                echo ${f#"$(dirname $HOME)"} >> $file
            fi
        done
        echo >> $file
        let i++
    done
    return 
}


# count_file $HOME
# cd $HOME

path=$HOME
dir_queue+=($path)
count_file


echo >> $file
echo [ Directories Count ] : $dir_counter >> $file
echo [ File Count ] : $file_counter >> $file
echo "Counting Finished."