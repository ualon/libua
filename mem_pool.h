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

#ifndef __UA_MEM_POOL_H__
#define __UA_MEM_POOL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct _mem_pool *mem_pool_t;

  mem_pool_t mem_pool_new (uint32_t block_size, uint32_t blocks,
                           uint32_t min, uint32_t max);
  void mem_pool_destroy (mem_pool_t p);

  void *mem_pool_alloc (mem_pool_t p, uint32_t * size);
  void mem_pool_free (mem_pool_t p, void *t);

  int mem_pool_is_empty (mem_pool_t p);

#ifdef __cplusplus
}
#endif

#endif				/* !__UA_MEM_POOL_H__ */
