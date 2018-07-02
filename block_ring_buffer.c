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

#include "block_ring_buffer.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _block_ring_buffer
{
  char *body;
  volatile uint32_t n_write;
  uint32_t b_size;
  uint32_t b_count;
  uint32_t length;
} _block_ring_buffer;

_block_ring_buffer *
block_ring_buffer_new (const uint32_t block_count, const uint32_t block_size)
{
  if (0 == block_count || 0 == block_size)
    return NULL;

  _block_ring_buffer *i =
    (_block_ring_buffer *) malloc (sizeof (_block_ring_buffer));
  if (!i)
    return NULL;

  i->b_size = block_size;
  i->b_count = block_count;
  i->length = block_count * block_size;
  i->n_write = 0;

  i->body = (char *) malloc (sizeof (char) * i->length);
  if (!i->body)
    {
      free (i);
      return NULL;
    }

  return i;
}

void
block_ring_buffer_destory (_block_ring_buffer * buf)
{
  free (buf->body);
  free (buf);
}

uint32_t
block_ring_buffer_write (_block_ring_buffer * buf, char *block, uint32_t count)
{
  assert (buf);
  assert (buf->body);

  if (NULL == block || 0 == count)
    return 0;

  uint32_t bjump = 0;
  uint32_t r_count = count;
  char *r_pb = block;
  char *r_p = buf->body + ((buf->n_write % buf->b_count) * buf->b_size);

  if (count > buf->b_count)
    {
      bjump =
	(((count / buf->b_count) - 1) * buf->b_count) + (count % buf->b_count);
      r_pb = block + bjump * buf->b_size;
      r_count = buf->b_count;
      r_p = buf->body + (((buf->n_write + bjump) % buf->b_count) * buf->b_size);
    }

  uint32_t left;
  if (r_p == buf->body)
    memcpy (r_p, r_pb, (sizeof (char) * buf->b_size * r_count));
  else
    {
      if (r_count > 1 && (buf->n_write + bjump + r_count) > buf->b_count)
	{
	  left = (r_count + buf->n_write + bjump) - buf->b_count;
	  memcpy (r_p, r_pb, (sizeof (char) * buf->b_size) * (r_count - left));
	  memcpy (buf->body, r_pb + (buf->b_size * (r_count - left)),
		  (sizeof (char) * buf->b_size) * left);
	}			/* two step write */
      else
	memcpy (r_p, r_pb, (sizeof (char) * buf->b_size * r_count));
    }

  buf->n_write += count;

  return buf->n_write;
}

int
block_ring_buffer_read (const block_ring_buffer_t buf, char *block,
                        const uint32_t index)
{
  assert (buf);
  assert (block);

  if (0 == buf->n_write)
    return 0;

  uint32_t last = buf->n_write;
  if (index > last)
    return 0;

  if ((last - index) >
      (buf->b_count * BLOCK_RING_BUFFER_PROTECT_AREA_PERCENT / 100 + 1))
    return 0;

  char *p = buf->body + (((index - 1) % buf->b_count) * buf->b_size);
  memcpy (block, p, sizeof (char) * buf->b_size);

  return 1;
}

uint32_t
block_ring_buffer_read_last (const block_ring_buffer_t buf, char *block)
{
  assert (buf);
  assert (block);

  if (0 == buf->n_write)
    return 0;

  uint32_t index = buf->n_write;
  char *p = buf->body + (((index - 1) % buf->b_count) * buf->b_size);
  memcpy (block, p, sizeof (char) * buf->b_size);

  return index;
}

void
block_ring_buffer_clear (_block_ring_buffer * buf)
{
  assert (buf);

  memset (buf->body, '\0', buf->length);
  buf->n_write = 0;
}

uint32_t
block_ring_buffer_dump (_block_ring_buffer * buf, char *t)
{
  assert (buf);
  assert (t);

  uint32_t len = 0;
  if (buf->n_write >= buf->b_count)
    len = buf->length;
  else
    len = buf->n_write * buf->b_size;

  memcpy (t, buf->body, len);

  return len;
}
