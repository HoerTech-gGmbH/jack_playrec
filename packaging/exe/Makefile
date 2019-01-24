.PHONY: build

REPO=../../

exe: build
	./find_and_copy_dependencies.sh
	makensis jack_playrec.nsi

build:
		@(cd $(REPO) && make install)
