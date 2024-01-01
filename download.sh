#!/usr/bin/env bash

echo "Running Download.sh!"
root_directory=$(pwd)
build_directory=$(pwd)/src/build

if [ -d "$root_directory/src/lib" ]; then
 echo "lib directory exist\n"
else
    # -L: Follow redirects.
    # -J: Save the file with the server-specified name.
    # -O: Save the file with the original name.
    echo "lib does not exist!, downloading dependencies using curl"
    curl -LO https://github.com/chriskohlhoff/asio/archive/master.zip

    echo "Unzipping!"
    unzip master.zip

    rm master.zip
    echo "moving the file!"
    mv asio-master/asio/include $root_directory/src/lib
    rm -rf asio-master
fi

echo "Checking if build directory exist"
#directory exist
if [ -d "$build_directory" ]; then #directory exist
    if [ -z "$(ls -A "$build_directory")" ]; then
    echo "Directory is empty!, building cmake!"
    cmake -B src/build
    # else
    # echo "@@ Directory is not empty!"
    fi
echo "Build Directory exist!"
fi
