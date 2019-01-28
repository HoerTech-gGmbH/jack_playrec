#!/bin/bash -ex
# Copy all binaries into the destination folder
cp ../../README.md .
cp ../../LICENSE .
mkdir -p bin
cp ../../jack_playrec.exe bin/.
cp ../../jack_par.exe bin/.
# Walk the dependency tree, exclude all files that are already present in ./bin and files in windows/system
cd ./bin
for file in $(ls ./); do
    cygpath -u $(cygcheck.exe ./$file | tr -d "[:blank:]") | grep -v "$(pwd)" | grep -iv "/c/" >> ../tmp;
    if grep -iq "not find" ../tmp; then
        echo "find_and_copy_dependencies:" \n "Error: " `grep -i "not find" ../tmp` >&2
    fi
done
cd ../
sort -u tmp > tmp2
# Sanity check - see if all dependencies that we normally expect are present
for file in $(cat expected_dependencies.txt); do
    if ! grep -iq $file tmp2; then
        echo "find_and_copy_dependencies:" \n "Expected $file to be in list of dependencies!" >&2
        exit 1
    fi
done
# Copy the files found by the dependency walk
for file in $(cat tmp2); do
    cp $file bin/.;
done
rm tmp*
# Matlab toolbox files
mkdir -p mfiles
cp -r ../../*.m mfiles/.
find ./mfiles -name ".*" -exec rm {} \;

