main: main_timer.o
	ld -o main_timer main_timer.o
main.o: main_timer.s
	as -o main_timer.o main_timer.s
clean:
	rm *.o