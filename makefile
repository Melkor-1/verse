CFLAGS 	:= -std=c17
CFLAGS 	+= -no-pie
CFLAGS 	+= -g3
CFLAGS 	+= -ggdb
CFLAGS 	+= -Wall
CFLAGS 	+= -Wextra
CFLAGS 	+= -Warray-bounds
CFLAGS 	+= -Wconversion
CFLAGS 	+= -Wmissing-braces
CFLAGS 	+= -Wno-parentheses
CFLAGS 	+= -Wno-format-truncation
CFLAGS 	+= -Wpedantic
CFLAGS 	+= -Wstrict-prototypes
CFLAGS 	+= -Wwrite-strings
CFLAGS 	+= -Winline
CFLAGS 	+= -s
CFLAGS 	+= -O2
CFLAGS 	+= -D_FORTIFY_SOURCE=2

BINDIR	:= bin
BIN 	:= $(BINDIR)/verse
SRCS 	:= $(wildcard src/*.c)
OBJS 	:= $(patsubst src/%.c, obj/%.o, $(SRCS))
LDLIBS 	:= -ljansson -lcurl

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLIBS)

clean:
	$(RM) -rf $(OBJS) 

fclean:
	$(RM) -rf $(BIN) 

.PHONY: clean all fclean
.DELETE_ON_ERROR:
