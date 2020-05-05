productor_customer:productor_customer.cpp
	g++ $^ -o $@ -lpthread
p_cond:p_cond.c
	gcc $^ -o $@ -lpthread
