CPP = clang++
CPPFLAGS = -std=c++23 -ggdb -Wall -Wextra -O0

EXENAME = main
EXEDIR = bin
EXE = $(EXEDIR)/$(EXENAME)

$(EXE): src/main.cpp
	@mkdir -p $(EXEDIR)
	$(CPP) $(CPPFLAGS) $< -o $@

run: $(EXE)
	./$(EXE)

