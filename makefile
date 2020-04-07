roster: roster.c Student.h
	gcc -Wall -o roster roster.c

clean:
	rm -f *.o roster
