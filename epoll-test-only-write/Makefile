all:
	gcc -g -o epoll_server_only_write	epoll_server_only_write.c -lpthread
	gcc -g -o epoll_server_only_read	epoll_server_only_read.c -lpthread
	gcc -g -o epoll_server_read_and_write	epoll_server_read_and_write.c -lpthread
	gcc -g -o client	client.c
clean:
	rm -rf epoll_server client epoll_server_only_write epoll_server_only_read epoll_server_read_and_write
