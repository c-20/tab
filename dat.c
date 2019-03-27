#include "/tau/src/cmd/code.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "copy.h"
#include "table.h"
#include "basecode.h"
#include "shell.h"

#define DATDATURL	"/io/dat"
#define UPCACHEDIR	"/V:"
// later: DATBUFDIR "/R:"
#define UPCACHETABLE	"cache"
#define UPLABELTABLE	"label"
#define UPMATCHTABLE    "match"
#define UPWATCHTABLE    "watch"
#define UPLINEBUFSIZE	1024
#define UPMAXEXTLEN	32

//#define dat main
//IN dat($) {
int main(int argc, char **argv) {
  char pathbuf[TABLEMAXVALUESIZE + 1];
  char valuebuf[TABLEMAXVALUESIZE + 1];
  char labelbuf[TABLEMAXLABELSIZE + 1];
  char cmdlabelbuf[2 + TABLEMAXLABELSIZE + 1];
  char fnodebuf[TABLEMAXLABELSIZE + 1];
  char flinkbuf[TABLEMAXLABELSIZE + 1];
  char fnamebuf[TABLEMAXVALUESIZE + 1];
  char linebuf[UPLINEBUFSIZE + 1];
  char queryext[UPMAXEXTLEN + 1];
  char *range = getenv("HTTP_RANGE");
  char *gateway = getenv("GATEWAY_INTERFACE");
  char *querystr = getenv("QUERY_STRING");
  if (!gateway && argc > 1)
    { querystr = argv[1]; }
  int qstrlen = strlen(querystr);
  char *clabelstr = "";
  char htmlmode = 'n';
  char filemode = 'n';
  char pxpxmode = 'n';
  char loopmode = 'n';
  char underspace = 'y';
  char noheader = 'n';
  int iboxnum = 0;
  int linenum = 0;
  char qsprefix = querystr[0];
  char *filename = &querystr[1];
  if (qsprefix == ':') {
    noheader = 'y';
    querystr = &querystr[1];
    qsprefix = querystr[0];
    filename = &querystr[1];
  }
  char nodemode = 'n';
  char *qsdext = "";
  if (querystr[1] == qsprefix) {
    nodemode = 'y';
    filename = &querystr[2];
    if (filename[8] == '.' || filename[8] == '\0') {
      if (filename[8] == '.')
        { qsdext = &filename[9]; }
      filename[8] = '\0';
      sprintf(labelbuf, "%s", filename);
    } else { sprintf(labelbuf, "00000405"); }
  } // if prefix repeated -- == __, use filename as filenode - i.e. enable table lookup
  // should I be using single prefix for fetch?
  querystr = filename; // ditch prefix ... safe?
  qstrlen = strlen(querystr);
  char activeline = 'p';
//#include BUFLISTDIRSHEADER
//  char *cmddirbuf = buflistdirs[10];
  CS cmddirbuf = UPCACHEDIR;
  char commandmode = 'n';
  char *linevalue = NULL;
  char commandlbl = 'n';
  char *commandbuf = NULL;
  char *commandend = NULL;
  char commandendch = '\0';
  char commandrun[101];
  commandrun[0] = 'p';
  char commandnum = 0;
  int rangestarsecs = -1;
  int rangestarsubs = 0;
  int rangestopsecs = -1;
  int rangestopsubs = 0;
  int timestarsecs = -1;
  int timestarsubs = 0;
  int timestopsecs = -1;
  int timestopsubs = 0;
  if (qsprefix == '-' || qsprefix == '=' || qsprefix == '_' || qsprefix == ':') {
    // -file, =file, _file (raw, html, download)
    filemode = (qsprefix == '=') ? 'n' : 'y';
    htmlmode = (qsprefix == '=') ? 'y' : 'n';
    // ^^ when this is set later, it doesn't consider =NODE.EXT valid
    char tvl = tablevaluelabel(labelbuf, UPLABELTABLE, '0', filename);
//fprintf(stdout, "FM %c FN %s LB %s\n", filemode, filename, labelbuf);
    // IF TWO PREFICES && filename[8] == '.' || '\0' 
    if (tvl == 'l') {
      char tvm = tablelabelvalue(valuebuf, UPMATCHTABLE, '0', labelbuf);
      if (tvm != 'v' && underspace == 'y') {
        int fni = strlen(filename);
        while (--fni >= 0) {
          if (filename[fni] == '_')
            filename[fni] = ' ';
        } // replace underscore with space and try again
        tvl = tablevaluelabel(labelbuf, UPLABELTABLE, '0', filename);
        if (tvl == 'l')
          { tvm = tablelabelvalue(valuebuf, UPMATCHTABLE, '0', labelbuf); }
      } // allow hello_this_is_a_file to be hello this is a file
      if (tvm == 'v' && valuebuf[8] == '\0') {
        sprintf(&valuebuf[8], "[%s]", filename);
        sprintf(querystr, "%s", valuebuf);
        filename = &querystr[9];
        htmlmode = 'y';
      } else {
// SET ALTERNATE DEBUG/RESULT FLAG
//        fprintf(stdout, "label not matched: %s [%s %c %s]", filename, labelbuf, tvm, valuebuf);
      }
    } else {
// SET ALTERNATE DEBUG/RESULT FLAG
//      fprintf(stdout, "label not found: %s", filename);
    }
    qstrlen = strlen(querystr);
  } else if (qsprefix == '+' || qsprefix == '@') {
    // basecode string - @linenum:
    int curpos = -1;
    int outsum = 0;
    if (qsprefix == '+') {
      outsum = basecodestringchecksum(filename, linebuf, &curpos);
    } else { sprintf(linebuf, "@%s", filename); }
    char linech = linebuf[0];
    char linestate = '@';
    char prevlinestate = '@';
    int startime[4];
    int startin = 4;
    while (--startin >= 0)
      { startime[startin] = 0; }
    startin = 1;
    int stoptime[4];
    int stoptin = 4;
    while (--stoptin >= 0)
      { stoptime[stoptin] = 0; }
    stoptin = 1;
    char starstop = ':';
    if (linech == '@') {
      curpos = 0;
      while (++curpos < UPLINEBUFSIZE) {
        linech = linebuf[curpos];
        if (linech == '\0') { break; }
        else if (linestate == '@' && linech >= '0' && linech <= '9')
          { iboxnum = (iboxnum * 10) + (linech - '0'); }
        else if (linestate == '@' && linech == ',')
          { linestate = ','; }
        else if (linestate == ',' && linech >= '0' && linech <= '9')
          { linenum = (linenum * 10) + (linech - '0'); }
        else if (linestate == ',' && linech == ':') {
          querystr = &linebuf[curpos + 1];
          qstrlen = strlen(querystr);
          linestate = ':'; // in line
          activeline = (linebuf[curpos + 1] == ']') ? 'n' : 'y';
          linevalue = &linebuf[curpos + ((activeline == 'n') ? 1 : 0)];
        } else if (linestate == ':' && linech == '%') {
          linestate = '%';
        } else if (linestate == '%' && linech == '/') {
          querystr = &linebuf[curpos + 1];
          qstrlen = strlen(querystr);
          filename = &querystr[9]; // unused?
          linestate = '/';
//          break;
          // REPLACE WITH EARLIER CALL TO shellsrcnode() ..
        } else if ((linestate == ':' || linestate == '/') && linech == '<') {
          prevlinestate = linestate; linestate = '<';
        } else if (linestate == '<' && linech >= '0' && linech <= '9') {
          startime[startin] = (startime[startin] * 10) + (linech - '0');
        } else if (linestate == '<' && (linech == ':' || linech == '.')) {
          startin++; if (linech == '.' || startin > 3) { startin = 0; }
        } else if (linestate == '<' && (linech == '/' || linech == '-')) {
          linestate = '>'; starstop = linech;
        } else if (linestate == '>' && linech >= '0' && linech <= '9') {
          stoptime[stoptin] = (stoptime[stoptin] * 10) + (linech - '0');
        } else if (linestate == '>' && (linech == ':' || linech == '.')) {
          stoptin++; if (linech == '.' || stoptin > 3) { stoptin = 0; }
        } else if (linestate == '>' && linech == '>') {
          int starsecs = startime[1];
          int starsubs = startime[0];
          int stopsecs = stoptime[1];
          int stopsubs = stoptime[0];
          int thistin = 1;
          while (++thistin <= 3) {
            if (thistin <= startin)
              { starsecs = (starsecs * 60) + startime[thistin]; }
            if (thistin <= stoptin)
              { stopsecs = (stopsecs * 60) + stoptime[thistin]; }
          }
          if (starstop == '-') {
            rangestarsecs = starsecs;
            rangestarsubs = starsubs;
            rangestopsecs = stopsecs;
            rangestopsubs = stopsubs;
          } else if (starstop == '-') {
            timestarsecs = starsecs;
            timestarsubs = starsubs;
            timestopsecs = stopsecs;
            timestopsubs = stopsubs;
          }
          startin = 4;
          while (--startin >= 0)
            { startime[startin] = 0; }
          startin = 1;
          stoptin = 4;
          while (--stoptin >= 0)
            { stoptime[stoptin] = 0; }
          stoptin = 1;
          linestate = prevlinestate;
        } else if (activeline == 'y' && linech == '~') {
          commandmode = 'y';
        } else if (commandmode == 'y' && commandrun[0] == 'p') {
          commandnum++;
          commandbuf = &linebuf[curpos];
          commandend = commandbuf;
          while (commandend[0] != ':' && commandend[0] != '~' && commandend[0] != '\0')
            { commandend++; }
          commandendch = commandend[0];
          commandend[0] = '\0';
          if (commandnum == 1)
            { commandlbl = tablegenvaluelabel(&cmdlabelbuf[2], linevalue); }
          cmdlabelbuf[1] = (commandnum % 10) + '0';
          cmdlabelbuf[0] = ((cmdlabelbuf[1] - '0') / 10) + '0';
//          commandlbl = tablegenvaluelabel(&cmdlabelbuf[2], commandbuf);
          if (0 AND commandlbl == 'l') { // disabled, use shellexec
// instead of shellexec, use shell... which should be a recursive command
// interpreter that understands $() syntax
//            commandrun[commandnum] = shellcommand(commandbuf, linevalue, cmdlabelbuf); // should pass linevalue
            if (commandendch == ':') {
              // save first line of output and report to tab via dat message @9,4~progress: 49%
              // or report specific error ... failed to execute command
            }
          } else {
            commandrun[commandnum] = 'n';
            if (commandendch == ':') {
              // report global error to tab, .. failed to generate label
            }
          }
          commandmode = 'n';
        }
      }
    } else {
      // no @linenum: header? try without?
//      querystr = &linebuf[0]; // remember, no prefix
      querystr = linevalue;
      qstrlen = strlen(querystr);
    }
//    if (linestate == '/') {
    htmlmode = 'y';
//    } else if (linestate != '/') {
      // querystr will be set from after @n:
//      querystr = &linebuf[0];
//      qstrlen = strlen(querystr);
//    }
  }
  // scan for cache marker
  fnodebuf[0] = '\0';
  char qsdelim = querystr[8]; // == filename[8] if ! +
//  if (commandmode == 'y') { // command overrides cache
//    if (commandrun == 'y') {
//      sprintf(fnodebuf, "%s", labelbuf);
//    } else { sprintf(fnodebuf, "10101010"); }
//  } else if (qstrlen > 8 && (qsdelim == '[' || qsdelim == '=')) {
  if (qstrlen > 8 && (qsdelim == '[' || qsdelim == '=')) {
    htmlmode = 'y';
    querystr[8] = '\0';
    sprintf(fnodebuf, "%s", querystr);
    clabelstr = &querystr[9];
    if (strncmp(clabelstr, "file;", 5) == 0)
      { clabelstr = &clabelstr[5]; filemode = 'y'; }
    if (strncmp(clabelstr, "pxpx;", 5) == 0)
      { clabelstr = &clabelstr[5]; pxpxmode = 'y'; }
    if (strncmp(clabelstr, "loop;", 5) == 0)
      { clabelstr = &clabelstr[5]; loopmode = 'y'; }
    if (qsdelim == '=' || qsdelim == '[') {
//      filemode = (qsdelim == '=') ? 'y' : 'n';
      if (qsdelim == '[') {
        char *qsdend = &querystr[9];
        while (*qsdend != ']' && *qsdend != '\0') {
          if (*qsdend == '.')
            { qsdext = qsdend + 1; }
          // TODO: check for invalid filename chars
          qsdend++;
        } // left to right until .ext
        if (*qsdend == ']')
          { *qsdend = '\0'; }
      } // retrieve extension if filename given
      char tva = tablevaluelabeladdrow(labelbuf, UPLABELTABLE, '0', clabelstr);
      if (tva == 'a' || tva == 'e') { // tablegetaddrow(valbuf, ...
        char tvb = 0;
        if (tva == 'e') { // uses table A test for table B .. not safe/ideal
              // test MATCHTABLE == 'e' also
          tvb = tablesetrow(UPMATCHTABLE, '0', labelbuf, querystr);
        } else {
          tvb = tableaddrow(UPMATCHTABLE, '0', labelbuf, querystr);
        }
        if (tvb == 'a' || tvb == 's') {
          char *status = (tvb == 'a') ? "new pointer" : "replaced pointer";
          char tvc = tableaddrow(UPWATCHTABLE, '0', labelbuf, status);
          if (tvc == 'a') { }
        }
      }
    }
//    if (qsdext)
//      { sprintf(&querystr[8], ".%s", qsdext); }
    qstrlen = strlen(querystr);
  } else if (qstrlen > 8 && qsdelim == '.') { // NODE.EXT
    qsdext = &querystr[9];
    querystr[8] = '\0';
    sprintf(fnodebuf, "%s", querystr);
    querystr[8] = '.';
    qstrlen = strlen(querystr); // should be the same
  } else if (qstrlen == 8) { // NODE
    sprintf(fnodebuf, "%s", querystr);
  } else {
    sprintf(fnodebuf, "00000000");    
  }
  char *oldqstr = "(null)";
//  char fnodebuflen = strlen(fnodebuf);
//  if (fnodestrlen != 8) { oldqstr = querystr; querystr = "00000000"; }
//  if (fnodebuflen != 8) { oldqstr = querystr; querystr = "00000000"; } // error 0 querystr
  // all fully numeric indices assumed png ?
  if (commandmode == 'y') {
    sprintf(valuebuf, "(text/output)");
  } else {
//  fprintf(stdout, "%s -> %s [.%s] %c\n", fnodebuf, valuebuf, qsdext, tlv);
    char tlv = tablelabelvalue(valuebuf, UPCACHETABLE, '0', fnodebuf); //querystr);
    if (tlv != 'v' || qsdext[0] != '\0') { // extension overrides mimetype
      char *exttype = "text/plain";
      if (qsdext[0] != '\0') {
//        if (strcmp(qsdext, "html@") == 0)
//          { exttype = "text/html"; }
        if (strcmp(qsdext, "mp4") == 0)
          { exttype = "video/mp4"; }
        else if (strcmp(qsdext, "mpeg") == 0)
          { exttype = "video/mpeg"; }
        else if (strcmp(qsdext, "mpg") == 0)
          { exttype = "video/mpeg"; }
        else if (strcmp(qsdext, "avi") == 0)
          { exttype = "video/avi"; }
        else if (strcmp(qsdext, "mp3") == 0)
          { exttype = "audio/mp3"; }
        else if (strcmp(qsdext, "mid") == 0)
          { exttype = "audio/midi"; }
        else if (strcmp(qsdext, "wav") == 0)
          { exttype = "audio/wav"; }
        else if (strcmp(qsdext, "jpeg") == 0)
          { exttype = "image/jpeg"; }
        else if (strcmp(qsdext, "jpg") == 0)
          { exttype = "image/jpeg"; }
        else if (strcmp(qsdext, "png") == 0)
          { exttype = "image/png"; }
        else if (strcmp(qsdext, "gif") == 0)
          { exttype = "image/gif"; }
        else if (strcmp(qsdext, "zip") == 0)
          { exttype = "application/x-compressed"; }
        else if (strcmp(qsdext, "tar") == 0)
          { exttype = "application/x-compressed"; }
        else if (strcmp(qsdext, "gz") == 0)
          { exttype = "application/x-compressed"; }
        else if (strcmp(qsdext, "c") == 0)
          { exttype = "text/plain"; }
        else if (strcmp(qsdext, "h") == 0)
          { exttype = "text/plain"; }
        else if (strcmp(qsdext, "sh") == 0)
          { exttype = "text/plain"; }
        else if (strcmp(qsdext, "txt") == 0)
          { exttype = "text/plain"; }
        else if (strcmp(qsdext, "html") == 0)
          { exttype = "text/html"; }
        else if (strcmp(qsdext, "css") == 0)
          { exttype = "text/css"; }
        else if (strcmp(qsdext, "js") == 0)
          { exttype = "text/javascript"; }
        else if (strcmp(qsdext, "m") == 0)
          { exttype = "text/plain"; }
      }
      sprintf(valuebuf, "(%s)", exttype);
    }
  }
  int vsi = strlen(valuebuf);
  char vsmode = '-';
  char *ctypestr = "text/plain";
  while (--vsi >= 0) {
    if (vsmode == '-' && valuebuf[vsi] == ')') {
      valuebuf[vsi] = '\0';
      vsmode = '+';
    } else if (vsmode == '+' && valuebuf[vsi] == '(') {
      valuebuf[vsi] = '\0';
      ctypestr = &valuebuf[vsi + 1];
      vsmode = '=';
      break;
    }
  }
  char ctype = 't'; // imply text by default
  if (strncmp(ctypestr, "application/", 12) == 0) { ctype = 'D'; }
  if (strncmp(ctypestr, "video/", 6) == 0) { ctype = 'V'; }
  if (strncmp(ctypestr, "audio/", 6) == 0) { ctype = 'A'; }
  if (strncmp(ctypestr, "image/", 6) == 0) { ctype = 'I'; }
  if (strncmp(ctypestr, "text/", 5) == 0) { ctype = 'T'; }
// ADD APPLICATION TYPE
// need to save .c as text/plain instead ... override:
//  if (strncmp(ctypestr, "application/", 12) == 0) { ctype = 'T'; }
//  if (ctype == '-') { htmlmode = 'n'; }
//  if (ctype == '-') { ctype = 'T'; }

//  if (filemode == 'y') { htmlmode = 'n'; }
  if (htmlmode == 'y') { filemode = 'n'; }

  int rangestart = 0;
  int rangestop = -1;
  char rstat = 'x';
  if (range) {
    if (strncmp(range, "bytes=", 6) == 0) {
      rstat = '+';
      char *rch = &range[6];
      while (*rch != '\0') {
        if (*rch >= '0' && *rch <= '9') {
          if (rstat == '+') {
            rangestart = (rangestart * 10) + (*rch - '0');
          } else if (rstat == '-') {
            rstat = '=';
            rangestop = 0 + (*rch - '0');
          } else if (rstat == '=') {
            rangestop = (rangestop * 10) + (*rch - '0');
          }
        } else if (*rch == '-' && rstat == '+') {
          rstat = '-';
        } else { break; }
        rch++;
      }
    }
  }
  if (qsdext[0] != '\0') {
    sprintf(queryext, ".%s", qsdext);
  } else if (ctype == 't') {
    sprintf(queryext, ".txt");
  } else { queryext[0] = '\0'; }

  if (commandmode == 'y') {
    sprintf(pathbuf, "%s/%s", cmddirbuf, fnodebuf);
  } else {
    sprintf(pathbuf, "%s/%s", UPCACHEDIR, fnodebuf); // querystr);
  }

/////////////////////////////////////////////================

  int rangelength = 0;
  int totallength = 0;
  FILE *ftest = fopen(pathbuf, "r");
  if (ftest && fseek(ftest, 0, SEEK_END) == 0) {
    totallength = ftell(ftest);
    fclose(ftest);
  } else {
    if (ftest) { fclose(ftest); }
    totallength = 0;
  }
  if (rstat != 'x') {
    if (rangestart >= totallength || rangestart < 0)
      { rangestart = totallength; } // 9:9 == 1
    if (rangestop >= totallength || rangestop < 0)
      { rangestop = totallength - 1; } // 10:9 == 0
    rangelength = rangestop - rangestart + 1;
  } else {
    rangestop = totallength - 1;
    rangelength = rangestop - rangestart + 1;
  }
  if (rstat != 'x')
    { fprintf(stdout, "Status: 206 Partial Content\r\n"); }
  if (filemode == 'y' && qsprefix == '_') {
    int labels = tablevaluelabelmatch(flinkbuf, 1, UPMATCHTABLE, '0', fnodebuf);
    if (labels == 1) { // - if no match label, -- if match but no label value
      char dfn = tablelabelvalue(fnamebuf, UPLABELTABLE, '0', flinkbuf);
      if (dfn != 'v') { sprintf(fnamebuf, "%c-%s--%s%s", dfn, fnodebuf, flinkbuf, queryext); }
    } else { sprintf(fnamebuf, "%d-%s-%s", labels, fnodebuf, queryext); }
    if (noheader == 'n')
      { fprintf(stdout, "Content-Disposition: attachment; filename=\"%s\"\r\n", fnamebuf); }
  }
  if (noheader == 'n')
    { fprintf(stdout, "Content-Type: %s\r\n", (htmlmode == 'y') ? "text/html" : ctypestr); }
  if (htmlmode != 'y' && noheader == 'n')
    { fprintf(stdout, "Content-Length: %d\r\n", rangelength); }
  if (noheader == 'n')
    { fprintf(stdout, "Cache-Control: max-age=0, must-revalidate\r\n"); }
  if (noheader == 'n')
    { fprintf(stdout, "Content-Encoding: identity\r\n"); }
  if (noheader == 'n')
    { fprintf(stdout, "Accept-Ranges: bytes\r\n"); }
  if (rstat != 'x' && noheader == 'n')
    { fprintf(stdout, "Content-Range: bytes %d-%d/%d\r\n", rangestart, rangestop, totallength); }
  if (noheader == 'n')
    { fprintf(stdout, "Access-Control-Allow-Origin: *\r\n"); }
  if (noheader == 'n')
    { fprintf(stdout, "Pragma: no-cache\r\n\r\n"); }
  if (htmlmode == 'y') {
    fprintf(stdout, "<html><head><title>%s</title>\n", ctypestr);
    fprintf(stdout, "<style type=\"text/css\">\n");
    fprintf(stdout, "  body { background-color: #000; overflow: auto;\n");
    fprintf(stdout, "         -webkit-overflow-scrolling: touch; }\n");
//    fprintf(stdout, "  #container { position: fixed; background-color: #000;\n");
//    fprintf(stdout, "    left: 2px; right: 2px; top: 2px; bottom: 32px;\n"); // max-height: 85%;\n");
//    fprintf(stdout, "    text-align: center; overflow: auto;\n");
//    fprintf(stdout, "    border: #777 solid 2px; border-radius: 8px; }\n");
//    fprintf(stdout, "  #container > * { position: absolute; top: 0; bottom: 0;\n");
//    fprintf(stdout, "    left: 0; right: 0; margin: auto; }\n");
//    fprintf(stdout, "    padding-left: 1%; padding-right: 1%;\n");
//    fprintf(stdout, "  #container > video { min-width: 50%; max-width: 98%; overflow: hidden; }\n");
////    fprintf(stdout, "  #container { position: absolute; bottom: 0; left: 0; }\n");
    fprintf(stdout, "  #container > div { text-align: center; line-height: 96px; font-size: 14px;\n");
    fprintf(stdout, "    color: #fc3; background-color: #002; text-shadow: 1px 1px 0px #444; font-family: monospace; }\n");
    fprintf(stdout, "  #container > video { max-width: 100%; max-height: 100%; }\n");
    fprintf(stdout, "  #container > audio { width: 100%; height: 100px; }\n");
    fprintf(stdout, "  #container > img { max-width: 100%; max-height: 100%; }\n");
    fprintf(stdout, "  #container > iframe { position: relative; }\n"); // max-width: 100%; max-height: 100%; overflow: auto; }\n");
    fprintf(stdout, "  #output { position: fixed; background-color: rgba(0, 0, 0, 0.25); opacity: 0.75;\n");
    fprintf(stdout, "    top: 32px; right: 32px;\n");
    fprintf(stdout, "    border: #090 solid 1px; border-radius: 8px; }\n");
    fprintf(stdout, "  #output > div { border-left: #0c0 solid 1px; padding-left: 16px; padding-right: 16px;\n");
    fprintf(stdout, "    font-family: monospace; background-color: rgba(0, 255, 0, 0.1); border-radius: 4px;\n");
    fprintf(stdout, "    color: #0f0; font-size: 14px; }\n");
//    fprintf(stdout, "  #container > audio { padding-bottom: 16px; padding-top: 16px; height: 32px; }\n");
//    fprintf(stdout, "    top: 50%; left: 50%; transform: translate(-50%, -50%); }\n");
//    fprintf(stdout, "  @media screen and (max-device-width: 480px) {\n");
//    fprintf(stdout, "    #container { margin-left: -50%; margin-top: -40%; } }\n");
    fprintf(stdout, "  #label { z-index: 2; position: absolute; background-color: #222; font-weight: bold;\n");
    fprintf(stdout, "    top: 0; left: 25%; right: 25%; text-align: center;\n"); // ; transform: translate(-50%);\n");
    fprintf(stdout, "    padding: 1px; border-radius: 5px; border-left: #fc3 solid 1px;\n");
    fprintf(stdout, "    border-right: #fc3 solid 1px; color: #fff; font-family: monospace; font-size: 12px; }\n");
    fprintf(stdout, "  #label:hover { cursor: pointer; color: #fc3; }\n");
    fprintf(stdout, "  #labeltext { position: relative; z-index: 4; }\n");
    fprintf(stdout, "  #percent { z-index: 3; position: absolute; top: 0; bottom: 0;\n");
    fprintf(stdout, "    left: 0; background-color: #038; border-radius: 12px; }\n");
//    fprintf(stdout, "  #label:hover > #percent { background-color: #02e; }\n");
    fprintf(stdout, "  #dlt { position: absolute; bottom: 0; right: 0;\n");
    fprintf(stdout, "    width: 5px; height: 5px; border: none; visibility: hidden; }\n");
    fprintf(stdout, "</style><script type=\"text/javascript\">");
    fprintf(stdout, "  function $$(id) { return document.getElementById(id); }\n");
    fprintf(stdout, "  var inrangestart = %d.%d;\n", rangestarsecs, rangestarsubs);
    fprintf(stdout, "  var inrangestop = %d.%d;\n", rangestopsecs, rangestopsubs);
    fprintf(stdout, "  var intimestart = %d.%d;\n", timestarsecs, timestarsubs);
    fprintf(stdout, "  var intimestop = %d.%d;\n", timestopsecs, timestopsubs);
    fprintf(stdout, "  var objtime = -1;\n");
    fprintf(stdout, "  var objdurn = 0;\n");
    fprintf(stdout, "  function tab(ch) {\n");
    fprintf(stdout, "    var obj = $$('dat');\n");
    fprintf(stdout, "    if (obj) {\n");
    fprintf(stdout, "      objdurn = parseInt(obj.duration);\n");
    fprintf(stdout, "      if (ch == '+') {\n");
    fprintf(stdout, "        var newtime = parseInt(obj.currentTime);\n");
    fprintf(stdout, "        var newtimef = parseFloat(obj.currentTime);\n");
    fprintf(stdout, "        if (inrangestart >= 0 && newtimef >= inrangestop) {\n");
    fprintf(stdout, "          var newmsg = 'JMP @%d,%d ' + inrangestop + ' ' + inrangestart;\n", iboxnum, linenum);
    fprintf(stdout, "          obj.currentTime = inrangestart;\n");
    fprintf(stdout, "          parent.postMessage(newmsg, '*');\n");
    fprintf(stdout, "        } else if (newtime != objtime) {\n");
    fprintf(stdout, "          var newmsg = 'CUE @%d,%d ' + newtime + ' ' + objdurn;\n", iboxnum, linenum);
    fprintf(stdout, "          parent.postMessage(newmsg, '*');\n");
    fprintf(stdout, "          objtime = newtime;\n");
    fprintf(stdout, "        }\n");
    fprintf(stdout, "      } else if (ch == '>') {\n");
    fprintf(stdout, "        var newmsg = 'CUE @%d,%d 0 ' + objdurn;\n", iboxnum, linenum);
    fprintf(stdout, "        if (intimestart >= 0) {\n");
    fprintf(stdout, "          obj.currentTime = intimestart;\n");
    fprintf(stdout, "          newmsg = 'CUE @%d,%d ' + intimestart + ' ' + objdurn;\n", iboxnum, linenum);
    fprintf(stdout, "        }\n");
    fprintf(stdout, "        parent.postMessage(newmsg, '*');\n");
    fprintf(stdout, "      } else if (ch == '|') {\n");
    fprintf(stdout, "        var newmsg = 'CUE @%d,%d | ' + objdurn;\n", iboxnum, linenum);
    fprintf(stdout, "        parent.postMessage(newmsg, '*');\n");
    fprintf(stdout, "      }\n");
    fprintf(stdout, "      objpcnt = Math.round(objtime / objdurn * 1000) / 10;\n");
    fprintf(stdout, "      $$('percent').style.width = '' + objpcnt + '%';\n");
    fprintf(stdout, "    }\n");
    fprintf(stdout, "  }\n");
    fprintf(stdout, "  function key(e) {\n");
    fprintf(stdout, "    var ekc = e.keyCode;\n");
    fprintf(stdout, "    if (ekc == 192 || ekc == 96) {\n");
    fprintf(stdout, "      parent.postMessage('KEY 96', '*');\n");
    fprintf(stdout, "    } else if (ekc == 27) {\n");
    fprintf(stdout, "      parent.postMessage('KEY 27', '*');\n");
    fprintf(stdout, "    }\n");
    fprintf(stdout, "  }\n");
    fprintf(stdout, "  function dl() {\n");
    fprintf(stdout, "    $$('dlt').src = '%s?_%s%s';", DATDATURL, fnodebuf, queryext);
    fprintf(stdout, "  }\n");
    fprintf(stdout, "  function datheight() {\n");
    fprintf(stdout, "    $$('dat').height = $$('dat').contentWindow.document.body.scrollHeight;\n");
    fprintf(stdout, "  }\n");
    fprintf(stdout, "</script></head><body onkeydown=\"key(event);\">\n");
    fprintf(stdout, "<div id=\"container\">\n");
    if (ctype == 'D') {
      fprintf(stdout, "<div id=\"dat\">");
      fprintf(stdout, "Click the filename to download...</div>");
    } else if (ctype == 'V') {
      fprintf(stdout, "<video id=\"dat\" autoplay%s", (loopmode == 'y') ? " loop" : "");
      fprintf(stdout, " ontimeupdate=\"tab('+');\" onended=\"tab('|');\"");
      fprintf(stdout, " src=\"%s?--%s%s\"></video>\n", DATDATURL, fnodebuf, queryext); // querystr, queryext);
    } else if (ctype == 'A') {
      fprintf(stdout, "<audio id=\"dat\" controls autoplay%s", (loopmode == 'y') ? " loop" : "");
      fprintf(stdout, " oncanplay=\"tab('>');\" ontimeupdate=\"tab('+');\" onended=\"tab('|');\"");
      fprintf(stdout, " src=\"%s?--%s%s\"></audio>\n", DATDATURL, fnodebuf, queryext); // querystr, queryext);
    } else if (ctype == 'I') {
      char *imgstretch = " style=\"max-width: 100%; max-height: 100%;\"";
      fprintf(stdout, "<img id=\"dat\"%s", (pxpxmode == 'n') ? imgstretch : "");
      fprintf(stdout, " src=\"%s?--%s%s\" />\n", DATDATURL, fnodebuf, queryext); // querystr, queryext);
    } else if (ctype == 'T') {
      char *ifrviewa = " style=\"background-color: #fff; border: none; border-radius: 8px;";
      char *ifrviewb = " max-width: 100%;\"";  // also in css
      fprintf(stdout, "<iframe id=\"dat\"%s%s width=\"100%\"", ifrviewa, ifrviewb);
      fprintf(stdout, " src=\"%s?--%s%s\" onload=\"datheight();\"></iframe>\n", DATDATURL, fnodebuf, queryext); // querystr, queryext);
    } else if (ctype == 't') {
      fprintf(stdout, "<div id=\"dat\">");
      fprintf(stdout, "File type not explicitly supported.</div>");    
//      char *ifrviewa = " style=\"background-color: #aaa; border: none; border-radius: 8px;\"";
//      char *ifrviewb = " max-width: 100%; max-height: 100%;\"";
//      fprintf(stdout, "<iframe id=\"dat\"%s%s width=\"100%\" height=\"100%\"", ifrviewa, ifrviewb);
//      fprintf(stdout, " src=\"dat?--%s%s\"></iframe>\n", fnodebuf, queryext); //querystr, queryext);
    } else { fprintf(stdout, "<div id=\"dat\">CTYPE %c</div>", ctype); }
    fprintf(stdout, "</div>\n");
    fprintf(stdout, "<div id=\"output\">\n");
    char cmdpathbuf[UPLINEBUFSIZE];
    int cmdi = 0;
    while (++cmdi <= commandnum) {
      cmdlabelbuf[1] = (cmdi % 10) + '0';
      cmdlabelbuf[0] = ((cmdlabelbuf[1] - '0') / 10) + '0';
      sprintf(cmdpathbuf, "%s/%s", cmddirbuf, cmdlabelbuf);
      FILE *cmdf = fopen(cmdpathbuf, "r");
      int inch = (cmdf) ? fgetc(cmdf) : 0;
      if (inch == EOF) { fprintf(stdout, "<div class=\"err\">--EMFILE[%c]--</div>\n", commandrun[cmdi]); }
      else if (!cmdf) { fprintf(stdout, "<div class=\"err\">--NOFILE[%c]--</div>\n", commandrun[cmdi]); }
      else {
        fprintf(stdout, "<div class=\"out%c%c\">", cmdlabelbuf[0], cmdlabelbuf[1]);
        while (inch != EOF) {
          if (inch == '\n') { fprintf(stdout, "<br>"); }
          else if (inch == '<') { fprintf(stdout, "&lt;"); }
          else if (inch == '>') { fprintf(stdout, "&gt;"); }
          else if (inch == '\'') { fprintf(stdout, "&apos;"); }
          else if (inch == '"') { fprintf(stdout, "&quot;"); }
          else if (inch == '\\') { fprintf(stdout, "&bsol;"); }
          else if (inch < ' ') { fprintf(stdout, "(%d)", inch); }
          else { fprintf(stdout, "%c", inch); }
          inch = fgetc(cmdf);
        }
        fprintf(stdout, "</div>\n");
        fclose(cmdf);
      }
    }
    fprintf(stdout, "</div>\n");
    if (clabelstr[0] != '\0') {
      fprintf(stdout, "<div id=\"label\" onclick=\"dl();\">\n");
      fprintf(stdout, "<div id=\"percent\"></div>\n");
      fprintf(stdout, "<iframe id=\"dlt\"></iframe>\n");
      fprintf(stdout, "<div id=\"labeltext\">%s</div></div>\n", clabelstr);
    }
    fprintf(stdout, "<script type=\"text/javascript\">\n");
    fprintf(stdout, "  \n");
    fprintf(stdout, "</script></body></html>\n");
  } else { // if filemode == 'y'
    FILE *f = fopen(pathbuf, "r");
    if (!f) {
      fprintf(stdout, "<html><head><title>404</title></head>\n");
      fprintf(stdout, "<body style=\"color: #fc3; background-color: #400; text-align: center;\">\n");
      fprintf(stdout, "</body></html>\n");
      if (rstat != 'x') { return 206; } else { return 0; }
    }
//    fprintf(stdout, "<%s><%s>", querystr, qsdext);
    int rangecount = 0;
    int inch = fgetc(f);
    while (inch != EOF) {
      if (rangecount >= rangestart) {
        if (vsmode == '=')
          { fprintf(stdout, "%c", inch); }
        else { fprintf(stdout, "(%d)", inch); }
      }
      if (rangestop >= 0) {
        if (rangecount >= rangestop)
          { break; }
      }
      inch = fgetc(f);
      rangecount++;
    }
    fclose(f);
  }
  if (rstat != 'x')
    { return 206; }
  return 0;
}

