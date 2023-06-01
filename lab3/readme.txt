jerryshi

Exercise 0:
==4096774== LEAK SUMMARY:
==4096774==    definitely lost: 0 bytes in 0 blocks
==4096774==    indirectly lost: 0 bytes in 0 blocks
==4096774==      possibly lost: 2,784 bytes in 27 blocks
==4096774==    still reachable: 131,189 bytes in 1,396 blocks
==4096774==                       of which reachable via heuristic:
==4096774==                         length64           : 1,256 bytes in 20 blocks
==4096774==                         newarray           : 1,744 bytes in 29 blocks
==4096774==         suppressed: 0 bytes in 0 blocks

Exercise 1:
==4097604== Conditional jump or move depends on uninitialised value(s)
==4097604==    at 0x109300: fun0 (vgme.c:27)
==4097604==    by 0x1098DA: main (vgme.c:186)

==4097642== Conditional jump or move depends on uninitialised value(s)
==4097642==    at 0x109358: fun1 (vgme.c:35)
==4097642==    by 0x1098DA: main (vgme.c:186)

==4097838== Conditional jump or move depends on uninitialised value(s)
==4097838==    at 0x10939D: fun2 (vgme.c:43)
==4097838==    by 0x1098DA: main (vgme.c:186)

2. Func 0 is the only one with errors from stack allocation.
The other two errors come from heap allocation.

3. No visible difference. In both cases, conditional jump is from an uninitialized value.

