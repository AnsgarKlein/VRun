
exec: scandir
	./scandir

scandir: scandir.c
	gcc scandir.c -o scandir -D"$(DEFINE)" $(FLAGS) -DSCANDIR_MAIN
	chmod +x scandir

scandir.c:
