
CFLAGS = -pedantic -Wall -std=c99
LFLAGS = -pedantic -Wall -std=c99
CO     = gcc
LD     = $(CO)

THREADS_NUM = 4
CRIT_SECTION_NUM = 3

NAME1  = cw9

OBJS1  = $(NAME1).o

EXEC1  = $(NAME1).x


.PHONY: help
help:
	@echo "make $(NAME1)        Kompiluje program $(NAME1)"
	@echo "make run-$(NAME1)        Uruchamia program $(NAME1)"
	@echo "make clean       Usuwa pliki *.o oraz *.x oraz czysci konsole"
	


$(EXEC1): $(OBJS1)
	$(LD) $(LFLAGS) -o $@ $^


%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<


.PHONY: $(NAME1) 
$(NAME1): $(EXEC1)
	
	

.PHONY: run-$(NAME1) 
run-$(NAME1): $(EXEC1)
	./$(EXEC1) $(THREADS_NUM) $(CRIT_SECTION_NUM)


.PHONY: clean
clean:
	rm *.o *.x & clear


