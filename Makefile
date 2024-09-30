BINARY_NAME = shegang
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
CTYPES_LIB_DIR = ctypes_lib/bin

CC = clang
CFLAGS = -Wall -Wextra -Werror=implicit-function-declaration -Wpedantic -O3 -g -pipe -fno-fat-lto-objects -fPIC -mtune=native -march=native
CC_SO_FLAGS = -Wall -Wextra -shared -fPIC -O2 -std=c11
LDFLAGS = -L$(BIN_DIR)

INCLUDE_PATHS = -I$(INCLUDE_DIR)

SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
OBJ_SO_FILES = $(patsubst $(SRC_DIR)/%.c, $(CTYPES_LIB_DIR)/%.o, $(SRC_FILES))
LIBRARY = $(CTYPES_LIB_DIR)/libshegang.so

LIB_ISOCLINE = libisocline.a

SUDO		  	= sudo
DEL_FILE      	= rm -f
CHK_DIR_EXISTS	= test -d
MKDIR         	= mkdir -p
COPY          	= cp -f
COPY_FILE     	= cp -f
COPY_DIR      	= cp -f -R
INSTALL_FILE   	= install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   	= cp -f -R
DEL_FILE      	= rm -f
SYMLINK       	= ln -f -s
DEL_DIR       	= rmdir
MOVE          	= mv -f
TAR           	= tar -cf
COMPRESS      	= gzip -9f
LIBS_DIRS     	= -I./include/
SED           	= sed
STRIP         	= strip

all: $(BIN_DIR)/$(BINARY_NAME)

build: $(BIN_DIR)/$(BINARY_NAME)

example_plugin:
	@echo "Compile example plugin"
	@$(CC) $(CC_SO_FLAGS) plugins/pipegang_plugin.c -o pipegang_plugin.so

install: $(BIN_DIR)/$(BINARY_NAME)
	$(SUDO) $(INSTALL_PROGRAM) $(BIN_DIR)/$(BINARY_NAME) /usr/local/bin/

remove:
	$(SUDO) $(DEL_FILE) /usr/local/bin/$(BINARY_NAME)

ctypes: $(LIBRARY)
	@$(DEL_FILE) $(CTYPES_LIB_DIR)/*.o

$(LIBRARY): $(OBJ_SO_FILES)
	@mkdir -p $(CTYPES_LIB_DIR)
	@$(CC) $(INCLUDE_PATHS) $(CC_SO_FLAGS) -o $@ $^

$(CTYPES_LIB_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(INCLUDE_PATHS) $(CC_SO_FLAGS) -c -o $@ $<

$(BIN_DIR)/$(BINARY_NAME): $(OBJ_FILES)
	@echo "CC 		| $@"
	@mkdir -p $(BIN_DIR)
	@$(CC) $(LDFLAGS) $(INCLUDE_PATHS) -o $@ $(OBJ_FILES) $(LIB_ISOCLINE)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC 		| $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE_PATHS) -c $< -o $@ $(LIB_ISOCLINE)

clean:
	@echo "Clean..."
	@rm -rf $(BIN_DIR)
	@rm -rf $(CTYPES_LIB_DIR)/*

reinstall: clean all

.PHONY: all clean reinstall
