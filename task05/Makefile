CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -O2

TARGET = mycom
SRC = mycom.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.log

deploy: $(TARGET)
	scp $(TARGET) ishawyha@$(TARGET_HOST):~/Desktop/

format:
	clang-format -i $(SRC)

.PHONY: all clean deploy format
