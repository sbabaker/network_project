all: ProxyServer
	g++ -g -std=c++0x ProxyServer.o -o ProxyServer

ProxyServer:
	g++ -c -g -std=c++0x ProxyServer.cpp

clean:
	@rm ProxyServer ProxyServer.o
