INSTALL_BIN_PATH = $${HOME}/bin

CXXFLAGS=-g

all: build/cgnale build/cgnale_libs/libcgnale_base.so

build/cgnale: cgnale.cpp
	g++ $(CXXFLAGS) cgnale.cpp -ldl -o build/cgnale

build/cgnale_libs/libcgnale_base.so: libcgnale_base.cpp
	g++ $(CXXFLAGS) -shared libcgnale_base.cpp -fPIC -o build/cgnale_libs/libcgnale_base.so

install:
	cp build/cgnale ${INSTALL_BIN_PATH}
	cp cgnalec ${INSTALL_BIN_PATH}
	mkdir -p $${HOME}/.local/share/cgnale
	cp -r include $${HOME}/.local/share/cgnale
	cp -r build/cgnale_libs/ $${HOME}/.local/share/cgnale

clean:
	rm build/cgnale
	rm build/cgnale_libs/*
