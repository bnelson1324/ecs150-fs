CC := cc -Wall -Wextra -Werror

AR := ar
ARFLAGS := rcs

OBJ = fs.o disk.o

# Target library
lib := libfs.a

all: $(lib)

$(lib): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) -c $<

run:
	make all; ./$(lib)

clean:
	rm -f $(OBJ) $(lib)
