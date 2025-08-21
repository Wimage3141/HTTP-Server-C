#include <stdio.h>

int main() {
	FILE *fp;
	fp = fopen("test.txt", "r");

	fputs("testtest\ntest", fp);
	fclose(fp);
}