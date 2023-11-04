CC:=gcc
CFLAGS:= -Wall -fprofile-arcs -ftest-coverage
PROJECT_ROOT := $(shell pwd)
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
OBJECTS_FOR_TEST:= $(filter-out ${BUILDDIR}/${EXECUTABLE}.o, ${OBJECTS})
TEST_DEPFILES:= $(patsubst ${TEST_BUILD_DIR}/%.o, ${TEST_DEP_DIR}/%.d, ${TEST_OBJECTS})
TEST_CASES := $(patsubst ${TEST_BUILD_DIR}/%.o, ${TEST_BINARY_DIR}/%, ${TEST_OBJECTS})

TEST_LIBS := -I${TEST_LIB_DIR} -I${LIBDIR}
TEST_DEPFLAGS = -MT $@ -MMD -MP -MF $(TEST_DEP_DIR)/$*.d
COVERAGE_DATA := $(BUILDDIR)/coverage_data
COVERAGE_REPORT_DIR:= $(COVERAGE_DATA)/coverage_report

all : $(EXECUTABLE) test

run : $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	@echo "Linking..."
	$(CC) $(CFLAGS) $(SDL_LDFLAGS) $(LDLIBS) -o $@ $^
	@echo "Linking for target $(EXECUTABLE) succeeded!"

$(BUILDDIR)/%.o : $(SRCDIR)/%.c $(DEPDIR)/%.d | $(DEPDIR) $(BUILDDIR)
	@echo "Building $*"
	$(CC) $(CFLAGS) $(DEPFLAGS) $(SDL_CFLAGS) $(LIBS) -c -o $@ $<


$(DEPDIR): ; @mkdir -p $@

$(BUILDDIR): ; @mkdir -p $@

$(DEPFILES):

include $(wildcard $(DEPFILES))


test : clean_coverage_data $(TEST_CASES)
	@echo "Running all test cases"
	@for test_case in $(TEST_CASES); do \
		$$test_case; \
	done
	@echo "All test cases passed"


${TEST_BINARY_DIR}/% : $(TEST_BUILD_DIR)/%.o $(OBJECTS_FOR_TEST) | $(TEST_BINARY_DIR)
	${CC} ${LDLIBS} ${CFLAGS} ${SDL_LDFLAGS} -o $@ $^

${TEST_BUILD_DIR}/%.o : ${TEST_SRC_DIR}/%.c $(TEST_DEP_DIR)/%.d | $(TEST_DEP_DIR) $(TEST_BUILD_DIR)
	@echo "Building $*"
	${CC} ${CFLAGS} ${TEST_DEPFLAGS} ${TEST_LIBS} ${SDL_CFLAGS}  -c -o $@ $<

run_tests: test | $(COVERAGE_DATA) 
	@GCOV_PREFIX_STRIP=1 GCOV_PREFIX=${BUILDDIR} gcov $(OBJECTS_FOR_TEST)
	@find . -type d -name $(BUILDDIR) -prune -o -type f -name "*.gcov" -exec mv {} $(COVERAGE_DATA) \;

coverage: run_tests | $(COVERAGE_REPORT_DIR)
	@echo "Generating code coverage report..."
	gcovr -r . --html --html-details -o $(COVERAGE_REPORT_DIR)/coverage.html
	@echo "Code coverage report generated"

coverage_report: coverage
	open $(COVERAGE_REPORT_DIR)/coverage.html

clean_coverage_data: $(BUILDDIR)
	@rm -rf $(COVERAGE_DATA)
	@find $(BUILDDIR) -type f -name "*.gcda" -exec rm {} \;

$(COVERAGE_DATA): ; @mkdir -p $@

$(COVERAGE_REPORT_DIR): ; @mkdir -p $@

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

.PHONY: all test run_tests coverage clean clean_coverage_data
clean:
	rm -rf $(BUILDDIR) $(EXECUTABLE)
