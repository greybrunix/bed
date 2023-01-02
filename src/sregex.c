#include "sregex.h"
#include <stdio.h>
/*
 * let c represent a literal character
 * let . be a substitute for any character
 * let \* represent kleene closure (0 or more)
 * let \+ represent transitive closure (no less than 1)
 * let \^ represent beginning of a line
 * let \$ represent EOL
 * let \| represent the alternative operator (a+b in trad regex)
 * let us consider the empty regular expression to not have representation
   * and as being trivially recognized
 */


int 
match_(char *regexp,
      char*text)
{
  if (regexp[0] && regexp[1] == '^')
    return match_here(regexp+2,text);
  do
  {
    if (match_here(regexp, text))
      return 1;
  } while (*text++ != '\0');
  return 0;
}
int 
match_here(char*regexp,
          char*text)
{
  if (regexp[0] == '\0')
    return 1;
  if (regexp[1]=='\\'&&regexp[2] == '|'&&regexp[3]!='\0')
     return match_choice(regexp[0],regexp[3],regexp+4,text);
  if (regexp[1]=='\\'&&regexp[2] == '*')
    return match_kleene(regexp[0], regexp+3,text);
  if (regexp[1]=='\\'&&regexp[2] == '+') 
    return match_transitive(regexp[0], regexp+3,text);
  if (regexp[0] == '\\'&&regexp[1] == '$' && regexp[2] == '\0')
    return *text == '\0';
  if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
    return match_here(regexp+1,text+1);
  return 0;
}
/*
int
match_kleene(int c,
            char*regexp,
            char*text)
{
  do
  {
    if (match_here(regexp, text))
       return 1;
  } while (*text != '\0' && (*text++ == c || c == '.'))
  return 0;
}
*/
int
match_transitive(int c,
                char*regexp,
                char*text)
{
  while (*text != '\0' && (*text++ == c || c == '.'))
  {
    if (match_here(regexp, text))
      return 1;
  }
  return 0;
}

/* longest path */
int
match_kleene(int c, char*regexp, char*text)
{
  char*t;
  for (t=text; *t!='\0' && (*t == c || c=='.');t++);
  do 
  {
    if (match_here(regexp, t))
      return 1;
  } while (t-- > text);
  return 0;
}

int
match_choice(int c1,
            int c2,
            char*regexp,
            char*text)
{
  if (*text == c1 || *text == c2)
    return 1;
  return 0;
}

int
main(void)
{
int res;
char reg[20] = "\\^regexde\\*znu\\+ts\\$";
char text[100] = "regexdeeeeeeznuuuuuuuuuuuts";
res = match_(reg, text);
printf("%d\n",res);
return 0;
}
