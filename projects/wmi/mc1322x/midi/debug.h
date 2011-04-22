/* P() macro can be used if on-chip
 * debugger is not available */
#define P() printf("line: %d", __LINE__)

/* Breaker function for gdb */
volatile void dbg(void);

/* To desable INFO, set it to -1 */

#   if INFO >= 0
#   define info0(...) printf(__VA_ARGS__)
#   else
#   define info0(...)
#   endif

#   if INFO >= 1
#   define info1(...) printf(__VA_ARGS__)
#   else
#   define info1(...)
#   endif

#   if INFO >= 2
#   define info2(...) printf(__VA_ARGS__)
#   else
#   define info2(...)
#   endif

/* set STAT to 0 for no stats,
 * and set it to 1 for basic
 * current rate couter. When
 * set to 2 - lost and sent
 * counters are enabled.
*/
#if STAT == 1

#define incr_stat(x) do { \
	x->stat += x->size; \
	printf("\n%lu\t++stat= %d\n", clock_time(), x->stat); \
} while(0)

#define decr_stat(x) do { \
	x->stat -= x->size; \
	printf("\n%lu\t--stat= %d\n", clock_time(), x->stat); \
} while(0)

#define zero_stat(x) do { \
	x->stat = 0; \
	printf("\n%lu\t..stat= %d\n", clock_time(), x->stat); \
} while(0)

#define norm_stat(x) do { \
	x->stat = x->size; \
	printf("\n%lu\t**stat= %d\n", clock_time(), x->stat); \
} while(0)

#define stat_lost(x)
#define stat_sent(x)
#define zero_sent(x)
#define zero_lost(x)

#define stat_init(x) do { \
	zero_stat(x); \
} while(0)

#elif STAT == 2

#define incr_stat(x) do { \
	x->stat += x->size; \
	printf("\n%lu\t++stat= %d\n", clock_time(), x->stat); \
} while(0)

#define decr_stat(x) do { \
	x->stat -= x->size; \
	printf("\n%lu\t--stat= %d\n", clock_time(), x->stat); \
} while(0)

#define zero_stat(x) do { \
	x->stat = 0; \
	printf("\n%lu\t..stat= %d\n", clock_time(), x->stat); \
} while(0)

#define norm_stat(x) do { \
	x->stat = x->size; \
	printf("\n%lu\t**stat= %d\n", clock_time(), x->stat); \
} while(0)


#define stat_lost(x) do { \
	x->lost += x->size; \
	printf("\n%lu\t\t< lost= %d >\n", clock_time(), x->lost); \
} while(0)

#define stat_sent(x) do { \
	x->sent += x->size; \
	printf("\n%lu\t\t< sent= %d >\n", clock_time(), x->sent); \
} while(0)

#define zero_sent(x) do { \
	x->sent = 0; \
	printf("\n%lu\t..sent= %d\n", clock_time(), x->sent); \
} while(0)

#define zero_lost(x) do { \
	x->lost = 0; \
	printf("\n%lu\t..lost= %d\n", clock_time(), x->lost); \
} while(0)

#define stat_init(x) do { \
	zero_stat(x); \
	zero_sent(x); \
	zero_lost(x); \
} while(0)

#else
#  define incr_stat(x)
#  define decr_stat(x)
#  define zero_stat(x)
#  define norm_stat(x)
#  define stat_init(x)
#  define stat_sent(x)
#  define stat_lost(x)
#endif
