#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMOIZED 100
#define ARRAY_SIZE (MAX_MEMOIZED + 1)
#define NO_VALUE_YET -1

typedef long long (*long_long_func_ptr)(int param);
long_long_func_ptr fibonacci_provider;
long long fibonacci(int of_num) {
  printf(__FILE__ ":%2d fibonacci(%d) called\n", __LINE__, of_num);
  if (of_num < 2) {
    return of_num;
  } else {
    return (*fibonacci_provider)(of_num - 1) +
           (*fibonacci_provider)(of_num - 2);
  }
}

long_long_func_ptr _original_provider;
long long _memoization_data[ARRAY_SIZE];

long long cache_func(int of_num) {
  printf(__FILE__ ":%2d   cache_func(%d) called\n", __LINE__, of_num);

  if (of_num > MAX_MEMOIZED)
    return (*_original_provider)(of_num);

  if (_memoization_data[of_num] == NO_VALUE_YET)
    _memoization_data[of_num] = (*_original_provider)(of_num);

  return _memoization_data[of_num];
}

long_long_func_ptr init_cache(long_long_func_ptr real_provider) {
  for (int ix = 0; ix < ARRAY_SIZE; ix++)
    _memoization_data[ix] = NO_VALUE_YET;

  _original_provider = real_provider;
  return cache_func;
}

int main(int argc, char *argv[]) {
  int test_val;

  if (argc < 2 || 1 != sscanf(argv[1], "%d", &test_val) || test_val < 0)
    return 1;

  fibonacci_provider = init_cache(fibonacci);

  for (int xx = test_val; xx > 0; xx--) {
    printf(__FILE__ ":%2d fibonacci(%d) = %lld\n", __LINE__, xx,
           (*fibonacci_provider)(xx));
  }

  return 0;
}
