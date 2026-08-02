CXX := g++
CXXFLAGS := -O2 -std=c++17 -I../compiler/src/parser
LDFLAGS :=

PARSER_CLI_SRC := server/src/parser_cli.cpp ../compiler/src/parser/parser.cpp
PARSER_CLI_EXE := server/parser_cli

# Default target
all: $(PARSER_CLI_EXE) ts

# Build parser_cli
$(PARSER_CLI_EXE): $(PARSER_CLI_SRC) ../compiler/src/parser/parser.h
	$(CXX) $(CXXFLAGS) -o server/out/parser_cli $(PARSER_CLI_SRC) $(LDFLAGS)

# Run TypeScript compilation
ts:
	npm run compile

# Clean
clean:
	rm -f $(PARSER_CLI_EXE)
