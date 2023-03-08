#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
struct dir_list
{
    int number;
    struct dirent *start;
};

struct dir_list *dir_loader(const char *path)
{
    int capacity = 0;
    struct dir_list *list = calloc(1, sizeof(*list));

    DIR *directory = opendir(path);
    if (directory == NULL)
    {
        perror("Directory dosen't exist");
        return NULL;
    }

    struct dirent *read_pointer;

    while (read_pointer != NULL)
    {

        read_pointer = readdir(directory);
        if (read_pointer == NULL)
            break;

        if (read_pointer->d_name[0] == '.')
        {
            if (read_pointer->d_name[1] == 0)
                continue;
            if ((read_pointer->d_name[1] == '.') && (read_pointer->d_name[2] == 0))
                continue;
            if (strcmp(read_pointer->d_name, ".idea") == 0)
                continue;
            if (strcmp(read_pointer->d_name, "visualstudio") == 0)
                continue;
            if (strcmp(read_pointer->d_name, ".vscode") == 0)
                continue;
        }

        if (list->number >= capacity)
        {
            capacity += 10;
            list->start = realloc(list->start, sizeof(*list->start) * capacity);
            if (list->start == NULL)
            {
                perror("Reallocation was not successfull");
                exit(2);
            }
        }

        list->start[list->number++] = *read_pointer;
    }

    closedir(directory);

    list->start = realloc(list->start, sizeof(*list->start) * list->number);
    if (list->start == NULL)
    {
        perror("Reallocation was not sucessfull");
        exit(2);
    }

    return list;
}

int dir_comparison(const void *l, const void *r)
{
    const struct dirent *lhs = l;
    const struct dirent *rhs = r;
    int comparator;

    comparator = strcmp(lhs->d_name, rhs->d_name);

    return comparator;
}

void sort_dir(struct dir_list *d)
{

    if (d->number > 0)
        qsort(d->start, d->number, sizeof(*d->start), dir_comparison);
}

void print_dir(const struct dir_list *d)
{
    const struct dirent *p = d->start;
    for (int i = 0; i < d->number; ++i, ++p)
    {

        printf("%s ", p->d_name);
    }
    printf("\n");
}

void free_dir(struct dir_list *d)
{

    if (d != NULL)
        free(d->start);
    free(d);
}
void dir_help()
{
    printf("Usage: dir...[FILE]...\n");
    printf("List information about the FILEs(the current directory by default).\n");
    printf("Sort entries alphabetically\n");
    printf("No option of the original dir is implemented\n");
    return;
}

void own_Dir_command(int argc, char *argv[])
{
    const char *dir;
    if (argc > 1)
        dir = argv[1];
    else
        dir = ".";

    struct dir_list *list = dir_loader(dir);
    if (list == NULL)
        return;
    sort_dir(list);
    print_dir(list);
    if (list->start != NULL)
        free(list->start);
    free(list);
    return;
}
