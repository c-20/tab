#define BASECODELINESIZE		8192

// -1 for no cursor, -2 for end of line cursor
int stringbasecodechecksum(char *string, char *basecode, int cursorindex) {
  int linesum = 0;
  int sti = -1, bci = -1;
  while (string[++sti] != '\0') {
    int charcode = string[sti];
    int charc0 = charcode % 16;
    int charc1 = (charcode - charc0) / 16;
    basecode[++bci] = (char)(charc1 + 65);
    basecode[++bci] = (char)(charc0 + 65);
    if (cursorindex == sti) {
      basecode[++bci] = 'A';
      basecode[++bci] = 'A';
    }
    linesum += charcode;
  }
  if (cursorindex == -2) {
    basecode[++bci] = 'A';
    basecode[++bci] = 'A';
  }
  basecode[++bci] = '\0';
  return linesum;
}

int basecodestringchecksum(char *basecode, char *string, int *cursorindex) {
  int linesum = 0;
  char charcode = '\0';
  int charc0 = -1, charc1 = -1;
  int bci = -1, sti = -1;
  while (basecode[++bci] != '\0') {
    if (bci % 2 == 0)
      { charc1 = basecode[bci] - 65; }
    else {
      charc0 = basecode[bci] - 65;
      charcode = (char)((charc1 * 16) + charc0);
      if (charcode == 0)
        { *cursorindex = sti + 1; }
      else {
        string[++sti] = charcode;
        linesum += string[sti];
      }
    }
  }
  string[++sti] = '\0';
  return linesum;
}
