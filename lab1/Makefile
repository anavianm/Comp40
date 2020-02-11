#
#
# Makefile Lab 1 
#
#

# Executables to built using "make all"

EXECUTABLES = brightness fgroups



INCLUDES = 

GCC = gcc


COMP40 = /comp/40


CIIFLAGS = `pkg-config --cflags cii40`
CIILIBS = `pkg-config --libs cii40`

CFLAGS = -I. -I$(COMP40)/build/include $(CIIFLAGS)
LIBS = $(CIILIBS) -lm    
LFLAGS = -L$(COMP40)/build/lib

FLAGS = -g -O -Wall -Wextra -Werror -Wfatal-errors -std=c99 -pedantic


all: $(EXECUTABLES)


clean:
	rm -f $(EXECUTABLES) *.o


%.o:%.c $(INCLUDES)
	$(GCC) $(FLAGS) $(CFLAGS) -c $<


brightness: brightness.o
	$(GCC) $(FLAGS) $(LFLAGS) -o brightness brightness.o -lpnmrdr $(LIBS)

fgroups: fgroups.o
	$(GCC) $(FLAGS) $(LFLAGS) -o fgroups  fgroups.o $(LIBS)

echo:
	echo "$(CIIFLAGS)"