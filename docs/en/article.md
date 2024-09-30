# Programming in C on Linux using the example of creating your own command shell
Hello, habr! The C programming language is fundamental, as I believe. And, no, it is necessary to know him, but it is quite desirable. Most languages refer to C, and knowing C (or C++) another language will be much easier.

I recently decided to improve my C proficiency by writing projects. The very first thought that came to my mind was a command interpreter, a command shell, shell in other words. I will also talk about the make build system, and how to write and document C code correctly.

In this tutorial, I will use the CLANG compiler, not GCC, and tell you its advantages

So, to create your shell in C for Linux, you will need a simple Soviet penny...

---

Link to my command interpreter implementation repository [here](https://github.com/alxvdev/shegang ).

C is one of the most famous and quite popular (even after more than 30 years) a programming language created by Dennis Ritchie, Ken Thompson and Brian Kernighan (he had a hand in popularizing the language). Despite its low-level nature, it is still used in many places. It is fast and lightweight, and also has a relatively clear syntax, which allows you to embed it anywhere, and in principle you can do almost anything in C.

# A brief history of C
C is a razor—sharp tool: you can use it to create both an elegant program and a bloody mess.

The C language has its roots in the ALGOL language (stands for ALGorithmic Language), which was created in 1958 together with a committee of European and American scientists in the field of computer science at a meeting in 1958 at the Swiss Higher Technical School of Zurich. The language was a response to some of the shortcomings of the FORTRAN language and an attempt to fix them.

Inspired by the ALGOL-60 language, the Mathematical Laboratory of the University of Cambridge, together with the Computer Department of the University of London, created the CPL (Combined Programming Language) language in 1963.

The CPL language was considered difficult, and in response to this, Martin Richardson created the BCPL language in 1966, the main purpose of which was to write compilers. Now it is practically not used, but at one time, due to its good portability, it played an important role.

BCPL served as the ancestor for the Bi language, developed in 1969 at the already familiar AT&T Bell Telephone Laboratories, no less familiar by Ken Thompson and Dens Ritchie.

The C programming language was developed between 1969 and 1973 at Bell Labs. C was named specifically C because it was considered an extension of the B language.

By 1973, the C language had become quite strong, and most of the UNIX kernel, originally written in PDP-11/20 assembly language, was rewritten in C. It was one of the very first operating system kernels written in a language other than assembly language; earlier systems were only Multics (written in PL/1) and TRIPOS (written in BCPL).

As his life progressed, myths about the reasons for the development appeared. According to one of them, the development of Si was the result of the fact that its future authors loved a computer game similar to the popular Asteroids game. They had been playing it for a long time on the company's main server, which was not powerful enough and had to serve about a hundred users. Thompson and Ritchie felt that they did not have enough control over the spacecraft in order to avoid collisions with some rocks. So they decided to move the game to a free PDP-7 in the office. However, this computer did not have an operating system, which made them write it. In the end, they decided to transfer this operating system to the office PDP-11 as well, which was very difficult because its code was entirely written in assembly language. A proposal was made to use some high-level portable language so that it would be easy to transfer the OS from one computer to another. The Bi language, which they first wanted to use for this, turned out to be devoid of functionality capable of using the new features of the PDP-11. That's why they decided to develop the C language.

There is another legend. The first computer, for which UNIX was originally written, was intended to create an automatic document filling system. The first version of UNIX was written in assembly language. Later, in order to rewrite this operating system, the C language was developed.

The first book dedicated to the C language was written by Kernighan and Ritchie in 1978 and published under the title "The C Programming Language". This book, better known among programmers as "K&R", has become an unofficial standard for the C language.

The C99 standard is now more or less supported by all modern C compilers. Ideally, code written in C in compliance with standards and without the use of hardware- and system-dependent calls became both hardware- and platform-independent code.

![C Programming Language](https://acm.bsu.by/w/images/1/1a/The_C_Programming_Language_preview.png)

What is written in C:

 + Linux kernel.
 + Windows kernel.
 + Python, Perl, PHP, bash interpreters...
+ Classic UNIX utilities: grep, sed, awk, vim, ...
+ Familiar Windows utilities: PuTTY.
 + Version control systems: git, SVN.
 + nginx and Apache web servers.
 + DBMS: SQLite, MySQL (~30%), PostgreSQL (~85%).
+ Computing tools: MATLAB, Mathematica, numpy, ...
+ Multimedia libraries: ffmpeg, libjpeg, ...

# Brief basics of C
Let's move on to some basics of C. If you think you know C, you can skip this part and move on to the next one. I'm not going to dive deep here, just remember the base.

C is a strongly typed programming language, which means that there are data types in it. A double cannot be written to an int variable, etc.

The following types of data exist:

 + char: represents one character. It takes up 1 byte (8 bits) in memory. It can store any value from the range from -128 to 127
+ unsigned char: represents a single character. It takes up 1 byte (8 bits) in memory. It can store any value from the range from 0 to 255
 + signed char: same as char
 + short: represents an integer in the range from -32768 to 32767. It takes up 2 bytes (16 bits) in memory. It has the aliases short int, signed short and signed short int.
 + unsigned short: represents an integer in the range from 0 to 65535. It takes up 2 bytes (16 bits) in memory.
 + int: represents an integer. Depending on the processor architecture, it can take up 2 bytes (16 bits) or 4 bytes (32 bits). If we take the main platforms - 64-bit Windows, Linux (along with Android) and macOS, then the int size is 4 bytes. The range of limit values, respectively, can also vary from -32768 to 32767 (at 2 bytes) or from -2147,483,648 to 2,147,483,647 (at 4 bytes) and above. It has the aliases signed int and signed
 + unsigned int: represents a positive integer. Depending on the processor architecture, it can take up 2 bytes (16 bits) or 4 bytes (32 bits), and because of this, the range of limit values can vary: from 0 to 65535 (for 2 bytes), or from 0 to 4,294,967,295 (for 4 bytes).
 + long: represents an integer and occupies 4 bytes (32 bits) or 8 bytes (64 bits) in memory. Depending on the size, it can be in the range from -2,147,483,648 to 2,147,483,647 (4 bytes), or in the range from -9223372036854775807 to +9 223 372 036 854 775 807 (8 bytes). If we take widespread platforms, then on 64-bit Windows long takes 4 bytes, and on 64-bit Linux/macOS it takes 8 bytes. It has the aliases long int, signed long int and signed long.
 + unsigned long: Represents an integer and occupies 4 bytes (32 bits) or 8 bytes (64 bits) in memory. Depending on the size, it can be in the range from 0 to 4,294,967,295 (4 bytes) or in the range from 0 to 18,446,744,073,709,551,615 (8 bytes).
+ long long: represents an integer in the range from -9223372036854775807 to +9 223 372 036 854 775 807 . It usually occupies 8 bytes (64 bits) in memory.
+ unsigned long long: represents an integer in the range from 0 to 18,446,744,073,709,551,615. It takes up, as a rule, 8 bytes (64 bits) in memory.
 + float: Represents a single precision floating point real number in the range +/- 3.4E-38 to 3.4E+38. It takes 4 bytes (32 bits)
+ double in memory: it represents a double precision floating point real number in the range +/- 1.7E-308 to 1.7E+308. It takes up 8 bytes (64 bits) in memory
 + long double: Represents a double precision floating point real number in the range +/- 3.4E-4932 to 1.1E+4932. It takes up 10 bytes (80 bits) in memory. On some systems, it may take 96 and 128 bits.
 + void: type without value

![C Data types](https://habrastorage.org/webt/zj/_x/p-/zj_xp-pzendwqs6wvrktm7npx-s.jpeg )

Here's what the classic "Hello, World" example looks like in C:

```c
#include <stdio.h>

int main(void) {
	printf("Hello, World");

	return 0;
}
```

The `main` function is the entry point to the program. The string `#include <stdio.h>` includes a library (or rather a header file), in this case stdio.h, which has a printf function.

## Pointers
C is mostly a low-level programming language, so memory management plays a big role here. One of these parts is working with pointers.

Each program has data, and this data is stored at a specific address in memory. And that's why there are pointers - they point to the memory area to which this or that variable belongs.

So, to create a pointer, you need the following construction:

```c
datatype* designator name;

# for example
int *first_number;
int second_number = 10;

first_number = &second_number;
```

The pointer stores the address of the object in the computer's memory. And the & operation is applied to the variable to get the address. This operation applies only to objects that are stored in computer memory, that is, to variables and array elements.

Naturally, all data types must match. 

In order to learn something new in memory, you may need to learn something new from print:

```
#enable <stdio.h>

int main(empty) {
int *first_name;
int secondary number = 10;

	first_number = &second_number;

	printf("%p\n", first_number); // enter, example: 0x7ffce01f1a6c

	returns 0;
}
```

The hexadecimal system is used for addresses in memory. In fact, the address represents an integer value expressed in hexadecimal format.

But since the pointer stores the address, we can use this address to get the value stored there, that is, the value of the variable. To do this, use the * operation or the dereference option (dereference operator). The result of this operation is always the object that the pointer points to. Let's take this opera and get the value of the variable x:

```c
#enable <stdio.h>

int main(empty) {
int *first_name;
int secondary number = 10;

	first_number = &second_number;

	printf("%d\n", *first_name); // outputs 10

	returns 0;
}
```

The pointer value can also be passed to another variable:

```c
#include <standard.h file>

int main(void) {
int *first_number;
int second_number = 10;
int last_number;

	first_number = &second_number;
	last_number = *first_number;

	printf("%d\n", last number); // outputs 10

	returns 0;
}
```

And you can also change the values at the address:

```c
#enable <stdio.h>

int main(void) {
int *first_name;
int secondary number = 10;

	primary number = &secondary number;
	*first_name = 0;

	printf("%d\n", first_number); // outputs 0

	returns 0;
}
```

## Memory Management
A number of functions are used to control dynamic memory allocation, which are defined in the stdlib.h header file:

`malloc()` — allocates memory of the specified size and returns a pointer (void*) to the memory.

Allocates s bytes of memory and returns a pointer to the beginning of the allocated memory. In case of an unsuccessful execution, it returns NULL.

'calloc()` — allocates space for array elements, initializes it to zero and returns a pointer to memory.

Allocates memory for n elements of m bytes each and returns a pointer to the beginning of the allocated memory. In case of an unsuccessful execution, it returns NULL.

`free()` — frees up previously allocated space.

Frees up a previously allocated block of memory, the beginning of which is indicated by the pointer bl.

`realloc()' — changes the size of the previously allocated space.

Modifies the size of the previously allocated memory block, at the beginning of which the bl pointer points, to a size of ns bytes. If the bl pointer is NULL, that is, no memory has been allocated, then the function's action is similar to the malloc action.

You can read more in [this good tutorial] (https://metanit.com/c/tutorial/5.8.php ).

---

If you want to learn more, I recommend [this series of lessons on the C language (ru)](https://metanit.com/c/tutorial/1.1.php ) and [this article (ru)](https://habr.com/ru/articles/464075 /).

# Compilers: gcc or clang
I also decided to make a small explanation of why I chose to use clang rather than gcc. GCC has extensive support for the legacy of idioms and constructs from the C language that are supported in modern C++. Even today, GCC repeatedly violates C++ standards due to the strategy of supporting exotic C functionality in C++ code.

Clang today is an ideal option for following the standards. It supports all standards in all details, has an extensive system of statistical and dynamic verification of the year, the same `clang format` is banal. It also gives access to the parsing tree.

Also, clang is much stricter than gcc - many things that can be a common warning in gcc can become an error. And clang is also supported by many big tech companies. And it is thanks to a more free license than the GNU GPL (openness does not mean freedom).

Clang has a modular architecture - a separate parser, an optimizer and a code generator. Thus, it is easy to add support for a new architecture or programming language (for example, rust runs on llvm). And thanks to this, it is easy to create a programming language based on llvm. Just google it: how to create your own programming language llvm3

And also clang had the libclang library right from the very beginning. This library is used by 99% of all IDEs for C++: KDevelop, Qt Creator, Clion.

Although GCC is easier to use than clang at times, for example, gcc is used in all OS projects (kolibri os, os/2).

CLANG/LLVM turned out to be more suitable for the UNIX-WAY, because gcc has acquired legacy code, anyway.

In terms of optimization, clang and gcc are about the same, in some scenarios clang is not much faster, in others gcc is also not much faster.

# Writing our own command interpreter
## Project architecture
The most important thing in any project is to implement its architecture correctly so that you don't have to suffer later. In fact, there is nothing difficult in drawing up a competent architecture - you just need to think one step ahead. Think about if you add any functionality, will it be convenient to add it?

I got this architecture:

```
.
├── build_scripts
│   ├── build_make.sh
│   ├── create_package.sh
│   ├── uninstall.sh
│   └── update.sh
├── CHANGELOG.md
├── config
├── ctypes_lib
│   ├── bin
│   ├── README.md
│   └── shegang.py
├── include
│   ├── builtin.h
│   ├── colors.h
│   ├── config.h
│   ├── executor.h
│   ├── tasks_processing.h
│   └── userinput.h
├── LICENSE
├── Makefile
├── README.md
├── shegang_en.man
└── src
    ├── config
    │   └── config.c
    ├── core
    │   └── userinput.c
    ├── execution
    │   ├── builtin
    │   │   └── base.c
    │   ├── executor.c
    │   └── tasks_processing.c
    ├── features
    │   └── colors.c
    └── shegang.c
```

Let's take a closer look. Auxiliary shell scripts are stored in the build_scripts directory, such as `build_make.sh ` - to build the project, `create_package.sh ` - to create a tarball archive with a binary, manual, etc., `uninstall.sh ` - to delete and `update.sh ` - for updating.

The ctypes_lib directory is needed for examples of using our shell, but not as a binary, but as a shared object, a library in python using the ctypes module. It contains an empty bin directory - that's where the library file is moved during compilation.

The include directory is needed for the header files of the modules.

The src directory is the main one, where the source code files are located. Config contains the configuration reading code, core contains the basic basic features, that is, reading user input. The execution directory contains files for creating processes and basic commands, as well as built-in functionality located in the builtin subdirectory. Features contains some cool features, in this case colors and functions for more beautiful and convenient text and message output.

The shegang.c file is the main one, all modules are imported into it and it is responsible for launching the shell.

CHANGELOG.md , LICENSE and README.md - these are files for the github repository.

But the Makefile is a file of assembly instructions for make, which we'll talk about now.

## Make Build System
It is not convenient to manually drive commands for assembly, and therefore assembly systems have been created. One of the oldest, most popular and quite simple is Make.

The basis of the build is a Makefile. It has its own syntax and is needed for instructions on how to build programs.

That's what happened to me:

```Makefile
BINARY_NAME = shegang
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
CTYPES_LIB_DIR = ctypes_lib/bin

CC = clang
CFLAGS = -Wall -Wextra -Werror=implicit-function-declaration -Wpedantic -O2 -g -pipe -fno-fat-lto-objects -fPIC -mtune=native -march=native
CC_SO_FLAGS = -Wall -Wextra -shared -fPIC -O2 -std=c99 -lreadline
LDFLAGS = -L$(BIN_DIR) -lreadline

INCLUDE_PATHS = -I$(INCLUDE_DIR)

SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
OBJ_SO_FILES = $(patsubst $(SRC_DIR)/%.c, $(CTYPES_LIB_DIR)/%.o, $(SRC_FILES))
LIBRARY = $(CTYPES_LIB_DIR)/libshegang.so

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

install: $(BIN_DIR)/$(BINARY_NAME)
	$(SUDO) $(INSTALL_PROGRAM) $(BIN_DIR)/$(BINARY_NAME) /usr/local/bin/

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
	@$(CC) $(LDFLAGS) $(INCLUDE_PATHS) -o $@ $(OBJ_FILES) 

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC 		| $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE_PATHS) -c $< -o $@

clean:
	@echo "Clean..."
	@rm -rf $(BIN_DIR)
	@rm -rf $(CTYPES_LIB_DIR)/*

reinstall: clean all
```

We will analyze it in stages:

```Makefile
BINARY_NAME = shegang
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
CTYPES_LIB_DIR = ctypes_lib/bin
```

From above you can see, so to speak, the variables. They point to the file paths - that is, the name of the binaries, the source code directory, the directory for binaries, the directory for header files, and the directory for the library binaries.

```make
CC = clang
CFLAGS = -Wall -Wextra -Werror=implicit-function-declaration -Wpedantic -O2 -g -pipe -fno-fat-lto-objects -fPIC -mtune=native -march=native
CC_SO_FLAGS = -Wall -Wextra -shared -fPIC -O2 -std=c99 -lreadline
LDFLAGS = -L$(BIN_DIR) -lreadline
```

You can see the compiler from above - we use clang flags for it, as well as flags for creating the library, as well as flags for LD, and we indicate that we are connecting the `readline` library. This library is needed to read user experience and input history.

```make
INCLUDE_PATHS = -I$(INCLUDE_DIR)

SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
OBJ_SO_FILES = $(patsubst $(SRC_DIR)/%.c, $(CTYPES_LIB_DIR)/%.o, $(SRC_FILES))
LIBRARY = $(CTYPES_LIB_DIR)/libshegang.so
```

And we come to variables for files and paths. Through the construction `$(<VAR>)` we can get data from a variable. We get a flag for including header files (for the compiler). We get the source code files through the command. To execute the command, first use the keyword `shell`: `$(shell find $(SRC_DIR) -type f -name "*.c")`. The command `find$(SRC_DIR) -type -f -name "*.c"` searches for everything.C files in the source code directory.

But in OBJ_FILES and OBJ_SO_FILES, patsubst is already used - corny to change the paths of c-code files to .o files.

```make
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
```

There are just a bunch of aliases of useful commands above.

```make
all: $(BIN_DIR)/$(BINARY_NAME)

build: $(BIN_DIR)/$(BINARY_NAME)

install: $(BIN_DIR)/$(BINARY_NAME)
	$(SUDO) $(INSTALL_PROGRAM) $(BIN_DIR)/$(BINARY_NAME) /usr/local/bin/

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
	@$(CC) $(LDFLAGS) $(INCLUDE_PATHS) -o $@ $(OBJ_FILES) 

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC 		| $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE_PATHS) -c $< -o $@

clean:
	@echo "Clean..."
	@rm -rf $(BIN_DIR)
	@rm -rf $(CTYPES_LIB_DIR)/*

reinstall: clean all
```

Above is the most basic part - directives, functions, commands. The structure of the team is as follows:

```make
<goal>: <dependencies>
	# commands
```

That is, for example, when you type `make clean`, the clean directive will work. But the build command has dependencies, make looks for goals where these dependencies are and executes it first, and then only the initial function.

In some functions you can see $@ and $<. It's simple - $@ is a goal, and $< is a dependency.

And the @ sign in front of the commands is needed to hide the command, that is, so that the command text itself is not output.

There is also an all function, it runs if you just run `make'.

## Writing the code
So we have reached the main part of the article. So, the basic minimum functionality of the shell:

 + reading user input;
 + team history;
 + displaying messages about an incorrect command or error;
 + running processes and tasks on POSIX
 + read the configuration to configure the shell;
 + basic built-in commands;
 + shell prompta setting;

### Colors, formatting
To begin with, a file with no dependencies - that is, `colors.c`, for displaying colors, formatting messages and for displaying a shell prompt line.

```c
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <time.h>
#include <wait.h>

// Macros for ANSI codes (formatting). The names of macros are replaced with their values during compilation.

// Formatting
#define RESET 			"\033[0m"
#define BOLD 			"\033[1m"
#define DIM 			"\033[2m"
#define ITALIC 			"\033[3m"
#define UNDERLINE 		"\033[4m"
#define BLINK 			"\033[5m"
#define REVERSE 		"\033[7m"
#define HIDDEN 			"\033[8m"
#define BOLD_ITALIC 	"\033[3;1m"

// Colors
#define BLACK			"\033[0;30m"
#define RED				"\033[0;31m"
#define GREEN			"\033[0;32m"
#define YELLOW			"\033[0;33m"
#define BLUE			"\033[0;34m"
#define MAGENTA			"\033[0;35m"
#define CYAN			"\033[0;36m"
#define WHITE			"\033[0;37m"
#define GRAY			"\033[90m"

// Macros for displaying messages, that is, their type. To make the code more readable.
#define DEBUG -1
#define INFO 0
#define WARNING 1
#define ERROR 2

// Macro of the maximum directory path
#define MAX_DIRECTORY_PATH 1024

// This will be in the section about downloading the config. That is, the color of the user name, the color of the current directory and the color of the current time
// this is necessary for
the extern char* username_color prompt;
extern char* pwd_color;
extern char* curr_time_color;

/**
* @brief Function for getting ANSI color code by its name
 *
* @param const char* color_name Color name
 * 
 * @return char* ANSI color code
 **/
char* get_color_by_name(const char* color_name) {
	char* color_value;

	if (strcmp(color_name, "RED") == 0) {
		color_value = RED;
	} else if (strcmp(color_name, "GREEN") == 0) {
		color_value = GREEN;
	} else if (strcmp(color_name, "YELLOW") == 0) {
		color_value = YELLOW;
	} else if (strcmp(color_name, "BLUE") == 0) {
		color_value = BLUE;
	} else if (strcmp(color_name, "MAGENTA") == 0) {
		color_value = MAGENTA;
	} else if (strcmp(color_name, "CYAN") == 0) {
		color_value = CYAN;
	} else if (strcmp(color_name, "WHITE") == 0) {
		color_value = WHITE;
	} else if (strcmp(color_name, "GRAY") == 0) {
		color_value = GRAY;
	} else if (strcmp(color_name, "BLACK") == 0) {
		color_value = BLACK;
	} else {
		color_value = RESET;
	}

	return color_value;
}

/**
* @brief The simplest function for displaying text with a new line
*
* @param const char* message Message text 
 **/
void println(const char* message) {
	printf("%s\n", message);
}

/**
* @brief Text output with a new line in a certain color
 *
* @param const char* message Message text
 * @param char* message_color ANSI color code
 **/
void println_colored(const char* message, char* message_color) {
	printf("%s%s%s\n", message_color, message, RESET);
}

/**
* @brief Text output without a new line in a certain color
 *
* @param const char* message Message text
 * @param char* message_color ANSI color code
 **/
void print_colored(const char* message, char* message_color) {
	printf("%s%s%s", message_color, message, RESET);
}

/**
 * @brief Function for displaying a log message
 *
* @param const char* message Message text
 * @param int message_type Color macro
 * 
 * @return void
 **/
void print_message(const char* message, int message_type) {
	const char* color;
	const char* format;
	const char* msgtype_string;

	switch (message_type) {
		case DEBUG:
			color = CYAN;
			format = BOLD;
			msgtype_string = "[DEBUG]";
			break;
		case INFO:
			color = GREEN;
			format = BOLD;
			msgtype_string = "[INFO]";
			break;
		case WARNING:
			color = YELLOW;
			format = DIM;
			msgtype_string = "[WARNING]";
			break;
		case ERROR:
			color = RED;
			format = BOLD_ITALIC;
			msgtype_string = "[ERROR]";
			break;
		default:
			color = WHITE;
			format = RESET;
			msgtype_string = "[DEFAULT]";
			break;
	}

	if (message_type == ERROR) {
// If the message type is an error message, then output the text to stderr
		fprintf(stderr, "%s%s%s%s%s %s\n", RESET, color, format, msgtype_string, RESET, message);
	} else {
		printf("%s%s%s%s %s%s\n", RESET, color, format, msgtype_string, RESET, message);
	}

	printf(RESET);
}

/**
* @brief Function for shell prompt output
 **/
void display_ps(void) {
	pid_t uid = geteuid(); // user uid
struct passwd *pw = getpwuid(uid); // user structure
char cwd[MAX_DIRECTORY_PATH]; // character array(string) of the current directory
	time_t rawtime; // raw time
struct tm * timeinfo; // structure of time information

	time(&rawtime); // getting the time
	timeinfo = localtime(&rawtime); // local time information

	// first line
	printf("%s┬─%s%s[%s", DIM, RESET, GRAY, RESET);

	// if the user structure is not empty
if (pw != NULL) {
		printf("%s%s%s:", username_color, pw->pw_name, RESET);
	}

	// if it is possible to get the current directory
if (getcwd(cwd, MAX_DIRECTORY_PATH) != NULL) {
		printf("%s%s%s]%s", pwd_color, cwd, GRAY, RESET);
	}

	printf("%s─%s", DIM, RESET);

	// getting the current time
	printf("%s[%s%s%d:%d:%d%s%s]%s", GRAY, RESET, curr_time_color, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, RESET, GRAY, RESET);

	/*
example of a prompt:

	┬─[alexeev:/home/alexeev/Desktop/Projects/shegang]─[20:0:55]
	╰─> $ 
	 */
}
```

### Reading user input
The main part of our shell can be considered reading user input, which is why I brought the basic functionality to the core.

```c
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "colors.h"

#define DEFAULT_BUFFER_SIZE 128 // buffer size
#define TOKENS_DELIMETERS "\t" // split tokens

/**
* @brief Function for reading user input (and writing it to history)
 * 
 * @return char* Input buffer
 **/
char* read_user_input(void) {
	rl_getc_function = getc;
	fflush(stdout);

	// Input buffer (that is, we get the output via readline from a new line with coloring in colors for beauty)
char* input_buffer = readline("\n\033[2m╰─>\033[0m\033[0;36m $\033[0m");

	// If the line is missing everything (ctrl+d, ctrl+c)
if (input_buffer == NULL) {
		print_message("Failed to read user input", WARNING);
		return NULL;
	}

	// If the line is not empty
	if (*input_buffer != '\0') {
		add_history(input_buffer);
	}

	return input_buffer;
}

/**
 * @brief Legacy user input reading method (no history and no auto stand)
 * 
 * @return char* a string entered by the user 
 **/
char* fgets_input(void) {
	size_t max_length = DEFAULT_BUFFER_SIZE; // maximum buffer size
	char* input_buffer = (char*)calloc(max_length, sizeof(char)); // allocating memory for the input buffer

	print_message("This function is legacy. Use read_user_input instead of fgets_input", WARNING);

	// If memory has not been allocated
if (input_buffer == NULL) {
		print_message("\nFailed to allocate memory for input buffer", ERROR);
		return NULL;
	}

	// Exception handler
if (fgets(input_buffer, max_length, stdin) == NULL) {
if (ferror(stdin)) {
			// If ctrl+c and ctrl+d
are pressed print_message("\nFailed to read user input", ERROR);
		}

		// freeing up memory
		free(input_buffer);
		return NULL;
	}

	// checking for a new line
	char* newline = strchr(input_buffer, '\n');

	if (newline != NULL) {
		*newline = '\0';
	}

	return input_buffer;
}

/**
* @brief Function to split a command into an array of data (tokenization)
*
* @param char* line Line to split
 * @return char** 
 **/
char** split_into_tokens(char* line) {
	size_t position = 0; // position
size_t buffer_size = DEFAULT_BUFFER_SIZE; // buffer size

	char* token; // current token
char** tokens = (char**)malloc(sizeof(char*) * buffer_size); // allocating memory for tokens

	// If tokens are empty or memory allocation error
if (tokens == NULL) {
		print_message("Couldn't allocate buffer for splitting", ERROR);
		return NULL;
	}

	// tokenize the string
	token = strtok(line, TOKENS_DELIMETERS);

	// Division into tokens
	while (token != NULL) {
		tokens[position++] = token;

		// if the position is greater than or equal to the buffer size
		if (position >= buffer_size) {
			buffer_size *= 2;

			tokens = (char**)realloc(tokens, buffer_size * sizeof(char*));

			if (tokens == NULL) {
				print_message("Couldn't reallocate buffer for tokens", ERROR);
				return NULL;
			}
		}

		// setting the token
		token = strtok(NULL, TOKENS_DELIMETERS);
	}

	tokens[position] = NULL;

	return tokens;
}
```

### Reading the configuration
Now let's look at the code for reading the configuration. At the moment, it is simple and contains the basic settings for the shell prompt.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "colors.h"

#define CONFIG_FILE "/.shegangrc" // config file, / is the root of the user's home directory
#define DEFAULT_USERNAME_COLOR "\033[0;33m" // default username color
#define DEFAULT_PWD_COLOR "\033[0;32m" // the default color of the current directory
#define DEFAULT_CURR_TIME_COLOR "\033[0;35m" // default color of the current time

// Setting variables (in colors.c we use extend to globalize them)
char* username_color;
char* pwd_color;
char* curr_time_color;

/**
* @brief Function for loading the config
 **/
void load_config(void) {
	char* home_dir = getenv("HOME");
	char line[256];

	if (!home_dir) {
		username_color = DEFAULT_USERNAME_COLOR;
		pwd_color = DEFAULT_PWD_COLOR;
		curr_time_color = DEFAULT_CURR_TIME_COLOR;
		return;
	}

	char config_path[strlen(home_dir) + strlen(CONFIG_FILE) + 1];
	sprintf(config_path, "%s%s", home_dir, CONFIG_FILE);

	FILE* config_file = fopen(config_path, "r");

	if (!config_file) {
		username_color = DEFAULT_USERNAME_COLOR;
		pwd_color = DEFAULT_PWD_COLOR;
		return;
	}

	// read the config and set
	while (fgets(line, sizeof(line), config_file)) {
		char* key = strtok(line, "=");
		char* value = strtok(NULL, "\n");

		if (key && value) {
			if (strcmp(key, "USERNAME_COLOR") == 0) {
				username_color = get_color_by_name(value);
			} else if (strcmp(key, "PWD_COLOR") == 0) {
				pwd_color = get_color_by_name(value);
			} else if (strcmp(key, "TIME_COLOR") == 0) {
				curr_time_color = get_color_by_name(value);
			}
		}
	}

	fclose(config_file);

	if (!username_color) {
		username_color = DEFAULT_USERNAME_COLOR;
	}
	if (!pwd_color) {
		pwd_color = DEFAULT_PWD_COLOR;
	}
	if (!curr_time_color) {
		curr_time_color = DEFAULT_CURR_TIME_COLOR;
	}
}
```

### Completing tasks
It's a complicated thing, you need to create tasks and processes using posix.

To do this, we need to find out the database. Let's start with fork - in simple words, the fork system call creates a complete clone of the current process, they differ only in their identifier, i.e. pid.

The exec system call replaces the current process with a third-party one. Naturally, the third-party process is set through the function parameters.

```c
#include <wait.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "colors.h"

// Setting functions (they are needed to avoid warnings)
int kill(pid_t pid, int);
int quit();

/**
 * Background task structure
 *
* @param pid_t PID zalachi
 * @param int is_finished Boolean value (0 or 1), whether the task is completed
 * @param char* timestamp Timestamp
 * @param char* command Task command
 **/
struct background_task_t {
	pid_t pid;
	int is_finished;
	char* timestamp;
	char* command;
};
typedef struct background_task_t bg_task;

/**
 * The structure of the foreground task
 *
* @param pid_t task PID
 * @param int is_finished Boolean value (0 or 1), whether the task is completed
 **/
struct foreground_task_t {
	pid_t pid;
	int is_finished;
};
typedef struct foreground_task_t fg_task;

/**
 * Task structure
 *
* @param fg_task foreground_task Foreground Task Structure
 * @param bg_task* background_task Background task structure
 * @param size_t cursor Cursor
 * @param size_t capacity Task capacity
 **/
struct tasks_t {
	fg_task foreground_task;
	bg_task* background_task;
	size_t cursor;
	size_t capacity;
};
typedef struct tasks_t tasks;

/**
 * @brief global task structure
 **/
tasks tasks_structure = {
	.foreground_task = {
		.pid = -1,
		.is_finished = 1
	},
	.background_task = 0,
	.cursor = 0,
	.capacity = 0
};

/**
* @brief Function to set the foreground task
 *
* @param pid_t pid PID of the process
 **/
void set_foreground_task(pid_t pid) {
	tasks_structure.foreground_task.pid = pid;
	tasks_structure.foreground_task.is_finished = 0;
}

/**
* @brief Functions for adding a background task
 *
* @param pid_t pid PID of the process
 * @param char* name The name of the task
 * 
 * @return int 
 **/
int add_background_task(pid_t pid, char* name) {
	bg_task* bt;

	if (tasks_structure.cursor >= tasks_structure.capacity) {
		tasks_structure.capacity = tasks_structure.capacity * 2 + 1;
		tasks_structure.background_task = (bg_task*)realloc(tasks_structure.background_task, sizeof(bg_task) * tasks_structure.capacity);

		if (tasks_structure.background_task == 0 || tasks_structure.background_task == NULL) {
			print_message("Couldn't reallocate buffer for background tasks!", ERROR);
			return -1;
		}
	}

	printf("[%zu] task started\n", tasks_structure.cursor);

	bt = &tasks_structure.background_task[tasks_structure.cursor];

	bt->pid = pid;
	bt->is_finished = 0;

	time_t timestamp = time(NULL);
	bt->timestamp = ctime(&timestamp);

	bt->command = name;

	tasks_structure.cursor += 1;

	return 1;
}

/**
* @brief Function to complete the foreground task
 **/
void kill_foreground_task(void) {
	if (tasks_structure.foreground_task.pid != -1) {
		kill(tasks_structure.foreground_task.pid, SIGTERM);
		tasks_structure.foreground_task.is_finished = 1;

		printf("\n");
	}
}

/**
* @brief Function to complete the background task
 * @param char** args Command line arguments
 * 
 * @return int Result code
 **/
int term_background_task(char** args) {
	char* idx_str;
	int proc_idx = 0;

	if (args[1] == NULL) {
		print_message("No process index to stop", ERROR);
	} else {
		idx_str = args[1];

		while (*idx_str >= '0' && *idx_str <= '9') {
			proc_idx = (proc_idx * 10) + ((*idx_str) - '0');

			idx_str += 1;
		}

		if (*idx_str != '\0' || (size_t)proc_idx >= tasks_structure.cursor) {
			print_message("Incorrect background process index!", ERROR);
		} else if (tasks_structure.background_task[proc_idx].is_finished == 0) {
			kill(tasks_structure.background_task[proc_idx].pid, SIGTERM);
		}
	}

	return 1;
}

/**
* @brief Function to check if the background task is
 *
* @param char** args Arguments
 * 
 * @return int (bool) 1 if yes, 0 if no
 **/
int is_background_task(char** args) {
	int last_arg_id = 0;

	while (args[last_arg_id + 1] != NULL) {
		last_arg_id++;
	}

	if (strcmp(args[last_arg_id], "&") == 0) {
		args[last_arg_id] = NULL;

		return 1;
	}

	return 0;
}

/**
* @brief Function for creating and running a task
 * 
 * @param char** args Array of arguments
 **/
int launch_task(char** args) {
	pid_t pid;
	int background = is_background_task(args);

	pid = fork();

	if (pid < 0) {
		print_message("Couldn't create child process!", ERROR);
	} else if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			print_message("Couldn't execute unknown command!", ERROR);
		}

		exit(1);
	} else {
		if (background == 1) {
			if (add_background_task(pid, args[0]) == -1) {
				quit();
			}
		} else {
			set_foreground_task(pid);

			if (waitpid(pid, NULL, 0) == -1) {
				if (errno != EINTR) {
					print_message("Couldn't track the completion of the process", WARNING);
				}
			}
		}
	}

	return 1;
}


/**
* @brief Function for marking completed tasks
*/
void mark_ended_task(void) {
bg_task* bt;

	pid_t pid = waitpid(-1, NULL, 0);

	if (pid == tasks_structure.foreground_task.pid) {
		tasks_structure.foreground_task.is_finished = 1;
	} else {
		for (size_t i = 0; i < tasks_structure.cursor; i++) {
			bt = &tasks_structure.background_task[i];

			if (bt->pid == pid) {
				printf("Task %zu is finished\n", i);

				bt->is_finished = 1;

				break;
			}
		}
	}
}
```

### Built-in functions and command execution
So, there will be two files here. First, we will analyze executor.c:

```c
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/history.h>
#include "colors.h"
#include "tasks_processing.h"
#include "config.h"
#include "builtin.h"

extern tasks tasks_structure;
extern char* username_color;
extern char* pwd_color;
extern char* curr_time_color;

// status code: 1 - continued operation, 0 - end

/**
* @brief Function for displaying shell information
 * 
 * @return int Status code
 **/
int help(void) {
	println("she#gang Linux Shell in C  @  by alxvdev\n");
	printf(
		"Built-in shell special functions:\n"
		" cd     <path>           - Change the directory\n"
		" term   <bg_task_idx>    - Kill background task by id\n"
		" help                    - Prints info about she#gang\n"
		" bg                      - Prints list with background tasks\n"
		" quit/exit               - Terminate shell with all active tasks\n"
		" history                 - Print the shell commands history\n"
	);

	return 1;
}

/**
* @brief Function to exit and complete all tasks
 * 
 * @return int Status code
 **/
int quit(void) {
	bg_task* bt;

	signal(SIGCHLD, SIG_IGN);

	if (!tasks_structure.foreground_task.is_finished) {
		kill_foreground_task();
	}

	for (size_t i = 0; i < tasks_structure.cursor; i++) {
		bt = &tasks_structure.background_task[i];

		if (bt->is_finished == 0) {
			kill(bt->pid, SIGTERM);
		}

		free(bt->command);
	}

	return 0;
}

/**
* @brief Function for displaying command history
 * 
 * @return int Status code
 **/
int print_history(void) {
	HIST_ENTRY **history = history_list();

	if (history) {
		for (int i=0; history[i]; i++) {
			printf("%s\n", history[i]->line);
		}
	}

	return 1;
}

/**
* @brief Function for shell reboot (i.e. config reboot)
**/
int reload_shell(void) {
	load_config();

	return 1;
}

/**
* @brief Function for the built-in shegang_config function (inspired by fish_config from fish)
*
* @param char **args Array of arguments
 * 
 * @return int Status codes
 **/
int shegang_config(char** args) {
	if (args[1] == NULL) {
		print_message("Expected argument for \"shegang_config\" command. Launch with help for view help page", WARNING);
		return 1;
	} else if (strcmp(args[1], "help") == 0) {
		println("Built-in function `shegang_config`\n");
		printf(
			"set <VAR> <VALUE>           - set value for var (ex. set USERNAME_COLOR RED)\n"
			"\nExisting variables: USERNAME_COLOR; PWD_COLOR; TIME_COLOR;\n"
			"Existing colors: RED, GREEN, BLUE, YELLOW, MAGENTA, GRAY, BLACK, WHITE, CYAN\n"
		);
	} else if (strcmp(args[1], "set") == 0) {
		if (args[3] == NULL) {
			print_message("Expected argument for \"shegang_config\" command: color", WARNING);
			return 1;
		}

		char* color = get_color_by_name(args[3]);

		if (strcmp(args[2], "USERNAME_COLOR") == 0) {
			username_color = color;
		} else if (strcmp(args[2], "PWD_COLOR") == 0) {
			pwd_color = color;
		} else if (strcmp(args[2], "TIME_COLOR") == 0) {
			curr_time_color = color;
		} else {
			print_message("Expected argument for \"shegang_config\" command: variable name", WARNING);
			return 1;
		}
	}

	return 1;
}

/**
* @brief Function to run the command
 * 
 * @return int Status code
 **/
int execute(char** args) {
	if (args[0] == NULL) {
		return 1;
	} else if (strcmp(args[0], "cd") == 0) {
		return change_directory(args);
	} else if (strcmp(args[0], "help") == 0) {
		return help();
	} else if (strcmp(args[0], "quit") == 0 || strcmp(args[0], "exit") == 0) {
		return quit();
	} else if (strcmp(args[0], "bg") == 0) {
		return bg_tasks();
	} else if (strcmp(args[0], "term") == 0) {
		return term_background_task(args);
	} else if (strcmp(args[0], "history") == 0) {
		return print_history();
	} else if (strcmp(args[0], "reload") == 0) {
		return reload_shell();
	} else if (strcmp(args[0], "shegang_config") == 0) {
		return shegang_config(args);
	} else {
		return launch_task(args);
	}
}
```

And builtin/base.c:

```c
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/history.h>
#include "colors.h"
#include "tasks_processing.h"
#include "config.h"

extern tasks tasks_structure;
extern char* username_color;
extern char* pwd_color;
extern char* curr_time_color;

/**
* @brief Function for changing the directory
 *
* @param char** args Array of arguments
 * 
 * @return int Status code
 **/
int change_directory(char** args) {
	if (args[1] == NULL) {
		print_message("Expected argument for \"cd\" command", ERROR);
	} else if (chdir(args[1]) != 0) {
		print_message("Couldn't change directory", ERROR);
	}

	return 1;
}

/**
 * @brief Function for displaying background tasks
 * 
 * @return int Status code
 **/
int bg_tasks(void) {
	bg_task* bt;

	for (size_t i = 0; i < tasks_structure.cursor; i++) {
		bt = &tasks_structure.background_task[i];

		printf(
			"[%zu]%s command; %s%s;%s pid: %s%d; %s"
			"state: %s%s;%s timestamp: %s%s", i,
			MAGENTA, RESET, bt->command,
			MAGENTA, RESET, bt->pid,
			MAGENTA, RESET, bt->is_finished ? "is_finished" : "active",
			MAGENTA, RESET, bt->timestamp
		);
	}

	return 1;
}
```

### Connecting everything together
And the final main file is shegang.c:

```c
#include <stdio.h>
#include <unistd.h>
#include "userinput.h"
#include "executor.h"
#include "tasks_processing.h"
#include "config.h"

extern tasks tasks_structure;
extern char* username_color;
extern char* pwd_color;

// main function
int main(void) {
char* line;
	char** args;
	int status;

	signal(SIGINT, kill_foreground_task);
	signal(SIGCHLD, mark_ended_task);

	load_config();

	// a small preview greeting
	printf(
		"%s     ____ %s   %s%sSHE#GANG - powerful command interpreter (shell) for linux written in C%s\n"
		"%s  __/ / /_%s   %s%sBlazing fast, cool, simple shell in C%s\n"
		"%s /_  . __/%s   %s%sdeveloped by alxvdev%s\n"
		"%s/_    __/ %s   %s%shttps://github.com/alxvdev/shegang%s\n"
		"%s /_/_/    %s   %sMIT License%s\n\n", GREEN,
		RESET, GREEN, BOLD, RESET,
		GREEN, RESET, GREEN, ITALIC, RESET,
		GREEN, RESET, CYAN, DIM, RESET,
		GREEN, RESET, CYAN, UNDERLINE, RESET,
		GREEN, RESET, DIM, RESET
	);

	// infinite loop
	do {
		display_ps(); // output the shell prompt

		line = read_user_input(); // reading user output

		// go to the next iteration if NULL is returned
if (line == NULL) {
			free(line);
			continue;
		}

		// split user input into arguments and execute them
args = split_into_tokens(line);
		status = execute(args);

		// freeing up memory
		free(line);
		free(args);
	} while(status);

	return 0;
}
```

## Optional: connecting C code to Python via ctypes
I told you that I will also show you how to connect a binary library to python via ctypes. I'll show you a simple example:

```python
from pathlib import Path
import sys
import os
import ctypes


class LibShegang:
	"""
LibShegang class
"""
def __init__(self, filepath: str):
"""
		Initialization

:param filepath: Path to the library
		:type: str
		"""
		self.filepath = Path(filepath)

		if not self.filepath.exists():
			raise FileNotFoundError(f"Library at path {self.filepath} is not exists")
		else:
			self.filepath = Path(os.path.abspath(os.path.join(os.path.dirname(__file__), filepath)))

		self.cdll = ctypes.CDLL(self.filepath)

	def launch_shell(self):
		"""
		Run the shell (main function) directly in python
		"""
		print(f'\nPython implementation of shegang ({self.filepath})\n')
		self.cdll.main()


def main():
"""
Main function
"""
libshegang = LibShegang("bin/libshegang.so")
libshegang.launch_shell()


if __name__ == '__main__':
	main()
```

And this partially justifies the joke.:

 > Python is the largest C library

And as for me, it's cool. You can combine two popular programming languages and increase the possibilities.

# Conclusion
Thanks for reading! It was quite an interesting experience for me. I hope you enjoyed it.

If you have comments on the article or on the code, please write, there is probably a more experienced and professional C programmer who can help both the readers of the article and me.

Link to my command interpreter implementation repository [here](https://github.com/alxvdev/shegang ).

I will be glad if you join my small [telegram blog](https://t.me/hex_warehouse ).

## Sources

 + [Habr / History of the C language](https://habr.com/ru/articles/114588 /)
+ [IRunner / History of the programming language C](https://acm.bsu.by/wiki/C2018/%D0%98%D1%81%D1%82%D0%BE%D1%80%D0%B8%D1%8F_%D1%8F%D0%B7%D1%8B%D0%BA%D0%B0_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F_C )
+ [Metanit / C Data Types](https://metanit.com/c/tutorial/2.3.php )
+ [Metanit / Pointers in C](https://metanit.com/c/tutorial/5.1.php )
 + [Habr / Let's write a Linux command shell](https://habr.com/ru/articles/537156 /)
+ [Metanit / Memory allocation](https://metanit.com/c/tutorial/5.8.php )
