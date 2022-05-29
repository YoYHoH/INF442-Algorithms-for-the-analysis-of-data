CXX = g++
CFLAGS = -std=c++11 -O2
INCLUDES = -Igradinglib -Igrading -Icloud -Ipoint -Ikernel -Ikernel/radial -Ikernel/radial/flat -Ikernel/radial/gaussian -Ikernel/knn

# TD-specific part

OBJECTS_SPECIFIC = point.o cloud.o kernel.o radial.o flat.o gaussian.o knn.o
SOURCES_SPECIFIC = point/point.cpp cloud/cloud.cpp kernel/kernel.cpp kernel/radial/radial.cpp kernel/radial/flat/flat.cpp kernel/radial/gaussian/gaussian.cpp kernel/knn/knn.cpp

point.o: point/point.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) point/point.cpp

cloud.o: cloud/cloud.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) cloud/cloud.cpp

kernel.o: kernel/kernel.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) kernel/kernel.cpp

radial.o: kernel/radial/radial.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) kernel/radial/radial.cpp

flat.o: kernel/radial/flat/flat.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) kernel/radial/flat/flat.cpp

gaussian.o: kernel/radial/gaussian/gaussian.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) kernel/radial/gaussian/gaussian.cpp

knn.o: kernel/knn/knn.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) kernel/knn/knn.cpp

# Common part

SOURCES_COMMON = gradinglib/gradinglib.cpp grading/grading.cpp main.cpp
OBJECTS_COMMON = gradinglib.o grading.o main.o

grader: $(OBJECTS_COMMON) $(OBJECTS_SPECIFIC) 
	$(CXX) $(INCLUDES) $(CFLAGS) -o grader $(OBJECTS_COMMON) $(OBJECTS_SPECIFIC)

gradinglib.o: gradinglib/gradinglib.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) -o gradinglib.o gradinglib/gradinglib.cpp

grading.o: grading/grading.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) -o grading.o grading/grading.cpp

main.o: main.cpp grading/grading.hpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) -o main.o main.cpp

all: grader

.PHONY: all clean

clean:
	rm -f *.o
	rm -f grader

