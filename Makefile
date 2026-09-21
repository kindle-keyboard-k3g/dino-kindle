CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2 -Isrc
ARM_CXX ?= arm-linux-gnueabi-g++
ARM_CXXFLAGS ?= -std=c++17 -march=armv6j -mtune=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -Wl,--gc-sections -Isrc

SRC_COMMON := $(wildcard src/util/*.cpp) \
              $(wildcard src/domain/*.cpp) \
              $(wildcard src/game/*.cpp) \
              $(wildcard src/graphics/*.cpp) \
              $(wildcard src/hal/*.cpp) \
              $(wildcard src/persistence/*.cpp) \
              $(wildcard src/application/*.cpp)

SRC_MAIN := src/main.cpp
SRC_TESTS := $(wildcard tests/*.cpp)

OBJ_COMMON := $(patsubst src/%.cpp, obj/%.o, $(SRC_COMMON))
OBJ_MAIN := obj/main.o
OBJ_TESTS := $(patsubst tests/%.cpp, obj/tests/%.o, $(SRC_TESTS))

all: host test

bin obj:
	mkdir -p bin obj/util obj/domain obj/game obj/graphics obj/hal obj/persistence obj/application obj/tests

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/tests/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Itests -c $< -o $@

host: bin/dino

bin/dino: $(OBJ_COMMON) $(OBJ_MAIN)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

test: bin/dino-tests
	./bin/dino-tests

bin/dino-tests: $(OBJ_COMMON) $(OBJ_TESTS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(OBJ_COMMON:.o=.d) $(OBJ_MAIN:.o=.d) $(OBJ_TESTS:.o=.d)

asan: CXXFLAGS += -fsanitize=address,undefined -g
asan: clean bin/dino-tests
	./bin/dino-tests

kindle: bin
	$(ARM_CXX) $(ARM_CXXFLAGS) $(SRC_COMMON) $(SRC_MAIN) -o bin/dino-arm32

package: kindle
	./scripts/package_kindle.sh

run-ansi: host
	./bin/dino --ansi

clean:
	rm -rf bin obj output.ppm dino.ppm tmp_*.dat

.PHONY: all host test asan kindle package run-ansi clean
