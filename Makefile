##############
## C
##############
CC:= gcc
CFLAGS:= -c -Wall


LIBDIR := lib
SRCDIR := src
BUILDDIR := build

SOURCES:= $(wildcard ${SRCDIR}/*.c)
OBJECTS:= $(addprefix ${BUILDDIR}/,$(notdir $(SOURCES:.c=.o)))


SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LDFLAGS := $(shell pkg-config --libs sdl2)
LIBS := -I${LIBDIR}
LDLIBS:= -lSDL2
EXECUTABLE:= main

# Instantly runs the game after linking
run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Linking..."
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^
	@echo "Linking for target $(EXECUTABLE) succeeded!"

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(LIBDIR) -o $@ -c $<

.PHONY: clean
clean:
	-rm $(OBJECTS)
