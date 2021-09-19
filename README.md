# MultiThread-GameOfLife
## Threads,Synchronization,pthread Library.
### Developing simple version of thread pool and utilizing it to complete a workload in parallel for Game of Life.
game of life example https://playgameoflife.com.

the task description can be found [HERE](https://github.com/abir006/MultiThread-GameOfLife/blob/7207e170148d3830c0ed9a6edafdb8476553c081/The%20Task.pdf).

## Part1
Implementing Semaphore and PCQueue (which is independant on the Semaphore).

## Part2
Implementing the multi-thread game using a thread pool, where each thread gets a task from the PCQueue while using semaphores for syncronizing.
