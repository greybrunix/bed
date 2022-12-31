# BED

A line editor for personal use aptly named bed (Bruno EDitor, pronounced bee-eh-dee) after myself.

As a line editor it takes inspiration from the official standard UNIX editor ed.

FEATURES/MNEMONICS:
1. **e**dit file
2. **i**nsert line to buffer (overwrites current content of buffer)
3. **a**ppend line to buffer
4. **+**1 to line number (go to next line)
5. **-**1 to line number (go to previous line)
6. **w**rite contents of buffer to a file
7. **q**uit and exit the editor

MISSING
1. when loading a file, load also it's contents to the buffer

TODO
1. DELETE lines
2. GOTO any given (existing) absolute line
3. GOTO a relative line
4. PRINT multiple lines
5. DELETE multiple lines
6. REGULAR EXPRESSIONS (an article by BRIAN KERNIGHAN was added to help
   in regards to this feature)
7. SUBSTITUTE contents in a line via REGEX
8. GLOBALY match contents via REGEX
9. VISUAL DISPLAY as an alternative method to showing the editor
