#include<cstdio>
#include<string>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define MAX_LISTEN 5
#define CHECK_RET(q) if((q) == false){return -1;}
class TcpSocket
{
public:
	TcpSocket()
	:_sockfd(-1)
	{}
	void SetFd(int fd)
	{
		_sockfd = fd;
	}
	int GetFd()
	{
		return _sockfd;
	}
	bool Socket()
	{
		//socket(地址域，套接字类型，协议类型)
		_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(_sockfd < 0)
		{
			perror("socker, error");
			return false;
		}
		return true;
	}
	bool Bind(const std::string& ip, uint16_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		socklen_t len = sizeof(addr);
		int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
		if(ret < 0)
		{
			perror("bind error");
			return false;
		} 
		return true;
	}
	bool Listen(int backlog = MAX_LISTEN)
	{
		//listen(套接字描述符，最大并发链接数字)
		int ret = listen(_sockfd, backlog);
		if(ret < 0)
		{
			perror("listen error");
			return false;
		}
		return true;
	}
	bool Accept(TcpSocket* new_sock, std::string* ip = NULL, uint16_t* port = NULL)
	{
		//新建套接字描述符 = accept(监听套接字描述符， 客户端地址信息，地址长度)
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int new_fd = accept(_sockfd, (struct sockaddr*)& addr, &len);
		if(new_fd < 0)
		{
			perror("accept error");
			return false;
		}
		new_sock->_sockfd = new_fd;
		if(ip != NULL)
		{
			*ip = inet_ntoa(addr.sin_addr);
		}
		if(port != NULL)
		{
			*port = ntohs(addr.sin_port);
		}
		return true;
	}
	bool Recv(std::string* buf)
	{
		char tmp[4096] = {0};
		int ret = recv(_sockfd, tmp, 4096, 0);
		if(ret < 0)
		{
			perror("recv error");
			return false;
		}
		else if(ret == 0)
		{
			perror("connection broken");
			return false;
		}
		buf->assign(tmp, ret);
		return true;
	}
	bool Send(const std::string& data)
	{
		int ret = send(_sockfd, data.c_str(), data.size(), 0);
		if(ret < 0)
		{
			perror("send error");
			return false;
		}
		return true;
	}
	bool Close()
	{
		if(_sockfd > 0)
		{
			close(_sockfd);
			_sockfd = -1;
		}
		return true;
	}
	bool Connect(const std::string& ip, uint16_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		socklen_t len = sizeof(addr);
		//connecr(描述符，服务端地址信息，地址信息长度)
		int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
		if(ret < 0)
		{
			perror("connect error");
			return false;
		}
		return true;
	}
private:
	int _sockfd;
};
