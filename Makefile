main:
	gcc -c queue.c crc.c frame.c protocol.c comm.c uwmodem.c 
	ar rcs libuwmodem.a *.o
	
	gcc main.c libuwmodem.a -o main

main2: main
	gcc main2.c libuwmodem.a -o main2

clean:
	rm *.o
	rm libuwmodem.a
	rm main.exe
	rm main2.exe