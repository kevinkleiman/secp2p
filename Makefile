TARGET = peer

build: 
	gcc $(TARGET).c -o peer -lpthread
clean: 
	rm -rf *.out
	rm -rf $(TARGET)
