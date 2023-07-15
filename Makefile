SRC=src/
INC=inc/
BIN=bin/
BUILD=build/
TEST=test/

CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wshadow -std=c11 -I$(INC) #-O2 -Werror -Wshadow

.PHONY : clear

$(BUILD)lasm : $(BIN)lasm.o $(BIN)lasm_string.o $(BIN)lasm_hash.o $(BIN)lasm_file.o $(BIN)lasm_instruction.o $(BIN)lasm_parse.o $(BIN)lasm_translate.o 
	$(CC) $(CFLAGS)  $^ -o $@
	cp $@ $(TEST)	
$(BIN)lasm.o : $(SRC)lasm.c 
	$(CC) $(CFLAGS) -c $^ -o $@	
$(BIN)lasm_hash.o : $(SRC)lasm_hash.c
	$(CC) $(CFLAGS) -c $^ -o $@
$(BIN)lasm_string.o : $(SRC)lasm_string.c
	$(CC) $(CFLAGS) -c $^ -o $@
$(BIN)lasm_file.o : $(SRC)lasm_file.c
	$(CC) $(CFLAGS) -c $^ -o $@
$(BIN)lasm_instruction.o : $(SRC)lasm_instruction.c
	$(CC) $(CFLAGS) -c $^ -o $@
$(BIN)lasm_parse.o : $(SRC)lasm_parse.c
	$(CC) $(CFLAGS) -c $^ -o $@
$(BIN)lasm_translate.o : $(SRC)lasm_translate.c
	$(CC) $(CFLAGS) -c $^ -o $@	

clear :
	rm -r *.out
