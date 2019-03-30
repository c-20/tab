#include "code.h"
#include "utf8.h"

// soon: utf8chars ..... and translations ....

// tib @ "{s:0,f:aBcDeFgH,c:0.0,l:1,o:-,n:-}"
// saline <linenumber> <-a newvalue|-r newvalue|-d ""> [filename]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include "copy.h"
#include "table.h"
#include "tablecount.h"
#include "basecode.h"
#include "writebuf.h"
#include "shell.h"

#define TIBBUFDIR       "/V:"

#define NUMAXLEN	8
#define NUSRCLEN	1
#define NUFILEN		8
#define NUCONTEN	6
#define NULINEN		6
#define NUOLDCHK	4
#define NUNEWCHK	4

#define NUNAMETABLE     "nodes"
#define NUCOUNTABLE     "count"
#define NUDIRLISTTABLE  "dirlist"

#define MAXINTDIGITS	10
#define TABTIBPATHSIZE		1024
#define TABTIBLINESIZE		8192
#define NULINEDISMISS		"<pre>{ \"result\": \"dismiss\" }</pre>"
#define NULINEDECLINE		"<pre>{ \"result\": \"decline\" }</pre>"
#define NULINENOMATCH		"<pre>{ \"result\": \"nomatch\" }</pre>"

// s/buf/file/g

#define tib main
IN tib($) {
  char canlistdir = 'y';
  char canlistcmd = 'y';
  char *base62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~@-.!";
  char inpathname[TABTIBPATHSIZE + 1];
  char outpathname[TABTIBPATHSIZE + 1];
  char labelbuf[TABLEMAXLABELSIZE + 1];
  char valuebuf[TABLEMAXVALUESIZE + 1];
  CS dirbuf = TIBBUFDIR; // assume cmd vars go in vardir
  CS vardir = dirbuf;
  CS cmddir = dirbuf;
  IN debugmode = 0;
  CS querystr = getenv("QUERY_STRING");
  IF (querystr EQNULL AND $N GQ 1) {
    querystr = $1; // ./tib "querystr"
    debugmode = 1; // do not translate query from basecode
  } EF (querystr EQNULL) { RT 0; } // blank return if no query
  IN querystrlen = STRLEN(querystr);
  fprintf(stdout, "Content-Type: text/html\r\n");
  fprintf(stdout, "Cache-Control: no-cache, no-store, must-revalidate\r\n");
  fprintf(stdout, "Content-Encoding: identity\r\n");
  fprintf(stdout, "Pragma: no-cache\r\n");
  fprintf(stdout, "Expires: 0\r\n\r\n");
  fprintf(stdout, "<html><head><meta charset=\"UTF-8\"></head><body>\n");
  fflush(stdout);
  if (querystr == NULL)
    { fprintf(stdout, "%s A</body></html>\n", NULINEDISMISS); fflush(stdout); return 0; }
  else if (querystrlen == 0)
    { fprintf(stdout, "%s B</body></html>\n", NULINEDISMISS); fflush(stdout); return 0; }
  else {
    char sourceindex[NUSRCLEN + 1]; int sii = NUSRCLEN;
    sourceindex[sii] = 255; while (--sii >= 0) { sourceindex[sii] = 0; }
    char filenode[NUFILEN + 1]; int fni = NUFILEN;
    filenode[fni] = 255; while (--fni >= 0) { filenode[fni] = 0; }
    char context[NUCONTEN + 1]; int cni = NUCONTEN; // variable length str
    context[cni] = 255; while (--cni >= 0) { context[cni] = 0; }
    char lineindex[NULINEN + 1]; int lii = NULINEN; // variable length num
    lineindex[lii] = 255; while (--lii >= 0) { lineindex[lii] = 255; }
    char oldsum[NUOLDCHK + 1]; int osi = NUOLDCHK; // varnum < 0, varstr > 0
    oldsum[osi] = 255; while (--osi >= 0) { oldsum[osi] = 255; }
    char newsum[NUNEWCHK + 1]; int nsi = NUNEWCHK; // varnum < 0, varstr > 0
    newsum[nsi] = 255; while (--nsi >= 0) { newsum[nsi] = 255; }
 //   char linevalue[TABLEMAXVALUESIZE + 1]; linevalue[0] = '\0';
//    if (querystr[querystrlen - 1] == '\n')
//      { querystrlen--; querystr[querystrlen] = '\0'; }
//fprintf(stderr, "QUERYSTR: %s\n", querystr);

// consider transation from %NN ! copyfromhex to get querystr!---

    if (querystr[querystrlen - 1] != '}') {
      fprintf(stdout, "%s \"\"!!</body></html>\n", NULINEDISMISS);
      fflush(stdout); return 0;
    } else {
      int qpi = querystrlen - 1;
      while (qpi >= 0 && querystr[qpi] != '{') { qpi--; }
      if (querystr[qpi] != '{') {
        fprintf(stdout, "%s</body></html>\n", NULINEDISMISS);
        fflush(stdout);
        return 0;
      } else {
        querystr[qpi] = '\0';
        while (querystr[++qpi] != '}') {
          if (querystr[qpi] == ':') {
            char qpc = querystr[qpi - 1];
            if (qpc == 's' || qpc == 'f' || qpc == 'c' ||
                qpc == 'l' || qpc == 'o' || qpc == 'n') {
              // 1 digit source index
              // 8 digit base62 filename
              // 6 digit line number (< 1 million lines)
              // 2 digit oldchecksum (-1 == addnewline)
              // 2 digit newchecksum (-1 == deleteline)
              // all allow full base62 charset (^-)?[a-zA-Z0-9@~]+
              // value (filename or linedata) should use basecode [A-Z]++
              int qpo = 0; int qpoco = -1;
              char *chkstr = NULL; int chkstrlen = 0;
              if (qpc == 's') { chkstr = sourceindex; chkstrlen = NUSRCLEN; }
              else if (qpc == 'f') { chkstr = filenode; chkstrlen = NUFILEN; }
              else if (qpc == 'c') { chkstr = context; chkstrlen = NUCONTEN; }
              else if (qpc == 'l') { chkstr = lineindex; chkstrlen = NULINEN; }
              else if (qpc == 'o') { chkstr = oldsum; chkstrlen = NUOLDCHK; }
              else if (qpc == 'n') { chkstr = newsum; chkstrlen = NUNEWCHK; }
//              if (qpc != 'l') { // variable length values can't be negative
//              int qpf = 0;
              if (querystr[qpi + 1] == '-') // ^^ are negative by default
                { chkstr[chkstrlen] = 255; qpo++; } // 255 last byte
              else if (qpc != 'l')
                { chkstr[chkstrlen] = '\0'; } // 255 -> \0 termination
//              }
              while (++qpo <= chkstrlen) {
                if ((querystr[qpi + qpo] >= '0' && querystr[qpi + qpo] <= '9') ||
                    (querystr[qpi + qpo] >= 'a' && querystr[qpi + qpo] <= 'z') ||
                    (querystr[qpi + qpo] >= 'A' && querystr[qpi + qpo] <= 'Z') ||
                    (querystr[qpi + qpo] == '~' || querystr[qpi + qpo] == '@') ||
                    (querystr[qpi + qpo] == '-' || querystr[qpi + qpo] == '.')   ) {
                  char qpoc = querystr[qpi + qpo];
                  if (qpoc == '~') { qpoc = 62; }
                  else if (qpoc == '@') { qpoc = 63; }
                  else if (qpoc == '-') { qpoc = 64; }
                  else if (qpoc == '.') { qpoc = 65; }
                  else if (qpoc >= 'A' && qpoc <= 'Z') { qpoc -= 'A' - 36; }
                  else if (qpoc >= 'a' && qpoc <= 'z') { qpoc -= 'a' - 10; }
                  else if (qpoc >= '0' && qpoc <= '9') { qpoc -= '0'; }
                  else { qpoc = 66; }
                  if (qpc == 'f' || qpc == 'c')
                    { chkstr[++qpoco] = base62[qpoc]; }
                  else { chkstr[++qpoco] = qpoc; }
//                    chkstr[++qpoco] = base62[qpoc];
                  if (chkstr[chkstrlen] == '\0')
                    { chkstr[qpoco + 1] = '\0'; }
                } else { break; }
              }
            }
          }
        }
      }
    }
    // -- assign source index [0-9]
    char source = '@';
    if (sourceindex[NUSRCLEN] == '\0') {
      if (sourceindex[0] >= 0 && sourceindex[0] <= 9)
        { source = '0' + sourceindex[0]; }
//dirbuf = buflistdirs[sourceindex[0]]; }
// cannot change source att
    }
    // -- ensure directory is accessible
    // note: dirbuf has no trailing / !
    // dirbuf is cmddir is vardir, so test vardir
    struct stat dirstatus;
    if ((stat(vardir, &dirstatus) == -1) || (!S_ISDIR(dirstatus.st_mode)))
       { fprintf(stdout, "%s</body></html>\n", NULINEDECLINE); fflush(stdout); return 0; }
    FILE *infp = NULL, *outfp = NULL;
    char action = '.';
    int filesize = 0, indexed = 0;
    // -- evaluate action
    int fnt = filenode[NUFILEN];
    int ost = oldsum[NUOLDCHK];
    int nst = newsum[NUNEWCHK];
    if (fnt == 255) { action = 'o'; } // f:-,l:f
    else {
      if (lineindex[0] == 255) { action = '-'; }              // l:- (var length)
      else if (ost == 255 && nst == 0 && newsum[0] == 63) { action = 's'; } // s-@
      else if (ost == 255 && nst == 255) { action = 'l'; }    // l--
      else if (ost == 255 && nst == 0  ) { action = 'a'; }    // l-n
      else if (ost == 0   && nst == 0  ) { action = 'r'; }    // lnn
      else if (ost == 0   && nst == 255) { action = 'd'; }    // ln-
      else { action = '!'; }
    }
    char infilesuffix = '-';
    char outfilesuffix = '-';
    int labelindex = -1;
    int valuesum = 0, valuecursor = -1;
    int linesum = 0, linecursor = -1;
    int linenumber = 0;
    int linenumstart = 0;
    int linenumlen = 0;
    int linenumlim = 0;
    int linenumstop = 0;
    char inlinebuf[TABTIBLINESIZE + 1];
    inlinebuf[0] = '\0';
    char outlinebuf[TABTIBLINESIZE + 1];
    outlinebuf[0] = '\0';
    int outlinebufi = 0;
    int inbex = -1;
    // -- open, line, add, replace, delete (olard)
    int valbuflen = -1;
    char nudirlist = 'u';
    char nucmdlist = 'u';
    char *nutable = NUNAMETABLE;
    if (action == 'o') { // open
      if (querystr[0] != '\0') {
        if (debugmode == 1) { sprintf(valuebuf, "%s", querystr); }
        else { valuesum = basecodestringchecksum(querystr, valuebuf, &valuecursor); }
      } else { valuebuf[0] = '@'; valuebuf[1] = '\0'; }
      while (valuebuf[++valbuflen] != '\0');
      nudirlist = (valuebuf[valbuflen - 1] == '/') ? 'y' : 'n';
      nucmdlist = (valuebuf[0] == '@') ? 'y' : 'n';
      if (nucmdlist == 'y' && nudirlist == 'y')
        { nudirlist = 'n'; }
      if (nudirlist == 'y') { //        nutable = NUDIRLISTTABLE;
//        strcat(valuebuf, "ls -1dF *");
// todo: fix shellexec for command strings
        strcat(valuebuf, "*[ls,-1dF]");
        while (valuebuf[++valbuflen] != '\0');
      } else if (nucmdlist == 'y') {
        valuebuf[valbuflen] = ']';
        valbuflen++;
        valuebuf[valbuflen] = '\0';
        valuebuf[0] = '[';
      }
      char tvl = tablevaluelabel(labelbuf, nutable, source, valuebuf);
      labelindex = (tvl == 'l') ? tablelabelindex(nutable, source, labelbuf) : 0;
      char tva = (labelindex > 0) ? 'e' : 'n';
      if (tvl == 'l' && tva == 'n') {
        tva = tableaddrow(nutable, source, labelbuf, valuebuf);
        labelindex = (tva == 'a') ? tablelabelindex(nutable, source, labelbuf) : 0;
      }
      if (nudirlist == 'y') { // lists and cmds in cmddir (dirbuf)
        if (canlistdir == 'y') { // lists start with =
          nudirlist = shellexec(valuebuf, cmddir, labelbuf);
          if (nudirlist != 'y') { action = '!'; }
          sprintf(valuebuf, "%s/=%s", cmddir, labelbuf);
          infilesuffix = '\0';
          outfilesuffix = '0';
        } else { action = '!'; }
      } else if (nucmdlist == 'y') { // commands start with @
        if (canlistcmd == 'y') {
          nucmdlist = shellexec(valuebuf, cmddir, labelbuf);
//          if (nucmdlist != 'y') { action = '!'; }
          sprintf(valuebuf, "%s/@%s", cmddir, labelbuf);
          infilesuffix = '\0';
          outfilesuffix = '0';
        } else { action = '!'; }
      } else {
        infilesuffix = '\0';
        outfilesuffix = tableresetcounter(NUCOUNTABLE, source, labelbuf);
      }
      linenumber = (tva == 'n') ? -1 : 0;
//      linesuffix = (tva == 'a') ? '+' : (tva == 'e') ? '=' : '!';
      indexed = (linenumber == 0) ? 1 : 0;
    } else if (action == 's' || action == 'l' || action == 'a' || action == 'r' || action == 'd') {
      char tlv = tablelabelvalue(valuebuf, nutable, source, filenode);
      labelindex = (tlv == 'v') ? tablelabelindex(nutable, source, filenode) : 0;
      while (valuebuf[++valbuflen] != '\0');
// /path/[ls,-1d] later find ] then [ then * then / for dirlist = 'y';
// nucmdlist should also match this--
      nudirlist = (valuebuf[valbuflen - 1] == ']') ? 'y' : 'n';
      nucmdlist = nudirlist;
      if (nudirlist == 'y') {
//        if (labelindex == 0) { filenode = "LERROR"; }
        sprintf(valuebuf, "%s/-%s", cmddir, filenode);
        infilesuffix = '\0';
      }
      int lsi = -1; while (lineindex[++lsi] != 255)
        { linenumstart = (linenumstart * 10) + lineindex[lsi]; }
      linenumber = linenumstart;
      if (action == 'l') {
        if (debugmode == 1) { sprintf(inlinebuf, "%s", querystr); }
        else { linesum = basecodestringchecksum(querystr, inlinebuf, &linecursor); }
// probably unnecessary __
        int nii = -1;
        if (inlinebuf[++nii] == '(') {
          if (inlinebuf[++nii] == ':') {
            int *ntgt = &linenumlen;
            char nneg = '+';
            char nch = inlinebuf[++nii];
            while (nch != '\0') {
              if (nch == '-')
                { nneg = '-'; }
              else if (nch == ':' || nch == ')') {
                if (nneg == '-')
                  { *ntgt = 0 - *ntgt; }
                 nneg = '+';
                 if (nch == ':')
                   { ntgt = &linenumlim; }
                 else { break; }
              } else if (nch >= '0' && nch <= '9') {
                *ntgt = (*ntgt * 10) + (nch - '0');
              }
              nch = inlinebuf[++nii];
            }
          }
        }
/*
          int newinbex = 0;
          int nii = 0; while (inlinebuf[++nii] != '\0') {
            if (inlinebuf[nii] >= '0' && inlinebuf[nii] <= '9')
              { newinbex = (newinbex * 10) + (inlinebuf[nii] - '0'); }
            else if (inlinebuf[nii] == ')')
              { inbex = newinbex; break; }
            else { break; }
          }
        }
// probably unnecessary ^^
        int lli = -1; while (oldsum[++lli] != 255)
          { linenumlen = (linenumlen * 10) + oldsum[lli]; }
        int lii = -1; while (newsum[++lii] != 255)
          { linenumlim = (linenumlim * 10) + newsum[lii]; }
*/ // hmmm wut was that.. maybe params
        inlinebuf[0] = '\0';
        linenumstop = linenumstart + linenumlen - 1;
        if (linenumlim >= linenumstart && linenumstop > linenumlim)
          { linenumstop = linenumlim; }
      } else { linenumstop = linenumstart; }
      indexed = (tlv != 'x' && labelindex != 0) ? 1 : 0;
      if (nudirlist == 'y' || nucmdlist == 'y') { infilesuffix = '\0'; }
      else { infilesuffix = tablecheckcounter(NUCOUNTABLE, source, filenode); }
      if (action == 's') {
        outfilesuffix = '@';
        inlinebuf[0] = '\0';
      } else {
        if (nudirlist == 'y' || nucmdlist == 'y') { outfilesuffix = '0'; }
        else if (action == 'a' || action == 'r' || action == 'd') {
          outfilesuffix = tablecountcounter(NUCOUNTABLE, source, filenode);
        } else { outfilesuffix = infilesuffix; }
        if (querystr[0] != '\0' && (action == 'a' || action == 'r')) {
          if (debugmode == 1) { sprintf(inlinebuf, "%s", querystr); }
          else { linesum = basecodestringchecksum(querystr, inlinebuf, &linecursor); }
        } else { inlinebuf[0] = '\0'; }
      }
    }
    // -- write pathnames, open files
    // valuebuf contains the dir ... copy it and add the filename
    IF (infilesuffix EQNUL) { // NUL if opening a file
      STRF(inpathname, "%s", valuebuf); // value includes /dir/
    } EL { // reading from a temporary file
      STRF(inpathname, "%s/", vardir); // dirbuf
      CS inpn = &inpathname[STRLEN(inpathname)];
      copynopath(inpn, valuebuf);
      inpn = &inpathname[STRLEN(inpathname)];
      STRF(inpn, ".%c", infilesuffix);
    } // inputfile at /dir/source or dirbuf/-dir-source.n
    IF (outfilesuffix == '@' && newsum[1] == 63) { // save file on @@
      STRF(outpathname, "%s", valuebuf); // value includes /dir/
    } EL { // writing to a temporary file
      STRF(outpathname, "%s/", vardir); // dirbuf
      CS outpn = &outpathname[STRLEN(outpathname)];
      copynopath(outpn, valuebuf); // /dir/ becomes -dir-
      outpn = &outpathname[STRLEN(outpathname)];
      STRF(outpn, ".%c", outfilesuffix);
//      sprintf(outpathname, "%s.%c", valuebuf, outfilesuffix);
    } // outputfile at /dir/target or dirbuf/-dir-target.n
    // ^ ^ writing inpathname and outpathname !
    if (valuebuf[0] != '\0') {
      infp = fopen(inpathname, "r");
      if (infp == NULL) {
        infp = fopen(inpathname, "a");
        if (infp != NULL) { // file created
//          fputc('\n', infp);
          fclose(infp);
          infp = fopen(inpathname, "r");
        }
      }
      if (infp && (fseek(infp, 0, SEEK_END) == 0))
        { filesize = ftell(infp); fclose(infp); }
      if (infp && action != '-' && action != '!') { // oslard - all write
        infp = fopen(inpathname, "r");
        if (action == 'l') { outfp = NULL; }
        else { outfp = fopen(outpathname, "w"); }
      }
    }
    // -- start output structure
    fprintf(stdout, "<pre>{ \"tokn\": \"\"\n");
    fprintf(stdout, ", \"node\": \"%s\"\n", (action == 'o') ? labelbuf : filenode);
    fprintf(stdout, ", \"path\": \"%s\"\n", inpathname);
    if (action == 's' && outfilesuffix == '@' && newsum[1] == 63)
      { fprintf(stdout, ", \"file\": \"%c://%d/%s\"\n", source, labelindex, valuebuf); }
    else { fprintf(stdout, ", \"file\": \"%c://%d/%s%c%c\"\n", source, labelindex, valuebuf, '.', outfilesuffix); }
    // -- read input / write output file
    int success = 0;
    int linestart = -1, linestop = -1;
    if (infp && (outfp || action == 'l')) {
      int linestopchar = '\n';
      int bytei = -1, bytelimit = 134217728; // 128MB
      int linei = 1;
      int chi = -1, chj = -1;
      if (linenumber == 1) {
        linestart = 0;
        if (action == 'a' || action == 'r') {
          if (action == 'a')
            { linestop = 0; }
          if (action != 'l') {
            while (inlinebuf[++chi] != '\0')
              { fputc(inlinebuf[chi], outfp); }
            if (action == 'a')
              { fputc('\n', outfp); }
          }
        }
      }
      int inch = '\0';
      char ch = '\0';
      char lastch = '\0';
      while (++bytei < bytelimit) {
        inch = fgetc(infp);
        if (inch == EOF) {
          if (linestart != -1 && linestop == -1)
            { linestop = bytei; linestopchar = EOF; }
          if (lastch == '\n') { // || tagline == 'y') {
            if (action == 'l')
              { fprintf(stdout, ", \"elof\": \"%d\"\n", linei); }
//            if (tagline == 'y')
//              { fprintf(stdout, ", \"tags\": \"yes\"\n"); }
            break;
          } else { inch = '\n'; } // force end with newline
        }
//        } else if (lastch == '\n' && ch == '.') { tagline = 'y'; }
//        } else if (lastch == '\n' && ch != '.') { tagline = 'n'; }
        ch = (char)inch;
        if (ch == '\n') {
 //         if (action == 'd' && linenumber == 1)
 //           { linestop = bytei; }
          if (++linei == linenumber) {
            if (linestart == -1) {
              linestart = bytei + 1;
              if (action == 'a' || action == 'r') {
                if (action == 'a')
                  { linestop = bytei + 1; }
                if (action != 'l') {
                  fputc('\n', outfp);
                  while (inlinebuf[++chj] != '\0')
                    { fputc(inlinebuf[chj], outfp); }
                }
              }
            }
          } else if (linestart != -1 && linestop == -1) {
            if (action == 'l' && linenumber < linenumstop) {
              fprintf(stdout, ", \"line%d\": \"", linenumber);
              writebufhighlight(outlinebuf, 1);
              fprintf(stdout, "\"\n");
              outlinebuf[0] = '\0';
              outlinebufi = 0;
              linenumber++;
            } else {
              linestop = bytei;
              if (action == 'l') {
                fprintf(stdout, ", \"line%d\": \"", linenumber);
                writebufhighlight(outlinebuf, 1);
                fprintf(stdout, "\"\n");
              }
              if (action == 'd' && linenumber == 1)
                { lastch = ch; continue; }
            }
          }
        }
        if (linestart == -1 && linestop == -1) {
          if (action != 'l') { fputc(ch, outfp); }
        } else if (linestart != -1 && linestop != -1) {
          if (action != 'l') { fputc(ch, outfp); }
        } else if (linestart != -1 && linestop == -1) {
          if (action != 'l' && action != 'r' && action != 'd')
            { fputc(ch, outfp); }
          if (ch != '\n' && outlinebufi < TABTIBLINESIZE) {
            outlinebuf[outlinebufi] = ch;
            outlinebuf[++outlinebufi] = '\0';
          }
        }
        lastch = ch;
      }
      if (action != 'l')
        { fclose(outfp); }
      fclose(infp);
      success = 1;
    } else if (infp) { fclose(infp); }
    else if (outfp) { fclose(outfp); }
    // -- continue output structure
    if (action == 'a' || action == 'r') {
      fprintf(stdout, ", \"line\": \"");
      writebufhighlight(inlinebuf, 1);
      fprintf(stdout, "\"\n");
//    } else if (action == 'a') {
//      // content of new line if splitting
    }
//    if (action == 'l') {
//      if (inbex != -1) {
    if (action == 'o' && nucmdlist == 'y') { action = 'c'; }
    if (action == 'o' && nudirlist == 'y') { action = 'c'; } // 'd' when supported - tree mode?
    fprintf(stdout, ", \"seek\": \"%c@%d:%d:%d(%s) [1:[%d:%d]:%d]\"\n"
      , action, linenumstart, linenumlen, linenumlim
      , context, linestart, linestop, filesize);
//      } else {
//        fprintf(stdout, ", \"qstr\": \"%s\"\n", querystr);
//        fprintf(stdout, ", \"seek\": \"%c@%d:%d:%d [1:[%d:%d]:%d]\"\n"
//          , action, linenumstart, linenumlen, linenumlim, linestart, linestop, filesize);
//      }
//    } else {
//      fprintf(stdout, ", \"seek\": \"%c@%d [1:[%d:%d]:%d]\"\n"
//        , action, linenumber, linestart, linestop, filesize);
//    }
    fprintf(stdout, ", \"chek\": \"%c%c | %c%c %d %d\"\n",
      base62[oldsum[0]], base62[oldsum[1]], base62[newsum[0]], base62[newsum[1]], newsum[0], newsum[1]);
    fprintf(stdout, ", \"result\": \"%s\" }</pre>\n", (success) ? "success" : "failure");
    fprintf(stdout, "</body></html>\n");
    fflush(stdout);
    // -- re-read output
/*
    if (outfp) {
      int inch = '\0'; char ch = '\0';
      int byteoi = -1, byteolimit = 134217728; // 128MB
      outfp = fopen(outpathname, "r");
      if (outfp) {
        fprintf(stdout, "------------<br>\n");
        while (++byteoi < byteolimit) {
          inch = fgetc(outfp);
          if (inch == EOF) { break; }
          if (inch == '\n') { fprintf(stdout, "<br>"); }
          else if (inch < 32 || inch > 126) { fputc('#', stdout); }
          else { fprintf(stdout, "%c", inch); }
        }
        fprintf(stdout, "------------<br>\n");
      }
      fclose(outfp);
    }
*/
  }
  RT 0;
}
