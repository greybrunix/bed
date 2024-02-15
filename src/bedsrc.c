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
#include "llist.h"
#include "redef.h"
#define NOR 0  /* Normal mode for issuing commands, deleting or changing
                  lines */
#define INS 1  /* Insert mode for writing a line to a file */
#define OUT 2  /* State to tell the
                  program to graciously terminate */
#define GRA 0
#define ERR 1
const U64 COLS = 128;
const U64 LINS = 2<<12;


void
format_file_name(char*dest,
                 const char*src,
                 ssize_t size_dest)
{ int i;

  strncpy(dest,src+2,size_dest); /* remove first two chars from src */
  for (i = 0; (ssize_t) i < size_dest && *(dest+i)!='\n'; i++);
  *(dest+i) = '\0';
  return;
}

void
cleanup(LLIST HEAD)
{ free(HEAD.CONTENT);
  return;
}

I32
main(I32 argc,CHAR**argv)
{ extern const u_int64_t COLS;
  extern const u_int64_t LINS;
  auto LLIST curr_line;
  auto u_int64_t max_line;
  auto u_int32_t flag_no_file, flag_changes,
                 flag_err, mode, i;
  auto int32_t fd_file;
  auto char*normal, *file_name_buff;


  auto ssize_t bytes_read,throw_out;
  auto ssize_t bytes_written;


  /* Atributions */
  if (argc == 1)
  { curr_line.CONTENT=(char*)malloc(COLS*sizeof (char));
    curr_line.NEXT=NULL;
    curr_line.LEN=(u_int64_t) 0;
    curr_line.INDEX=(u_int32_t) 1;
    flag_no_file = 1;
  }
  else flag_no_file = 0;
  mode = NOR;
  throw_out = flag_err = bytes_written = 0;
  while ((mode!=OUT) && (flag_err-ERR))
  {
    while (mode==INS)
    {

    }
    while (mode==NOR)
    { normal = malloc(COLS*sizeof(char));
      bytes_read = read(0,normal,128);
      if (normal[bytes_read] != '\n');
      else
        switch (*normal)
        { case '+':
          break;
          case '-':
          break;
          case 'a':
          break;
          case 'i':
          break;
          case 'q':
          mode=OUT;
          break;
          case 'w':
          if (flag_no_file)
		  ;
          break;
          case 'e':
          break;
          default:
          break;
        }
      free(normal);
    }
  }
  cleanup(curr_line);
  return flag_err;
}
