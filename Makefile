# Makefile

# Variables
CC = gcc
CFLAGS = -Wall -g
PREFIX = /usr
BINDIR = $(PREFIX)/games
DATADIR = $(PREFIX)/share/games/bible-verses
TARGET = bible-verse
SRC_DIR = src
SRC = $(SRC_DIR)/script.c
DATA_DIR = data
DATA = $(DATA_DIR)/*.txt

# Build the application
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Install the application and data files
install: $(TARGET)
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/$(TARGET)
	install -d $(DATADIR)
	cp $(DATA) $(DATADIR)/

# Clean up build files
clean:
	rm -f $(TARGET)

# Uninstall the application and data files
uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(DATADIR)

.PHONY: all install clean uninstall
