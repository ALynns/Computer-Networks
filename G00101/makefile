object = client runningopt
headFiles = runningopt.h
cFiles = client.c  runningopt.c

client : $(headFiles) $(cFiles)


.PHONY : all clean

all : $(object)

clean :
	-rm -rf $(object)
