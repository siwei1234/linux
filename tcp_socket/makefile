all:tcp_cli tcp_thread
tcp_thread:tcp_thread.cpp
	g++ -std=c++11 $^ -o $@ -lpthread
tcp_cli:tcp_cli.cpp
	g++ -std=c++11 $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ -std=c++11 $^ -o $@
