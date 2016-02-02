all:
	g++ -std=c++11 -o Meep src/Meep.cpp  src/main.cpp

clean:
	rm Meep
