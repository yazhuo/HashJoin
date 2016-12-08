objects = main.o global.o join.o loader.o

parallel : $(objects)
	cc -o parallel $(objects) -fopenmp

main.o : main.c global.h loader.h join.h
	cc -c main.c
global.o : global.c global.h
	cc -c global.c
loader.o : loader.c loader.h
	cc -c loader.c
join.o : join.c join.h global.h
	cc -c join.c -fopenmp

.PHONY : clean
clean:
	rm parallel $(objects)
