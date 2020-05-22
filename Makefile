build_cont1:
        gcc src/lcont1.c -o bin/lcont1

build_cont2:
        gcc src/lcont2.c  -o bin/lcont2
        
clean:
   rm -rf bin
