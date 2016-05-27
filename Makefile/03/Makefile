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

#使用变量;
#让make自动推导

#另类风格的Makfile(不推荐，因为文件的依赖关系显得凌乱)

objects = main.o kbd.o command.o display.o insert.o \
          search.o files.o utils.o 

edit : $(objects) 
	gcc -o edit $(objects) 

$(objects) : defs.h

kbd.o command.o files.o : command.h

display.o insert.o search.o files.o : buffer.h

#.PHONY 表示clean是个伪目标文件
#在 rm命令前面加了一个小减号的意思就是: 
#     也许某些文件出现问题，但不要管，继续做后面的事。
.PHONY : clean
clean : 
	-rm edit $(objects) 
