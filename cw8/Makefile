.SUFFIXES: .c .cpp .o .x .h

DIR = $(notdir $(CURDIR))

SYSNAME = $(shell uname -s)

CFLAGS = -Wall -std=gnu99 -pedantic -fPIC
LFLAGS = -Wall -std=gnu99 -pedantic
CO     = gcc
LD     = $(CO)

NAME1  = klient
NAME2  = serwer

LIBS1  = msg_q_fun

OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o

LIB_OBJS1  = $(LIBS1).o

HEADS1 = $(LIBS1).h

EXEC1s  = $(NAME1)s.x
EXEC2s  = $(NAME2)s.x

EXEC1d1  = $(NAME1)d1.x
EXEC2d1  = $(NAME2)d1.x

EXEC1d2  = $(NAME1)d2.x
EXEC2d2  = $(NAME2)d2.x

LIB_NAME = MojeFunkcje
LIB_DIR = ./lib
LIB_STAT = lib$(LIB_NAME).a

LIB_OBJS = $(LIB_OBJS1)

AR = ar

ARFLAGS = rsv

%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<

%.o: %.c
	$(CO) $(CFLAGS) -c $<
	
.PHONY: help
help:
	@echo "make all        Kompiluje wszystkie programy wraz z bibliotekami"
	@echo "make run-s    Uruchamia $(NAME1) i $(NAME2)"
	@echo "make clean            Usuwa pliki powstale przy kompilacji"

.PHONY: all
all: $(EXEC1s) $(EXEC2s) $(EXEC3s) $(EXEC1d1) $(EXEC2d1) $(EXEC3d1) $(EXEC1d2) $(EXEC2d2) $(EXEC3d2)


$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)


$(EXEC1s): $(OBJS1) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)

$(EXEC2s): $(OBJS2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)


$(OBJS1): $(HEADS1)
$(OBJS2): $(HEADS1)


.PHONY: run 
run: $(EXEC1s) $(EXEC2s)
	xterm -hold -title serwer -bg green -e ./$(EXEC2s) &
	xterm -hold -title konsument1 -bg orange -e ./$(EXEC1s) &
	xterm -hold -title konsument2 -bg red -e ./$(EXEC1s)

.PHONY: clean
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}


