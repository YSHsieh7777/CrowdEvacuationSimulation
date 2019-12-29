# CrowdEvacuationSimulation

## FPS

### Average FPS before Optimization

```
average fps = 400
```

## Cache Miss

### Cache Miss before Optimization

```
==14167== I   refs:      18,602,857,830
==14167== I1  misses:        47,150,206
==14167== LLi misses:            28,021
==14167== I1  miss rate:           0.25%
==14167== LLi miss rate:           0.00%
==14167==
==14167== D   refs:       9,815,983,606  (6,831,762,055 rd   + 2,984,221,551 wr)
==14167== D1  misses:        12,292,983  (    6,162,512 rd   +     6,130,471 wr)
==14167== LLd misses:           295,793  (       65,884 rd   +       229,909 wr)
==14167== D1  miss rate:            0.1% (          0.1%     +           0.2%  )
==14167== LLd miss rate:            0.0% (          0.0%     +           0.0%  )
==14167==
==14167== LL refs:           59,443,189  (   53,312,718 rd   +     6,130,471 wr)
==14167== LL misses:            323,814  (       93,905 rd   +       229,909 wr)
==14167== LL miss rate:             0.0% (          0.0%     +           0.0%  )
```

## Memory Leak

## Memory Leak before Optimization

```
==15896==
==15896== HEAP SUMMARY:
==15896==     in use at exit: 135,597 bytes in 465 blocks
==15896==   total heap usage: 155,282 allocs, 154,817 frees, 65,659,943 bytes allocated
==15896==
==15896== LEAK SUMMARY:
==15896==    definitely lost: 0 bytes in 0 blocks
==15896==    indirectly lost: 0 bytes in 0 blocks
==15896==      possibly lost: 0 bytes in 0 blocks
==15896==    still reachable: 135,597 bytes in 465 blocks
==15896==         suppressed: 0 bytes in 0 blocks
==15896== Reachable blocks (those to which a pointer was found) are not shown.
==15896== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==15896==
==15896== For counts of detected and suppressed errors, rerun with: -v
==15896== Use --track-origins=yes to see where uninitialised values come from
==15896== ERROR SUMMARY: 10000000 errors from 667 contexts (suppressed: 0 from 0)
```