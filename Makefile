# Copyright (C) 2021 m4gh3

#This file is part of cgnale.

#    cgnale is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    cgnale is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with cgnale.  If not, see <http://www.gnu.org/licenses/>.

INSTALL_BIN_PATH = $${HOME}/bin

CXXFLAGS=-g

all: build/cgnale build/cgnale_libs/libcgnalec.so

build/cgnale: cgnale.cpp
	g++ $(CXXFLAGS) cgnale.cpp -ldl -o build/cgnale

build/cgnale_libs/libcgnalec.so: libcgnalec.cpp
	g++ $(CXXFLAGS) -shared libcgnalec.cpp -fPIC -o build/cgnale_libs/libcgnalec.so

install:
	cp build/cgnale ${INSTALL_BIN_PATH}
	cp cgnalec ${INSTALL_BIN_PATH}
	mkdir -p $${HOME}/.local/share/cgnale
	cp -r include $${HOME}/.local/share/cgnale
	cp -r build/cgnale_libs/ $${HOME}/.local/share/cgnale

clean:
	rm build/cgnale
	rm build/cgnale_libs/*
