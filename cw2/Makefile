
CFLAGS = -pedantic -Wall -std=c99
LFLAGS = -pedantic -Wall -std=c99
CO     = gcc
LD     = $(CO)

NAME1  = cw2
NAME2  = wypisz

OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o

EXEC1  = $(NAME1).x
EXEC2  = $(NAME2).x


.PHONY: help
help:
	@echo "make all         Kompiluje wszystkie programy"
	@echo "make $(NAME1)        Kompiluje program $(NAME1)"
	@echo "make run-$(NAME1)    Uruchamia program $(NAME1)"
	@echo "make clean       Usuwa pliki *.o oraz *.x"
	

.PHONY: all
all: $(EXEC1) $(EXEC2) 
$(EXEC1): $(OBJS1)
	$(LD) $(LFLAGS) -o $@ $^

$(EXEC2): $(OBJS2)
	$(LD) $(LFLAGS) -o $@ $^


%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<


.PHONY: $(NAME1)
$(NAME1): $(EXEC1)	


.PHONY: run-$(NAME1) 
run-$(NAME1): $(EXEC2) $(EXEC1)
	./$(EXEC1)

.PHONY: clean
clean:
	rm *.o *.x


