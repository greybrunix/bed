#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NOR 0  /* Normal mode for issuing commands, deleting or changing
                  lines */
#define INS 1  /* Insert mode for writing a line to a file */
#define OUT 2  /* This mode is not really mode but a state to tell the
                  program to graciously terminate */


int
main(int argc,char**argv)
{
  u_int32_t LINS, COLS;
  u_int32_t curr_line;
  u_int32_t curr_col_max[128];
  u_int32_t curr_line_max;
  int flag_no_file, flag_err, mode, fd_file, i;
  char*normal;char**buffer;
  ssize_t bytes_read;


  /* Atributions */
  LINS = 128; COLS = 180;
  curr_line = 0; curr_line_max = 0;
  normal = malloc(64*sizeof(char));
  buffer = (char**) malloc(LINS*sizeof(char *));
  for (i = 0; i <= LINS; i++)
    buffer[i] = (char *) malloc(COLS*sizeof(char));
  flag_err = 0;
  flag_no_file = 1;

  /* Error Checking and checking for existing file */
  if (argc == 2)
  {
    fd_file = open(argv[1], O_CREAT|O_WRONLY, 777);
    if (fd_file < 0)
    {
      write(2,"?\n", 2);
      flag_no_file = 1;
    }
    else flag_no_file = 0;
  }
  if (argc > 2) flag_err = 1; /* set so it is only ed and file name */

  /* Code Logic */
  if (flag_err == 0)
  {
    mode = NOR;
    while (mode != OUT)
    {
      if (mode == NOR)
      {
        while (mode == NOR)
        {
          read(0, normal, 64);
          if (*normal == 'q') mode = OUT; /* exit command */
          else if (*normal == 'i') mode = INS;
          else if (*normal == '+')
          {
            if (curr_line < curr_line_max)
              curr_line++; /* if > curr_max */
            else write(1,"?\n",2);
          }
          else if (*normal == '-')
          {
            if (curr_line > 0)
              curr_line--; /* if < 0 */
            else write(1,"?\n",2);
          }
          else if (*normal == 'p')
          {
            if (buffer[curr_line][0] == '\0') write(1,"?\n",2);
            else write(1,buffer[curr_line],(int) COLS);
          }
          else if (*normal == 'w')
          {
            if (flag_no_file) write(1,"?\n",2);
            else for (i = 0;
                      i < curr_line_max;
                      write(fd_file, buffer[i], curr_col_max[i]),i++
                     );
          }
          else write(1, "?\n",2);

          *normal = '\0'; /* reset the normal buffer */
        }
        *normal = '\0'; /* reset the normal buffer */
      }
      if (mode == INS)
      {
        bytes_read = read(0,buffer[curr_line],(int) COLS);
        curr_col_max[curr_line] = bytes_read;
        if (curr_line == curr_line_max) curr_line_max++;
        mode = NOR;
      }

    }
  }
  free(normal);
  free(buffer);
  return flag_err;
}
