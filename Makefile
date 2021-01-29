all:
	cd build/; cmake ..

build:
	cd build/; make 

run:
	./build/hello


.PHONY: all build
