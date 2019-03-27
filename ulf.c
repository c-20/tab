#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "copy.h"
#include "table.h"

#define BOUNDBUFSIZE	1024
#define UPCACHEDIR      "/V:"
#define UPCACHETABLE	"cache"

int main(int argc, char **argv) {
  char *querystr = getenv("QUERY_STRING");
  if (querystr[0] == '-' || querystr[0] == '\0') {
    fprintf(stdout, "Content-Type: text/html\r\n");
    fprintf(stdout, "Cache-Control: max-age=0, must-revalidate\r\n");
    fprintf(stdout, "Content-Encoding: identity\r\n");
    fprintf(stdout, "Pragma: no-cache\r\n\r\n");
    fprintf(stdout, "<html><head><title>ulf</title>\n");
    fprintf(stdout, "<style type=\"text/css\">\n");
    fprintf(stdout, "  html, body { margin: 0; padding: 0; background-color: #282; }\n");
    fprintf(stdout, "  body > div { position: absolute; top: 5%; bottom: 5%;\n");
    fprintf(stdout, "    left: 4%; right: 4%; border-radius: 24px; text-align: center; }\n");
    fprintf(stdout, "  body > div > span { display: inline-block; text-align: center; width: 75%;\n");
    fprintf(stdout, "    margin: 10px; border: #22c inset 2px; border-radius: 5px; z-index: 1; }\n");
    fprintf(stdout, "  body > div > span.pending { background-color: #3cf; }\n");
    fprintf(stdout, "  body > div > span.loading { background-color: #fc3; }\n");
    fprintf(stdout, "  body > div > span.success { background-color: #52b152; }\n");
    fprintf(stdout, "  body > div > span.failure { background-color: #b15252; }\n");
    fprintf(stdout, "  body > div > span.problem { background-color: #f00; }\n");
    fprintf(stdout, "  body > div > span > div { position: relative; z-index: 2; }\n");
    fprintf(stdout, "  body > div > span > div > div { position: absolute; top: 0; left: 0;\n");
    fprintf(stdout, "    height: 42px; background-color: #0f0; z-index: 3; }\n");
    fprintf(stdout, "  body > div > span > span { display: inline-block; position: relative;\n");
    fprintf(stdout, "    font-weight: bold; font-size: 16px; line-height: 42px; z-index: 4; }\n");
    fprintf(stdout, "</style><script type=\"text/javascript\">\n");
    fprintf(stdout, "  var hasdragged = false;\n");
    fprintf(stdout, "  function $$(id) { return document.getElementById(id); }\n");
    fprintf(stdout, "  var updragfalse = function(event) { return false; };\n");
    fprintf(stdout, "  var updragover = function(event) {\n");
    fprintf(stdout, "    var that = event.target;\n");
    fprintf(stdout, "    that.style.backgroundColor = '#fc3';\n");
    fprintf(stdout, "  };\n");
    fprintf(stdout, "  var updragaway = function(event) {\n");
    fprintf(stdout, "    var that = event.target;\n");
    fprintf(stdout, "    that.style.backgroundColor = '';\n");
    fprintf(stdout, "  };\n");
    fprintf(stdout, "  var updragdrop = function(event) {\n");
    fprintf(stdout, "    hasdragged = true;\n");
    fprintf(stdout, "    var that = event.target;\n");
    fprintf(stdout, "    var node = document.body.firstElementChild;\n");
    fprintf(stdout, "    var fi = -1;\n");
    fprintf(stdout, "    var files = event.dataTransfer.files;\n");
    fprintf(stdout, "    while (++fi < files.length) {\n");
    fprintf(stdout, "      var fdat = files[fi];\n");
    fprintf(stdout, "      fdat.date = fdat.lastModifiedDate.toString();\n");
    fprintf(stdout, "      var fnum = fi + 1;\n");
    fprintf(stdout, "      var fdel = document.createElement('span');\n");
    fprintf(stdout, "      fdel.id = 'file' + fnum;\n");
    fprintf(stdout, "      fdel.className = 'pending';\n");
    fprintf(stdout, "      var fdeld = document.createElement('div');\n");
    fprintf(stdout, "      var fdelp = document.createElement('span');\n");
    fprintf(stdout, "      fdelp.innerHTML = fdat.name;\n");
    fprintf(stdout, "      fdel.appendChild(fdeld);\n");
    fprintf(stdout, "      fdel.appendChild(fdelp);\n");
    fprintf(stdout, "      fdel.filedata = fdat;\n");
    fprintf(stdout, "      node.appendChild(fdel);\n");
    fprintf(stdout, "      node.appendChild(document.createElement('br'));\n");
    fprintf(stdout, "    }\n");
//    fprintf(stdout, "    if (that.value)\n");
//    fprintf(stdout, "      { that.style.backgroundColor = '#1c1'; }\n");
//    fprintf(stdout, "    else { that.style.backgroundColor = '#c1c'; }\n");
    fprintf(stdout, "    that.style.backgroundColor = '#1c1';\n");
    fprintf(stdout, "    return false;\n");
    fprintf(stdout, "  };\n");
    fprintf(stdout, "  var upkgo = function() {\n");
    fprintf(stdout, "    if (!hasdragged) { return false; }\n");
    fprintf(stdout, "    var loading = 0;\n");
    fprintf(stdout, "    var loadmax = 2;\n");
    fprintf(stdout, "    var acted = 0;\n");
    fprintf(stdout, "    var node = $$('node');\n"); // document.body.firstElementChild;\n");
    fprintf(stdout, "    var fdel = node;\n"); // node.firstElementChild;\n");
    fprintf(stdout, "    if (!node.value) { return; }\n");
    fprintf(stdout, "    while (fdel = fdel.nextElementSibling) {\n");
    fprintf(stdout, "      if (fdel.tagName.toLowerCase() != 'span') { continue; }\n");
    fprintf(stdout, "      if (fdel.className == 'success' && acted == 0) {\n");
    fprintf(stdout, "        parent.loadinsertline(fdel.lastElementChild.innerHTML);\n");
    fprintf(stdout, "        fdel.parentElement.removeChild(fdel);\n");
    fprintf(stdout, "        acted = 1;\n");
    fprintf(stdout, "      } else if (fdel.className == 'pending') {\n");
    fprintf(stdout, "        fdel.className = 'loading';\n");
    fprintf(stdout, "        var prog = document.createElement('div');\n");
    fprintf(stdout, "        prog.style.width = '0%';\n");
    fprintf(stdout, "        fdel.firstElementChild.appendChild(prog);\n");
    fprintf(stdout, "        var xhr = new XMLHttpRequest();\n");
    fprintf(stdout, "        xhr.progress = prog;\n");
    fprintf(stdout, "        if (xhr.upload) {\n");
    fprintf(stdout, "          xhr.upload.onprogress = function(event) {\n");
    fprintf(stdout, "            var pct = parseInt(event.loaded / event.total * 100);\n");
    fprintf(stdout, "            xhr.progress.style.width = pct + '%';\n");
    fprintf(stdout, "          };\n");
    fprintf(stdout, "          xhr.onreadystatechange = function(event) {\n");
    fprintf(stdout, "            if (xhr.readyState == 4) {\n");
    fprintf(stdout, "              var fpel = xhr.progress.parentElement.parentElement;\n");
    fprintf(stdout, "              if (xhr.status == 200 && xhr.responseText) {\n");
    fprintf(stdout, "                fpel.className = 'success';\n");
    fprintf(stdout, "                var savedata = xhr.responseText;\n");
    fprintf(stdout, "                savedata = savedata.substr(savedata.indexOf('<div>') + 5);\n");
    fprintf(stdout, "                savedata = savedata.substr(0, savedata.indexOf('</div>'));\n");
    fprintf(stdout, "                var savejson = JSON.parse(savedata);\n");
    fprintf(stdout, "                if (savejson.path) {\n");
    fprintf(stdout, "                  fpel.lastElementChild.innerHTML = savejson.path;\n");
//    fprintf(stdout, "                  parent.loadinsertline(savejson.path);\n");
    fprintf(stdout, "                } else { console.log(savedata); fpel.className = 'problem'; }\n");
    fprintf(stdout, "              } else { console.log(xhr.responseText); fpel.className = 'failure'; }\n");
    fprintf(stdout, "              xhr.progress.parentElement.removeChild(xhr.progress);\n");
    fprintf(stdout, "            }\n");
    fprintf(stdout, "          };\n");
    fprintf(stdout, "          var form = new FormData();\n");
    fprintf(stdout, "          form.append('node', $$('node').value);\n");
    fprintf(stdout, "          form.append('file', fdel.filedata);\n");
    fprintf(stdout, "          xhr.open('POST', '/~@/ulf?=', true);\n");
    fprintf(stdout, "          xhr.send(form);\n");
    fprintf(stdout, "        }\n");
    fprintf(stdout, "        acted = 1;\n");
    fprintf(stdout, "        break;\n");
    fprintf(stdout, "      } else if (fdel.className == 'loading') {\n");
    fprintf(stdout, "        acted = 1;\n");
    fprintf(stdout, "        loading++;\n");
    fprintf(stdout, "        if (loading >= loadmax)\n");
    fprintf(stdout, "          { break; }\n");
    fprintf(stdout, "      } else if (fdel.className == 'problem') {\n");
    fprintf(stdout, "        acted = -1;\n");	
    fprintf(stdout, "        break;\n");	
    fprintf(stdout, "      } else if (fdel.className == 'failure') {\n");
    fprintf(stdout, "        acted = -1;\n");	
    fprintf(stdout, "        break;\n");
    fprintf(stdout, "      }\n");
    fprintf(stdout, "    }\n");
    fprintf(stdout, "    if (acted == 0)\n");
    fprintf(stdout, "      { parent.loadcompleted(); }\n");
    fprintf(stdout, "  };\n");
    fprintf(stdout, "</script></head><body><div>\n");
    fprintf(stdout, "<input type=\"button\" value=\"   kgo   \" onclick=\"upkgo();\" /><br>\n");
//    fprintf(stdout, "<form id=\"form\" action=\"/~@/ulf?=\"");
//    fprintf(stdout, " method=\"post\" enctype=\"multipart/form-data\">");
    fprintf(stdout, "<input id=\"node\" name=\"node\" type=\"text\" value=\"\" /><br>");
/*
    fprintf(stdout, "<input id=\"file\" name=\"file\" type=\"file\"");
    fprintf(stdout, " ondragover=\"this.style.backgroundColor = '#fc3';\"");
    fprintf(stdout, " ondragleave=\"this.style.backgroundColor = '';\"");
    fprintf(stdout, " onchange=\"if (this.value) {\n");
    fprintf(stdout, " this.style.backgroundColor = '#11cc11';");
    fprintf(stdout, " $$(\'form\').submit(); }\" />");
*/
    fprintf(stdout, "</div>\n");
    fprintf(stdout, "<script type=\"text/javascript\">\n");
    fprintf(stdout, "  var body = document.body;\n");
    fprintf(stdout, "  var drop = body.firstElementChild;\n");
    fprintf(stdout, "  body.ondragover = updragfalse;\n");
    fprintf(stdout, "  body.ondragleave = updragfalse;\n");
    fprintf(stdout, "  body.ondrop = updragfalse;\n");
    fprintf(stdout, "  drop.ondragover = updragover;\n");
    fprintf(stdout, "  drop.ondragleave = updragaway;\n");
    fprintf(stdout, "  drop.ondrop = updragdrop;\n");
//    fprintf(stdout, "  document.body.onmousedown = updragfalse;\n");
//    fprintf(stdout, "  document.body.onmouseup = updragfalse;\n");
//    fprintf(stdout, "  document.body.onselectstart = updragfalse;\n");
 // function(e) { console.log('DRAGOVER'); return dragover(e); };\n");
//    fprintf(stdout, "  document.body.ondragleave = function(e) { return dragaway(e); };\n");
//    fprintf(stdout, "  document.body.onchange = function(e) { return dragdrop(e); };\n");
    fprintf(stdout, "</script>\n");
    fprintf(stdout, "</body></html>\n");
    //fflush(stdout);
// ------------------------------------------------------------------------------------------
// ==========================================================================================
  } else if (querystr[0] == '=') {
    fprintf(stdout, "Content-Type: text/html\r\n");
    fprintf(stdout, "Cache-Control: max-age=0, must-revalidate\r\n");
    fprintf(stdout, "Content-Encoding: identity\r\n");
    fprintf(stdout, "Pragma: no-cache\r\n\r\n");
    fprintf(stdout, "<html><body>\n");
    //fflush(stdout);
    char *contenttype = getenv("CONTENT_TYPE");
    char *contentlength = getenv("CONTENT_LENGTH");
    int clength = atoi(contentlength);
    int contentvalid = (strncmp(contenttype, "multipart/form-data", 19) == 0);
    int ccount = 0;
    char boundbuf[BOUNDBUFSIZE];
    int boundbufi = 0;
    boundbuf[boundbufi] = '\0';
    int boundmode = 'I';
    char boundpre = '\n'; // start at EOL anticipating boundary
    char *boundstr = strstr(contenttype, "; boundary=");
    if (boundstr) { boundstr = &boundstr[11]; }
    if (contentvalid && boundstr) {
      fprintf(stdout, "<span>\n");
      char outlabel[TABLEMAXLABELSIZE + 1];
      char outpath[TABLEMAXVALUESIZE + 1];
      char nodelabel[TABLEMAXLABELSIZE + 1];
      int nodeli = 0;
      nodelabel[nodeli] = '\0';
      FILE *outfile = stdout;
      char *fnamestr = "/dev/null";
      char *fnameend = "";
      char *ctypestr = "text/plain";
      char *fnodestr = "";
      int boundix = 0;
      int boundextra = 0;
      int lastinch = '\0';
      int ctotalpct = 0;
      int inch = fgetc(stdin);
      ccount++;
      ctotalpct = ccount * 100 / clength;
      while (inch != EOF) {
// fprintf(stdout, "(%d)", inch);
        if (inch == '\r') {
          if (boundpre == '\r')
            { fprintf(outfile, "\r"); }
          else if (boundpre == '\n') {
            if (boundix > 2 && boundstr[boundix - 2] == '\0' && inch != '-') {
              if (boundextra == 2) {
                if (outfile != stdout) {
                  fclose(outfile);
                  outfile = stdout;
                }
                boundmode = 'X';
                fprintf(stdout, "[ENDBOUNDARY]");
//              } else { printf("[ENDBOUND(%c%d)]", boundmode, boundpre); }
              } else {
//if (boundmode == 'O')
//  { fprintf(stdout, "[NODE:%s]", nodelabel); }
                boundmode = 'D';
//                boundpre = '\0';
                fprintf(stdout, "[BOUNDARY]");
              }
              boundbufi = 0;
              boundbuf[boundbufi] = '\0';
//              boundix = 0;
//              boundpre = '\0';
            } else { fprintf(outfile, "\r\n"); }
          }
          boundpre = '\r';
          boundix = 0;
        } else if (inch == '\n') {
          if (boundpre == '\r')
            { boundpre = '\n'; }
          else if (boundpre == '\n') {
            fprintf(outfile, "\r\n");
            boundpre = '\0';
          }
//          if (boundpre == '\0')
//            { fprintf(outfile, "[n]"); }
//          else if (boundpre == '\r')
//            { boundpre = '\n'; }
//          else if (boundpre == '\n') {
//            fprintf(outfile, "[r][n][n]"); 
//            boundpre = '\0';
//          }
          boundix = 0;
        }
//        } else {
  //        if (boundpre == '\n') { //  || boundpre == '\r') { // \r for boundary match
//fprintf(stdout, "%c%d%c%d,", boundmode, boundpre, inch, boundix);
//            if (inch == '\n') { boundix = 0; }
// else 
        if (boundpre == '\n' && inch != '\n') {
          if (boundix > 2 && boundstr[boundix - 2] == '\0' && inch == '-')
            { boundextra++; }
          else if (boundix == 0 && inch == '-')
            { boundix++; }
          else if (boundix == 0 && inch != '-') {
            fprintf(outfile, "\r\n%c", inch);
            boundpre = '\0';
          } else if (boundix == 1 && inch == '-')
            { boundix++; }
          else if (boundix == 1 && inch != '-') {
            fprintf(outfile, "\r\n%c%c", lastinch, inch);
            boundpre = '\0';
            boundix = 0;
          } else if (boundix >= 2) {
  //          if (boundstr[boundix - 2] == '\0') {
  //            if (inch == '-') { boundextra++; }
  //            else { } // boundstr match triggered elsewhere
  //          } else ^ should be matched earlier
            if (boundstr[boundix - 2] == inch)
              { boundix++; }
            else {
              fprintf(outfile, "\r\n");
              int bif = -1;
              while (++bif <= boundix) {
                if (bif == 0 || bif == 1 || bif == boundix)
                  { fprintf(outfile, "%c", (bif <= 1) ? '-' : inch); }
                else { fprintf(outfile, "%c", boundstr[boundix - 2]); }
              }
              boundix = 0;
            }
          }
        } else if (boundpre == '\r') {
          if (inch != '\r') {
            fprintf(outfile, "\r%c", inch);
            boundpre = '\0';
          }
        } else if (boundpre == '\0') {
          fprintf(outfile, "%c", inch);
        }
  //        } else if (boundpre == '\r' && boundmode == 'F') {
  //      
  //        } else if (boundpre == '\0' && boundmode == 'F') {
  //          fprintf(outfile, "(%d)", inch);
  //        }
//        }
//fprintf(stdout, "%c{%d%c%d}", inch, boundpre, boundmode, boundix);
//if (boundpre != '\0')
//  { printf("{%d%c%d}", boundpre, boundmode, boundix); }
        if (boundpre == '\n' && boundmode == 'D') {
          if (strncmp(boundbuf, "Content-Disposition: ", 21) == 0) {
            fnamestr = strstr(boundbuf, "; filename=\"");
            if (fnamestr) {
              fnamestr = &fnamestr[12];
              fnameend = strstr(fnamestr, "\"");
              if (fnameend) {
                fnameend[0] = '\0';
                while (--fnameend >= fnamestr)
                  { if (fnameend[0] == '.') { break; } }
                if (fnameend[0] != '.')
                  { fnameend = ""; }
                boundmode = 'T';
                boundpre = '\0';
              }
            } else {
              fnodestr = strstr(boundbuf, "; name=\"node\"");
              if (fnodestr) {
                boundmode = 'R';
                boundpre = '\0';
                nodeli = 0;
                nodelabel[nodeli] = '\0';
              }
            }
          }
        } else if (boundpre == '\n' && boundmode == 'T') {
          if (strncmp(boundbuf, "Content-Type: ", 14) == 0) {
            ctypestr = &boundbuf[14];
            // CONSIDER SAVING AS ALTERNATIVE MIME TYPE
            // E.G> .c and .m match as application/octet-stream
// need to check for invalid filenames
// set filename as caculated from filenode
            boundmode = 'S'; // remember 96 char limit includes filename ...
            boundpre = '\0';
            sprintf(outpath, "%s/%s(%s)", nodelabel, fnamestr, ctypestr);
            char tva = tablevaluelabeladdrow(outlabel, UPCACHETABLE, '0', outpath);
            if (tva == 'a' || tva == 'e') {
              sprintf(outpath, "%s/%s", UPCACHEDIR, outlabel);
              outfile = fopen(outpath, "w");
              if (!outfile) {
                fprintf(stdout, "<script type=\"text/javascript\">\n");
                fprintf(stdout, "  document.body.style.color = '#0f0';\n");
                fprintf(stdout, "  document.body.style.backgroundColor = '#000';\n");
                fprintf(stdout, "  $$('loadbar').style.backgroundColor = '#f00';\n");
                fprintf(stdout, "  $$('loadbar').style.opacity = '0.2';\n");
                fprintf(stdout, "  $$('loadpct').innerHTML = '%s';\n", outpath);
                fprintf(stdout, "</script>\n");
//                  fflush(stdout);
                outfile = stdout;
              } // fnameend == .ext
              sprintf(outpath, "%%/%s[%s]", outlabel, fnamestr);
//              fprintf(stdout, "<div id=\"loadfile\">%s</div>\n", outpath);
            }
            fprintf(stdout, "[TYPE]");
          }
        } else if (boundpre == '\n' && boundmode == 'F') { }


        if (boundmode == 'D' || boundmode == 'T') {
          if (inch == '\r') { }
          else if (inch == '\n') {
            boundbufi = 0;
            boundbuf[boundbufi] = '\0';
          } else if (boundbufi < BOUNDBUFSIZE - 1) {
            boundbuf[boundbufi] = inch;
            boundbufi++;
            boundbuf[boundbufi] = '\0';
          } // ^ buffer line if name/type header
        } else if (boundmode == 'N') {
          if (inch == '\r') { }
          else if (inch == '\n') {
            boundmode = 'O';
          } else if (nodeli < TABLEMAXLABELSIZE) {
            if (inch >= '0' && inch <= 'z') {
              nodelabel[nodeli] = inch;
              nodeli++;
              nodelabel[nodeli] = '\0';
            }
          } // ^ save label if node value
        } else if (boundmode == 'R') {
          if (boundpre == '\n') {
            boundmode = 'N';
            boundpre = '\0';
          }
//          if (boundpre == '\r' && inch == '\n') {
//            boundpre = '\0';
//            boundmode = 'N';
//            fprintf(stdout, "[NODE+]\n");
//          }
        } else if (boundmode == 'S') {
          if (boundpre == '\n') {
            boundmode = 'F';
            boundpre = '\0';
          }
//          if (boundpre == '\r' && inch == '\n') {
//            boundpre = '\0';
//            boundmode = 'F';
//            fprintf(stdout, "[START+]\n");
//          }
        } else if (boundmode == 'F') { }


        lastinch = inch;
        inch = fgetc(stdin);
        ccount++;
//printf("[%d:%c(%d)%c]", ccount, inch, inch, boundmode);
        int newctpct = ccount * 100 / clength;
        if (newctpct - ctotalpct >= 10 || newctpct == 100) {
          ctotalpct = ccount * 100 / clength;
        }
      }

      fprintf(stdout, "\n</span>\n");

      fprintf(stdout, "<div>\n");
      fprintf(stdout, "{ \"node\": \"%s\", \"path\": \"%s\"", nodelabel, outpath);
      fprintf(stdout, ", \"name\": \"%s\", \"type\": \"%s\"", fnamestr, ctypestr);
      fprintf(stdout, ", \"length\": \"%d\", \"count\": \"%d\" }\n", clength, ccount);
      fprintf(stdout, "</div>\n");

    } else { fprintf(stdout, "CONTENT ERROR: %s", contenttype); }
//    fprintf(stdout, "</body></html>\n");
  //  fflush(stdout);
    fprintf(stdout, "</body></html>\n");
  }
  return 0;
}
