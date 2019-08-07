#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//void __gcov_flush(void);

int main(int argc, char *argv[])
{

	if (rand () % 2)
	{
		printf ("Hello\n");
	}
	else
	{
		printf ("Bye!\n");
	}
	//__gcov_flush();
}
