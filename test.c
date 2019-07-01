#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <assert.h>

#define UV_ASSERTR(x) if((x)<0){fprintf(stderr, "%s:%i %i:%s\n",__FILE__,__LINE__,(x),uv_strerror(x));exit((x));}

void on_alloc (uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	assert (handle);
	assert (buf);
	printf("on_alloc: suggested_size=%i\n", (int)suggested_size);
	buf->base = malloc(suggested_size);
	buf->len = (unsigned long) suggested_size;
}


void on_read (uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	assert (stream);
	assert (buf);
	printf ("on_read: nread=%i buf_len=%i\n", (int)nread, (int)buf->len);
	fwrite (buf->base, 1, buf->len, stdout);
}


void on_exit (uv_process_t* process,int64_t exit_status, int term_signal)
{
	printf ("on_exit: exit_status=%i term_signal=%i", (int)exit_status, (int)term_signal);
	uv_close((uv_handle_t*) process, NULL);
}


int main(int argc, char *argv[])
{
	assert (argc);
	assert (argv);
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	int r;
	uv_loop_t *loop;
	uv_pipe_t in;
	uv_pipe_t out;
	//uv_pipe_t out2;

	printf("uv_default_loop\n");
	loop = uv_default_loop();

	uv_process_t process = {0};
	uv_stdio_container_t stdio[2] = {{0}};
	char* args[10] = {NULL};

	//args [0] = "ls";
	//args [1] = NULL;

	args[0] = "C:/arm-q4/bin/arm-none-eabi-gdb";
	args[1] = "--interpreter=mi2";
	args[2] = "C:/Users/Johan/Documents/test_STM32F469IDISCOVERY/main.elf";
	args[3] = "-ex";
	args[4] = "target extended-remote :3333";
	args[5] = "-ex";
	args[6] = "load";


	uv_process_options_t options = {0};
	options.file = args[0];
	options.args = args;
	options.exit_cb = on_exit;


	r = uv_pipe_init(loop, &out, 0);
	UV_ASSERTR (r);

	//r = uv_pipe_init(loop, &out2, 0);
	//UV_ASSERTR (r);

	r = uv_pipe_init(loop, &in, 0);
	UV_ASSERTR (r);

	options.stdio = stdio;
	options.stdio[0].flags = UV_CREATE_PIPE | UV_READABLE_PIPE;
	options.stdio[0].data.stream = (uv_stream_t*)&in;
	options.stdio[1].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
	options.stdio[1].data.stream = (uv_stream_t*)&out;
	//options.stdio[2].flags = UV_INHERIT_FD;
	//options.stdio[2].data.fd = 2;
	//options.stdio[2].data.stream = (uv_stream_t*)&out2;
	//options.stdio[2].data.stream = (uv_stream_t*)&out2;
	options.stdio_count = 2;


	printf("uv_spawn\n");
	r = uv_spawn (loop, &process, &options);
	UV_ASSERTR (r);
	printf ("pid %i\n", process.pid);

	r = uv_read_start((uv_stream_t*) &out, on_alloc, on_read);
	UV_ASSERTR (r);

	//r = uv_read_start((uv_stream_t*) &out2, on_alloc, on_read);
	//UV_ASSERTR (r);

	printf("uv_run\n");
	r = uv_run(loop, UV_RUN_DEFAULT);
	UV_ASSERTR (r);
}
