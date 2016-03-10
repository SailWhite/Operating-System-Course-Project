# Makefile
FOLDERS	= lib firmware bootsect userapp kernel
msim:
	@for folder in $(FOLDERS);\
		do make -C ./$$folder msim;\
	done
all:
	@for folder in $(FOLDERS);\
		do make -C ./$$folder all;\
	done
clean:
	@for folder in $(FOLDERS);\
		do make -C ./$$folder clean;\
	done
.PHONY:all clean msim
