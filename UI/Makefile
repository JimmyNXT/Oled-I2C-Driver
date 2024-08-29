# cc game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main
PREFIX ?= /usr/local
OUT = main

# Compile Stuff
CC = cc
CFLAGS = -ggdb -Wall 

LIBS += -L${PREFIX}/lib
LIBS += -lraylib
LIBS += -lGL 
LIBS += -lm 
LIBS += -lpthread 
LIBS += -ldl 
LIBS += -lrt 
LIBS += -lX11


LDFLAGS += ${LIBS}

#Commands
RM = rm

#Headers
HDRS = main.h

#Sources
SRCS = main.c

OBJS = ${SRCS:.c=.o}

#Clear out all siffixes
.SUFFIXES:
#List only thise we use
.SUFFIXES: .o .c

# define a suffix rule for .c -> .o
.c.o :
	@echo -e "Creating Objects..."
	${CC} ${CFLAGS} -c $< -o $@

.MAIN: all

all: ${OUT}

${OBJS}:

${OUT}: ${OBJS}
	@echo -e "Compiling sources..."
	${CC} ${OBJS} ${LDFLAGS} -o ${OUT}

# ${OBJS}:

clean:
	@echo -e "Cleaning..."
	${RM} -f ${OBJS}
	${RM} -f ${OUT}


