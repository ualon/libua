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
#include <stdio.h>

int
main (int argc, char *argv[])
{
  char a[9], b[9], c[9], d[9], e[9], f[9];

  memcpy (a, "{aaaaaaa}", sizeof (char) * 9);
  memcpy (b, "{bbbbbbb}", sizeof (char) * 9);
  memcpy (c, "{ccccccc}", sizeof (char) * 9);
  memcpy (d, "{ddddddd}", sizeof (char) * 9);
  memcpy (e, "{eeeeeee}", sizeof (char) * 9);
  memcpy (f, "{fffffff}", sizeof (char) * 9);

  char ab[18];
  memcpy (ab, a, sizeof (char) * 9);
  memcpy (ab + 9, b, sizeof (char) * 9);

  char abc[27];
  memcpy (abc, a, sizeof (char) * 9);
  memcpy (abc + 9, b, sizeof (char) * 9);
  memcpy (abc + 18, c, sizeof (char) * 9);

  char def[27];
  memcpy (def, d, sizeof (char) * 9);
  memcpy (def + 9, e, sizeof (char) * 9);
  memcpy (def + 18, f, sizeof (char) * 9);

  char abcdef[54];
  memcpy (abcdef, abc, sizeof (char) * 27);
  memcpy (abcdef + 27, def, sizeof (char) * 27);

  char result[64];
  memset (result, '\0', 64);

  const uint32_t buffer_size = 5;
  const uint32_t block_size = 9;

  block_ring_buffer_t rf = block_ring_buffer_new (buffer_size,
						  block_size);
  printf ("buf settings: size: %u, bleng: %u\n\n", buffer_size, block_size);

  char rfMonitor[50];
  memset (rfMonitor, '\0', 50);

  uint32_t writed = 0;

  /* case 1 */
  block_ring_buffer_write (rf, a, 1);
  writed++;
  block_ring_buffer_read_last (rf, result);
  printf ("a in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_write (rf, b, 1);
  writed++;
  block_ring_buffer_read_last (rf, result);
  printf ("b in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_write (rf, c, 1);
  writed++;
  block_ring_buffer_read_last (rf, result);
  printf ("c in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_write (rf, d, 1);
  writed++;
  block_ring_buffer_read_last (rf, result);
  printf ("d in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_write (rf, e, 1);
  writed++;
  block_ring_buffer_read_last (rf, result);
  printf ("e in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_write (rf, f, 1);
  writed++;
  block_ring_buffer_read_last (rf, result);
  printf ("f in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_clear (rf);
  writed = 0;
  printf ("clear \n\n");

  /* case 2 */
  block_ring_buffer_write (rf, abc, 3);
  writed += 3;
  block_ring_buffer_read_last (rf, result);
  printf ("'abc' in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_write (rf, def, 3);
  writed += 3;
  block_ring_buffer_read_last (rf, result);
  printf ("'def' in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_clear (rf);
  writed += 3;
  printf ("clear \n\n");

  /* case 3 */
  block_ring_buffer_write (rf, abcdef, 6);
  writed += 6;
  block_ring_buffer_read_last (rf, result);
  printf ("'abcdef' in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_clear (rf);
  writed = 0;
  printf ("clear \n\n");

  /* case 4 */
  block_ring_buffer_write (rf, a, 1);
  writed += 1;
  block_ring_buffer_write (rf, abcdef, 6);
  writed += 6;
  block_ring_buffer_read_last (rf, result);
  printf ("'a + abcdef' in, read_last: %s\n", result);
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  block_ring_buffer_clear (rf);
  writed = 0;
  printf ("clear \n\n");

  /* case 5 */
  block_ring_buffer_write (rf, abcdef, 6);
  writed += 6;
  printf ("'abcdef' in, query by index, last %u can be query out;\n\
           others at protect area or been overwited, can't read out\n",
          buffer_size - (buffer_size * BLOCK_RING_BUFFER_PROTECT_AREA_PERCENT / 100 + 1));
  memset (rfMonitor, '\0', 50);
  block_ring_buffer_dump (rf, rfMonitor);
  printf ("buffer state: length=%u, data:%s\n", writed, rfMonitor);

  int rt;
  for (int i = 1; i <= writed; ++i)
    {
      result[0] = '\0';
      rt = block_ring_buffer_read (rf, result, i);
      printf ("read, index=%d : %s %s\n", i, (rt) ? "true : " : "false",
	      result);
    }

  block_ring_buffer_destory (rf);

  return 0;
}
