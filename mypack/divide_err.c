#include <limits.h>
#include <stdio.h>

int main(){


	int a = INT_MIN/(-1);
	printf("a = %d\n",a);
	int b  = 0;
	a = a/b;
	printf("a = %d\n",a);
}
