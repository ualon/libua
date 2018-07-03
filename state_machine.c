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

#include "state_machine.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef STATE_MACHINE_WITH_MUTEX
#include <pthread.h>
#endif

typedef struct _state_machine
{
  state_e curr;
  int inited;

  const char *name[_state_num_];
  void *arr_func[(_state_num_ - 1) * 2];
  void *arr_para[(_state_num_ - 1) * 2];

#ifdef STATE_MACHINE_WITH_MUTEX
  pthread_mutex_t *_pMutex;
  pthread_mutexattr_t *_pMutexAttr;
#endif
} _state_machine;

_state_machine *
state_machine_new (void)
{
  _state_machine *i = (_state_machine *) malloc (sizeof (_state_machine));
  if (!i)
    return NULL;

#ifdef STATE_MACHINE_WITH_MUTEX
  i->_pMutex = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  if (!i->_pMutex)
    {
      free (i);
      return NULL;
    }

  i->_pMutexAttr =
    (pthread_mutexattr_t *) malloc (sizeof (pthread_mutexattr_t));
  if (!i->_pMutexAttr)
    {
      free (i->_pMutex);
      free (i);
      return NULL;
    }
  pthread_mutexattr_init (i->_pMutexAttr);
  pthread_mutexattr_settype (i->_pMutexAttr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init (i->_pMutex, i->_pMutexAttr);
#endif

  memset (i->name, 0x00, sizeof (char *) * (_state_num_));

  memset (i->arr_func, 0x00, sizeof (void *) * (_state_num_ - 1) * 2);
  memset (i->arr_para, 0x00, sizeof (void *) * (_state_num_ - 1) * 2);

  i->inited = 0;
  i->curr = 0;

  return i;
}

void
state_machine_destory (_state_machine * sm)
{
#ifdef STATE_MACHINE_WITH_MUTEX
  pthread_mutex_destroy (sm->_pMutex);
  free (sm->_pMutexAttr);
  free (sm->_pMutex);
#endif
  free (sm);
}

void
state_machine_regiest_name (_state_machine * sm, state_e st, const char *name)
{
  if (st < _state_num_)
    sm->name[st] = name;
}

int
state_machine_regiest_func (_state_machine * sm,
			    int (*func) (void *para), void *para,
                state_e from, state_e to)
{
  assert (sm);
  assert (func);

  if ((from == to) || from >= _state_num_ || to >= _state_num_)
    return 0;

  int dif = to - from;
  if (dif > 0)
    {
      if (dif != 1)
	return 0;
      sm->arr_func[from * 2] = func;
      sm->arr_para[from * 2] = para;
    }
  else
    {
      if (dif != -1)
	return 0;
      sm->arr_func[to * 2 + 1] = func;
      sm->arr_para[to * 2 + 1] = para;
    }

  return 1;
}

int
state_machine_do_change (_state_machine * sm, state_e from, state_e to)
{
  assert (sm);

  if ((from == to) || from >= _state_num_ || to >= _state_num_)
    return 0;

  if (from != sm->curr)
    return 0;

  if (!sm->inited)
    sm->inited = state_machine_is_inited (sm);
  if (!sm->inited)
    return 0;

  int rt = 1;

  int dif = to - from;
  int cur;

#ifdef STATE_MACHINE_WITH_MUTEX
  pthread_mutex_lock (sm->_pMutex);
#endif
  if (dif > 0)
    {
      cur = from;
      while (cur != to)
	{
	  if (NULL == sm->arr_func[cur * 2])
	    {
	      rt = 0;
	      goto err;
	    }

	  int (*func) (void *) = sm->arr_func[cur * 2];
	  rt = func (sm->arr_para[cur * 2]);
	  if (!rt)
	    goto err;

	  cur++;
	}
    }
  else
    {
      cur = from;
      while (cur != to)
	{
	  if (NULL == sm->arr_func[cur * 2 - 1])
	    {
	      rt = 0;
	      goto err;
	    }

	  int (*func) (void *) = sm->arr_func[cur * 2 - 1];
	  rt = func (sm->arr_para[cur * 2 - 1]);
	  if (!rt)
	    goto err;

	  cur--;
	}
    }
  sm->curr = cur;

err:

#ifdef STATE_MACHINE_WITH_MUTEX
  pthread_mutex_unlock (sm->_pMutex);
#endif

  return rt;
}

const char *
state_machine_get_current (_state_machine * sm)
{
  assert (sm);
  return sm->name[sm->curr];
}

int
state_machine_is_inited (_state_machine * sm)
{
  assert (sm);
  int i;

  /* name array */
  for (i = 0; i < _state_num_; ++i)
    {
      if (NULL == sm->name[i])
	{
	  sm->inited = 0;
	  return 0;
	}
    }

  /* func array */
  for (i = 0; i < (_state_num_ - 1) * 2; ++i)
    {
      if (NULL == sm->arr_func)
	{
	  sm->inited = 0;
	  return 0;
	}
    }
  sm->inited = 1;

  return 1;
}
