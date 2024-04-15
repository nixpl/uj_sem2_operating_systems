
CFLAGS = -pedantic -Wall -std=c99
LFLAGS = -pedantic -Wall -std=c99
CO     = gcc
LD     = $(CO)

NAME1  = cw1a
NAME2  = cw1b
NAME3  = cw1c
NAME4  = cw1d
NAME5  = cw1e

OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
OBJS3  = $(NAME3).o
OBJS4  = $(NAME4).o
OBJS5  = $(NAME5).o

EXEC1  = $(NAME1).x
EXEC2  = $(NAME2).x
EXEC3  = $(NAME3).x
EXEC4  = $(NAME4).x
EXEC5  = $(NAME5).x


.PHONY: help
help:
	@echo "make all         Kompiluje wszystkie programy"
	@echo "make $(NAME1)        Kompiluje program $(NAME1)"
	@echo "make $(NAME2)        Kompiluje program $(NAME2)"
	@echo "make $(NAME3)        Kompiluje program $(NAME3)"
	@echo "make $(NAME4)        Kompiluje program $(NAME4)"
	@echo "make $(NAME5)        Kompiluje program $(NAME5)"
	@echo "make run-$(NAME1)    Uruchamia program $(NAME1)"
	@echo "make run-$(NAME2)    Uruchamia program $(NAME2)"
	@echo "make run-$(NAME3)    Uruchamia program $(NAME3)"
	@echo "make run-$(NAME4)    Uruchamia program $(NAME4)"
	@echo "make run-$(NAME5)    Uruchamia program $(NAME5)"
	@echo "make clean       Usuwa pliki *.o oraz *.x"
	

.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

$(EXEC1): $(OBJS1)
	$(LD) $(LFLAGS) -o $@ $^

$(EXEC2): $(OBJS2)
	$(LD) $(LFLAGS) -o $@ $^

$(EXEC3): $(OBJS3)
	$(LD) $(LFLAGS) -o $@ $^

$(EXEC4): $(OBJS4)
	$(LD) $(LFLAGS) -o $@ $^

$(EXEC5): $(OBJS5)
	$(LD) $(LFLAGS) -o $@ $^


%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<


.PHONY: $(NAME1) $(NAME2) $(NAME3) $(NAME4) $(NAME5)
$(NAME1): $(EXEC1)
	
$(NAME2): $(EXEC2)
	
$(NAME3): $(EXEC3)
	
$(NAME4): $(EXEC4)
	
$(NAME5): $(EXEC5)
	


.PHONY: run-$(NAME1) run-$(NAME2) run-$(NAME3) run-$(NAME4) run-$(NAME5)
run-$(NAME1): $(EXEC1)
	./$(EXEC1)

run-$(NAME2): $(EXEC2)
	./$(EXEC2)

run-$(NAME3): $(EXEC3)
	./$(EXEC3)

run-$(NAME4): $(EXEC4)
	./$(EXEC4)

run-$(NAME5): $(EXEC5)
	./$(EXEC5)


.PHONY: clean
clean:
	rm *.o *.x


