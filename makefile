CC = clang++-9
FLAG = -Wall -c

all: mainQueue mainGuard mainAO

# queue: queue.o
# 	$(CC) -o  queue.o

# ActiveObject: ActiveObject.o
# 	$(CC) -o ActiveObject.o

# guard: guard.o 
# 	$(CC) -o guard.o

mainQueue: queue.o mainQueue.o 
	$(CC) -o mainQueue mainQueue.o queue.o -lpthread

mainAO: queue.o ActiveObject.o mainAO.o 
	$(CC) -o mainAO mainAO.o queue.o ActiveObject.o  -lpthread

mainGuard: mainGuard.o
	$(CC) -o mainGuard mainGuard.o -lpthread

client: client.o
	$(CC) -o client client.o -lpthread

guard.o: guard.cpp 
	$(CC) $(FLAG) guard.cpp 

ActiveObject.o: ActiveObject.cpp
	$(CC) $(FLAG) ActiveObject.cpp

queue.o: queue.cpp 
	$(CC) $(FLAG) queue.cpp 

mainQueue.o: mainQueue.cpp 
	$(CC) $(FLAG) mainQueue.cpp 
	
mainAO.o: mainAO.cpp 
	$(CC) $(FLAG) mainAO.cpp 

client.o: client.cpp 
	$(CC) $(FLAG) client.cpp 

mainGuard.o: mainGuard.cpp 
	$(CC) $(FLAG) mainGuard.cpp 

clean: 
	rm -rf *.o *.a queue mainQueue mainAO mainGuard client