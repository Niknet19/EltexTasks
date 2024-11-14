# Правила для сборки динамических библиотек

libsum.so: sum.c
	gcc -fPIC sum.c -c
	gcc -shared sum.o -o libsum.so

libsub.so: sub.c
	gcc -fPIC sub.c -c
	gcc -shared sub.o -o libsub.so

libmul.so: mul.c
	gcc -fPIC mul.c -c
	gcc -shared mul.o -o libmul.so

libdiv.so: div.c
	gcc -fPIC div.c -c
	gcc -shared div.o -o libdiv.so

# Правило для сборки исполняемого файла
main: main.c libsum.so libsub.so libmul.so libdiv.so
	gcc -o main main.c -ldl -lm

# Очистка промежуточных и собранных файлов
clean:
	rm -f *.so *.o main
