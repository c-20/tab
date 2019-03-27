#define TABLEHEADER

#include "table.c"
#include "tablematch.c"
#include "hash64.c"

#define TABLEHASHINITSIZE	((u_int64_t)14695981039346656037ULL)
#define TABLEHASHTRUESIZE	((u_int64_t)218340105584896ULL)

// table -subcat-name [-L|-S] [-a label "value"] [-r index] [index -l label] [index -v "value"] [-t [minrow,maxrow]] [-L|-S]
// table -subcat-name [-L|-S] index -c column
char tableaddrow(char *name, char source, char *label, char *value) {
  char *argv[5];
  int argc = 5;
  char subcat[9];
  sprintf(subcat, "-%s-%c", name, source);
  argv[0] = "@table";
  argv[1] = subcat;
  argv[2] = "-a";
  argv[3] = label;
  argv[4] = value;
  if (tablemain(argc, argv) == 0)
    { return 'a'; } else { return '-'; }
}

int tablelabelindex(char *name, char source, char *label) {
  char *argv[5];
  int argc = 5;
  char subcat[9];
  sprintf(subcat, "-%s-%c", name, source);
  char atlabel[TABLEMAXLABELSIZE + 1];
  atlabel[0] = '@';
  sprintf(&atlabel[1], "%s", label);
  char forindex[TABLEMAXINDEXSIZE + 1];
  forindex[0] = '\0';
  argv[0] = forindex;
  argv[1] = subcat;
  argv[2] = atlabel;
  argv[3] = "-c";
  argv[4] = "1";
  if (tablemain(argc, argv) == 0) {
    int labelindex = 0, lii = -1;
    while (++lii < TABLEMAXINDEXSIZE && argv[0][lii] != '\0')
      { labelindex = (labelindex * 10) + (int)(argv[0][lii] - '0'); }
    return labelindex;
  } else { return 0; }
}

char tablesetrow(char *name, char source, char *label, char *value) {
  int tli = tablelabelindex(name, source, label);
  char labelindex[TABLEMAXINDEXSIZE + 1];
  sprintf(labelindex, "%d", tli);
  char *argv[5];
  int argc = 5;
  char subcat[9];
  sprintf(subcat, "-%s-%c", name, source);
  argv[0] = "@table";
  argv[1] = subcat;
  argv[2] = labelindex;
  argv[3] = "-v";
  argv[4] = value;
  if (tablemain(argc, argv) == 0)
    { return 's'; } else { return '-'; }
}

char tablelabelvalue(char *valueout, char *name, char source, char *labelin) {
  char *argv[5];
  int argc = 5;
  char subcat[9];
  sprintf(subcat, "-%s-%c", name, source);
  char atlabel[TABLEMAXLABELSIZE + 1];
  atlabel[0] = '@';
  sprintf(&atlabel[1], "%s", labelin);
  char forvalue[TABLEMAXVALUESIZE + 1];
  forvalue[0] = '\0';
  argv[0] = forvalue;
  argv[1] = subcat;
  argv[2] = atlabel;
  argv[3] = "-c";
  argv[4] = "3";
  int tableresult = tablemain(argc, argv);
  if (tableresult == -1) {
    valueout[0] = '\0';
    return 'x';
  } else if (tableresult == 0) {
    int vii = -1;
    while (++vii < TABLEMAXVALUESIZE && argv[0][vii] != '\0')
      { valueout[vii] = argv[0][vii]; }
    valueout[vii] = '\0';
    return 'v';
  } else { return tableresult; }
}

// label hashes are 8-byte, 62 bits per byte
// labelout should be exactly 9 characters wide, with [8] = '\0'
// does not access table - NAME AND SOURCE UNUSED
char tablevaluelabel(char *labelout, char *name, char source, char *valuein) {
  char *labelcharset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int valuelen = 0; while (valuein[valuelen] != '\0') { valuelen++; }
  u_int64_t hashtotal = fnv_64_buf(valuein, valuelen, TABLEHASHINITSIZE);
  hashtotal %= TABLEHASHTRUESIZE;
  char hashlabel[TABLEMAXLABELSIZE WITHNULL];
  hashlabel[TABLEMAXLABELSIZE] = '\0';
  hashlabel[0] = hashlabel[1] = hashlabel[2] = hashlabel[3] = '@';
  hashlabel[4] = hashlabel[5] = hashlabel[6] = hashlabel[7] = '@';
  int hashindex = TABLEMAXLABELSIZE, hashdiff = 0;
  while (--hashindex >= 0) {
    hashdiff = (int)(hashtotal % 62);
    hashlabel[hashindex] = labelcharset[hashdiff];
    labelout[hashindex] = labelcharset[hashdiff];
    hashtotal /= 62;
  }
  labelout[TABLEMAXLABELSIZE] = '\0';
  return (hashtotal == 0) ? 'l' : '-';
}
char tablegenvaluelabel(char *labelout, char *valuein) {
  return tablevaluelabel(labelout, NULL, 0, valuein);
} // name and source unused in tablevaluelabel for now

char tablevaluelabeladdrow(char *labelout, char *name, char source, char *valuein) {
  char tlab = tablevaluelabel(labelout, name, source, valuein);
  if (tlab == 'l') {
    int tlri = tablelabelindex(name, source, labelout);
    if (tlri == 0) {
      char tadd = tableaddrow(name, source, labelout, valuein);
      return (tadd == 'a') ? 'a' : '-';
    } else { return 'e'; }
  } else { return '!'; }
}

int tablevaluelabelmatch(char *labelsout, int labelsmax, char *name, char source, char *valuein) {
//int table value label match label index (char *name, char source, char *label) {
// labelsmax == 3? labelsout == xxxxxxxx,xxxxxxxx,xxxxxxxx
  char *argv[3];
  int argc = 3;
  char subcat[9]; // TODO: SET MAX SUBCAT SIZE!!!!
  sprintf(subcat, "-%s-%c", name, source);
  argv[0] = labelsout;
  argv[1] = subcat;
  argv[2] = valuein;
  labelsout[0] = labelsmax + '0';
  if (tablematchmain(argc, argv) == 0) {
    int labels = (labelsout[0] == '\0') ? 0 : 1;
    int loi = -1;
    while (labelsout[++loi] != '\0') {
      if (labelsout[loi] == ',')
        { labels++; }
    } // count entries in label,label,label list
    return labels;
  } else { return 0; }
}


#undef TABLEHEADER
