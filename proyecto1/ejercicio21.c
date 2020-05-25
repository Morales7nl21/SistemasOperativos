#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void create_process_child();
char *get_command();
int call_proc_from_string(char *proc);
int get_arg_count(char *proc);

int main(int argc, const char **argv)
{
  create_process_child();
  return 0;
}

void create_process_child()
{
  int pid, status, isvalid = 1;
  while (isvalid != 0)
  {
    char *command = get_command();
    pid = fork();
    if (pid == -1)
    {
      printf("Error en el fork");
      exit(1);
    }
    else
    {
      if (pid == 0)
      {
        exit(call_proc_from_string(command));
      }
      else
      {
        pid = wait(&status);
        isvalid = strcmp(command, "exit");
      }
    }
  }
}

int call_proc_from_string(char *command)
{
  int count = 1;
  char *args[get_arg_count(command) + 1];
  char *token = strtok(command, " ");
  args[0]  = "./comando.out";
  while (token != NULL)
  {
    args[count] = token;
    count += 1;
    token = strtok(NULL, " ");
  }
  args[count] = (char *)0;
  int res = execvp(args[0], args);
  return res;
}

int get_arg_count(char *proc)
{
  int count = 0;
  for (int i = 0; i < strlen(proc); ++i)
  {
    if (proc[i] == ' ')
    {
      ++count;
    }
  }
  return count;
}

char *get_command()
{
  char *command = malloc(100 * sizeof(char));
  printf("Introduce el comando>\t");
  scanf("%[^\n]", command);
  fflush(stdin);
  return command;
}
