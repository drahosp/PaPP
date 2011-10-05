CC=gcc
LIBS=-lpthread

all: basic joins mutex condition semaphore

basic: basic.c
	$(CC) -o $@ basic.c $(LIBS)

joins: joins.c
	$(CC) -o $@ joins.c $(LIBS)

mutex: mutex.c
	$(CC) -o $@ mutex.c $(LIBS)

condition: condition.c
	$(CC) -o $@ condition.c $(LIBS)

condition2: condition2.c
  $(CC) -o $@ condition2.c $(LIBS)

semaphore: semaphore.c
	$(CC) -o $@ semaphore.c $(LIBS)
