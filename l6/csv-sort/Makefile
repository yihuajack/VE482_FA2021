CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla
L6 = l6
L6MC = l6_memory_check
L6MC_FLAGS = -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined
.PHONY: clean

all: $(L6) $(L6MC)

dl: dl_text.c dl_csv.c plugin_manager.h plugin_manager.c
	$(CC) -c $(CFLAGS) -fPIC dl_text.c plugin_manager.h plugin_manager.c util.h util.c forward_list.h forward_list.c algorithm.h algorithm.c
	$(CC) -shared -o dl_text.so dl_text.o plugin_manager.o util.o forward_list.o algorithm.o
	$(CC) -c $(CFLAGS) -fPIC dl_csv.c plugin_manager.h plugin_manager.c util.h util.c forward_list.h forward_list.c algorithm.h algorithm.c
	$(CC) -shared -o dl_csv.so dl_csv.o plugin_manager.o util.o forward_list.o algorithm.o

$(L6): dl dl_main.c
	$(CC) -g -o $(L6) dl_main.c -ldl plugin_manager.h plugin_manager.c

$(L6MC): dl dl_main.c
	$(CC) -g $(L6MC_FLAGS) -o $(L6MC) dl_main.c -ldl plugin_manager.h plugin_manager.c

clean:
	$(RM) *.o *.gch *.so $(L6) $(L6MC)
