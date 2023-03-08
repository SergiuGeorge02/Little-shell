#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include "rm.h"
#include "uniq.h"
#include "dir.h"
#define MAX_DIRECTORY 1024
#define MAX_READLINE 10000
void clearshell()
{
    // function to clean my shell before the user uses it.
    printf("\033[H\033[J");
}

void getcurrentdirectory()
{
    // This function will print the current working directory as the normal shell does
    char current_working_directory[MAX_DIRECTORY];
    getcwd(current_working_directory, sizeof(char) * MAX_DIRECTORY);
    printf("\n%s", current_working_directory);
}

void get_line_arguments(char *readed_line, int *argc, char *argv[])
{
    char copystring[MAX_READLINE];
    strcpy(copystring, readed_line);

    char *pipeposition = strchr(copystring, '|');
    while (pipeposition)
    {
        if (pipeposition > copystring && *(pipeposition - 1) != ' ')
        {
            memmove(pipeposition + 1, pipeposition, strlen(pipeposition) + 1);
            *pipeposition = ' ';
        }
        if (*(pipeposition + 1) != '\0' && *(pipeposition + 1) != ' ')
        {
            int length = strlen(pipeposition);
            memmove(pipeposition + 2, pipeposition + 1, length);
            *(pipeposition + 1) = ' ';
            pipeposition[length + 1] = '\0';
        }

        pipeposition = strchr(pipeposition + 1, '|');
    }
    char *redirect = strchr(copystring, '>');
    while (redirect)
    {
        if (redirect > copystring && *(redirect - 1) != ' ')
        {
            memmove(redirect + 1, redirect, strlen(redirect) + 1);
            *redirect = ' ';
        }
        if (*(redirect + 1) != '\0' && *(redirect + 1) != ' ')
        {
            int length = strlen(redirect);
            memmove(redirect + 2, redirect + 1, length);
            *(redirect + 1) = ' ';
            redirect[length + 1] = '\0';
        }

        redirect = strchr(redirect + 1, '>');
    }

    char *words = strtok(copystring, " ");
    int n = 0;
    while (words)
    {
        argv[n] = malloc(strlen(words) + 1);
        strcpy(argv[n], words);
        words = strtok(NULL, " ");
        n++;
    }
    *argc = n;
    argv[n] = NULL;
}
void checking_pipes(char *argv[], int *n, int pipesposition[1000])
{
    int i, j;
    for (i = 0, j = 0; argv[i]; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            pipesposition[j] = i;
            j++;
        }
    }
    *n = j;
}
int greater_redirection(char *argv[], int index)
{

    int i = index + 1;
    if (strcmp(argv[i], ">") == 0)
        return 0;
    return 1;
}

int checking_redirection(char *argv[])
{
    int k = -1;
    int i = 0;
    while (argv[i] != NULL)
    {
        if (strcmp(argv[i], ">") == 0)
        {
            k = i;
            break;
        }
        i++;
    }

    return k;
}
int check_single_pipe_operator(char *argv[])
{
    int k = -1;
    int i = 0;
    while (argv[i] != NULL)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            k = i;
            break;
        }
        i++;
    }

    return k;
}

void getpipes(int start, char *arguments[], char *arguments1[])
{
    int i = 0;
    for (int j = start; arguments[j] != NULL && strcmp(arguments[j], "|") != 0; j++)
    {
        arguments1[i] = strdup(arguments[j]);
        i++;
    }
    arguments1[i] = NULL;
}
void command_execution(char *arguments[])
{
    pid_t id = fork();

    if (id < 0)
    {
        perror("Error creating the process\n");
        exit(2);
    }
    else if (id == 0)

    {
        if (execvp(arguments[0], arguments) < 0)
        {
            printf("No such command was found\n");
            exit(4);
        }
    }
    else
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            int statuscode = 0;
            statuscode = WEXITSTATUS(status);
            if (statuscode != 0)
            {
                printf("Command could not be executed, exit code %d \n", statuscode);
            }
        }
        return;
    }
}
void pipe_handling_execution(char *arguments[], int n, int pipe_position[])
{

    char *array[200][300];
    getpipes(0, arguments, array[0]);
    int i = 0;
    while (i < n)
    {
        getpipes(pipe_position[i] + 1, arguments, array[i + 1]);
        i++;
    }
    int commands = n + 1;
    int pipes = n;
    int fd[commands][2];

    for (i = 0; i < pipes; i++)
        if (pipe(fd[i]) == -1)
        {
            printf("Error creating the pipe pipe\n");
            exit(2);
        }

    pid_t id = fork();
    if (id < 0)
    {
        perror("Error creating the process\n");
        exit(3);
    }
    else if (id == 0)
    {
        close(fd[0][0]);
        dup2(fd[0][1], STDOUT_FILENO);

        if (execvp(array[0][0], array[0]) < 0)

        {
            printf("No such command was found\n");
            exit(4);
        }
    }
    else
    {
        close(fd[0][1]);
        for (i = 1; i < commands; i++)
        {
            pid_t id1 = fork();
            if (id1 < 0)
            {
                perror("Error creating the process\n");
                exit(3);
            }
            else if (id1 == 0)
            {
                close(fd[i][0]);
                dup2(fd[i - 1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);

                if (execvp(array[i][0], array[i]) < 0)
                {
                    printf("No such command was found\n");
                    exit(4);
                }
            }
            else
            {
                int status1;
                wait(&status1);
                if (WIFEXITED(status1))
                {
                    int statuscode = 0;
                    statuscode = WEXITSTATUS(status1);
                    if (statuscode != 0)
                    {
                        printf("Error executing the command, exit code: %d \n", statuscode);
                    }
                }
                close(fd[i][1]);
            }
        }
        int status2;
        wait(&status2);
        if (WIFEXITED(status2))
        {
            int statuscode = 0;
            statuscode = WEXITSTATUS(status2);
            if (statuscode != 0)
            {
                printf("Error executing the command, exit code: %d \n", statuscode);
            }
        }
    }
    return;
}

void own_CD_command(char *arguments[], int n)
{
    if (n < 2)
    {
        printf("\n");
    }
    else
    {
        chdir(arguments[1]);
    }
    return;
}
void own_Help_command()
{
    printf("\n                 ===Help menu=== \n\n");
    printf("This shell supports all command from linux shell\n");
    printf("Besides that it also suports the commands implemented by me\n");
    printf("rm command. with parameters: -i, -r (-R), -v\n");
    printf("uniq command. with parameters: -i, -d, -u\n");
    printf("dir command. with no parameters\n");
    printf("vs,version,VS,VERSION, Version command is also supported and it will print the version of my shell\n");
    printf("The shell also supports pipe handling for all commands\n");

    return;
}
void own_Exit_command()
{
    clearshell();
    printf("Thank you for using my shell\n");
    printf("         Goodbye!\n");
    exit(1);
}
void own_Version_command()
{
    clearshell();
    printf("  Shell version command\n\n");
    printf("==Sergiu's terminal version 1.0=== \n");
    return;
}
void delete_redirection(char *argv[], int number)
{
    int index = checking_redirection(argv);
    if (index >= 0 && strcmp(argv[index], ">") == 0)
    {
        int i;
        for (i = index; argv[i]; i++)
            argv[i] = argv[i + 2];
        number -= 2;
    }
}
void command_execution_and_greater_redirection(char *arguments[], char file[1024])
{
    pid_t id = fork();

    if (id < 0)
    {
        perror("Error creating the process\n");
        exit(2);
    }
    else if (id == 0)
    {
        freopen(file, "w", stdout);
        if (execvp(arguments[0], arguments) < 0)
        {
            printf("Error executing command\n");
            exit(4);
        }
    }
    else
    {
        int status;
        waitpid(id, &status, 0);
        if (WIFEXITED(status))
        {
            int statuscode = WEXITSTATUS(status);
            if (statuscode != 0)
            {
                printf("Error executing the command, exit code: %d \n", statuscode);
            }
        }
    }
}
void delete_redirection_greater_greater(char *argv[], int number)
{
    int index = checking_redirection(argv);
    index++;
    if (index >= 0 && strcmp(argv[index], ">") == 0)
    {
        int i;
        for (i = index; argv[i]; i++)
            argv[i] = argv[i + 2];
        number -= 2;
    }
}
void pipe_command_execution_greater_redirection(char *arguments[], char file[1024])
{
    int index = check_single_pipe_operator(arguments);

    int i = 0;
    while (arguments[i])
    {
        char *argument = strtok(arguments[i], "|");
        arguments[i] = argument;
        i++;
    }
    int fd[2];
    if (pipe(fd) < 0)
    {
        printf("Error creating the pipe\n");
        exit(4);
    }

    pid_t id = fork();
    if (id < 0)
    {
        perror("Error creating the process\n");
        exit(3);
    }
    else if (id == 0)
    {

        close(fd[0]);
        dup2(fd[1], 1);

        if (execvp(arguments[0], arguments) < 0)
        {
            printf("Error executing the command\n");
            exit(5);
        }
    }
    else
    {

        close(fd[1]);

        pid_t id2 = fork();
        if (id2 < 0)
        {
            perror("Error creating the process\n");
            exit(3);
        }
        else if (id2 == 0)
        {

            int fd1 = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            dup2(fd[0], 0);
            dup2(fd1, 1);

            if (execvp(arguments[index + 1], &arguments[index + 1]) < 0)
            {
                printf("Error executing the command\n");
                exit(5);
            }
        }
        close(fd[0]);

        wait(NULL);
        wait(NULL);
    }
}
void handle_redirection_symbol_greater(char *arguments[], int number, int index)
{
    if (index == number - 1)
    {
        printf("Error: no file specified for redirection\n");
        return;
    }
    if (arguments[index + 1] == NULL)
    {
        printf("Error: no file specified for redirection\n");
        return;
    }

    char file[1024];
    strcpy(file, arguments[index + 1]);
    delete_redirection(arguments, number);
    number = number - 1;
    int filepos = 0;
    int pipes = 0;
    int pipespositions[1000] = {0};
    checking_pipes(arguments, &pipes, pipespositions);
    if (pipes == 0)
    {
        command_execution_and_greater_redirection(arguments, file);
    }
    else
    {
        pipe_command_execution_greater_redirection(arguments, file);
    }
}
void command_execution_and_greater_greater_redirection(char *arguments[], char file[1024])
{
    pid_t id = fork();

    if (id < 0)
    {
        perror("Error creating the process\n");
        exit(2);
    }
    else if (id == 0)
    {
        fprintf(stdout, "\n");
        freopen(file, "a", stdout);
        if (execvp(arguments[0], arguments) < 0)
        {
            printf("Error executing command\n");
            exit(4);
        }
    }
    else
    {
        int status;
        waitpid(id, &status, 0);
        if (WIFEXITED(status))
        {
            int statuscode = WEXITSTATUS(status);
            if (statuscode != 0)
            {
                printf("Error executing the command, exit code: %d \n", statuscode);
            }
        }
    }
}
void pipe_command_execution_greater_greater_redirection(char *arguments[], char file[1024])
{
    int index = check_single_pipe_operator(arguments);

    int i = 0;
    while (arguments[i])
    {
        char *argument = strtok(arguments[i], "|");
        arguments[i] = argument;
        i++;
    }
    int fd[2];
    if (pipe(fd) < 0)
    {
        printf("Error creating the pipe\n");
        exit(4);
    }

    pid_t id = fork();
    if (id < 0)
    {
        perror("Error creating the process\n");
        exit(3);
    }
    else if (id == 0)
    {

        close(fd[0]);
        dup2(fd[1], 1);

        if (execvp(arguments[0], arguments) < 0)
        {
            printf("Error executing the command\n");
            exit(5);
        }
    }
    else
    {

        close(fd[1]);

        pid_t id2 = fork();
        if (id2 < 0)
        {
            perror("Error creating the process\n");
            exit(3);
        }
        else if (id2 == 0)
        {

            int fd1 = open(file, O_WRONLY | O_APPEND | O_CREAT, 0644);

            dup2(fd[0], 0);
            dup2(fd1, 1);

            if (execvp(arguments[index + 1], &arguments[index + 1]) < 0)
            {
                printf("Error executing the command\n");
                exit(5);
            }
        }
        close(fd[0]);

        wait(NULL);
        wait(NULL);
    }
}
void handle_redirection_symbol_greater_greater(char *arguments[], int number, int index)
{
    int i = index;
    if (i == number - 1)
    {
        printf("Error: no file specified for redirection\n");
        return;
    }
    if (arguments[i + 1] == NULL)
    {
        printf("Error: no file specified for redirection\n");
        return;
    }

    char file[1024];
    strcpy(file, arguments[i + 2]);
    delete_redirection(arguments, number);
    number = number - 2;
    int filepos = 0;
    for (int i = 0; arguments[i]; i++)
    {
        if (strcmp(arguments[i], file) == 0)
        {
            filepos = i;
            break;
        }
    }
    arguments[filepos] = NULL;
    number = number - 1;
    int pipes = 0;
    int pipespositions[1000] = {0};
    checking_pipes(arguments, &pipes, pipespositions);
    if (pipes == 0)
    {
        command_execution_and_greater_greater_redirection(arguments, file);
    }
    else
    {
        pipe_command_execution_greater_greater_redirection(arguments, file);
    }
}

int main()
{
    clearshell();
    while (1)
    {
        getcurrentdirectory();
        char *read_line = readline(">$ ");

        char *arguments[1024];
        int number_of_arguments = 0;
        get_line_arguments(read_line, &number_of_arguments, arguments);
        int pipes = 0;
        int pipespositions[1000] = {0};
        int index = checking_redirection(arguments);
        add_history(read_line);
        if (index == -1)
        {
            checking_pipes(arguments, &pipes, pipespositions);

            if (pipes == 0)
            {
                if (strcmp(arguments[0], "dir") == 0)
                    own_Dir_command(number_of_arguments, arguments);
                else if (strcmp(arguments[0], "rm") == 0)
                    own_RM_command(arguments, number_of_arguments);
                else if (strcmp(arguments[0], "uniq") == 0)
                    own_UNIQ_command(arguments, number_of_arguments);
                else if (strcmp(arguments[0], "help") == 0)
                    own_Help_command();
                else if (strcmp(arguments[0], "cd") == 0)
                {
                    own_CD_command(arguments, number_of_arguments);
                }
                else if (strcmp(arguments[0], "vs") == 0 || strcmp(arguments[0], "version") == 0 || strcmp(arguments[0], "VS") == 0 || strcmp(arguments[0], "Vs") == 0 || strcmp(arguments[0], "Version") == 0 || strcmp(arguments[0], "VERSION") == 0)
                {
                    own_Version_command();
                }

                else if (strcmp(arguments[0], "exit") != 0)
                {
                    command_execution(arguments);
                }
                else if (strcmp(arguments[0], "exit") == 0)
                {
                    own_Exit_command();
                }
            }
            else
            {
                pipe_handling_execution(arguments, pipes, pipespositions);
            }
        }
        else
        {
            if (greater_redirection(arguments, index) == 1)
            {
                handle_redirection_symbol_greater(arguments, number_of_arguments, index);
            }
            else
            {
                handle_redirection_symbol_greater_greater(arguments, number_of_arguments, index);
            }
        }

        free(read_line);
    }
    return 0;
}