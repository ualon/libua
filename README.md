libua
=====

C library implementing a set of APIs to efficiently manage some basic data structures : memory pool, state machine, block ring buffer

The provided APIs are :

* mem_pool.[ch] :general block memory pool, for frequently alloc & free fixed-sized block 
* state_machine.[ch] :state machine with mutex
* block_ring_buffer.[ch] :thread **NOT** safe. for one write N reader condition
---
license：
new BSD
