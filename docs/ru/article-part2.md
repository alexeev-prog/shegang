# Программирование на C в Linux на примере создания командной оболочки: часть 2
Я решил недавно улучшить свой навык владения C, путем написания проектов. Самая первая мысль, которая пришла мне на ум - это командный интерпретатор, командная оболочка, shell проще говоря. А также я расскажу о системе сборки make, и о том, как правильно писать и документировать C-код.

В [первой части](https://habr.com/ru/companies/timeweb/articles/839890/) мы задали базовую структуру кода, разобрались с чтением вывода и созданием процессов. А в этой части нашей задачи будет дойти с альфа-стадии на бета-стадию - то есть реализовать прочий важный функционал, такой как: минимальная поддержка плагинов; автодополнение; подсветка синтаксиса; переменные окружения, новые встроенные утилиты.

Да-да, мы превратим наш велосипед в мопед! Я вынес из прошлой статьи итоги, и попытался решить все проблемы и замечания. Продолжаем погружение в пучины разработки под Linux!

---

Я прочитал ваши комментарии под прошлой статьей, за замечания и советы я выражаю благодарность следующим пользователям: BulldozerBSG (за советы и книгу "Linux Application Development 2ed, K.Johnson", а также за разьяснение стандарта), bigbamblbee (за разьянение на тему GCC vs CLANG), grdvo (за совет), checkpoint (за улучшение кода и советы), а также: domix32, ChePeter. А также спасибо всем остальным, кто прочел мою статью и отреагировал.

Несколько поправок: размерность базовых типов в стандарте C плавающая, и указано лишь отношение размерностей (int >= short, short >= char). Внезапно на некоторых платформах char может оказаться размером 32 бита. Про char лучше думать как о третьем типа наравне с unsigned char, signed char. К какому типу char будет приводиться - зависит от конкретной реализации компилятора под конкретную платформу.

Собственно, сам выбор компилятора зависит от заданных условий и ограничений.

А также, я много где поменял код. Где именно - вы увидите в статье и в [моем репозитории](https://github.com/alxvdev/shegang).

Для понимания кода и самой структуры проекта я настоятельно рекомендую прочитать [первую часть статьи](https://habr.com/ru/companies/timeweb/839890). В первой части я рассказал об истории языка C, базовых основах, и, собственно, базовая структура командного интерпретатора. В этой же части мы подвергнем нашу программу большим изменениям. Заинтригую: мы добавим автодополнение, подсветку синтаксиса, добавим базовые утилиты, улучшим структуру и чистоту кода. Пожалуй, данная статья претендует на место одной из лучших русскоязычных статей про написание консольных утилит (в частности командного интерпретатора). А может и нет.

Итак, начнем!

---

По моим канонам, я обычно начинаю с теории - и в этот раз я хочу побольше рассказать о процессе компиляции программы на C/C++. Эта информация взята из [моей статьи про формат ELF](https://habr.com/ru/companies/timeweb/articles/784534/). Я напоминаю, что вы ___можете пропустить этот раздел и перейти к следующему___.

# Процесс компиляции
Для того чтобы узнать, как устроен бинарный формат, давайте скомпилируем программу на C. Это поможет нам узнать, какие этапы проходит наша программа перед тем, как оказаться запущенной.

Двоичные файлы создаются в процессе компиляции, то есть трансляции понятного человеку исходного кода, например на языке программирования C/C++, в машинный код, исполняемый процессором.

![](https://habrastorage.org/r/w1560/getpro/habr/upload_files/b5b/abb/42c/b5babb42ce4b49dc40a972e5a1a35fe1.png)

Компиляция C/C++, или другого кода состоит из четырех этапов — препроцессирование, компиляция, ассемблирование и компоновка. На практике современные компиляторы часто объединяют некоторые или даже все этапы, но для демонстрации можно будет использовать их по отдельности.

## Этап препроцессирования
Давайте изучим этап препроцессирования. Процесс компиляции начинается с обработки нескольких файлов, которые вы хотите откомпилировать.

Хэдеры, включенные в программу с помощью директивы #include, рекурсивно проходят стадию препроцессинга и включаются в выпускаемый файл. Однако, каждый хэдер может быть открыт во время препроцессинга несколько раз, поэтому, обычно, используются специальные препроцессорные директивы, предохраняющие от циклической зависимости.

Препроцессор — это макро процессор, который преобразовывает вашу программу для дальнейшего компилирования. На данной стадии происходит работа с препроцессорными директивами. Например, препроцессор добавляет хэдеры в код (#include), убирает комментирования, заменяет макросы (#define) их значениями, выбирает нужные куски кода в соответствии с условиями #if, #ifdef и #ifndef.

Исходный файл может быть всего один, но крупные программы обычно состоят из большого количества файлов (это является хорошей практикой, чтобы главный файл не был засорен ненужным кодом).
Исходные C и C++ файлы могут содержать макросы (директивы #define) и директивы #include. Последний служат для включения библиотек и заголовочных файлов (с расширением .h), от которых зависит исходный файл. На этапе препроцессирования все директивы #define и #include расширяются, так что остается код на чистом C, подлежащий компиляции.

```cpp
#include <stdio.h> // подключаем хедер стандартной сишной библиотеки ввода-вывода

// Создаем макросы GREETING и MESSAGE
#define GREETING "Hello, %s" // макрос для printf-функции, который принимает %s - строку
#define MESSAGE "Habr" // макрос сообщения, в нашем случае - Habr

// Главная функция
int main() {
    printf(GREETING, MESSAGE); // выводим макросы

    return 0; // выходим из программы посредством возвращения нуля
}
```

Скоро мы увидим, что происходит на других этапах процесса компиляции, но пока рассмотрим только результат этапа препроцессирования. По умолчанию GCC выполняет все этапы компиляции разом, но существуют флаги для остановки компиляции на разных этапах. Для остановки компиляции на режиме препроцессирования нам нужно ввести команду `gcc -E -P <example.с> -o <example_processed.ii>`.

Разберем данную команду. Флаг -E требует остановиться после препроцессирования, а -P заставляет опустить отладочную информацию. Флаг -o означает, куда должен записаться результат. Ниже я приведу пример файла на этапе препроцессирования, для краткости измененный.

```cpp
typedef long unsigned int size_t;
typedef __builtin_va_list __gnuc_va_list;
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;

/* ... */

extern int __uflow (FILE *);
extern int __overflow (FILE *, int);

// Ага! Вот и наш код. Но немного измененный - нету макросов
int main() {
    printf("Hello, %s", "Habr");
    return 0;
}
```

Заголовочный файл stdio.h включен целиком, вместе со всеми определениями типов, глобальными переменными, прототипов функций — все это скопировано в главный файл. Поскольку это делается для каждой директивы #include, если подключить несколько библиотек или заголовочных файлов, результат будет очень длинный. Кроме того, препроцессор расширяет все макросы, определенные с помощью ключевого слова #define. В данном примере это означает, что оба аргумента printf (GREETING и MESSAGE) вычисляются и заменяются соответствующими строками.

## Этап компиляции
Итак, после того как мы получили код на C, нам нужно его скомпилировать. Но не сразу в двоичный исполняемый файл — а в ассемблерный код. После завершения препроцессирования исходный файл готов к компиляции. На этапе компиляции обработанный препроцессором код транслируется на язык ассемблера (Ассемблерный код — это доступное для понимания человеком представление машинного кода). Большинство компиляторов на этом этапе выполняют более или менее агрессивную оптимизацию, уровень которой задается флагами, в случае с gcc это флаги от -O0 до -O3.

Почему на этапе компиляции порождается код на ассемблере, а не машинный код? Это решение кажется бессмысленным в контексте одного конкретного языка (например, C), но обретает смысл, если вспомнить о других языках программирования. Из наиболее популярных компилируемых языков назовем C, C++, Common Lisp, Go и Haskell. Писать компилятор, который порождает машинный код для каждого из них, было бы чрезвычайно трудоемким и долгим занятием. Проще генерировать код на языке ассемблера и обрабатывать его на последнем этапе процесса одним и тем же ассемблером.

Таким образом, результатом этапа компиляции становится ассемблерный код, все еще понятый человеку, в котором вся символическая информация сохранена. Как уже было сказано, gcc обычно вызывает все этапы компиляции автоматически, поэтому, чтобы увидеть ассемблерный код, сгенерированный на этапе компиляции, нужно попросить gcc остановиться после этого этапа и сохранить ассемблерные файлы на диске. Для этого служит флаг -S (расширение .s традиционно используется для файлов на языке ассемблера, хотя довольно часто используют и просто .asm). Кроме того, передадим gcc флаг -masm=intel, который заставляет использовать язык ассемблера в синтаксисе Intel, а не AT&T. Синтаксис AT&T менее популярный и менее читаемый, по сравнению с Intel-овским синтаксисом. Итак, и вот команда для компиляции: `gcc -S -masm=intel <example.c> -o <example_asm>.s`.

Ниже я вставил краткую выдержку из ассемблерного файла нашего кода:

```asm
    .file   "hello.c"
    .intel_syntax noprefix
    .text
.Ltext0:
    ;;; /home/argentum/Coding/ELF-x86-research это моя директория для изучения ELF файлов, а 
    ;;; src/hello.c - директория с исходным кодом нашей программы
    .file 0 "/home/argentum/Coding/ELF-x86-research" "src/hello.c"
    .section    .rodata.str1.1,"aMS",@progbits,1
.LC0:
    .string "Habr"
.LC1:
    .string "Hello, %s"
    .section    .text.startup,"ax",@progbits
    .p2align 4,,10
    .p2align 3
    .globl  main
    .type   main, @function
```

Кстати, в процессе оптимизации кода ваш исходный код может немного измениться. Компилятор знает, что сложение стоит дешевле умножения, и если требуется умножить 2 на 2, то он вместо умножения просто сложит 2 и 2.

## Этап ассемблирования
Так как x86 процессоры исполняют команды на бинарном коде, необходимо перевести ассемблерный код в машинный с помощью ассемблера. Ассемблер преобразовывает ассемблерный код в машинный код, сохраняя его в объектном файле.

В конце этапа ассемблирования мы наконец-то получим настоящий, с пылу с жару, машинный код! На вход этого этапа поступают ассемблерные файлы, сгенерированные на этапе компиляции, а на выходе имеем набор объектных файлов, которые иногда называют модулями. Объектные файлы содержат машинные команды, которые в принципе могут быть выполнены процессором. Но, как мы скоро узнаем, прежде чем появится готовый к запуску исполняемый двоичный файл, необходимо проделать еще кое-какую работу. Обычно одному исходному файлу соответствует один ассемблерный файл, а одному ассемблерному файлу — один объектный. Чтобы сгенерировать объектный файл, нужно передать gcc флаг -c.

Объектный файл — это созданный ассемблером промежуточный файл, хранящий кусок машинного кода. Этот кусок машинного кода, который еще не был связан вместе с другими кусками машинного кода в конечную выполняемую программу, называется объектным кодом.

Далее возможно сохранение данного объектного кода в статические библиотеки для того, чтобы не компилировать данный код снова.

Чтобы убедиться, что сгенерированный объектный файл действительно объектный, можно воспользоваться утилитой file:

```bash
file <example_obj.o>

<example_obj.o>: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), with debug_info, not stripped
```

Первая часть нам говорит, что файл отвечает спецификации формата исполняемых двоичных файлов ELF. Точнее, это 64-разрядный ELF-файл, а буквы LSB означают, что при размещении чисел в памяти первым располагается младший байт (Least Significant Byte). Но самое главное здесь — слово relocatable (перемещаемый).

Перемещаемые файлы не привязаны к какому-то конкретному адресу в памяти, их можно перемещать, не нарушая никаких принятых в коде предположений. Увидев в выводе file слово relocatable, мы понимаем, что речь идет об объектном, а не исполняемом файле (но существуют также позиционно-независимые (перемещаемые) файлы, но о них file сообщает, что это разделяемые объекты, а не перемещаемые файлы. Отличить их от обыкновенных разделяемых библиотек можно по наличию адреса точки входа). Объектные файлы компилируются независимо, поэтому, обрабатывая один файл, ассемблер может не знать, какие адреса упоминаются в других объектных файлах. Именно поэтому объектные файлы должны быть перемещаемыми, тогда мы сможем скомпоновать их в любом порядке и получить полный исполняемый двоичный файл. Если бы объектные файлы не были перемещаемыми, то это было бы невозможно. Но на данном шаге еще ничего не закончено, ведь объектных файлов может быть много и нужно их все соединить в единый исполняемый файл с помощью компоновщика (линкера). Поэтому мы переходим к следующей стадии.

## Этап компоновки
Компоновка — последний этап процесса компиляции. На этом этапе все объектные файлы объединяются в один исполняемый двоичный файл. В современных системах этап компоновки иногда включает дополнительный проход, называемый оптимизацией на этапе компоновки (link-time optimization — LTO).

Неудивительно, что программа, выполняющая компоновку, называется компоновщиком (линкером). Обычно линкер отделен от компилятора, который выполняет все предыдущие этапы. Компоновщик (линкер) связывает все объектные файлы и статические библиотеки в единый исполняемый файл, который мы и сможем запустить в дальнейшем. Для того, чтобы понять как происходит связка, следует рассказать о таблице символов.

Таблица символов — это структура данных, создаваемая самим компилятором и хранящаяся в самих объектных файлах. Таблица символов хранит имена переменных, функций, классов, объектов и т.д., где каждому идентификатору (символу) соотносится его тип, область видимости. Также таблица символов хранит адреса ссылок на данные и процедуры в других объектных файлах.
Именно с помощью таблицы символов и хранящихся в них ссылок линкер будет способен в дальнейшем построить связи между данными среди множества других объектных файлов и создать единый исполняемый файл из них.

Как мы уже выяснили, объектные файлы перемещаемые, потому что компилируются независимо друг от друга, и компилятор не может делать никаких предположений о начальном адресе объектного файла в памяти. Кроме того, объектные файлы могут содержать ссылки на функции и переменные, находящиеся в других объектных файлах или внешних библиотеках. До этапа компоновки адреса, по которым будут размещены код и данные, еще неизвестны, поэтому объектные файлы содержат только перемещаемые символы, которые определяют, как в конечном итоге будут разрешены ссылки на функции и переменные. В контексте компоновки ссылки, зависящие от перемещающего символа, называются символическими ссылками. Если объектный файл ссылается на одну из собственных функций или переменных по абсолютному адресу, то такая ссылка то же будет символической.

Задача компоновщика — взять все принадлежащие программе объектные файлы и объединить их в один исполняемый файл, который, как правило, должен загружаться с конкретного адреса в памяти. Теперь, когда известно, из каких модулей состоит исполняемый файл, компоновщик может разрешить большинство символических ссылок. Но ссылки на библиотеки могут остаться неразрешенными — это зависит от типа библиотеки.

Статистические библиотеки (в Linux они обычно имеют расширение .a) включаются в исполняемый двоичный файл, поэтому ссылки на них можно разрешить окончательно. Но существуют также динамические (разделяемые) библиотеки, которые совместно используются всеми программами, работающими в системе. Иными словами, динамическая библиотека не копируется в каждый использующий ее двоичный файл, а загружаются в память лишь однажды, и все нуждающиеся в ней двоичные файлы пользуются этой разделяемой копией. На этапе компоновки адреса, по которым будут размещаться динамические библиотеки, еще неизвестны, поэтому ссылки на них разрешить невозможно. Поэтому компоновщик оставляет символические ссылки на такие библиотеки даже в окончательном исполняемом файле, и эти ссылки разрешаются, только когда двоичный файл будет загружен в память для выполнения. Большинство компиляторов, в т.ч. и gcc, автоматически вызывают компоновщик в конце процесса компиляции. Поэтому для создания полного двоичного исполняемого файла можно просто вызвать gcc без специальных флагов.

```bash
gcc <example.c> -o <example_bin>

# Давайте вызовем команду file к нашему двоичному исполняемому файлу
file <example_bin>

<example_bin>: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=1189a5182dc9274591321961fea250aa18893450, for GNU/Linux 3.2.0, with debug_info, not stripped
```

Утилита file сообщает, что мы имеем файл типа ELF 64-bit LSB pie executable. Добавилась аббревиатура PIE, что означает Position Independent Executable (исполняемый позиционно-независимый код). У вас может быть конечно и без PIE. Файл теперь исполняемый, а не перемещаемый, как после этапа ассемблирования. Важно также, что файл динамически скомпонован, то есть в нем используются библиотеки, не включенные в его состав, а разделенные с другими программами в системе. Наконец, слова interpreter /lib64/ld-linux-x86-64.so.2 говорит нам, какой динамический компоновщик будет использован для окончательного разрешения зависимостей от динамических библиотек на этапе загрузки исполняемого файла в память. Запустив двоичный файл, вы увидите, что он делает то, что и ожидалось. Рабочий двоичный файл! Какой огромный и сложный путь ради небольшой программы, верно?

# Подготовительные работы
Перед тем, как мы полноценно начнем писать код, надо подготовить окружение. То есть: настроить систему сборки, дополнительные скрипты и библиотеки. И начнем мы с библиотек - я решил использовать вместо библиотеки GNU readline использовать [isocline](https://github.com/daanx/isocline). В этой библиотеки присутствует много уже готового базового функционала: подсветка синтаксиса, автодополнение и история. Я долго искал замену readline, и как раз нашел эту библиотеку. Согласно описанию, она состоит из 8к строк кода на чистом C!

Скомпилировать ее в .a файл можно следующим путем:

```bash
git clone https://github.com/daanx/isocline
mkdir -p build/release
cd build/release
cmake ../..
cmake --build .
mv libisocline.a ../../../ # Переносим файл библиотеки в корневую директорию проекта
```

Или вы можете скачать [готовую скомпилированную мной библиотеку](https://github.com/alxvdev/shegang/raw/main/libisocline.a).

Теперь займемся Makefile - системой сборки:

```make
# Директории и название бинаря
BINARY_NAME = shegang
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
CTYPES_LIB_DIR = ctypes_lib/bin

# Компиляторы и флаги
CC = clang
CFLAGS = -Wall -Wextra -Werror=implicit-function-declaration -Wpedantic -O2 -g -pipe -fno-fat-lto-objects -fPIC -mtune=native -march=native
CC_SO_FLAGS = -Wall -Wextra -shared -fPIC -O2 -std=c11
LDFLAGS = -L$(BIN_DIR)

# Файлы и пути исходных кодов: файлы заголовков, исходного кода, объектные
INCLUDE_PATHS = -I$(INCLUDE_DIR)

SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
OBJ_SO_FILES = $(patsubst $(SRC_DIR)/%.c, $(CTYPES_LIB_DIR)/%.o, $(SRC_FILES))
LIBRARY = $(CTYPES_LIB_DIR)/libshegang.so

# та самая библиотека
LIB_ISOCLINE = libisocline.a

# Команды
SUDO		  	= sudo
DEL_FILE      	= rm -f
CHK_DIR_EXISTS	= test -d
MKDIR     	= mkdir -p
COPY      	= cp -f
COPY_FILE     	= cp -f
COPY_DIR      	= cp -f -R
INSTALL_FILE   	= install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   	= cp -f -R
DEL_FILE      	= rm -f
SYMLINK       	= ln -f -s
DEL_DIR       	= rmdir
MOVE      	= mv -f
TAR       	= tar -cf
COMPRESS      	= gzip -9f
LIBS_DIRS     	= -I./include/
SED       	= sed
STRIP     	= strip

# когда вводим просто make
all: $(BIN_DIR)/$(BINARY_NAME)

# make build
build: $(BIN_DIR)/$(BINARY_NAME)

# make example_plugin. Будем компилировать пример плагина. Да-да, в этой части статьи мы реализуем базовую систему загрузки плагинов
example_plugin:
	@echo "Compile example plugin"
	@$(CC) $(CC_SO_FLAGS) plugins/pipegang_plugin.c -o pipegang_plugin.so

# установка в /usr/local/bin/shegang
install: $(BIN_DIR)/$(BINARY_NAME)
	$(SUDO) $(INSTALL_PROGRAM) $(BIN_DIR)/$(BINARY_NAME) /usr/local/bin/

# удаление
remove:
	$(SUDO) $(DEL_FILE) /usr/local/bin/$(BINARY_NAME)

# компилируем .so библиотеку для ctypes
ctypes: $(LIBRARY)
	@$(DEL_FILE) $(CTYPES_LIB_DIR)/*.o

$(LIBRARY): $(OBJ_SO_FILES)
	@mkdir -p $(CTYPES_LIB_DIR)
	@$(CC) $(INCLUDE_PATHS) $(CC_SO_FLAGS) -o $@ $^

$(CTYPES_LIB_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(INCLUDE_PATHS) $(CC_SO_FLAGS) -c -o $@ $<

# Компиляция программы
$(BIN_DIR)/$(BINARY_NAME): $(OBJ_FILES)
	@echo "CC 		| $@"
	@mkdir -p $(BIN_DIR)
	@$(CC) $(LDFLAGS) $(INCLUDE_PATHS) -o $@ $(OBJ_FILES) $(LIB_ISOCLINE)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC 		| $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE_PATHS) -c $< -o $@ $(LIB_ISOCLINE)

# Очистка
clean:
	@echo "Clean..."
	@rm -rf $(BIN_DIR)
	@rm -rf $(CTYPES_LIB_DIR)/*

reinstall: clean all

.PHONY: all build example_plugin ctypes clean reinstall
```

У нас появилась новая метка - компиляция базового плагина. В этой части мы создадим простую систему загрузки и создания плагинов.

Последний шаг - это структура, архитектура проекта:

```
.
├── build_scripts
│   ├── build_make.sh
│   ├── create_package.sh
│   ├── uninstall.sh
│   └── update.sh
├── CHANGELOG.md
├── config/
    ├── .sgaliases
    └── .shegangrc
├── ctypes_lib
│   ├── bin
│   ├── README.md
│   └── shegang.py
├── docs
│   ├── en
│   │   ├── article.md
│   │   ├── changes.md
│   │   ├── index.md
│   │   └── project_architecture.md
│   ├── preview.gif
│   └── ru
│       ├── article.md
│       ├── article-part2.md
│       ├── index.md
│       └── project_architecture.md
├── include
│   ├── autocomplete.h
│   ├── builtin.h
│   ├── colorschemes.h
│   ├── colors.h
│   ├── config.h
│   ├── environment.h
│   ├── executor.h
│   ├── fuzzy.h
│   ├── isocline.h
│   ├── plugins_manager.h
│   ├── shell_api.h
│   ├── tasks_processing.h
│   └── userinput.h
├── libisocline.a
├── LICENSE
├── Makefile
├── pipegang_plugin.so
├── plugins
│   └── pipegang_plugin.c
├── README.md
├── shegang_en.man
└── src
    ├── config
    │   └── config.c
    ├── core
    │   └── userinput.c
    ├── execution
    │   ├── builtin
    │   │   ├── base.c
    │   │   ├── gapf.c
    │   │   ├── lsblk.c
    │   │   ├── ls.c
    │   │   └── shegang_config.c
    │   ├── executor.c
    │   └── tasks_processing.c
    ├── features
    │   ├── autocomplete.c
    │   ├── colors.c
    │   ├── colorschemes.c
    │   ├── environment.c
    │   └── fuzzysearch.c
    ├── plugins
    │   └── plugins_manager.c
    └── shegang.c
```

Дерево проекта, как вы заметили, сильно увеличилось. Начнем сверху вниз:

 + build_scripts - вспомогательные баш-скрипты для сборки (`build_make.sh`), создания архива программы (`create_package.sh`), удаления (`uninstall.sh`) и обновления (`update.sh`).
 + CHANGELOG.md - файл изменений и истории коммитов git.
 + config - директория с примерами конфигурации шелла:

```bash
# .sgaliases
ls=sgls

# .shegangrc
USERNAME_COLOR=YELLOW
PWD_COLOR=GREEN
TIME_COLOR=MAGENTA
COLORSCHEME=gruvbox
```

 + ctypes_lib - то же самое что и в первой части статьи, ничего пока не изменилось.
 + docs - большая директория с документацией. Кстати, вы можете увидеть текстовые версии этих статей в этой директории.
 + include - директория для заголовочных файлов: builtin.h (все C-файлы из директории src/builtin), shell_api.h - файл с макросами (а в будущем возможно там будут храниться общие функции), isocline.h - заголовочный файл библиотеки isocline ([здесь](https://github.com/alxvdev/shegang/raw/main/include/isocline.h)). Остальные соответствуют файлам исходного кода.
 + libisocline.a - библиотека isocline.
 + LICENSE - лицензия.
 + Makefile - файл сборки.
 + pipegang_plugin.so - файл плагина (именно в виде shared object они и подключаются к нашему шеллу).
 + plugins - исходный код плагинов.
 + shegang_en.man - файл для утилиты man.
 + src - директория исходного кода. Поддиректория config имеет файл config.c для чтения конфигурации; поддиректория core имеет файл userinput.c для чтения пользовательского ввода и других базовых функций; поддиректория execution самая обширная, она имеет executor.c для выполнения команд, tasks_processing.c для запуска процессов, поддиректорию builtin с встроенными утилитами, такие, как: ls (sgls), lsblk (sglsblk), shegang_config, gapf (аналог cat) и base.c с базовыми командами; директория features где находятся важные фичи, такие как автодополнение и подсветка (autocomplete.c), colors.c для цветного вывода, colorschemes.c для цветовых схем подсветки синтаксиса, environment для работы с переменными окружения, fuzzysearch для fuzzy finding команд (если пользователь ввел несуществующую команду); директория plugins с менеджером плагинов. Также сам главный файл shegang.c

Итак, начнем с самого простого файла - `include/shell_api.h`. Это заголовочный файл с макросами:

```cpp
#ifndef SHELL_API_H
#define SHELL_API_H

#define CONFIG_FILE                 "/.shegangrc" // файл конфигурации (~/.shegangrc)
#define CONFIG_ALIASES              "/.sgaliases" // файл для алиасов (~/.sgaliases)
#define DEFAULT_USERNAME_COLOR          "\033[0;33m" // цвет юзернейма в промпте по умолчанию
#define DEFAULT_PWD_COLOR               "\033[0;32m" // цвет текущей директории в промпте по умолчанию
#define DEFAULT_CURR_TIME_COLOR         "\033[0;35m" // цвет текущего времени в промпте по умолчанию

#define MAX_LINE_LENGTH             256
#define MAX_ALIASES                 128

#define DEFAULT_BUFFER_SIZE             256
#define TOKENS_DELIMETERS               " \t" // разделитель для токенизации введеного текста

#define MAX_FILE_NAME_LENGTH            1024 // максимальная длина имени файла (для sgls (ls))
#define MAX_GRID_COLUMNS            3 // максимальное количество колонок

#define INIT_CAPACITY               1024 // начальная длина массива системных бинарей (для автодополнения)
#define MAX_PROGRAMS                4096 // максимальная длина массива
#define BIN_DIR                 "/bin" // директория системных бинарей

// цвета и стили
#define RESET                   "\x1b[0m"
#define BOLD                    "\x1b[1m"
#define DIM                     "\x1b[2m"
#define ITALIC                  "\x1b[3m"
#define UNDERLINE                   "\x1b[4m"
#define BLINK                   "\x1b[5m"
#define REVERSE                 "\x1b[7m"
#define HIDDEN                  "\x1b[8m"

#define BLACK                   "\x1b[30m"
#define RED                     "\x1b[31m"
#define GREEN                   "\x1b[32m"
#define YELLOW                  "\x1b[33m"
#define BLUE                    "\x1b[34m"
#define MAGENTA                  "\x1b[35m"
#define CYAN                    "\x1b[36m"
#define WHITE                   "\x1b[37m"
#define GRAY                    "\x1b[90m"

// для функции print_message из colors.c
#define DEBUG                   -1
#define INFO                    0
#define WARNING                 1
#define ERROR                   2

#define MAX_DIRECTORY_PATH              2048 // Максимальная длина директории

#define MAX_SCHEME_NAME_LENGTH          64 // максимальная длина имени цветовой схемы
#define MAX_COLOR_NAME_LENGTH           64 // максимальная длина названия цвета в цветовой схеме
#define MAX_COLOR_HEX_LENGTH            8 // максимальная длина цвета (hex) в цветовой схеме

#endif // SHELL_API_H
```

Следующий шаг, как и в прошлой статье - модуль `colors.c`:

```cpp
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include "shell_api.h"

extern char* username_color;
extern char* pwd_color;
extern char* curr_time_color;

int global_status_code = 0; // глобальная переменная статус кода

/**
 * @brief      Получение цвета по его имени
 *
 * @param[in]  color_name  Имя цвета
 *
 * @return     ANSI код цвета
 */
char* get_color_by_name(const char* color_name) {
    struct {char *text, *code; } colors[] = {
        {.text="RESET"  , .code = RESET},
        {.text="BLACK"  , .code = BLACK},
        {.text="RED"    , .code = RED},
        {.text="GREEN"  , .code = GREEN},
        {.text="YELLOW" , .code = YELLOW},
        {.text="BLUE"   , .code = BLUE},
        {.text="MAGENTA", .code = MAGENTA},
        {.text="CYAN"   , .code = CYAN},
        {.text="WHITE"  , .code = WHITE},
        {.text="GRAY"   , .code = GRAY}
    };

    const int len = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < len; ++i) {
        if (strcmp(color_name, colors[i].text) == 0) {
            return colors[i].code;
        }
    }

    return NULL;
}

/**
 * @brief      Простейшая функция для вывода сообщения с новой строки
 *
 * @param[in]  message  Сообщение
 * @param      const  char* message Сообщение для вывода
 */
void println(const char* message) {
    printf("%s\n", message);
}

/**
 * @brief      Простейшая функция для цветного вывода сообщения с новой строки
 *
 * @param[in]  message    Сообщение
 * @param      message_color  Цвет сообщения
 * @param      const  char* message Сообщение для вывода
 * @param      char*  message_color ANSI цвет сообщения
 */
void println_colored(const char* message, char* message_color) {
    printf("%s%s%s\n", message_color, message, RESET);
}

/**
 * @brief      Простейшая функция для цветного вывода сообщения без новой строки
 *
 * @param[in]  message    Сообщение
 * @param      message_color  Цвет сообщения
 * @param      const  char* message Сообщение для вывода
 * @param      char*  message_color ANSI цвет сообщения
 */
void print_colored(const char* message, char* message_color) {
    printf("%s%s%s", message_color, message, RESET);
}

/**
 * @brief      Функция для вывода форматированного сообщения
 *
 * @param[in]  message       Сообщение
 * @param[in]  message_type  The message type
 * @param      const  char* message Текст сообщения
 * @param      int    message_type Макрос цвета
 *
 * @return     void
 */
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
            format = BOLD;
            msgtype_string = "[ERROR]";
            break;
        default:
            color = WHITE;
            format = RESET;
            msgtype_string = "[DEFAULT]";
            break;
    }

    if (message_type == ERROR) {
        fprintf(stderr, "%s%s%s%s%s %s\n", RESET, color, format, msgtype_string, RESET, message);
    } else {
        printf("%s%s%s%s %s%s\n", RESET, color, format, msgtype_string, RESET, message);
    }

    printf(RESET);
}

/**
 * display_ps

 @brief      Функция для вывода промпта шелла
*/
void display_ps(int status) {
    pid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    char cwd[MAX_DIRECTORY_PATH];
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("%s┬─%s%s[%s", DIM, RESET, GRAY, RESET);

    if (pw != NULL) {
        printf("%s%s%s:", username_color, pw->pw_name, RESET); // имя пользователя
    }

    if (getcwd(cwd, MAX_DIRECTORY_PATH) != NULL) {
        printf("%s%s%s]%s", pwd_color, cwd, GRAY, RESET); // текущая директория
    }

    printf("%s─%s", DIM, RESET);

    printf("%s[%s%s%d:%d:%d%s%s] %s", GRAY, RESET, curr_time_color, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, RESET, GRAY, RESET); // время

    // статус код
    if (global_status_code != 0) {
        printf("%s%d ✘%s", RED, global_status_code, RESET);
    } else if (global_status_code == 0 && status != 1) {
        printf("%s%d ✘%s", RED, status, RESET);
    }

    // ресет статус кода
    global_status_code = 0;
}
```

Следующим шагом будет чтение пользовательского ввода - файл `core/userinput.c`. Там также находится функции токенизации сообщения:

```cpp
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "isocline.h"
#include "colors.h"
#include "autocomplete.h"
#include "shell_api.h"

/**
 * @brief      Вспомогательная функция получения строки из файла по ее номеру
 *
 * @param      filename  Путь до файла
 * @param[in]  line_n    Номер строки
 *
 * @return     строка
 */
char* r_line(char* filename, int line_n) {
    FILE* file = fopen(filename, "r");
    char line[256];

    char *data = malloc(sizeof(char) * 256);

    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        i++;

        if (i == line_n)
            strcpy(data, line);
    }

    fclose(file);

    return data;
}

/**
 * @brief      Функция для получения пути до файла истории
 *
 * @return     путь до файла истории
 */
char* get_history_file_path(void) {
    char* home_dir = getenv("HOME");

    if (home_dir == NULL) {
        return NULL;
    }

    char* history_file_path = (char*)malloc(1024);

    if (history_file_path == NULL) {
        return NULL;
    }

    snprintf(history_file_path, 1024, "%s/.sghistory", home_dir);

    return history_file_path;
}

/**
 * @brief      Настройка isocline
 */
void setup_isocline(void) {
    ic_style_def("kbd", "white underline"); // keybinding
    ic_style_def("ic-prompt", "gray"); // промпт

    printf("%s\n", get_history_file_path()); // выводим путь до истории
    ic_set_history(get_history_file_path(), 1024); // задаем файл истории с максимальной длиной 1024 команды

    ic_set_default_completer(&completer, NULL); // задаем функцию дополнения (будет потом)
    ic_set_default_highlighter(highlighter, NULL); // функция подсветки (будет потом)

    ic_enable_auto_tab(true); // auto tab
    ic_set_hint_delay(100); // задержка переж подсказкой в мс
    ic_enable_brace_matching(true); // brace matchong
    ic_enable_brace_insertion(true); // brace insertion
    ic_enable_inline_help(true); // inline help
}

/**
 * @brief      Функция для чтения пользовательского ввода
 *
 * @return     char* буфер ввода
 */
char* read_user_input(void) {
    printf("\n");

    char* input_buffer = ic_readline("╰─");

    // If input buffer is not plain string
    if (input_buffer == NULL) {
        print_message("Failed to read user input", WARNING);
        return NULL;
    }

    // if input buffer is not empty
    if (*input_buffer != '\0') {
        ic_history_add(input_buffer);
    }

    return input_buffer;
}

/**
 * @brief      токенизация строки по разделителям
 *
 * @param      line   линия
 * @param      delims  строка с разделителями
 *
 * @return     массив команд
 */
char** split_by_delims(char* line, const char* delims, int *num_tokens) {
    char **tokens = NULL;
    char *token;
    int i = 0;

    char *line_copy = malloc(strlen(line) + 1);
    strcpy(line_copy, line);

    token = strtok(line_copy, delims);

    while (token != NULL) {
        tokens = realloc(tokens, (i + 1) * sizeof(char *));
        tokens[i] = malloc(strlen(token) + 1);
        strcpy(tokens[i], token);
        i++;
        token = strtok(NULL, delims);
    }

    *num_tokens = i;
    free(line_copy);
    return tokens;
}

/**
 * @brief      токенизация команды
 *
 * @param      char* line Строка
 *
 * @return     char**
 */
char** split_into_tokens(char* line) {
    size_t position = 0;
    size_t buffer_size = DEFAULT_BUFFER_SIZE;

    char* token;
    char** tokens = (char**)malloc(sizeof(char*) * buffer_size);

    // если строка пустая
    if (tokens == NULL) {
        print_message("Couldn't allocate buffer for splitting", ERROR);
        return NULL;
    }

    // токенизация
    token = strtok(line, TOKENS_DELIMETERS);

    while (token != NULL) {
        tokens[position++] = token;

        if (position >= buffer_size) {
            buffer_size *= 2;

            tokens = (char**)realloc(tokens, buffer_size * sizeof(char*));

            if (tokens == NULL) {
                print_message("Couldn't reallocate buffer for tokens", ERROR);
                return NULL;
            }
        }

        token = strtok(NULL, TOKENS_DELIMETERS);
    }

    tokens[position] = NULL;

    return tokens;
}
```

Теперь нам предстоит создать модуль для автодополнения и подсветки синтаксиса - autocomplete.c:

```cpp
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "isocline.h"
#include "colorschemes.h" // реализуем позже, это модуль цветовых схем
#include "tasks_processing.h" // уже реализовали в прошлой статье, но мы ее немного изменим
#include "shell_api.h"

extern ColorScheme* current_theme; // расширяем видимость переменной цветовой схемы. Опять же, реализация будет ниже.

// массив со встроенными утилитами
const char* builtin_utils[] = {"shegang_config", "sgls", "sglsblk", "help", "exit", 
                "term", "reload", "history", "gapf", "quit", "bg", "term", 
                "environment", "sghint", "echon", "sghint", NULL};

/**
 * @brief      Получаем системные бинари
 *
 * @return     Массив с системными бинарями
 */
char **get_sys_binaries(void) {
    DIR *dir;
    struct dirent *entry;
    char **progs = malloc(INIT_CAPACITY * sizeof(char *));
    size_t count = 0, capacity = INIT_CAPACITY;

    // открываем директорию с бинарями
    dir = opendir(BIN_DIR);
    if (dir == NULL) {
        fprintf(stderr, "Ошибка открытия каталога %s: %s\n", BIN_DIR, strerror(errno));
        return NULL;
    }

    // читаем файлы в директории и пополняем массив
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        char path[128];
        snprintf(path, 128, BIN_DIR "/%s", entry->d_name);
        struct stat st;
        if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR)) {
            if (count >= capacity) { // если начальный размер массива превышен, расширим его
                capacity *= 2;
                progs = realloc(progs, capacity * sizeof(char *));
                if (progs == NULL) {
                    fprintf(stderr, "Ошибка выделения памяти: %s\n", strerror(errno));
                    closedir(dir);
                    return NULL;
                }
            }

            progs[count++] = strdup(entry->d_name);
        }
    }

    // встраиваем в массив встроенные утилиты
    for (int i = 0; i < sizeof(builtin_utils) / sizeof(builtin_utils[0]); i++) {
        if (builtin_utils[i] == 0)
            break;

        progs[count++] = strdup(builtin_utils[i]);
    }

    // завершаем массив
    progs = realloc(progs, (count + 1) * sizeof(char *));
    progs[count] = NULL;

    if (closedir(dir) != 0) {
        fprintf(stderr, "Ошибка закрытия каталога %s: %s\n", BIN_DIR, strerror(errno));
    }

    return progs;
}

/**
 * @brief      автодополнение по словам
 *
 * @param      cenv  Completion Env
 * @param[in]  word  Слово
 */
void word_completer(ic_completion_env_t* cenv, const char* word) {
    ic_add_completions(cenv, word, get_sys_binaries()); // подключаем системные бинари для автодополнения

    if (strcmp(word, "s") == 0) { // если пользователь ввел "s" то добавляем подсказки
        ic_add_completion(cenv, "sgls");
        ic_add_completion(cenv, "sghint");
        ic_add_completion(cenv, "sglsblk");
        ic_add_completion(cenv, "shegang_config");
        ic_add_completion(cenv, "su");
        ic_add_completion(cenv, "sudo");
    } else if (strcmp(word, "sghint") == 0 || strcmp(word, "sghint ") == 0) {
        // sghint - встроенная утилита для просмотра коротких подсказок о встроенных командах
        char* command_completion = (char*)malloc(1024);

        // проходимся по всем возможным вариантам и добавляем подсказки
        for (int i = 0; i < sizeof(builtin_utils) / sizeof(builtin_utils[0]); i++) {
            if (builtin_utils[i] == NULL) {
                break;
            }

            snprintf(command_completion, 1024, "sghint %s", builtin_utils[i]);

            ic_add_completion(cenv, command_completion);
        }
    } else if (strcmp(word, "shegang") == 0) {
        ic_add_completion(cenv, "shegang_config");
    } else if (strcmp(word, "t") == 0) {
        ic_add_completion(cenv, "top");
        ic_add_completion(cenv, "time");
        ic_add_completion(cenv, "touch");
        ic_add_completion(cenv, "tar");
        ic_add_completion(cenv, "touch");
        ic_add_completion(cenv, "tail");
        ic_add_completion(cenv, "term");
    } else if (strcmp(word, "e") == 0) {
        ic_add_completion(cenv, "environment");
    }
}

/**
 * @brief      функция автодополнения для isocline
 *
 * @param      cenv   cenv
 * @param[in]  input  ввод
 */
void completer(ic_completion_env_t* cenv, const char* input) {
    ic_complete_filename(cenv, input, 0, ".;/usr/local;c:\\Program Files", NULL); // автодополнение файлов

    ic_complete_word(cenv, input, &word_completer, NULL); // и автодополнение по фразам
}

/**
 * @brief      Подсветка синтаксиса
 *
 * @param      henv   highlight env
 * @param[in]  input  Ввод
 * @param      arg    Аргумент
 */
void highlighter(ic_highlight_env_t* henv, const char* input, void* arg) {
    long len = (long)strlen(input);

    for (long i = 0; i < len;) {
        const char* keywords[] = {"then", "if", "fi", "else", "do", "while", "function", "return", "not", "null",
                                "false", "true", "and", "or", NULL}; // кейворды
        const char* commands[] = {"bash", "diff", "cat", "chown", "chmod", "chgrp", "cp", "dd", "du", "df", "ln",
                                "ls", "mkdir", "mkfifo", "mv", "rm", "head", "tail", "md5sum", "basename",
                                "chroot", "date", "echo", "env", "nice", "nohup", "sleep", "printf",
                                "find", "xargs", "awk", "grep", "grub", "gzip", "unzip", "zip", "tar",
                                "ssh", "telnet", "time", "make", "gcc", "clang", "gdb", "rmdir", "mkdir",
                                "cd", NULL}; // команды
        const char* operators[] = {"==", ">=", "<=", "!=", ">", "<", "+", "-", "*", "/", "&&", "||", NULL}; // операторы

        long tlen; // длина

        if ((tlen = ic_match_any_token(input, i, &ic_char_is_idletter, keywords)) > 0) {
            ic_highlight(henv, i, tlen, get_color(current_theme, "keywords").hex); // если фраза есть в массиве то подсвечиваем его. get_color это функция из colorschemes.c, а current_theme это глобальная переменная текущей цветовой схемы. "keywords" название цвета, а .hex для получения hex-значения цвета (например #ff5555)
            i += tlen;
        } else if ((tlen = ic_match_any_token(input, i, &ic_char_is_idletter, commands)) > 0) {
            ic_highlight(henv, i, tlen, get_color(current_theme, "commands").hex); // команды
            i += tlen;
        } else if ((tlen = ic_match_any_token(input, i, &ic_char_is_idletter, builtin_utils)) > 0) {
            ic_highlight(henv, i, tlen, get_color(current_theme, "programs").hex); // программы
            i += tlen;
        } else if ((tlen = ic_is_token(input, i, &ic_char_is_digit)) > 0) { // если это цифра
            ic_highlight(henv, i, tlen, get_color(current_theme, "digits").hex); // цифры
            i += tlen;
        } else if ((tlen = ic_is_token(input, i, &ic_char_is_separator)) > 0) { // сеператор
            ic_highlight(henv, i, tlen, get_color(current_theme, "light_gray").hex); // светло серый
            i += tlen;
        } else if (ic_starts_with(input + i, "#")) { // комментарии #
            tlen = 1;
            while (i + tlen < len && input[i + tlen] != '\n') {
                tlen++;
            }
            ic_highlight(henv, i, tlen, get_color(current_theme, "dark_gray").hex); // темно серый
            i += tlen;
        } else if (ic_starts_with(input + i, "//")) { // комментарии //
            tlen = 2;
            while (i + tlen < len && input[i + tlen] != '\n') {
                tlen++;
            }
            ic_highlight(henv, i, tlen, get_color(current_theme, "dark_gray").hex); // темно серый
            i += tlen;
        } else {
            if ((tlen = ic_match_any_token(input, i, &ic_char_is_nonwhite, operators)) > 0) {
                ic_highlight(henv, i, tlen, get_color(current_theme, "operators").hex); // операторы
                i += tlen;
            } else {
                ic_highlight(henv, i, 1, NULL); // иначе без подсветки
                i++;
            }
        }
    }
}
```

Для более полного понимания кода автодополнения, в особенности подсветки синтаксиса, реализуем colorschemes.c:

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_api.h"

// структура Color. name - название, hex - значение
typedef struct {
    char name[MAX_SCHEME_NAME_LENGTH];
    char hex[MAX_COLOR_HEX_LENGTH];
} Color;

// Структура цветовой схемы, название и цвета
typedef struct {
    char name[MAX_SCHEME_NAME_LENGTH];
    Color blue, red, orange, purple, yellow, green, cyan, white, black, light_gray, dark_gray;
    Color keywords, operators, digits, strings, comments, programs, commands;
} ColorScheme;

// Gruvbox
ColorScheme gruvbox = {
    .name = "gruvbox",
    .blue = {"blue", "#83a598"},
    .red = {"red", "#fb4934"},
    .orange = {"orange", "#fe8019"},
    .purple = {"purple", "#d3869b"},
    .yellow = {"yellow", "#fabd2f"},
    .green = {"green", "#b8bb26"},
    .cyan = {"cyan", "#8ec07c"},
    .white = {"white", "#edbbb2"},
    .black = {"black", "#282828"},
    .light_gray = {"light_gray", "#b9aa95"},
    .dark_gray = {"dark_gray", "#504945"},
    .keywords = {"keywords", "#83a598"},
    .operators = {"operators", "#fb4934"},
    .digits = {"digits", "#b8bb26"},
    .strings = {"strings", "#b8bb26"},
    .comments = {"comments", "#a89984"},
    .programs = {"programs", "#d3869b"},
    .commands = {"commands", "#fe8019"}
};

// onedark
ColorScheme onedark = {
    .name = "gruvbox",
    .blue = {"blue", "#61afef"},
    .red = {"red", "#e06c75"},
    .orange = {"orange", "#d19a66"},
    .purple = {"purple", "#c678dd"},
    .yellow = {"yellow", "#e5c07b"},
    .green = {"green", "#98c379"},
    .cyan = {"cyan", "#56b6c2"},
    .white = {"white", "#abb2bf"},
    .black = {"black", "#282c34"},
    .light_gray = {"light_gray", "#9aa1ae"},
    .dark_gray = {"dark_gray", "#3e4452"},
    .keywords = {"keywords", "#61afef"},
    .operators = {"operators", "#e06c75"},
    .digits = {"digits", "#98c379"},
    .strings = {"strings", "#98c379"},
    .comments = {"comments", "#9aa1ae"},
    .programs = {"programs", "#c678dd"},
    .commands = {"commands", "#d19a66"}
};

// текущая цветовая схема
ColorScheme* current_theme = &gruvbox;

/**
 * @brief      Задаем цветовую схему
 *
 * @param      scheme  The scheme
 */
void set_color_scheme(ColorScheme* scheme) {
    current_theme = scheme;
}

/**
 * @brief      Получаем цветовую схему по имени
 *
 * @param      name  Имя цвета
 */
void get_scheme_by_name(char* name) {
    if (strcmp(name, "gruvbox") == 0) {
        set_color_scheme(&gruvbox);
        return;
    } else if (strcmp(name, "onedark") == 0) {
        set_color_scheme(&onedark);
        return;
    }
    
    set_color_scheme(&gruvbox);
}

/**
 * @brief      Получаем цвет
 *
 * @param      scheme  Цветовая схема
 * @param      name    Название
 *
 * @return     цвет (Color)
 */
Color get_color(ColorScheme* scheme, char* name) {
    struct {char *text; Color color; } colors[] = {
        {.text="blue"       , .color = scheme->blue},
        {.text="red"        , .color = scheme->red},
        {.text="orange"     , .color = scheme->orange},
        {.text="yellow"     , .color = scheme->yellow},
        {.text="green"      , .color = scheme->green},
        {.text="cyan"       , .color = scheme->cyan},
        {.text="white"      , .color = scheme->white},
        {.text="black"      , .color = scheme->black},
        {.text="light_gray" , .color = scheme->light_gray},
        {.text="dark_gray"  , .color = scheme->dark_gray},
        {.text="keywords"   , .color = scheme->keywords},
        {.text="operators"  , .color = scheme->operators},
        {.text="purple"     , .color = scheme->purple},
        {.text="digits"     , .color = scheme->digits},
        {.text="strings"    , .color = scheme->strings},
        {.text="comments"   , .color = scheme->comments},
        {.text="programs"   , .color = scheme->programs},
        {.text="commands"   , .color = scheme->commands},
    };

    const int len = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < len; ++i) {
        if (strcmp(name, colors[i].text) == 0) {
            return colors[i].color;
        }
    }

    return (Color){.name = "invalid", .hex="#FFFFFF"};
}
```

Итак, код явно можно сократить. Предлагаю сделать это вам, и написать в комментарии или отправить в [github репозиторий](https://github.com/alxvdev/shegang) исправленный вариант.

Следующими двумя шагами мы закончим работу над директорией src/features. Если вы помните структуру проекта, то могли заметить, что осталось два нереализованных модуля - environment.c и fuzzysearch.c. Начнем с environment.c. Он содержит в себе пару вспомогательных команд для получения и создания переменных окружения.

```cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int setenv(const char* envname, const char*envval, int overwrite);

/**
 * @brief      Получаем значение переменной окружения
 *
 * @param[in]  variable_name  Название переменной
 *
 * @return     Значение переменной окружения
 */
char *get_environment_variable(char* variable_name) {
    char* variable_value = getenv(variable_name);

    if (variable_value == NULL)
        return NULL;

    return variable_value;
}

/**
 * @brief      Создание переменной окружения
 *
 * @param[in]  variable_name   Имя перменной
 * @param[in]  variable_value  Значение переменной
 *
 * @return     string Статус
 */
char* set_environment_variable(char* variable_name, const char* variable_value) {
    int status = setenv(variable_name, variable_value, 1);

    if (status == 1) {
        return "ERROR";
    } else {
        return "SUCCESS";
    }
}
```

Теперь можно взяться за последний модуль в директории feature - fuzzysearch.c.

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "fuzzy.h"

/**
 * @brief      Fuzzy search
 *
 * @param[in]  text         The text
 * @param[in]  query        The query
 * @param[in]  build_score  The build score
 * @param      score        The score
 * @param      score_len    The score length
 *
 * @return     { description_of_the_return_value }
 */
int fuzzy_search(const char *text, const char *query, bool build_score, int **score, size_t *score_len) {
    size_t total_score = 0;
    if (build_score) { // Build score is an optimization when searching through large database
        (*score) = (int*)malloc(sizeof(int) * strlen(text));
        memset(*score, 0, sizeof(int)*strlen(text));
        *score_len = strlen(text);
    }

    size_t first_character_boosts = 1;

    for (size_t t_idx = 0; t_idx < strlen(text); t_idx++) {
        char t = tolower(text[t_idx]); // NOTE(deter0): to lower performs kind of strangely probably due to UTF8
        for (size_t q_idx = 0; q_idx < strlen(query); q_idx++) {
            char q = tolower(query[q_idx]);

            if (t == q) {
             // Start of word awards more but falls off fast
            if (t_idx == 0 || (t_idx > 0 && isspace(text[t_idx - 1]))) {
                int factor = 8/(first_character_boosts++);
                
                if (build_score) (*score)[t_idx] += factor;
                total_score += factor;
            } else { 
                if (build_score) (*score)[t_idx]++;
                total_score++;
            }
            
            size_t streak = 0;
            for (size_t s_idx = 1; s_idx < strlen(query)-q_idx; s_idx++) {
                char sq = tolower(query[q_idx + s_idx]);
                char st = tolower(text[t_idx + s_idx]);

                if (sq != st) break;
                    streak++;
                
                // Beginning of string yields few points more; eg -> "Term" :: "Terminus", "Fluent Terminal"
                if (((float)t_idx / (float)strlen(text)) <= 0.35) {
                    streak++;
                }

                int factor = streak*3/(strlen(query)*0.2);
                if (build_score)
                    (*score)[t_idx + s_idx] += factor;
                total_score += factor;
            }

            // (N * (N + 1) ) /2
            // (*score)[t_idx] += (streak * (streak + 1)) / 2;
            t_idx += streak;
            }
        }
    }

    return total_score;
}
```

Теперь мы перейдем к директории config, а точнее, к одноименному и единственному на этот момент файлу, config.c. Он отвечает за конфигурацию. Пока он дубовый и наипростейший, без широких настроек:

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "colors.h"
#include "colorschemes.h"
#include "shell_api.h"

char* username_color;
char* pwd_color;
char* curr_time_color;

typedef struct {
    char name[64];
    char command[256];
} Alias;

Alias aliases[MAX_ALIASES];
int num_aliases = 0;

/**
 * load_config
 * 
 * @brief      Загрузка алиасов. Алиасы записываются в отдельный файл (.sgaliases) и записываются в формате ALIAS=COMMAND
 */
void load_config_aliases(void) {
    char *home_dir = getenv("HOME");
    
    char config_path[strlen(home_dir) + strlen(CONFIG_ALIASES) + 1];
    sprintf(config_path, "%s%s", home_dir, CONFIG_ALIASES);
    FILE* config_file = fopen(config_path, "r");

    if (!config_file) {
        return;
    }

    FILE *fp;
    char alias_file[strlen(home_dir) + 11];
    sprintf(alias_file, "%s%s", home_dir, CONFIG_ALIASES);

    fp = fopen(alias_file, "r");

    if (fp == NULL) {
        print_message("Could not open config", ERROR);
        return;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL && num_aliases < MAX_ALIASES) {
        char *alias_name = strtok(line, "=");
        char *alias_cmd = strtok(NULL, "\n");

        if (alias_name != NULL && alias_cmd != NULL) {
            strncpy(aliases[num_aliases].name, alias_name, sizeof(aliases[num_aliases].name) - 1);
            strncpy(aliases[num_aliases].command, alias_cmd, sizeof(aliases[num_aliases].command) - 1);
            num_aliases++;
        }
    }

    fclose(fp);
}

/**
 * load_config

 @brief      Загрузка конфига
*/
void load_main_config(void) {
    char* home_dir = getenv("HOME");
    char line[256];

    username_color = DEFAULT_USERNAME_COLOR;
    pwd_color = DEFAULT_PWD_COLOR;
    curr_time_color = DEFAULT_CURR_TIME_COLOR;

    if (!home_dir) {
        return;
    }

    // составляем путь до конфига (.shegangrc)
    char config_path[strlen(home_dir) + strlen(CONFIG_FILE) + 1];
    sprintf(config_path, "%s%s", home_dir, CONFIG_FILE);

    FILE* config_file = fopen(config_path, "r");

    if (!config_file) {
        username_color = DEFAULT_USERNAME_COLOR;
        pwd_color = DEFAULT_PWD_COLOR;
        return;
    }

    // читаем конфиг
    while (fgets(line, sizeof(line), config_file)) {
        char* key = strtok(line, "=");
        char* value = strtok(NULL, "\n");

        if (key && value) {
            if (strcmp(key, "USERNAME_COLOR") == 0) { // цвет пользователя
                username_color = get_color_by_name(value);
            } else if (strcmp(key, "PWD_COLOR") == 0) { // цвет текущей директории
                pwd_color = get_color_by_name(value);
            } else if (strcmp(key, "TIME_COLOR") == 0) { // цвет времени
                curr_time_color = get_color_by_name(value);
            } else if (strcmp(key, "COLORSCHEME") == 0) { // цветовая схема (название)
                get_scheme_by_name(value);
            }
        }
    }

    fclose(config_file);
}
```

# Самая обширная часть
Мы переходим к самой обширной части - директории execution. Здесь вы можете увидеть:

```bash
src/execution/
├── builtin
│   ├── base.c # базовые встроенные утилиты
│   ├── gapf.c # gapf - аналог cat
│   ├── lsblk.c # sglsblk - простейший аналог lsblk
│   ├── ls.c # sgls - аналог ls, но с цветным выводом и иконками Nerd Font Icons
│   └── shegang_config.c # встроенная утилита shegang_config для настройки текущей сессии шелла
├── executor.c # модуль, который отвечает за определение и запуск команд
└── tasks_processing.c # запуск процессов
```

Начнем с tasks_processing, как наиболее основного модуля.

Но для реализации нам нужно узнать базу. Начнем с fork - простыми словами системный вызов fork создает полный клон текущего процесса, отличаются они лишь своим идентификатором, т. е. pid:

```cpp
#include <unistd.h>
#include <stdio.h>
#include <wait.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Я дочерний процесс с pid=0");
    } else {
        printf("Я родительский процесс с pid=%d", pid);
        wait(NULL);
    }

    return 0;
}
```

Системый вызов fork создает клон процесса, то есть родительский и дочерний. Порядок выполнения процессов не указан, поэтому родительский процесс ожидает завершения дочернего (`wait(NULL);`).

Системный вызов exec заменяет текущий процесс сторонним. Естественно, сторонний процесс задается через параметры функции.

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("echo", "echo", "I", "Love", "C", NULL);
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }

    return 0;
}
```

Данная программа выведет "I Love C". Как это произошло? Все просто: родительский процесс ожидает завершения дочернего, а дочерний, в свою очередь, заменяет себя на команду "echo I Love C".

Теперь можно начать писать код. В принципе, ничего почти не изменилось с прошлого раза, но мы добавили использование fuzzy search и global status code из colors.c:

```cpp
#include <wait.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "colors.h"
#include "executor.h"
#include "autocomplete.h"
#include "fuzzy.h"

// Functions definitions
int kill(pid_t pid, int);

extern int global_status_code; // расширяем зону видимости переменной

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
 * @brief      FZF для поиска похожей команды
 *
 * @param      args  Аргументы
 */
void fuzzy_search_valid_command(char** args) {
    int *score = NULL;
    size_t score_len = 0;
    int max_score = 0;
    const char *text = "N/A";
    const char** apps = get_sys_binaries(); // список системных бинарей из /bin (или другой заданной директории)

    for (size_t i = 0; i < sizeof(apps)/sizeof(apps[0]); i++) {
        int *score_tmp;
        size_t score_len_tmp;
        
        int total_score = fuzzy_search(apps[i], args[0], true, &score_tmp, &score_len_tmp);

        if (total_score > max_score) {
            if (score) free(score);
            
            score = score_tmp;
            score_len = score_len_tmp;
            text = apps[i];

            max_score = total_score;
        } else {
            free(score_tmp);
        }
    }

    if (max_score > 0) // Если максимальное количество "очков" схожести больше ноля, то уведомляем пользователя о
        // последней подходящей команде
        printf("Did you mean the command %s%s%s?\n", BOLD, text, RESET);
}

/**
 * @brief      Функция для создания и запуска процессов и задач
 *
 * @param      char**  args Массив аргументов
 *
 * @return     { description_of_the_return_value }
 */
int launch_task(char** args) {
    pid_t pid;
    int background = is_background_task(args);

    pid = fork();

    if (pid < 0) {
        print_message("Couldn't create child process!", ERROR);
    } else if (pid == 0) {

        if (execvp(args[0], args) != 0) { // команда исполнилась неудачно
            print_message("Couldn't execute unknown command!", ERROR);
            fuzzy_search_valid_command(args); // ищем возможную команду
            global_status_code = -1; // статус код из colors.c для промпта шелла
        }
        
        exit(-1);
    } else {
        if (background == 1) {
            if (add_background_task(pid, args[0]) == -1) {
                quit_from_shell(args);
            }
        } else {
            set_foreground_task(pid);

            if (waitpid(pid, NULL, 0) == -1) {
                if (errno != EINTR) {
                    print_message("Couldn't track the completion of the process", WARNING);
                    global_status_code = -1;
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

Главное изменение это добавление функции fuzzy_search_valid_command и изменение launch_task:

```cpp
/**
 * @brief      FZF для поиска похожей команды
 *
 * @param      args  Аргументы
 */
void fuzzy_search_valid_command(char** args) {
    int *score = NULL;
    size_t score_len = 0;
    int max_score = 0;
    const char *text = "N/A";
    const char** apps = get_sys_binaries(); // список системных бинарей из /bin (или другой заданной директории)

    for (size_t i = 0; i < sizeof(apps)/sizeof(apps[0]); i++) {
        int *score_tmp;
        size_t score_len_tmp;
        
        int total_score = fuzzy_search(apps[i], args[0], true, &score_tmp, &score_len_tmp);

        if (total_score > max_score) {
            if (score) free(score);
            
            score = score_tmp;
            score_len = score_len_tmp;
            text = apps[i];

            max_score = total_score;
        } else {
            free(score_tmp);
        }
    }

    if (max_score > 0) // Если максимальное количество "очков" схожести больше ноля, то уведомляем пользователя о
        // последней подходящей команде
        printf("Did you mean the command %s%s%s?\n", BOLD, text, RESET);
}

/**
 * @brief      Функция для создания и запуска процессов и задач
 *
 * @param      char**  args Массив аргументов
 *
 * @return     { description_of_the_return_value }
 */
int launch_task(char** args) {
    pid_t pid;
    int background = is_background_task(args);

    pid = fork();

    if (pid < 0) {
        print_message("Couldn't create child process!", ERROR);
    } else if (pid == 0) {

        if (execvp(args[0], args) != 0) { // команда исполнилась неудачно
            print_message("Couldn't execute unknown command!", ERROR);
            fuzzy_search_valid_command(args); // ищем возможную команду
            global_status_code = -1; // статус код из colors.c для промпта шелла
        }
        
        exit(-1);
    } else {
        if (background == 1) {
            if (add_background_task(pid, args[0]) == -1) {
                quit_from_shell(args);
            }
        } else {
            set_foreground_task(pid);

            if (waitpid(pid, NULL, 0) == -1) {
                if (errno != EINTR) {
                    print_message("Couldn't track the completion of the process", WARNING);
                    global_status_code = -1;
                }
            }
        }
    }

    return 1;
}
```

Перейдем к executor.c. В этом модуле будет включено много модулей из директории builtin, но не бойтесь, мы их реализуем сразу после этого.

```cpp
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <linenoise.h>

// Все написанные нами модули
#include "colors.h"
#include "tasks_processing.h"
#include "userinput.h"
#include "config.h"
#include "builtin.h"
#include "executor.h"
#include "environment.h"
#include "isocline.h"

extern tasks tasks_structure; // расширяем структуру задач

extern Alias aliases[MAX_ALIASES]; // алиасы
extern int num_aliases; // число алиасов

extern int global_status_code; // статус код

/**
 * @brief      Функция для вывода информации о shegang и некоторых встроенных утилитах
 *
 * @return     int статус код
 */
int help(char** args) {
    println("she#gang Linux Shell in C  @  by alxvdev\n");

    if (args[1] == NULL) {
        printf(
            "Built-in shell special functions:\n\n"
            
            " cd     <path>           - Change the directory\n"
            " term   <bg_task_idx>    - Kill background task by id\n"
            " help                    - Prints info about she#gang\n"
            " bg                      - Prints list with background tasks\n"
            " quit/exit               - Terminate shell with all active tasks\n"
            " reload                  - Reload the shell and config\n"
            " shegang_config          - Configurate the some config values in current session without saving (help exists)\n"
            " history                 - Print the shell commands history\n"
            " sgls                    - `ls` command analogue, with colors and nerd icons (help exists)\n"
            " sglsblk                 - `lsblk` command analogue\n"
            " environment             - Set and get env vars\n"
            " echon                   - `echo -n` command analogue\n"
            " sghint                  - small util for view built-in command hints\n"
            " gapf                    - `cat` command simple analogue\n\n"

            " Additional arguments: shegang_config, sgls; environment\n"
        );
    } else if (strcmp(args[1], "shegang_config") == 0) {
        println("Built-in function `shegang_config`\n");
        printf(
            "set <VAR> <VALUE>           - set value for var (ex. set USERNAME_COLOR RED)\n"
            "\nExisting variables: USERNAME_COLOR; PWD_COLOR; TIME_COLOR;\n"
            "Existing colors: RED, GREEN, BLUE, YELLOW, MAGENTA, GRAY, BLACK, WHITE, CYAN\n"
        );
    } else if (strcmp(args[1], "sgls") == 0) {
        println("Built-in function `sgls`\n");
        printf(
                "SGLS - A `ls` command analogue from shegang\n\n"
                "-t             Show time\n"
                "-a             Show hidden\n"
                "-l             Show as list\n"
                "-p             Show permissions\n"
        );
    } else if (strcmp(args[1], "environment") == 0) {
        println("Built-in function `sgls`\n");
        printf(
            "get <VAR>                   - get value of var (ex. get USER)\n"
            "set <VAR> <VALUE>           - set value for var (ex. set GANG SHELL)\n"
        );
    }

    return 1;
}

/**
 * @brief      Выход из шелла и завершение всех задач
 *
 * @return     int Статус код
 */
int quit_from_shell(char** args) {
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
 * @brief      Функция для вывода истории ввода или ее очистки
 *
 * @return     int Статус код
 */
int print_history(char** args) {
    if (args[1] != NULL && strcmp(args[1], "clear") == 0) { // если пользователь ввел `history clear`, то есть args[2] == clear
        printf("Clean...\n");
        ic_history_clear(); // очистка
        return 1;
    }

    // читаем файл истории
    FILE* file = fopen(get_history_file_path(), "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char line[256];
    int lineNumber = 1;

    // выводим
    while (fgets(line, sizeof(line), file)) {
        printf("%3d: %s", lineNumber, line);
        lineNumber++;
    }

    fclose(file);

    return 1;
}

/**
 * @brief      Перезагрузка шелла (конфигурации)
 */
int reload_shell(char** args) {
    load_main_config();
    load_config_aliases();

    return 1;
}

/**
 * @brief      Выполнение алиаса
 *
 * @param      args  Аргументы
 *
 * @return     статус код
 */
int execute_alias(char **args) {
    for (int i = 0; i < num_aliases; i++) {
        if (strcmp(aliases[i].name, args[0]) == 0) {
            char cmd[strlen(aliases[i].command)];
            sprintf(cmd, "%s", aliases[i].command);
            printf("alias %s=%s\n\n", aliases[i].name, aliases[i].command); // уведомляем что это алиас
            
            char** command_tokens = split_into_tokens(cmd);

            return execute(command_tokens);
        }
    }

    return 0;
}

/**
 * @brief      встроенная утилита для работы с переменными окружения (модуль environment.c)
 *
 * @param      args  The arguments
 *
 * @return     status code
 */
int environment(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        global_status_code = -1;
        println("Usage: environment <get> <var> OR environment <set> <var> <value>");
        return 1;
    }

    if (strcmp(args[1], "get") == 0) {
        printf("%s\n", get_environment_variable(args[2]));
        return 1;
    } else if (strcmp(args[1], "set") == 0 && args[2] != NULL && args[3] != NULL) {
        char* result = set_environment_variable(args[2], args[3]);
        printf("ENV: %s\n", result);
        return 1;
    }

    println("Usage: environment <get> <var> OR environment <set> <var> <value>");
    return 1;
}

/**
 * @brief      Аналог команды echo (echon), с поддержкой переменных окружения (echon Hi, $USER выведет Hi, <имя юзера>)
 *
 * @param      args  Аргументы
 *
 * @return     Статус код
 */
int echo_n(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        if (strncmp(args[i], "$", 1) == 0) {
            char* env_var = get_environment_variable(args[i] + 1);

            if (env_var != NULL) {
                printf("%s", env_var);
            } else {
                printf("Error: environment variable '%s' not found\n", args[i] + 1);
            }
        } else {
            printf("%s", args[i]);
            if (args[i + 1] != NULL) {
                printf(" ");
            }
        }
    }

    printf("\n");

    return 1;
}

/**
 * @brief      Функция для исполнения команды
 *
 * @param      Аргументы
 *
 * @return     int Статус код
 */
int execute(char** args) {
    struct { char *text, *hint; int arguments_count; int (*command)(char**); } commands[] = {
        //  Команда      Минимальное количество аргументов      Подсказка для команды                Указатель функции
        {.text="cd"             , .arguments_count=1, .hint="Change directory: cd <dir>",       .command=&change_directory},
        {.text="exit"           , .arguments_count=0, .hint="Quit/exit from shell: exit",       .command=&quit_from_shell},
        {.text="help"           , .arguments_count=0, .hint="Help command. Support args.",      .command=&help},
        {.text="bg"             , .arguments_count=0, .hint="Print background tasks",           .command=&bg_tasks},
        {.text="term"           , .arguments_count=1, .hint="Terminate background task",        .command=&term_background_task},
        {.text="history"        , .arguments_count=0, .hint="Shows or clean command history",   .command=&print_history},
        {.text="reload"         , .arguments_count=0, .hint="Reload shell configuration",       .command=&reload_shell},
        {.text="shegang_config" , .arguments_count=0, .hint="Configurate current session",      .command=&shegang_config},
        {.text="sgls"           , .arguments_count=0, .hint="list: sgls <dir> -p|t|l|a",        .command=&sgls},
        {.text="sglsblk"        , .arguments_count=0, .hint="lsblk command analogue",           .command=&sglsblk},
        {.text="gapf"           , .arguments_count=1, .hint="cat analogue: gapf <file>",        .command=&gapf},
        {.text="environment"    , .arguments_count=2, .hint="Env vars (view help environment)", .command=&environment},
        {.text="echon"          , .arguments_count=0, .hint="echo with envvars",                .command=&echo_n}
    };

    const int len = sizeof(commands) / sizeof(commands[0]);
    int status = 1;

    // если команда это sghint то пытаемся вывести подсказку о встроенной команде
    if (strcmp(args[0], "sghint") == 0) {
        for (int i = 0; i < len; ++i) {
            if (args[1] == NULL) {
                printf("Usage: sghint <built-in command>\n");
                return 1;
            } else if (strcmp(args[1], commands[i].text) == 0) {
                printf("%ssghint%s: %s\n", GRAY, RESET, commands[i].hint);
            }
        }

        return 1;
    }

    // выполнение команды, если она есть в встроенных
    for (int i = 0; i < len; ++i) {
        if (strcmp(args[0], commands[i].text) == 0) {
            if (strcmp(commands[i].text, "exit") == 0) {
                commands[i].command(args);
            }
            if (commands[i].arguments_count > 0) {
                for (int j = 1; j < commands[i].arguments_count + 1; ++j) {
                    if (args[j] == NULL) {
                        printf("[Argument Count Error at %s:%d] %s\n", commands[i].text, j, commands[i].hint);
                        global_status_code = -1;
                        return 1;
                    }
                }
            }

            return commands[i].command(args);
        }
    }
    
    // проверяем, запустился ли алиас
    if (execute_alias(args) == 0) {
        return launch_task(args);
    }

    return 1;
}
```

 > Вы можете убрать возможность исполнения некоторых встроенных утилит - отредактируйте структуру commands.

Дело осталось за малым, если так можно выразиться. Поддиректория execution/builtin - встроенные утилиты. Начнем с `base.c`:

```cpp
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "colors.h"
#include "tasks_processing.h"
#include "config.h"

extern tasks tasks_structure;
extern int global_status_code;

/**
 * @brief Команда `cd`. Смена директории
 * 
 * @param char** args Аргументы
 * 
 * @return int Статус код
 **/
int change_directory(char** args) {
    if (args[1] == NULL) {
        print_message("Expected argument for \"cd\" command", ERROR);
    } else if (chdir(args[1]) != 0) {
        global_status_code = -1;
        print_message("Couldn't change directory", ERROR);
    }

    return 1;
}

/**
 * @brief Функция для вывода задач заднего плана
 * 
 * @return int Статус код
 **/
int bg_tasks(char** args) {
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

Следующий - gapf.c. GAPF (get and print file) - это моя небольшая утилита, аналог cat. Он выводит контент файла вместе с нумерацией строк, а в конце подводит информацию о нем:

```bash
File Details:
File Permissions: -rw-r--r--(644)
Last modified: Sun Sep  8 15:39:14 2024
File Type: Markdown
```

Итак, вот сам код:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "colors.h"

/**
 * @brief      Выводим информацию о файле
 *
 * @param[in]  filename  путь до файла
 */
void printFileDetails(const char* filename) {
    struct stat fileStat;
    if (stat(filename, &fileStat) < 0) {
        perror("stat");
        return;
    }

    printf("\nFile Details:\n");
    printf("File Permissions: ");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("(%o)\n", fileStat.st_mode & 0777);
    printf("Last modified: %s", ctime(&fileStat.st_mtime));
}

/**
 * @brief      Выводим тип файла
 *
 * @param[in]  filename  The filename
 */
void printFileType(const char* filename) {
    const char* extension = strrchr(filename, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".md") == 0) {
            printf("File Type: Markdown\n");
        } else if (strcmp(extension, ".c") == 0) {
            printf("File Type: C Code\n");
        } else if (strcmp(extension, ".cpp") == 0) {
            printf("File Type: C++ Code\n");
        } else if (strcmp(extension, ".py") == 0) {
            printf("File Type: Python Code\n");
        } else if (strcmp(extension, ".sh") == 0) {
            printf("File Type: Shell code\n");
        } else {
            printf("File Type: Unknown\n");
        }
    } else {
        printf("File Type: Unknown\n");
    }
}

/**
 * @brief      Get And Print File - аналог cat
 *
 * @param      char** args   Аргументы
 *
 * @return     Статус код
 */
int gapf(char** args) {
    if (args[1] == NULL) {
        printf("Usage: %s <filename>\n", args[0]);
        return 1;
    }

    FILE* file = fopen(args[1], "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char line[256];
    int lineNumber = 1;

    while (fgets(line, sizeof(line), file)) {
        printf("%s%3d: %s%s", BLUE, lineNumber, RESET, line);
        lineNumber++;
    }

    fclose(file);

    printFileDetails(args[1]);
    printFileType(args[1]);

    return 1;
}

```

Следующий шаг - это утилита sgls. Самая большая, пожалуй, утилита из приведенных. sgls - аналог ls, но с поддержкой цветов и иконками [Nerd Font Icons](nerdfonts.com). Собственно, для ее адекватной работы нужно скачать шрифт с поддержкой Nerd Icons.

Вот сам код (учтите, у вас могут не отображаться иконки. Если вам не нужны иконки, можете их убрать, это достаточно просто):

```cpp
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "colors.h"
#include "shell_api.h"

/**
 * @brief      Стилизованный вывод
 *
 * @param[in]  color  Цвет
 * @param[in]  text   текст
 * @param[in]  spch   спец-символ
 */
void print_styled(const char* color, const char* text, char* spch) {
    if (strcmp(spch, " ") == 1) { // if special char is not empty
        printf("%s%s%s%s\t", spch, color, text, RESET);
    } else {
        printf("%s%s%s\t", color, text, RESET);
    }
}

/**
 * @brief      проверка и вывод прав файлов
 *
 * @param[in]  fileStat  file stat
 */
void check_permissions(struct stat fileStat) {
    printf(YELLOW);
    printf((S_ISDIR(fileStat.st_mode)) ? "d"
            : ((S_ISLNK(fileStat.st_mode)) ? : "-"));
    printf(GREEN);
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf(BLUE);
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf(MAGENTA);
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(RESET);
}

/**
 * @brief      Вывод файлов и директории в виде грида
 *
 * @param      items   Элементы
 * @param[in]  count   Количество элементов
 * @param      colors  Цвета
 */
void print_grid(char** items, int count, char** colors) {
    int max_length = 0;
    int length = 0;

    for (int i = 0; i < count; i++) {
        length = strlen(items[i]) + strlen(colors[i]); // длина элемента + длина цвета
        if (length > max_length) {
            max_length = length;
        }
    }

    int columns = MAX_GRID_COLUMNS;
    int rows = (count + columns - 1) / columns;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            int index = r + c * rows;
            if (index < count) {
                printf("%s%-*s", colors[index], max_length, items[index]);
            }
        }
        printf("\n");
    }
}

/**
 * @brief      листинг файлов
 *
 * @param[in]  dir_path          путь до директории
 * @param[in]  show_permissions  флаг для отображения прав
 * @param[in]  show_time         флаг для отображения времени изменения
 * @param[in]  show_hidden       флаг для отображения скрытых файлов
 * @param[in]  list_show         флаг для отображения в виде списка
 */
void list_files(const char* dir_path, bool show_permissions, bool show_time,
                bool show_hidden, bool list_show) {
    DIR* dir;
    struct dirent* entry;
    struct stat file_stat;
    char** items = (char**)malloc(sizeof(char*) * MAX_FILE_NAME_LENGTH);
    char** colors = (char**)malloc(sizeof(char*) * MAX_FILE_NAME_LENGTH);
    int count = 0;

    // Open the directory
    dir = opendir(dir_path);
    if (dir == NULL) {
        print_message("Error opening the directory", ERROR);
        return;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        char full_path[MAX_FILE_NAME_LENGTH];
        char formattedString[MAX_FILE_NAME_LENGTH];
        char color[MAX_FILE_NAME_LENGTH];
        stat(full_path, &file_stat);
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        // Skip hidden files and current/parent directories
        if (entry->d_name[0] == '.' && !show_hidden) {
            continue;
        }

        // Get the file stats
        if (lstat(full_path, &file_stat) < 0) {
            print_message("Error get file stats", ERROR);
            continue;
        }

        if (show_permissions) {
            list_show = true;
            check_permissions(file_stat);
            printf("%s(%lo)%s ", YELLOW,
                    (unsigned long)file_stat.st_mode & 0777, RESET);
        }

        if (show_time) {
            list_show = true;
            char time_str[100];
            strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M:%S",
                    localtime(&file_stat.st_ctime));
            printf("%s[%s]%s ", CYAN, time_str, RESET);
        }

        if (S_ISDIR(file_stat.st_mode)) { // директории
            if (!list_show) {
                sprintf(formattedString, "  %s", entry->d_name); // сам текст элемента, иконку можно убрать
                sprintf(color, "%s", BLUE); // цвет
            } else {
                print_styled(BLUE, entry->d_name, "  "); // если задан флаг для просмотра в виде списка
            }
        } else if (S_ISLNK(file_stat.st_mode)) { // ссылки
            if (!list_show) {
                sprintf(formattedString, "  %s", entry->d_name);
                sprintf(color, "%s", CYAN);
            } else {
                print_styled(CYAN, entry->d_name, "  ");
            }
        } else if ((file_stat.st_mode & S_IXUSR) || // бинарные файлы
                    (file_stat.st_mode & S_IXGRP)
                    || (file_stat.st_mode & S_IXOTH)) {
            if (!list_show) {
                sprintf(formattedString, "  %s", entry->d_name);
                sprintf(color, "%s", GREEN);
            } else {
                print_styled(GREEN, entry->d_name, "  ");
            }
        } else if (access(full_path, R_OK) == 0 && // изображение
                    (strstr(entry->d_name, ".png") != NULL
                    || strstr(entry->d_name, ".jpg") != NULL
                    || strstr(entry->d_name, ".jpeg") != NULL
                    || strstr(entry->d_name, ".svg") != NULL
                    || strstr(entry->d_name, ".bmp") != NULL)) {
            if (!list_show) {
                sprintf(formattedString, "  %s", entry->d_name);
                sprintf(color, "%s", MAGENTA);
            } else {
                print_styled(MAGENTA, entry->d_name, "  ");
            }
        } else {
            if (!list_show) {
                sprintf(formattedString, "  %s", entry->d_name);
                sprintf(color, "%s", WHITE);
            } else {
                print_styled(BOLD, entry->d_name, "  ");
            }
        }

        if (!list_show) {
            items[count] = strdup(formattedString);
            colors[count] = strdup(color);
        } else {
            printf("\n");
        }
        count++;
    }

    if (!list_show)
        print_grid(items, count, colors);

    closedir(dir);
    free(items);
}

/**
 * @brief      Получаем длинну массива
 *
 * @param      array  массив
 *
 * @return     размер массива
 */
int get_array_size(char** array) {
    int count = 0;

    if (array != NULL) {
        while (array[count] != NULL) {
            count++;
        }
    }

    return count;
}

/**
 * @brief      sgls - аналог ls
 *
 * @param      args  The count of arguments
 *
 * @return     Статус код
 */
int sgls(char** args) {
    char *dir_path = ".";
    bool show_permissions = false;
    bool show_time = false;
    bool show_hidden = false;
    bool show_list = false;

    for (int i=0; args[i] != NULL; i++) {
        if (strcmp(args[i], "-p") == 0) {
            show_permissions = true;
        } else if (strcmp(args[i], "-t") == 0) {
            show_time = true;
        } else if (strcmp(args[i], "-a") == 0) {
            show_hidden = true;
        } else if (strcmp(args[i], "-l") == 0) {
            show_list = true;
        } else if (strcmp(args[i], "-h") == 0) {
            println("Built-in function `sgls`\n");
            printf(
                "SGLS - A `ls` command analogue from shegang\n\n"
                "-t             Show time\n"
                "-a             Show hidden\n"
                "-l             Show as list\n"
                "-p             Show permissions\n"
            );
            return 1;
        } else {
            dir_path = args[i];
        }
    }

    if (args[1] == NULL) {
        dir_path = ".";
    }

    list_files(dir_path, show_permissions, show_time,
                        show_hidden, show_list);

    return 1;
}
```

Теперь давайте напишем простейший аналог lsblk - `sglsblk`:

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <linux/kdev_t.h>
#include "colors.h"
#include "shell_api.h"

/**
 * @brief      Структура диска
 */
struct Disk {
    char name[DEFAULT_BUFFER_SIZE];
    int maj_min;
    int rm;
    long long size;
    bool ro;
    char type[DEFAULT_BUFFER_SIZE];
    char mountpoint[DEFAULT_BUFFER_SIZE];
};

/**
 * @brief      выводим информацию о диске
 *
 * @param[in]  disk  диск (Disk)
 */
void print_disk(struct Disk disk) {
    printf("%-6s %-7d %-2d %-6lld %-2d %-6s %-12s\n", disk.name, disk.maj_min, disk.rm, disk.size, disk.ro, disk.type, disk.mountpoint);
}


/**
 * @brief      sglsblk (lsblk)
 *
 * @return     статус код
 */
int sglsblk(char** args) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[DEFAULT_BUFFER_SIZE];
    struct Disk disk;
    FILE *mounts;

    printf("%-6s %-7s %-3s %-7s %-3s %-7s %-12s\n", "NAME", "MAJ:MIN", "RM", "SIZE", "RO", "TYPE", "MOUNTPOINTS");

    dir = opendir("/sys/block");
    if (dir == NULL) {
        print_message("Error opening the dir", ERROR);
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        snprintf(path, DEFAULT_BUFFER_SIZE, "/sys/block/%s/removable", entry->d_name);
        if (stat(path, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode) && st.st_size > 0) {
            disk.rm = 1;
        } else {
            disk.rm = 0;
        }

        snprintf(path, DEFAULT_BUFFER_SIZE, "/sys/block/%s/ro", entry->d_name);
        if (stat(path, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode) && st.st_size > 0) {
            disk.ro = true;
        } else {
            disk.ro = false;
        }

        snprintf(path, DEFAULT_BUFFER_SIZE, "/sys/block/%s/size", entry->d_name);

        if (stat(path, &st) == -1)
            continue;

        disk.size = st.st_size * 512ULL;

        disk.maj_min = makedev(st.st_rdev >> 8, st.st_rdev & 0xFF);

        strcpy(disk.name, entry->d_name);

        snprintf(path, DEFAULT_BUFFER_SIZE, "/dev/%s", entry->d_name);

        if ((disk.maj_min != 0 || disk.size != 0) && stat(path, &st) == 0 && S_ISBLK(st.st_mode)) {
            strcpy(disk.type, "disk");
            strcpy(disk.mountpoint, "-");
        } else {
            strcpy(disk.type, "part");
            sprintf(path, "/sys/block/%s/%s/mountpoint", entry->d_name, disk.name);

            mounts = fopen(path, "r");

            if (mounts == NULL) {
                print_message("Error get mountpoint", ERROR);
                strcpy(disk.mountpoint, "-");
            } else {
                fgets(disk.mountpoint, DEFAULT_BUFFER_SIZE, mounts);
                strtok(disk.mountpoint, "\n");
                fclose(mounts);
            }
        }

        print_disk(disk);
    }

    closedir(dir);

    return 1;
}
```

Сама это утилита предоставляет примерно такой вывод:

```bash
NAME   MAJ:MIN RM  SIZE    RO  TYPE    MOUNTPOINTS
sdb    0       1  2097152 1  disk   -
sda    0       1  2097152 1  disk   -
zram0  0       1  2097152 1  disk   -
```

И наконец, последний файл - shegang_config.c:

```cpp
#include <stdio.h>
#include <string.h>
#include "colors.h"
#include "colorschemes.h"
#include "userinput.h"

extern char* username_color;
extern char* pwd_color;
extern char* curr_time_color;

/**
 * @brief      Задаем значение для цветовой переменной
 *
 * @param      var_name  Название
 * @param      value     Значение
 *
 * @return     int       Статус код
 */
int set_value_for_var(char* var_name, char* value) {
    char* color = get_color_by_name(value);

    if (strcmp(var_name, "USERNAME_COLOR") == 0) {
        username_color = color;
    } else if (strcmp(var_name, "PWD_COLOR") == 0) {
        pwd_color = color;
    } else if (strcmp(var_name, "TIME_COLOR") == 0) {
        curr_time_color = color;
    } else {
        print_message("Expected argument for \"shegang_config\" command: variable name", WARNING);
    }

    return 1;
}

/**
 * @brief      shegang_config
 *
 * @param      args  Аргументы
 *
 * @return     int   Статус код
 */
int shegang_config(char** args) {
    if (args[1] == NULL) {
        print_message("Expected argument for \"shegang_config\" command. Launch with help for view help page", WARNING);
        return 1;
    } else if (strcmp(args[1], "help") == 0 || strcmp(args[1], "-h") == 0 || strcmp(args[1], "--help") == 0) {
        println("Built-in function `shegang_config`\n");
        printf(
            "set <VAR> <VALUE>           - set value for var (ex. set USERNAME_COLOR RED)\n"
            "\nExisting variables: USERNAME_COLOR; PWD_COLOR; TIME_COLOR\n"
            "Existing colors: RED, GREEN, BLUE, YELLOW, MAGENTA, GRAY, BLACK, WHITE, CYAN\n"
            "Special variables: COLORSCHEME (values: onedark, gruvbox)"
        );
    } else if (strcmp(args[1], "set") == 0) {
        // задаем настройки
        if (args[3] == NULL || args[2] == NULL) {
            print_message("Expected argument for \"shegang_config\" command: <value>", WARNING);
            return 1;
        }

        // если задаем COLORSCHEME (цветовую схему) то задаем ее как текущую и перезагружаем isccline
        if (strcmp(args[2], "COLORSCHEME") == 0) {
            get_scheme_by_name(args[3]);
            setup_isocline();
            return 1;
        }

        // иначе задаем цветовое значение для переменных USERNAME_COLOR, PWD_COLOR, TIME_COLOR
        char* color = get_color_by_name(args[3]);

        return set_value_for_var(args[2], color);
    }

    return 1;
}
```

Ну и получается все. Хотя нет, не все. Я обещал показать базовую минимальную реализацию системы плагинов. 

Данный модуль требует заголовчный файл `plugins_manager.h`. Согласно нашей структуре проекта, заголовочные файлы хранятся в директории include.

Так что такое плагин в понимании нашего шелла? Плагин - это код на языке C, который имеет некоторые обязательные параметры (пример плагина я покажу позже). Для подключения он должен быть скомплирован в файл библиотеки .so и в дальнейшем он может быть подключен к шеллу.

```cpp
#ifndef PLUGINS_MANAGER_H
#define PLUGINS_MANAGER_H

// структура метаданных плагина: название, описание, автор и лицензия
typedef struct {
    char* name;
    char* description;
    char* author;
    char* license;
} plugin_metadata_t;

// основные функции плагина: *init (инициализация), *mainloop (то есть основный цикл плагина, который встраивается в цикл шелла) и *deinit (деинициализация)
typedef struct {
    int (*init)(void);
    int (*mainloop)(char* input);
    int (*deinit)(void);
} plugin_functions_t;

// структура плагина
typedef struct {
    plugin_metadata_t metadata;
    plugin_functions_t functions;
} plugin_t;

// функции менеджера плагинов
int load_plugin(const char* plugin_path, plugin_t* plugin);
int unload_plugin(plugin_t* plugin);
int execute_plugin(plugin_t* plugin, char* input);
int print_plugin_info(plugin_t* plugin);

#endif // PLUGINS_MANAGER_H
```

И вот сам код:

```cpp
#include "dlfcn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "plugins_manager.h"

/**
 * @brief      загрузка плагина
 *
 * @param[in]  plugin_path  путь до плагина
 * @param      plugin       структура плагина
 *
 * @return     статус код
 */
int load_plugin(const char* plugin_path, plugin_t* plugin) {
    void* handle = dlopen(plugin_path, RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "%sError occured when loading plugin: %s%s\n", RED, dlerror(), RESET);
        return -1;
    }

    // получаем функции
    plugin->functions.init = dlsym(handle, "plugin_init");
    plugin->functions.mainloop = dlsym(handle, "plugin_mainloop");
    plugin->functions.deinit = dlsym(handle, "plugin_deinit");

    if (!plugin->functions.init ||
        !plugin->functions.mainloop ||
        !plugin->functions.deinit) {
        fprintf(stderr, "%sError occured when get plugin functions:%s%s\n", RED, dlerror(), RESET);
        dlclose(handle);

        return -1;
    }

    void (*get_plugin_metadata)(plugin_metadata_t*) = dlsym(handle, "get_plugin_metadata");

    if (!get_plugin_metadata) {
        fprintf(stderr, "%sError occured when get plugin metadata:%s%s\n", RED, dlerror(), RESET);
        dlclose(handle);

        return -1;
    }

    // сообщаем о загрузке
    get_plugin_metadata(&plugin->metadata);
    printf("Plugin %s has been loaded\n", plugin->metadata.name);

    return 1;
}

/**
 * @brief      выгрузка плагина
 *
 * @param      plugin  Плагин
 *
 * @return     Статус код
 */
int unload_plugin(plugin_t* plugin) {
    printf("Plugin %s has been unloaded\n", plugin->metadata.name);

    return 0;
}

/**
 * @brief      Вызов основного цикла плагина
 *
 * @param      plugin  Плагин
 * @param      input   Ввод пользователя (т.к. основной цикл плагина встраивается в цикл шелла, то он также может работать с пользовательским вводом в виде строки)
 *
 * @return     Статус код
 */
int execute_plugin(plugin_t* plugin, char* input) {
    return plugin->functions.mainloop(input);
}

/**
 * @brief      Информация о плагине
 *
 * @param      plugin  Плагин
 */
int print_plugin_info(plugin_t* plugin) {
    printf("Plugin: %s\n", plugin->metadata.name);
    printf("\tDescription: %s\n", plugin->metadata.description);
    printf("\tAuthor: %s\n", plugin->metadata.author);
    printf("\tLicense: %s\n", plugin->metadata.license);

    return 1;
}
```

А вот и сам пример плагина:

```cpp
/**
 * Example plugin for Shegang shell
 * by alxvdev
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Metadata
typedef struct {
    char* name;
    char* description;
    char* author;
    char* license;
} plugin_metadata_t;

// Init - before shell mainloop
int plugin_init(void) {
    printf("Plugin Pipegang has been initialized. !Pipegang plugin is not ready to use!\n");
    return 1;
}

/**
 * @brief      Work with user input in shell mainloop
 *
 * @param      input  The input
 *
 * @return     status code
 */
int plugin_mainloop(char* input) {
    printf("INPUT: %s\n\n", input);

    return 0;
}

// Deinit - after exit from shell
int plugin_deinit(void) {
    printf("Deinitialize Pipegang plugin\n");

    return 0;
}

// Get plugin metadata;
void get_plugin_metadata(plugin_metadata_t* metadata) {
    metadata->name = "Pipegang";
    metadata->description = "This is example plugin for SheGang";
    metadata->author = "Alexeev Bronislav";
    metadata->license = "MIT License";
}
```

Компилируется он командой:

```bash
clang -Wall -Wextra -shared -fPIC -O2 -std=c11 <путь до плагина> -o example_plugin.so
```

# Финал

Теперь точно все. Осталось дело за малым - соединить весь функционал в одном файле - shegang.c:

```cpp
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include "userinput.h"
#include "executor.h"
#include "tasks_processing.h"
#include "plugins_manager.h"
#include "config.h"
#include "isocline.h"

extern tasks tasks_structure;
extern char* username_color;
extern char* pwd_color;

/**
 * @brief      вывод приветственного сообщения.
 */
void print_welcome_message(void) {
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
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "C.UTF-8");

    char* line;
    char** args;
    char** args_by_delims;
    int status = 1;
    int returned_value = 0;
    int have_plugin = 0;
    int num_tokens;

    // загрузка конфигурации
    load_main_config();
    load_config_aliases();
    setup_isocline();

    print_welcome_message();

    // загрузка плагина
    plugin_t myplugin;

    // плагин загружается так: shegang <путь до плагина>
    if (argv[1] != NULL) {
        have_plugin = 1;
    }

    if (have_plugin) {
        if (load_plugin(argv[1], &myplugin) == -1) {
            fprintf(stderr, "Plugin loading error\n");
            return -1;
        }

        myplugin.functions.init();
        print_plugin_info(&myplugin);
    }

    ic_async_stop();

    signal(SIGINT, kill_foreground_task); // ловим исключения
    signal(SIGCHLD, mark_ended_task);

    do {
        display_ps(status);

        line = read_user_input();

        if (line == NULL) {
            continue;
        }

        // сначала, если плагин подключен, то выполняем его функцию основного цикла
        if (have_plugin) {
            myplugin.functions.mainloop(line);
        }

        args_by_delims = split_by_delims(line, "&&||;", &num_tokens); // токенизация строки ввода по разделителям

        // проходимся в цикле по массиву и выполняем команды поочередно.
        for (int i = 0; i < num_tokens; i++) {
            args = split_into_tokens(args_by_delims[i]);
            status = execute(args);
        }

        free(line);
        free(args);
    } while(status >= 1 || status < 0); 

    return 0;
}
```

Вот и все. Наш шелл почти готов! В третьей части я планирую допилить его до ума. А вообще, предлагайте в комментариях также свои идеи для функционала в шелле.

Среди основных идей у меня - улучшение конфигурации и загрузки плагинов. С плагинами немного туго - они как бы есть, но могут выполнять минимальный функционал, и в шелле может работать только один плагин одновременно. Загружать, кстати, плагины можно так: `shegang <путь до плагина>.so`.

Напоминаю, что shegang - название моего шелла, вы можете использовать какое угодно название, просто изменяйте код.

Репозиторий с исходным кодом [доступен по ссылке](https://github.com/alxvdev/shegang).

# Заключение
Спасибо за внимание! Это был довольно интересный опыт для меня, т.к. это мой первый большой проект на языке C.

Если у вас есть замечания по статье или по коду - пишите, наверняка есть более опытный и профессиональный программист на C, который может помочь как и читателям статьи, так и мне.

Ссылка на мой репозиторий реализации командного интерпретатора [здесь](https://github.com/alxvdev/shegang).
Ссылка на первую часть статьи [здесь](https://habr.com/ru/companies/timeweb/839890).

Буду рад, если вы присоединитесь к моему небольшому [телеграм-блогу](https://t.me/hex_warehouse). Анонсы статей, новости из мира IT и полезные материалы для изучения программирования и смежных областей.

## Источники

 + [Habr / Давайте напишем командную оболочку Linux](https://habr.com/ru/articles/537156/)
