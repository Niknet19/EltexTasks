# Компиляция list.c в динамическую библиотеку
liblist.so: list.c 
	gcc -fPIC list.c -c
	gcc -shared list.o -o liblist.so

# Компиляция основного файла 6.2.c с динамической библиотекой
main: 6.2.c liblist.so
	gcc -o main 6.2.c -ldl -lm

# Удаление скомпилированных файлов
clean:
	rm -f *.o liblist.so main

