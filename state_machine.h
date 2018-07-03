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

#ifndef __UA_STATE_MACHINE_H__
#define __UA_STATE_MACHINE_H__

#include <stdint.h>

/*
 * default not with mutex(pmutex),
 * if need it, define STATE_MACHINE_WITH_MUTEX
 */

#ifdef __cplusplus
extern "C"
{
#endif

  /* becareful the order */
  typedef enum
  {
    UA_STATE_NULL = 0,
    UA_STATE_READY,		/* memory allocatd */
    UA_STATE_PAUSED,		/* pasue source */
    UA_STATE_RUNNING,
    _state_num_,
  } state_e;

  typedef struct _state_machine *state_machine_t;

  state_machine_t state_machine_new (void);
  void state_machine_destory (state_machine_t sm);

  int state_machine_regiest_func (state_machine_t sm,
				  int (*func) (void *para),
                  void *para, state_e from, state_e to);

  void state_machine_regiest_name (state_machine_t sm,
                   state_e st, const char *name);

  int state_machine_do_change (state_machine_t sm, state_e from, state_e to);

  const char *state_machine_get_current (state_machine_t sm);

  int state_machine_is_inited (state_machine_t sm);

#ifdef __cplusplus
}
#endif
#endif				/* __UA_STATE_MACHINE_H__ */
