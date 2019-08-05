#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <assert.h>

#define UV_ASSERTR(x) if((x)<0){fprintf(stderr, "%s:%i %i:%s\n",__FILE__,__LINE__,(x),uv_strerror(x));exit((x));}


int main(int argc, char *argv[])
{
	assert (argc);
	assert (argv);
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	static uv_fs_t scandir_req;
	int r = uv_fs_scandir (NULL, &scandir_req, "../test", 0, NULL);
	UV_ASSERTR (r);
	uv_dirent_t dent;
	while (UV_EOF != uv_fs_scandir_next (&scandir_req, &dent))
	{
		printf ("%s\n", dent.name);
	}
}
