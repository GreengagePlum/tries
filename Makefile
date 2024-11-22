# Inspired partly by Unity unit test framework makefile on 29 May 2024:
# https://www.throwtheswitch.org/s/makefile
# https://www.throwtheswitch.org/build/make

##### Common
CC = gcc

##### Paths
SPATH = src/
HPATH = include/
BPATH = build/
OPATH = $(BPATH)obj/
DPATH = $(BPATH)dep/
RPATH = $(BPATH)results/
TPATH = test/
UPATH = unity/src/
JPATH = cJSON/

BUILD_PATHS = $(BPATH) $(DPATH) $(OPATH) $(RPATH)

##### Executables
EXEC = tries

##### Options
CPPFLAGS = -DNDEBUG
CFLAGS = -Wall -Wextra -Werror -std=iso9899:2018 -pedantic -march=native -O3 -I$(HPATH) -I$(JPATH) -I$(UPATH)
LDFLAGS =
DEPFLAGS = -MT $@ -MMD -MP -MF $(DPATH)$*.Td

##### Files
SRC = $(wildcard $(SPATH)*.c)
HDR = $(wildcard $(HPATH)*.h)
OBJ = $(addprefix $(OPATH), $(patsubst %.c,%.o, $(notdir $(SRC))))
DEP = $(addprefix $(DPATH), $(patsubst %.c,%.d, $(notdir $(SRC)))) $(addprefix $(DPATH), $(patsubst %.c,%.d, $(notdir $(SRCT))))
SRCT = $(wildcard $(TPATH)*.c)

##### Tests
RESULTS = $(patsubst $(TPATH)Test%.c,$(RPATH)Test%.txt,$(SRCT) )

PASSED = `grep -s PASS $(RPATH)*.txt`
FAIL = `grep -s FAIL $(RPATH)*.txt`
IGNORE = `grep -s IGNORE $(RPATH)*.txt`

##### Documentation generation
DOCGEN = doxygen
DOXYFILE = doc/Doxyfile
DOCPATH = doc/public/

##### Various
### Protection against interruptions during the compilation
POSTCOMPILE = mv -f $(DPATH)$*.Td $(DPATH)$*.d && touch $@

##### Build rules
.PHONY: all test doc clean cleandoc cleanall

all: $(EXEC) test

$(EXEC): $(OBJ) $(OPATH)cJSON.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(OPATH)%.o:: $(SPATH)%.c $(DPATH)%.d | $(OPATH) $(DPATH)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -o $@ $<
	@$(POSTCOMPILE)

$(OPATH)%.o:: $(TPATH)%.c $(DPATH)%.d | $(OPATH) $(DPATH)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -o $@ $<
	@$(POSTCOMPILE)

$(OPATH)%.o:: $(UPATH)%.c $(UPATH)%.h | $(OPATH)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OPATH)cJSON.o:: $(JPATH)cJSON.c $(JPATH)cJSON.h | $(OPATH)
	$(CC) -c $(CFLAGS) -o $@ $<

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

$(RPATH)%.txt: $(BPATH)%
	-./$< > $@ 2>&1

$(BPATH)Test%: $(OPATH)Test%.o $(OPATH)%.o $(OPATH)unity.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(OPATH):
	mkdir -p $@

$(DPATH):
	mkdir -p $@

$(RPATH):
	mkdir -p $@

$(BPATH):
	mkdir -p $@

$(DEP):

-include $(DEP)

clean:
	rm -f $(EXEC) $(OBJ) $(DEP) $(RESULTS)

cleandoc:
	rm -rf $(DOCPATH)

cleanall: clean
cleanall: cleandoc
cleanall:
	rm -rf $(BUILD_PATHS)

doc: $(DOCPATH)

$(DOCPATH): $(SRC) $(HDR) README.md $(DOXYFILE)
	DOXY_OUT=$@ $(DOCGEN) $(DOXYFILE)

.PRECIOUS: $(BPATH)Test%
.PRECIOUS: $(DPATH)%.d
.PRECIOUS: $(OPATH)%.o
.PRECIOUS: $(RPATH)%.txt
