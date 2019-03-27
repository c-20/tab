      fprintf(stdout, "  var commandkey = false;\n");
      fprintf(stdout, "  var alertkey = false;\n");
      fprintf(stdout, "  window.onkeydown = function(event) {\n");
      fprintf(stdout, "    var esk = keysdown.shift = event.shiftKey;\n");
      fprintf(stdout, "    keysdown.reshift = false;\n");
      fprintf(stdout, "    var eck = keysdown.control = event.ctrlKey;\n");
      fprintf(stdout, "    var eak = keysdown.alt = event.altKey;\n");
      fprintf(stdout, "    var ekc = event.keyCode;\n");
      fprintf(stdout, "    var tln = lineid[inbosel];\n");
      fprintf(stdout, "    $$('optbox').innerHTML = '' + ekc;\n");
      fprintf(stdout, "    if (event.preventDefault === false)\n");
      fprintf(stdout, "      { commandkey = false; }\n");
      fprintf(stdout, "    var pastekey = false;\n");
      fprintf(stdout, "    if (commandkey == true && ekc == 86) {\n"); // CTRL-V
      fprintf(stdout, "      pastefocus();\n"); 
//      fprintf(stdout, "      commandkey = false;\n");
      fprintf(stdout, "      return true;\n");
      fprintf(stdout, "    } else if (commandkey == true && ekc == 67) {\n"); // CTRL-C
      fprintf(stdout, "      if (copying == false) {\n");
      fprintf(stdout, "        copyfocus();\n");
      fprintf(stdout, "        event.preventDefault();\n");
      fprintf(stdout, "      } else if (copying == true)\n");
      fprintf(stdout, "        { copying = 'up'; }\n");
      fprintf(stdout, "      return true;\n");
//      fprintf(stdout, "    } else if (pasting === undefined) {\n");
//      fprintf(stdout, "      pasting = undefined;\n");
//      fprintf(stdout, "      pastefocus();\n");
//      fprintf(stdout, "      return true;\n");
//      fprintf(stdout, "    } else if (commandkey == true && ekc == 67) {\n"); // CTRL-+


      fprintf(stdout, "    } else if (event.preventDefault == false) {\n");
      fprintf(stdout, "      pastekey = true;\n"); // called from function with struct
      fprintf(stdout, "    } else if (ekc == 224 || ekc == 17 || ekc == 91 || ekc == 93) {\n");
      fprintf(stdout, "      commandkey = true;\n"); // double .. a
/*
      fprintf(stdout, "    else if ((copying !== false || pasting !== false) && ekc == 27) {\n");
      fprintf(stdout, "      pasting = undefined;\n");
      fprintf(stdout, "      pastefocus();\n");
      fprintf(stdout, "      event.preventDefault();\n");
      fprintf(stdout, "      return false;\n");
      fprintf(stdout, "    } else if (loading !== false && commandkey == false) {\n");
//      fprintf(stdout, "      pasting = undefined;\n");
//      fprintf(stdout, "      loadfocus();\n");
      fprintf(stdout, "      if (ekc == 27)\n");
      fprintf(stdout, "        { bodyundrag(); }\n");
      fprintf(stdout, "      event.preventDefault();\n");
      fprintf(stdout, "      return false;\n");
*/
      fprintf(stdout, "    } else if (ekc == 76 && commandkey == true) {\n"); // CMD-L
      fprintf(stdout, "      commandkey = false; return true;\n");
      fprintf(stdout, "    } else if (ekc == 187 && commandkey == true) {\n"); // CMD-+
      fprintf(stdout, "      console.log('187!!!!'); return true;\n");
      fprintf(stdout, "    } else if (ekc == 189 && commandkey == true) {\n"); // CMD--
      fprintf(stdout, "      console.log('189!!!'); return true;\n");
      fprintf(stdout, "    } else if (pasting == true) {\n");
      fprintf(stdout, "      $$('pastext').value += '(' + ekc + ')';\n");
      fprintf(stdout, "      event.preventDefault();\n");
      fprintf(stdout, "      return false;\n");
      fprintf(stdout, "    } else { event.preventDefault(); }\n");
//      fprintf(stdout, "    if (commandkey == false) { event.preventDefault(); }\n");
      fprintf(stdout, "    if (ekc == 224 || ekc == 17 || ekc == 91 || ekc == 93)\n");
      fprintf(stdout, "      { commandkey = true; }\n"); // 17 == ctrl.. cross-platform[?]
      fprintf(stdout, "    else if (ekc == 82 && commandkey == true)\n"); // CMD-R
      fprintf(stdout, "      { location.reload(true); commandkey = false; }\n");
      fprintf(stdout, "    else if (ekc >= 65 && ekc <= 90)\n");
      fprintf(stdout, "      { wispkey(String.fromCharCode(esk ? ekc : ekc + 32)); }\n");
      fprintf(stdout, "    else if (ekc >= 97 && ekc <= 122)\n");
      fprintf(stdout, "      { wispkey(String.fromCharCode(esk ? ekc + 32 : ekc)); }\n");
      fprintf(stdout, "    else if (ekc >= 48 && ekc <= 57 && eck) {\n");
      fprintf(stdout, "      if (esk) { iosetinboxrangefrominbox(ekc - 48, inbosel); }\n");
      fprintf(stdout, "      iosetinbox(ekc - 48);\n");
//      fprintf(stdout, "      highlightnextline(0);\n");
      fprintf(stdout, "      wispswitch();\n");
      fprintf(stdout, "    } else if (ekc == 48) { wispkey(esk ? ')' : '0'); }\n");
      fprintf(stdout, "    else if (ekc == 41) { wispkey(')'); }\n");
      fprintf(stdout, "    else if (ekc == 49) { wispkey(esk ? '!' : '1'); }\n");
      fprintf(stdout, "    else if (ekc == 33) { wispkey('!'); }\n");
      fprintf(stdout, "    else if (ekc == 50) { wispkey(esk ? '@' : '2'); }\n");
      fprintf(stdout, "    else if (ekc == 64) { wispkey('@'); }\n");
      fprintf(stdout, "    else if (ekc == 51) { wispkey(esk ? '#' : '3'); }\n");
      fprintf(stdout, "    else if (ekc == 35) { wispkey('#'); }\n");
      fprintf(stdout, "    else if (ekc == 52) { wispkey(esk ? '$' : '4'); }\n");
      fprintf(stdout, "    else if (ekc == 36) { wispkey('$'); }\n");
      fprintf(stdout, "    else if (ekc == 53) { wispkey(esk ? '%' : '5'); }\n");
      fprintf(stdout, "    else if (ekc == 337) { wispkey('%'); }\n");
      fprintf(stdout, "    else if (ekc == 54) { wispkey(esk ? '^' : '6'); }\n");
      fprintf(stdout, "    else if (ekc == 94) { wispkey('^'); }\n");
      fprintf(stdout, "    else if (ekc == 55) { wispkey(esk ? '&amp;' : '7'); }\n");
      fprintf(stdout, "    else if (ekc == 338) { wispkey('&amp;'); }\n");
      fprintf(stdout, "    else if (ekc == 56) { wispkey(esk ? '*' : '8'); }\n");
      fprintf(stdout, "    else if (ekc == 42) { wispkey('*'); }\n");
      fprintf(stdout, "    else if (ekc == 57) { wispkey(esk ? '(' : '9'); }\n");
      fprintf(stdout, "    else if (ekc == 340) { wispkey('('); }\n");
//      fprintf(stdout, "    else if (ekc == 192) { wispkey(esk ? '~' : '`'); }\n");
      fprintf(stdout, "    else if (ekc == 192) { if (esk) { wispkey('~'); }\n");
      fprintf(stdout, "                           else { wisptoggleesc(1); } }\n");
      fprintf(stdout, "    else if (ekc == 126) { wispkey('~'); }\n");
      fprintf(stdout, "    else if (ekc == 96) { wisptoggleesc(1); }\n"); //{ wispkey('`'); }\n");
      fprintf(stdout, "    else if (ekc == 188) { wispkey(esk ? '&lt;' : ','); }\n");
      fprintf(stdout, "    else if (ekc == 44) { wispkey(','); }\n");
      fprintf(stdout, "    else if (ekc == 60) { wispkey('&lt;'); }\n");
      fprintf(stdout, "    else if (ekc == 190) { wispkey(esk ? '&gt;' : '.'); }\n");
      fprintf(stdout, "    else if (ekc == 46) { wispkey('.'); }\n");
      fprintf(stdout, "    else if (ekc == 62) { wispkey('&gt;'); }\n");
      fprintf(stdout, "    else if (ekc == 191) { wispkey(esk ? '?' : '/'); }\n");
      fprintf(stdout, "    else if (ekc == 47) { wispkey('/'); }\n");
      fprintf(stdout, "    else if (ekc == 63) { wispkey('?'); }\n");
      fprintf(stdout, "    else if (ekc == 186) { wispkey(esk ? ':' : ';'); }\n");
      fprintf(stdout, "    else if (ekc == 58) { wispkey(':'); }\n");
      fprintf(stdout, "    else if (ekc == 59) { wispkey(';'); }\n");
      fprintf(stdout, "    else if (ekc == 222) { wispkey(esk ? '&quot;' : '&apos;'); }\n");
      fprintf(stdout, "    else if (ekc == 34) { wispkey('&quot;'); }\n");
      fprintf(stdout, "    else if (ekc == 339) { wispkey('&apos;'); }\n");
      fprintf(stdout, "    else if (ekc == 219) { wispkey(esk ? '{' : '['); }\n");
      fprintf(stdout, "    else if (ekc == 123) { wispkey('{'); }\n");
      fprintf(stdout, "    else if (ekc == 91) { wispkey('['); }\n");
      fprintf(stdout, "    else if (ekc == 391) { wispkey('['); }\n");
      fprintf(stdout, "    else if (ekc == 221) { wispkey(esk ? '}' : ']'); }\n");
      fprintf(stdout, "    else if (ekc == 125) { wispkey('}'); }\n");
      fprintf(stdout, "    else if (ekc == 93) { wispkey(']'); }\n");
      fprintf(stdout, "    else if (ekc == 393) { wispkey(']'); }\n");
      fprintf(stdout, "    else if (ekc == 220) { wispkey(esk ? '|' : '&bsol;'); }\n");
      fprintf(stdout, "    else if (ekc == 124) { wispkey('|'); }\n");
      fprintf(stdout, "    else if (ekc == 92) { wispkey('&bsol;'); }\n");
      fprintf(stdout, "    else if (ekc == 189) { wispkey(esk ? '_' : '-'); }\n");
      fprintf(stdout, "    else if (ekc == 95) { wispkey('_'); }\n");
      fprintf(stdout, "    else if (ekc == 45) { wispkey('-'); }\n");
      fprintf(stdout, "    else if (ekc == 187) { wispkey(esk ? '+' : '='); }\n");
      fprintf(stdout, "    else if (ekc == 43) { wispkey('+'); }\n");
      fprintf(stdout, "    else if (ekc == 61) { wispkey('='); }\n");
      fprintf(stdout, "    else if (ekc == 32) { wispkey('&nbsp;'); }\n");
//      fprintf(stdout, "    else if (ekc == 27) { wispapplytriggers(); }\n");
      fprintf(stdout, "    else if (ekc == 27) {\n");					// ESCAPE
      fprintf(stdout, "      if (esk) { wisptoggletab(); }\n"); // enable/disable editing
      fprintf(stdout, "      else if (loading) { bodyundrag(); }\n"); // cancel upload
      fprintf(stdout, "      else if (copying || pasting)\n");
      fprintf(stdout, "        { pasting = undefined; pastefocus(); }\n"); // cancel copy/paste
      fprintf(stdout, "      else { wisptoggleesc(1); }\n"); // switch window
      fprintf(stdout, "    } else if (ekc == 9 && !pastekey) {\n");			// TAB
      fprintf(stdout, "      if (esk) { wisptoggleesc(0); }\n"); // line zero
      fprintf(stdout, "      else { wisptoggleesc(1); }\n"); // switch window.. maybe next tab later
      fprintf(stdout, "    } else if (ekc == 8 && !pastekey) {\n");			// BACKSPACE
      fprintf(stdout, "      if (eck && esk) { wispremoveline(eak ? 0 : -2); }\n"); // CHECK
      fprintf(stdout, "      else if (esk) { wisptogglestrike(); }\n");
      fprintf(stdout, "      else { wispbackspace(eck ? 5 : 1, esk); }\n");
      fprintf(stdout, "    } else if ((ekc == 10 || ekc == 13) && !pastekey) {\n");	// ENTER
      fprintf(stdout, "      if (wispinline()) {\n");
      fprintf(stdout, "        if (esk) { wispgoto('tsol'); }\n");
      fprintf(stdout, "        wispaddline('s');\n");
      fprintf(stdout, "        if (esk) { wispgoto('pl'); }\n");
      fprintf(stdout, "      } else {\n");
      fprintf(stdout, "        wispopen((eck) ? 'l' : 'h');\n");
      fprintf(stdout, "        if (esk) { wispgoto('nl'); }\n");
      fprintf(stdout, "      }\n");
      fprintf(stdout, "    } else if (ekc == 37 && !pastekey) {\n");			// LEFT ARROW
      fprintf(stdout, "      if (eak) {\n");
      fprintf(stdout, "        if (esk) { ionextinbox('l'); }\n");
      fprintf(stdout, "        else { iopageinbox('l'); }\n"); // ioprevoption
      fprintf(stdout, "      } else if (eck) {\n");
//      fprintf(stdout, "        if (esk) { iopageinbox('l'); }\n");
      fprintf(stdout, "        if (esk) { wispgoto('tsol'); }\n");
      fprintf(stdout, "        else { wispgoto('jl10'); }\n");
//      fprintf(stdout, "        else { wispopen('l'); }\n");
      fprintf(stdout, "      } else { wispleft(esk ? 5 : 1); }\n");
      fprintf(stdout, "    } else if (ekc == 39 && !pastekey) {\n");			// RIGHT ARROW
      fprintf(stdout, "      if (eak) {\n");
      fprintf(stdout, "        if (esk) { ionextinbox('r'); }\n");
      fprintf(stdout, "        else { iopageinbox('r'); }\n"); // ionextoption
      fprintf(stdout, "      } else if (eck) {\n");
//      fprintf(stdout, "        if (esk) { iopageinbox('r'); }\n");
      fprintf(stdout, "        if (esk) { wispgoto('teol'); }\n");
      fprintf(stdout, "        else { wispgoto('jr10'); }\n");
//      fprintf(stdout, "        else { wispopen('r'); }\n");
      fprintf(stdout, "      } else { wispright(esk ? 5 : 1); }\n");
//      fprintf(stdout, "    else if (ekc == 38) { clearhighlight(); prevlines(false); sethighlight(-5); }\n");
//      fprintf(stdout, "    else if (ekc == 40) { clearhighlight(); nextlines(false); sethighlight(5); }\n");
      fprintf(stdout, "    } else if (ekc == 38 && !pastekey) {\n");			// UP ARROW
      fprintf(stdout, "      if (eak) {\n");
      fprintf(stdout, "        if (esk) { ionextinbox('u'); }\n");
      fprintf(stdout, "        else { ioscrollinbox('u'); }\n");
//      fprintf(stdout, "      else { wispright(esk ? 5 : 1); }\n");
//      fprintf(stdout, "      if (eck) { if (wispen == 'n')\n");
//      fprintf(stdout, "        { rebuildinbox(-1, (esk ? -10 : -2)); }\n");
      fprintf(stdout, "      } else if (esk) {\n");
      fprintf(stdout, "        if (eck) { ioscrollinbox('u'); }\n");
      fprintf(stdout, "        else { highlightprevline(1); wispopen('h'); }\n");
//      fprintf(stdout, "        if (esk) { ioscrollinbox('u'); }\n");
//      fprintf(stdout, "        else { wispopen('u'); }\n");
      fprintf(stdout, "      } else {\n");
      fprintf(stdout, "        var wp = wisppos();\n");
      fprintf(stdout, "        if (wp < 0) {\n");
      fprintf(stdout, "          wispgoto('pl', true);\n");
      fprintf(stdout, "          if (esk) { wispopen('h'); }\n");
      fprintf(stdout, "        } else { wispgoto('psl', true); }\n");
//      fprintf(stdout, "        if (wp < 0) {\n");
//      fprintf(stdout, "          iohighlightnextline(eck ? -10 : -1);\n");
//      fprintf(stdout, "          if (!esk) { wisppos(wp); }\n");
//      fprintf(stdout, "        ioqueuemods();\n");
//      fprintf(stdout, "        wispswitch();\n");
//      fprintf(stdout, "          if (esk) { wispopen('h'); }\n");
//      fprintf(stdout, "        iohandlequeue();\n");
//      fprintf(stdout, "        } else { wispgoto('psl'); }\n");
      fprintf(stdout, "      }\n");
      fprintf(stdout, "    } else if (ekc == 40 && !pastekey) {\n"); // DOWN ARROW
      fprintf(stdout, "      if (eak) {\n");
      fprintf(stdout, "        if (esk) { ionextinbox('d'); }\n");
      fprintf(stdout, "        else { ioscrollinbox('d'); }\n");
//      fprintf(stdout, "      if (eck) { if (wispen == 'n')\n");
//      fprintf(stdout, "        { rebuildinbox(-1, (esk ? 10 : 2)); }\n");
      fprintf(stdout, "      } else if (esk) {\n");
      fprintf(stdout, "        if (eck) { ioscrollinbox('d'); }\n");
      fprintf(stdout, "        else { highlightnextline(1); wispopen('h'); }\n");
//      fprintf(stdout, "        if (esk) { ioscrollinbox('d'); }\n");
//      fprintf(stdout, "        else { wispopen('d'); }\n");
      fprintf(stdout, "      } else {\n");
      fprintf(stdout, "        var wp = wisppos();\n");
      fprintf(stdout, "        if (wp < 0) {\n");
      fprintf(stdout, "          wispgoto('nl', true);\n");
      fprintf(stdout, "          if (esk) { wispopen('h'); }\n");
      fprintf(stdout, "        } else { wispgoto('nsl', true); }\n");
//      fprintf(stdout, "          iohighlightnextline(eck ? 10 : 1);\n");
//      fprintf(stdout, "          if (!esk) { wisppos(wp); }\n");
//      fprintf(stdout, "          ioqueuemods();\n");
//      fprintf(stdout, "          wispswitch();\n");
//      fprintf(stdout, "          iohandlequeue();\n");
      fprintf(stdout, "      }\n");
      fprintf(stdout, "    } else if (ekc == 16) {\n");
      fprintf(stdout, "      if (esk && tln && tln.className == 'sel')\n");
      fprintf(stdout, "        { tln.className = 'set'; }\n");
      fprintf(stdout, "    } else if (ekc == 17) {\n");
      fprintf(stdout, "    } else if (ekc == 18) {\n");
      fprintf(stdout, "    }\n"); // else { alert(ekc); }\n");
      fprintf(stdout, "    if (alertkey == true) { alertkey = false; }\n");
      fprintf(stdout, "    else if (esk == true && eck == true && eak == true) { alertkey = true; }\n");
      fprintf(stdout, "  }\n");
      fprintf(stdout, "  window.onkeypress = function(event) {\n");
//      fprintf(stdout, "    if (event.keyCode == 38 || event.keyCode == 40)\n");
//      fprintf(stdout, "    if (commandkey == true && event.keyCode == 86)\n");
//      fprintf(stdout, "      { alert('paste!'); }\n");
//      fprintf(stdout, "    else if (event.keyCode == 86)\n");
//      fprintf(stdout, "      { alert('paste!'); }\n");
//      fprintf(stdout, "    else { event.preventDefault(); }\n");
      fprintf(stdout, "    var ekc = event.keyCode;\n");
      fprintf(stdout, "    if (copying == 'up') { return true; }\n");
      fprintf(stdout, "    else if ((ekc == 187 || ekc == 61) && commandkey == true) { return true; }\n");
      fprintf(stdout, "    else if ((ekc == 189 || ekc == 45) && commandkey == true) { return true; }\n");
      fprintf(stdout, "    else { event.preventDefault(); }\n");
      fprintf(stdout, "  }\n");
      fprintf(stdout, "  window.onkeyup = function(event) {\n");
      fprintf(stdout, "    var esk = keysdown.shift = event.shiftKey;\n");
      fprintf(stdout, "    var eck = keysdown.control = event.ctrlKey;\n");
      fprintf(stdout, "    var eak = keysdown.alt = event.altKey;\n");
      fprintf(stdout, "    var ekc = event.keyCode;\n"); // event.preventDefault();\n");
      fprintf(stdout, "    var tln = lineid[inbosel];\n");
//      fprintf(stdout, "    if (event.keyCode == 86)\n");
//      fprintf(stdout, "      { alert('paste!'); }\n");
//      fprintf(stdout, "    else { event.preventDefault(); }\n");
      fprintf(stdout, "    if (copying == 'up')\n");
      fprintf(stdout, "      { copying = true; copyfocus(); }\n");
      fprintf(stdout, "    else if (ekc == 187 && commandkey == true) { return true; }\n");
      fprintf(stdout, "    else if (ekc == 189 && commandkey == true) { return true; }\n");
      fprintf(stdout, "    else {\n");
      fprintf(stdout, "      event.preventDefault();\n");
//      fprintf(stdout, "    if (ekc == 38 || ekc == 40 || ekc == 13) { wispswitch(); }\n");
      fprintf(stdout, "      if (ekc == 224 || ekc == 17 || ekc == 91 || ekc == 93)\n");
      fprintf(stdout, "        { commandkey = false; }\n");
      fprintf(stdout, "      if (ekc == 16) {\n");
      fprintf(stdout, "        if (tln && tln.className == 'set')\n");
      fprintf(stdout, "          { tln.className = 'sel'; }\n");
      fprintf(stdout, "      } else if (ekc == 17) {\n");
      fprintf(stdout, "      } else if (ekc == 18) {\n");
      fprintf(stdout, "      }\n");
      fprintf(stdout, "      if (esk == false || eck == false || eak == false)\n");
      fprintf(stdout, "        { alertkey = false; }\n");
      fprintf(stdout, "    }\n");
      fprintf(stdout, "  }\n");
