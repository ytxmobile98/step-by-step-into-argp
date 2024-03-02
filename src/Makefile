STEPS := $(foreach step,$(shell ls step*.c),$(basename $(step)))

DOTDASH := dotdash
DOTDASH_C := $(DOTDASH).c
DOTDASH_H := $(DOTDASH).h
DOTDASH_O := $(DOTDASH).o
LIBDOTDASH_A := lib$(DOTDASH).a

all: $(STEPS)

step11: step11.c $(LIBDOTDASH_A)
	$(CC) $< -L./ -l$(DOTDASH) -o $@

$(LIBDOTDASH_A): $(DOTDASH_O)
	ar cru $@ $^
	ranlib $@

$(DOTDASH_O): $(DOTDASH_C) $(DOTDASH_H)
	$(CC) -c -o $@ $<

clean:
	rm -f $(STEPS) $(LIBDOTDASH_A) $(DOTDASH_O)