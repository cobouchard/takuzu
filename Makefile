all:
	@make -C src
	@cp src/takuzu .

clean:
	@rm -f src/*.o takuzu src/takuzu
test:
	@cd tests
	@tests/tester.sh
report:
	@pdflatex report/report.tex
help:
	@echo "Usage: "
	@echo "make [all]\t Build the software"
	@echo "make clean\t Remove all files generated by make"
	@echo "make test\t Execute testing script (tester.sh in tests folder)"
	@echo "make help\t Display this help"
	@echo "make report\t Create the pdf for the report of the project"


.PHONY: all clean test report help
