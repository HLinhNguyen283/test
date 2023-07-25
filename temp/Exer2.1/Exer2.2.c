// // // ==========================================================================
// // // Include files

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <sys/types.h>
// // #include <sys/socket.h>
// // #include <netinet/in.h>
// // #include <unistd.h>
// // #include <arpa/inet.h>
// // #include <sys/ipc.h>
// // #include <sys/shm.h>
// // #include <sys/wait.h>
// // #include <fcntl.h>

// // // ==========================================================================
// // // Define constants

// // #define PORT 8080
// // #define BUFFER_SIZE 1024
// // #define SHARED_MEM_SIZE 1024
// // #define SHUTDOWN_MESSAGE "shutdown"
// // #define ALIVE_MESSAGE "alive"
// // #define IS_ALIVE_MESSAGE "is_alive"
// // // ==========================================================================
// // // Define the email structure

// // struct Email {
// //     char sender[256];
// //     char receiver[256];
// //     char content[1024];
// // };

// // const char* import_files[] = {
// //     "email_1.txt",
// //     "email_2.txt",
// //     "email_3.txt",
// //     "email_4.txt",
// //     "email_5.txt"
// // };

// // void sendMessage(int socket_fd, const char* message);
// // void receiveMessage(int socket_fd, const char* buffer);
// // void writeToSharedMemory(int shm_id, const struct Email* email);
// // void readFromSharedMemory(int shm_id, struct Email* email);

// // // Function to write email data to the summary file
// // void writeToSummaryFile(const char* filename, const struct Email* email) 
// // {
// //     FILE* file = fopen(filename, "a");
// //     if (file) {
// //         fprintf(file, "%s;%s;%s\n", email->sender, email->receiver, email->content);
// //         fclose(file);
// //     } else {
// //         perror("Error opening summary file");
// //     }
// // }

// // // Function to process child process
// // void processChild(int socket_fd, int shm_id, const char* summary_file)
// // {
// //     while (true)
// //     {
// //         // Wait for "is_alive" message from Process 1
// //         int client_socket = accept(socket_fd, NULL, NULL);
// //         if (client_socket < 0) {
// //             perror("Accepting connection failed!");
// //             exit(EXIT_FAILURE);
// //         } else {
// //             printf("Accepting connection successed!");
// //         }

// //         // Wait for "is_alive" message from the parent
// //         char buffer[1024];
// //         receiveMessage(socket_fd, buffer);

// //         if (strcmp(buffer, IS_ALIVE_MESSAGE) == 0) 
// //         {
// //             // Respond with "alive" message
// //             sendMessage(client_socket, ALIVE_MESSAGE);

// //             // Process shared memory data if available
// //             struct Email email;
// //             memset(&email, 0, sizeof(struct Email));
// //             readFromSharedMemory(shm_id, &email);
// //             if (strlen(email.sender) > 0) {

// //                 // Log content to summary file (email_archive.csv)struct
// //                 // strcpy(email.sender, strtok(shared_memory, "\n"));
// //                 // strcpy(email.receiver, strtok(NULL, "\n"));
// //                 // strcpy(email.content, strtok(NULL, "\n"));

// //                 writeToSummaryFile(summary_file, &email);
                
// //                 // Clear shared memory
// //                 memset(&email, 0, sizeof(struct Email));
// //                 write_to_shared_memory(shm_id, &email);
// //             }
// //         }
// //         // Wait for "shutdown" message from the parent
// //         recv(socket_fd, buffer, sizeof(buffer), 0);

// //         // Detach and remove shared memory
// //         shmdt(shared_memory);
// //         shmctl(shmid, IPC_RMID, NULL);

// //         // Close the socket and exit
// //         close(socket_fd);
// //         exit(0);
// //     }
// // }

// // // Function to process parent process
// // void processParent(int socket_fd, int shm_id, const char* files[], int num_files)
// // {
// //     // Check if Process 2 is alive
// //     sendMessage(socket_fd, IS_ALIVE_MESSAGE);

// //     // Wait for response from Process 2
// //     char response[256] = {0};
// //     receive_message(socket_fd, response);

// //     if (strcmp(buffer, "alive") != 0) {
// //         // Process 2 is not alive, fork new process
// //         pid_t pid = fork();
// //         if (pid == 0) {
// //             // Child process
// //             prinft("Child process is created!");
// //             processChild(socket_fd, "email_archive.csv");
// //         } else if (pid < 0) {
// //             perror("Error forking process");
// //             exit(1);
// //         }
// //     }

// //     // Process data from import files
// //     for (int i = 0; i < num_files; ++i) {
// //         FILE* file = fopen(files[i], "r");
// //         if (file) {
// //             struct Email email;

// //             // Read email data from the file
// //             fgets(email.sender, sizeof(email.sender), file);
// //             fgets(email.receiver, sizeof(email.receiver), file);
// //             fgets(email.content, sizeof(email.content), file);

// //             // Push email data to shared memory
// //             strcpy(shared_memory, email.sender);
// //             strcat(shared_memory, "\n");
// //             strcat(shared_memory, email.receiver);
// //             strcat(shared_memory, "\n");
// //             strcat(shared_memory, email.content);
// //             strcat(shared_memory, "\n");

// //             fclose(file);
// //         } else {
// //             printf("Error opening file: %s\n", files[i]);
// //         }
// //     }

// //     if (strcmp(response, ALIVE_MESSAGE) == 0) {
// //         // Process 2 is alive, save email data to shared memory
// //         write_to_shared_memory(shm_id, &email);
// //     } else {
// //         // Process 2 is not alive, fork new Process 2
// //         pid_t new_pid = fork();

// //         if (new_pid < 0) {
// //             perror("Forking new Process 2 failed");
// //             exit(EXIT_FAILURE);
// //         } else if (new_pid == 0) {
// //             // Child process (new Process 2)
// //             execlp("./process2", "./process2", NULL);
// //         } else {
// //             // Parent process (Process 1)
// //             sleep(1); // Give some time for Process 2 to start
// //         }
// //     }

// //     // Send "shutdown" message to the child
// //     send(socket_fd, "shutdown", 9, 0);
// //     recv(socket_fd, buffer, sizeof(buffer), 0);

// //     // Detach and remove shared memory
// //     shmdt(shared_memory);
// //     shmctl(shmid, IPC_RMID, NULL);

// //     // Close the socket
// //     close(socket_fd);
// // }

// // int main() 
// // {
// //     // Create a socket for IPC
// //     int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
// //     if (socket_fd < 0) {
// //         perror("Socket creation failed!");
// //         exit(EXIT_FAILURE);
// //     } else {
// //         prinf("Socket creation successed!");
// //     }

// //     // Bind the socket
// //     struct sockaddr_in server_address;
// //     server_address.sin_family = AF_INET;
// //     server_address.sin_addr.s_addr = INADDR_ANY;
// //     server_address.sin_port = htons(PORT);

// //     if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
// //         perror("Socket binding failed!");
// //         exit(EXIT_FAILURE);
// //     } else {
// //         prinf("Socket binding successed!");
// //     }

// //     // Listen for connections
// //     if(listen(socket_fd, 1) < 0) {
// //         perror("Socket listening failed!");
// //         exit(EXIT_FAILURE);
// //     } else {
// //         prinf("Socket listening successed!");
// //     }

// //     // Create shared memory segment
// //     int shm_id = shmget(IPC_PRIVATE, sizeof(struct Email), IPC_CREAT | 0666);
// //     if (shm_id < 0) {
// //         perror("Shared memory creation failed!");
// //         exit(EXIT_FAILURE);
// //     } else {
// //         perror("Shared memory creation sucessed!");
// //         exit(EXIT_FAILURE);
// //     }

// //     // Fork to create process 2
// //     pid_t pid = fork();
// //     if (pid < 0) {
// //         perror("Forking process 2 failed");
// //         exit(EXIT_FAILURE);
// //     } else if (pid == 0) {
// //         processChild(socket_fd, shm_id, "email_archive.csv");
// //     } else {
// //         processParent(socket_fd, import_files, MAX_FILES);
// //     }


// //     return 0;
// // }

// // void sendMessage(int socket_fd, const char* message) {
// //     send(socket_fd, message, strlen(message), 0);

// //     //time out 1 second
// //     sleep(1);
// // }

// // void receiveMessage(int socket_fd, const char* buffer) {
// //     recv(socket_fd, buffer, strlen(buffer), 0);

// //     //time out 1 second
// //     sleep(1);
// // }

// // void writeToSharedMemory(int shm_id, const struct Email* email) {
// //     struct Email* shared_data = (struct Email*)shmat(shm_id, NULL, 0);
// //     if (shared_data == (void*)-1) {
// //         perror("Attaching to shared memory failed");
// //         exit(EXIT_FAILURE);
// //     }

// //     memcpy(shared_data, email, sizeof(struct Email));

// //     shmdt(shared_data);
// // }

// // void readFromSharedMemory(int shm_id, struct Email* email) {
// //     struct Email* shared_data = (struct Email*)shmat(shm_id, NULL, 0);
// //     if (shared_data == (void*)-1) {
// //         perror("Attaching to shared memory failed");
// //         exit(EXIT_FAILURE);
// //     }

// //     memcpy(email, shared_data, sizeof(struct Email));

// //     shmdt(shared_data);
// // }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <sys/shm.h>
// #include <sys/ipc.h>

// #define SHARED_MEMORY_KEY 1234
// #define SHUTDOWN_MESSAGE "shutdown"
// #define ALIVE_MESSAGE "alive"
// #define IS_ALIVE_MESSAGE "is_alive"
// #define BUFFER_SIZE 1024

// struct Email {
//     char from[100];
//     char to[100];
//     char content[500];
// };

// void process_file(const char* filename, int socket_fd, int shm_id);
// void send_message(int socket_fd, const char* message);
// void receive_message(int socket_fd, char* buffer);
// void write_to_shared_memory(int shm_id, const struct Email* email);
// void read_from_shared_memory(int shm_id, struct Email* email);

// int main() {
//     // Create a socket for IPC
//     int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (socket_fd < 0) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     // Bind the socket
//     struct sockaddr_in server_address;
//     server_address.sin_family = AF_INET;
//     server_address.sin_addr.s_addr = INADDR_ANY;
//     server_address.sin_port = htons(8080);

//     if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
//         perror("Socket binding failed");
//         exit(EXIT_FAILURE);
//     }

//     // Listen for connections
//     listen(socket_fd, 1);

//     // Create shared memory segment
//     int shm_id = shmget(SHARED_MEMORY_KEY, sizeof(struct Email), IPC_CREAT | 0666);
//     if (shm_id < 0) {
//         perror("Shared memory creation failed");
//         exit(EXIT_FAILURE);
//     }

//     // Fork to create process 2
//     pid_t pid = fork();

//     if (pid < 0) {
//         perror("Forking process 2 failed");
//         exit(EXIT_FAILURE);
//     } else if (pid == 0) {
//         // Process 2 (Child process)
//         struct Email email;

//         while (1) {
//             // Wait for "is_alive" message from Process 1
//             int client_socket = accept(socket_fd, NULL, NULL);
//             if (client_socket < 0) {
//                 perror("Accepting connection failed");
//                 exit(EXIT_FAILURE);
//             }

//             char buffer[BUFFER_SIZE];
//             receive_message(client_socket, buffer);

//             if (strcmp(buffer, IS_ALIVE_MESSAGE) == 0) {
//                 send_message(client_socket, ALIVE_MESSAGE);

//                 // Process shared memory data if available
//                 read_from_shared_memory(shm_id, &email);
//                 if (strlen(email.from) > 0) {
//                     // Log content to summary file (email_archive.csv)
//                     FILE* archive_file = fopen("email_archive.csv", "a");
//                     fprintf(archive_file, "%s;%s;%s\n", email.from, email.to, email.content);
//                     fclose(archive_file);

//                     // Clear shared memory
//                     memset(&email, 0, sizeof(struct Email));
//                     write_to_shared_memory(shm_id, &email);
//                 }
//             }

//             close(client_socket);

//             // Check if "shutdown" message received
//             if (strcmp(buffer, SHUTDOWN_MESSAGE) == 0) {
//                 break;
//             }
//         }

//         // Detach and remove shared memory segment
//         shmdt(&email);
//         shmctl(shm_id, IPC_RMID, NULL);

//         exit(EXIT_SUCCESS);
//     } else {
//         // Process 1 (Parent process)
//         const char* filenames[] = {
//             "file1.txt",
//             "file2.txt",
//             "file3.txt",
//             "file4.txt",
//             "file5.txt"
//         };

//         struct Email email;
//         int i;

//         for (i = 0; i < 5; i++) {
//             process_file(filenames[i], socket_fd, shm_id);
//         }

//         // Send "shutdown" message to Process 2
//         send_message(socket_fd, SHUTDOWN_MESSAGE);

//         // Wait for Process 2 to shutdown
//         sleep(1);

//         // Close the socket
//         close(socket_fd);

//         exit(EXIT_SUCCESS);
//     }
// }

// void process_file(const char* filename, int socket_fd, int shm_id) {
//     FILE* file = fopen(filename, "r");
//     if (file == NULL) {
//         perror("Error opening file");
//         return;
//     }

//     struct Email email;
//     char buffer[BUFFER_SIZE];

//     while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
//         if (strncmp(buffer, "From:", 5) == 0) {
//             strcpy(email.from, buffer + 6);
//             email.from[strcspn(email.from, "\n")] = '\0'; // Remove trailing newline
//         } else if (strncmp(buffer, "To:", 3) == 0) {
//             strcpy(email.to, buffer + 4);
//             email.to[strcspn(email.to, "\n")] = '\0'; // Remove trailing newline
//         } else if (strlen(buffer) > 1) {
//             strcpy(email.content, buffer);
//             email.content[strcspn(email.content, "\n")] = '\0'; // Remove trailing newline
//         }
//     }

//     fclose(file);

//     // Check if Process 2 is alive
//     send_message(socket_fd, IS_ALIVE_MESSAGE);

//     // Wait for response from Process 2
//     char response[BUFFER_SIZE];
//     receive_message(socket_fd, response);

//     if (strcmp(response, ALIVE_MESSAGE) == 0) {
//         // Process 2 is alive, save email data to shared memory
//         write_to_shared_memory(shm_id, &email);
//     } else {
//         // Process 2 is not alive, fork new Process 2
//         pid_t new_pid = fork();

//         if (new_pid < 0) {
//             perror("Forking new Process 2 failed");
//             exit(EXIT_FAILURE);
//         } else if (new_pid == 0) {
//             // Child process (new Process 2)
//             execlp("./process2", "./process2", NULL);
//         } else {
//             // Parent process (Process 1)
//             sleep(1); // Give some time for Process 2 to start
//         }
//     }
// }

// void send_message(int socket_fd, const char* message) {
//     send(socket_fd, message, strlen(message), 0);
// }

// void receive_message(int socket_fd, char* buffer) {
//     recv(socket_fd, buffer, BUFFER_SIZE, 0);
// }

// void write_to_shared_memory(int shm_id, const struct Email* email) {
//     struct Email* shared_data = (struct Email*)shmat(shm_id, NULL, 0);
//     if (shared_data == (void*)-1) {
//         perror("Attaching to shared memory failed");
//         exit(EXIT_FAILURE);
//     }

//     memcpy(shared_data, email, sizeof(struct Email));

//     shmdt(shared_data);
// }

// void read_from_shared_memory(int shm_id, struct Email* email) {
//     struct Email* shared_data = (struct Email*)shmat(shm_id, NULL, 0);
//     if (shared_data == (void*)-1) {
//         perror("Attaching to shared memory failed");
//         exit(EXIT_FAILURE);
//     }

//     memcpy(email, shared_data, sizeof(struct Email));

//     shmdt(shared_data);
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SHARED_MEMORY_KEY 1234
#define SHUTDOWN_MESSAGE "shutdown"
#define ALIVE_MESSAGE "alive"
#define IS_ALIVE_MESSAGE "is_alive"
#define BUFFER_SIZE 1024

struct Email {
    char from[100];
    char to[100];
    char content[500];
};

void process_file(const char* filename, int socket_fd, int shm_id);
void send_message(int socket_fd, const char* message);
void receive_message(int socket_fd, char* buffer);
void write_to_shared_memory(int shm_id, const struct Email* email);
void read_from_shared_memory(int shm_id, struct Email* email);

int main() {
    // Create a socket for IPC
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    listen(socket_fd, 1);

    // Create shared memory segment
    int shm_id = shmget(SHARED_MEMORY_KEY, sizeof(struct Email), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("Shared memory creation failed");
        exit(EXIT_FAILURE);
    }

    // Fork to create process 2
    pid_t pid = fork();

    if (pid < 0) {
        perror("Forking process 2 failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Process 2 (Child process)
        struct Email email;

        while (1) {
            // Wait for "is_alive" message from Process 1
            int client_socket = accept(socket_fd, NULL, NULL);
            if (client_socket < 0) {
                perror("Accepting connection failed");
                exit(EXIT_FAILURE);
            }

            char buffer[BUFFER_SIZE];
            receive_message(client_socket, buffer);

            if (strcmp(buffer, IS_ALIVE_MESSAGE) == 0) {
                send_message(client_socket, ALIVE_MESSAGE);

                // Process shared memory data if available
                read_from_shared_memory(shm_id, &email);
                if (strlen(email.from) > 0) {
                    // Log content to summary file (email_archive.csv)
                    FILE* archive_file = fopen("email_archive.csv", "a");
                    fprintf(archive_file, "%s;%s;%s\n", email.from, email.to, email.content);
                    fclose(archive_file);

                    // Clear shared memory
                    memset(&email, 0, sizeof(struct Email));
                    write_to_shared_memory(shm_id, &email);
                }
            }

            close(client_socket);

            // Check if "shutdown" message received
            if (strcmp(buffer, SHUTDOWN_MESSAGE) == 0) {
                break;
            }
        }

        // Detach and remove shared memory segment
        shmdt(&email);
        shmctl(shm_id, IPC_RMID, NULL);

        exit(EXIT_SUCCESS);
    } else {
        // Process 1 (Parent process)
        const char* filenames[] = {
            "file1.txt",
            "file2.txt",
            "file3.txt",
            "file4.txt",
            "file5.txt"
        };

        struct Email email;
        int i;

        for (i = 0; i < 5; i++) {
            process_file(filenames[i], socket_fd, shm_id);
        }

        // Wait for Process 2 to finish
        int status;
        waitpid(pid, &status, 0);

        // Close the socket
        close(socket_fd);

        exit(EXIT_SUCCESS);
    }
}

void process_file(const char* filename, int socket_fd, int shm_id) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    struct Email email;
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        if (strncmp(buffer, "From:", 5) == 0) {
            strcpy(email.from, buffer + 6);
            email.from[strcspn(email.from, "\n")] = '\0'; // Remove trailing newline
        } else if (strncmp(buffer, "To:", 3) == 0) {
            strcpy(email.to, buffer + 4);
            email.to[strcspn(email.to, "\n")] = '\0'; // Remove trailing newline
        } else if (strlen(buffer) > 1) {
            strcpy(email.content, buffer);
            email.content[strcspn(email.content, "\n")] = '\0'; // Remove trailing newline
        }
    }

    fclose(file);

    // Check if Process 2 is alive
    send_message(socket_fd, IS_ALIVE_MESSAGE);

    // Wait for response from Process 2
    char response[BUFFER_SIZE];
    receive_message(socket_fd, response);

    if (strcmp(response, ALIVE_MESSAGE) == 0) {
        // Process 2 is alive, save email data to shared memory
        write_to_shared_memory(shm_id, &email);
    } else {
        // Process 2 is not alive, fork new Process 2
        pid_t new_pid = fork();

        if (new_pid < 0) {
            perror("Forking new Process 2 failed");
            exit(EXIT_FAILURE);
        } else if (new_pid == 0) {
            // Child process (new Process 2)
            execlp("./process2", "./process2", NULL);
        } else {
            // Parent process (Process 1)
            sleep(1); // Give some time for Process 2 to start
        }
    }
}

void send_message(int socket_fd, const char* message) {
    send(socket_fd, message, strlen(message), 0);
}

void receive_message(int socket_fd, char* buffer) {
    recv(socket_fd, buffer, BUFFER_SIZE, 0);
}

void write_to_shared_memory(int shm_id, const struct Email* email) {
    struct Email* shared_data = (struct Email*)shmat(shm_id, NULL, 0);
    if (shared_data == (void*)-1) {
        perror("Attaching to shared memory failed");
        exit(EXIT_FAILURE);
    }

    memcpy(shared_data, email, sizeof(struct Email));

    shmdt(shared_data);
}

void read_from_shared_memory(int shm_id, struct Email* email) {
    struct Email* shared_data = (struct Email*)shmat(shm_id, NULL, 0);
    if (shared_data == (void*)-1) {
        perror("Attaching to shared memory failed");
        exit(EXIT_FAILURE);
    }

    memcpy(email, shared_data, sizeof(struct Email));

    shmdt(shared_data);
}
