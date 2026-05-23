Questions:
1. What does the & operator return? What does the * operator do when
   placed in front of a pointer variable?

2. What is the difference between these two declarations?
   int *ptr;
   int val;
   Which one can you use to indirectly modify another variable?

3. In pointer arithmetic, if ptr points to a uint32_t and you do ptr++,
   by how many bytes does the address change?

4. What does the % (modulo) operator return when given 9 % 8?

5. What memory section do these variables live in?
   a) A local variable inside a function
   b) A global variable initialized to zero
   c) A variable declared with the static keyword inside a function
   d) Memory returned by malloc()

6. Why does the circular buffer return bool from cb_write() instead of void?

7. If BUFFER_CAPACITY is 8 and tail is currently 7, what is
   (tail + 1) % BUFFER_CAPACITY?

8. What is the purpose of the count field in the CircularBuffer struct?
   Could you implement the buffer without it? What would be harder?

Answers:
The '&' operator returns the address of the variable it is placed in front of. '*' is a dereferencer when it is placed in front of a pointer variable, basically meaning "go to the address stored in this pointer and read or write the value there."

int *ptr; means pointer to int. It initializes ptr to hold the address of an int variable. int val; means variable "val" is to hold an integer value.

If ptr points to a uint32_t and you do ptr++, the address changes by 1 * sizeof(uint32_t).

The modulo operator returns 1.

a. stack;   b. .bss;    c. .bss;    d. heap;

The caller of cb_write needs to know whether the write succeeded or failed. void cannot communicate anything back. A return value of 1 or 0 tells the caller "it worked" or "the buffer was full, nothing was written" — so it can respond accordingly.

0

It is to keep track of how many elements are filled. Without count, you cannot reliably distinguish between a completely empty buffer and a completely full buffer. When head equals tail, it could mean either condition — zero elements or CAPACITY elements. Count removes that ambiguity entirely.
