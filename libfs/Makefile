CC := cc -Wall -Wextra -Werror
OBJ = fs.o disk.o

# Target library
lib := libfs.a

all: $(lib)

$(lib): $(OBJ)
	$(CC) -o $(lib) $^

%.o: %.c
	$(CC) -c $<

run:
	make all; ./$(lib)

clean:
	rm -f $(OBJ) $(lib)
