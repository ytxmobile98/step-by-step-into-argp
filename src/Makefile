BUILD_FILES = $(EXECS) *.a *.o

# executable files
EXECS = $(STEPS) morse-tool
STEPS := $(foreach step,$(shell ls step*.c),$(basename $(step)))

# static libraries
STEP_11_LIBS_FROM_SRC := dotdash
MORSE_TOOL_LIBS_FROM_SRC := dotdash morse readline-argp

.PHONY: all
all: $(EXECS)

step11: step11.c $(STEP_11_LIBS_FROM_SRC:%=lib%.a)
	$(CC) "$<" -L./ $(STEP_11_LIBS_FROM_SRC:%=-l%) -o "$@"

morse-tool: morse-tool.c $(MORSE_TOOL_LIBS_FROM_SRC:%=lib%.a)
	$(CC) "$<" -L./ $(MORSE_TOOL_LIBS_FROM_SRC:%=-l%) -lreadline -o "$@"

lib%.a: %.o
	ar cru "$@" "$^"
	ranlib "$@"

%.o: %.c %.h
	$(CC) -c -o "$@" "$<"

.PHONY: clean
clean:
	-rm -f $(BUILD_FILES)