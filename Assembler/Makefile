# Or Dahan 201644929

CC=gcc
CFLAGS=-c -Wall -g -O0
LDFLAGS=
SOURCES=main.c assembler.c parser.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=assembler

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) *.o *.out
	
# This is done only for my own testing,
# should not be executed
TEST_EXECUTALBE=test_$(EXECUTABLE).out
TEST_SOURCES=test_main.c test_assembler.c test_parser.c
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)

test: $(TEST_SOURCES) $(SOURCES) $(TEST_EXECUTALBE)

$(TEST_EXECUTALBE): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(LDFLAGS) -etest_main $(TEST_OBJECTS) $(OBJECTS) -o $@