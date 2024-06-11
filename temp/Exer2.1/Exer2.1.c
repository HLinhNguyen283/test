#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILES 100

int main() {
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    char *folder = "."; // thư mục hiện tại
    char filenames[MAX_FILES][256];

    if ((dir = opendir(folder)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strlen(ent->d_name) > 4 && strcmp(ent->d_name + strlen(ent->d_name) - 4, ".txt") == 0) {
                strncpy(filenames[count], ent->d_name, 255);
                printf("%s\n", filenames[count]);
                count++;
            }
        }
        closedir(dir);
    } else {
        perror("opendir");
        return 1;
    }

    printf("Tổng số file .txt: %d\n", count);

    return 0;
}
