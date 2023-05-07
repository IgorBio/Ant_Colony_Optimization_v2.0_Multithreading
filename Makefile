PROJECT=Parallels

.PHONY: all build ant run_ant rebuild dvi check style cppcheck clean

CPPCHECKFLAG = --enable=all --language=c++ --suppress=unusedStructMember --suppress=missingIncludeSystem --std=c++17

all: build

build: ant
	
ant:
	@cmake -S . -B build
	@cmake --build build

run_ant: ant
	@./build/Ant

rebuild: clean build

dvi:
	doxygen ./docs/Doxyfile
	open ./docs/html/index.html

check: style cppcheck

style: 
	clang-format -style=google -n -verbose *.cc *.h

cppcheck:
	@cppcheck $(CPPCHECKFLAG) */*.cc */*/*.cc *.cc  */*.h */*/*.h *.h

clean:
	@rm -rf *.o *.out ./docs/html ./build
