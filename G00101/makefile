object = test runningopt
headFiles = runningopt.h
cFiles = test.c  runningopt.c

test : $(headFiles) $(cFiles)


.PHONY : all clean

all : $(object)

clean :
	-rm -rf $(object)
