#!/bin/bash

chknbr () {
    re='^[0-9]+$'
    if ! [[ "$1" =~ $re ]] ; then
        echo "Error: Not a number: $1" >&2; exit 1
    fi
}
(
if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] ; then
    echo "USAGE: change_version_number.sh MAJOR MINOR RELEASE" >&2;
    exit 1;
fi

chknbr $1
chknbr $2
chknbr $3
JACK_PLAYREC_VERSION_MAJOR=$1
JACK_PLAYREC_VERSION_MINOR=$2
JACK_PLAYREC_VERSION_RELEASE=$3

# sed -i is not portable: BSD sed expects a suffix, GNU sed can do without but misinterprets the command as filename
# I could not get both sed flavours to work without this workaround
tmpfile=$(mktemp)
cp defs.hh $tmpfile
sed -e "s/JACK_PLAYREC_VERSION_MAJOR [0-9]*/JACK_PLAYREC_VERSION_MAJOR $JACK_PLAYREC_VERSION_MAJOR/" \
    -e "s/JACK_PLAYREC_VERSION_MINOR [0-9]*/JACK_PLAYREC_VERSION_MINOR $JACK_PLAYREC_VERSION_MINOR/" \
    -e "s/JACK_PLAYREC_VERSION_RELEASE [0-9]*/JACK_PLAYREC_VERSION_RELEASE $JACK_PLAYREC_VERSION_RELEASE/" < $tmpfile > defs.hh

cp packaging/pkg/Makefile $tmpfile
sed "s/PACKAGE_VERSION=.*/PACKAGE_VERSION=$JACK_PLAYREC_VERSION_MAJOR.$JACK_PLAYREC_VERSION_MINOR.$JACK_PLAYREC_VERSION_RELEASE/" < $tmpfile > packaging/pkg/Makefile

cp packaging/exe/jack_playrec.nsi $tmpfile
sed "s/!define PRODUCT_VERSION.*/!define PRODUCT_VERSION $JACK_PLAYREC_VERSION_MAJOR.$JACK_PLAYREC_VERSION_MINOR.$JACK_PLAYREC_VERSION_RELEASE/" < $tmpfile > packaging/exe/jack_playrec.nsi

rm $tmpfile
)
