yta12 tjb309
We split the parts into one doing the malloc function and one doing the free function.
for the malloc function We got the start of the heap and treverse through it using "next_chunk" function, which chunks the next chunk
if that chunk is not allocated and if its size is greater than or equal to the input size we set that chunk to allocated and return a pointed to the payload
but if the chunk size - the size of the input is greater than 16 we split the chunk and set the chunk we are on to allocated and change the size
then for the other chunk we set the size to its regular size - input size + size of header and its not allocated