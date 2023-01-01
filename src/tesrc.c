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
#include "sregex.h"

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
  static u_int64_t LINS, COLS;
  auto u_int64_t curr_line;
  auto u_int64_t *curr_col_max;
  auto u_int64_t curr_line_max;
  auto u_int32_t flag_no_file, flag_err, mode, i;
  auto int32_t fd_file;
  auto char*normal, *file_name_buff, **buffer;
  auto ssize_t bytes_read,throw_out;
  auto ssize_t bytes_written;


  /* Atributions */
  LINS = 2<<12; COLS = 128; // One mebi of data
  curr_line = 0; curr_line_max = 0;
  bytes_written = 0;
  normal = malloc(COLS*sizeof(char));
  curr_col_max = (u_int64_t *) malloc(COLS* sizeof (u_int64_t));
  buffer = (char**) malloc(LINS*sizeof(char *));
  for (i = 0; (u_int64_t) i <= LINS; i++)
    buffer[i] = (char *) malloc(COLS*sizeof(char));
  throw_out = 0;
  flag_err = throw_out;
  flag_no_file = 1;
  mode = NOR;

  /* Error Checking and checking for existing file */
  if (argc == 2)
  {
    fd_file = open(argv[1], O_CREAT|O_RDWR, 0666);
    if (fd_file < 0)
    {
      throw_out = write(1,"?\n", 2);
      flag_no_file = 1;
    }
    else flag_no_file = 0;
  }
  if (argc > 2) flag_err = 1; /* set so it is only editor name
                                 and file name */

  /* Code Logic */
  while (!flag_err && mode != OUT)
  {
    while (mode == NOR)
    {
      throw_out = read(0, normal, COLS);
      switch(*normal)
      {
        case 'q': mode = OUT; goto NORMAL_SWITCH;/* exit command */
        case 'i': mode = INS; goto NORMAL_SWITCH;/* switch to insert mode */
        case 'a': mode = INS;
                  curr_line_max++;
                  curr_line++;
                  if (*(*buffer + curr_line-1) == '\0')
                      *(*buffer + curr_line-1) = '\n';
                  goto NORMAL_SWITCH;
        case '+': if (curr_line < curr_line_max)
                    curr_line++; /* if > curr_max */
                  else throw_out = write(1,"?\n",2);
                  goto NORMAL_SWITCH;
        case '-': if (curr_line > 0)
                    curr_line--; /* if < 0 */
                  else throw_out = write(1,"?\n",2);
                  goto NORMAL_SWITCH;
        case 'p': if (buffer[curr_line][0] == '\0')
                    throw_out = write(1,"?\n",2);
                  else throw_out = write(1,
                                         buffer[curr_line],
                                         curr_col_max[curr_line]);
                  goto NORMAL_SWITCH;
        case 'w': if (flag_no_file) throw_out = write(1,"?\n",2);
                  else for (i = 0; (u_int64_t) i <= curr_line_max; i++)
                  bytes_written += write(fd_file,
                                         *(buffer+i),
                                         *(curr_col_max+i));
                 /* the correct functioning of this code is dependant
                    on the correct opening and reading of file to buffer */
                  goto NORMAL_SWITCH;
        case 'e': if (*(normal + 2) == ' ') 
                    throw_out = write(1,"?\n",2);
                  else
                  {
                    file_name_buff = (char *) malloc((COLS-2)*sizeof (char));
                    format_file_name(file_name_buff,normal,(ssize_t) (COLS-2));
                    fd_file = open(file_name_buff, O_CREAT|O_RDWR, 0666);
                    if (fd_file < 0) throw_out = write(1,"?\n",2);
                    else flag_no_file = 0;
                    /* Requires passing the contents of the file
                       to the buffer */
                    free(file_name_buff);
                  }
                  goto NORMAL_SWITCH;
        default : throw_out = write(1, "?\n",2);
        NORMAL_SWITCH:
        *normal = '\0'; /* reset the normal buffer */
      }
    }
    if (mode == INS)
    {
      if (**(buffer + curr_line) != '\0') /* signalling edit/overwrite mode */
        for (i=0; (u_int64_t) i<curr_col_max[curr_line];i++)
             *(*(buffer + curr_line) + i) = '\0';
      bytes_read = read(0,buffer[curr_line], COLS);
      *(*(buffer + curr_line)+bytes_read) = '\0';
      curr_col_max[curr_line] = bytes_read;
      mode = NOR;
    }
  }
  free(normal);
  for (i=0; (u_int64_t) i<=LINS;free(buffer[i++]));
  free(buffer);
  free(curr_col_max);
  if (!flag_no_file)
  {
    bytes_written = lseek(fd_file, 0, SEEK_END);
    close(fd_file);
    fflush(stdin);
    printf("%ld\n",bytes_written);
  }
  return flag_err;
}
