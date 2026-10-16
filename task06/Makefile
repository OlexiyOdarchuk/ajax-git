CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -O2

TARGET = mycom
SRC = mycom.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -static

clean:
	rm -f $(TARGET) *.log

deploy: $(TARGET)
	scp $(TARGET) ishawyha@$(TARGET_HOST):~/Desktop/

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)/usr/local/bin/$(TARGET)
	install -Dm644 init/mycom.service $(DESTDIR)/etc/systemd/system/mycom.service

format:
	clang-format -i $(SRC)

.PHONY: all clean deploy install format
