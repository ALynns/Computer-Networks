targets=client test_compare

objs=runningopt.o compare.o
headFiles=runningopt.h compare.h

all:$(targets)

.PHONY:all clean

$(objs):$(headFiles)
$(targets):$(objs)

$(objs):%.o:%.c
	gcc -c $<

$(targets):%:%.o
	gcc -o $@ $< $(objs)

clean:
	-rm $(targets) *.o
