include		../_conf.txt

OBJS		=		\
		be_misc.o	\
		be_scan.o	\

OBJS_QT4	= $(OBJS)	\
		moc_qt4.o	\
		qt4_main.o	\
		qt4_tab.o	\

OBJS_QT5	= $(OBJS_QT4)


.PHONY:		all install uninstall clean

all:		$(BIN_NAME)

mtraft-qt4:	$(OBJS_QT4)
		$(CXX) $(OBJS_QT4) -o $@ $(LDFLAGS)

mtraft-qt5:	$(OBJS_QT5)
		$(CXX) $(OBJS_QT5) -o $@ $(LDFLAGS)

$(OBJS_QT4) $(OBJS_QT5):	*.h *.xpm

# .c.o Implicit rule
# .cpp.o Implicit rule

moc_qt4.cpp:	qt4.h
		$(MOC) qt4.h -o $@

install:
		install -d $(DESTDIR)$(BIN_INSTALL)
		install $(BIN_NAME) $(DESTDIR)$(BIN_INSTALL)

uninstall:
		rm $(DESTDIR)$(BIN_INSTALL)/$(BIN_NAME)

clean:
		rm -f moc_*.cpp *.o $(BIN_NAME)
