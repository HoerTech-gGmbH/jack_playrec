.PHONY: all
all: build pkg
.PHONY= build
include luggage.make

#jack_playrec project directory
REPO=../../

#Include meta-information in the package
PACKAGE_VERSION=1.0.0
TITLE=jack_playrec
REVERSE_DOMAIN=org.openmha

#Targets to execute before packing
PAYLOAD= \
        usr-local-bin \
        usr-local-lib \
	mfiles \
	docfiles \
	bundle-uninstall-jack_playrec


#Basename of the resulting package. Set PACKAGE-VERSION on the command line
UTIL=jack_playrec-installer

build:
	@./find_and_copy_dependencies.sh
	@(cd $(REPO) && make install)

usr-local-bin: l_usr_local_bin
	@${CP} ./bin/* ${WORK_D}/usr/local/bin/

usr-local-lib: l_usr_local_lib
	@${CP} ./lib/* ${WORK_D}/usr/local/lib/

mfiles: l_usr_local_lib
	@mkdir -p ${WORK_D}/usr/local/lib/jack_playrec/mfiles
	@${CP} ${REPO}/*m ${WORK_D}/usr/local/lib/jack_playrec/mfiles

docfiles:
	@mkdir -p ${WORK_D}/usr/local/doc/jack_playrec/
	@${CP} ${REPO}/README.md ${WORK_D}/usr/local/doc/jack_playrec/

clean:
	@rm -rf tmp/
	@rm -rf bin/
	@rm -rf lib/
	@rm -rf doc/
	@rm -rf mfiles/
	@rm -rf *.pkg
	@rm -rf *.dmg
