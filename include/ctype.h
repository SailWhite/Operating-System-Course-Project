#ifndef CTYPE_H
#define CTYPE_H

static inline int isprint(char c) {
	return 0x20<=c && c<=0x7e || c=='\n';
}

static inline int isgraph(char c) {
	return 0x20<c && c<=0x7e;
}

static inline int isdigit(char c) {
	return '0'<=c && c<='9';
}

#endif
