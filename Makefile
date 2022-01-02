CC = g++ 

CFLAGS  = -g -Wall

TARGET = MirrorsV1

MODULE = mirror_module

RM = del
#rmdir /s /q

all: $(TARGET)

$(TARGET) : $(TARGET).o $(MODULE).o

	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(MODULE).o


$(TARGET).o: $(TARGET).cpp $(MODULE).h
	$(CC) $(CFLAGS) -c $(TARGET).cpp 

$(MODULE).o: $(MODULE).cpp $(MODULE).h	
	$(CC) $(CFLAGS) -c $(MODULE).cpp 
clean:
#	rm -rf *o test.o
	$(RM) $(TARGET).o
#	rmdir /s /q *o test.o
