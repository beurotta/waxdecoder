# all files (including SDK) should be in same directory

CPP=g++
CXXFLAGS=-W -Wall
LDFLAGS=-mdll -lgdi32 -lgdiplus -static-libgcc -static-libstdc++
EXEC=WaxDecoder
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

all: $(EXEC)

WaxDecoder: $(OBJ)
	@$(CPP) -o WaxDecoder.usr $^ $(LDFLAGS)

%.o: %.c
	@$(CPP) -o $@ -c $< $(CXXFLAGS)

clean:
	@rm *.o

mrproper: clean
	@rm *.usr
