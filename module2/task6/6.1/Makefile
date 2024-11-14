# Название статической библиотеки
LIB_NAME = liblist.a

# Файлы
LIST_SRC = list.c
LIST_OBJ = list.o
MAIN_SRC = 6.1.c
MAIN_OBJ = 6.1.o
EXECUTABLE = 6.1

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g



# Цель по умолчанию
all: $(EXECUTABLE)

# Компилируем объектный файл для библиотеки
$(LIST_OBJ): $(LIST_SRC)
	$(CC) $(CFLAGS) -c $(LIST_SRC) -o $(LIST_OBJ)

# Создаем статическую библиотеку
$(LIB_NAME): $(LIST_OBJ)
	ar r $(LIB_NAME) $(LIST_OBJ)

# Компилируем основной файл
$(MAIN_OBJ): $(MAIN_SRC)
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o $(MAIN_OBJ)

# Создаем исполняемый файл, используя статическую библиотеку
$(EXECUTABLE): $(MAIN_OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(MAIN_OBJ) -L . $(LIB_NAME)


lib: $(LIB_NAME)

# Компилируем объектный файл для списка
$(LIST_OBJ): $(LIST_SRC)
	$(CC) $(CFLAGS) -c $(LIST_SRC) -o $(LIST_OBJ)

# Создаем статическую библиотеку
$(LIB_NAME): $(LIST_OBJ)
	ar r $(LIB_NAME) $(LIST_OBJ)

# Очистка
clean:
	rm -f $(EXECUTABLE) $(MAIN_OBJ) $(LIST_OBJ) $(LIB_NAME)
