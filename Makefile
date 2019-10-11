output: main.o fpuasm.o
	g++ main.o fpuasm.o -o cppSQRT -fPIC

main.o: main.cpp
	g++ -c main.cpp

fpuasm.o: fpuasm.asm
	nasm -f elf64 fpuasm.asm

clean:
	rm *.o cppSQRT
