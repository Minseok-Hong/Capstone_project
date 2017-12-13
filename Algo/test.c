#include <stdio.h>

int main()

{

	FILE *f;

	f=fopen("a.txt","w+t");
	fflush(f);	
	rewind(f);
	fprintf(f,"123");

	fclose(f);

}



