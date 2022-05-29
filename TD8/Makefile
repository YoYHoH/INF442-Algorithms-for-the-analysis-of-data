CXX = g++
CXXFLAGS = ${INCLUDES} -std=c++11 -O2

.PHONY: all clean

all: grader

# TD-specific part

SOURCES_SPECIFIC =
OBJECTS_SPECIFIC =

%: %.o
	$(CXX)  $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# Common part
SOURCES_COMMON = gradinglib/gradinglib.cpp grading/grading.cpp main.cpp
OBJECTS_COMMON = gradinglib.o grading.o main.o

grader: $(OBJECTS_COMMON) $(OBJECTS_SPECIFIC) libsvm-3.23/svm-train.c libsvm-3.23/svm-predict.c libsvm-3.23/svm.cpp libsvm-3.23/svm.h
	cd libsvm-3.23 && $(MAKE) -f Makefile all && cd ..
	$(CXX) $(CXXFLAGS) -o grader $(OBJECTS_COMMON) $(OBJECTS_SPECIFIC)

gradinglib.o: gradinglib/gradinglib.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(CXXFLAGS) -o gradinglib.o gradinglib/gradinglib.cpp

grading.o: grading/grading.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(CXXFLAGS) -o grading.o grading/grading.cpp 

main.o: main.cpp grading/grading.hpp
	$(CXX) -c $(CXXFLAGS) -o main.o main.cpp

include libsvm-3.23/Makefile

clean:
	cd libsvm-3.23 && $(MAKE) -f Makefile $@ && cd ..
	rm -f grader *.o
	rm -f *~ output.txt

