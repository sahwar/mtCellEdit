include		../_conf.txt


.PHONY:		all valg time build install uninstall clean


BIN_ALL		= mkdata
TMP_DIR		= d0 d1 d2 d3 output tmp
TMP_FILES	= valg_log.txt log.txt $(BIN_ALL) *.o
PREPARE		= mkdir -p $(TMP_DIR)
MKDATA_OBJ	=			\
		mkdata.o		\
		mkdata_flac.o		\
		mkdata_png.o		\


all:		$(BIN_ALL)
		$(PREPARE)
		$(SUITE)

valg:		$(BIN_ALL)
		$(PREPARE)
		VALG=Y $(SUITE)

time:		$(BIN_ALL)
		$(PREPARE)
		VALG=T $(SUITE)

build:		$(BIN_ALL)

$(MKDATA_OBJ):	*.h

mkdata:		$(MKDATA_OBJ)
		$(CXX) $(MKDATA_OBJ) -o $@ $(LDFLAGS)

# .c.o Implicit rule
# .cpp.o Implicit rule

install:

uninstall:

clean:
		rm -rf $(TMP_DIR) $(TMP_FILES)

