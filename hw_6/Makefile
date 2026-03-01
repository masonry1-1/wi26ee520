CXX = g++
CXXFLAGS = -std=c++17 -O2
LIBS = -lcurl

all:
	$(CXX) $(CXXFLAGS) main.cc -o main
	$(CXX) $(CXXFLAGS) unit_tests.cc -o unit_tests
	$(CXX) $(CXXFLAGS) llm_agent.cpp -o llm_agent $(LIBS)

clean:
	rm -f main unit_tests llm_agent
