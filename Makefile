CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

grades: main.cpp Person.cpp Person.h
	$(CXX) $(CXXFLAGS) main.cpp Person.cpp -o grades

clean:
	rm -f grades

.PHONY: clean
