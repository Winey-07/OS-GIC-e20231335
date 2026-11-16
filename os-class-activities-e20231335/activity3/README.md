## Task 1
### Answers

1. **Role of `bind()` / Why the client doesn’t call it:**  
   `bind()` assigns the server socket a local IP address and port so clients can find it. The client normally does not call `bind()` because the operating system automatically assigns it an available temporary local port.

2. **What `accept()` returns:**  
   `accept()` waits for a client to connect and returns a new socket file descriptor for communicating with that client. The original server socket remains open and is used to listen for connections.

3. **Starting the client before the server:**  
   The client cannot connect because no server is listening on port 8080. The connection attempt typically fails with `Connection refused`.  
   

4. **What `htons(PORT)` does:**  
   `htons()` means “host to network short.” It converts the port number from the computer’s byte order to network byte order, which networking functions expect. Computers may store multi-byte numbers in different byte orders, so conversion ensures the port is represented consistently.

5. **Socket call sequence:**

   ```text
   Server: socket() → bind() → listen() → accept() → read() → send() → close()
   Client: socket() → connect() → send() → read() → close()
   ```

## Task 2: POSIX Threads (C)

### Answers

1. **What is a race condition? Why can `shared_counter` be incorrect?**  
   A race condition occurs when multiple threads access shared data at the same time and the result depends on their timing. In `threads.c`, `shared_counter++` is not a single indivisible operation: a thread reads the counter, adds one, and writes the result. Two threads can read the same value and then overwrite each other’s updates, so some increments may be lost. The program has a data race, so its behavior is not guaranteed by C; the counter may sometimes appear correct, but it is not safe to rely on that result.

2. **What does `pthread_mutex_lock()` do? Why does it fix the race condition?**  
   `pthread_mutex_lock()` locks a mutex. If another thread already holds that mutex, the calling thread waits until it becomes available. Locking before the increment and unlocking afterward ensures that only one thread at a time modifies `shared_counter`, preventing conflicting updates.

3. **What happens if you forget to call `pthread_join()`?**  
   `pthread_join()` makes the calling thread wait for a worker to finish. If `main` returns without joining the workers, the process exits, which can stop the workers before they finish. The final output may be missing or incomplete. The exact behavior depends on timing. In this program, removing the joins also means the final counter value could be read while workers are still updating it.

4. **How is a thread different from a process? What do threads share and what is private?**  
   A process has its own address space and operating-system resources. Threads within the same process share the process’s code, global data, heap, and open files. Each thread has its own execution state, including its stack and CPU registers. Because threads share data, concurrent access to that data may need synchronization.



## Task 3: Java Multithreading

### Answers

1. **What is the difference between extending `Thread` and implementing `Runnable`? When would you prefer each?**  
   Extending `Thread` creates a subclass that combines the task with the thread object. Implementing `Runnable` defines a task separately, which can then be passed to a `Thread` or an executor. Prefer `Runnable` when you want to separate the task from how it runs, reuse the task, or extend another class—Java classes can extend only one class. Extending `Thread` can be suitable for a simple example or when you specifically need to customize the thread class.

2. **Why do only two tasks run simultaneously in `PoolDemo`?**  
   `PoolDemo` creates a fixed thread pool with two worker threads. At most two tasks can run at a time; the other submitted tasks wait until a worker becomes available.

3. **What does `thread.join()` do? What happens if you remove it from `ThreadDemo`?**  
   `join()` makes the current thread wait until the specified thread finishes. In `ThreadDemo`, the main thread joins the three worker threads before printing that all threads have finished. Without the joins, `main` may print that message before the workers complete. The Java process normally remains alive while its non-daemon worker threads are running, but the message order and timing can change.

4. **Why is `ExecutorService` often better than manually creating threads in large applications?**  
   An `ExecutorService` manages a pool of reusable threads and provides a convenient way to submit and coordinate tasks. Reusing a bounded number of threads avoids creating a new thread for every task, helps control concurrency and resource use, and makes task completion and shutdown easier to manage.

## Task 4:

### Answers

1. **LWP column meaning:**  
   `LWP` means Lightweight Process ID. In Linux, it identifies an individual thread. Threads in the same process share a process ID (`PID`), but each thread has its own LWP/thread ID.

2. **Number of `/proc/<PID>/task/` entries:**  
   I found **[number]** entries in `/proc/<PID>/task/`. The program creates four worker threads, so I expected five entries while it was running: four workers and the main thread. **[It matched / It did not match]** my observed `NLWP` count.

3. **Extra Java threads:**  
   Task Manager shows threads for the entire `java.exe` process, not only the three threads created by `ThreadDemo`. The Java Virtual Machine (JVM) may also use internal threads for work such as garbage collection, runtime management, and other background tasks.

4. **Linux vs. Windows thread display:**  
   Linux `ps` and `/proc` can show individual thread IDs and information such as thread count and CPU core. Task Manager makes it easy to see the thread count for a process, but provides less per-thread detail in its basic view. For this activity, Linux gave me more detail about individual threads.

5. **`kthreadd` and kernel threads:**  
   `kthreadd` is the Linux kernel process that creates and manages kernel threads; its PID is normally **2**. I observed these kernel-thread names: **[name 1]**, **[name 2]**, and **[name 3]**. I identified them as kernel threads because they appeared as children of `kthreadd` in the process tree or in the output of `ps --ppid 2`.  
   **If they were hidden instead, use this:** My environment did not show the kernel threads in `htop` or the `ps` output. This may be due to system or environment restrictions; it does not mean the kernel has no threads.

6. **Thread ID, `NLWP`, and CPU cores:**  
   The main thread’s LWP/thread ID matched the process PID. While the program was running, `NLWP` was **[observed count; expected 5]**. The threads showed these `PSR`/`PROCESSOR` core values: **[record the values you observed]**. These values indicate the CPU cores on which the threads last ran and may differ between observations.

7. **`H` and `K` in `htop`:**  
   `H` toggles the display of user threads belonging to processes. `K` toggles the display of kernel threads. Without these views enabled, individual user-thread rows or kernel-thread rows may be hidden.