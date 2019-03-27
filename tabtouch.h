      fprintf(stdout, "  var touch = { enabled: false, down: false\n");
      fprintf(stdout, "              , lasttouchcount: 0\n");
      fprintf(stdout, "              , sx: 0, sy: 0, ex: 0, ey: 0\n");
      fprintf(stdout, "              , dx: 0, dy: 0, mx: 0, my: 0\n");
      fprintf(stdout, "              , minmx: 50, minmy: 50 }\n");
      fprintf(stdout, "  function touchenable() {\n");
      fprintf(stdout, "    if ('ontouchstart' in document.documentElement) {\n");
      fprintf(stdout, "      document.addEventListener('touchstart', touchdown, false);\n");
      fprintf(stdout, "      document.addEventListener('touchmove', touchup, false);\n");
      fprintf(stdout, "      touch.enabled = true;\n");
      fprintf(stdout, "    }\n");
      fprintf(stdout, "  }\n");
      fprintf(stdout, "  function touchdown(evt) {\n");
      fprintf(stdout, "    if (!copying && !pasting) {\n");
      fprintf(stdout, "      evt.stopPropagation();\n");
      fprintf(stdout, "      evt.preventDefault();\n");
      fprintf(stdout, "    }\n");
      fprintf(stdout, "    var touchcount = evt.touches.length;\n");
      fprintf(stdout, "    var lasttc = touch.lasttouchcount;\n");
      fprintf(stdout, "    if (touchcount > 0) {\n");
      fprintf(stdout, "      touch.sx = evt.touches[0].pageX;\n");
      fprintf(stdout, "      touch.sy = evt.touches[0].pageY;\n");
      fprintf(stdout, "      touch.down = true;\n");
      fprintf(stdout, "    }\n");
      fprintf(stdout, "  }\n");
      fprintf(stdout, "  function touchup(evt) {\n");
      fprintf(stdout, "    evt.stopPropagation();\n");
      fprintf(stdout, "    evt.preventDefault();\n");
//      fprintf(stdout, "    evt.stopPropagation();\n");
 //     fprintf(stdout, "    evt.stopImmediatePropagation();\n");
      fprintf(stdout, "    if (!touch.down) { return; }\n");
      fprintf(stdout, "    var touchcount = evt.touches.length;\n");
      fprintf(stdout, "    var touchshift = (touchcount == 2);\n");
      fprintf(stdout, "    var touchmeta = (touchcount == 3);\n");
      fprintf(stdout, "    var lasttc = touch.lasttouchcount;\n");
      fprintf(stdout, "    touch.lasttouchcount = touchcount;\n");
      fprintf(stdout, "    touch.ex = evt.touches[0].pageX;\n");
      fprintf(stdout, "    touch.ey = evt.touches[0].pageY;\n");
      fprintf(stdout, "    touch.dx = touch.ex - touch.sx;\n");
      fprintf(stdout, "    touch.dy = touch.ey - touch.sy;\n");
      fprintf(stdout, "    touch.mx = Math.abs(touch.dx);\n");
      fprintf(stdout, "    touch.my = Math.abs(touch.dy);\n");
      fprintf(stdout, "    var event = { ctrlKey: false, altKey: false\n");
      fprintf(stdout, "                , shiftKey: touchshift, keyCode: 0\n");
      fprintf(stdout, "                , preventDefault: function() { } }\n");
      fprintf(stdout, "    if (touch.mx > touch.minmx) {\n");
      fprintf(stdout, "      var touchdir = (touch.dx < 0) ? 'l' : 'r';\n");
      fprintf(stdout, "      var touchcode = (touch.dx < 0) ? 37 : 39;\n");
      fprintf(stdout, "      var touchkey = false;\n");
      fprintf(stdout, "      if (touchshift) {\n");
      fprintf(stdout, "        if (touchdir == 'l')\n");
      fprintf(stdout, "          { touchcode = 9; touchkey = true; }\n");
      fprintf(stdout, "        if (touchdir == 'r')\n");
      fprintf(stdout, "          { touchcode = 96; touchkey = true; }\n");
      fprintf(stdout, "      } else if (touchmeta) {\n");
      fprintf(stdout, "        if (touchdir == 'l')\n");
      fprintf(stdout, "          { wispgoto('tsol'); }\n");
      fprintf(stdout, "        if (touchdir == 'r')\n");
      fprintf(stdout, "          { wispgoto('teol'); }\n");
      fprintf(stdout, "      } else { touchkey = true; }\n");
      fprintf(stdout, "      if (touchkey) {\n");
      fprintf(stdout, "        event.keyCode = touchcode;\n");
      fprintf(stdout, "        touch.down = false;\n");
      fprintf(stdout, "        window.onkeydown(event);\n");
      fprintf(stdout, "        window.onkeyup(event);\n");
      fprintf(stdout, "      }\n");
      fprintf(stdout, "    } else if (touch.my > touch.minmy) {\n");
//      fprintf(stdout, "      if (touchshift) {\n");
//      fprintf(stdout, "        if (wisplinelen() == 0) {\n");
//      fprintf(stdout, "        { bodydragtoggle(); }\n");
//      fprintf(stdout, "      else if (touchshift && touch.dy < -100)\n");
//      fprintf(stdout, "        { pastefocus(); }\n");
//      fprintf(stdout, "      else if (touchshift && touch.dy > 100)\n");
      fprintf(stdout, "      var touchdir = (touch.dy < 0) ? 'u' : 'd';\n");
      fprintf(stdout, "      var touchcode = (touch.dy < 0) ? 38 : 40;\n");
      fprintf(stdout, "      if (touchshift) {\n"); //  && touch.dy < -200)\n");
      fprintf(stdout, "        if (touchdir == 'u')\n");
      fprintf(stdout, "          { bodydragtoggle(); }\n");
      fprintf(stdout, "        if (touchdir == 'd')\n");
      fprintf(stdout, "          { wispopen('h'); }\n");
      fprintf(stdout, "      } else if (touchmeta) {\n");
      fprintf(stdout, "        if (touchdir == 'u')\n");
      fprintf(stdout, "          { copyfocus(); }\n");
      fprintf(stdout, "        if (touchdir == 'd')\n");
      fprintf(stdout, "          { pastefocus(); }\n");
      fprintf(stdout, "      } else {\n");
      fprintf(stdout, "        event.keyCode = touchcode;\n");
      fprintf(stdout, "        window.onkeydown(event);\n");
      fprintf(stdout, "        window.onkeyup(event);\n");
      fprintf(stdout, "      }\n");
      fprintf(stdout, "      touch.down = false;\n");
      fprintf(stdout, "    }\n");
      fprintf(stdout, "  }\n");