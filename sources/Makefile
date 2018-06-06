OBJECTS = main.o face_rec_driver.o socket_communication.o
CC = gcc
CFLAGS = -Wall -Wextra
LINK = -L/usr/lib/python3.5/config-3.5m-arm-linux-gnueabihf -L/usr/lib -lpython3.5m -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions 

main: $(OBJECTS) 
	$(CC) $(CFLAGS) $^

face_rec_driver.o: face_rec_driver.c
	$(CC) $(CFLAGS) -c -pthread -I /usr/include/python3.5 $(LINK)

socket_communication.o: socket_communication.c
	$(CC) $(CFLAGS) -c socket_communication.c

clean:
	rm -f $(OBJECTS)

	
