/* The contents of this file are subject to the The 3-Clause BSD
 * License (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at https://opensource.org/licenses/BSD-3-Clause
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * Contributor(s):  Wang Bodong <wang.bodong@hotmail.com>
 *
 * Copyright Contributors. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem_pool.h"

typedef struct
{
  const char *name;
  int passed;

  uint32_t block_size;
  uint32_t blocks;
  uint32_t min;
  uint32_t max;
  uint32_t times;
} pool_test_case;

static void
test_pool (pool_test_case * tc)
{
  mem_pool_t pool = mem_pool_new (tc->block_size, tc->blocks,
				  tc->min, tc->max);

  tc->passed = 1;
  void *alloced[tc->times];
  memset (alloced, 0, tc->times * sizeof (void *));

  uint32_t size_got;
  for (int i = 0; i < tc->times; ++i)
    {
      alloced[i] = mem_pool_alloc (pool, &size_got);
      if (!alloced[i] || tc->block_size != size_got)
	tc->passed = 0;
    }

  if (tc->passed)
    {
      for (int i = 0; i < tc->times; ++i)
	mem_pool_free (pool, alloced[i]);

      if (!mem_pool_is_empty (pool))
	tc->passed = 0;
    }
  mem_pool_destroy (pool);
}

static inline void
print_result (pool_test_case * tc)
{
  if (tc->passed)
    printf ("    Test Case:    %s -------- passed\n", tc->name);
  else
    printf ("    Test Case:    %s -------- failed\n", tc->name);
}

int
main (int argc, char *argv[])
{
  printf ("Memory-Pool Test:\n");
  pool_test_case case_1 = { "basic       ", 0, 4, 8, 1, 1, 8 };
  test_pool (&case_1);
  print_result (&case_1);

  pool_test_case case_2 = { "size_limit  ", 0, 4, 8, 1, 1, 9 };
  test_pool (&case_2);
  case_2.passed = !case_2.passed;
  print_result (&case_2);

  pool_test_case case_3 = { "default     ", 0, 4, 8, 2, 0, 35 };
  test_pool (&case_3);
  print_result (&case_3);

  pool_test_case case_4 = { "huge        ", 0, 37, 32, 4, 0, 2048 };
  test_pool (&case_4);
  print_result (&case_4);

  printf ("Memory-Pool Test Finished\n");

  return 0;
}
