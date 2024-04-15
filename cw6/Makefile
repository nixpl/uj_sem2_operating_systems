
CFLAGS = -pedantic -Wall -std=c99
LFLAGS = -pedantic -Wall -std=c99
CO     = gcc
LD     = $(CO)

NUM_PROCESSES = 3
NUM_SECTIONS = 4

NAME1  = multipler
NAME2  = exclusion
LIB_NAME  = semaphore_fun

OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
LIB_OBJ  = $(LIB_NAME).o

EXEC1  = $(NAME1).x
EXEC2  = $(NAME2).x


.PHONY: help
help:
	@echo "make $(NAME1)        Kompiluje programy $(NAME1), $(NAME2) oraz biblioteke $(LIB_NAME)"
	@echo "make run-$(NAME1)    Uruchamia program $(NAME1)"
	@echo "make clean            Usuwa plik numer.txt oraz pliki *.o oraz *.x"


lib$(LIB_NAME).a: $(LIB_OBJ)
	ar rcs $@ $^

$(LIB_OBJ): $(LIB_NAME).c $(LIB_NAME).h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: $(NAME1)
$(NAME1): $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJS1) lib$(LIB_NAME).a 
	$(CC) $(CFLAGS) -o $@ $< -L. -l$(LIB_NAME)

$(EXEC2): $(OBJS2) lib$(LIB_NAME).a 
	$(CC) $(CFLAGS) -o $@ $< -L. -l$(LIB_NAME)



.PHONY: run-$(NAME1) 
run-$(NAME1): $(EXEC1) $(EXEC2)
	./$(EXEC1) $(EXEC2) $(NUM_PROCESSES) $(NUM_SECTIONS)

.PHONY: clean
clean:
	rm *.o *.x numer.txt


