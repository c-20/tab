#define WRITEBUFMAXCHARLEN				128
#define WRITEBUFMAXBASELEN				256

void writebufclean(char *buf, int basecode) {
  int bfi = -1; while (buf[++bfi] != '\0') {
    if (buf[bfi] == '"')
      { fprintf(stdout, "&quot;"); }
    else if (buf[bfi] == '&')
      { fprintf(stdout, "&amp;"); }
    else if (buf[bfi] == '<')
      { fprintf(stdout, "&lt;"); }
    else if (buf[bfi] == '>')
      { fprintf(stdout, "&gt;"); }
    else if (buf[bfi] == '\\')
      { fprintf(stdout, "&bsol;"); }
    else { fprintf(stdout, "%c", buf[bfi]); }
//    if (buf[bfi] == '"')
//      { fprintf(stdout, "%c%c", '\\', '"'); }
//    else { fprintf(stdout, "%c", buf[bfi]); }
//    fprintf(stdout, "%c", buf[bfi]);
  }
}

void writebufhighlight(char *buf, int basecode) {
  char charbuf[WRITEBUFMAXCHARLEN];
  char basebuf[WRITEBUFMAXBASELEN];
  char *cfmt = "<b class=\"%s\">%c</b>";
  char *sfmt = "<b class=\"%s\">%s</b>";
  char *cescfmt = "<b class=\\\"%s\\\">%c</b>";
  char *sescfmt = "<b class=\\\"%s\\\">%s</b>";
  #define WRITEBUFMATCHLISTLEN 8
  char *clr[WRITEBUFMATCHLISTLEN];
  char *mch[WRITEBUFMATCHLISTLEN];
  clr[0] = "clr0"; mch[0] = "abcdefghijklmnopqrstuvwxyz";
  clr[1] = "clr1"; mch[1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  clr[2] = "clr2"; mch[2] = "{};";
  clr[3] = "clr3"; mch[3] = "()[]:,.=~#$%@";
  clr[4] = "clr4"; mch[4] = "'\"";
  clr[5] = "clr5"; mch[5] = "0123456789";
  clr[6] = "clr6"; mch[6] = "\\<>+-*/^_`&|!?";
  clr[7] = "clr9"; mch[7] = " \t";
  #define WRITEBUFBLACKLISTLEN 8
  char *blxx[WRITEBUFBLACKLISTLEN];
  char *blx = "<>\" \t\\\r~";
  blxx[0] = "&lt;";
  blxx[1] = "&gt;";
  blxx[2] = "&quot;";
  blxx[3] = "&nbsp;";
  blxx[4] = "&nbsp;&nbsp;";
  blxx[5] = "&bsol;";
  blxx[6] = "</div><div class=\"subline\">";
  blxx[7] = "</div><div class=\"subline\">";
  char trmode = '\0';
  char submode = '-';
  char *trclr = "clr7";
  char *limclr = "clr6";
  char *subclr = "clr8";
  char *rightsquareclr = "rsqb";
  char *dashclr = "dash";
  char *scoreclr = "score";
  char *equalsclr = "equals";
  char *actionclr = "action";
  char *actendclr = "actend";
  char *spaceclr = clr[7];
  char *wordclr = clr[0];
  char *commandclr = clr[1];
  char *datasrcclr = clr[5];
  char *tagclr = "tag";
  char *tagnameclr = "clr5";
  char activeline = 'n';
  char commandname = 'n';
  char datasrcname = 'n';
  char tagname = 'n';
  char actionlist = 'n';
  char actionname = 'n';
  char lschar = '\0';
  char tschar = '\n'; // for sol matches
  char trchar = '%';
  char tbchar = ']';
  char *selclr = "";
  char *lsclr = selclr;
  char subline = 'n';
  char *subclass = NULL;
  int charsum = -1;
  int bfi = -1;
  char *linestart = "<div class=\"line\">";
  char *linestop = "</div>";
  if (basecode == 1) {
    charsum = stringbasecodechecksum(linestart, basebuf, -1);
    fprintf(stdout, "%s", basebuf);
  } else { fprintf(stdout, "%s", linestart); }
  if (basecode == 1) // SOL wisp
    { fprintf(stdout, "AA"); }
  while (buf[++bfi] != '\0') {
    int bfsfmt = 0;
    int bfn = -1; while (++bfn < WRITEBUFMATCHLISTLEN) {
      int bfa = -1; while (mch[bfn][++bfa] != '\0') {
        if (mch[bfn][bfa] == buf[bfi]) {
          lschar = tschar;
          lsclr = selclr;
          tschar = buf[bfi];
          selclr = (trmode == '\0' || tschar == '*' || tschar == '/') ? clr[bfn] : trclr;
          if (trmode == '\n' && tschar == '\n') { trmode = '\0'; }
          if (trmode == '\t' && lschar == '*' && tschar == '/') { trmode = '\0'; }
          if (lschar == '/' || lschar == '#' || lschar == '$' || lschar == '%')
            { if (trmode == '\0' && tschar == '/') { trmode = '\n'; trchar = lschar; } }
          if (submode == '[' && lschar == '[' && tschar == '.') { submode = '.'; }
          else if (submode == '[') { submode = '-'; }
          if (trmode == '\0' && lschar == '\n' && tschar == '[') { submode = '['; }
          if (submode == '.' && tschar == ']') { submode = '-'; }
          else if (submode == '.') { selclr = subclr; }
          if ((trmode == '\0' || trmode == '\n') && lschar == '/' && tschar == '*') { trmode = '\t'; }
          if (selclr == trclr && tschar == '[') { selclr = limclr; tbchar = '['; }
          if (selclr == trclr && tschar == ']') { selclr = limclr; tbchar = ']'; }
          if (selclr == trclr && tschar == '.') { selclr = limclr; } // clr[bfn]; }
          if (tbchar == ']' && trchar != '/' && tschar == ' ') { trmode = '\0'; }
          if (lschar == '\n' && tschar == ']') { selclr = rightsquareclr; activeline = 'y'; }
          if (lsclr == rightsquareclr && tschar == '-') { selclr = dashclr; activeline = 'n'; }
          if (lsclr == rightsquareclr && tschar == ':') { selclr = scoreclr; commandname = 'y'; }
          if (lsclr == rightsquareclr && tschar == '=') { selclr = equalsclr; datasrcname = 'y'; }
 //         if (lsclr == rightsquareclr && tschar == '[') { actionlist = 'y'; }
          if (activeline == 'n' && tschar == '[') { actionname = 'y'; selclr = actionclr; }
          else if (actionname == 'y' && tschar == ']') { actionname = 'n'; selclr = actendclr; }
          else if (actionname == 'y') { selclr = wordclr; }
          if (activeline == 'y' && tschar == '.') { tagname = 'y'; selclr = tagclr; }
          else if (tagname == 'y' && (tschar < 'a' || tschar > 'z')) { tagname = 'n'; }
          else if (tagname == 'y') { selclr = tagnameclr; }
          if (commandname == 'y' && selclr == wordclr) { selclr = commandclr; }
          if (commandname == 'y' && selclr == spaceclr) { commandname = 'n'; }
          if (datasrcname == 'y' && selclr == wordclr) { selclr = datasrcclr; }
          if (datasrcname == 'y' && selclr == spaceclr) { datasrcname = 'n'; }
          if (datasrcname == 'y' && selclr == wordclr) { selclr = datasrcclr; }
          if (datasrcname == 'y' && selclr == spaceclr) { datasrcname = 'n'; }
          int bfxs = 0, bfx = -1;
          if (subline == 'h') {
            if (buf[bfi] == '>') { subline = '>'; subclass = "output"; }
            else if (buf[bfi] == '<') { subline = '<'; subclass = "input"; }
            else if (buf[bfi] == ':') { subline = ':'; subclass = "label"; }
            else if (buf[bfi] == '|') { subline = '|'; subclass = "command"; }
            else if (buf[bfi] == '@') { subline = '@'; subclass = "context"; }
            else { subline = 'y'; }
            if (subclass != NULL) { selclr = subclass; }
          } else if (buf[bfi] == '~') { subline = 'h'; subclass = NULL; }
          while (subclass == NULL && blx[++bfx] != '\0') {
            if (blx[bfx] == buf[bfi]) { bfxs = 1; break; }
          }
          if (basecode == 1) {
            if (bfxs == 0) { sprintf(charbuf, cfmt, selclr, buf[bfi]); }
            if (bfxs == 1) { sprintf(charbuf, sfmt, selclr, blxx[bfx]); }
            charsum = stringbasecodechecksum(charbuf, basebuf, -1);
            fprintf(stdout, "%s", basebuf);
          } else {
            if (bfxs == 0) { sprintf(charbuf, cescfmt, selclr, buf[bfi]); }
            if (bfxs == 1) { sprintf(charbuf, sescfmt, selclr, blxx[bfx]); }
            fprintf(stdout, "%s", charbuf);
          }
          bfsfmt = 1;
        }
      }
    }
    if (bfsfmt == 0) {
      if (basecode == 1) {
        sprintf(charbuf, "%c", buf[bfi]);
        charsum = stringbasecodechecksum(charbuf, basebuf, -1);
        fprintf(stdout, "%s", basebuf);
      } else {
        sprintf(charbuf, "%c", buf[bfi]);
        fprintf(stdout, "%s", charbuf);
      }
      bfsfmt = 0; // reset after each char (changed)
    }
  }
  if (basecode == 1) {
    charsum = stringbasecodechecksum(linestop, basebuf, -1);
    fprintf(stdout, "%s", basebuf);
  } else { fprintf(stdout, "%s", linestop); }
//  if (basecode == 1) // EOL wisp
//    { fprintf(stdout, "AA"); }
}
