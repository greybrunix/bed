/*
   A modal line editor based on ed for the personal use of the creator
   and for the pedagogic purpose of creating a vi like text editor in 
   the future.
   Started development on 2022/12/28.
   Bruno Giao 2022/2023
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define NOR 0  /* Normal mode for issuing commands, deleting or changing
                  lines */
#define INS 1  /* Insert mode for writing a line to a file */
#define OUT 2  /* This mode is not really mode but a state to tell the
                  program to graciously terminate */

void
format_file_name(char*dest,
                 const char*src,
                 ssize_t size_dest)
{
  int i;

  strncpy(dest,src+2,size_dest); /* remove first two chars from src */
  for (i = 0; (ssize_t) i < size_dest && *(dest+i)!='\n'; i++); 
  *(dest+i) = '\0';
  return;
}

int
main(int argc,char**argv)
{
  /* argc = 1 || argc = 2 */
  static u_int32_t LINS, COLS;
  auto u_int32_t curr_line;
  auto u_int32_t curr_col_max[128];
  auto u_int32_t curr_line_max;
  auto int flag_no_file, flag_err, mode, fd_file, i;
  auto char*normal, **buffer, *file_name_buff;
  auto ssize_t bytes_read;


  /* Atributions */
  LINS = 128; COLS = 120;
  curr_line = 0; curr_line_max = 0;
  normal = malloc(64*sizeof(char));
  buffer = (char**) malloc(LINS*sizeof(char *));
  for (i = 0; (u_int32_t) i <= LINS; i++)
    buffer[i] = (char *) malloc(COLS*sizeof(char));
  flag_err = 0;
  flag_no_file = 1;

  /* Error Checking and checking for existing file */
  if (argc == 2)
  {
    fd_file = open(argv[1], O_CREAT|O_RDWR, 777);
    if (fd_file < 0)
    {
      write(2,"?\n", 2);
      flag_no_file = 1;
    }
    else flag_no_file = 0;
  }
  if (argc > 2) flag_err = 1; /* set so it is only editor name
                                 and file name */

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
          read(0, normal, COLS);
          switch(*normal)
          {
            case 'q': mode = OUT; break;/* exit command */
            case 'i': mode = INS; break;/* switch to insert mode */
            case 'a': mode = INS; 
                      curr_line_max++; 
                      curr_line++;
                      if (*(*buffer + curr_line-1) == '\0')
                        *(*buffer + curr_line-1) = '\n';
                      break;
            case '+': if (curr_line < curr_line_max)
                        curr_line++; /* if > curr_max */
                      else write(1,"?\n",2);
                      break;
            case '-': if (curr_line > 0)
                        curr_line--; /* if < 0 */
                      else write(1,"?\n",2);
                      break; 
            case 'p': if (buffer[curr_line][0] == '\0') write(1,"?\n",2);
                      else write(1,buffer[curr_line], curr_col_max[curr_line]);
                      break;
            case 'w': if (flag_no_file) write(1,"?\n",2);
                      else for (i = 0;
                                (u_int32_t) i <= curr_line_max;
                                write(fd_file, buffer[i], curr_col_max[i]),
                                i++
                                ); /* the correct functioning of this 
                                      code is dependant
                                      on the correct opening and reading 
                                      of file to buffer */
                      break;
            case 'e': file_name_buff = (char *) malloc((COLS-2)*sizeof (char));
                      format_file_name(file_name_buff,normal,(ssize_t) (COLS-2));
                      fd_file = open(file_name_buff, O_CREAT|O_RDWR, 777); 
                      if (fd_file < 0) write(1,"?\n",2);
                      else flag_no_file = 0;
                      /* Requires passing the contents of the file
                         to the buffer */
                      free(file_name_buff);
                      break;
            default: write(1, "?\n",2);
          }
        *normal = '\0'; /* reset the normal buffer */
        }
      }
      if (mode == INS)
      {
        if (**(buffer + curr_line) != '\0')
        for (i=0; (u_int32_t) i<curr_col_max[curr_line];i++)
        *(*(buffer + curr_line) + i) = '\0';
        bytes_read = read(0,buffer[curr_line],(int) COLS);
        *(*(buffer + curr_line)+bytes_read) = '\0';
        /*printf("%ld\n", bytes_read);
        fflush(stdin);*/
        curr_col_max[curr_line] = bytes_read;
        mode = NOR;
      }

    }
  }
  free(normal);
  for (i=0; (u_int32_t) i<LINS;free(buffer[i++]));
  free(buffer);
  return flag_err;
}
