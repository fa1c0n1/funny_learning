########################################################
#                                                      #
#     target ... : prerequisites ...                   #
#     	command                                        #
#     	...                                            #
#     	...                                            #
#                                                      #
# c-src: main.c kbd.c command.c display.c insert.c /   #
#        search.c files.c utils.c                      #
#
# 要实现的编译规则:
#   1) 如果这个工程没有编译过，那么我们的所有C文件都要编译并被链接.
#   2) 如果这个工程的某几个C文件被修改，那么我们只编译被修改的C文件，并链接目标程序.
#   3) 如果这个工程的头文件被改变了，那么我们需要编译引用了这几个头文件的C文件，并链接目标程序.
#
#                                                      #
########################################################

edit : main.o kbd.o command.o display.o insert.o /
		search.o files.o utils.o
	gcc -o edit main.o kbd.o command.o display.o insert.o /
			search.o files.o utils.o	

main.o : main.c defs.h
	gcc -c main.c

kbd.o : kbd.c defs.h command.h
	gcc -c kbd.c

command.o : command.c defs.h command.h
	gcc -c command.c

display.o : display.c defs.h buffer.h
	gcc -c display.c

insert.o : insert.c defs.h buffer.h
	gcc -c insert.c

search.o : search.c defs.h buffer.h
	gcc -c search.c

files.o : files.c defs.h buffer.h command.h
	gcc -c files.c

utils.o : utils.c defs.h
	gcc -c utils.c

clean : 
	rm edit main.o kbd.o command.o display.o insert.o search.o files.o utils.o
