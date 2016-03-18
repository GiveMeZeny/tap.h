#ifndef TAP_H
#define TAP_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int num_tests, num_nots, num_planned;

/* variadic wrappers */
#define ok(...) okt(__VA_ARGS__, "")
#define is(...) ist(__VA_ARGS__, "")
#define isnt(...) isntt(__VA_ARGS__, "")
#define isstr(...) isstrt(__VA_ARGS__, "")
#define isntstr(...) isntstrt(__VA_ARGS__, "")
#define ismem(...) ismemt(__VA_ARGS__, "")
#define isntmem(...) isntmemt(__VA_ARGS__, "")
#define cmp_ok(...) cmp_okt(__VA_ARGS__, "")
#define done_testing(...) done_testing((__VA_ARGS__ ## 0) / 10)

/* real macros */
#define okt(test, ...) \
	(ok)((test) ? 1 : 0, __FILE__, __LINE__, __VA_ARGS__)

#define ist(a, b, fmt, ...)                                             \
	(!ok((a) == (b), __VA_ARGS__) ?                                 \
	 (fprintf(stderr,                                               \
	          "#          got: '"fmt"'\n#     expected: '"fmt"'\n", \
	          (a), (b)), 0) : 1)

#define isntt(a, b, fmt, ...)                                                 \
	(!ok((a) != (b), __VA_ARGS__) ?                                       \
	 (fprintf(stderr,                                                     \
	          "#          got: '"fmt"'\n#     expected: anything else\n", \
	          (a)), 0) : 1)

#define isstrt(a, b, ...)                                                 \
	(!ok(!strcmp((a), (b)), __VA_ARGS__) ?                            \
	 (fprintf(stderr, "#          got: '%s'\n#     expected: '%s'\n", \
	          (a), (b)), 0) : 1)

#define isntstrt(a, b, ...)                                                \
	(!ok(strcmp((a), (b)), __VA_ARGS__) ?                              \
	 (fprintf(stderr,                                                  \
	          "#          got: '%s'\n#     expected: anything else\n", \
	          (a)), 0) : 1)

#define ismemt(a, b, size, ...)                        \
	(!ok(!memcmp((a), (b), (size)), __VA_ARGS__) ? \
	 (diagmem((a), (b), (size)), 0) : 1)

#define isntmemt(a, b, size, ...)                     \
	(!ok(memcmp((a), (b), (size)), __VA_ARGS__) ? \
	 (diagmem((a), NULL, (size)), 0) : 1)

#define cmp_okt(a, op, b, fmt, ...)                                  \
	(!ok((a) op (b), __VA_ARGS__) ?                              \
	 (fprintf(stderr,                                            \
	          "#     '"fmt"'\n#         "#op"\n#     '"fmt"'\n", \
	          (a), (b)), 0) : 1)

static inline void BAIL_OUT(const char *format, ...)
{
	va_list ap;
	char buf[512];
	int len;

	va_start(ap, format);
	len = vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);
	printf("Bail out!");
	if (len) {
		printf("  ");
		fwrite(buf, 1, len, stdout);
	}
	puts("");
	exit(255);
}

static inline void plan(int num)
{
	printf("1..%d\n", num);
	num_planned = num;
}

static inline int (ok)(int test, const char *filename, int line,
                       const char *format, ...)
{
	va_list ap;
	char buf[512], *pos = strrchr(filename, '/');
	int len;

	va_start(ap, format);
	len = vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);
	if (!test)
		printf("not ");
	printf("ok %d", ++num_tests);
	if (len) {
		printf(" - ");
		fwrite(buf, 1, len, stdout);
	}
	puts("");
	if (!test) {
		const char *harness_active = getenv("HARNESS_ACTIVE");

		if (harness_active && strtol(harness_active, NULL, 10) == 1)
			fprintf(stderr, "\n");
		if (len)
			fprintf(stderr, "#   Failed test '%s'\n#   ", buf);
		else
			fprintf(stderr, "#   Failed test ");
		fprintf(stderr, "at %s line %d.\n", pos ? pos + 1 : filename,
		                                    line);
		num_nots++;
	}
	return test;
}

static inline void (done_testing)(int num_expected)
{
	if (!num_planned) {
		plan(num_expected ? num_expected : num_tests);
	} else {
		if (num_expected && num_expected != num_planned)
			ok(0, "planned to run %d but done_testing() expects %d",
			   num_planned, num_expected);
	}
	if (num_tests != num_planned)
		fprintf(stderr,
		        "# Looks like you planned %d test%s but ran %d.\n",
		        num_planned, num_planned > 1 ? "s" : "", num_tests);
	if (num_nots)
		fprintf(stderr,
		        "# Looks like you failed %d test%s of %d run.\n",
		        num_nots, num_nots > 1 ? "s" : "", num_tests);
	exit(num_tests != num_planned ? 255 : num_nots < 255 ? num_nots : 254);
}

static inline void diagmemprint(const void *ptr, size_t size)
{
	const unsigned char *buf = ptr;
	size_t i;

	for (i = 0; i < size; i++) {
		if (i)
			fprintf(stderr, " ");
		fprintf(stderr, "%02x", (unsigned int)*buf++);
	}
}

static inline void diagmem(const void *a, const void *b, size_t size)
{
	fprintf(stderr, "#          got: '");
	diagmemprint(a, size);
	fprintf(stderr, "'\n#     expected: ");
	if (b) {
		fprintf(stderr, "'");
		diagmemprint(b, size);
		fprintf(stderr, "'");
	} else {
		fprintf(stderr, "anything else");
	}
	fprintf(stderr, "\n");
}

#endif /* TAP_H */
