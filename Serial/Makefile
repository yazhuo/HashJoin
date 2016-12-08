objects = main.o global.o join.o loader.o
serial : $(objects)
	cc -o serial $(objects)

main.o : global.h loader.h join.h
global.o : global.h
loader.o : loader.h
join.o : join.h global.h

.PHONY : clean
clean:
	rm serial $(objects)
