CFLAGS=-ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
CFLAGS += ${MODE}
TO_OBJ = obj
TO_SRC = src
TO_PIC = pictures
TO_LOG = logs
MKDO = mkdir -p ${TO_OBJ}
MKDP = mkdir -p ${TO_PIC}
MKDL = mkdir -p ${TO_LOG}
INCLUDE_DIRS = -Iheaders
CC = g++

.PHONY: clean png_clean build

all: build
	./${TO_LOG}/tree

${TO_OBJ}/main.o: ${TO_SRC}/main.cpp
	${MKDO}
	${MKDP}
	${MKDL}
	${CC} ${TO_SRC}/main.cpp ${CFLAGS} ${INCLUDE_DIRS} -c -o ${TO_OBJ}/main.o

${TO_OBJ}/treefuncs.o: ${TO_SRC}/TreeFuncs.cpp
	${MKDO}
	${MKDP}
	${MKDL}
	${CC} ${TO_SRC}/TreeFuncs.cpp ${CFLAGS} ${INCLUDE_DIRS} -c -o ${TO_OBJ}/treefuncs.o

${TO_OBJ}/dump.o: ${TO_SRC}/Dump.cpp
	${MKDO}
	${MKDP}
	${MKDL}
	${CC} ${TO_SRC}/Dump.cpp ${CFLAGS} ${INCLUDE_DIRS} -c -o ${TO_OBJ}/dump.o

${TO_OBJ}/filefuncs.o: ${TO_SRC}/FileFuncs.cpp
	${MKDO}
	${MKDP}
	${MKDL}
	${CC} ${TO_SRC}/FileFuncs.cpp ${CFLAGS} ${INCLUDE_DIRS} -c -o ${TO_OBJ}/filefuncs.o

${TO_OBJ}/calcfuncs.o: ${TO_SRC}/CalcFuncs.cpp
	${MKDO}
	${MKDP}
	${MKDL}
	${CC} ${TO_SRC}/CalcFuncs.cpp ${CFLAGS} ${INCLUDE_DIRS} -c -o ${TO_OBJ}/calcfuncs.o


build: ${TO_OBJ}/main.o ${TO_OBJ}/treefuncs.o ${TO_OBJ}/dump.o ${TO_OBJ}/filefuncs.o ${TO_OBJ}/calcfuncs.o
	${CC} ${TO_OBJ}/main.o ${TO_OBJ}/treefuncs.o ${TO_OBJ}/dump.o ${TO_OBJ}/filefuncs.o ${TO_OBJ}/calcfuncs.o ${CFLAGS} ${INCLUDE_DIRS} -o ${TO_LOG}/tree

clean:
	rm -rf ${TO_OBJ}/*.o

png_clean:
	rm -rf ${TO_PIC}/*.png
