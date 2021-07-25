CXXFLAGS = -g -std=c++17 -march=native -flto -mavx2 -O3 -funroll-loops

PRGM  = src/main
SRCS := $(wildcard src/main.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(PRGM)

$(PRGM): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


test_largeinput_1000_runs:
	for i in `seq 1 1000`; do ./$(PRGM) test/8kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test/16kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test/32kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test/64kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test/128kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test/256kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test/512kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test/1mb_input.txt $$i; done;

test_smallinput_1000_runs:
	for i in `seq 1 1000`; do ./$(PRGM) test5 $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test8 $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) test16 $$i; done;

single_1000_runs:
	for i in `seq 1 1000`; do ./$(PRGM) single $$i; done;

run: $(SRCS)
	./$(PRGM) test/8kb_input.txt
	./$(PRGM) test/16kb_input.txt
	./$(PRGM) test/32kb_input.txt
	./$(PRGM) test/64kb_input.txt
	./$(PRGM) test/128kb_input.txt
	./$(PRGM) test/256kb_input.txt
	./$(PRGM) test/512kb_input.txt
	./$(PRGM) test/1mb_input.txt

test_smallinput:
	./$(PRGM) test5
	./$(PRGM) test8
	./$(PRGM) test16

single:
	./$(PRGM) single

size_1:
	./$(PRGM) test5

size_2:
	./$(PRGM) test8

size_4:
	./$(PRGM) test16

size_8:
	./$(PRGM) test/8kb_input.txt

size_16:
	./$(PRGM) test/16kb_input.txt

size_32:
	./$(PRGM) test/32kb_input.txt

size_64:
	./$(PRGM) test/64kb_input.txt

size_128:
	./$(PRGM) test/128kb_input.txt

size_256:
	./$(PRGM) test/256kb_input.txt

size_512:
	./$(PRGM) test/512kb_input.txt

size_1mb:
	./$(PRGM) test/1mb_input.txt

clean:
	rm -rf $(OBJS) $(DEPS) $(PRGM)

-include $(DEPS)
