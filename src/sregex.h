#ifndef SREGEX_H
#define SREGEX_H

int match_(char*regexp,char*text);
int match_here(char*regexp,char*text);
int match_kleene(int c, char*regexp, char*text);
int match_transitive(int c, char*regexp, char*text);
int match_choice(int c1,int c2,char*regexp,char*text);

#endif
