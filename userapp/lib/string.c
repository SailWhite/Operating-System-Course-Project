#include<string.h>

/*
 * sizeof(word) MUST BE A POWER OF TWO
 * SO THAT wmask BELOW IS ALL ONES
 */
typedef	long word;		/* "word" used for optimal copy speed */

#define	wsize	sizeof(word)
#define	wmask	(wsize - 1)

/*
 * Copy a block of memory, not handling overlap.
 */
void *memcpy(void *dst0, const void *src0, size_t length) {
	char *dst = dst0;
	const char *src = src0;
	size_t t;

	if (length == 0 || dst == src)		/* nothing to do */
		goto done;

	/*
	 * Macros: loop-t-times; and loop-t-times, t>0
	 */
#define	TLOOP(s) if (t) TLOOP1(s)
#define	TLOOP1(s) do { s; } while (--t)

	/*
	 * Copy forward.
	 */
	t = (long)src;	/* only need low bits */
	if ((t | (long)dst) & wmask) {
		/*
		 * Try to align operands.  This cannot be done
		 * unless the low bits match.
		 */
		if ((t ^ (long)dst) & wmask || length < wsize)
			t = length;
		else
			t = wsize - (t & wmask);
		length -= t;
		TLOOP1(*dst++ = *src++);
	}
	/*
	 * Copy whole words, then mop up any trailing bytes.
	 */
	t = length / wsize;
	TLOOP(*(word *)dst = *(word *)src; src += wsize; dst += wsize);
	t = length & wmask;
	TLOOP(*dst++ = *src++);
done:
	return (dst0);
}

void *memset(void *dst, int c, size_t n) {
	if (n != 0) {
		unsigned char *d = dst;

		do
			*d++ = (unsigned char)c;
		while (--n != 0);
	}
	return (dst);
}

int strcmp(unsigned char *s1, unsigned char *s2)
{
   //assert((NULL != dest) && (NULL != source));
   while (*s1 && *s2 && (*s1 == *s2)) {
       s1 ++;
       s2 ++;
   }
   return *s1 - *s2;
}

