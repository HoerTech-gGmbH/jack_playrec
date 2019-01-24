#!/bin/bash
# Copy all binaries into the destination folder

function fix_install_names {
    path=$1
    file=$2;
    if [[ $1 == "lib" ]]; then
        INSTALL_NAME=$(echo @rpath/../lib/`basename $file` | sed -E 's/\.[0-9]+//g');
        install_name_tool -id $INSTALL_NAME ./lib/`basename $file`;
        DYLIBS=$(otool -L ./$path/`basename $file` | grep -v "/usr/lib"| grep -v "/System" | grep -v ":" | awk -F' ' '{ print $1 }' | sed '1d')
    else
        DYLIBS=$(otool -L ./$path/`basename $file` | grep -v "/usr/lib"| grep -v "/System" | grep -v ":" | awk -F' ' '{ print $1 }')
    fi
    for dylib in $DYLIBS; do
        LIB_NAME=$(echo @rpath/../lib/`basename $dylib`| sed -E 's/\.[0-9]+//g');
        install_name_tool -change $dylib $LIB_NAME ./$path/`basename $file`;
    done;
}
mkdir -p bin
mkdir -p lib
cp ../../bin/* bin/.
for file in $(cat expected_dependencies.txt); do
    cp $file lib/.;
done;
for file in lib/*dylib; do
    fix_install_names lib $file;
done;
for file in bin/*; do
    fix_install_names bin $file;
done;
