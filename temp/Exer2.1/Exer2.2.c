// ==========================================================================
// Include files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>

// ==========================================================================
// Define constants

#define PORT 8080
#define BUFFER_SIZE 1024
#define SHARED_MEM_SIZE 1024
#define SHUTDOWN_MESSAGE "shutdown"
#define ALIVE_MESSAGE "alive"
#define IS_ALIVE_MESSAGE "is_alive"
// ==========================================================================
// Define the email structure

struct Email
{
    char sender[256];
    char receiver[256];
    char content[1024];
};

const char *import_files[] = {
    "file1.txt",
    "file2.txt",
    "file3.txt",
    "file4.txt",
    "file5.txt"};

void sendMessage(int socket_fd, const char *message);
void receiveMessage(int socket_fd, const char *buffer);
void writeToSharedMemory(int shm_id, const struct Email *email);
void readFromSharedMemory(int shm_id, struct Email *email);

// Function to write email data to the summary file
void writeToSummaryFile(const char *filename, const struct Email *email)
{
    printf("a\n");
    FILE *file = fopen(filename, "a");
    if (ftell(file) == 0)
    {
        fprintf(file, "From;To;Content\n");
    }
    if (file)
    {
        fprintf(file, "%s;%s;%s\n", email->sender, email->receiver, email->content);
        fclose(file);
    }
    else
    {
        perror("Error opening summary file");
    }
}

// Function to process child process
void processChild(int socket_fd, int shm_id, const char *summary_file)
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }

    sendMessage(client_fd, ALIVE_MESSAGE);
    printf("Hello message sent\n");
    valread = read(client_fd, buffer, 1024);
    // printf("%s\n", buffer);

    while (true)
    {
        //         // Wait for "is_alive" message from the parent
        //         const char buffers[1024];
        //         // Respond with "alive" message
        //         sendMessage(client_fd, ALIVE_MESSAGE);
        //         read(client_fd, buffers, 1024);
        //         printf("a");
        //         printf("%s", buffers);
        if (strcmp(buffer, IS_ALIVE_MESSAGE) == 0)
        {
            // Process shared memory data if available
            struct Email email;
            memset(&email, 0, sizeof(struct Email));
            // readFromSharedMemory(shm_id, &email);
            printf("%s", email.sender);
            if (strlen(email.sender) > 0)
            {

                // Log content to summary file (email_archive.csv)struct
                // strcpy(email.sender, strtok(shared_memory, "\n"));
                // strcpy(email.receiver, strtok(NULL, "\n"));
                // strcpy(email.content, strtok(NULL, "\n"));

                writeToSummaryFile(summary_file, &email);

                // Clear shared memory
                memset(&email, 0, sizeof(struct Email));
                writeToSharedMemory(shm_id, &email);
            }
        }
        else if (strcmp(buffer, SHUTDOWN_MESSAGE) == 0)
        {
            // Check if "shutdown" message received
            break;
        }

        // Detach and remove shared memory
        // shmdt(shared_memory);
        shmctl(shm_id, IPC_RMID, NULL);

        // Close the socket and exit
        close(socket_fd);
        exit(0);
    }
}

// Function to process parent process
void processParent(int socket_fd, int shm_id, const char *files[])
{

    // // Check if Process 2 is alive
    // sendMessage(socket_fd, IS_ALIVE_MESSAGE);

    // // Wait for response from Process 2
    // char response[256] = {0};
    // receive_message(socket_fd, response);

    // if (strcmp(response, ALIVE_MESSAGE) == 0) {
    //     // Process 2 is alive, save email data to shared memory
    //     write_to_shared_memory(shm_id, &email);
    // } else {
    //     // Process 2 is not alive, fork new Process 2
    //     pid_t new_pid = fork();

    //     if (new_pid < 0) {
    //         perror("Forking new Process 2 failed");
    //         exit(EXIT_FAILURE);
    //     } else if (new_pid == 0) {
    //         // Child process (new Process 2)
    //         execlp("./process2", "./process2", NULL);
    //     } else {
    //         // Parent process (Process 1)
    //         sleep(1); // Give some time for Process 2 to start
    //     }
    // }

    // // Send "shutdown" message to the child
    // send(socket_fd, "shutdown", 9, 0);
    // recv(socket_fd, buffer, sizeof(buffer), 0);

    // // Detach and remove shared memory
    // shmdt(shared_memory);
    // shmctl(shmid, IPC_RMID, NULL);

    // // Close the socket
    // close(socket_fd);
    // Process 1 (Parent process)

    int i;

    for (i = 0; i < 5; i++)
    {
        processToImportfile((const char *)files[i], socket_fd, shm_id);
    }

    // Wait for Process 2 to finish
    // int status;
    // waitpid(pid, &status, 0);

    // Close the socket
    close(socket_fd);

    exit(EXIT_SUCCESS);
}

int main()
{
    // Create a socket for IPC
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket creation successed!\n");
    }

    int opt = 1;
    // Forcefully attaching socket to the port 8080
    if (setsockopt(socket_fd, SOL_SOCKET,
                   SO_REUSEADDR, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Socket binding failed!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket binding successed!\n");
    }

    // Listen for connections
    if (listen(socket_fd, 1) < 0)
    {
        perror("Socket listening failed!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket listening successed!\n");
    }

    // Create shared memory segment
    int shm_id = shmget(IPC_PRIVATE, sizeof(struct Email), IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror("Shared memory creation failed!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Shared memory creation sucessed!\n");
    }

    // Fork to create process 2
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Forking process 2 failed\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {

        // while (true)
        // {
        processChild(socket_fd, shm_id, "email_archive.csv");
        // }
    }
    else
    {
        int addrlen = sizeof(server_address);
        int new_socket = accept(socket_fd, (struct sockaddr *)&server_address,
                                (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept failed!\n");
            exit(EXIT_FAILURE);
        }

        processParent(new_socket, shm_id, import_files);
    }

    return 0;
}

void sendMessage(int socket_fd, const char *message)
{
    send(socket_fd, (void *)message, strlen(message), 0);

    // time out 1 second
    // sleep(1);
}

void receiveMessage(int socket_fd, const char *buffer)
{
    recv(socket_fd, (void *)buffer, strlen(buffer), 0);

    // time out 1 second
    // sleep(1);
}

void writeToSharedMemory(int shm_id, const struct Email *email)
{
    struct Email *shared_data = (struct Email *)shmat(shm_id, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("Attaching to shared memory failed");
        exit(EXIT_FAILURE);
    }

    memcpy(shared_data, email, sizeof(struct Email));

    shmdt(shared_data);
}

void readFromSharedMemory(int shm_id, struct Email *email)
{
    struct Email *shared_data = (struct Email *)shmat(shm_id, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("Attaching to shared memory failed");
        exit(EXIT_FAILURE);
    }

    memcpy(email, shared_data, sizeof(struct Email));

    shmdt(shared_data);
}

void processToImportfile(const char *filename, int socket_fd, int shm_id)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file!");
        return;
    }

    struct Email email = {0};
    char buffer[BUFFER_SIZE] = {0};

    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        if (strncmp(buffer, "From:", 5) == 0)
        {
            strcpy(email.sender, buffer + 6);
            email.sender[strcspn(email.sender, "\n")] = '\0'; // Remove trailing newline
        }
        else if (strncmp(buffer, "To:", 3) == 0)
        {
            strcpy(email.receiver, buffer + 4);
            email.receiver[strcspn(email.receiver, "\n")] = '\0'; // Remove trailing newline
        }
        else if (strlen(buffer) > 1)
        {
            strcat(email.content, buffer);
            email.content[strcspn(email.content, "\n")] = '\0'; // Remove trailing newline
        }
    }
    fclose(file);

    // Check if Process 2 is alive
    sendMessage(socket_fd, IS_ALIVE_MESSAGE);

    // Wait for response from Process 2
    char response[BUFFER_SIZE];
    receiveMessage(socket_fd, response);
    printf("abc: %s\n", response);
    writeToSharedMemory(shm_id, &email);
    struct Email email1;
    memset(&email1, 0, sizeof(struct Email));
    readFromSharedMemory(shm_id, &email1);
    // if (strcmp(response, ALIVE_MESSAGE) == 0) {
    //     // Process 2 is alive, save email data to shared memory
    //     writeToSharedMemory(shm_id, &email);
    // } else {
    //     // Process 2 is not alive, fork new Process 2
    //     pid_t new_pid = fork();

    //     if (new_pid < 0) {
    //         perror("Forking new Process 2 failed");
    //         exit(EXIT_FAILURE);
    //     } else if (new_pid == 0) {
    //         // Child process (new Process 2)
    //         execlp("./process2", "./process2", NULL);
    //     } else {
    //         // Parent process (Process 1)
    //         sleep(1); // Give some time for Process 2 to start
    //     }
    // }
}