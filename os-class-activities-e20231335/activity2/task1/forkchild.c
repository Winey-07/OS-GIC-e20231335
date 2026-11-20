/* forkchild.c — Process creation with fork() and exec() */

#include <stdio.h> // Provides printf() and perror()
#include <stdlib.h> // Provides exit() and EXIT_FAILURE
#include <unistd.h> // Provides fork(), getpid(), getppid(), sleep(), and execlp()
#include <sys/wait.h> // Provides waitpid() and related functions

int main() {
    pid_t pid;

    // Getting the process ID
    printf("Parent process (PID: %d) — creating child...\n", getpid()); // getpid() returns the ID of the currently running process. 
    // PID = Process ID. Every running process has its own PID.

    //Creating the child process
    pid = fork();

//     Return value	Meaning
// pid < 0	Process creation failed
// pid == 0	You are inside the child process
// pid > 0	You are inside the parent; the value is the child's PID

    if (pid < 0) {
        /* fork failed */
        perror("fork"); // pid < 0	Process creation failed
        exit(EXIT_FAILURE);
    }
    //the child branch
    else if (pid == 0) { // pid == 0 You are inside the child process
        /* CHILD PROCESS */

        printf("Child process (PID: %d, Parent PID: %d)\n",
               getpid(), getppid()); //getpid() = the child's own PID.
               //getppid() = the PID of the child's parent.

        printf("Child: sleeping 30s — observe the process tree now!\n");
        sleep(30); //This makes the child pause for 30 seconds.
        //Why sleep for 30 seconds? 
        //It gives you time to open another Terminal and see the parent and child processes before the child changes into ls.

        printf("Child: now executing 'ls -la'...\n");
        // This replaces the child program with ls.
        // ls = list files.
        // -la = show detailed information, including hidden files.
        // execlp() = searches for the program through the system's PATH and replaces the current process image.

        /* Replace child process with ls */
        execlp("ls", "ls", "-la", NULL);

        /* If execlp fails */
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    //The parent branch
    else {
        /* PARENT PROCESS */

        int status;

        printf("Parent: waiting for child (PID: %d) to finish...\n", pid);

        waitpid(pid, &status, 0); //The parent waits until the child finishes.
        //Without waiting, the parent might finish before the child, and your output may appear in a different order.

        if (WIFEXITED(status)) {
            printf("Parent: child exited with status %d\n",
                   WEXITSTATUS(status));
        }

        printf("Parent: done.\n");
    }

    return 0;
}
