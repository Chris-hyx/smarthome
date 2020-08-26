cc=g++
exe=main
obj=main.o Socket.o
$(exe):$(obj)
	$(cc) -o $(exe) $(obj)
main.o:main.cpp
	$(cc) -c main.cpp
Socket.o:Socket.cpp
	$(cc) -c Socket.cpp -lphtread  -ljson
clean:
	rm -rf *.o main
