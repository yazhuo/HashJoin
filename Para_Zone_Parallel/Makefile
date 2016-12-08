objects = main.o global.o join.o loader.o partition.o

p_zone_para : $(objects)
	cc -o p_zone_para $(objects) -fopenmp

main.o : main.c global.h loader.h join.h partition.h
	cc -c main.c
global.o : global.c global.h
	cc -c global.c
loader.o : loader.c loader.h
	cc -c loader.c
join.o : join.c join.h global.h
	cc -c join.c -fopenmp
partition.o : partition.c partition.h global.h
	cc -c partition.c -fopenmp

.PHONY : clean
clean:
	rm p_zone_para $(objects)
