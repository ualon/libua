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
#include "string.h"
#include "stdio.h"

typedef struct
{
  char str[16];
} testContex_t;

int _cb_state_null2ready (void *para);
int _cb_state_ready2null (void *para);

int _cb_state_ready2pause (void *para);
int _cb_state_pause2ready (void *para);

int _cb_state_pause2running (void *para);
int _cb_state_running2pause (void *para);

int
main (int argc, char *argv[])
{
  testContex_t ctx;
  strcpy (ctx.str, "iContext");

  state_machine_t staMachine = state_machine_new ();

  state_machine_regiest_name (staMachine, UA_STATE_NULL, "NULL");
  state_machine_regiest_name (staMachine, UA_STATE_READY, "READY");
  state_machine_regiest_name (staMachine, UA_STATE_PAUSED, "PAUSED");
  state_machine_regiest_name (staMachine, UA_STATE_RUNNING, "RUNNING");

  state_machine_regiest_func (staMachine,
			      _cb_state_null2ready, &ctx,
			      UA_STATE_NULL, UA_STATE_READY);
  state_machine_regiest_func (staMachine,
			      _cb_state_ready2null, &ctx,
			      UA_STATE_READY, UA_STATE_NULL);

  state_machine_regiest_func (staMachine,
			      _cb_state_ready2pause, &ctx,
			      UA_STATE_READY, UA_STATE_PAUSED);
  state_machine_regiest_func (staMachine,
			      _cb_state_pause2ready, &ctx,
			      UA_STATE_PAUSED, UA_STATE_READY);

  state_machine_regiest_func (staMachine,
			      _cb_state_pause2running, &ctx,
			      UA_STATE_PAUSED, UA_STATE_RUNNING);
  state_machine_regiest_func (staMachine,
			      _cb_state_running2pause, &ctx,
			      UA_STATE_RUNNING, UA_STATE_PAUSED);

  if (!state_machine_is_inited (staMachine))
    return -1;

  printf ("\nforword\n");
  printf ("[S] : %s\n", state_machine_get_current (staMachine));
  state_machine_do_change (staMachine, UA_STATE_NULL, UA_STATE_READY);

  printf ("[S] : %s\n", state_machine_get_current (staMachine));
  state_machine_do_change (staMachine, UA_STATE_READY, UA_STATE_PAUSED);
  printf ("[S] : %s\n", state_machine_get_current (staMachine));
  state_machine_do_change (staMachine, UA_STATE_PAUSED, UA_STATE_RUNNING);
  printf ("[S] : %s\n", state_machine_get_current (staMachine));

  printf ("\nbackword\n");
  state_machine_do_change (staMachine, UA_STATE_RUNNING, UA_STATE_PAUSED);
  printf ("[S] : %s\n", state_machine_get_current (staMachine));
  state_machine_do_change (staMachine, UA_STATE_PAUSED, UA_STATE_READY);
  printf ("[S] : %s\n", state_machine_get_current (staMachine));
  state_machine_do_change (staMachine, UA_STATE_READY, UA_STATE_NULL);
  printf ("[S] : %s\n", state_machine_get_current (staMachine));

  printf ("\njump\n");
  printf ("[S] : %s\n", state_machine_get_current (staMachine));
  state_machine_do_change (staMachine, UA_STATE_NULL, UA_STATE_RUNNING);
  printf ("[S] : %s\n", state_machine_get_current (staMachine));
  state_machine_do_change (staMachine, UA_STATE_RUNNING, UA_STATE_NULL);
  printf ("[S] : %s\n", state_machine_get_current (staMachine));

  state_machine_destory (staMachine);
  return 0;
}

int
_cb_state_null2ready (void *para)
{
  testContex_t *ctx = (testContex_t *) para;
  printf ("I'm null --> ready, using '%s'\n", ctx->str);
  return 1;
}

int
_cb_state_ready2null (void *para)
{
  testContex_t *ctx = (testContex_t *) para;
  printf ("I'm ready --> null, using '%s'\n", ctx->str);
  return 1;
}

int
_cb_state_ready2pause (void *para)
{
  testContex_t *ctx = (testContex_t *) para;
  printf ("I'm ready --> paused, using '%s'\n", ctx->str);
  return 1;
}

int
_cb_state_pause2ready (void *para)
{
  testContex_t *ctx = (testContex_t *) para;
  printf ("I'm paused --> ready, using '%s'\n", ctx->str);
  return 1;
}

int
_cb_state_pause2running (void *para)
{
  testContex_t *ctx = (testContex_t *) para;
  printf ("I'm paused --> running, using '%s'\n", ctx->str);
  return 1;
}

int
_cb_state_running2pause (void *para)
{
  testContex_t *ctx = (testContex_t *) para;
  printf ("I'm running --> paused, using '%s'\n", ctx->str);
  return 1;
}
