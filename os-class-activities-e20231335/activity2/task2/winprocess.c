
/*
 * winprocess.c
 * Task 2: Windows Process Creation using CreateProcess()
 */

#include <stdio.h>
#include <windows.h>

int main(void)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // Initialize the structures
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    si.cb = sizeof(si);

    // Display parent process information
    printf("Parent process PID: %lu\n", GetCurrentProcessId());

    printf("Creating child process (mspaint.exe)...\n");

    // Create the child process
    // Windows searches for mspaint.exe using its normal executable search path.
    char commandLine[] = "mspaint.exe";

    BOOL success = CreateProcessA(
        NULL,           // Application name (search command line)
        commandLine,    // Command line (must be writable)
        NULL,           // Process security attributes
        NULL,           // Thread security attributes
        FALSE,          // Do not inherit handles
        0,              // Creation flags
        NULL,           // Use parent's environment
        NULL,           // Use parent's current directory
        &si,            // Startup information
        &pi             // Process information
    );

    if (success)
    {
        printf("Child process created successfully!\n");
        printf("Child PID: %lu\n", pi.dwProcessId);
        printf("Child process is running...\n");

        printf("Parent is waiting for child to finish.\n");

        // Wait until the child process terminates
        DWORD waitResult = WaitForSingleObject(
            pi.hProcess,
            INFINITE
        );

        if (waitResult == WAIT_OBJECT_0)
        {
            printf("Child process has finished.\n");
        }
        else
        {
            printf("Waiting failed. Error: %lu\n", GetLastError());
        }

        // Close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        printf("Process handles closed successfully.\n");
    }
    else
    {
        printf("CreateProcess failed. Error: %lu\n", GetLastError());
    }

    return 0;
}
