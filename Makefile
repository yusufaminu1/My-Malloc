CC = gcc
CFLAGS = -Wall -Wextra -g

TESTS = memtest test_dealloc test_errorbadptr test_errormidptr test_errordoublefree test_leak \
        test_coalesce_order test_chunk_refill test_randomnoleak test_toolarge \
        test_nonadjacent test_toolarge_coalesced test_randomleak

ifeq ($(OS),Windows_NT)
    EXE = .exe
else
    EXE =
endif

BINS = memgrind$(EXE) $(addsuffix $(EXE),$(TESTS))

all: $(BINS)

memgrind$(EXE): memgrind.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ memgrind.c mymalloc.c

memtest$(EXE): memtest.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ memtest.c mymalloc.c

test_dealloc$(EXE): test_dealloc.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_dealloc.c mymalloc.c

test_errorbadptr$(EXE): test_errorbadptr.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_errorbadptr.c mymalloc.c

test_errormidptr$(EXE): test_errormidptr.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_errormidptr.c mymalloc.c

test_errordoublefree$(EXE): test_errordoublefree.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_errordoublefree.c mymalloc.c

test_leak$(EXE): test_leak.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_leak.c mymalloc.c

test_coalesce_order$(EXE): test_coalesce_order.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_coalesce_order.c mymalloc.c

test_chunk_refill$(EXE): test_chunk_refill.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_chunk_refill.c mymalloc.c

test_randomnoleak$(EXE): test_randomnoleak.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_randomnoleak.c mymalloc.c

test_toolarge$(EXE): test_toolarge.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_toolarge.c mymalloc.c

test_nonadjacent$(EXE): test_nonadjacent.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_nonadjacent.c mymalloc.c

test_toolarge_coalesced$(EXE): test_toolarge_coalesced.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_toolarge_coalesced.c mymalloc.c

test_randomleak$(EXE): test_randomleak.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o $@ test_randomleak.c mymalloc.c

ifeq ($(OS),Windows_NT)
clean:
	del /f /q $(subst /,\,$(BINS)) *.o 2>nul & exit 0
else
clean:
	rm -f $(BINS) *.o
endif
