CC = g++ 

CFLAGS  = -g -Wall

TARGET = MirrorsV1

RM = rm -rf


all: $(TARGET)

$(TARGET) : $(TARGET).o

	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o


$(TARGET).o: $(TARGET).cpp
	$(CC) $(CFLAGS) -c $(TARGET).cpp

clean:

	$(RM) $(TARGET).o

