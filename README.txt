========================================
Project 1: My Little malloc()
CS 214 Spring 2026
========================================

AUTHORS
-------
Yusuf Aminu    (yta12)
Tyler Bertrand (tjb309)

========================================
TEST PLAN
========================================

Our testing strategy maps directly to the correctness requirements:

  - malloc() returns non-overlapping memory regions.
  - free() actually releases memory for reuse.
  - Adjacent free chunks coalesce into a single larger chunk,
     both in sequential and non-sequential free order.
  - The three detectable free() misuse errors are caught,
     reported to stderr, and terminate the process with exit(2).
  - malloc() returns NULL and prints to stderr when no free chunk
     is large enough, whether the heap is fragmented or just full.
  - The leak detector reports any unreleased objects at exit.

For every requirement we identify a concrete detection method and
write a program whose output changes clearly when the requirement
is violated.


========================================
TEST PROGRAMS
========================================

All test programs take no command-line arguments.

------------------------------------------------------------
test_dealloc
------------------------------------------------------------

Test 1 - Deallocation:
  Allocate 200 one-byte objects (filling most of the heap).
  Free all 200. Allocate 200 more. If free() does not actually
  release memory the second round fails with NULL returns.

Test 2 - Basic coalescing:
  Allocate 200 one-byte objects, free them all, then request a
  2000-byte block. This only succeeds if the freed chunks merged
  back into one contiguous free region.

Expected output:
    PASS test_free_deallocates
    PASS test_coalescing

------------------------------------------------------------
test_coalesce_order
------------------------------------------------------------

Allocates 4 chunks of 1016 bytes each (4 * 1024 = 4096 bytes,
filling the heap exactly). Frees them in order: 1st, 3rd, 2nd,
4th. Freeing the 2nd triggers a forward coalesce with the already-
free 3rd and a backward coalesce with the already-free 1st;
freeing the 4th triggers a backward coalesce with the merged
block. After all frees the heap must be one contiguous free block,
verified by a single 4088-byte allocation that would fail if any
fragment remained.

Expected output:
    PASS test_coalesce_order

------------------------------------------------------------
test_chunk_refill
------------------------------------------------------------

Phase 1: 64 chunks of 56 bytes (64 * 64 = 4096, fills heap).
Phase 2: after freeing all 64, allocate 32 chunks of 120 bytes
(32 * 128 = 4096, fills heap). If coalescing failed after phase 1
the second wave of larger allocations would return NULL.

Expected output:
    PASS test_chunk_refill

------------------------------------------------------------
test_randomnoleak
------------------------------------------------------------

Maintains an array of up to 120 pointers. On each step, randomly
chooses to allocate a new 1-byte object or free a randomly chosen
live object. Once 120 total allocations have been made, frees all
remaining live pointers. Uses a fixed seed (12345) for
reproducibility. The leak detector at exit must report nothing.

Expected output:
    PASS test_randomnoleak (leak detector should report nothing on stderr)
Expected stderr: (empty)

------------------------------------------------------------
test_toolarge
------------------------------------------------------------

Requests 5000 bytes from a heap whose maximum payload is 4088
bytes. malloc() must return NULL and print an error to stderr.

Expected output:
    PASS test_toolarge (malloc correctly returned NULL, error on stderr)
Expected stderr:
    malloc: Unable to allocate 5000 bytes (test_toolarge.c:<line>)

------------------------------------------------------------
test_errordoublefree
------------------------------------------------------------

Allocates an object, frees it, then calls free() a second time
via a second pointer holding the same address. After the first
free the chunk is marked unallocated; myfree() detects the second
call and exits with code 2.

Expected stderr:
    free: Innapropriate pointer (test_errordoublefree.c:<line>)
Expected exit code: 2

------------------------------------------------------------
test_errormidptr
------------------------------------------------------------

Allocates a two-int object and calls free() on (p + 1), which is
4 bytes past the payload start. myfree() walks the heap, finds
no chunk whose payload begins at that address, and exits with
code 2.

Expected stderr:
    free: Innapropriate pointer (test_errormidptr.c:<line>)
Expected exit code: 2

------------------------------------------------------------
test_errorbadptr
------------------------------------------------------------

Calls free() on the address of a local stack variable. myfree()
detects that the address falls outside the heap bounds and exits
with code 2.

Expected stderr:
    free: Innapropriate pointer (test_errorbadptr.c:<line>)
Expected exit code: 2

------------------------------------------------------------
test_nonadjacent
------------------------------------------------------------

Fills the heap with 4 chunks of 1016 bytes each, then frees only
the 1st and 4th. The two free chunks are separated by two
allocated chunks so they cannot coalesce. A 1500-byte request
(rounded to 1504) exceeds either free block individually, so
malloc() must return NULL.

Expected output:
    PASS test_nonadjacent (malloc correctly returned NULL, error on stderr)
Expected stderr:
    malloc: Unable to allocate 1504 bytes (test_nonadjacent.c:<line>)

------------------------------------------------------------
test_toolarge_coalesced
------------------------------------------------------------

Fills the heap with 64 chunks of 56 bytes, frees them all
(producing a fully coalesced 4088-byte free block), then requests
5000 bytes. This verifies both that coalescing succeeded (the 64
individual frees worked) and that even a fully coalesced heap
cannot satisfy a request exceeding its capacity.

Expected output:
    PASS test_toolarge_coalesced (malloc correctly returned NULL, error on stderr)
Expected stderr:
    malloc: Unable to allocate 5000 bytes (test_toolarge_coalesced.c:<line>)

------------------------------------------------------------
test_leak
------------------------------------------------------------


Allocates 10 objects of 8 bytes each (80 bytes total) and exits
without freeing them. The leak detector registered with atexit()
must fire and report the count and total size.

Expected stderr:
    mymalloc: 80 bytes leaked in 10 objects.

------------------------------------------------------------
test_randomleak
------------------------------------------------------------


Performs the same random alloc/free pattern as test_randomnoleak
(same seed) but intentionally skips freeing odd-indexed survivors.
The leak detector must detect and report the unreleased objects.

Expected stderr:
    mymalloc: <N> bytes leaked in <M> objects.


========================================
MEMGRIND STRESS TASKS
========================================

memgrind runs each task 50 times and reports the average elapsed
time in microseconds using gettimeofday().

Task 1 - Immediate malloc/free:
  malloc() and immediately free() a 1-byte object, repeated 120
  times per run. Tests the fast path where a freshly freed chunk
  is reallocated right away with minimal fragmentation.

Task 2 - Batch malloc then batch free:
  Allocate 120 one-byte objects storing all pointers in an array,
  then free all 120 in order. Tests allocation under a nearly-full
  heap and bulk deallocation followed by forward coalescing.

Task 3 - Random alloc/free:
  Maintain an array of up to 120 pointers. On each iteration
  randomly choose to either allocate a new one-byte object or free
  a randomly chosen already-allocated object. Once 120 total
  allocations have been made, free all remaining live pointers.
  Tests interleaved allocation and deallocation patterns that
  produce fragmentation.

Task 4 - Linked list:
  Build a singly-linked list of 30 Node structs (each holding an
  int and a next pointer) using malloc(), then traverse the list
  and free every node. Simulates a common real-world pattern of
  allocating same-sized structs and freeing them via pointer chase.

Task 5 - 2D matrix:
  Allocate a 20-element array of char pointers, then allocate a
  20-char row for each pointer (simulating a 20x20 char matrix).
  Fill every cell with (row + col). Free each row individually,
  then free the pointer array. Tests a two-level dynamic
  allocation pattern with mixed object sizes.


========================================
DESIGN NOTES
========================================

Minimum chunk size:
  16 bytes (8-byte header + 8-byte minimum payload). Before
  splitting, mymalloc() checks that the leftover region would
  contain at least sizeof(chunk_header) + 16 bytes; otherwise the
  entire chunk is given to the caller unsplit.

Alignment:
  All requested sizes are rounded up to the nearest multiple of 8
  using: (size + 7) & ~7

Traversal:
  next_chunk(h) returns (char*)h + sizeof(chunk_header) + h->size.
  The first chunk always starts at heap.bytes[0]. Iteration stops
  when the resulting pointer falls outside the heap bounds.

Initialization:
  A static flag guards a one-time call to initialize_heap(), which
  writes a single free chunk header covering all 4088 available
  payload bytes and registers leak_detector() with atexit(). Both
  mymalloc() and myfree() trigger initialization on first call so
  clients never need to call an init function explicitly.

Coalescing (performed inside myfree):
  Forward:  after marking a chunk free, repeatedly absorb the
            immediate successor chunk while it is also free.
  Backward: if the predecessor chunk (tracked by the prev pointer
            during traversal) is already free, absorb the newly
            freed chunk into it.

Error handling:
  All three detectable free() errors print to stderr and call
  exit(2). mymalloc() prints to stderr and returns NULL when no
  free chunk is large enough to satisfy the request.
