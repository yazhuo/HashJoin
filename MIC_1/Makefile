objects = main.o global.o join.o loader.o partition.o

mic1 : $(objects)
	icc -o mic1 -fopenmp

main.o : main.c global.h loader.h join.h partition.h
	icc -c main.c
global.o : global.c global.h
	icc -c global.c
loader.o : loader.c loader.h
	icc -c loader.c
join.o : join.c join.h global.h
	icc -c join.c -fopenmp
partition.o : partition.c partition.h global.h
	icc -c partition.c -fopenmp

.PHONY : clean
clean:
	rm mic1 $(objects)
