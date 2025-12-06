CXXFLAGS = -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -fsanitize=address
CXXFLAGS += ${MODE}
LDFLAGS  = -fsanitize=address -fsanitize=undefined -fsanitize=leak	
TO_OBJ = ./obj
TO_SRC = ./src
TO_PIC = pictures
TO_LOG = logs
INCLUDE_DIRS = -Iheaders
CXX = g++

SRC =	${TO_SRC}/main.cpp ${TO_SRC}/TreeFuncs.cpp ${TO_SRC}/Dump.cpp ${TO_SRC}/FileFuncs.cpp ${TO_SRC}/CalcFuncs.cpp ${TO_SRC}/VarsFuncs.cpp
OBJ = 	${TO_OBJ}/main.o ${TO_OBJ}/TreeFuncs.o ${TO_OBJ}/Dump.o ${TO_OBJ}/FileFuncs.o ${TO_OBJ}/CalcFuncs.o ${TO_OBJ}/VarsFuncs.o

.PHONY: all clean png_clean tex_clean make
 
$(TO_OBJ)/%.o: $(TO_SRC)/%.cpp
	${CXX} ${CXXFLAGS} ${INCLUDE_DIRS} -c $^ -o $@

build: $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

all: build
	./build

clean:
	rm -rf ${TO_OBJ}/*.o

png_clean:
	rm -rf ${TO_PIC}/*.png

tex_clean:
	rm -rf *.aux *.log *.pdf
