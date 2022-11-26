image:
	g++ main.cpp Image.cpp Color.cpp ImageCLI.cpp Kernel.cpp Point.cpp -o Image.o

test:
	g++ test.cpp -o test.o