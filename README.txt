yta12 tjb309
Yusuf Aminu
We split the parts into one doing the malloc function and one doing the free function.
for the malloc function We got the start of the heap and treverse through it using "next_chunk" function, which chunks the next chunk
if that chunk is not allocated and if its size is greater than or equal to the input size we set that chunk to allocated and return a pointed to the payload
but if the chunk size - the size of the input is greater than 16 we split the chunk and set the chunk we are on to allocated and change the size
then for the other chunk we set the size to its regular size - input size + size of header and its not allocated

Tyler Bertrand
For the memgrind.c we followed the three tasks assigned to us, and for the additional tasks that we
were able to choose on our own we decided to go with a 2D Matrix array, and a linked list traversal allocation and freeing.
for the free function we it went through three usage error checks for where you can't free, and printed innappropriate pointer
for those three errors. It begins in the error to see if the ptr is within the heap, or if the address is obtained from malloc, and if it passes that
then a header for chunk and a prev variable for chunks are initialized. A loop while the chunk is still inside the loop is ran
and checks if the payload is the chunk that is to be freed, if not then traverse through the heap using "next_chunk" until it is, and
when it is it'll check for the double free error, and if the chunk is already allocated send an innapropriate pointer error, if not it will set chunk->allocated to 0.
then the two collalescing will take places, forwards, and backwards, and this goes on until the chunk is no longer in the loop
and it will reach the last usage error which is the calling free with an address not at the start of a chunk.