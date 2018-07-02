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

#include "mem_pool.h"

#include <stdlib.h>
#include <assert.h>

typedef struct _mem_pool_element
{
  char **catalog;		/* catalog for blocks */
  char *body;			/* address */
  uint32_t count;
  uint32_t size;
  struct _mem_pool_element *next;
} _mem_pool_element;

static _mem_pool_element *
_elem_new (uint32_t b_num, uint32_t b_size)
{
  if (!b_num || !b_size)
    return NULL;

  uint32_t total =
    sizeof (_mem_pool_element) + b_num * (b_size + sizeof (char *));
  _mem_pool_element *i = (_mem_pool_element *) malloc (total);
  if (!i)
    return NULL;

  i->body = (char *) i + sizeof (_mem_pool_element);
  i->catalog = (char **) (i->body + b_num * b_size);
  i->count = b_num;
  i->size = b_size;

  i->catalog[0] = i->body;
  for (int j = 1; j < b_num; ++j)
    i->catalog[j] = (i->catalog[j - 1] + b_size);

  i->next = NULL;

  return i;
}

static inline void
_elem_destory (_mem_pool_element * e)
{
  free (e);
}

static void *
_elem_alloc (_mem_pool_element * e)
{
  assert (e);

  char *i = NULL;
  int j = 0;
  while (j < e->count)
    {
      if (e->catalog[j])
	{
	  i = e->catalog[j];
	  e->catalog[j] = NULL;
	  break;
	}
      j++;
    }

  return i;
}

static int
_elem_free (_mem_pool_element * e, char *t)
{
  assert (e);

  if (!t)
    return 0;
  if (t < e->body)
    return 0;
  if ((t - e->body) % e->size)
    return 0;
  if ((t - e->body) / e->size > e->count)
    return 0;

  int i = 0;
  while (i < e->count)
    {
      if (!e->catalog[i])
	{
	  e->catalog[i] = t;
	  break;
	}
      i++;
    }

  return 1;
}

static inline int
_elem_is_empty (_mem_pool_element * e)
{
  assert (e);

  for (int i = 0; i < e->count; ++i)
    {
      if (!e->catalog[i])
	return 0;
    }
  return 1;
}

typedef struct _mem_pool
{
  uint32_t min_elements;	/* mininum number of blocks */
  uint32_t max_elements;	/* maxinum number of blocks */

  uint32_t num_block;		/* num of blocks per element */
  uint32_t block_size;

  int limit;
  _mem_pool_element *curr_e;

  uint32_t length;		/* length of element list */
  _mem_pool_element *start;
} _mem_pool;

_mem_pool *
mem_pool_new (uint32_t block_size, uint32_t blocks, uint32_t min, uint32_t max)
{
  _mem_pool *i = (_mem_pool *) malloc (sizeof (_mem_pool));
  if (!i)
    return NULL;

  i->start = _elem_new (blocks, block_size);
  i->length = 1;

  i->min_elements = min;
  if (max)
    {
      i->limit = 1;
      i->max_elements = max > min ? max : min;
    }
  else
    {
      i->limit = 0;
      i->max_elements = 0;
    }
  i->block_size = block_size;
  i->num_block = blocks;

  i->curr_e = i->start;

  _mem_pool_element *e = i->start;
  for (int j = 1; j < min; ++j)
    {
      e->next = _elem_new (blocks, block_size);
      e = e->next;
      i->length++;
    }

  return i;
}

void
mem_pool_destroy (_mem_pool * p)
{
  _mem_pool_element *i = p->start;
  _mem_pool_element *n = NULL;

  for (; NULL != i; i = n)
    {
      n = i->next;
      _elem_destory (i);
    }

  free (p);
}

int
mem_pool_is_empty (_mem_pool * p)
{
  _mem_pool_element *i = p->start;
  _mem_pool_element *n = NULL;

  for (; NULL != i; i = n)
    {
      n = i->next;
      if (!_elem_is_empty (i))
	return 0;
    }

  return 1;
}

void *
mem_pool_alloc (_mem_pool * p, uint32_t * size)
{
  assert (p);

  char *target = NULL;

  while (NULL == (target = _elem_alloc (p->curr_e)))
    {
      if (NULL == p->curr_e->next)
	break;
      p->curr_e = p->curr_e->next;
    }

  if (!target)
    {
      if (p->limit)
	{
	  if (p->length == p->max_elements)
	    return NULL;
	}
      p->curr_e->next = _elem_new (p->num_block, p->block_size);
      ++p->length;
      p->curr_e = p->curr_e->next;
      target = _elem_alloc (p->curr_e);
    }

  if (size && target)
    *size = p->block_size;

  return (void *) target;
}

void
mem_pool_free (_mem_pool * p, void *t)
{
  _mem_pool_element *e = p->start;

  int i = 0;
  while (e)
    {
      if ((i = _elem_free (e, (char *) t)))
	break;
      e = e->next;
    }
  assert (i);
  assert (e);

  if (e < p->curr_e)
    p->curr_e = e;
}
