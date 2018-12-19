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
  CS querystr = getenv("QUERY_STRING");
  IF (querystr NQNULL) {
    webmode = 1;
    IF (querystr[0] EQ 'X' AND querystr[1] EQNUL)
      { censormode = 1; }
  }
  IF (webmode EQ 1) {
    printf("Content-type: text/html; charset=utf-8\r\n\r\n");
    IF (censormode EQ 1) {
      printf("<html><head><title>RECV</title>\n");
    } EL {
      printf("<html><head><title>recv</title>\n");
    }
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
    printf("  .WC { display: inline-block; width: 18px;\n");
    printf("        font-weight: normal; font-size: 14px; }\n");
    printf("</style>\n");
    printf("<script type=\"text/javascript\">\n");
//    printf("  function id() { return document.getElementById(id); }     \n");
    printf("  function kgo() {                                            \n");
    printf("    var txt = document.body.innerHTML;                        \n");
    printf("    var newtxt = '';                                          \n");
    printf("    var esc = 0;                                              \n");
    printf("    var escix = 0;                                            \n");
    printf("    var esccol = 0;                                           \n");
    printf("    var escvalone = 0;                                        \n");
    printf("    var escvaltwo = 0;                                        \n");
    printf("    var col = 1;                                              \n");
    printf("    for (var i = 0; i < txt.length; i++) {                    \n");
    printf("      var txtch = txt.charAt(i);                              \n");
    printf("      if (txtch.charCodeAt(0) == 27) {                        \n");
    printf("        esc = 1;                                              \n");
    printf("        escix = i;                                            \n");
    printf("        esccol = col;                                         \n");
    printf("        escwidth = 1;                                         \n");
    printf("      } else if (esc == 1 && txtch == '[') {                  \n");
    printf("        esc = 2;                                              \n");
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
    printf("        } else if (esc == 0 && txtch == '\\n') {                 \n");
    printf("          col = 1; newtxt += '<br>\\n';                           \n");
    printf("        } else if (esc == 0 && txtch.charCodeAt(0) > 20000) {      \n");
    printf("          col += 2; newtxt += '<b class=\"WC\">' + txtch + '</b>';  \n");
    printf("        } else if (esc == 0) {                                      \n");
    printf("          col++; newtxt += txtch;                                  \n");
    printf("        } else { newtxt += txtch; }                               \n");
    printf("      }                                                          \n");
    printf("    }                                                           \n");
    printf("    document.body.innerHTML = newtxt;                        \n");
    printf("  }                                                          \n");
    printf("</script>\n");
    printf("</head><body onload=\"kgo();\">\n");
  }
  CHDIRORFAIL(RECVMEMDIR) { LOG("NOACCESS"); RT 0; }
  showdir(".", 0);
  IF (webmode EQ 1) {
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
                    B3("%2d:%02d:%02d", hrdiff, mindiff, secdiff);
                  } EF (mindiff NQ 0) {
                    B2("%5d:%02d", mindiff, secdiff);
                  } EL { B1("%8d", secdiff); }
                }
                IF (fileixofrange EQ 0) {
                  IN extralevel = (currentprefix[0] NQNUL) ? 1 : 0;
                  Un(Uboxvt, dirlevel + extralevel);
                  GOTOCOL(TAILCATHEADWIDTH);
                } EL {
                  GOTOCOL(TAILCATHEADWIDTH);
                }
                fileixofrange = 0; // reset counter, new value expected soon
                IN totalvalues = thisindex; // + 1;
                Bc('[');
                IN pni = -1;
                WI (INC pni LT totalvalues) {
                  IF (censormode EQ 1) {
                    Clfnf(thesevalues[pni], 5, 1);
                    printf("       ");
                  } EL {
                    Clfnf(thesevalues[pni], 5, 8);
                  }
                }
                Bc(']');
                IN boxrightoffset = 1 + 5 + 1;
                IF (filesetix GT 0)
                  { boxrightoffset += 1; }
                GOTOCOL(1 + boxwidth - dirlevel - boxrightoffset);
                Wc('('); C1("%5d", totalcount); Wc(')');
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
            B3("%2d:%02d:%02d", hrdiff, mindiff, secdiff);
          } EF (mindiff NQ 0) {
            B2("%5d:%02d", mindiff, secdiff);
          } EL { B1("%8d", secdiff); }
        }
        IF (fileixofrange EQ 0) {
          IN extralevel = (currentprefix[0] NQNUL) ? 1 : 0;
          Un(Uboxvt, dirlevel + extralevel);
          GOTOCOL(TAILCATHEADWIDTH);
        } EL {
          GOTOCOL(TAILCATHEADWIDTH);
        }
        fileixofrange = 0; // reset counter, tho no new value expected...
        IN totalvalues = thisindex; // + 1;
        Mc('['); // Wi(totalvalues); Ys(": ");
        IN pni = -1;
        WI (INC pni LT totalvalues) {
          IF (censormode EQ 1) {
            Clfnf(thesevalues[pni], 5, 1);
            printf("       ");
          } EL {
            Clfnf(thesevalues[pni], 5, 8);
          }
        }
        Mc(']');
        IN boxrightoffset = 1 + 5 + 1;
        IF (filesetix GT 0)
          { boxrightoffset += 1; }
        GOTOCOL(1 + boxwidth - dirlevel - boxrightoffset);
        Wc('('); C1("%5d", totalcount); Wc(')');
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
