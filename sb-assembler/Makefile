CC=gcc
CFLAGS=-std=c99 -g -pg -Wall -Wextra -Werror
SRC_DIR=src/
HEADERS_DIR=src/
TEST_DIR=test/
BUILD_DIR=build/

BUCKET_TEST_SOURCES=bucket.c
MAP_TEST_SOURCES=map.c bucket.c
PSEUDO_INSTR_TEST_SOURCES=pseudo_instr.c
REAL_MD_TEST_SOURCES=real_md.c
IDT_TEST_SOURCES=map.c bucket.c pseudo_instr.c real_md.c idt.c hash_aux.c
ASSEMBLER_SOURCES=assembler.c bucket.c hash_aux.c idt.c main.c map.c pseudo_instr.c real_md.c

tests:
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/bucket $(TEST_DIR)/bucket.c $(BUCKET_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/map $(TEST_DIR)/map.c $(MAP_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/pseudo_instr $(TEST_DIR)/pseudo_instr.c $(PSEUDO_INSTR_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/real_md $(TEST_DIR)/real_md.c $(REAL_MD_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/idt $(TEST_DIR)/idt.c $(IDT_TEST_SOURCES:%=$(SRC_DIR)%)
	$(CC) $(CFLAGS) -I$(HEADERS_DIR) -o $(BUILD_DIR)/main $(ASSEMBLER_SOURCES:%=$(SRC_DIR)%)
