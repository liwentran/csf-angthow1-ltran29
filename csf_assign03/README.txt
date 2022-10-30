Anthony Sky Ng-Thow-Hing (angthow1)
Liwen Tran (ltran29)

**MS1**
Implemented all MS1 functions together through side-by-side partner coding. 

**MS2**
Implemented all MS2 functions together through side-by-side partner coding. 

**MS3**
Implemented all MS2 functions together through side-by-side partner coding. 


We will run a variety of tests while holding one factor constant. With `./csim 256 4 128 write-allocate write-through  lru < ./traces/swim.trace` our control.
Total loads: 220668
Total stores: 82525
Load hits: 220374
Load misses: 294
Store hits: 80967
Store misses: 1558
Total cycles: 14480241


Number of Sets:
Decrease: `./csim 128 4 128 write-allocate write-through  lru < ./traces/swim.trace`

Total loads: 220668
Total stores: 82525
Load hits: 220350
Load misses: 318
Store hits: 80964
Store misses: 1561
Total cycles: 14566614


Increase: `./csim 512 4 128 write-allocate write-through  lru < ./traces/swim.trace`

Total loads: 220668
Total stores: 82525
Load hits: 220394
Load misses: 274
Store hits: 80970
Store misses: 1555
Total cycles: 14406664

We conclude that the more sets we have, the better the performance because it requires less cycles.


Number block:
Increase:
./csim 256 8 128 write-allocate write-through  lru < ./traces/swim.trace

Total loads: 220668
Total stores: 82525
Load hits: 220396
Load misses: 272
Store hits: 80971
Store misses: 1554
Total cycles: 14397067

Decrease:
./csim 256 2 128 write-allocate write-through  lru < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220321
Load misses: 347
Store hits: 80947
Store misses: 1578
Total cycles: 14713768

We conclude that the more blocks we have, the better the performance because it requires less cycles.


Number of bytes per block
Increase:
./csim 256 2 256 write-allocate write-through  lru < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220444
Load misses: 224
Store hits: 81718
Store misses: 807
Total cycles: 15153062

Decrease:
./csim 256 2 16 write-allocate write-through  lru < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 218709
Load misses: 1959
Store hits: 71759
Store misses: 10766
Total cycles: 13632968

We conclude that the more bytes we have, the better the performance because it requires less cycles.


Write allocate vs No write allocate
write allocate:
./csim 256 4 128 write-allocate write-through  lru < ./traces/swim.trace

Total loads: 220668
Total stores: 82525
Load hits: 220374
Load misses: 294
Store hits: 80967
Store misses: 1558
Total cycles: 14480241

no write allocate:
v./csim 256 4 128 no-write-allocate write-through  lru < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 219718
Load misses: 950
Store hits: 70275
Store misses: 12250
Total cycles: 11582493

We conclude that not having write allocate is better for performance because it requires less cycles.

Write through vs Write back
Increase:
./csim 256 4 128 write-allocate write-through  lru < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220374
Load misses: 294
Store hits: 80967
Store misses: 1558
Total cycles: 14480241


Decrease:
./csim 256 4 128 write-allocate write-back  lru < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220374
Load misses: 294
Store hits: 80967
Store misses: 1558
Total cycles: 8707741

We conclude that write back is better for performance because it requires less cycles.


LRU vs fifo

LRU:
./csim 256 4 128 write-allocate write-through  lru < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220374
Load misses: 294
Store hits: 80967
Store misses: 1558
Total cycles: 14480241

FIFO:
./csim 256 4 128 write-allocate write-through fifo < ./traces/swim.trace
Total loads: 220668
Total stores: 82525
Load hits: 220345
Load misses: 323
Store hits: 80960
Store misses: 1565
Total cycles: 14595405

We conclude that LRU is better for performance because it requires less cycles.





From our experiments, we can conclude that best cache will be the one with the most number of sets, blocks, number of bytes per block with no-write-allocate, write-back, and LRU.