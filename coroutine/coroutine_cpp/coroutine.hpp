#pragma once
#include<ucontext.h>
#include<map>
#include<vector>

#define STACKSZ (1024*64)


class schedule_t;


enum State {DEAD, READY, RUNNING, SUSPEND};

typedef void*(*call_back)(schedule_t* s, void* args);
class coroutine_t
{
	friend class schedule_t;
public:
	coroutine_t(call_back call, void* args)
	{
		_call = call;
		_args = args;
		state = READY;
		getcontext(&ctx);
		ctx.uc_stack.ss_sp = stack;
		ctx.uc_stack.ss_size = STACKSZ;
		ctx.uc_stack.ss_flags = 0;
	}
private:
	call_back _call;
	void* _args;
	ucontext_t ctx;
	char stack[STACKSZ];
	enum State state;
};

class schedule_t
{
	friend class coroutine_t;
public:
	schedule_t()
	{
		current_fd = -1;
	}

	~schedule_t()
	{
		std::map<int, coroutine_t*>::iterator it = coroutines.begin();
		while(it != coroutines.end())
		{
			delete it->second;
			it++;
		}
		coroutines.clear();
	}

	static void* main_fun(schedule_t* s)
	{
		int fd = s->current_fd;
		if(fd != -1)
		{
			coroutine_t* c = s->coroutines[fd];
			c->_call(s, c->_args);
			c->state = DEAD;
			s->Erase();
			s->current_fd = -1;
		}
	}
	
	int create(call_back call, int fd, void* args = NULL)
	{
		coroutine_t* c = new coroutine_t(call, args);
		//coroutines.insert(std::make_pair(fd, c));
		coroutines[fd] = c;
		c->ctx.uc_link = &ctx_main;
		makecontext(&c->ctx, (void(*)())&main_fun, 1, this);
		return fd;
	}


	void running(int fd)
	{
		if(coroutines.end() != coroutines.find(fd))
		{
			coroutine_t* c = coroutines[fd];
			c->state = RUNNING;
			current_fd = fd;
			swapcontext(&ctx_main, &c->ctx);
		}
	}

	void yield()
	{
		if(current_fd != -1)
		{
			coroutine_t* c = coroutines[current_fd];
			c->state = SUSPEND;
			current_fd = -1;
			swapcontext(&c->ctx, &ctx_main);
		}
	}

	void resume(int fd)
	{
		coroutine_t* c = coroutines[fd];
		if(c->state == SUSPEND)
		{
			c->state = RUNNING;
			current_fd = fd;
			swapcontext(&ctx_main, &c->ctx);
		}
	}

	int getfd()
	{
		return current_fd;
	}

	std::vector<int> getfds()
	{
		std::vector<int> ret;
                std::map<int, coroutine_t*>::iterator it = coroutines.begin();
                while(it != coroutines.end())
		{
			ret.push_back(it->first);
			it++;
		}

		return ret;
	}

	void Erase()
	{
		delete coroutines[current_fd];
		coroutines.erase(current_fd);
	}


	int finished()
	{
		if(current_fd != -1)
			return 0;
		std::map<int, coroutine_t*>::iterator it = coroutines.begin();
		while(it != coroutines.end())
		{
			coroutine_t* c = it->second;
			if(c->state != DEAD)
				return 0;
			it++;
		}
		return 1;
	}
private:
	std::map<int, coroutine_t*> coroutines;
	int current_fd;
	ucontext_t ctx_main;
};
