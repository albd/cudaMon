cudaMon : cudaMon.cpp test.cpp
	g++ -o test.out test.cpp
	g++ -pthread -o cudaMon cudaMon.cpp
