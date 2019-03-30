// table -subcat-name [-L|-S] [-a label "value"] [-r index] [index -l label] [index -v "value"] [-t [minrow,maxrow]] [-L|-S]
// table -subcat-name [-L|-S] index -c column
// tablematch -subcat-name [-i(gnorecase)] [-c matchcount] [-b maxbefore] [-a maxafter] "matchvalue"
// presumably -[-]subcat-name reset feature is inactive due to read-only

#ifndef TABLEHEADER
#include <stdio.h>
#include <string.h>
#include "copy.h"
#endif

#define TABLEMAXFILENAMESIZE		256
#define TABLEMAXINDEXSIZE		8
#define TABLEMAXLABELSIZE		8
#define TABLEMAXVALUESIZE		96
#define WITHNULL			+ 1

// set in table.c !

#ifdef TABLEHEADER
int tablematchmain(int argc, char **argv) {
  char *matchout = argv[0]; // 'xxxxxxxx,xxxxxxxx,xxxxxxxx,xx...'
  char matchmax = argv[0][0]; // '0'-'9'
#else
int main(int argc, char **argv) {
#endif
  argc--; argv = &argv[1];
  char filename[TABLEMAXFILENAMESIZE];
//nosave  char savename[TABLEMAXFILENAMESIZE];
  int reset = 0;
  int load = 0;
  if (argc >= 1 && argv[0][0] == '-' && argv[0][1] != '\0' && argv[0][2] != '\0') {
    if (argv[0][1] == '-')
      reset = (argv[0][2] == '\0') ? 2 : 1;
    sprintf(filename, TABLESAVESUBCATPATH, &argv[0][reset]);
    argc--; argv = &argv[1];
  } else {
    sprintf(filename, TABLESAVECOMMONPATH);
  }
  char *matchvalue = "[NOMATCHVALUE]";
  if (argc >= 1) {
    matchvalue = argv[0];
    argc--; argv = &argv[1];
  }
/* nosave
  sprintf(savename, TABLESAVECELLARPATH, filename);
  int avi = TABLESAVECELLAROFFSET; <- avi since redefined
  while (savename[avi] != '\0') {
    if (savename[avi] == '/') {
      savename[avi] = '-';
    }
    avi++;
  }
*/
/*
  int indexn = 0;
  int columnn = 0;
  char label[TABLEMAXLABELSIZE WITHNULL];
  int labeli = 0;
  while (labeli < TABLEMAXLABELSIZE) {
    label[labeli] = '_';
    labeli++;
  }
  label[labeli] = '\0';
  char value[TABLEMAXVALUESIZE WITHNULL];
  int valuei = 0;
  while (valuei < TABLEMAXVALUESIZE) {
    value[valuei] = '_';
    valuei++;
  }
  value[valuei] = '\0';
*/
/*
  // before operation, give an opportunity to load or save -------------------------------------------------------------
  if (argc >= 1 && argv[0][0] == '-' && argv[0][1] == 'L') {
    copy(savename, filename);
    argc--; argv = &argv[1];
  } else if (argc >= 1 && argv[0][0] == '-' && argv[0][1] == 'S') {
    copy(filename, savename);
    argc--; argv = &argv[1];
  }
*/
  // perform operation -------------------------------------------------------------------------------------------------
/* noargs
  int needlabelindex = 0;
  char *labelindexlabel;
  if (argc >= 3 && argv[1][0] == '-' && (argv[1][1] == 'l' || argv[1][1] == 'v' || argv[1][1] == 'c') && argv[1][2] == '\0') {
    if (argv[0][0] == '@') {
      needlabelindex = 1;
      labelindexlabel = &argv[0][1];
      indexn = -1;
    } else {
      avi = 0;
      while (argv[0][avi] != '\0') {
        indexn = (indexn * 10) + (int)(argv[0][avi] - '0');
        avi++;
      }
    }
    if (argv[1][1] == 'l') {
      labeli = 0;
      while (labeli < TABLEMAXLABELSIZE &&argv[2][labeli] != '\0') {
        label[labeli] = argv[2][labeli];
        labeli++;
      }
    } else if (argv[1][1] == 'v') {
      valuei = 0;
      while (valuei < TABLEMAXVALUESIZE && argv[2][valuei] != '\0') {
        value[valuei] = argv[2][valuei];
        valuei++;
      }
    } else if (argv[1][1] == 'c') {
      columnn = (int)(argv[2][0] - '0');
    }
    mode = argv[1][1];
    argc -= 3; argv = &argv[3];
  } else if (argc >= 2 && argv[0][0] == '-' && argv[0][1] == 'c' && argv[0][2] == '\0') {
    columnn = (int)(argv[1][0] - '0');
    mode = '|';
    argc -= 2; argv = &argv[2];
  } else if (argc >= 3 && argv[0][0] == '-' && argv[0][1] == 'a' && argv[0][2] == '\0') {
    labeli = 0;
    while (argv[1][labeli] != '\0') {
      label[labeli] = argv[1][labeli];
      labeli++;
    }
    valuei = 0;
    while (argv[2][valuei] != '\0') {
      value[valuei] = argv[2][valuei];
      valuei++;
    }
    mode = argv[0][1];
    argc -= 3; argv = &argv[3];
  } else if (argc >= 2 && argv[0][0] == '-' && argv[0][1] == 'r' && argv[0][2] == '\0') {
    avi = 0;
    while (argv[1][avi] != '\0') {
      if (argv[1][avi] < '0' || argv[1][avi] > '9') { return -2; }
      indexn = (indexn * 10) + (int)(argv[1][avi] - '0');
      avi++;
    }
    mode = argv[0][1];
    argc -= 2; argv = &argv[2];
  } else if (argc >= 1) {
    mode = '1';
    avi = 0;
    while (argv[0][avi] != '\0') {
      if (argv[0][avi] < '0' || argv[0][avi] > '9') {
        if (argv[0][avi] == 'i') {
          mode += 1;
        } else if (argv[0][avi] == 'l') {
          mode += 2;
        } else if (argv[0][avi] == 'v') {
          mode += 3;
        } else { return -2; }
      } else {
        indexn = (indexn * 10) + (int)(argv[0][avi] - '0');
      }
      avi++;
    }
    argc--; argv = &argv[1];
  } else mode = '*';
*/
//  int needlabelindex = 0;
//  char *labelindexlabel = "LBLMATCH";
  char mode = '/';
  int matchi = 0;
  int matchn = matchmax - '0';
  char *headout = matchout;
  headout[0] = '\0';
  int avi = 0; // arbitrary value index (1 + endpadding per line)
  char thisindex[TABLEMAXINDEXSIZE WITHNULL];
  char thislabel[TABLEMAXLABELSIZE WITHNULL];
  char thisvalue[TABLEMAXVALUESIZE WITHNULL];
  int tvi = 0;
//  int ixi = 0;
  int line = 0;
  int inch;
  char prevch = '?';
  char ch = '?';
  char fmode = '+';
  FILE *ff;
  // if printing the contents of a particular column
  // or need to find the index of a label
//  if (mode == 'c'  || needlabelindex == 1) {
  if (mode == '/') {
    ff = fopen(filename, "r");
    if (!ff) { return -1; }
    // translate label index if necessary
    if (matchn > 0) {
      avi = 0;
      ch = '\b';
      while (matchi < matchn) {
        // treat backspace as start-of-file
        if (ch != '\b') {
          if ((inch = fgetc(ff)) == EOF) break;
          ch = (char)inch;
          if (ch == '\b') break;
        }
        if (ch == '\b' || ch == '\n') {
          if (ch == '\b') { ch = '\n'; } // ..should never happen?
          else { line++; }
          fread(thisindex, sizeof(char), TABLEMAXINDEXSIZE, ff);
          thisindex[TABLEMAXINDEXSIZE] = '\0';
          fgetc(ff); // get ' '
          fread(thislabel, sizeof(char), TABLEMAXLABELSIZE, ff);
          thislabel[TABLEMAXLABELSIZE] = '\0';
          fgetc(ff); // get ' '
          fread(thisvalue, sizeof(char), TABLEMAXVALUESIZE, ff);
          thisvalue[TABLEMAXVALUESIZE] = '\0';
          tvi = 0;
          while ((thisvalue[tvi] == matchvalue[tvi]) ||
                 (thisvalue[tvi] == '_' && matchvalue[tvi] == '\0')) {
            if ((thisvalue[tvi] == '\0' && matchvalue[tvi] == '\0') ||
                (thisvalue[tvi] == '_' && matchvalue[tvi] == '\0')) {
              sprintf(headout, "%s%s", (matchi) ? "," : "", thislabel);
              while (headout[0] != '\0') { headout++; }
              matchi++;
              break; // next row after match
            } else if (thisvalue[tvi] == '\0' ^ matchvalue[tvi] == '\0') {
              break; // next row, no match
            }
            tvi++;
          }
          avi++;
        } else {
          avi++;
        }
      }
    }
//      fseek(ff, 0, SEEK_SET);
    fclose(ff);
  }



/*
    // move to and write column at indexn, columnn
    if (mode == 'c') {
      // if (tablemain(..) == -1) { no match .. }
      if (needlabelindex == 1) {
        fclose(ff);
        return -1;
      }
      avi = 0;
      while (avi < indexn - 1) {
        if ((inch = fgetc(ff)) == EOF) break;
        ch = (char)inch;
        if (ch == '\n') avi++;
      }
      char *coutvalue = "";
      if (columnn == 1) {
        fread(thisindex, sizeof(char), TABLEMAXINDEXSIZE, ff);
        avi = TABLEMAXINDEXSIZE - 1;
        while (thisindex[avi] == '_') {
          thisindex[avi] = '\0';
          avi--;
        }
        coutvalue = thisindex;
      } else if (columnn == 2) {
        fseek(ff, TABLEMAXINDEXSIZE + 1, SEEK_CUR);
        fread(thislabel, sizeof(char), TABLEMAXLABELSIZE, ff);
        avi = TABLEMAXLABELSIZE - 1;
        while (thislabel[avi] == '_') {
          thislabel[avi] = '\0';
          avi--;
        }
        coutvalue = thislabel;
      } else if (columnn == 3) {
        fseek(ff, TABLEMAXINDEXSIZE + 1 + TABLEMAXLABELSIZE + 1, SEEK_CUR);
        fread(thisvalue, sizeof(char), TABLEMAXVALUESIZE, ff);
        avi = TABLEMAXVALUESIZE - 1;
        while (thisvalue[avi] == '_') {
          thisvalue[avi] = '\0';
          avi--;
        }
        coutvalue = thisvalue;
      }
      // comparison feature needs to be documented
      if (argc >= 2 && argv[0][0] == '=' && argv[0][1] == '=' && argv[0][2] == '\0') {
        if (strcmp(coutvalue, argv[1]) == 0)
          { return 1; } else { return 0; }
      } else {
#ifdef TABLEHEADER
        sprintf(headout, "%s", coutvalue);
#else
        fprintf(stdout, "%s\n", coutvalue);
#endif
      }
      fclose(ff);
      return 0;
    }
  } else if (reset) {
    ff = fopen(filename, "w");
    fmode = '=';
  } else {
    ff = fopen(filename, "r+");
  }
  if (!ff & mode == 'a' && fmode == '+') {
    ff = fopen(filename, "w");
    fmode = '=';
  }
  if (!ff) { return -1; }
  while (fmode == '+' && (line != indexn) && (inch = fgetc(ff))) {
    if (inch == EOF) break;
    ch = (char)inch;
    if (ch == '\n') line++;
  }
  if (mode == 'a') indexn = line;
  char index[TABLEMAXINDEXSIZE WITHNULL];
  sprintf(index, "%d", indexn);
  int indexi = 0;
  while (indexi < TABLEMAXINDEXSIZE) {
    if (index[indexi] == '\0' || (indexi > 0 && index[indexi - 1] == '_')) {
      index[indexi] = '_';
    }
    indexi++;
  }
  index[indexi] = '\0';

  if (((mode >= '1' && mode <= '4') || mode == 'l' || mode == 'v' || mode == 'r') && line == indexn) {
    int filepos = ftell(ff);
    fread(thisindex, 1, TABLEMAXINDEXSIZE, ff);
    thisindex[TABLEMAXINDEXSIZE] = '\0';
    fgetc(ff);
    fread(thislabel, 1, TABLEMAXLABELSIZE, ff);
    thislabel[TABLEMAXLABELSIZE] = '\0';
    fgetc(ff);
    fread(thisvalue, 1, TABLEMAXVALUESIZE, ff);
    thisvalue[TABLEMAXVALUESIZE] = '\0';
    if (strcmp(index, thisindex) == 0) {
      if (mode >= '1' && mode <= '4') {
        if (mode == '1') {
          fprintf(stdout, "%s\t%s\t%s\n", thisindex, thislabel, thisvalue);
        } else if (mode == '2') {
          avi = TABLEMAXINDEXSIZE - 1;
          while (thisindex[avi] == '_') {
            thisindex[avi] = '\0';
            avi--;
          }
          fprintf(stdout, "%s\n", thisindex);
        } else if (mode == '3') {
          avi = TABLEMAXLABELSIZE - 1;
          while (thislabel[avi] == '_') {
            thislabel[avi] = '\0';
            avi--;
          }
          fprintf(stdout, "%s\n", thislabel);
        } else if (mode == '4') {
          avi = TABLEMAXVALUESIZE - 1;
          while (thisvalue[avi] == '_') {
            thisvalue[avi] = '\0';
            avi--;
          }
          fprintf(stdout, "%s\n", thisvalue);
        }
      } else {
        if (mode == 'l') {
          strcpy(value, thisvalue);
        } else if (mode == 'v') {
          strcpy(label, thislabel);
        } else if (mode == 'r') {
          avi = 0;
          while (avi < TABLEMAXINDEXSIZE) {
            index[avi] = '/';
            avi++;
          }
          avi = 0;
          while (avi < TABLEMAXLABELSIZE) {
            label[avi] = '/';
            avi++;
          }
          avi = 0;
          while (avi < TABLEMAXVALUESIZE) {
            value[avi] = '/';
            avi++;
          }
        }
        fseek(ff, filepos, SEEK_SET);
        fprintf(ff, "%s\t%s\t%s", index, label, value);
#ifndef TABLEHEADER
        fprintf(stdout, "%s\t%s\t%s\n", index, label, value);
#endif
      }
    }
  } else if (mode == 'a') {
    fseek(ff, (fmode == '+') ? -1 : 0, SEEK_END);
    if (fmode == '=' || ((char)fgetc(ff) == '\n')) {
      fprintf(ff, "%s\t%s\t%s\n", index, label, value);
    } else {
      fprintf(ff, "!\n%s\t%s\t%s\n", index, label, value);
    }
#ifndef TABLEHEADER
    fprintf(stdout, "%s\t%s\t%s\n", index, label, value);
#endif
  } else if (mode == '*' || mode == '|') {
    fseek(ff, 0, SEEK_SET);
    int col = 1;
    int outch = 0;
    while (inch = fgetc(ff)) {
      if (inch == EOF) break;
      ch = (char)inch;
      if (mode == '|') {
        if (ch == '\t') {
          col++;
          outch = 0;
        } else if (ch == '\n') {
          fprintf(stdout, "\n");
          col = 1;
          outch = 0;
        } else if (col == columnn) {
          if ((col == 1 && outch < TABLEMAXINDEXSIZE) ||
              (col == 2 && outch < TABLEMAXLABELSIZE) ||
              (col == 3 && outch < TABLEMAXVALUESIZE)   ) {
//#ifdef TABLEHEADER
//            headout[outch] = ch;
//            outch++;
//            headout[outch] = '\0';
//#else
            fprintf(stdout, "%c", ch); // for printing multiple column entries
//#endif
          }
        }
      } else if (mode == '*') {
        fprintf(stdout, "%c", ch); // for printing multiple full row entries
      }
    }
  }
  fclose(ff);
*/

/* nosave
  // after operation, give an opportunity to load or save (but only if an operation occurred)---------------------------
  if (mode != '?' && argc >= 1 && argv[0][0] == '-' && argv[0][1] == 'L') {
    copy(savename, filename);
    argc--; argv = &argv[1];
  } else if (mode != '?' && argc >= 1 && argv[0][0] == '-' && argv[0][1] == 'S') {
    copy(filename, savename);
    argc--; argv = &argv[1];
  }
*/
  return 0;
}
