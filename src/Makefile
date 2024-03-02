BUILD_FILES = $(EXECS) *.a *.o
EXECS = $(STEPS)

LIB_NAMES := dotdash
STEPS := $(foreach step,$(shell ls step*.c),$(basename $(step)))

.PHONY: all
all: $(EXECS)

step11: step11.c libdotdash.a
	$(CC) "$<" -L./ -ldotdash -o "$@"

lib%.a: %.o
	ar cru "$@" "$^"
	ranlib "$@"

%.o: %.c %.h
	$(CC) -c -o "$@" "$<"

.PHONY: clean
clean:
	-rm -f $(BUILD_FILES)