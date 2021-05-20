#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char *intro = "void quine();\n";
char *out0 = "\nvoid quine() {\n\tchar c[] = {9, 10, 34, 92};\n\tchar *s = \"";
char *out1 = "\";\n\tprintf(s";
char *out2 = ");%c}%c";
char *out3 = ");\n}\n";
char *sbuf;
uint8_t *ibuf;
char *obuf;
uint16_t is = 0;
uint16_t ii = 0;
uint16_t io = 0;
uint16_t ls = 256;
uint16_t li = 256;
uint16_t lo = 256;

void spush(uint8_t c) {
	sbuf[is++] = c;
	if (is == ls) {
		ls *= 2;
		sbuf = realloc(sbuf, ls);
	}
}

void ipush(uint8_t x) {
	ibuf[ii++] = x;
	if (ii == li) {
		li *= 2;
		ibuf = realloc(ibuf, li);
	}
}

void opush(uint8_t c) {
	obuf[io++] = c;
	if (io == lo) {
		lo *= 2;
		obuf = realloc(obuf, lo);
	}
}

void sprintc(uint8_t c) {
	switch (c) {
	case '\t':
		putchar('%');
		putchar('c');
		ipush(0);
		break;
	case '\n':
		putchar('%');
		putchar('c');
		ipush(1);
		break;
	case '\"':
		putchar('%');
		putchar('c');
		ipush(2);
		break;
	case '\\':
		putchar('%');
		putchar('c');
		ipush(3);
		break;
	default:
		putchar(c);
	}
	opush(c);
}

void sprints(char *s) {
	while(*s) {
		sprintc(*(s++));
	}
}

void printc(uint8_t c) {
	switch (c) {
	case '\t':
		spush('%');
		spush('c');
		ipush(0);
		break;
	case '\n':
		spush('%');
		spush('c');
		ipush(1);
		break;
	case '\"':
		spush('%');
		spush('c');
		ipush(2);
		break;
	case '\\':
		spush('%');
		spush('c');
		ipush(3);
		break;
	default:
		spush(c);
	}
	putchar(c);
}

void cprint() {
	for(uint16_t i = 0; i <= ii; i++) {
		if(ibuf[i] == 4) {
			sprints(", s");
			continue;
		}
		sprints(", c[");
		sprintc('0'+ ibuf[i]);
		sprintc(']');
	}
}

int main(int argc, char **argv) {
	sbuf = malloc(256);
	ibuf = malloc(256);
	obuf = malloc(256);
	int c;
	while(*intro) {
		printc(*(intro++));
	}
	while(EOF != (c = getchar())) {
		printc(c);
	}
	while(*out0) {
		printc(*(out0++));
	}
	spush('\0');
	printf("%s", sbuf);
	printf("%%s");
	ipush(4);
	sprints(out1);
	ipush(1);
	ipush(1);
	cprint();
	sprints(out3);
	printf("%s", obuf);
	return 0;
}
