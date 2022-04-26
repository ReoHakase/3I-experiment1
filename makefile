CC = gcc
DEPS = main.o regex.o file.o

exe: $(DEPS)
	$(CC) -o exe $(DEPS)

.c.o: inc.h
	$(CC) -c -o $@ $<

clean:
	rm -f exe $(DEPS)