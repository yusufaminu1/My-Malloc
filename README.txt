
AUTHORS
-------
Yusuf Aminu    (yta12)
Tyler Bertrand (tjb309)


TEST PLAN

We tested for:
  - malloc() giving back non-overlapping regions
  - free() actually releasing memory so it can be reused
  - adjacent free chunks merging back together (both in order and out of order)
  - the three bad free() cases being caught, printed to stderr, and killed with exit(2)
  - malloc() returning NULL when there's no room, whether the heap is full or fragmented
  - the leak detector catching anything left over at exit


TEST PROGRAMS:

All test programs take no command-line arguments.

------------------------------------------------------------
test_dealloc
------------------------------------------------------------

Test 1 - Deallocation:
  Allocates 200 one-byte objects, frees them all, then tries to allocate 200 more.
  If free() is broken and doesn't actually release memory, the second round runs
  out of heap and returns NULL.

Test 2 - Basic coalescing:
  Allocates 200 one-byte objects, frees them all, then asks for a 2000-byte block.
  Only works if the freed chunks got merged back into one big free region.

Expected output:
    PASS test_free_deallocates
    PASS test_coalescing

------------------------------------------------------------
test_coalesce_order
------------------------------------------------------------

Allocates 4 chunks of 1016 bytes (fills the heap), then frees them in the order
1st, 3rd, 2nd, 4th to make sure coalescing works even when chunks are freed
out of order. At the end, tries a single 4088-byte allocation to confirm the
heap is fully merged.

Expected output:
    PASS test_coalesce_order

------------------------------------------------------------
test_chunk_refill
------------------------------------------------------------

Fills the heap with 64 chunks of 56 bytes, frees them all, then allocates
32 chunks of 120 bytes. If coalescing failed after the first round, the
second wave of larger allocations would return NULL.

Expected output:
    PASS test_chunk_refill

------------------------------------------------------------
test_randomnoleak
------------------------------------------------------------

Randomly allocates and frees 1-byte objects using a fixed seed (12345) so
it's reproducible. After 120 total allocations, everything still alive gets
freed. The leak detector should have nothing to report.

Expected output:
    PASS test_randomnoleak (leak detector should report nothing on stderr)
Expected stderr: (empty)

------------------------------------------------------------
test_toolarge
------------------------------------------------------------

Asks for 5000 bytes from a heap that can only hold 4088. malloc() should
return NULL and print an error to stderr.

Expected output:
    PASS test_toolarge (malloc correctly returned NULL, error on stderr)
Expected stderr:
    malloc: Unable to allocate 5000 bytes (test_toolarge.c:<line>)

------------------------------------------------------------
test_errordoublefree
------------------------------------------------------------

Frees a pointer twice. The second call should be caught and exit with code 2.

Expected stderr:
    free: Innapropriate pointer (test_errordoublefree.c:<line>)
Expected exit code: 2

------------------------------------------------------------
test_errormidptr
------------------------------------------------------------

Calls free() on a pointer that's 4 bytes into a payload (not the start).
myfree() walks the heap, can't find a matching chunk, and exits with code 2.

Expected stderr:
    free: Innapropriate pointer (test_errormidptr.c:<line>)
Expected exit code: 2

------------------------------------------------------------
test_errorbadptr
------------------------------------------------------------

Calls free() on a stack variable. myfree() sees it's outside the heap and
exits with code 2.

Expected stderr:
    free: Innapropriate pointer (test_errorbadptr.c:<line>)
Expected exit code: 2

------------------------------------------------------------
test_nonadjacent
------------------------------------------------------------

Fills the heap with 4 chunks of 1016 bytes, then frees only the 1st and 4th.
Since they're separated by two allocated chunks they can't coalesce, so a
1500-byte request should fail with NULL.

Expected output:
    PASS test_nonadjacent (malloc correctly returned NULL, error on stderr)
Expected stderr:
    malloc: Unable to allocate 1504 bytes (test_nonadjacent.c:<line>)

------------------------------------------------------------
test_toolarge_coalesced
------------------------------------------------------------

Fills the heap with 64 small chunks, frees them all (so the heap is one big
free block), then asks for 5000 bytes. Even after full coalescing the heap
can't fit it, so malloc() should return NULL.

Expected output:
    PASS test_toolarge_coalesced (malloc correctly returned NULL, error on stderr)
Expected stderr:
    malloc: Unable to allocate 5000 bytes (test_toolarge_coalesced.c:<line>)

------------------------------------------------------------
test_leak
------------------------------------------------------------

Allocates 10 objects of 8 bytes and exits without freeing any of them.
The leak detector should catch all 80 bytes.

Expected stderr:
    mymalloc: 80 bytes leaked in 10 objects.

------------------------------------------------------------
test_randomleak
------------------------------------------------------------

Same random pattern as test_randomnoleak (same seed), but intentionally
skips freeing the odd-indexed survivors. The leak detector should report
the leftover objects.

Expected stderr:
    mymalloc: <N> bytes leaked in <M> objects.



MEMGRIND STRESS TASKS:


memgrind runs each task 50 times and reports the average time in microseconds.

Task 1 - 3:
  Followed directions

Task 4 - Linked list:
  Builds a 30-node linked list using malloc(), walks it, and frees every node.

Task 5 - 2D matrix:
  Allocates a 20-element array of char pointers, then a 20-char row for each one
  (20x20 matrix). Fills every cell with (row + col), then frees each row and
  the pointer array.



DESIGN NOTES:


Minimum chunk size:
  16 bytes (8-byte header + 8-byte minimum payload). We only split a chunk if
  the leftover would have at least sizeof(chunk_header) + 16 bytes; otherwise
  the whole chunk goes to the caller.

Alignment:
  Sizes are rounded up to the nearest multiple of 8 with: (size + 7) & ~7

Traversal:
  next_chunk(h) = (char*)h + sizeof(chunk_header) + h->size. We start at
  heap.bytes[0] and stop when the next pointer falls outside the heap.

Initialization:
  A static flag makes sure initialize_heap() only runs once. It sets up a
  single free chunk covering all 4088 bytes and registers the leak detector
  with atexit(). First call to malloc() or free() triggers it automatically.

Coalescing (inside myfree):
  Forward:  after marking a chunk free, absorb the next chunk if it's also free.
            Keep going until we hit an allocated chunk or the end of the heap.
  Backward: if the previous chunk is already free, absorb the newly freed chunk
            into it.

Error handling:
  Bad free() calls print to stderr and call exit(2). malloc() prints to stderr
  and returns NULL when there's no chunk big enough.
