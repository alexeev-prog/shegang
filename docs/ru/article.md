# Программирование на C в Linux на примере создания своей командной оболочки
Привет, хабр! Язык программирования C - основополагающий, как я считаю. И его знать, нет, не обходимо, но довольно желательно. Большинство языков отсылают к C, и зная C (или C++) другой язык будет даваться намного легче.

Я решил недавно улучшить свой навык владения C, путем написания проектов. Самая первая мысль, которая пришла мне на ум - это командный интерпретатор, командная оболочка, shell проще говоря. А также я расскажу о системе сборки make, и о том, как правильно писать и документировать C-код.

В этом туториале я буду использовать компилятор CLANG, а не GCC, и расскажу его преимущества

Итак, для создания своего шелла на C для Linux вам понадобится простой советский копеечный...

---

Ссылка на мой репозиторий реализации командного интерпретатора [здесь](https://github.com/alxvdev/shegang).

C - один из самых знаменитых и довольно популярных (даже спустя более 30 лет) язык программирования, созданным Деннисом Ритчи, Кеном Томпсоном и Брайаном Керниганом (он приложил руку к популяризации языка). Несмотря на свою низкоуровневость, его до сих пор используют много где. Он быстрый и легкий, а также имеет относительно понятный синтаксис, что позволяет встраивать его куда угодно, да и в принципе на C можно сделать практически все.

# Краткая история C
Си — это инструмент, острый, как бритва: с его помощью можно создать и элегантную программу, и кровавое месиво.

Язык Си восходит корнями к языку ALGOL (расшифровывается как ALGorithmic Language), который был создан в 1958 году совместно с комитетом Европейских и Американских учёных в сфере компьютерных наук на встрече в 1958 в Швейцарской высшей технической школе Цюриха. Язык был ответом на некоторые недостатки языка FORTRAN и попыткой их исправить.

Вдохновлённые языком ALGOL-60, Математическая лаборатория Кембриджского Университета совместно с Компьютерным отделом Лондонского университета создали в 1963 году язык CPL (Combined Programming Language).

Язык CPL посчитали сложным, и в ответ на это Мартином Ричардсоном был создан в 1966 году язык BCPL, основное предназначение которого заключалось в написании компиляторов. Сейчас он практически не используется, но в своё время из-за хорошей портируемости он играл важную роль.

BCPL послужил предком для языка Би, разработанного в 1969 в уже знакомой всем AT&T Bell Telephone Laboratories, не менее знакомыми Кеном Томпсоном и Денсом Ритчи.

Язык программирования Си разрабатывался в период с 1969 по 1973 годы в лабораториях Bell Labs. C был назван именно C из-за того, что он считался продолжением языка B.

К 1973 году язык Си стал достаточно силён, и большая часть ядра UNIX, первоначально написанная на ассемблере PDP-11/20, была переписана на Си. Это было одно из самых первых ядер операционных систем, написанное на языке, отличном от ассемблера; более ранними были лишь системы Multics (написана на ПЛ/1) и TRIPOS (написана на BCPL).

По мере своей жизни появились мифы насчет причин разработки. Согласно одной из них разработка Си стала результатом того, что его будущие авторы любили компьютерную игру, подобную популярной игре Asteroids. Они уже давно играли в неё на главном сервере компании, который был недостаточно мощным и должен был обслуживать около ста пользователей. Томпсон и Ритчи посчитали, что им не хватает контроля над космическим кораблём для того, чтобы избегать столкновений с некоторыми камнями. Поэтому они решили перенести игру на свободный PDP-7, стоящий в офисе. Однако этот компьютер не имел операционной системы, что заставило их её написать. В конце концов, они решили перенести эту операционную систему ещё и на офисный PDP-11, что было очень тяжело, потому что её код был целиком написан на ассемблере. Было вынесено предложение использовать какой-нибудь высокоуровневый портируемый язык, чтобы можно было легко переносить ОС с одного компьютера на другой. Язык Би, который они хотели сначала задействовать для этого, оказался лишён функциональности, способной использовать новые возможности PDP-11. Поэтому они и остановились на разработке языка Си.

Существует и другая легенда. Первый компьютер, для которого была первоначально написана UNIX, предназначался для создания системы автоматического заполнения документов. Первая версия UNIX была написана на ассемблере. Позднее для того, чтобы переписать эту операционную систему, был разработан язык Си.

Первая книга, посвящённая языку Си была написана Керниганом и Ритчи в 1978 году и вышла в свет под названием «Язык программирования Си». Эта книга, в среде программистов более известная как «K&R», стала неофициальным стандартом языка Си.

Стандарт С99 сейчас в большей или меньшей степени поддерживается всеми современными компиляторами языка Си. В идеале, код написанный на Си с соблюдением стандартов и без использования аппаратно- и системно-зависимых вызовов, становился как аппаратно- так и платформенно-независимым кодом.

![C Programming Language](https://acm.bsu.by/w/images/1/1a/The_C_Programming_Language_preview.png)

Что написано на C:

 + Ядро Linux.
 + Ядро Windows.
 + Интерпретаторы Python, Perl, PHP, bash...
 + Классические UNIX-утилиты: grep, sed, awk, vim, ...
 + Привычные Windows-утилиты: PuTTY.
 + Системы контроля версий: git, SVN.
 + Web-серверы nginx, Apache.
 + СУБД: SQLite, MySQL (~30%), PostgreSQL (~85%).
 + Вычислительные инструменты: MATLAB, Mathematica, numpy, ...
 + Мультимедиа-библиотеки: ffmpeg, libjpeg, ...

# Краткие основы C
Переходим к некоторым основам Си. Если вы считаете что знаете си - можете пропустить эту часть и перейти к следующей. Здесь я не буду глубоко погружаться, просто вспомним базу.

Си - это строго типизированный язык программирования, что значит присутствие в нем типов данных. В переменную типа int нельзя записать double и т.д.

Существуют следующие типы данных:

 + char: представляет один символ. Занимает в памяти 1 байт (8 бит). Может хранить любое значение из диапазона от -128 до 127
 + unsigned char: представляет один символ. Занимает в памяти 1 байт (8 бит). Может хранить любой значение из диапазона от 0 до 255
 + signed char: то же самое, что и char
 + short: представляет целое число в диапазоне от –32768 до 32767. Занимает в памяти 2 байта (16 бит). Имеет псевдонимы short int, signed short и signed short int.
 + unsigned short: представляет целое число в диапазоне от 0 до 65535. Занимает в памяти 2 байта (16 бит).
 + int: представляет целое число. В зависимости от архитектуры процессора может занимать 2 байта (16 бит) или 4 байта (32 бита). Если брать основные платформы - 64-разрядные Windows, Linux (вместе с Android) и MacOS, то размер int составляет 4 байта. Диапазон предельных значений соответственно также может варьироваться от –32768 до 32767 (при 2 байтах) или от −2 147 483 648 до 2 147 483 647 (при 4 байтах) и выше. Имеет псевдонимы signed int и signed
 + unsigned int: представляет положительное целое число. В зависимости от архитектуры процессора может занимать 2 байта (16 бит) или 4 байта (32 бита), и из-за этого диапазон предельных значений может меняться: от 0 до 65535 (для 2 байт), либо от 0 до 4 294 967 295 (для 4 байт).
 + long: представляет целое число и занимает в памяти 4 байта (32 бита) или 8 байт (64 бита). В зависимости от размера может находиться в в диапазоне от -2 147 483 648 до 2 147 483 647 (4 байта), либо в диапазоне от -9223372036854775807 до +9 223 372 036 854 775 807 (8 байт). Если брать распростренные платформы, то на 64-разрядном Windows long занимает 4 байта, а на 64-разрядных Linux/MacOS - 8 байт. Имеет псевдонимы long int, signed long int и signed long.
 + unsigned long: представляет целое число и занимает в памяти 4 байта (32 бита) или 8 байт (64 бита). В зависимости от размера может находиться в в диапазоне от 0 до 4 294 967 295 (4 байта) или в диапазоне от 0 до 18 446 744 073 709 551 615 (8 байт).
 + long long: представляет целое число в диапазоне от -9223372036854775807 до +9 223 372 036 854 775 807. Занимает в памяти, как правило, 8 байт (64 бита).
 + unsigned long long: представляет целое число в диапазоне от 0 до 18 446 744 073 709 551 615. Занимает в памяти, как правило, 8 байт (64 бита).
 + float: представляет вещественное число одинарной точности с плавающей точкой в диапазоне +/- 3.4E-38 до 3.4E+38. В памяти занимает 4 байта (32 бита)
 + double: представляет вещественное число двойной точности с плавающей точкой в диапазоне +/- 1.7E-308 до 1.7E+308. В памяти занимает 8 байт (64 бита)
 + long double: представляет вещественное число двойной точности с плавающей точкой в диапазоне +/- 3.4E-4932 до 1.1E+4932. В памяти занимает 10 байт (80 бит). На некоторых системах может занимать 96 и 128 бит.
 + void: тип без значения

![Типы данных C](https://habrastorage.org/webt/zj/_x/p-/zj_xp-pzendwqs6wvrktm7npx-s.jpeg)

Вот как выглядит классический пример "Hello, World" на C:

```c
#include <stdio.h>

int main(void) {
	printf("Hello, World");

	return 0;
}
```

Функция `main` - это точка входа в программу. Строка `#include <stdio.h>` включает библиотеку (а точнее заголовочный файл), в данном случае stdio.h, в которой есть функция printf.

## Указатели
C - большей частью низкоуровневый язык программирования, поэтому здесь большую роль играет управление памятью. Одна из таких частей - это работа с указателями.

В каждой программе есть данные, и эти данные хранятся по определенному адресу в памяти. И поэтому существуют указатели - они указывают на область памяти, которой принадлежит та или иная переменная.

Итак, чтобы создать указатель, нужна следующая конструкция:

```c
тип_данных* название_указателя;

# например
int *first_number;
int second_number = 10;

first_number = &second_number;
```

Указатель хранит адрес объекта в памяти компьютера. И для получения адреса к переменной применяется операция &. Эта операция применяется только к таким объектам, которые хранятся в памяти компьютера, то есть к переменным и элементам массива.

Естественно, все типы данных должны совпадать.

Для того, чтобы узнать адрес переменной в памяти, можно его вывести через printf:

```c
#include <stdio.h>

int main(void) {
	int *first_number;
	int second_number = 10;

	first_number = &second_number;

	printf("%p\n", first_number); // выведет, например: 0x7ffce01f1a6c

	return 0;
}
```

Для адресов в памяти применяется шестнадцатеричная система. Фактически адрес представляет целочисленное значение, выраженное в шестнадцатеричном формате.

Но так как указатель хранит адрес, то мы можем по этому адресу получить хранящееся там значение, то есть значение переменной. Для этого применяется операция * или операция разыменования (dereference operator). Результатом этой операции всегда является объект, на который указывает указатель. Применим данную операцию и получим значение переменной x:

```c
#include <stdio.h>

int main(void) {
	int *first_number;
	int second_number = 10;

	first_number = &second_number;

	printf("%d\n", *first_number); // выведет 10

	return 0;
}
```

Значение указателя можно передать и в другую переменную:

```c
#include <stdio.h>

int main(void) {
	int *first_number;
	int second_number = 10;
	int last_number;

	first_number = &second_number;
	last_number = *first_number;

	printf("%d\n", last_number); // выведет 10

	return 0;
}
```

И можно также менять значения по адресу:

```c
#include <stdio.h>

int main(void) {
	int *first_number;
	int second_number = 10;

	first_number = &second_number;
	*first_number = 0;

	printf("%d\n", first_number); // выведет 0

	return 0;
}
```

## Управление памятью
Для управления динамическим выделением памяти используется ряд функций, которые определены в заголовочном файле stdlib.h:

`malloc()` — выделяет память заданного размера и возвращает указатель (void*) на память.

Выделяет память длиной в s байт и возвращает указатель на начало выделенной памяти. В случае неудачного выполнения возвращает NULL.

`calloc()` — выделяет пространство для элементов массива, инициализирует его до нуля и возвращает указатель на память.

Выделяет память для n элементов по m байт каждый и возвращает указатель на начало выделенной памяти. В случае неудачного выполнения возвращает NULL.

`free()` — освобождает ранее выделенное пространство.

Освобождает ранее выделенный блок памяти, на начало которого указывает указатель bl.

`realloc()` — изменяет размер ранее выделенного пространства.

Изменяет размер ранее выделенного блока памяти, на начало которого указывает указатель bl, до размера в ns байт. Если указатель bl имеет значение NULL, то есть память не выделялась, то действие функции аналогично действию malloc.

Больше вы можете прочитать в [этом хорошем туториале](https://metanit.com/c/tutorial/5.8.php).

---

Если хотите узнать больше, советую [эту серию уроков по языку си](https://metanit.com/c/tutorial/1.1.php) и [эту статью](https://habr.com/ru/articles/464075/).

# Компиляторы: gcc или clang
Я решил также сделать небольшое пояснение, почему я предпочел использовать clang, а не gcc. GCC обладает обширной поддержкой наследия идиом и конструкций из языка C, которые поддерживаются в современном C++. GCC даже сегодня многократно нарушает стандарты C++ из-за стратегии поддержки экзотической функциональности C в коде C++.

Clang же сегодня является идеальным вариантом следования стандартам. Он поддерживает все стандарты по всех деталях, обла1дает обширной системой статистической и динамической проверки года, банально тот же `clang format`. Также он дает доступ к дереву синтаксического разбора.

Также clang намного строже gcc - многое что может быть обычным предупреждением в gcc, может стать ошибкой. А также clang поддерживается многими бигтех-компаниями. И именно благодаря более свободной лицензии, чем GNU GPL (открытость не значит свобода).

Clang имеет модульную архитектура - отдельно парсер, отдельно оптимизатор и кодогенератор. Таким образом легко добавить поддержку новой архитектуры или языка программирования (например, раст работает на llvm). И благодаря этому легко можно создать язык программирования на основе llvm. Просто загуглите: how to create your own programming language llvm3

И также clang прямо с самого начала имел библиотеку libclang. Эту библиотеку используют 99% всех IDE для C++: KDevelop, Qt Creator, Clion.

Хотя моментами GCC более легок в применении чем clang, например во всяких проектах ОС (kolibri os, os/2) используется именно gcc.

CLANG/LLVM банально получился более подходящим под UNIX-WAY, ибо gcc оброс легаси-кодом, как не крути.

По оптимизации, clang и gcc примерно одинаковы, в одних сценариях clang ненамного быстрее, в других gcc также ненамного быстрее.

# Пишем свой командный интерпретатор
## Архитектура проекта
Самое главное в любом проекте - правильно реализовать его архитектуру, чтобы потом не мучиться. На самом деле, в составлении грамотной архитектуры нет ничего сложного - надо просто думать на шаг вперед. Продумывать то, если добавить какой-либо функционал, будет ли удобно его добавлять?

У меня получилась такая архитектура:

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

Давайте разберем подробнее. В директории build_scripts хранятся вспомогательные шелл-скрипты - такие как `build_make.sh` - для сборки проекта, `create_package.sh` - для создания тарболл-архива с бинарником, мануалом и т.д., `uninstall.sh` - для удаления и `update.sh` - для обновления.

Директория ctypes_lib нужна для примеров использования нашего шелла, но не как бинарник, а как shared object, библиотеку в python при помощи модуля ctypes. В ней находится пустая директория bin - именно туда при компиляции перемещается файл библиотеки.

Директория include нужна для заголовочных файлов модулей.

Директория src - основная, там находятся файлы исходного кода. В config находится код чтения конфигурации, в core базовые основные возможности, то есть чтение пользовательского ввода. В директории execution находятся файлы для создания процессов и базовые команды, а также встроенный функционал находится в поддиректории builtin. В features находятся некоторые классные фичи, в данном случае цвета и функции для более красивого и удобного вывода текста и сообщений.

Файл shegang.c - основной, в него импортируются все модули и он отвечает за запуск шелла.

CHANGELOG.md, LICENSE и README.md - это файлы для гитхиб-репозитория.

А вот Makefile - это файл инструкций по сборке для make, о чем мы сейчас и поговорим.

## Система сборки Make
Вручную водить команды для сборки не удобно, и поэтому были созданы системы сборки. Одной из самых старых, популярных и довольно простых является Make.

Основа сборки - это файл Makefile. Он имеет свой синтаксис и нужен для инструкций по сборке программ.

Вот что получилось у меня:

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

Разберем поэтапно:

```Makefile
BINARY_NAME = shegang
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
CTYPES_LIB_DIR = ctypes_lib/bin
```

Сверху вы видите, так сказать, переменные. Они указывают на пути к файлам - то есть название бинаря, директории с исходным кодом, директория для бинарников, директория для файлов заголовков, и директория для бинаря библиотеки.

```make
CC = clang
CFLAGS = -Wall -Wextra -Werror=implicit-function-declaration -Wpedantic -O2 -g -pipe -fno-fat-lto-objects -fPIC -mtune=native -march=native
CC_SO_FLAGS = -Wall -Wextra -shared -fPIC -O2 -std=c99 -lreadline
LDFLAGS = -L$(BIN_DIR) -lreadline
```

Сверху вы видите компилятор - мы используем clang - флаги к нему, а также флаги для создания библиотеки, а также флаги для LD, и мы указываем что подключаем библиотеку `readline`. Эта библиотека нужна для чтения пользовательского опыта, истории ввода.

```make
INCLUDE_PATHS = -I$(INCLUDE_DIR)

SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
OBJ_SO_FILES = $(patsubst $(SRC_DIR)/%.c, $(CTYPES_LIB_DIR)/%.o, $(SRC_FILES))
LIBRARY = $(CTYPES_LIB_DIR)/libshegang.so
```

И мы подходим к переменным для файлов и путей. Через конструкцию `$(<VAR>)` мы можем получить данные из переменной. Мы получаем флаг для инклуда заголовочных файлов (для компилятора). Файлы исходного кода мы получаем через команду. Для исполнения команды в начале надо использовать ключевое слово `shell`: `$(shell find $(SRC_DIR) -type f -name "*.c")`. Команда `find $(SRC_DIR) -type -f -name "*.c"` ищет все .C-файлы в директории исходного кода.

А вот в OBJ_FILES и OBJ_SO_FILES уже используется patsubst - банально для изменения путей файлов c-кода на .o файлы.

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

Выше просто куча алиасов полезных команд.

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

Выше самая основная часть - директивы, функции, команды. Структура команды такая:

```make
<цель>: <зависимости>
	# команды
```

То есть, например, при вводе `make clean` заработает директива clean. А вот команда build имеет зависимости, make ищет цели где есть эти зависимости и исполняет сначала ее, а потом только первоначальную функцию.

В некоторых функциях вы можете увидеть $@ и $<. Все просто - $@ это цель, а $< зависимость.

А знак @ перед командами нужен для скрытия команды, то есть чтобы сам текст команды не выводился.

Есть также функция all, она запускается если просто запустить `make`.

## Пишем код
Вот мы и достигли основной части статьи. Итак, основной минимальный функционал шелла:

 + чтение пользовательского ввода;
 + история команд;
 + вывод сообщений о неправильной команде или ошибке;
 + запуск процессов и задач по POSIX
 + чтение конфигурации для настройки шелла;
 + базовые встроенные команды;
 + настройка промпта шелла;

### Цвета, форматирование
Для начала файл с отсутствием зависимостей - то есть `colors.c`, для вывода цветов, форматирования сообщений и для вывода строки промпта шелла.

```c
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <time.h>
#include <wait.h>

// Макросы для ANSI кодов (форматирование). Названия макросов в процессе компиляции заменяются на их значения.

// Форматирование
#define RESET 			"\033[0m"
#define BOLD 			"\033[1m"
#define DIM 			"\033[2m"
#define ITALIC 			"\033[3m"
#define UNDERLINE 		"\033[4m"
#define BLINK 			"\033[5m"
#define REVERSE 		"\033[7m"
#define HIDDEN 			"\033[8m"
#define BOLD_ITALIC 	"\033[3;1m"

// Цвета
#define BLACK			"\033[0;30m"
#define RED				"\033[0;31m"
#define GREEN			"\033[0;32m"
#define YELLOW			"\033[0;33m"
#define BLUE			"\033[0;34m"
#define MAGENTA			"\033[0;35m"
#define CYAN			"\033[0;36m"
#define WHITE			"\033[0;37m"
#define GRAY			"\033[90m"

// Макросы для вывода сообщений, то есть их тип. Чтобы сделать код более читабельным.
#define DEBUG -1
#define INFO 0
#define WARNING 1
#define ERROR 2

// Макрос максимального пути директории
#define MAX_DIRECTORY_PATH 1024

// Это будет в разделе о загрузке конфига. То есть цвет имени пользователя, цвет текущей директории и цвет текущего времени
// это нужно для промпта
extern char* username_color;
extern char* pwd_color;
extern char* curr_time_color;

/**
 * @brief Функция по получению ANSI кода цвета по его названию
 * 
 * @param const char* color_name Название цвета
 * 
 * @return char* ANSI код цвета
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
 * @brief Простейшая функция для вывода текста с новой строкой
 * 
 * @param const char* message Текст сообщения 
 **/
void println(const char* message) {
	printf("%s\n", message);
}

/**
 * @brief Вывод текста с новой строкой в определенном цвете
 * 
 * @param const char* message Текст сообщения
 * @param char* message_color ANSI код цвета
 **/
void println_colored(const char* message, char* message_color) {
	printf("%s%s%s\n", message_color, message, RESET);
}

/**
 * @brief Вывод текста без новой строки в определенном цвете
 * 
 * @param const char* message Текст сообщения
 * @param char* message_color ANSI код цвета
 **/
void print_colored(const char* message, char* message_color) {
	printf("%s%s%s", message_color, message, RESET);
}

/**
 * @brief Функция для вывода сообщения-лога
 * 
 * @param const char* message Текст сообщения
 * @param int message_type Макрос цвета
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
		// Если тип сообщения это сообщение об ошибке, то выводим текст в stderr
		fprintf(stderr, "%s%s%s%s%s %s\n", RESET, color, format, msgtype_string, RESET, message);
	} else {
		printf("%s%s%s%s %s%s\n", RESET, color, format, msgtype_string, RESET, message);
	}

	printf(RESET);
}

/**
 * @brief Функция для вывода промпта шелла
 **/
void display_ps(void) {
	pid_t uid = geteuid(); // uid пользователя
	struct passwd *pw = getpwuid(uid); // структура пользователя
	char cwd[MAX_DIRECTORY_PATH]; // массив символов (строка) текущей директории
	time_t rawtime; // время в сыром виде
	struct tm * timeinfo; // структура информации о времени

	time(&rawtime); // получаем время
	timeinfo = localtime(&rawtime); // информация о локальном времени

	// первая строка
	printf("%s┬─%s%s[%s", DIM, RESET, GRAY, RESET);

	// если структура пользователя не пуста
	if (pw != NULL) {
		printf("%s%s%s:", username_color, pw->pw_name, RESET);
	}

	// если возможно получить текущую директорию
	if (getcwd(cwd, MAX_DIRECTORY_PATH) != NULL) {
		printf("%s%s%s]%s", pwd_color, cwd, GRAY, RESET);
	}

	printf("%s─%s", DIM, RESET);

	// получаем текущее время
	printf("%s[%s%s%d:%d:%d%s%s]%s", GRAY, RESET, curr_time_color, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, RESET, GRAY, RESET);

	/*
	пример промпта:

	┬─[alexeev:/home/alexeev/Desktop/Projects/shegang]─[20:0:55]
	╰─> $ 
	 */
}
```

### Чтение пользовательского ввода
Основной частью нашего шелла можно считать чтение пользовательского ввода, именно поэтому я вынес в основной базовый функционал.

```c
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "colors.h"

#define DEFAULT_BUFFER_SIZE 128 // размер буфера
#define TOKENS_DELIMETERS " \t" // разделить токенов

/**
 * @brief Функция для чтения пользовательского ввода (и запись его в историю)
 * 
 * @return char* Буфер ввода
 **/
char* read_user_input(void) {
	rl_getc_function = getc;
	fflush(stdout);

	// Буфер ввода (то есть получаем вывод через readline с новой строки с окрашиванием в цвета для красоты)
	char* input_buffer = readline("\n\033[2m╰─>\033[0m\033[0;36m $ \033[0m");

	// Если в строке отстутствие всего (ctrl+d, ctrl+c)
	if (input_buffer == NULL) {
		print_message("Failed to read user input", WARNING);
		return NULL;
	}

	// Если строка не пустая
	if (*input_buffer != '\0') {
		add_history(input_buffer);
	}

	return input_buffer;
}

/**
 * @brief Легаси метод чтения пользовательского ввода (без истории и без авто подставки)
 * 
 * @return char* строка введеная пользователем 
 **/
char* fgets_input(void) {
	size_t max_length = DEFAULT_BUFFER_SIZE; // максимальный размер буфера
	char* input_buffer = (char*)calloc(max_length, sizeof(char)); // выделяем память под буфер ввода

	print_message("This function is legacy. Use read_user_input instead of fgets_input", WARNING);

	// Если память не была выделена
	if (input_buffer == NULL) {
		print_message("\nFailed to allocate memory for input buffer", ERROR);
		return NULL;
	}

	// Обработчик исключений
	if (fgets(input_buffer, max_length, stdin) == NULL) {
		if (ferror(stdin)) {
			// Если нажато ctrl+c и ctrl+d
			print_message("\nFailed to read user input", ERROR);
		}

		// освобождаем память
		free(input_buffer);
		return NULL;
	}

	// проверка на новую строку
	char* newline = strchr(input_buffer, '\n');

	if (newline != NULL) {
		*newline = '\0';
	}

	return input_buffer;
}

/**
 * @brief Функция для разделения команды на массив данных (токенизация)
 * 
 * @param char* line Строка для разделения
 * @return char** 
 **/
char** split_into_tokens(char* line) {
	size_t position = 0; // позиция
	size_t buffer_size = DEFAULT_BUFFER_SIZE; // размер буфера

	char* token; // текущий токен
	char** tokens = (char**)malloc(sizeof(char*) * buffer_size); // выделяем память под токены

	// Если токены пусты или ошибка при выделении памяти
	if (tokens == NULL) {
		print_message("Couldn't allocate buffer for splitting", ERROR);
		return NULL;
	}

	// токенизируем строку
	token = strtok(line, TOKENS_DELIMETERS);

	// Разделение на токены
	while (token != NULL) {
		tokens[position++] = token;

		// если позиция больше или равна размеру буфера
		if (position >= buffer_size) {
			buffer_size *= 2;

			tokens = (char**)realloc(tokens, buffer_size * sizeof(char*));

			if (tokens == NULL) {
				print_message("Couldn't reallocate buffer for tokens", ERROR);
				return NULL;
			}
		}

		// задаем токен
		token = strtok(NULL, TOKENS_DELIMETERS);
	}

	tokens[position] = NULL;

	return tokens;
}
```

### Чтение конфигурации
Теперь займемся кодом для чтения конфигурации. На данный момент она простая и в ней находятся базовые настройки для промпта шелла.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "colors.h"

#define CONFIG_FILE				"/.shegangrc" // конфиг файл, / - это корень домашней директории пользователя
#define DEFAULT_USERNAME_COLOR  "\033[0;33m" // цвет юзернейма по умолчанию
#define DEFAULT_PWD_COLOR		"\033[0;32m" // цвет текущей директории по умолчанию
#define DEFAULT_CURR_TIME_COLOR "\033[0;35m" // цвет текущего времени по умолчанию

// Задаем переменные (в colors.c мы использовани extend для их глобализации)
char* username_color;
char* pwd_color;
char* curr_time_color;

/**
 * @brief Функция для загрузки конфига
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

	// читаем конфиг и задаем
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

### Выполнение задач
Сложная вещь, нужна для создания задач и процессов по posix.

Для этого нам нужно узнать базу. Начнем с fork - простыми словами системный вызов fork создает полный клон текущего процесса, отличаются они лишь своим идентификатором, т. е. pid.

Системный вызов exec заменяет текущий процесс сторонним. Естественно, сторонний процесс задается через параметры функции.

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

// Задаем функции (они нужны чтобы избежать варнингов)
int kill(pid_t pid, int);
int quit();

/**
 * Структура задачи заднего плана
 * 
 * @param pid_t PID залачи
 * @param int is_finished Логическое значение (0 или 1), завершена ли задача
 * @param char* timestamp Отметка времени
 * @param char* command Команда задачи
 **/
struct background_task_t {
	pid_t pid;
	int is_finished;
	char* timestamp;
	char* command;
};
typedef struct background_task_t bg_task;

/**
 * Структура задачи переднего плана
 * 
 * @param pid_t PID задачи
 * @param int is_finished Логическое значение (0 или 1), завершена ли задача
 **/
struct foreground_task_t {
	pid_t pid;
	int is_finished;
};
typedef struct foreground_task_t fg_task;

/**
 * Структура задач
 * 
 * @param fg_task foreground_task Структура задачи переднего плана
 * @param bg_task* background_task Структура задачи заднего плана
 * @param size_t cursor Курсор
 * @param size_t capacity Вместимость задачи
 **/
struct tasks_t {
	fg_task foreground_task;
	bg_task* background_task;
	size_t cursor;
	size_t capacity;
};
typedef struct tasks_t tasks;

/**
 * @brief глобальная структура задач
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
 * @brief Функция для того, чтобы задать задачу переднего плана
 * 
 * @param pid_t pid PID процесса
 **/
void set_foreground_task(pid_t pid) {
	tasks_structure.foreground_task.pid = pid;
	tasks_structure.foreground_task.is_finished = 0;
}

/**
 * @brief Функции для добавления задачи заднего плана
 * 
 * @param pid_t pid PID процесса
 * @param char* name Название задачи
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
 * @brief Функция для завершения задачи переднего плана
 **/
void kill_foreground_task(void) {
	if (tasks_structure.foreground_task.pid != -1) {
		kill(tasks_structure.foreground_task.pid, SIGTERM);
		tasks_structure.foreground_task.is_finished = 1;

		printf("\n");
	}
}

/**
 * @brief Функция для завершения задачи заднего плана
 * @param char** args Аргументы командной строки
 * 
 * @return int Код результата
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
 * @brief Функция для проверци, задача ли заднего плана
 * 
 * @param char** args Аргументы
 * 
 * @return int (bool) 1 если да, 0 если нет
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
 * @brief Функция для создания и запуска задачи
 * 
 * @param char** args Массив аргументов
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
 * @brief Функция для отметки законченных задач
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

### Встроенные функции и выполнение команд
Итак, здесь будут два файла. Первым мы разберем executor.c:

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

// статус код: 1 - продолжение работы, 0 - конец

/**
 * @brief Функция для вывода информации о шелле
 * 
 * @return int Статус код
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
 * @brief Функция для выхода и завершения всех задач
 * 
 * @return int Статус код
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
 * @brief Функция для вывода истории команд
 * 
 * @return int Статус код
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
 * @brief Функция для перезагрузки шелла (то есть перезагрузка конфига)
 **/
int reload_shell(void) {
	load_config();

	return 1;
}

/**
 * @brief Функция для встроенной функции shegang_config (вдохновлен fish_config из fish)
 * 
 * @param char **args Массив аргументов
 * 
 * @return int Статус кодов
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
 * @brief Функция для запуска команды
 * 
 * @return int Статус код
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

И builtin/base.c:

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
 * @brief Функция для смены директории
 * 
 * @param char** args Массив аргументов
 * 
 * @return int Статус код
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
 * @brief Функция для вывода задач заднего плана
 * 
 * @return int Статус код
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

### Соединяем все вместе
И финальный главный файл shegang.c:

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

// главная функция
int main(void) {
	char* line;
	char** args;
	int status;

	signal(SIGINT, kill_foreground_task);
	signal(SIGCHLD, mark_ended_task);

	load_config();

	// небольшое превью-приветствие
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

	// бесконечный цикл
	do {
		display_ps(); // выводим промпт шелла

		line = read_user_input(); // читаем пользовательский вывод

		// переходим на следующую итерацию если возвращено NULL
		if (line == NULL) {
			free(line);
			continue;
		}

		// сплитим пользовательский ввод на аргументы и исполняем их
		args = split_into_tokens(line);
		status = execute(args);

		// освобождаем память
		free(line);
		free(args);
	} while(status);

	return 0;
}
```

## Дополнительно: подключение C-кода в Python через ctypes
Я рассказывал, что также покажу как можно бинарную библиотеку подключить в python через ctypes. Покажу простой пример:

```python
from pathlib import Path
import sys
import os
import ctypes


class LibShegang:
	"""
	LibShegang класс
	"""
	def __init__(self, filepath: str):
		"""
		Инициализация
	
		:param filepath: Путь до библиотеки
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
		Запускаем шелл (main функцию) прямо в python
		"""
		print(f'\nPython implementation of shegang ({self.filepath})\n')
		self.cdll.main()


def main():
	"""
	Главная функция
	"""
	libshegang = LibShegang("bin/libshegang.so")
	libshegang.launch_shell()


if __name__ == '__main__':
	main()
```

И частично это оправдывает шутку:

 > Python - самая большая библиотека C

И как по мне, это круто. Можно два популярных языка программирования соединить, и увеличить возможности.

# Заключение
Спасибо за прочтение! Это был довольно интересный опыт для меня. Я надеюсь что вам понравилось.

Если у вас есть замечания по статье или по коду - пишите, наверняка есть более опытный и профессиональный программист на C, который может помочь как и читателям статьи, так и мне.

Ссылка на мой репозиторий реализации командного интерпретатора [здесь](https://github.com/alxvdev/shegang).

Буду рад если вы присоединитесь к моему небольшому [телеграм-блогу](https://t.me/hex_warehouse).

## Источники

 + [Habr / История языка C](https://habr.com/ru/articles/114588/)
 + [IRunner / История языка программирования C](https://acm.bsu.by/wiki/C2018/%D0%98%D1%81%D1%82%D0%BE%D1%80%D0%B8%D1%8F_%D1%8F%D0%B7%D1%8B%D0%BA%D0%B0_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F_C)
 + [Metanit / C Типы данных](https://metanit.com/c/tutorial/2.3.php)
 + [Metanit / Указатели в C](https://metanit.com/c/tutorial/5.1.php)
 + [Habr / Давайте напишем командную оболочку Linux](https://habr.com/ru/articles/537156/)
 + [Metanit / Выделение памяти](https://metanit.com/c/tutorial/5.8.php)
