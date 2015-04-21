all: ProxyServer ContentFilter
	g++ -g -std=c++0x ProxyServer.o ContentFilter.o -o ProxyServer

ContentFilter:
	g++ -c -g -std=c++0x ContentFilter.cpp

ProxyServer:
	g++ -c -g -std=c++0x ProxyServer.cpp

clean:
	@rm ProxyServer ProxyServer.o ContentFilter.o
