#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "rm.h"
#include <readline/readline.h>
#include <readline/history.h>
void help_rm()
{

    printf("rm is a command that removes files and directories\n");
    printf("Option available:\n");
    printf("-i   interactive will ask the user if he really want to delete the file\n");
    printf("-v   verbose it will show what files are being deleted\n");
    printf("-r and -R recursive it will recursively delete the files\n");
}
int is_directory(const char *file)
{
    struct stat fileStat;
    if (stat(file, &fileStat) < 0)
    {
        perror("Stat error");
        return -1;
    }
    return S_ISDIR(fileStat.st_mode);
}
int is_file(const char *filename)
{
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fclose(file);
        return 1;
    }
    else
    {
        return 0;
    }
}
void Recursive_deletion(const char *file)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(file)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        char subpath[1024];

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(subpath, sizeof(subpath), "%s/%s", file, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            Recursive_deletion(subpath);
        }
        else
        {
            unlink(subpath);
        }
    }

    closedir(dir);
    rmdir(file);
}

void own_RM_command(char *arguments[], int n)
{
    int opt_i = 0;
    int opt_v = 0;
    int opt_r = 0;
    int opt_R = 0;
    if (strcmp(arguments[0], "rm") != 0)
    {
        printf("invalid usage, first argument has to be rm\n");
        return;
    }
    if (n < 2)
    {
        printf("USAGE: rm: [OPTION]...[FILE]\n");
        printf("You can use rm --help to display the help menu of the command\n");
        return;
    }
    else if (strcmp(arguments[1], "--help") == 0)
    {
        help_rm();
        return;
    }

    char *files[1000];
    int filenumber = 0;
    char *options[1000];
    int optionsnumber = 0;
    int i = 1;
    while (arguments[i])
    {
        if (strcmp(arguments[i], "-i") == 0 || strcmp(arguments[i], "-v") == 0 || strcmp(arguments[i], "-r") == 0 || strcmp(arguments[i], "-R") == 0)
        {
            options[optionsnumber] = malloc(strlen(arguments[i]) + 1);
            strcpy(options[optionsnumber], arguments[i]);
            optionsnumber++;
        }
        else
        {
            files[filenumber] = malloc(strlen(arguments[i]) + 1);
            strcpy(files[filenumber], arguments[i]);
            filenumber++;
        }
        i++;
    }
    if (filenumber == 0)
    {
        printf("No file was specified\n");
        printf("USAGE: rm: [OPTION]...[FILE]\n");
        printf("You can use rm --help to display the help menu of the command\n");
        return;
    }
    if (optionsnumber == 0)
    {

        for (int i = 0; files[i]; i++)
        {

            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;

            if (is_directory(files[i]) == 1)
            {
                printf("rm: cannot remove %s: is a directory\n", files[i]);
                continue;
            }

            if (remove(files[i]) != 0)
            {
                printf("Error deleting the file %s\n", files[i]);
                printf("The file does not exists\n");
                return;
            }
        }
    }
    if (optionsnumber != 0)
    {
        opt_i = 0;
        opt_v = 0;
        opt_r = 0;
        opt_R = 0;
        for (int i = 0; options[i]; i++)
        {
            if (strcmp(options[i], "-i") == 0)
                opt_i = 1;
            else if (strcmp(options[i], "-v") == 0)
                opt_v = 1;
            else if (strcmp(options[i], "-r") == 0)
                opt_r = 1;
            else
                opt_R = 1;
        }
    }
    if (opt_i == 1 && opt_v == 0 && opt_R == 0 && opt_r == 0)
    {

        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
            {
                printf("Cannot remove %s because it is a directory\n", files[i]);
                continue;
            }
            if (is_file(files[i]) == 0)
            {
                printf("The file does not exists\n");
                return;
            }
            char c;
            printf("rm: remove regular file %s ? ", files[i]);
            scanf(" %c", &c);
            if (c != 'y' && c != 'Y')
            {
                continue;
            }
            else
            {
                if (remove(files[i]) != 0)
                {
                    printf("Error deleting the file %s\n", files[i]);
                    printf("The file does not exists\n");
                    return;
                }
            }
        }
        opt_i = 0;
    }
    if (opt_i == 1 && opt_v == 1 && opt_R == 0 && opt_r == 0)
    {

        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
            {
                printf("Cannot remove %s because it is a directory\n", files[i]);
                continue;
            }
            char c;
            printf("rm: remove regular file %s ? ", files[i]);
            scanf(" %c", &c);
            if (c != 'y' && c != 'Y')
            {
                continue;
            }
            else
            {
                if (remove(files[i]) != 0)
                {
                    printf("Error deleting the file %s\n", files[i]);
                    printf("The file does not exists\n");
                    return;
                }
                printf("removed file: %s\n", files[i]);
            }
        }
        opt_i = 0;
        opt_v = 0;
    }
    if (opt_i == 1 && opt_v == 0 && opt_R == 0 && opt_r == 1)
    {

        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            char c;
            if (is_directory(files[i]) != 1 && is_file(files[i]) != 1)
            {
                continue;
            }
            if (is_directory(files[i]) == 1)
            {
                printf("rm: remove regular directory %s ? ", files[i]);
            }
            else
            {
                printf("rm: remove regular file %s ? ", files[i]);
            }
            scanf(" %c", &c);
            if (c != 'y' && c != 'Y')
            {
                continue;
            }
            else
            {
                if (is_directory(files[i]) == 1)
                    Recursive_deletion(files[i]);
                else
                {
                    if (remove(files[i]) != 0)
                    {
                        printf("Error deleting the file %s\n", files[i]);
                        printf("The file does not exists\n");
                        return;
                    }
                }
            }
        }
        opt_i = 0;
        opt_r = 0;
    }
    if (opt_i == 1 && opt_v == 0 && opt_R == 1 && opt_r == 0)
    {

        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            char c;
            if (is_directory(files[i]) != 1 && is_file(files[i]) != 1)
            {
                continue;
            }
            if (is_directory(files[i]) == 1)
            {
                printf("rm: remove regular directory %s ? ", files[i]);
            }
            else
            {
                printf("rm: remove regular file %s ? ", files[i]);
            }
            scanf(" %c", &c);
            if (c != 'y' && c != 'Y')
            {
                continue;
            }
            else
            {
                if (is_directory(files[i]) == 1)
                    Recursive_deletion(files[i]);
                else
                {
                    if (remove(files[i]) != 0)
                    {
                        printf("Error deleting the file %s\n", files[i]);
                        printf("The file does not exists\n");
                        return;
                    }
                }
            }
        }
        opt_R = 0;
        opt_i = 0;
    }
    if (opt_i == 1 && opt_v == 1 && opt_R == 0 && opt_r == 1)
    {

        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            char c;
            if (is_directory(files[i]) != 1 && is_file(files[i]) != 1)
            {
                continue;
            }
            if (is_directory(files[i]) == 1)
            {
                printf("rm: remove regular directory %s ? ", files[i]);
            }
            else
            {
                printf("rm: remove regular file %s ? ", files[i]);
            }
            scanf(" %c", &c);
            if (c != 'y' && c != 'Y')
            {
                continue;
            }
            else
            {
                if (is_directory(files[i]) == 1)
                {

                    Recursive_deletion(files[i]);
                    printf("removed directory: %s\n", files[i]);
                }
                else
                {
                    if (remove(files[i]) != 0)
                    {
                        printf("Error deleting the file %s\n", files[i]);
                        printf("The file does not exists\n");
                        return;
                    }
                    printf("removed file: %s\n", files[i]);
                }
            }
        }
        opt_r = 0;
        opt_v = 0;
        opt_i = 0;
    }
    if (opt_i == 1 && opt_v == 1 && opt_R == 1 && opt_r == 0)
    {

        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            char c;
            if (is_directory(files[i]) != 1 && is_file(files[i]) != 1)
            {
                continue;
            }
            if (is_directory(files[i]) == 1)
            {
                printf("rm: remove regular directory %s ? ", files[i]);
            }
            else
            {
                printf("rm: remove regular file %s ? ", files[i]);
            }
            scanf(" %c", &c);
            if (c != 'y' && c != 'Y')
            {
                continue;
            }
            else
            {
                if (is_directory(files[i]) == 1)
                {

                    Recursive_deletion(files[i]);
                    printf("removed directory: %s\n", files[i]);
                }
                else
                {
                    if (remove(files[i]) != 0)
                    {
                        printf("Error deleting the file %s\n", files[i]);
                        printf("The file does not exists\n");
                        return;
                    }
                    printf("removed file: %s\n", files[i]);
                }
            }
        }
        opt_i = 0;
        opt_v = 0;
        opt_R = 0;
    }
    if (opt_i == 1 && opt_v == 1 && opt_R == 1 && opt_r == 1)
    {

        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            char c;
            if (is_directory(files[i]) != 1 && is_file(files[i]) != 1)
            {
                continue;
            }
            if (is_directory(files[i]) == 1)
            {
                printf("rm: remove regular directory %s ? ", files[i]);
            }
            else
            {
                printf("rm: remove regular file %s ? ", files[i]);
            }
            scanf(" %c", &c);
            if (c != 'y' && c != 'Y')
            {
                continue;
            }
            else
            {
                if (is_directory(files[i]) == 1)
                {

                    Recursive_deletion(files[i]);
                    printf("removed directory: %s\n", files[i]);
                }
                else
                {
                    if (remove(files[i]) != 0)
                    {
                        printf("Error deleting the file %s\n", files[i]);
                        printf("The file does not exists\n");
                        return;
                    }
                    printf("removed file: %s\n", files[i]);
                }
            }
        }
        opt_i = 0;
        opt_v = 0;
        opt_r = 0;
        opt_R = 0;
    }
    if (opt_i == 0 && opt_v == 1 && opt_R == 0 && opt_r == 0)
    {
        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
            {
                printf("Cannot remove %s because it is a directory\n", files[i]);
                continue;
            }
            if (remove(files[i]) != 0)
            {
                printf("Error deleting the file %s\n", files[i]);
                return;
            }
            printf("removed file: %s\n", files[i]);
        }
        opt_v = 0;
    }
    if (opt_i == 0 && opt_v == 1 && opt_R == 0 && opt_r == 1)
    {
        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
            {
                Recursive_deletion(files[i]);
                printf("removed directory %s\n", files[i]);
            }
            else
            {
                if (remove(files[i]) != 0)
                {
                    printf("Error deleting the file %s\n", files[i]);
                    return;
                }
                printf("removed file: %s\n", files[i]);
            }
        }
        opt_v = 0;
        opt_r = 0;
    }
    if (opt_i == 0 && opt_v == 1 && opt_R == 1 && opt_r == 0)
    {
        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
            {
                Recursive_deletion(files[i]);
                printf("removed directory: %s\n", files[i]);
            }
            else
            {
                if (remove(files[i]) != 0)
                {
                    printf("Error deleting the file %s\n", files[i]);
                    return;
                }
                printf("removed file: %s\n", files[i]);
            }
        }
        opt_v = 0;
        opt_R = 0;
    }
    if (opt_i == 0 && opt_v == 1 && opt_R == 1 && opt_r == 1)
    {
        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
            {
                Recursive_deletion(files[i]);
                printf("removed directory: %s\n", files[i]);
            }
            else
            {
                if (remove(files[i]) != 0)
                {
                    printf("Error deleting the file %s\n", files[i]);
                    return;
                }
                printf("removed file: %s\n", files[i]);
            }
        }
        opt_v = 0;
        opt_R = 0;
        opt_r = 0;
    }
    if (opt_i == 0 && opt_v == 0 && opt_R == 0 && opt_r == 1)
    {
        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
                Recursive_deletion(files[i]);
            else if (remove(files[i]) != 0)
            {
                printf("Error deleting the file %s\n", files[i]);
                return;
            }
        }
        opt_r = 0;
    }
    if (opt_i == 0 && opt_v == 0 && opt_R == 1 && opt_r == 1)
    {
        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
                Recursive_deletion(files[i]);
            else if (remove(files[i]) != 0)
            {
                printf("Error deleting the file %s\n", files[i]);
                return;
            }
        }
        opt_R = 0;
        opt_r = 0;
    }
    if (opt_i == 0 && opt_v == 0 && opt_R == 1 && opt_r == 0)
    {
        for (int i = 0; files[i]; i++)
        {
            if (strcmp(files[i], "rm.c") == 0 || strcmp(files[i], "shell.c") == 0 || strcmp(files[i], "backup.c") == 0 || strcmp(files[i], "dir.c") == 0 || strcmp(files[i], "rm.h") == 0 || strcmp(files[i], "dir.h") == 0 || strcmp(files[i], "uniq.c") == 0 || strcmp(files[i], "uniq.h") == 0 || strcmp(files[i], "OS") == 0)
                continue;
            if (is_directory(files[i]) == 1)
                Recursive_deletion(files[i]);
            else if (remove(files[i]) != 0)
            {
                printf("Error deleting the file %s\n", files[i]);
                return;
            }
        }
        opt_R = 0;
    }
    for (int i = 0; i < filenumber; i++)
    {
        free(files[i]);
    }
        for (int i = 0; i < optionsnumber; i++)
    {
        free(options[i]);
    }
}
