#include <stdio.h>
#include <windows.h>

int main(void)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    /* Initialize STARTUPINFO and PROCESS_INFORMATION */
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    /* Create child process (Paint) */
    if (!CreateProcess(
        NULL,              /* Application name */
        "mspaint",         /* Command line */
        NULL,              /* Process handle not inheritable */
        NULL,              /* Thread handle not inheritable */
        FALSE,             /* Set handle inheritance to FALSE */
        0,                 /* No creation flags */
        NULL,              /* Use parent's environment */
        NULL,              /* Use parent's directory */
        &si,               /* STARTUPINFO pointer */
        &pi))              /* PROCESS_INFORMATION pointer */
    {
        fprintf(stderr, "CreateProcess failed. Error = %lu\n", GetLastError());
        return 1;
    }

    /* Parent waits for child process to finish */
    WaitForSingleObject(pi.hProcess, INFINITE);
    printf("Child Complete\n");

    /* Close process and thread handles */
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

