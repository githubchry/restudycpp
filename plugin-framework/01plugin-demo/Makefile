SOURCE=PluginTestObject.cpp

make:${SOURCE}
	g++ -fPIC -shared $^ -o libto.so
	g++ -o main main.cpp -ldl


clean:
	rm -rf main *.o *.so


all:main
