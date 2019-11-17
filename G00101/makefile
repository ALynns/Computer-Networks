targets=client test_compare clientBlockFork serverBlockFork clientPoll serverPoll
 
objs=runningopt.o compare.o  savedialog.o tools.o serverbase.o mypoll.o
headFiles=runningopt.h compare.h savedialog.h tools.h common.h clientinfo.h serverbase.h mypoll.h

all:$(targets)

.PHONY:all clean test

$(objs):$(headFiles)
$(targets):$(objs)

$(objs):%.o:%.c
	gcc -c $<

$(targets):%:%.o
	gcc -o $@ $< $(objs)

test:$(objs) $(headFiles)
	gcc -c test.c
	gcc -o test test.o $(objs)

clean:
	-rm $(targets) *.o
