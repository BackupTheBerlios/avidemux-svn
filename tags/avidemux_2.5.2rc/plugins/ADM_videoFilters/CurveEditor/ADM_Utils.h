#ifndef UTILS_H
#define UTILS_H

#define PRINT_VAR(x) printf("[%s:%d] "#x"=%d (0x%X)\n", __FILE__, __LINE__, x, x);

//#ifdef __DEBUG__
#define PRINT_BEGIN() printf("BEGIN[%s:%d]\n", __FILE__, __LINE__);
#define PRINT_MARK() printf("MARK[%s:%d]\n", __FILE__, __LINE__);
#define PRINT_END() printf("END[%s:%d]\n", __FILE__, __LINE__);
/*#else
#define PRINT_BEGIN()
#define PRINT_MARK()
#define PRINT_END()
#endif*/

#define ROUND(x) ((x)>=0 ? (int)((x) + 0.5f) : (int)((x) - 0.5f))

#define BOUNDS(index, max) ((index) >= 0 && (index) <= (max))

#endif

