# Adjust the files accordingly if you are not using the machines of polytechnique
EIGEN=/usr/local/eigen-3.4.0
ANN_INCLUDE=/usr/local/ann-1.1.2/include
ANN_LIB=/usr/local/ann-1.1.2/lib
# EIGEN=/Users/elias/ET/et-data/teaching/X2020/eigen-3.4.0/
# ANN_HOME=/Users/elias/ET/et-data/teaching/X2020/ann_1.1.2
# ANN_INCLUDE=$(ANN_HOME)/include
# ANN_LIB=$(ANN_HOME)/lib

CXX = g++

INCLUDES=-I$(EIGEN) -I$(ANN_INCLUDE)
CXXFLAGS = ${INCLUDES} -std=c++11 -O2

LDFLAGS_ANN=-L$(ANN_LIB) -lANN

.PHONY: all clean

all: grader

# TD-specific part

SOURCES_SPECIFIC = Dataset.cpp KnnRegression.cpp LinearRegression.cpp Regression.cpp
OBJECTS_SPECIFIC = Dataset.o KnnRegression.o LinearRegression.o Regression.o

%: %.o
	$(CXX)  $(CXXFLAGS) $^ $(LDFLAGS) -o $@

Dataset.o: Dataset.cpp Dataset.hpp
KnnRegression.o: KnnRegression.cpp KnnRegression.hpp
LinearRegression.o: LinearRegression.cpp LinearRegression.hpp
Regression.o: Regression.cpp


# Common part

SOURCES_COMMON = gradinglib/gradinglib.cpp grading/grading.cpp main.cpp
OBJECTS_COMMON = gradinglib.o grading.o main.o

grader: $(OBJECTS_COMMON) $(OBJECTS_SPECIFIC) 
	$(CXX) $(CXXFLAGS) -o grader $(OBJECTS_COMMON) $(OBJECTS_SPECIFIC)  -L$(ANN_LIB) -lANN

gradinglib.o: gradinglib/gradinglib.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(CXXFLAGS) -o gradinglib.o gradinglib/gradinglib.cpp

grading.o: grading/grading.cpp gradinglib/gradinglib.hpp
	$(CXX) -c $(CXXFLAGS) -o grading.o grading/grading.cpp 

main.o: main.cpp grading/grading.hpp
	$(CXX) -c $(CXXFLAGS) -o main.o main.cpp


test_k_knn: test_k_knn.o KnnRegression.o LinearRegression.o Regression.o Dataset.o
	$(CXX)  -o test_k_knn test_k_knn.o $(OBJECTS_SPECIFIC)  -L$(ANN_LIB) -lANN

clean:
	rm -f grader *.o test_k_knn
	rm -f *~ output.txt