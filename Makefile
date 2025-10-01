CXX := g++

BASE_CPPFLAGS = -I./include -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla

NORMAL_CPPFLAGS = $(BASE_CPPFLAGS) -O2

DEBUG_CPPFLAGS = $(BASE_CPPFLAGS) -ggdb3 -O0 -D_DEBUG -D_CANARY -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

SOURCES := src/main.cpp src/stack.cpp
NORMAL_OBJECTS := $(SOURCES:src/%.cpp=obj/normal/%.o)
DEBUG_OBJECTS := $(SOURCES:src/%.cpp=obj/debug/%.o)

TARGET := my_stack

all: $(TARGET)

$(TARGET): $(NORMAL_OBJECTS)
	$(CXX) $(NORMAL_CPPFLAGS) -o $(TARGET) $(NORMAL_OBJECTS)

debug: $(TARGET)_debug

$(TARGET)_debug: $(DEBUG_OBJECTS)
	$(CXX) $(DEBUG_CPPFLAGS) -o $(TARGET)_debug $(DEBUG_OBJECTS)

obj/normal/%.o: src/%.cpp | obj/normal
	$(CXX) $(NORMAL_CPPFLAGS) -c $< -o $@

obj/debug/%.o: src/%.cpp | obj/debug
	$(CXX) $(DEBUG_CPPFLAGS) -c $< -o $@

obj/normal:
	mkdir -p obj/normal

obj/debug:
	mkdir -p obj/debug

clean:
	rm -rf obj $(TARGET) $(TARGET)_debug

.PHONY: all debug clean