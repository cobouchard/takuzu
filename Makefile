all:
	@make -C src
	@cp src/main .

clean: 
	@rm -f *.o main


help:
	@echo "Usage: "
	@echo "make [all]\t Build the software"
	@echo "make clean \t Remove all files generated by make"
	@echo "make help\t display this help"


.PHONY: all clean help
