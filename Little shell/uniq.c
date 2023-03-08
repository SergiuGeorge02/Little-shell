#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include "uniq.h"
#include <readline/readline.h>
#include <readline/history.h>
void help_uniq()
{
    printf("USAGE: uniq [OPTION]... [INPUT[OUTPUT]]\n");
    printf("Filter adjacent matching line from INPUT (or standard input),\n");
    printf("writting to OUTPUT (or standard output).\n");
    printf("Options available: \n");
    printf("-i      ignore differences in case when comparing\n");
    printf("-d      only print duplicate line, one for each group\n");
    printf("-u      only print unique lines\n");
}
int cmp(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}
int is_File(const char *filename)
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

void own_UNIQ_command(char *arguments[], int n)
{
    int opt_i = 0;
    int opt_d = 0;
    int opt_u = 0;
    if (n == 1)
    {

        char line[1000];
        char prev_line[1000];
        while (fgets(line, 1000, stdin) != NULL)
        {
            if (strcmp(line, prev_line) != 0)
            {
                printf("%s", line);
                strcpy(prev_line, line);
            }
        }
    }
    else
    {

        if (strcmp(arguments[1], "--help") == 0)
        {
            help_uniq();
            return;
        }
        char *files[1000];
        int filenumber = 0;
        char *options[1000];
        int optionsnumber = 0;
        int i = 1;
        int flag = 0;
        while (i < n)
        {
            if (strcmp(arguments[i], "-i") == 0 || strcmp(arguments[i], "-u") == 0 || strcmp(arguments[i], "-d") == 0)
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

            flag = 1;

        if (filenumber > 2)
        {
            printf("uniq: extra operand %s\n", files[2]);
            printf("Try uniq --help for more info\n");
            return;
        }

        if (optionsnumber != 0)
        {
            for (int i = 0; options[i]; i++)
            {
                if (strcmp(options[i], "-i") == 0)
                    opt_i = 1;
                if (strcmp(options[i], "-d") == 0)
                    opt_d = 1;
                if (strcmp(options[i], "-u") == 0)
                    opt_u = 1;
            }
        }
        if (flag == 1)
        {
            if (opt_i == 1 && opt_d == 0 && opt_u == 0)
            {
                char buf[1024];
                char lastline[1024] = "";
                int equal;

                while (fgets(buf, 1024, stdin))
                {
                    equal = (strcasecmp(buf, lastline) == 0);
                    if (!equal)
                    {
                        printf("%s", buf);
                    }
                    strcpy(lastline, buf);
                }
            }
            if (opt_i == 1 && opt_d == 1 && opt_u == 0)
            {
                char *line = NULL;
                size_t size = 0;
                char *prevline1 = " ";
                char *prevline2 = " ";
                while (getline(&line, &size, stdin) != -1)
                {

                    if (line[strlen(line) - 1] == '\n')
                    {
                        line[strlen(line) - 1] = '\0';
                    }
                    if (strcmp(line, prevline1) == 0)
                    {
                        prevline2 = malloc(sizeof(char) * (strlen(line) - 1));
                        strcpy(prevline2, line);
                    }
                    prevline1 = malloc(sizeof(char) * (strlen(line) - 1));
                    strcpy(prevline1, line);

                    if (strcmp(prevline1, prevline2) != 0 && strcmp(prevline1, " ") != 0 && strcmp(prevline2, " ") != 0)
                    {
                        printf("%s\n", prevline2);
                        prevline2 = malloc(sizeof(char) * (strlen(line) - 1));
                        strcpy(prevline2, line);
                    }
                }
                free(prevline1);
                free(prevline2);
                free(line);
            }
            if (opt_i == 1 && opt_d == 1 && opt_u == 1)
            {

                char *line = NULL;
                size_t len = 0;
                int ok = 0;
                while (1)
                {
                    while (getline(&line, &len, stdin) != -1)
                    {
                    }
                }
                free(line);
            }
            if (opt_i == 0 && opt_d == 1 && opt_u == 0)
            {
                char *line = NULL;
                size_t size = 0;
                char *prevline1 = " ";
                char *prevline2 = " ";
                while (getline(&line, &size, stdin) != -1)
                {

                    if (line[strlen(line) - 1] == '\n')
                    {
                        line[strlen(line) - 1] = '\0';
                    }
                    if (strcmp(line, prevline1) == 0)
                    {
                        prevline2 = malloc(sizeof(char) * (strlen(line) - 1));
                        strcpy(prevline2, line);
                    }
                    prevline1 = malloc(sizeof(char) * (strlen(line) - 1));
                    strcpy(prevline1, line);

                    if (strcmp(prevline1, prevline2) != 0 && strcmp(prevline1, " ") != 0 && strcmp(prevline2, " ") != 0)
                    {
                        printf("%s\n", prevline2);
                        prevline2 = malloc(sizeof(char) * (strlen(line) - 1));
                        strcpy(prevline2, line);
                    }
                }
                free(prevline1);
                free(prevline2);
                free(line);
            }
            if (opt_i == 0 && opt_d == 1 && opt_u == 1)
            {

                char *line = NULL;
                size_t len = 0;
                int ok = 0;
                while (1)
                {
                    while (getline(&line, &len, stdin) != -1)
                    {
                    }
                }
                free(line);
            }
            if (opt_i == 0 && opt_d == 0 && opt_u == 1)
            {

                char *line = NULL;
                size_t len = 0;
                int ok = 0;
                while (1)
                {
                    while (getline(&line, &len, stdin) != -1)
                    {
                    }
                }
                free(line);
            }
        }
        if (flag == 0)
        {
            if (optionsnumber == 0 && filenumber == 1)
            {
                char prev_line[1000] = "";
                char curr_line[1000];
                int is_repeated;
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                while (fgets(curr_line, 1000, fp))
                {
                    is_repeated = strcmp(prev_line, curr_line) == 0;
                    if (!is_repeated)
                    {
                        printf("%s", curr_line);
                        strcpy(prev_line, curr_line);
                    }
                }
                fclose(fp);
            }
            if (optionsnumber == 0 && filenumber == 2)
            {
                char prev_line[1000] = "";
                char curr_line[1000];
                int is_repeated;
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                FILE *output_fp = fopen(files[1], "w");
                if (output_fp == NULL)
                {
                    printf("Could not open output file\n");
                    return;
                }

                while (fgets(curr_line, 1000, fp))
                {
                    is_repeated = strcmp(prev_line, curr_line) == 0;
                    if (!is_repeated)
                    {
                        fprintf(output_fp, "%s", curr_line);
                        strcpy(prev_line, curr_line);
                    }
                }
                fclose(fp);
                fclose(output_fp);
            }
            if (filenumber == 1 && opt_i == 1 && opt_u == 0 && opt_d == 0)
            {
                char prev_line[1000] = "";
                char curr_line[1000];
                int is_repeated;
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                while (fgets(curr_line, 1000, fp))
                {
                    is_repeated = strcmp(prev_line, curr_line) == 0;
                    if (!is_repeated)
                    {
                        printf("%s", curr_line);
                        strcpy(prev_line, curr_line);
                    }
                }
                fclose(fp);
            }
            if (filenumber == 1 && opt_i == 0 && opt_u == 0 && opt_d == 1)
            {
                char buf[2000];
                char *last = NULL;
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                while (fgets(buf, 2000, fp) != NULL)
                {
                    if (last == NULL || strcmp(buf, last) != 0)
                    {
                        printf("%s", buf);
                        last = buf;
                    }
                }
                fclose(fp);
            }
            if (filenumber == 1 && opt_i == 0 && opt_u == 1 && opt_d == 0)
            {
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                char *lines[2000];
                int n = 0;
                char line[BUFSIZ];
                while (fgets(line, sizeof line, fp))
                {
                    int len = strlen(line);
                    if (line[len - 1] == '\n')
                    {
                        line[len - 1] = '\0';
                    }
                    lines[n++] = strdup(line);
                }
                fclose(fp);
                qsort(lines, n, sizeof(char *), cmp);
                for (int i = 0; i < n; i++)
                {
                    if (i == 0 || strcmp(lines[i], lines[i - 1]) != 0)
                    {
                        printf("%s\n", lines[i]);
                    }
                }
            }
            if (filenumber == 1 && opt_i == 1 && opt_u == 1 && opt_d == 0)
            {
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                char *lines[2000];
                int n = 0;
                char line[BUFSIZ];
                while (fgets(line, sizeof line, fp))
                {
                    int len = strlen(line);
                    if (line[len - 1] == '\n')
                    {
                        line[len - 1] = '\0';
                    }
                    lines[n++] = strdup(line);
                }
                fclose(fp);
                qsort(lines, n, sizeof(char *), cmp);
                for (int i = 0; i < n; i++)
                {
                    if (i == 0 || strcmp(lines[i], lines[i - 1]) != 0)
                    {
                        printf("%s\n", lines[i]);
                    }
                }
            }
            if (filenumber == 1 && opt_i == 1 && opt_u == 0 && opt_d == 1)
            {
                char buf[2000];
                char *last = NULL;
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                while (fgets(buf, 2000, fp) != NULL)
                {
                    if (last == NULL || strcmp(buf, last) != 0)
                    {
                        printf("%s", buf);
                        last = buf;
                    }
                }
                fclose(fp);
            }
            if (filenumber == 1 && opt_i == 0 && opt_u == 1 && opt_d == 1)
            {
                if (is_File(files[0]) == 0)
                    printf("The file %s does not exists\n", files[0]);
                return;
            }
            if (filenumber == 1 && opt_i == 1 && opt_u == 1 && opt_d == 1)
            {
                if (is_File(files[0]) == 0)
                    printf("The file %s does not exists\n", files[0]);
                return;
            }
            if (filenumber == 2 && opt_i == 1 && opt_u == 0 && opt_d == 0)
            {
                char prev_line[1000] = "";
                char curr_line[1000];
                int is_repeated;
                FILE *fp = fopen(files[0], "r");
                FILE *output_file = fopen(files[1], "w");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                while (fgets(curr_line, 1000, fp))
                {
                    is_repeated = strcmp(prev_line, curr_line) == 0;
                    if (!is_repeated)
                    {
                        fprintf(output_file, "%s", curr_line);
                        strcpy(prev_line, curr_line);
                    }
                }
                fclose(fp);
                fclose(output_file);
            }
            if (filenumber == 2 && opt_i == 1 && opt_u == 1 && opt_d == 0)
            {
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                char *lines[2000];
                int n = 0;
                char line[BUFSIZ];
                while (fgets(line, sizeof line, fp))
                {
                    int len = strlen(line);
                    if (line[len - 1] == '\n')
                    {
                        line[len - 1] = '\0';
                    }
                    lines[n++] = strdup(line);
                }
                fclose(fp);
                qsort(lines, n, sizeof(char *), cmp);

                FILE *output_fp = fopen(files[1], "w");
                for (int i = 0; i < n; i++)
                {
                    if (i == 0 || strcmp(lines[i], lines[i - 1]) != 0)
                    {
                        fprintf(output_fp, "%s\n", lines[i]);
                    }
                }
                fclose(output_fp);
            }
            if (filenumber == 2 && opt_i == 1 && opt_u == 1 && opt_d == 1)
            {
                if (is_File(files[0]) == 0)
                    printf("The file %s does not exists\n", files[0]);
                return;
            }
            if (filenumber == 2 && opt_i == 0 && opt_u == 1 && opt_d == 0)
            {
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                char *lines[2000];
                int n = 0;
                char line[BUFSIZ];
                while (fgets(line, sizeof line, fp))
                {
                    int len = strlen(line);
                    if (line[len - 1] == '\n')
                    {
                        line[len - 1] = '\0';
                    }
                    lines[n++] = strdup(line);
                }
                fclose(fp);
                qsort(lines, n, sizeof(char *), cmp);

                FILE *output_fp = fopen(files[1], "w");
                for (int i = 0; i < n; i++)
                {
                    if (i == 0 || strcmp(lines[i], lines[i - 1]) != 0)
                    {
                        fprintf(output_fp, "%s\n", lines[i]);
                    }
                }
                fclose(output_fp);
            }
            if (filenumber == 2 && opt_i == 0 && opt_u == 1 && opt_d == 1)
            {
                if (is_File(files[0]) == 0)
                    printf("The file %s does not exists\n", files[0]);
                return;
            }
            if (filenumber == 2 && opt_i == 0 && opt_u == 0 && opt_d == 1)
            {
                char buf[2000];
                char *last = NULL;
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                FILE *output_fp = fopen(files[1], "w");
                while (fgets(buf, 2000, fp) != NULL)
                {
                    if (last == NULL || strcmp(buf, last) != 0)
                    {
                        fprintf(output_fp, "%s", buf);
                        last = buf;
                    }
                }
                fclose(fp);
                fclose(output_fp);
            }
            if (filenumber == 2 && opt_i == 1 && opt_u == 0 && opt_d == 1)
            {
                char buf[2000];
                char *last = NULL;
                FILE *fp = fopen(files[0], "r");
                if (fp == NULL)
                {
                    printf("uniq: %s: No such file or directory\n", files[0]);
                    return;
                }
                FILE *output_fp = fopen(files[1], "w");
                while (fgets(buf, 2000, fp) != NULL)
                {
                    if (last == NULL || strcmp(buf, last) != 0)
                    {
                        fprintf(output_fp, "%s", buf);
                        last = buf;
                    }
                }
                fclose(fp);
                fclose(output_fp);
            }
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
}
