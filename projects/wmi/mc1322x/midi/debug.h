#define P() printf("line: %d", __LINE__)

#   if defined(INFO)
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

#if STAT
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
	printf("\n%lu\t\t++lost= %d\n", clock_time(), x->lost); \
} while(0)

#define stat_sent(x) do { \
	x->sent += x->size; \
	printf("\n%lu\t\t++sent= %d\n", clock_time(), x->sent); \
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
