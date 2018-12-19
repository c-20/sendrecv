#include "code.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

#define send main
#define SENDMAXNAMELEN   128
#define SENDMEMDIR   "/dev/shm/ltl"

IN send($) {
  printf("Content-type: text/html\r\n\r\n");
  CHDIRORFAIL(SENDMEMDIR) { LOG("NOACCESS"); RT 0; }
  CH sessionmode = NUL;
  CS querystr = getenv("QUERY_STRING");
  IF (querystr[0] EQ '+' AND querystr[1] EQ '+') {
    sessionmode = 'N'; // new session mode
    IF (querystr[2] NQNUL) {
      CS instancename = &querystr[2];
      IN mk = mkdir(instancename, 0777);
      IF (mk NQ 0 AND errno EQ EEXIST) { // if dir exists
        CHDIRORFAIL(instancename) // move into existing dir
          { printf("{ \"fail\": \"%s (%s)\", \"op\": \"%c\" }",
                          instancename, strerror(errno), '+'); RT 0; }
        printf("{ \"in\": \"%s\", \"op\": \"%c\" }", instancename, '+');
      } EF (mk NQ 0) {               // else if mkdir failed
        printf("{ \"fail\": \"%s (%s)\" }", instancename, strerror(errno));
      } EL { // mk was 0 - dir was created
        CHDIRORFAIL(instancename) // move into new dir
          { printf("{ \"fail\": \"%s (%s)\", \"op\": \"%c\" }",
                          instancename, strerror(errno), '='); RT 0; }
        printf("{ \"in\": \"%s\", \"op\": \"%c\" }", instancename, '=');
      }
    } EL { // no instance name was given
      CS remoteaddr = getenv("REMOTE_ADDR");
      IF (!remoteaddr) { remoteaddr = "0.0.0.0"; }
      printf("{ \"id\": \"%s\" }", remoteaddr);
    }
  } EF STREQ(querystr, "+") {
    sessionmode = 'D'; // data mode
  }
  IN clength = 0;
  CS contentlen = getenv("CONTENT_LENGTH");
  IF (contentlen) { clength = atoi(contentlen); }
  CH nameval[SENDMAXNAMELEN];
  IN namevali = 0;
  nameval[namevali] = NUL;
  CH subnameval[SENDMAXNAMELEN];
  IN subnamevali = 0;
  subnameval[subnamevali] = NUL;
  CH mode = 'n'; // expecting param name
  CH namechar = NUL;
  IN datalevel = 0;
  IN dateval = 0;
  IN timeval = 0;
  CH datemode = 'D'; // N for subname- prefix
  FS datafile = NULL;
  IN ci = -1;
  WI (INC ci LT clength) {
    IN cc = GETFCH(stdin);
    BKEQEOF(cc);
    IF (mode EQ 'n' AND cc EQ '=') { // start of value
      IF (namechar EQ 's')
        { mode = 's'; } EL { mode = 'v'; }
    } EFEQ3(cc, '&', '\n', '\0') { // end of value
      mode = 'n';
    } EF (mode EQ 'n') {
      namechar = cc;
    } EF (mode EQ 'v') {
      IF (namechar EQ 'n') {
        nameval[namevali] = cc;
        nameval[INC namevali] = NUL;
      }
    } EF (mode EQ 's') {
      IF (cc EQ '+') {
        cc = ' ';
      } EF (cc EQ '%') {
        IN hex1 = GETFCH(stdin);
        IN hex2 = GETFCH(stdin);
        cc = (HEXCHVALUE(hex1) * 16) + HEXCHVALUE(hex2);
      }
      IF (cc EQ '{') {
        INC datalevel;
        IF (datalevel EQ 1) {
          IF (nameval[0] EQNUL) { LOGF("NONAME"); RT 0; }
          CHDIRORFAIL(nameval) { LOGF("NOSESSION: %s", strerror(errno)); RT 0; }
          CH filename[SENDMAXNAMELEN];
          IF (subnameval[0] NQNUL) {
            STRF(filename, "%s-%06d+%06d", subnameval, dateval, timeval);
          } EL { STRF(filename, "%06d+%06d", dateval, timeval); }
          datafile = SAVEFILE(filename);
          IF (!datafile) { LOGF("SAVEERROR: %s", strerror(errno)); RT 0; }
        }
      } EF (cc EQ '}') {
        DEC datalevel;
        IF (datalevel EQ 0 AND datafile NQNULL) {
          CLOSEFILE(datafile);
          datafile = NULL;
          IN filecount = 0;
          struct dirent *entry;
          DIR *dirp = opendir(".");
          WI ((entry = readdir(dirp)) != NULL) {
            IF (entry->d_type EQ DT_REG)
              { INC filecount; } // count files only
          }; closedir(dirp);
          printf("[%d]", filecount);
        }
      } EF (datalevel EQ 0) {
        IF (cc EQ '-') {
          IF (datemode EQ 'N') { datemode = 'D'; }
        } EF (cc EQ '+') {
          IF (datemode EQ 'D') { datemode = 'T'; }
        } EF ISNUMBER(cc) {
          IF (datemode EQ 'D') { dateval = (dateval * 10) + (cc - '0'); }
          IF (datemode EQ 'T') { timeval = (timeval * 10) + (cc - '0'); }
        } EF (datemode EQ 'N') {
          subnameval[subnamevali] = cc;
          subnameval[INC subnamevali] = NUL;
        }
      } EF (datalevel EQ 1) {
        IF (PUTFCH(datafile, cc) EQEOF)
          { LOGF("SAVECHFAIL"); RT 0; }
        //printf("%c", cc);
      }
    } EL { printf("<!%c!>", cc); }
  }  
  IF (datafile NQNULL) // in case file was left open (no } in packet)
    { CLOSEFILE(datafile); datafile = NULL; }
  RT 0;
}
