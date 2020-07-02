#include <cstdio>
#include <vector>
#include <sys/select.h>
#include "tcpsocket.hpp"

#define MAX_TIMEOUT 3000
class Select{
	public:
		Select():_maxfd(-1){
			FD_ZERO(&_rfds);
		}
		bool Add(TcpSocket &sock) {
			int fd = sock.GetFd();
			FD_SET(fd, &_rfds);
			_maxfd = _maxfd > fd ? _maxfd : fd;
			return true;
		}
		bool Del(TcpSocket &sock) {
			int fd = sock.GetFd();
			FD_CLR(fd, &_rfds);
			for (int i = _maxfd; i >= 0; i--) {
				if (FD_ISSET(i, &_rfds)) {
					_maxfd = i;
					break;
				}
			}
			return true;
		}
		bool Wait(std::vector<TcpSocket> *list, int outtime = MAX_TIMEOUT) {
			struct timeval tv;
			tv.tv_sec = outtime / 1000;
			tv.tv_usec = (outtime % 1000) * 1000;
			fd_set set;
			set = _rfds;
			int ret = select(_maxfd+1, &set, NULL, NULL, &tv);
			if (ret < 0) {
				perror("select error");
				return false;
			}if (ret == 0) {
				printf("wait timeout\n");
				return true;
			}
			for (int i = 0; i < _maxfd+1; i++) {
				if (FD_ISSET(i, &set)) {
					TcpSocket sock;
					sock.SetFd(i);
					list->push_back(sock);
				}
			}
			return true;
		}
	private:
		fd_set _rfds; 
		int _maxfd;
};
