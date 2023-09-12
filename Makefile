CC:=gcc
CFLAGS:= -Wall

LIBDIR := lib
SRCDIR := src
BUILDDIR := build
DEPDIR := $(BUILDDIR)/deps

SOURCES:= $(wildcard ${SRCDIR}/*.c)
OBJECTS:= $(patsubst ${SRCDIR}/%.c, ${BUILDDIR}/%.o, ${SOURCES})
DEPFILES:= $(patsubst ${BUILDDIR}/%.o, ${DEPDIR}/%.d, ${OBJECTS})
EXECUTABLE:=main

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
LIBS:= -I${LIBDIR}
SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LDFLAGS := $(shell pkg-config --libs sdl2)
LDLIBS:= -lSDL2

TEST_DIR := tests
TEST_SRC_DIR := ${TEST_DIR}/src
TEST_LIB_DIR := ${TEST_DIR}/lib
TEST_BUILD_DIR := ${BUILDDIR}/tests
TEST_BINARY_DIR := ${TEST_BUILD_DIR}/bin
TEST_DEP_DIR := ${TEST_BUILD_DIR}/deps

TEST_SOURCES:= $(wildcard ${TEST_SRC_DIR}/*.c)
TEST_OBJECTS:= $(patsubst ${TEST_SRC_DIR}/%.c, ${TEST_BUILD_DIR}/%.o, ${TEST_SOURCES})
TEST_DEPFILES:= $(patsubst ${TEST_BUILD_DIR}/%.o, ${TEST_DEP_DIR}/%.d, ${TEST_OBJECTS})
TEST_CASES := $(patsubst ${TEST_BUILD_DIR}/%.o, ${TEST_BINARY_DIR}/%, ${TEST_OBJECTS})

TEST_LIBS := -I${TEST_LIB_DIR} -I${LIBDIR}

all : $(EXECUTABLE) tests

$(EXECUTABLE) : $(OBJECTS)
	@echo "Linking..."
	$(CC) $(SDL_LDFLAGS) $(LDLIBS) -o $@ $^
	@echo "Linking for target $(EXECUTABLE) succeeded!"

$(BUILDDIR)/%.o : $(SRCDIR)/%.c $(DEPDIR)/%.d | $(DEPDIR) $(BUILDDIR)
	@echo "Building $*"
	$(CC) $(CFLAGS) $(DEPFLAGS) $(SDL_CFLAGS) $(LIBS) -c -o $@ $<


$(DEPDIR): ; @mkdir -p $@

$(BUILDDIR): ; @mkdir -p $@

$(DEPFILES):

include $(wildcard $(DEPFILES))

tests : ${TEST_CASES}

${TEST_BINARY_DIR}/% : $(TEST_BUILD_DIR)/%.o $(OBJECTS) | $(TEST_BINARY_DIR)
	${CC} ${LDLIBS} ${SDL_LDFLAGS} -o $@ $^

${TEST_BUILD_DIR}/%.o : ${TEST_SRC_DIR}/%.c $(TEST_DEP_DIR)/%.d | $(TEST_DEP_DIR) $(TEST_BUILD_DIR)
	@echo "Building $*"
	${CC} ${CFLAGS} ${DEPFLAGS} ${TEST_LIBS} ${SDL_CFLAGS}  -c -o $@ $<

$(TEST_BUILD_DIR): ; @mkdir -p $@

$(TEST_DEP_DIR): ; @mkdir -p $@

$(TEST_BINARY_DIR) : ; @mkdir -p $@

$(TEST_DEPFILES):

include $(wildcard $(TEST_DEPFILES))

print-sources:
	@echo "Source files: $(SOURCES)"

# Print the list of object files
print-objects:
	@echo "Object files: $(OBJECTS)"

print-depfiles:
	@echo "Dependency files: $(DEPFILES)"

print-testfiles:
	@echo "test sources $(TEST_SOURCES)"

print-testobjs:
	@echo "test objects $(TEST_OBJECTS)"

print-testdepfiles:
	@echo "test depfiles $(TEST_DEPFILES)"

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(EXECUTABLE)
