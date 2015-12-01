APPS = assignment3

#Change to your own student number
STUDENT_NUMBER = 123456789

OBJECTS = parserClasses.o assignment3.o
HEADERS = parserClasses.h

CXXFLAGS += -g -Wall -std=c++11

all: $(APPS)

$(OBJECTS): $(HEADERS)
$(APPS): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.o $(APPS)

tar:
	tar -cf $(STUDENT_NUMBER).tar.gz Makefile *.cpp *.h


