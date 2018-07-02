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
    _ua_state_num_,
  } ua_state;

  typedef struct _ua_state_machine *ua_state_machine_t;

  ua_state_machine_t ua_state_machine_new (void);
  void ua_state_machine_destory (ua_state_machine_t sm);

  int ua_state_machine_regiest_func (ua_state_machine_t sm,
				     int (*func) (void *para),
				     void *para, ua_state from, ua_state to);

  void ua_state_machine_regiest_name (ua_state_machine_t sm,
				      ua_state st, const char *name);

  int ua_state_machine_do_change (ua_state_machine_t sm,
				  ua_state from, ua_state to);

  const char *ua_state_machine_get_current (ua_state_machine_t sm);

  int ua_state_machine_is_inited (ua_state_machine_t sm);

#ifdef __cplusplus
}
#endif
#endif				/* __UA_STATE_MACHINE_H__ */
