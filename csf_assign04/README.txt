Anthony Sky Ng-Thow-Hing (angthow1) and Liwen Tran (ltran29)

CONTRIBUTIONS

We coded all function through side-by-side partner coding

REPORT

Outputs of the time command:

With threshold of 2097152
real    0m0.406s
user    0m0.392s
sys     0m0.012s

With threshold of 1048576
real    0m0.259s
user    0m0.439s
sys     0m0.035s

With threshold of 524288
real    0m0.165s
user    0m0.448s
sys     0m0.051s

With threshold of 262144
real    0m0.142s
user    0m0.551s
sys     0m0.067s

With threshold of 131072
real    0m0.134s
user    0m0.513s
sys     0m0.100s

With threshold of 65536
real    0m0.149s
user    0m0.533s
sys     0m0.104s

With threshold of 32768
real    0m0.144s
user    0m0.534s
sys     0m0.141s

With threshold of 16384
real    0m0.149s
user    0m0.538s
sys     0m0.178s

Because the real time indicates the amount of time that elapsed between when the program started and exited, this is the best measure of how long the sorting took.
Using this indicator, running with threshold 131072 produced the lowest runtime of 0m0.134s.

Furthermore, as seen through the data, lowering the threshold greatly reduced the runtime. For example, the differnece in runtimes between thresholds 2097152 and 1048576
was from 0m0.406s to 0m0.259s, a roughly 36% decrease. However, this sizeable amount of reduction decreases as the thresholds get smaller. For example, from 262144 to 131072
we saw a mere 6% decrease in runtime. In fact, are data showed further decreasing the threshold actually yielded an increase in runtime. 

From such observations, we can conclude that creating more parallel processes decreases the runtime greatly at the start. However, at a certain ponit, creating too many
parallel processes can increase the runtime as well.

This makes intuitive sense because of how merge sort works. Mergesort has a O(n*logn) time complexity. This is because merge sort divides the data in two halves and takes linear time to merge two halves. Doing this with smaller data sets
would therefore take proportionally longer and would not be as effective. To elaborate, doing this with larger datasets would be more efficient because the program splits the sorting process into more forks that are executed in parallel. 
This explains why decreasing the threshold at the start improves performance because that means more data is being included in the merging process (hence more parrelel processes) instead of the sequential sorting process.

Moreover, creating an excess of parallel processes could potentially also harm runtime because of the context switches between the processes. Too many context switches can be computationally expensive when there are not enough CPU cores available. This could explain why we saw an increase in runtime when the threshold was too low as too many parallel proccesses were being made.