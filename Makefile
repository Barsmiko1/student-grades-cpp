CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

SOURCES = main.cpp Person.cpp Students.cpp FileIO.cpp Console.cpp SpeedTest.cpp
HEADERS = Person.h Students.h FileIO.h Console.h SpeedTest.h Errors.h Timer.h

grades: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o grades

clean:
	rm -f grades

.PHONY: clean
