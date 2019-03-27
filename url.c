#include "/tau/src/cmd/code.h"

#include <stdio.h>

#define HOMEPATH      "/tau"
#define URLBUFDIR     HOMEPATH

int main(int argc, char **argv) {
  char *dfn = (argc > 1) ? argv[1] : "";
  char dlurl[512];
  sprintf(dlurl, "%s/%s", URLBUFDIR, dfn);
// todo: copynopath for dfn
  int dln = 0; while (dfn[dln] != '\0') { dln++; }
  if (dln > 3) {
    if (dfn[dln - 3] == '.' && dfn[dln - 2] == 'j' && dfn[dln - 1] == 's')
      { fprintf(stdout, "Content-Type: text/javascript\r\n"); }
    else { fprintf(stdout, "Content-Type: text/plain\r\n"); }
  } else { fprintf(stdout, "Content-Type: text/plain\r\n"); }
  fprintf(stdout, "Cache-Control: max-age=0, must-revalidate\r\n");
  fprintf(stdout, "Content-Encoding: identity\r\n");
  fprintf(stdout, "Pragma: no-cache\r\n\r\n");
  FILE *f = fopen(dlurl, "r");
  int inch;
  char ch;
  while (1) {
    inch = fgetc(f);
    if (inch == EOF) { break; }
    ch = (char)inch;
    if ((ch >= ' ' && ch <= '~') || (ch == '\n' || ch == '\t'))
      { fprintf(stdout, "%c", ch); }
  }
  fclose(f);
  return 0;
}
