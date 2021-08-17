CXX = g++
CXXFLAGS = -g -w -Wall -Wextra -std=c++11

IncludeDir = ./mips_codes/

deps = $(wildcard $(IncludeDir)*.h)
src = $(wildcard $(IncludeDir)*.cpp)
obj = $(patsubst %.cpp,%.o,$(src))

all: bin bin/mips_run

bin/mips_run: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: $(IncludeDir)%.cpp $(deps)
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY:clean

bin:
	mkdir -p bin
clean:
	rm -rf bin
