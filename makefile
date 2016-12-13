CC = g++
CFLAGS1 = -std=c++11
project1:
	$(CC) $(CFLAGS1) -o recovery_data recovery_data.cpp 
clean:
	rm -rf *.o recovery_data