CC=gcc
CFLAGS=-std=c99 -g -pg -Wall -Wextra -Werror
SRC_DIR=src/
HEADERS_DIR=src/
TEST_DIR=tst/unit-tests/
BIN_BUILD_DIR=bin/
TEST_BUILD_DIR=$(BIN_BUILD_DIR)/test-bin/


BUCKET_TEST_SOURCES=bucket.c
MAP_TEST_SOURCES=map.c bucket.c
VECTOR_TEST_SOURCES=vector.c
LINKER_SOURCES=bucket.c map.c hash_aux.c str_aux.c vector.c linker.c sym_table.c main.c

linker:
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BIN_BUILD_DIR)/ligador $(LINKER_SOURCES:%=$(SRC_DIR)%)

tests:
	mkdir $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(TEST_BUILD_DIR)/bucket $(TEST_DIR)/bucket.c $(BUCKET_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(TEST_BUILD_DIR)/map $(TEST_DIR)/map.c $(MAP_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(TEST_BUILD_DIR)/vector $(TEST_DIR)/vector.c $(VECTOR_TEST_SOURCES:%=$(SRC_DIR)%)

clean:
	rm -rf $(TEST_BUILD_DIR)
	rm -f $(BIN_BUILD_DIR)*
