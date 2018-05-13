cudaMon : cudaMon.cpp test.cpp
	g++ --std=c++11 -o test.out test.cpp
	g++ --std=c++11 -pthread -o cudaMon cudaMon.cpp
