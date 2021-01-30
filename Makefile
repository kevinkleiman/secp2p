TARGET = peer

build: 
	gcc $(TARGET).c -o peer
clean: 
	rm -rf *.out
	rm -rf $(TARGET)
