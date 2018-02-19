#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>

#define _SVID_SOURCE
#define _BSD_SOURCE

#define MAX_ARGS 100
#define BUFF_SIZE 200       // size of shared buffer
#define BUFF_SHM "/OS_BUFF"
#define BUFF_MUTEX_A "/OS_MUTEX"
#define BUFF_MUTEX_B "/OS_MUTEX"

sem_t *mutexA;
sem_t *mutexB;

void *ptr;

// define data structure for table
struct table
{
    int num;
    char name[10];
    char reservationStatus[10];
};

// Initialize Tables for Section A and Section B and remove all current reservations
void initTables(struct table *ptr)
{
    // lock semaphores
    sem_wait(mutexA);
    sem_wait(mutexB);

    // Initialize tables for section A
    printf("\n ------------Section A------------- \n");
    for (int i = 100; i < 110; i++) // Table Numbers 100 - 109
    {
        
        strcpy(ptr->name, " ");                            // Remove any names
        strcpy((ptr + i)->reservationStatus, "Available"); // Reset reservation status to "Available"
        printf("Table %d: Available\n", i);
    }

    // initialize tables for Section B
    printf("\n ------------Section B------------- \n");
    for (int i = 200; i < 210; i++) // Table Numbers 200 - 209
    {
       
        strcpy(ptr->name, " ");
        strcpy((ptr + i)->reservationStatus, "Available");
        printf("Table %d: Available\n", i);
    }

    printf("\nInitialization Successful\n");
    sleep(10); // random sleep

    // unlock semapohores
    sem_post(mutexA);
    sem_post(mutexB);
}

// Print Tables with numbers, names and reservation status
void printTable(struct table *ptr)
{

    // lock semaphores
    sem_wait(mutexA);
    sem_wait(mutexB);

    
    printf("\n ------------Section A------------- \n");
    for (int i = 100; i < 110; i++)
    {
        
        if (strcmp((ptr + i)->reservationStatus, "Available") == 0)
        {
            printf("Table %d: Available\n", i);
        }
        else if (strcmp((ptr + i)->reservationStatus, "Reserved") == 0)
        {
            printf("Table %d: Reserved Under %s\n", i, (ptr + i)->name);
        }
    }

    printf("\n ------------Section B------------- \n");
    for (int i = 200; i < 210; i++)
    {

        if (strcmp((ptr + i)->reservationStatus, "Available") == 0)
        {
            printf("Table %d: Available\n", i);
        }
        else if (strcmp((ptr + i)->reservationStatus, "Reserved") == 0)
        {
            printf("Table %d: Reserved Under %s\n", i, (ptr + i)->name);
        }
    }

    
     sleep(10);

    sem_post(mutexA);
    sem_post(mutexB);

    return;
}

//-----------------------------RESERVE SPECIFIC TABLE----------------------------------------//

void reserve(struct table *ptr, char *personName, char *section, int tableNum)
{
    switch (section[0])
    {

    //if section A
    case 'A':

        // lock mutex for section A
        sem_wait(mutexA);

        //If correct number is not specified for A print error and exit
        if (tableNum < 100 || tableNum > 109)
        {
            printf("Reservation failed: Table %d does not exist in Section A.\n", tableNum);
            sem_post(mutexA);
            break;
        }
        else
        {
            // Reserve table for the name specified if Available
            if (strcmp((ptr + tableNum)->reservationStatus, "Available") == 0)
            {
                strcpy((ptr + tableNum)->name, personName);                             // change name to name provided
                strcpy((ptr + tableNum)->reservationStatus, "Reserved");                // Change reservationStatus to reserved
                printf("Table %d %s: %s\n", tableNum, section, (ptr + tableNum)->name); // print Table number, section and name reserved for
                printf("Reservation successful.\n");
            }
            else
            {
                printf("Sorry! Table %d %s is already reserved.\n", tableNum, section);
            }
        }
        sleep(10);

        sem_post(mutexA);
        break;

    // if section B
    case 'B':

        // Lock mutex for section B
        sem_wait(mutexB);

        ///If correct number is not specified for B print error and exit
        if (tableNum < 200 || tableNum > 210)
        {
            printf("Reservation failed: Table %d does not exist in Section B.\n", tableNum);
            sem_post(mutexB);
            break;
        }
        else
        {
            // Reserve table for the name specified if Available
            if (strcmp((ptr + tableNum)->reservationStatus, "Available") == 0)
            {
                strcpy((ptr + tableNum)->name, personName);
                strcpy((ptr + tableNum)->reservationStatus, "Reserved"); // Change Reservation status to Reserved
                printf("Table %d %s: %s\n", tableNum, section, (ptr + tableNum)->name);
                printf("Reservation successful.\n");
            }
            else
            {
                printf("Sorry! Table %d %s is already reserved.\n", tableNum, section);
            }
        }
        sleep(10);

        sem_post(mutexB);
        break;

    default:
        printf("Reservation failed: Section %s does not exist.", section);
        break;
    }
    return;
}

//------------------------ RESERVE RANDOM TABLE ----------------------- //
void randomReserve(struct table *ptr, char *personName, char *section)
{
    switch (section[0])
    {
    case 'A':

        sem_wait(mutexA); // lock semaphore

        int reserved = 0;
        for (int i = 100; i < 110; i++)
        {
            if (strcmp((ptr + i)->reservationStatus, "Available") == 0)
            {
                strcpy((ptr + i)->name, personName);
                strcpy((ptr + i)->reservationStatus, "Reserved");

                printf("Reservation successful.\n");
                printf("Table %d %s: %s\n", i, section, (ptr + i)->name);
                reserved = 1;

                sem_post(mutexA); // unlock semaphore
                return;
            }
        }

        if (reserved == 0)
            printf("Reservation failed: All tables are already reserved.\n");

        // Release mutex for section A
        sem_post(mutexA);
        break;

    case 'B':

        sem_wait(mutexB); // lock semaphore

        reserved = 0;
        for (int i = 200; i < 210; i++)
        {
            if (strcmp((ptr + i)->reservationStatus, "Available") == 0)
            {
                strcpy((ptr + i)->name, personName);
                strcpy((ptr + i)->reservationStatus, "Reserved");

                printf("Reservation successful.\n");
                printf("Table %d %s: %s\n", i, section, (ptr + i)->name);
                reserved = 1;

                sem_post(mutexA); // unlock semaphore
                return;
            }
        }

        if (reserved == 0)
            printf("Reservation failed: All tables are already reserved.\n");

        // Unlock semaphore
        sem_post(mutexB);
        break;
    }
}

    int processCmd(char *cmd, struct table *ptr)
    {
        char *name;
        char *section;
        char *tableNum;
        char *token;
        token = strtok(cmd, " "); // break cmd into tokens whenever you see a space

        if (strcmp(token, "reserve") == 0) // if first token = reserve
        {
            name = strtok(NULL, " "); //
            section = strtok(NULL, " ");
            tableNum = strtok(NULL, " ");

            // if name not given
            if (name == NULL)
            {
                printf("Error! Please provide a Name for reservation.\n");
                return 1;
            }

            // if section not specified
            if (section == NULL)
            {
                printf("Error. Please Choose Either Section A or Section B.\n");
                return 1;
            }

            // check if valid section is specified
            else if (strcmp(section, "A") != 0 && strcmp(section, "B") != 0)
            {
                printf("Error. Please Choose Either Section A or Section B.\n");
                return 1;
            }

            if (tableNum != NULL)                            // if table number is given
                reserve(ptr, name, section, atoi(tableNum)); // reserve that specific table
            else
            {
                randomReserve(ptr, name, section);
            }

            sleep(10);
        }
        else if (strcmp(token, "status") == 0)
        {
            printTable(ptr);
        }
        else if (strcmp(token, "init") == 0)
        {
            initTables(ptr);
        }
        else if (strcmp(token, "exit") == 0) // if token == exit, exit the system
        {
            printf("Bye!\n");
            return 0;
        }
        else
        {
            printf("Error! Please Input a Valid Command\n");
        }
        return 1;
    }

    // Clear all semaphores on Ctrl+C
    void ctrlCHandler()
    {
        signal(SIGINT, ctrlCHandler);
        printf("\nProcess Terminated.\n");

        //unlock semaphores
        sem_post(mutexA);
        sem_post(mutexB);

        //close semaphores
        sem_close(mutexA);
        sem_close(mutexB);

        fflush(stdout); // The function fflush() forces a write of all buffered data for the given output or update stream via the stream's underlying write function

        exit(-1); // exit the program
    }

    //------------------------- MAIN METHOD---------------------------//
    int main(int argc, char **argv)
    {
        signal(SIGINT, ctrlCHandler);

        // initialize semophores
        mutexA = sem_open(BUFF_MUTEX_A, O_CREAT, S_IRWXU, 1);
        mutexB = sem_open(BUFF_MUTEX_B, O_CREAT, S_IRWXU, 1);

        // File name is specifed
        if (argc > 1)
        {
            // Perform stdin rewiring if input file exists
            if (access(argv[1], R_OK) == 0)
            {
                close(0);
                open(argv[1], O_RDONLY);
            }
            else
            {
                printf("The specified input file does not exist.\n");
                exit(-1);
            }
        }

        if (mutexA == (void *)-1 || mutexB == (void *)-1) // if mutexA or mutexB have invalid address
        {
            printf("Semaphore failed: %s.\n", strerror(errno)); // print error,
            exit(-1);                                           // exit
        }

        // open a shared memory object BUFF_SHM and return the file descriptor to shm_fd
        int shm_fd = shm_open(BUFF_SHM, O_CREAT | O_RDWR, S_IRWXU);

        // if fd < 0 print error
        if (shm_fd < 0)
        {
            printf("shared memory failed\n");
            exit(-1);
        }

        struct stat s;
        if (fstat(shm_fd, &s) == -1)
        {
            printf("f_stat Error! ");
        }
        int mem_size = sizeof(struct table) * BUFF_SIZE;
       // ftruncate(shm_fd, mem_size); // truncate shm_fd to mem_size

        // map the shared memory object starting at NULL
        ptr = mmap(NULL, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        // if mmap fails
        if (ptr == MAP_FAILED)
        {

            printf("Memory map failed: %s.\n", strerror(errno)); // return MAP_FAILED and set errno to indicate the error
            shm_unlink(BUFF_SHM);                                // remove shared memory object
            exit(-1);                                            // exit program
        }

        // Array in which the user command is held
        char cmd[MAX_ARGS];

        int retStatus = 1;

        while (retStatus)
        {
            printf("\n>> ");                 // prompt
            fgets(cmd, sizeof(cmd), stdin); // read line from stdin and store it in cmd
            strtok(cmd, "\n");              // isolate tokens in cmd

            if (argc > 1) // if more than 1 string entered
                printf("Executing command: %s\n", cmd);

            retStatus = processCmd(cmd, ptr);
        }

        // Close the semphores
        sem_close(mutexA);
        sem_close(mutexB);

        // Reset the standard input
        int fd_stdin = STDIN_FILENO; // STDIN_FILENO is a file descriptor for stdin
        if (argc > 1)
        {
            close(0);      // close stdin fd
            dup(fd_stdin); // duplicate stdin fd
        }

        munmap(ptr, mem_size); // unmap
        close(shm_fd);         // close shared memory fd
        return 0;
    }
