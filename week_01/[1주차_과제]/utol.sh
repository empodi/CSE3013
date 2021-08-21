echo "working directory: "
read dirname #directory 이름 입력받아 저장

if [ -n "$dirname" ]; then
    if [ -d "$dirname" ]; then
        echo "$dirname directory exists"
    else
        echo "$dirname directory doesn't exist"
        dirname=""
    fi
else
    dirname=$PWD
    echo "$dirname directory exits"
fi

if [ -n "$dirname" ]; then
    for dir in "$dirname/"*
    do
    if [ -f $dir ]; then
    newname=`echo "$(basename "$dir")" | tr "[a-z][A-Z]" "[A-Z][a-z]"`
    mv $dir "$dirname/$newname"
    fi
    done
fi

