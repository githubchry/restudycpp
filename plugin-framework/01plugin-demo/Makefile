SOURCE=PluginTestObject.cpp

make:${SOURCE}
	g++ -fPIC -shared $^ -o libto.so
	g++ -o plugin-demo main.cpp -ldl


clean:
	rm -rf plugin-demo *.o *.so


all:plugin-demo
