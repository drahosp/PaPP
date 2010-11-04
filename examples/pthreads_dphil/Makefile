CC=gcc
LIBS=-lpthread

all: footman leftie locktable

footman: dphil_common.c dphil_footman.c
	$(CC) -o $@ dphil_common.c dphil_footman.c $(LIBS)

leftie: dphil_common.c dphil_leftie.c
	$(CC) -o $@ dphil_common.c dphil_leftie.c $(LIBS)

locktable: dphil_common.c dphil_locktable.c
	$(CC) -o $@ dphil_common.c dphil_locktable.c $(LIBS)
