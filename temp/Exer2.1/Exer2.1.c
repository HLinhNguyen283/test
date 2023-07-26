// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <sys/wait.h>

// #define MAX_BUFFER_SIZE 1024
// #define SHM_KEY 1234

// // Structure to store email content
// struct Email {
//     char sender[MAX_BUFFER_SIZE];
//     char receiver[MAX_BUFFER_SIZE];
//     char content[MAX_BUFFER_SIZE];
// };

// // Function to handle Process 1 (Parent process)
// void process1(char* filenames[], int num_files) {
//     int sockfd;
//     struct sockaddr_in serv_addr;

//     // Create socket
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0) {
//         perror("Error creating socket");
//         exit(1);
//     }

//     // Set up socket address
//     memset(&serv_addr, '0', sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(8080);

//     // Connect to Process 2 (Child process)
//     if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//         perror("Error connecting to Process 2");
//         exit(1);
//     }

//     // Create shared memory segment
//     int shmid;
//     struct Email* shared_memory;
//     shmid = shmget(SHM_KEY, sizeof(struct Email), IPC_CREAT | 0666);
//     if (shmid < 0) {
//         perror("Error creating shared memory segment");
//         exit(1);
//     }

//     // Attach shared memory
//     shared_memory = (struct Email*)shmat(shmid, NULL, 0);

//     // Loop through each file and read data
//     for (int i = 0; i < num_files; i++) {
//         read_data_from_files(filenames[i], shared_memory);
//         send_is_alive_message(sockfd);
//         if (!is_process2_alive(sockfd)) {
//             pid_t pid = fork();
//             if (pid == 0) {
//                 process2();
//             }
//         }
//         save_email_data_to_shared_memory(shared_memory);
//     }

//     // Send "shutdown" message to Process 2
//     send_shutdown_message(sockfd);

//     // Wait for Process 2 to terminate
//     wait(NULL);

//     // Detach and remove shared memory
//     shmdt(shared_memory);
//     shmctl(shmid, IPC_RMID, NULL);

//     // Close the socket
//     close(sockfd);
// }

// // Function to read data from import files
// void read_data_from_files(char* filename, struct Email* shared_memory) {
//     FILE* file = fopen(filename, "r");
//     if (file == NULL) {
//         perror("Error opening file");
//         exit(1);
//     }

//     // Read email data from the file
//     fscanf(file, "From: %[^\n]\n", shared_memory->sender);
//     fscanf(file, "To: %[^\n]\n", shared_memory->receiver);
//     fscanf(file, "%[^\n]\n", shared_memory->content);

//     fclose(file);
// }

// // Function to send "is_alive" message to Process 2 via socket
// void send_is_alive_message(int sockfd) {
//     char message[MAX_BUFFER_SIZE] = "is_alive";
//     send(sockfd, message, strlen(message), 0);
// }

// // Function to receive "alive" message from Process 2 via socket
// int receive_alive_message(int sockfd) {
//     char message[MAX_BUFFER_SIZE];
//     memset(message, 0, MAX_BUFFER_SIZE);
//     int bytes_received = recv(sockfd, message, MAX_BUFFER_SIZE, 0);
//     if (bytes_received < 0) {
//         perror("Error receiving data");
//         exit(1);
//     }
//     return strcmp(message, "alive") == 0;
// }

// // Function to check if Process 2 is alive
// int is_process2_alive(int sockfd) {
//     send_is_alive_message(sockfd);
//     return receive_alive_message(sockfd);
// }

// // Function to save email data to shared memory
// void save_email_data_to_shared_memory(struct Email* shared_memory) {
//     // Save email data to the shared memory segment
// }

// // Function to handle Process 2 (Child process)
// void process2() {
//     int sockfd, newsockfd;
//     struct sockaddr_in serv_addr, client_addr;
//     socklen_t client_len;
//     int shmid;
//     struct Email* shared_memory;

//     // Create socket
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0) {
//         perror("Error creating socket");
//         exit(1);
//     }

//     // Set up socket address
//     memset(&serv_addr, '0', sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//     serv_addr.sin_port = htons(8080);

//     // Bind socket to address
//     if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//         perror("Error binding socket");
//         exit(1);
//     }

//     // Listen for incoming connections
//     if (listen(sockfd, 10) < 0) {
//         perror("Error listening");
//         exit(1);
//     }

//     // Accept connection from Process 1 (Parent process)
//     client_len = sizeof(client_addr);
//     newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
//     if (newsockfd < 0) {
//         perror("Error accepting connection");
//         exit(1);
//     }

//     // Create shared memory segment
//     shmid = shmget(SHM_KEY, sizeof(struct Email), IPC_CREAT | 0666);
//     if (shmid < 0) {
//         perror("Error creating shared memory segment");
//         exit(1);
//     }

//     // Attach shared memory
//     shared_memory = (struct Email*)shmat(shmid, NULL, 0);

//     // Loop to process shared memory data
//     while (1) {
//         if (shared_memory->sender[0] != '\0') {
//             process_shared_memory_data(shared_memory);
//             shared_memory->sender[0] = '\0'; // Reset shared memory data
//         }
//         // Check for "shutdown" message
//         if (!is_process2_alive(newsockfd)) {
//             break;
//         }
//     }

//     // Detach shared memory
//     shmdt(shared_memory);

//     // Close the sockets
//     close(newsockfd);
//     close(sockfd);

//     // Terminate Process 2
//     exit(0);
// }

// // Function to read and process data from shared memory in Process 2
// void process_shared_memory_data(struct Email* shared_memory) {
//     // Process and log content to the "email_archive.csv" file
// }

// // Function to send "shutdown" message to Process 2 via socket
// void send_shutdown_message(int sockfd) {
//     char message[MAX_BUFFER_SIZE] = "shutdown";
//     send(sockfd, message, strlen(message), 0);
// }

// int main() {
//     char* filenames[] = {
//         "file1.txt",
//         "file2.txt",
//         "file3.txt",
//         "file4.txt",
//         "file5.txt"
//     };
//     int num_files = sizeof(filenames) / sizeof(filenames[0]);

//     // Call Process 1 function
//     process1(filenames, num_files);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAX_FILES 5
#define MAX_BUFFER_SIZE 1024

// Structure to hold email data
struct EmailData {
    char sender[MAX_BUFFER_SIZE];
    char receiver[MAX_BUFFER_SIZE];
    char content[MAX_BUFFER_SIZE];
};

// Function to process data and write to email_archive.csv
void process_and_save_data(struct EmailData* shared_memory, int num_emails) {
    FILE* fp = fopen("email_archive.csv", "a");
    if (fp == NULL) {
        perror("Error opening email_archive.csv");
        exit(1);
    }

    fprintf(fp, "From;To;Content\n");

    for (int i = 0; i < num_emails; ++i) {
        fprintf(fp, "%s;%s;%s\n", shared_memory[i].sender, shared_memory[i].receiver, shared_memory[i].content);
    }

    fclose(fp);
}

int main() {
    int num_files = 5;
    char* file_names[] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};
    char buffer[MAX_BUFFER_SIZE];

    // Create shared memory
    const int num_emails = 100; // Assuming a maximum of 100 emails in shared memory
    const size_t shared_memory_size = sizeof(struct EmailData) * num_emails;
    int shm_fd = shm_open("/email_data", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Shared memory creation failed");
        return 1;
    }

    ftruncate(shm_fd, shared_memory_size);
    struct EmailData* shared_memory = mmap(NULL, shared_memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("Shared memory mapping failed");
        return 1;
    }

    // Socket setup
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // Port number
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP (localhost)

    // Fork process 2
    pid_t pid = fork();

    if (pid < 0) {
        perror("Forking process 2 failed");
        return 1;
    } else if (pid == 0) {
        // Process 2 (Child process)

        while (1) {
            // Wait for "is_alive" message from process 1
            recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);

            // Respond with "alive" message
            sendto(sockfd, "alive", strlen("alive"), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

            // Check if shared memory has data
            int num_emails_in_shared_memory = 0;
            for (int i = 0; i < num_emails; ++i) {
                if (strlen(shared_memory[i].sender) > 0) {
                    ++num_emails_in_shared_memory;
                } else {
                    break;
                }
            }

            if (num_emails_in_shared_memory > 0) {
                process_and_save_data(shared_memory, num_emails_in_shared_memory);
                // Clear shared memory after processing
                memset(shared_memory, 0, sizeof(struct EmailData) * num_emails);
            }

            // Check if shutdown message received
            if (strcmp(buffer, "shutdown") == 0) {
                break;
            }
        }

        close(sockfd);
        munmap(shared_memory, shared_memory_size);
        close(shm_fd);
        return 0;
    } else {
        // Process 1 (Parent process)

        for (int i = 0; i < num_files; ++i) {
            // Read data from import files and save to shared memory
            FILE* fp = fopen(file_names[i], "r");
            if (fp == NULL) {
                perror("Error opening file");
                continue;
            }

            struct EmailData email;
            memset(&email, 0, sizeof(struct EmailData));

            fgets(email.sender, MAX_BUFFER_SIZE, fp);
            fgets(email.receiver, MAX_BUFFER_SIZE, fp);
            fgets(email.content, MAX_BUFFER_SIZE, fp);

            fclose(fp);

            // Send "is_alive" message to process 2 via socket
            sendto(sockfd, "is_alive", strlen("is_alive"), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

            // Wait for response from process 2
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int n = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
            buffer[n] = '\0';

            // Check if process 2 is alive
            if (strcmp(buffer, "alive") == 0) {
                // Save email data to shared memory
                memcpy(&shared_memory[i], &email, sizeof(struct EmailData));
            } else {
                // Fork new process 2
                pid_t new_pid = fork();

                if (new_pid < 0) {
                    perror("Forking new process 2 failed");
                    return 1;
                } else if (new_pid == 0) {
                    // This is the new process 2
                    break;
                }
            }
        }

        // Send "shutdown" message to process 2
        sendto(sockfd, "shutdown", strlen("shutdown"), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        // Wait for process 2 to end
        wait(NULL);

        close(sockfd);
        munmap(shared_memory, shared_memory_size);
        close(shm_fd);
        shm_unlink("/email_data");

        return 0;
    }
}
