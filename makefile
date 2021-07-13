CXXFLAGS = -g -std=c++17 -march=native -flto -mavx2 -O3 -funroll-loops 
LDLIBS =


PRGM  = main
SRCS := $(wildcard main.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean


all: $(PRGM)

$(PRGM): $(OBJS)
	$(CXX) $(OBJS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


test_largeinput:
	for i in `seq 1 1000`; do ./$(PRGM) 16kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) 32kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) 64kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) 128kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) 256kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) 512kb_input.txt $$i; done;
	for i in `seq 1 1000`; do ./$(PRGM) 1mb_input.txt $$i; done;

run: $(SRCS)
	./$(PRGM) 8kb_input.txt
	./$(PRGM) 16kb_input.txt
	./$(PRGM) 32kb_input.txt
	./$(PRGM) 64kb_input.txt
	./$(PRGM) 128kb_input.txt
	./$(PRGM) 256kb_input.txt
	./$(PRGM) 512kb_input.txt
	./$(PRGM) 1mb_input.txt

test_smallinput:
	./$(PRGM) test5
	./$(PRGM) test8
	./$(PRGM) test16

clean:
	rm -rf $(OBJS) $(DEPS) $(PRGM)

-include $(DEPS)