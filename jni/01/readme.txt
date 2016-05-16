

javac JNIDemo.java
gcc -I/usr/lib/jvm/java-1.7.0-openjdk-amd64/include/ -fPIC -shared -o libnative.so native.c
export LD_LIBRARY_PATH=.
java JNIDemo 


//gcc native.c -I/home/fa1c0n/datas/Program/jdk1.7.0_79/include/ -fPIC -shared -o libnative.so


