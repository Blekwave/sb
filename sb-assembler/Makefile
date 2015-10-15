CC=gcc
CFLAGS=-std=c99 -g -pg -Wall -Wextra -Werror
SRC_DIR=src/
HEADERS_DIR=src/
TEST_DIR=test/
BUILD_DIR=build/

BUCKET_TEST_SOURCES=bucket.c
MAP_TEST_SOURCES=map.c bucket.c

tests:
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/bucket $(TEST_DIR)/bucket.c $(BUCKET_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/map $(TEST_DIR)/map.c $(MAP_TEST_SOURCES:%=$(SRC_DIR)%)