# ./test.sh <input_folder> <word>

INPUT_FOLDER=$1
if [ -z "$INPUT_FOLDER" ]
then
    echo "Input folder not specified"
    exit 1
fi

word=$2
if [ -z "$word" ]
then
    echo "Word not specified"
    exit 1
fi

SMALL_CASE=small
MEDIUM_LARGE_CASE=medium_large
TEMP_FOLDER="temp"
mkdir -p $TEMP_FOLDER
mkdir -p $TEMP_FOLDER/$SMALL_CASE
mkdir -p $TEMP_FOLDER/$MEDIUM_LARGE_CASE

EXE=trab3
small_success_attempts=0
success_attempts=0
function check_diff(){
    file1=$1
    file2=$2
    echo "Checking $file1 and $file2..."
    diff $file1 $file2 > /dev/null 2>&1
    error=$?
    if [ $error -eq 0 ]
    then
        echo "STATUS: Ok!"
        success_attempts=$((success_attempts+1))
    elif [ $error -eq 1 ]
    then
        echo "STATUS: Error!"
    else
        echo "There was something wrong with the diff command"
    fi
    echo "=============================="
}


# check_diff a.txt b.txt
total_small=$(ls $INPUT_FOLDER/$SMALL_CASE/pages/*.txt | wc -l)
total_medium_large=$(ls $INPUT_FOLDER/$MEDIUM_LARGE_CASE/pages/*.txt | wc -l)

for file in ls $INPUT_FOLDER/$SMALL_CASE/pages/*.txt
do
    echo "Running $file..."
    #$EXE $file $word > $TEMP_FOLDER/$(basename $file) 
    cp $file $TEMP_FOLDER/$SMALL_CASE/$(basename $file)
    check_diff $TEMP_FOLDER/$SMALL_CASE/$(basename $file) $file
done

small_success_attempts=$success_attempts
success_attempts=0

for file in ls $INPUT_FOLDER/$MEDIUM_LARGE_CASE/pages/*.txt
do
    echo "Running $file..."
    #$EXE $file $word > $TEMP_FOLDER/$(basename $file) 
    cp $file $TEMP_FOLDER/$MEDIUM_LARGE_CASE/$(basename $file)
    check_diff $TEMP_FOLDER/$MEDIUM_LARGE_CASE/$(basename $file) $file
done

echo '----------------------------------------------------------------------------------------'
echo "Small Summary: $small_success_attempts/$total_small"
echo "Medium/Large Summary: $success_attempts/$total_medium_large"


rm -rf $TEMP_FOLDER