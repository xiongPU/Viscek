Viscek = Viscek.o
Simu = ViscekSimulate.o
objects = $(Viscek) $(Simu)
deps = Viscek.hpp
RM = rm
CXX= g++ -std=c++11
CXXFLAGS = -c -Wall

all: ViscekSimulate $(objects)

ViscekSimulate: $(objects)
	$(CXX) -o $@ $^

%.o: %.cpp $(deps)
	$(CXX) ${CXXFLAGS} -c -o $@ $<


#.PHONY: clean
clean:
	rm ViscekSimulate $(objects)

#-include .depend
