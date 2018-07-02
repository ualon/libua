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

#ifndef __UA_BLOCK_RING_BUFFER_H__
#define __UA_BLOCK_RING_BUFFER_H__

#include <stdint.h>

#define UA_BLOCK_RING_BUFFER_PROTECT_AREA_PERCENT  (30)

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct _ua_block_ring_buffer *ua_block_ring_buffer_t;

  ua_block_ring_buffer_t ua_block_ring_buffer_new (const uint32_t block_count,
						   const uint32_t block_size);
  void ua_block_ring_buffer_destory (ua_block_ring_buffer_t buf);
  void ua_block_ring_buffer_clear (ua_block_ring_buffer_t buf);
  uint32_t ua_block_ring_buffer_dump (ua_block_ring_buffer_t buf, char *t);

  uint32_t ua_block_ring_buffer_write (ua_block_ring_buffer_t buf, char *block,
				       uint32_t count);
  int ua_block_ring_buffer_read (const ua_block_ring_buffer_t buf, char *block,
				 const uint32_t index);
  uint32_t ua_block_ring_buffer_read_last (const ua_block_ring_buffer_t buf,
					   char *block);
  uint32_t ua_block_ring_buffer_last_index (const ua_block_ring_buffer_t buf);

#ifdef __cplusplus
}
#endif

#endif				/* !__UA_BLOCK_RING_BUFFER_H__ */
