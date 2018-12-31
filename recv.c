#include "code.h"
#include <sys/types.h>
#include <dirent.h>

#include "lf.h"
#include "utf8char.h"

#define recv main

#define TAILCATMAXVALUES    10
#define TAILCATMAXTIMELEN   40
#define TAILCATBOXWIDTH     85
#define TAILCATHEADPADWIDTH 5
#define TAILCATHEADWIDTH    30
#define TAILCATHEADTIMECOL  20

#define RECVMEMDIR   "/dev/shm/ltl"
#define RECVMAXNAMELEN   128

VD showdir(CS dirname, IN dirlevel);
CH censormode = 0;
IN recv($) {
  CH webmode = 0;
  CH datamode = 0;
  CS querystr = getenv("QUERY_STRING");
  IF (querystr NQNULL) {
    webmode = 1;
    IF (querystr[0] EQ 'X' AND querystr[1] EQNUL)
      { censormode = 1; }
    EF (querystr[0] EQ 'D' AND querystr[1] EQNUL)
      { datamode = 1; }
  }
  IF (webmode EQ 1) {
    printf("Content-type: text/html; charset=utf-8\r\n\r\n");
    IF (censormode EQ 1) {
      printf("<html><head><title>RECV</title>\n");
    } EL {
      printf("<html><head><title>recv</title>\n");
    }
    IF (datamode EQ 0) {
      printf("<style type=\"text/css\">\n");
      printf("  body { background-color: #000; color: #888;\n");
      printf("         font-family: Courier New, monospace; font-size: 16px; }\n");
      printf("  .R { font-weight: normal; color: #f00; }\n");
      printf("  .Y { font-weight: normal; color: #ff0; }\n");
      printf("  .G { font-weight: normal; color: #0f0; }\n");
      printf("  .C { font-weight: normal; color: #0ff; }\n");
      printf("  .B { font-weight: normal; color: #00f; }\n");
      printf("  .M { font-weight: normal; color: #f0f; }\n");
      printf("  .K { font-weight: normal; color: #000; }\n");
      printf("  .D { font-weight: normal; color: #666; }\n");
      printf("  .L { font-weight: normal; color: #bbb; }\n");
      printf("  .W { font-weight: normal; color: #fff; }\n");
      printf("  .WC { display: inline-block; width: 14px;\n");
      printf("        font-weight: normal; font-size: 12px; }\n");
      printf("  #data { display: none; }\n");
      printf("  #text { position: absolute; top: 0; left: 0; }\n");
      printf("  #textsel { position: absolute; top: 0; left: 0; opacity: 0.5; }\n");
      printf("  #map { position: fixed; top: 0; left: 0; opacity: 0.7; }\n");
      printf("</style>\n");
      printf("<script type=\"text/javascript\">\n");
      printf("  function id(id) { return document.getElementById(id); }     \n");
      printf("  var textcontext = undefined;                                \n");
      printf("  var textselcontext = undefined;                             \n");
      printf("  var mapcontext = undefined;                                 \n");
      printf("  var mapdata = [];                                           \n");
      printf("  var mapxdata = [];                                          \n");
      printf("  var mapbound = { lat: { min:  -90, max:  90, range: 1, scale: 1 },  \n");
      printf("                  long: { min: -180, max: 180, range: 1, scale: 1 } };\n");
      printf("  var mapcanvas  = { w:  1024,  h: 512 };                      \n");
      printf("  var mapcopy = undefined;                                     \n");
      printf("  var textsquare = { w:    7,  h:  13 };                       \n");
      printf("  var textcanvas = { w:  32*7, h: textsquare.h, gw: 0, gh: 0 }; \n");
      printf("  var textcursor = { x:    1,  y:   1,  s:  0 };               \n");
      printf("  var textselblinkslow = 1000;                                \n");
      printf("  var textselblinkfast = 500;                                 \n");
      printf("  var textselblinkrate = textselblinkfast;                    \n");
      printf("  var textseltrigger = undefined;                             \n");
      printf("  function getlinedata(linenum, charnum) {                    \n");
      printf("    if (linenum === undefined) { linenum = textcursor.y; }    \n");
      printf("    if (charnum === undefined) { charnum = textcursor.x; }    \n");
      printf("    var txt = id('data').innerHTML;                           \n");
      printf("    var init = txt.charAt(0);                                 \n");
      printf("    txt = txt.substr(1);                                      \n");
      printf("    var line = '';                                            \n");
      printf("    var char = '';                                            \n");
      printf("    var col = 1;                                              \n");
      printf("    var row = 1;                                              \n");
      printf("    var inprefix = 1;                                         \n");
      printf("    var insuffix = 0;                                         \n");
      printf("    var inlevel = 0;                                          \n");
      printf("    var borderline = 0;                                       \n");
      printf("    var label = '';                                           \n");
      printf("    var value = '';                                           \n");
      printf("    var data = [];                                            \n");
      printf("    var esc = 0;                                              \n");
      printf("    var escix = 0;                                            \n");
      printf("    var esccol = 0;                                           \n");
      printf("    var escvalone = 0;                                        \n");
      printf("    var escvaltwo = 0;                                        \n");
      printf("    for (var i = 0; i < txt.length; i++) {                    \n");
      printf("      var txtch = txt.charAt(i);                              \n");
      printf("      if (txtch.charCodeAt(0) == 27)                          \n");
      printf("        { esc = 1; escix = i; esccol = col; escwidth = 1; }   \n");
      printf("      else if (esc == 1 && txtch == '[')                      \n");
      printf("        { esc = 2; escvalone = 0; escwidth++; }               \n");
      printf("      else if (esc == 2 && txtch == ';')                      \n");
      printf("        { esc = 3; escvaltwo = 0; escwidth++; }               \n");
      printf("      else if (esc == 2 && txtch == 'G') {                    \n");
      printf("        escwidth++;                                           \n");
      printf("        while (col < escvalone)                               \n");
      printf("          { line += ' '; col++; }                             \n");
      printf("        esc = 0;                                              \n");
      printf("      } else if ((esc == 2 || esc == 3) && txtch == 'm') {     \n");
      printf("        if (esc == 2) { escvaltwo = escvalone; escvalone = 0; } \n");
      printf("        var c = '0';                                            \n");
      printf("        if (escvalone == 1 && escvaltwo == 31) { c = 'R'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 33) { c = 'Y'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 32) { c = 'G'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 36) { c = 'C'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 34) { c = 'B'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 35) { c = 'M'; }     \n");
      printf("        if (escvalone == 0 && escvaltwo == 30) { c = 'K'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 30) { c = 'D'; }      \n");
      printf("        if (escvalone == 0 && escvaltwo == 37) { c = 'L'; }       \n");
      printf("        if (escvalone == 1 && escvaltwo == 37) { c = 'W'; }       \n");
      printf("        if (escvalone == 0 && escvaltwo == 0)  { c = '-'; }       \n");
      printf("        if (c != '0') { colour = c; }                             \n");
      printf("        esc = 0;                                                  \n");
      printf("      } else if (esc == 2 && txtch >= '0' && txtch <= '9') {      \n");
      printf("        escvalone = (escvalone * 10) + (txtch - '0');             \n");
      printf("        escwidth++;                                               \n");
      printf("      } else if (esc == 3 && txtch >= '0' && txtch <= '9') {     \n");
      printf("        escvaltwo = (escvaltwo * 10) + (txtch - '0');           \n");
      printf("        escwidth++;                                             \n");
      printf("      } else if (txtch == '\\n') {                              \n");
      printf("        if (row == linenum) { break; }                          \n");
      printf("        else { row++; }                                         \n");
      printf("        line = '';                                              \n");
      printf("        label = '';                                             \n");
      printf("        col = 1;                                                \n");
      printf("        inprefix = 1;                                           \n");
      printf("        insuffix = 0;                                           \n");
      printf("        inlevel = 0;                                            \n");
      printf("        borderline = 0;                                         \n");
      printf("      } else {                                                 \n");
      printf("        if (row == linenum) {                                  \n");
      printf("          line += txtch;                                       \n");
      printf("          if (txtch.charCodeAt(0) > 20000) { line += ' '; }    \n");
      printf("          if (col == charnum) { char = txtch; }                \n");
      printf("        }                                                      \n");
      printf("        if (inprefix == 1) {                                   \n");
      printf("          if (txtch.charCodeAt(0) == 9474) {           \n"); // VT
      printf("            inlevel++;                                         \n");
      printf("          } else if (txtch.charCodeAt(0) == 9484) {    \n"); // TL
      printf("            borderline = 1;                                 \n");
      printf("          } else if (txtch.charCodeAt(0) == 9500) {    \n"); // TBL
      printf("            borderline = 2;                                 \n");
      printf("          } else if (txtch.charCodeAt(0) == 9492) {    \n"); // BL
      printf("            borderline = 3;                                 \n");
      printf("          } else if (txtch.charCodeAt(0) == 9472) {    \n"); // HZ
      printf("            inprefix = 1;                                      \n");
      printf("          } else { inprefix = 0; }                             \n");
      printf("        }                                                      \n");
      printf("        if (inprefix == 0 && insuffix == 0) {                  \n");
      printf("          if (txtch.charCodeAt(0) == 9472) { insuffix = 1; } \n"); // HZ
      printf("          if (txtch.charCodeAt(0) == 9474) { insuffix = 1; } \n"); // VT
      printf("        }                                              \n");
      printf("        if (inprefix == 0 && insuffix == 0 && row == linenum) { \n");
      printf("          if (borderline > 0) {                                \n");
      printf("            label += txtch;                                    \n");
      printf("            if (txtch.charCodeAt(0) > 20000) { label += ' '; }   \n");
      printf("          } else {                                              \n");
      printf("            if (txtch == '[' || txtch == ']') { txtch = ' '; }   \n");
      printf("            if (txtch == '(' || txtch == ')') { txtch = ' '; }   \n");
      printf("            if (txtch == ' ') {                                  \n");
      printf("              if (value != '') {  data.push(value); value = ''; } \n");
      printf("            } else { value += txtch; }                           \n");
      printf("          }                                                     \n");
      printf("        } else if (insuffix == 1 && value != '')                \n");
      printf("          { data.push(value); value = ''; }                     \n");
      printf("        if (txtch.charCodeAt(0) > 20000) { col += 2; }         \n");
      printf("        else { col++; }                                        \n");
      printf("      }                                                        \n");
      printf("    }                                                          \n");
      printf("    var linesize = line.length - (2 * inlevel);                \n");
      printf("    line = line.substr(inlevel, linesize);                     \n");
      printf("    return { label: label, data: data, char: char, line: line, \n");
      printf("             borderline: borderline, inlevel: inlevel };       \n");
      printf("  }                                                            \n");
      printf("  function datareload() {                                      \n");
      printf("                                                               \n");
      printf("  }                                                            \n");
      printf("  function dataredraw() {                                      \n");
      printf("                                                               \n");
      printf("  }                                                            \n");
      printf("  function mapinit() {                                         \n");
      printf("    mapcanvas.w = document.body.clientWidth;                   \n");
      printf("    mapcanvas.h = document.body.clientHeight;                  \n");
      printf("    id('map').width = mapcanvas.w;                             \n");
      printf("    id('map').height = mapcanvas.h;                            \n");
      printf("    mapcontext = id('map').getContext('2d');                   \n");
      printf("  }                                                            \n");
      printf("  function mapclear() {                                        \n");
      printf("    mapcontext.clearRect(0, 0, mapcanvas.w, mapcanvas.h);      \n");
      printf("  }                                                            \n");
      printf("  function mappointdraw(lat, long, rad, colour) {               \n");
      printf("    var py = (lat - mapbound.lat.min) * mapbound.lat.scale;      \n");
      printf("    var px = (long - mapbound.long.min) * mapbound.long.scale;    \n");
      printf("    mapcontext.strokeStyle = colour;                              \n");
      printf("    mapcontext.beginPath();                                       \n");
      printf("    mapcontext.arc(px, py, rad, 0, 2 * Math.PI);                  \n");
      printf("    mapcontext.stroke();                                         \n");
      printf("  }                                                             \n");
      printf("  function mapredraw() {                                       \n");
      printf("    var minlat  = 999, maxlat  = -999;                         \n");
      printf("    var minlong = 999, maxlong = -999;                         \n");
      printf("    for (var l = 1; l <= textcanvas.gh; l++) {                 \n");
      printf("      var line = getlinedata(l);                               \n");
      printf("      if (line.data.length == 6) {                             \n");
      printf("        var coord = [ parseFloat(line.data[3]),                \n");
      printf("                      parseFloat(line.data[4]),                \n");
      printf("                      parseFloat(line.data[2]) ];              \n");
      printf("        if (coord[0] < minlat)  { minlat  = coord[0]; }        \n");
      printf("        if (coord[0] > maxlat)  { maxlat  = coord[0]; }        \n");
      printf("        if (coord[1] < minlong) { minlong = coord[1]; }        \n");
      printf("        if (coord[1] > maxlong) { maxlong = coord[1]; }        \n");
      printf("        mapdata.push(coord);                                   \n");
      printf("      } else if (line.borderline == 1) {                       \n");
      printf("        mapdata.push(['>']);                                   \n");
      printf("      } else if (line.borderline == 2) {                       \n");
      printf("        mapdata.push(['-']);                                   \n");
      printf("      } else if (line.borderline == 3) {                       \n");
      printf("        mapdata.push(['<']);                                   \n");
      printf("      }                                                        \n");
      printf("    }                                                          \n");
      printf("    mapbound.lat.min    = minlat;                              \n");
      printf("    mapbound.long.min   = minlong;                             \n");
      printf("    mapbound.lat.max    = maxlat;                              \n");
      printf("    mapbound.long.max   = maxlong;                             \n");
      printf("    mapbound.lat.range  = maxlat - minlat;                     \n");
      printf("    mapbound.long.range = maxlong - minlong;                   \n");
      printf("    mapbound.lat.scale  = mapcanvas.h / mapbound.lat.range;    \n");
      printf("    mapbound.long.scale = mapcanvas.w / mapbound.long.range;   \n");
      printf("    var lastlat = 0;                                           \n");
      printf("    var lastlong = 0;                                          \n");
      printf("    var movetonext = 0;                                        \n");
      printf("    for (var i = 0; i < mapdata.length; i++) {                 \n");
      printf("      var coord = mapdata[i];                                  \n");
      printf("      if (coord.length == 1) {                                 \n");
      printf("        movetonext = 1;                                        \n");
      printf("        if (coord[0] == '>') {                                 \n");
      printf("          mapcontext.beginPath();                              \n");
      printf("        } else if (coord[0] == '-') {                          \n");
      printf("          mapcontext.stroke();                                 \n");
      printf("          mapcontext.beginPath();                              \n");
      printf("        } else if (coord[0] == '<') {                          \n");
      printf("          mapcontext.stroke();                                 \n");
      printf("        }                                                      \n");
      printf("      } else {                                                 \n");
      printf("        var py = (coord[0] - minlat) * mapbound.lat.scale;     \n");
      printf("        var px = (coord[1] - minlong) * mapbound.long.scale;   \n");
      printf("        if (movetonext == 1) {                                  \n");
      printf("          mappointdraw(coord[0], coord[1], coord[2] / 2, '#0f0'); \n");
      printf("          lastlat = py; lastlong = px;                            \n");
      printf("          movetonext = 0;                                         \n");
      printf("          mapxdata.push(['M', px, py]);                           \n");
      printf("        } else {                                                  \n");
      printf("          mappointdraw(coord[0], coord[1], coord[2] / 2, '#ff0'); \n");
      printf("          mapcontext.strokeStyle = 'red';                        \n");
      printf("          mapcontext.beginPath();                              \n");
      printf("          mapcontext.moveTo(lastlong, lastlat);                   \n");
      printf("          mapcontext.lineTo(px, py);                              \n");
      printf("          mapcontext.stroke();                                 \n");
      printf("          mapxdata.push(['L', px, py]);                            \n");
      printf("          lastlat = py; lastlong = px;                            \n");
      printf("        }                                                           \n");
      printf("      }                                                              \n");
      printf("    }                                                                 \n");
      printf("    mapcopy = mapcontext.getImageData(0, 0, mapcanvas.w, mapcanvas.h); \n");
      printf("  }                                                                    \n");
      printf("  function mapcopyredraw() {                                   \n");
      printf("    if (mapcopy)                                               \n");
      printf("      { mapcontext.putImageData(mapcopy, 0, 0); }              \n");
      printf("  }                                                            \n");
      printf("  function textselredraw() {                                   \n");
      printf("    var canvasw = textcanvas.w;                                \n");
      printf("    var canvash = textcanvas.h;                                \n");
      printf("    if (textcursor.s == 0) {                                   \n");
      printf("      textselcontext.fillStyle = 'deepskyblue';                \n");
      printf("    } else if (textcursor.s == 1) {                            \n");
      printf("      textselcontext.fillStyle = 'darkviolet';                 \n");
      printf("    }                                                          \n");
      printf("    var line = getlinedata();                                  \n");
      printf("    var cursorx = textsquare.w * textcursor.x;                 \n");
      printf("    var cursory = textsquare.h * (textcursor.y - 1);           \n");
      printf("    var cursorw = textsquare.w;                                \n");
      printf("    var cursorh = textsquare.h;                                 \n");
      printf("    if (line.borderline == 1 || line.borderline == 2) {          \n");
      printf("      var labelstart = line.inlevel + line.line.indexOf(' ');     \n");
      printf("      var labelstop = line.inlevel + line.line.lastIndexOf(' ');  \n");
      printf("      cursorx = labelstart * textsquare.w;                        \n");
      printf("      cursorw = (labelstop - labelstart + 1) * textsquare.w;      \n");
      printf("    } else if (line.inlevel >= 0) {                               \n");
      printf("      cursorx = line.inlevel * textsquare.w;                      \n");
      printf("      cursorw = line.line.length * textsquare.w;                  \n");
      printf("    }                                                              \n");
      printf("    textselcontext.clearRect(0, 0, canvasw, canvash);               \n");
      printf("    textselcontext.fillRect(cursorx, cursory + 3, cursorw, cursorh); \n");
      printf("    var lat = parseFloat(line.data[3]);                           \n");
      printf("    var long = parseFloat(line.data[4]);                          \n");
      printf("    var acc = parseFloat(line.data[2]);                           \n");
      printf("    mapclear();                                                   \n");
      printf("    mapcopyredraw();                                              \n");
      printf("    mappointdraw(lat, long, acc / 2, '#00f');                     \n");
      printf("  }                                                               \n");
      printf("  function texttogglecursor() {                                   \n");
      printf("    if (textcursor.s == 0) { textcursor.s = 1; }                  \n");
      printf("    else if (textcursor.s == 1) { textcursor.s = 0; }             \n");
      printf("    textselredraw();                                              \n");
 //   printf("    mapredraw();                                                  \n");
      printf("  }                                                               \n");
      printf("  function textselupdate() {                                      \n");
      printf("    texttogglecursor();                                           \n");
      printf("    textseltrigger = setTimeout(textselupdate, textselblinkrate); \n");
      printf("  }                                                               \n");
      printf("  function textselupdatestart() {                                 \n");
      printf("    if (textseltrigger) { clearTimeout(textseltrigger); }         \n");
      printf("    textseltrigger = setTimeout(textselupdate, textselblinkrate); \n");
      printf("  }                                                               \n");
      printf("  function textcursorreset() {                                   \n");
      printf("    if (!textselcontext) { return; }                            \n");
      printf("    textcursor.x = 1;                                           \n");
      printf("    textcursor.y = 1;                                           \n");
      printf("    textcursor.s = 0;                                           \n");
      printf("    textselredraw();                                            \n");
      printf("  }                                                             \n");
      printf("  function textcursorsetpos(x, y) {                             \n");
      printf("    if (!textselcontext) { return; }                            \n");
      printf("    textcursor.x = x;                                           \n");
      printf("    textcursor.y = y;                                           \n");
      printf("    textcursor.s = 0;                                           \n");
      printf("    textselredraw();                                           \n");
      printf("  }                                                            \n");
      printf("  function mouse(e) {                                          \n");
      printf("    var cursorx = Math.floor(e.pageX / textsquare.w);          \n");
      printf("    var cursory = Math.floor(e.pageY / textsquare.h);         \n");
      printf("    textcursorsetpos(cursorx + 1, cursory + 1);               \n");
      printf("  }                                                          \n");
      printf("  function textinitcanvas() {                               \n");
      printf("    var linewidth = 0;                                      \n");
      printf("    var boxwidth = 0;                                       \n");
      printf("    var boxheight = 1;                                      \n");
      printf("    var txt = id('data').innerHTML;                         \n");
      printf("    var init = txt.charAt(0);                               \n");
      printf("    txt = txt.substr(1);                                    \n");
      printf("    for (var i = 0; i < txt.length; i++) {                  \n");
      printf("      var txtch = txt.charAt(i);                            \n");
      printf("      if (txtch == '\\n') {                                 \n");
      printf("        boxheight++;                                        \n");
      printf("        if (linewidth > boxwidth)                           \n");
      printf("          { boxwidth = linewidth; }                         \n");
      printf("        linewidth = 0;                                      \n");
      printf("      } else {                                              \n");
      printf("        if (txtch.charCodeAt(0) > 9000) {                   \n");
      printf("          linewidth++;                                      \n");
      printf("        }                                                   \n");
      printf("      }                                                     \n");
      printf("    }                                                       \n");
      printf("    textcanvas.gw = boxwidth;                               \n");
      printf("    textcanvas.gh = boxheight;                              \n");
      printf("    textcanvas.w = boxwidth * textsquare.w;                 \n");
      printf("    textcanvas.h = boxheight * textsquare.h;                \n");
      printf("    id('textsel').width = textcanvas.w;                     \n");
      printf("    id('textsel').height = textcanvas.h;                    \n");
      printf("    textselcontext = id('textsel').getContext('2d');        \n");
      printf("    id('text').width = textcanvas.w;                        \n");
      printf("    id('text').height = textcanvas.h;                       \n");
      printf("    textcontext = id('text').getContext('2d');              \n");
      printf("    textcontext.font = '12px Courier New';                  \n");
      printf("    mapinit();                                              \n");
      printf("  }                                                         \n");
// query size: NUMLINES MAXWIDTH 
//             [192.168.20.129:[181220(99)][181221(99)]][next:[181221(3)]]
      printf("  function textsetcolumn(c) {                               \n");
      printf("    textcursor.x = c;                                       \n");
      printf("  }                                                           \n");
      printf("  function textsetcolour(c) {                               \n");
      printf("    if (c == 'R') { textcontext.fillStyle = 'red';        } \n");
      printf("    if (c == 'Y') { textcontext.fillStyle = 'yellow';     } \n");
      printf("    if (c == 'G') { textcontext.fillStyle = 'lime';       } \n");
      printf("    if (c == 'C') { textcontext.fillStyle = 'cyan';       } \n");
      printf("    if (c == 'B') { textcontext.fillStyle = 'blue';       } \n");
      printf("    if (c == 'M') { textcontext.fillStyle = 'magenta';    } \n");
      printf("    if (c == 'K') { textcontext.fillStyle = 'black';      } \n");
      printf("    if (c == 'D') { textcontext.fillStyle = 'darkgrey';   } \n");
      printf("    if (c == 'L') { textcontext.fillStyle = 'lightgrey';  } \n");
      printf("    if (c == 'W') { textcontext.fillStyle = 'white';      } \n");
      printf("    if (c == '-') { textcontext.fillStyle = 'darkorange'; } \n");
      printf("  }                                                         \n");
      printf("  function textwritechar(c) {                               \n");
      printf("    if (c == '\\n') {                                       \n");
      printf("      textcursor.x = 1; textcursor.y++;                     \n");
      printf("    } else if (c.charCodeAt(0) > 20000) {                   \n");
      printf("      var tx = textsquare.w * (textcursor.x - 1);           \n");
      printf("      var ty = textsquare.h * textcursor.y;                 \n");
      printf("      textcontext.fillText(c, tx, ty);                      \n");
      printf("      textcursor.x += 2;                                    \n");
      printf("    } else {                                                \n");
      printf("      var tx = textsquare.w * (textcursor.x - 1);           \n");
      printf("      var ty = textsquare.h * textcursor.y;                 \n");
      printf("      textcontext.fillText(c, tx, ty);                      \n");
      printf("      textcursor.x++;                                       \n");
      printf("    }                                                       \n");
      printf("  }                                                         \n");
      printf("  function textinit() {                                     \n");
      printf("    textinitcanvas();                                       \n");
      printf("    textsetcolour('-');                                     \n");
      printf("    var txt = id('data').innerHTML;                         \n");
      printf("    var init = txt.charAt(0);                               \n");
      printf("    txt = txt.substr(1);                                    \n");
      printf("    var newtxt = '';                                        \n");
      printf("    var esc = 0;                                            \n");
      printf("    var escix = 0;                                          \n");
      printf("    var esccol = 0;                                         \n");
      printf("    var escvalone = 0;                                      \n");
      printf("    var escvaltwo = 0;                                      \n");
      printf("    var col = 1;                                            \n");
      printf("    for (var i = 0; i < txt.length; i++) {                  \n");
      printf("      var txtch = txt.charAt(i);                            \n");
      printf("      if (txtch.charCodeAt(0) == 27) {                      \n");
      printf("        esc = 1;                                            \n");
      printf("        escix = i;                                          \n");
      printf("        esccol = col;                                       \n");
      printf("        escwidth = 1;                                       \n");
      printf("      } else if (esc == 1 && txtch == '[') {                \n");
      printf("        esc = 2;                                            \n");
      printf("        escvalone = 0;                                        \n");
      printf("        escwidth++;                                           \n");
      printf("      } else if (esc == 2 && txtch == ';') {                  \n");
      printf("        esc = 3;                                              \n");
      printf("        escvaltwo = 0;                                        \n");
      printf("        escwidth++;                                           \n");
      printf("      } else if (esc == 2 && txtch == 'G') {                  \n");
      printf("        escwidth++;                                           \n");
      printf("        while (col < escvalone)                               \n");
      printf("          { newtxt += '&nbsp;'; col++; }                      \n");
      printf("        textsetcolumn(escvalone);                           \n");
      printf("        esc = 0;                                              \n");
      printf("      } else if ((esc == 2 || esc == 3) && txtch == 'm') {     \n");
      printf("        if (esc == 2) { escvaltwo = escvalone; escvalone = 0; } \n");
      printf("        var c = '0';                                            \n");
      printf("        if (escvalone == 1 && escvaltwo == 31) { c = 'R'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 33) { c = 'Y'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 32) { c = 'G'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 36) { c = 'C'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 34) { c = 'B'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 35) { c = 'M'; }     \n");
      printf("        if (escvalone == 0 && escvaltwo == 30) { c = 'K'; }     \n");
      printf("        if (escvalone == 1 && escvaltwo == 30) { c = 'D'; }      \n");
      printf("        if (escvalone == 0 && escvaltwo == 37) { c = 'L'; }       \n");
      printf("        if (escvalone == 1 && escvaltwo == 37) { c = 'W'; }        \n");
      printf("        if (escvalone == 0 && escvaltwo == 0)  { c = '-'; }         \n");
      printf("        if (c == '-') { newtxt += '</b>'; }                          \n");
      printf("        else if (c != '0') { newtxt += '<b class=\"' + c + '\">'; }  \n");
      printf("        textsetcolour(c);                                          \n");
      printf("        esc = 0;                                                     \n");
      printf("      } else if (esc == 2 && txtch >= '0' && txtch <= '9') {        \n");
      printf("        escvalone = (escvalone * 10) + (txtch - '0');              \n");
      printf("        escwidth++;                                               \n");
      printf("      } else if (esc == 3 && txtch >= '0' && txtch <= '9') {     \n");
      printf("        escvaltwo = (escvaltwo * 10) + (txtch - '0');           \n");
      printf("        escwidth++;                                             \n");
      printf("      } else if (esc == 0 && txtch == '<') {                   \n");
      printf("        esc = -1;                                             \n");
      printf("        newtxt += '<';                                        \n");
      printf("      } else if (esc == -1 && txtch == '>') {                 \n");
      printf("        esc = 0;                                              \n");
      printf("        newtxt += '>';                                         \n");
      printf("      } else {                                                 \n");
      printf("        if (esc == 0 && txtch == ' ') {                        \n");
      printf("          col++; newtxt += '&nbsp;';                            \n");
      printf("          textwritechar(' ');                                 \n");
      printf("        } else if (esc == 0 && txtch == '\\n') {                 \n");
      printf("          col = 1; newtxt += '<br>\\n';                           \n");
      printf("          textwritechar('\\n');                                  \n");
      printf("        } else if (esc == 0 && txtch.charCodeAt(0) > 20000) {      \n");
      printf("          col += 2; newtxt += '<b class=\"WC\">' + txtch + '</b>';  \n");
      printf("          textwritechar(txtch);                                   \n");
      printf("        } else if (esc == 0) {                                      \n");
      printf("          col++; newtxt += txtch;                                   \n");
      printf("          textwritechar(txtch);                                   \n");
      printf("        } else { newtxt += txtch; }                      \n"); // still need?
      printf("      }                                                            \n");
      printf("    }                                                           \n");
//      printf("    id('text').innerHTML = newtxt;                            \n");
      printf("    textcursorreset();                                          \n");
      printf("    textselupdatestart();                                       \n");
      printf("  }                                                             \n");
      printf("</script>\n");
      printf("</head><body onload=\"textinit(); mapredraw();\"");
      printf(" onmousemove=\"mouse(event);\"");
      printf(" onclick=\"mapredraw();\">\n");
    } else if (datamode == 1) {
      printf("</head><body>\n");
    }
    printf("  <div id=\"data\">\n");
  }
  CHDIRORFAIL(RECVMEMDIR) { LOG("NOACCESS"); RT 0; }
  showdir(".", 0);
  IF (webmode EQ 1) {
    printf("  </div>\n");
    if (datamode == 0) {
      printf("  <canvas id=\"text\"></canvas>\n");
      printf("  <canvas id=\"textsel\"></canvas>\n");
      printf("  <canvas id=\"map\"></canvas>\n");
    }
    printf("</body></html>\n");
  }
  RT 0;
}

VD showdir(CS dirname, IN dirlevel) {
  IN boxwidth = TAILCATBOXWIDTH;
  struct dirent **namelist;
  CS realdirname = "."; // chdir into subdir before recurse, for safety
  IN numfiles = scandir(realdirname, &namelist, NULL, alphasort);
  IF (numfiles GQ 0) { // still need to free namelist if empty
    IN totalcount = 0;
    IN thisindex = 0; // prints this packet at start of next packet
    LF thesevalues[TAILCATMAXVALUES];
    CH firstfilename[TAILCATMAXTIMELEN];
    firstfilename[0] = NUL;
    CH firstdispname[TAILCATMAXTIMELEN];
    firstdispname[0] = NUL;
    CH lastfilename[TAILCATMAXTIMELEN];
    lastfilename[0] = NUL;
    CH currentprefix[TAILCATMAXTIMELEN];
    currentprefix[0] = NUL;
    IN pass = 0;
    IN fileixofrange = 0;
    IN filesetix = 0;
    WI (1) {
      IF (pass EQ 0) { pass = 1; } // read dirs
      EF (pass EQ 1) { pass = 2; } // read files
      EF (pass GQ 2) { BK; } // end after 2 passes
      IN fileix = -1;
      WI (INC fileix LT numfiles) {
        CS filename = namelist[fileix]->d_name;
        UCH filetype = namelist[fileix]->d_type;
        IF (filetype EQ DT_REG AND pass EQ 2) {
// ------------------------------------------------------------- REGULAR FILE -----------------
          CS dirnamep = dirname;
          CS dispname = filename;
          WI (dirnamep[0] NQ '+' AND dirnamep[0] NQNUL) { INC dirnamep; }
          IF (dirnamep[0] EQ '+') { // only truncate using dirs containing a +
            dirnamep = dirname;
            WI (dispname[0] EQ dirnamep[0] AND dirnamep[0] NQNUL)
              { INC dispname; INC dirnamep; } // truncate filename by dirname
          }
          IF (fileixofrange EQ 0) { // first filename in this range
            CH fnmatch = 1; // matching unless non-matching
            IN fnplix = 0; // find index of +, check for repeats
            WI (dispname[fnplix] NQ '+' AND dispname[fnplix] NQNUL) {
              IF (dispname[fnplix] NQ currentprefix[fnplix])
                { fnmatch = 0; } // prefix non-matching (only relevant if + found)
              INC fnplix;
            }
            IF (fnmatch EQ 1 AND dispname[fnplix] EQ '+') { // this is a repeating prefix
              // the date (pre +) is matching
              Un(Uboxvt, dirlevel + 1); // box only
              CS dispsubname = dispname;
              IF (dispname[fnplix] EQ '+')
                { dispsubname = &dispname[fnplix + 1]; }
              Ws(dispsubname);
              INC filesetix; // this is the next in a set
            } EF (fnmatch EQ 0 AND dispname[fnplix] EQ '+') { // this is a new prefix
              // end previous, create new, display rest of name at +1
              Un(Uboxvt, dirlevel);
              IF (filesetix EQ 0) {
                U1(Uboxtl);         // this is the first
              } EL { U1(Uboxudl); } // this is the next (last is footer)
              IN pheadpadwidth = TAILCATHEADPADWIDTH;
              Un(Uboxhz, pheadpadwidth); // padding of 1
              STRF(currentprefix, "%s", dispname);
              currentprefix[fnplix] = '\0';
              Ms(" \xE6\x97\xA5"); // ri, width of 2
              Y1("%s ", currentprefix); // display prefix in sunny colour
              IN prefixlen = strlen(currentprefix) + 2 + 2; // + 2 spaces
              IN pboxinnerwidth = boxwidth - (2 * (dirlevel + 1));
              IN ptailpadwidth = pboxinnerwidth - prefixlen - pheadpadwidth;
              Un(Uboxhz, ptailpadwidth);
              IF (filesetix EQ 0) {
                U1(Uboxtr);         // this is the first
              } EL { U1(Uboxudr); } // this is the next
              INC filesetix; // first index is 0, reset at end of set (footer)
              Un(Uboxvt, dirlevel);
              _; // next line
              Un(Uboxvt, dirlevel + 1);
              CS dispsubname = dispname;
              IF (dispname[fnplix] EQ '+')
                { dispsubname = &dispname[fnplix + 1]; }
              Ws(dispsubname);
            } EL { // no prefix was discovered
              Un(Uboxvt, dirlevel);
              W1("%s", dispname);
            }
            STRF(firstfilename, "%s", filename); // copy first to cmp w/ last
            STRF(firstdispname, "%s", dispname); // copy first to cmp w/ last
          } EL { // this is not the first filename in this setrange
            IF (fileixofrange EQ 1) // first next file will match
              { Wc('-'); } // display one dash, no matter how many files in range
          }; INC fileixofrange; // display first number, later display range limit
          STRF(lastfilename, "%s", filename); // copy in case needed later
          FILE *fs = OPENFILE(filename);
          IF (!fs) { LOGF("Cannot open %s\n", filename); }
          IN thiscount = 0;
          LFP thisvalue = &thesevalues[0]; // set again later
          CH thisvalueneg = 0;
          CH mode = '.'; // '+' '='
          WI (1) {
            IN ch = GETFCH(fs);
            BKEQEOF(ch);
            IF (ch EQ '\n') { LOG("/"); }
            EF (ch LT ' ') { Mi(ch); }
            EF (ch EQ ',') {
              // indicates another sample in this sample set.. can ignore
            } EF (ch EQ ' ') {
              // skip spaces, but end + or = mode
              IF (mode EQ '+') {
                totalcount += thiscount;
                mode = '.';
              } EF (mode EQ '=') {
                Rxc(96); // this shouldn't happen (valueVALUE)
                mode = '.';
              }
            } EF (ch EQ '+') {  //Mc('+');
              mode = '+';
              thiscount = 0;
            } EF (mode EQ '+' AND ISNUMBER(ch)) {                // start of repeat ..........
              thiscount = (thiscount * 10) + (ch - A0); // reads each char
            } EF (mode EQ '.' AND (ch EQ '-' OR ISNUMBER(ch))) { // start of sample ...........
              IF (totalcount GT 0) { // display previous number, read a sample
                IF (fileixofrange GT 1) {
                  CS firstnamep = firstfilename;
                  IN ffi = 0;
                  WI (firstfilename[ffi] EQ dirname[ffi]) { INC ffi; }
                  IF (dirname[ffi] EQNUL) // firstfilename matched dirname as prefix
                    { firstnamep = &firstfilename[ffi]; } // match remaining string
                  WI (dispname[0] EQ firstnamep[0] AND firstnamep[0] NQNUL) {
                    IF (firstnamep[0] EQ '+') {
                      INC dispname; INC firstnamep; // further truncate filename by firstname
                      BK; // stop at + in first instance
                    } EL { INC dispname; INC firstnamep; }
                  }
                  IN nowhrminsecs = atoi(dispname);
                  IN firsthrminsecs = atoi(firstnamep);
                  WI (dispname[0] EQ firstnamep[0] AND firstnamep[0] NQNUL)
                    { INC dispname; INC firstnamep; } // eat the rest, after +
                  L1("%s ", dispname);
                  IN nowhrs    =  nowhrminsecs / 10000;
                  IN nowmins   = (nowhrminsecs % 10000) / 100;
                  IN nowsecs   =  nowhrminsecs % 100;
                  IN firsthrs  =  firsthrminsecs / 10000;
                  IN firstmins = (firsthrminsecs % 10000) / 100;
                  IN firstsecs =  firsthrminsecs % 100;
                  IN secdiff = nowsecs - firstsecs;
                  IN mindiff = nowmins - firstmins;
                  IN hrdiff = nowhrs - firsthrs;
                  IF (secdiff LT 0)
                    { mindiff SUBS 1; secdiff ADDS 60; }
                  IF (mindiff LT 0)
                    { hrdiff SUBS 1; mindiff ADDS 60; }
                  GOTOCOL(TAILCATHEADTIMECOL);
                  IF (hrdiff NQ 0) {
                    C3("%2d:%02d:%02d", hrdiff, mindiff, secdiff);
                  } EF (mindiff NQ 0) {
                    C2("%5d:%02d", mindiff, secdiff);
                  } EL { C1("%8d", secdiff); }
//                } EL {
//                  GOTOCOL(TAILCATHEADTIMECOL);
//                  C1("%8d", 0);
                }
                IF (fileixofrange EQ 0) {
                  IN extralevel = (currentprefix[0] NQNUL) ? 1 : 0;
                  Un(Uboxvt, dirlevel + extralevel);
                  GOTOCOL(TAILCATHEADTIMECOL);
                  C1("%8d", 0);
                  GOTOCOL(TAILCATHEADWIDTH);
                } EF (fileixofrange EQ 1) {
                  GOTOCOL(TAILCATHEADTIMECOL);
                  C1("%8d", 0);
                  GOTOCOL(TAILCATHEADWIDTH);
                } EL {
                  GOTOCOL(TAILCATHEADWIDTH);
                }
                fileixofrange = 0; // reset counter, new value expected soon
                IN totalvalues = thisindex; // + 1;
                IN pni = -1;
                WI (INC pni LT totalvalues) {
                  IF (censormode EQ 1) {
                    Ylfnf(thesevalues[pni], 5, 1);
                    printf("       ");
                  } EL {
                    Ylfnf(thesevalues[pni], 5, 8);
                  }
                }
                IN boxrightoffset = 1 + 5 + 1;
                IF (filesetix GT 0)
                  { boxrightoffset += 1; }
                GOTOCOL(1 + boxwidth - dirlevel - boxrightoffset);
                Wc('('); G1("%5d", totalcount); Wc(')');
                IF (filesetix GT 0) {
                  Un(Uboxvt, dirlevel + 1); _;
                } EL { Un(Uboxvt, dirlevel); _; }
                totalcount = 0;
              } // end of displaying previous number
              mode = '='; // set value buffers
              // but if there were 0 values then set doesn't happen..
              totalcount = 1; // this is a new sample
              thisvalue = &thesevalues[thisindex = 0];
              INC thisindex; // counter = 1
              IN nextch = ch; // send first char
              *thisvalue = lfreadfslf(fs, &nextch);
              WI (nextch EQ '.' OR nextch EQ ',') {
                IF (nextch EQ '.') { // more of this set
                  thisvalue = &thesevalues[thisindex];
                  INC thisindex; // counter = N
                  nextch = NUL; // no need to read/send first char
                  *thisvalue = lfreadfslf(fs, &nextch);
                } EL {
                  Yxc(96); // multivalue,not,implemented
                  BK;
//                  thesevalues = &thosevalues[INC thatindex];
//                  nextch = NUL;
// need to store thisindex set, INC thisindex, *thisindex = 0
                }
              }
              IF (nextch EQ ' ') {
                mode = '.'; // end of sample, do not display until +dupes counted
              } EF (nextch EQ '+') { // could list dupes with no padding
                mode = '+';  // Yc('+');
                thiscount = 0; // reset dupe add value
              } EL { //EF (nextch EQ ',') {
                // mode stays as = ...
                R1("!?:%c", nextch);
              }
            }
          }
          CLOSEFILE(fs);
        } EF (filetype EQ DT_DIR AND pass EQ 1) {
// ------------------------------------------------------------- REGULAR DIRECTORY ------------->>--
          IF STREQ(filename,  ".") { NOP } //Ws(". ");  }
          EF STREQ(filename, "..") { NOP } //Ys(".. "); }
          EL {
            IF (chdir(filename) EQ 0) {
              // to put this into the function would be unsafe ... cd . , cd .. is not neutral
              IN filenamelen = strlen(filename) + 2 + 2; // + 2 spaces
              IN boxinnerwidth = boxwidth - 2;
              IN headpadwidth = TAILCATHEADPADWIDTH;
              IN tailpadwidth = boxinnerwidth - filenamelen - headpadwidth;
              Un(Uboxvt, dirlevel);
              U1(Uboxtl);
              Un(Uboxhz, headpadwidth);
              Ms(" \xE5\xA4\xB9"); // 2 chars wide (in console ..)
              C1("%s ", filename);
              Un(Uboxhz, tailpadwidth - (2 * dirlevel));
              U1(Uboxtr);
              Un(Uboxvt, dirlevel); _;
              showdir(filename, dirlevel + 1); // jump into subdir (by name only)
              IF (chdir("..") NQ 0) { Rs(".. fail"); RT; }
              Un(Uboxvt, dirlevel);
              U1(Uboxbl);
              Un(Uboxhz, boxinnerwidth - (2 * dirlevel));
              U1(Uboxbr);
              Un(Uboxvt, dirlevel); _;
              currentprefix[0] = NUL;
              // Ms(".."); Ws(":\n");
            } EL { R1("cannot open dir %s\n", filename); RT; }
          }
        } // EL { LOGF("Unrecognised file type for %s\n", filename); } // buneng cos passes
//      free(namelist[fileix]); <-- deallocate later
      } // end of ordered file lookup loop
      IF (totalcount GT 0) { // display last number if one is waiting
        IF (fileixofrange GT 1) {
          CS firstnamep = firstfilename;
          IN ffi = 0;
          WI (firstfilename[ffi] EQ dirname[ffi]) { INC ffi; }
          IF (dirname[ffi] EQNUL) // firstfilename matched dirname as prefix
            { firstnamep = &firstfilename[ffi]; } // match remaining string
          CS dirnamep = dirname;
          CS dispname = lastfilename;
          WI (ISNUMBER(dirnamep[0]) OR dirnamep[0] EQ '+') { INC dirnamep; }
          IF (dirnamep[0] EQNUL) { // only truncate using numeric dirs (allow +)
            dirnamep = dirname;
            WI (dispname[0] EQ dirnamep[0] AND dirnamep[0] NQNUL)
              { INC dispname; INC dirnamep; } // truncate filename by dirname
          }
          WI (dispname[0] EQ firstnamep[0] AND firstnamep[0] NQNUL) {
            IF (firstnamep[0] EQ '+') {
              INC dispname; INC firstnamep; // further truncate filename by firstname
              BK; // stop at + in first instance
            } EL { INC dispname; INC firstnamep; }
          }
          IN nowhrminsecs = atoi(dispname);
          IN firsthrminsecs = atoi(firstnamep);
          WI (dispname[0] EQ firstnamep[0] AND firstnamep[0] NQNUL)
            { INC dispname; INC firstnamep; } // further truncate filename by firstname
          L1("%s ", dispname);
          IN nowhrs    =  nowhrminsecs / 10000;
          IN nowmins   = (nowhrminsecs % 10000) / 100;
          IN nowsecs   =  nowhrminsecs % 100;
          IN firsthrs  =  firsthrminsecs / 10000;
          IN firstmins = (firsthrminsecs % 10000) / 100;
          IN firstsecs =  firsthrminsecs % 100;
          IN secdiff = nowsecs - firstsecs;
          IN mindiff = nowmins - firstmins;
          IN hrdiff = nowhrs - firsthrs;
          IF (secdiff LT 0)
            { mindiff SUBS 1; secdiff ADDS 60; }
          IF (mindiff LT 0)
            { hrdiff SUBS 1; mindiff ADDS 60; }
          GOTOCOL(TAILCATHEADTIMECOL);
          IF (hrdiff NQ 0) {
            C3("%2d:%02d:%02d", hrdiff, mindiff, secdiff);
          } EF (mindiff NQ 0) {
            C2("%5d:%02d", mindiff, secdiff);
          } EL { C1("%8d", secdiff); }
//        } EL {
//          GOTOCOL(TAILCATHEADTIMECOL);
//          C1("%8d", 0);
        }
        IF (fileixofrange EQ 0) {
          IN extralevel = (currentprefix[0] NQNUL) ? 1 : 0;
          Un(Uboxvt, dirlevel + extralevel);
          GOTOCOL(TAILCATHEADTIMECOL);
          C1("%8d", 0);
          GOTOCOL(TAILCATHEADWIDTH);
        } EF (fileixofrange EQ 1) {
          GOTOCOL(TAILCATHEADTIMECOL);
          C1("%8d", 0);
          GOTOCOL(TAILCATHEADWIDTH);
        } EL {
          GOTOCOL(TAILCATHEADWIDTH);
        }
        fileixofrange = 0; // reset counter, tho no new value expected...
        IN totalvalues = thisindex; // + 1;
        IN pni = -1;
        WI (INC pni LT totalvalues) {
          IF (censormode EQ 1) {
            Ylfnf(thesevalues[pni], 5, 1);
            printf("       ");
          } EL {
            Ylfnf(thesevalues[pni], 5, 8);
          }
        }
        IN boxrightoffset = 1 + 5 + 1;
        IF (filesetix GT 0)
          { boxrightoffset += 1; }
        GOTOCOL(1 + boxwidth - dirlevel - boxrightoffset);
        Wc('('); G1("%5d", totalcount); Wc(')');
        IF (filesetix GT 0) {
          Un(Uboxvt, dirlevel + 1); _;
        } EL { Un(Uboxvt, dirlevel); _; }
        totalcount = 0;
        IF (filesetix GT 0) { // at least one file set was recorded
          IN pboxinnerwidth = boxwidth - (2 * (dirlevel + 1));
          Un(Uboxvt, dirlevel);
          U1(Uboxbl);
          Un(Uboxhz, pboxinnerwidth);
          U1(Uboxbr);
          Un(Uboxvt, dirlevel); _; // Gc('^'); _;
          filesetix = 0; // reset
        }
      }
    } // WHILE PASS IS 1 OR 2 ............ ; //Ws("\n");
    IN ni = -1;
    WI (INC ni LT numfiles)
      { free(namelist[ni]); }
    free(namelist);
  } EL { LOGF("Cannot open directory %s\n", dirname); }
}
