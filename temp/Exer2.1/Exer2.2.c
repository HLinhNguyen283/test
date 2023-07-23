// ==========================================================================
// Include files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>

// ==========================================================================
// Define constants

#define MAX_FILES 5
#define SHARED_MEM_SIZE 1024
#define PORT 8080

// ==========================================================================
// Define the email structure

struct Email {
    char sender[256];
    char receiver[256];
    char content[1024];
};

// Function to write email data to the summary file
void writeToSummaryFile(const char* filename, const struct Email* email) {
    FILE* file = fopen(filename, "a");
    if (file) {
        fprintf(file, "%s;%s;%s\n", email->sender, email->receiver, email->content);
        fclose(file);
    } else {
        perror("Error opening summary file");
    }
}

// Function to process child process
void processChild(int socket_fd, const char* summary_file) {
    int shmid = shmget(IPC_PRIVATE, SHARED_MEM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error creating shared memory");
        exit(1);
    }

    char* shared_memory = (char*)shmat(shmid, NULL, 0);
    if (shared_memory == (char*)-1) {
        perror("Error attaching shared memory");
        exit(1);
    }

    // Wait for "is_alive" message from the parent
    char buffer[256];
    recv(socket_fd, buffer, sizeof(buffer), 0);

    // Respond with "alive" message
    send(socket_fd, "alive", 5, 0);

    // Process shared memory if data exists
    if (strlen(shared_memory) > 0) {
        struct Email email;
        strcpy(email.sender, strtok(shared_memory, "\n"));
        strcpy(email.receiver, strtok(NULL, "\n"));
        strcpy(email.content, strtok(NULL, "\n"));

        writeToSummaryFile(summary_file, &email);
    }

    // Wait for "shutdown" message from the parent
    recv(socket_fd, buffer, sizeof(buffer), 0);

    // Detach and remove shared memory
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    // Close the socket and exit
    close(socket_fd);
    exit(0);
}

// Function to process parent process
void processParent(const char* files[], int num_files) {
    int socket_fd;
    struct sockaddr_in server_address;
    char buffer[256] = {0};

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // Connect to child process
    if (connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error connecting to child process");
        exit(1);
    }

    // Send "is_alive" message to the child and wait for response
    send(socket_fd, "is_alive", 8, 0);
    recv(socket_fd, buffer, sizeof(buffer), 0);

    if (strcmp(buffer, "alive") != 0) {
        // Process 2 is not alive, fork new process
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            processChild(socket_fd, "email_archive.csv");
        } else if (pid < 0) {
            perror("Error forking process");
            exit(1);
        }
    }

    // Shared memory for data from import files
    int shmid = shmget(IPC_PRIVATE, SHARED_MEM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error creating shared memory");
        exit(1);
    }

    char* shared_memory = (char*)shmat(shmid, NULL, 0);
    if (shared_memory == (char*)-1) {
        perror("Error attaching shared memory");
        exit(1);
    }

    // Process data from import files
    for (int i = 0; i < num_files; ++i) {
        FILE* file = fopen(files[i], "r");
        if (file) {
            struct Email email;

            // Read email data from the file
            fgets(email.sender, sizeof(email.sender), file);
            fgets(email.receiver, sizeof(email.receiver), file);
            fgets(email.content, sizeof(email.content), file);

            // Push email data to shared memory
            strcpy(shared_memory, email.sender);
            strcat(shared_memory, "\n");
            strcat(shared_memory, email.receiver);
            strcat(shared_memory, "\n");
            strcat(shared_memory, email.content);
            strcat(shared_memory, "\n");

            fclose(file);
        } else {
            printf("Error opening file: %s\n", files[i]);
        }
    }

    // Send "shutdown" message to the child
    send(socket_fd, "shutdown", 9, 0);
    recv(socket_fd, buffer, sizeof(buffer), 0);

    // Detach and remove shared memory
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    // Close the socket
    close(socket_fd);
}

int main() {
    const char* import_files[MAX_FILES] = {
        "file1.txt",
        "file2.txt",
        "file3.txt",
        "file4.txt",
        "file5.txt"
    };

    processParent(import_files, MAX_FILES);

    return 0;
}
