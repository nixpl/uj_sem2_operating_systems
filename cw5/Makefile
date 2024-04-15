
CFLAGS = -pedantic -Wall -std=c99
LFLAGS = -pedantic -Wall -std=c99
CO     = gcc
LD     = $(CO)

FIFO   = fifo
DATA   = surowiec.txt
OUTPUT = towar.txt

NAME1  = cw5a
NAME2  = producent
NAME3  = konsument
NAME4  = cw5b

OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
OBJS3  = $(NAME3).o

EXEC1  = $(NAME1).x
EXEC2  = $(NAME2).x
EXEC3  = $(NAME3).x


.PHONY: help
help:
	@echo "make $(NAME1)        Kompiluje program $(NAME1).c oraz programy potomne"
	@echo "make $(NAME4)        Kompiluje programy $(NAME2).c oraz $(NAME3).c"
	@echo "make run-$(NAME1)    Uruchamia program $(NAME1).c oraz programy potomne"
	@echo "make run-$(NAME4)    Uruchamia programy $(NAME2).c oraz $(NAME3).c jednoczesnie"
	@echo "make clean       Usuwa plik $(OUTPUT) oraz pliki *.o oraz *.x"

.PHONY: $(NAME1)
$(NAME1): $(EXEC1) $(EXEC2) $(EXEC3) 
$(EXEC1): $(OBJS1)
	$(LD) $(LFLAGS) -o $@ $^

$(EXEC2): $(OBJS2)
	$(LD) $(LFLAGS) -o $@ $^

$(EXEC3): $(OBJS3)
	$(LD) $(LFLAGS) -o $@ $^

.PHONY: $(NAME4)
$(NAME4): $(EXEC2) $(EXEC3) 


.PHONY: run-$(NAME1) 
run-$(NAME1): $(EXEC1) $(EXEC2) $(EXEC3) 
	./$(EXEC1) $(FIFO) surowiec.txt towar.txt; diff -s $(DATA) $(OUTPUT)

.PHONY: run-$(NAME4)
run-$(NAME4): $(EXEC2) $(EXEC3)
	mkfifo -m 0666 $(FIFO)
	./$(EXEC2) $(DATA) $(FIFO) & ./$(EXEC3) $(OUTPUT) $(FIFO)	
	diff -s $(DATA) $(OUTPUT)
	rm $(FIFO)

.PHONY: clean
clean:
	rm *.o *.x $(OUTPUT)


