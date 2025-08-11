CXX := g++
CXXFLAGS := -O2 -std=c++17

GENERATOR_SRC := parse_generator.cpp
GENERATOR_EXE := parse_generator.exe

# Build generator only if sources change
$(GENERATOR_EXE): $(GENERATOR_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

generator: $(GENERATOR_EXE)

# Run the generator for jank
jank: $(GENERATOR_EXE) jank/jank.ebnf
	cd jank && ../$(GENERATOR_EXE) jank.ebnf
	mv jank/parser.cpp ../compiler/parser/
	mv jank/parser.h   ../compiler/parser/

# Optional cleanup
clean:
	rm -f $(GENERATOR_EXE)