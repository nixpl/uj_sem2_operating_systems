.SUFFIXES: .c .cpp .o .x .h

DIR = $(notdir $(CURDIR))

SYSNAME = $(shell uname -s)

CFLAGS = -Wall -std=gnu99 -pedantic -fPIC
LFLAGS = -Wall -std=gnu99 -pedantic
CO     = gcc
LD     = $(CO)

INPUT = surowiec.txt
OUTPUT = towar.txt

NAME1  = cw7
NAME2  = producent
NAME3  = konsument

LIBS1  = semaphore_fun
LIBS2  = shared_memory_fun

OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
OBJS3  = $(NAME3).o
LIB_OBJS1  = $(LIBS1).o
LIB_OBJS2  = $(LIBS2).o

HEADS1 = $(LIBS1).h
HEADS2 = $(LIBS2).h

EXEC1s  = $(NAME1)s.x
EXEC2s  = $(NAME2)s.x
EXEC3s  = $(NAME3)s.x

EXEC1d1  = $(NAME1)d1.x
EXEC2d1  = $(NAME2)d1.x
EXEC3d1  = $(NAME3)d1.x

EXEC1d2  = $(NAME1)d2.x
EXEC2d2  = $(NAME2)d2.x
EXEC3d2  = $(NAME3)d2.x

LIB_NAME = MojeFunkcje
LIB_DIR = ./lib
LIB_STAT = lib$(LIB_NAME).a
LIB_SHAR = lib$(LIB_NAME).so
LIB_OBJS = $(LIB_OBJS1) $(LIB_OBJS2)

AR = ar

ARFLAGS = rsv

%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<

%.o: %.c
	$(CO) $(CFLAGS) -c $<
	
.PHONY: help
help:
	@echo "make all        Kompiluje wszystkie programy wraz z bibliotekami"
	@echo "make run-$(NAME1)s    Uruchamia program $(NAME1) z biblioteka statyczna"
	@echo "make run-$(NAME1)d1    Uruchamia program $(NAME1) z biblioteka dzielona (sposob I)"
	@echo "make run-$(NAME1)d2    Uruchamia program $(NAME1) z biblioteka dzielona (sposob II)"
	@echo "make clean            Usuwa pliki powstale przy kompilacji"

.PHONY: all
all: $(EXEC1s) $(EXEC2s) $(EXEC3s) $(EXEC1d1) $(EXEC2d1) $(EXEC3d1) $(EXEC1d2) $(EXEC2d2) $(EXEC3d2)


$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)


$(LIB_SHAR): $(LIB_OBJS)
	$(LD) -shared -o $@ $(LFLAGS) $(LIB_OBJS)
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR) $(LIB_DIR)



$(EXEC1s): $(OBJS1) $(LIB_STAT)
	rm -f $(LIB_DIR)/$(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)

$(EXEC2s): $(OBJS2) $(LIB_STAT)
	rm -f $(LIB_DIR)/$(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)

$(EXEC3s): $(OBJS3) $(LIB_STAT)
	rm -f $(LIB_DIR)/$(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)



$(EXEC1d1): $(OBJS1) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -Wl,-R $(LIB_DIR)

$(EXEC2d1): $(OBJS2) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -Wl,-R $(LIB_DIR)

$(EXEC3d1): $(OBJS3) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -Wl,-R $(LIB_DIR)



$(EXEC1d2): $(OBJS1) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)

$(EXEC2d2): $(OBJS2) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)

$(EXEC3d2): $(OBJS3) $(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)

$(OBJS1): $(HEADS2) $(HEADS1)
$(OBJS2): $(HEADS2) $(HEADS1)
$(OBJS3): $(HEADS2) $(HEADS1)


.PHONY: run-$(NAME1)s 
run-$(NAME1)s: $(EXEC1s) $(EXEC2s) $(EXEC3s)
	./$(EXEC1s) $(EXEC2s) $(EXEC3s) $(INPUT) $(OUTPUT); diff -s $(INPUT) $(OUTPUT)
 
.PHONY: run-$(NAME1)d1
run-$(NAME1)d1: $(EXEC1d1) $(EXEC2d1) $(EXEC3d1)
	./$(EXEC1d1) $(EXEC2d1) $(EXEC3d1) $(INPUT) $(OUTPUT); diff -s $(INPUT) $(OUTPUT)

.PHONY: run-$(NAME1)d2
run-$(NAME1)d2: $(EXEC1d2) $(EXEC2d2) $(EXEC3d2)
	( export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}; \
	./$(EXEC1d2) $(EXEC2d2) $(EXEC3d2) $(INPUT) $(OUTPUT); diff -s $(INPUT) $(OUTPUT) )


.PHONY: clean
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}


