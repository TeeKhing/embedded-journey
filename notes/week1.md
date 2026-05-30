The '&' operator returns the address of the variable it is placed in front of. '*' is a dereferencer when it is placed in front of a pointer variable, basically meaning "go to the address stored in this pointer and read or write the value there."
int *ptr; means pointer to int. It initializes ptr to hold the address of an int variable. int val; means variable "val" is to hold an integer value.
If ptr points to a uint32_t and you do ptr++, the address changes by 1 * sizeof(uint32_t).
The modulo operator returns 1.
a. stack;   b. .bss;    c. .bss;    d. heap;
The caller of cb_write needs to know whether the write succeeded or failed. void cannot communicate anything back. A return value of 1 or 0 tells the caller "it worked" or "the buffer was full, nothing was written" — so it can respond accordingly.
0
It is to keep track of how many elements are filled. Without count, you cannot reliably distinguish between a completely empty buffer and a completely full buffer. When head equals tail, it could mean either condition — zero elements or CAPACITY elements. Count removes that ambiguity entirely.


DAY 2
1. What is the result of `0b11001100 & 0b10101010`? Work it out in
   binary, then convert to hex.

2. What single operation sets bit 6 of a register without
   disturbing any other bit? Write it as a C expression.

3. What single operation clears bit 2 of a register without
   disturbing any other bit? Write it as a C expression.

4. What does `(1 << 4)` evaluate to in decimal? In binary?

5. A function receives a `const uint8_t *buffer` parameter.
   Can it read buffer[3]? Can it write buffer[3] = 0xFF?
   Why or why not?

6. You have a variable `uint8_t flag` shared between an ISR and
   main(). What keyword must you add to its declaration and why?

7. A function is called 100 times. Inside it, there is a
   `static uint16_t count = 0; count++;` declaration.
   What is the value of count after the 100th call?

8. In button_is_pressed() from the Wokwi sketch:
       return !(PIND & (1 << BUTTON_PIN));
   Explain every operator and symbol in this single line.

Answers:
1.  Binary  = 0b10001000
    Hex     = 0x88

2.  OR operation,   reg |=  (1 << 6)
3.  AND operation,  reg &= ~(1 << 2)
4.  (1 << 4) = 16, 0b00010000
5.  Yes, it can read buffer[3].
    No, it cannot write buffer[3]. The const keyword flags the buffer[3] as immutable. If any attempt to change is noticed, the compiler will crash intentionally.
6.  volatile. This keyword flags the variable as "is expected to change at any time", so never cache.
7.  The value is 100.
8.  (1 << BUTTON_PIN) — create a mask with only the button's bit set.
    PIND & (1 << BUTTON_PIN) — isolate that specific bit from PIND. Result is non-zero if button pin is HIGH, zero if LOW
    !( ... ) — invert the result. Returns 1 when pin is LOW, 0 when HIGH