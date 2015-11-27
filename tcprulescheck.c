/*
vim:tw=76:ts=2:sw=2:cindent:expandtab
*/

#include <unistd.h>

#include "byte.h"
#include "buffer.h"
#include "strerr.h"
#include "env.h"
#include "rules.h"
#include "scan.h"
#include "getln.h"
#include "open.h"
#include "str.h"

static unsigned char delimch;
static unsigned long fieldnr;
static char *filterenv;
static unsigned int filterenvlen;
static char *fnrules;
static int flaginvert;
static int flagip;

static stralloc line = {0};
static stralloc filterln = {0};
static int match = 1;

static void die_mem(void)
{
  strerr_die1sys(111,"tcprulescheck: fatal: out of memory");
}

void found(char *data,unsigned int datalen)
{
  unsigned int next0;
  int flagfound = 0;

  if (filterenv) {
    while ((next0 = byte_chr(data,datalen,0)) < datalen) {
      switch(data[0]) {
        case '+':
          if (!byte_diff(data + 1, filterenvlen, filterenv) && data[filterenvlen+1] == '=') {
            flagfound = 1;
          }
          break;
      }
      ++next0;
      data += next0; datalen -= next0;
    }
    if ((flagfound && !flaginvert) || (!flagfound && flaginvert)) {
      buffer_put(buffer_1, line.s, line.len);
      buffer_puts(buffer_1,"\n");
      return;
    }
  } else {
    buffer_puts(buffer_1,"rule ");
    buffer_put(buffer_1,rules_name.s,rules_name.len);
    buffer_puts(buffer_1,":\n");
    while ((next0 = byte_chr(data,datalen,0)) < datalen) {
      switch(data[0]) {
        case 'D':
    buffer_puts(buffer_1,"deny connection\n");
    buffer_flush(buffer_1);
    _exit(0);
        case '+':
    buffer_puts(buffer_1,"set environment variable ");
    buffer_puts(buffer_1,data + 1);
    buffer_puts(buffer_1,"\n");
    break;
      }
      ++next0;
      data += next0; datalen -= next0;
    }
    buffer_puts(buffer_1,"allow connection\n");
    buffer_flush(buffer_1);
    _exit(0);
  }
}

int main(int argc,char **argv)
{
  int fd;
  char *ip;
  char *info;
  char *host;
  char *p;
  unsigned long u;
  unsigned int i;
  unsigned long currfield;
  char *x;
  char ch;
  unsigned long len;
  unsigned long delim;
  unsigned long delimend;

  fnrules = argv[1];
  if (!fnrules)
    strerr_die1x(100,"tcprulescheck: usage: tcprulescheck rules.cdb");

  ip = env_get("TCPREMOTEIP");
  if (!ip) ip = "0.0.0.0";
  info = env_get("TCPREMOTEINFO");
  host = env_get("TCPREMOTEHOST");
  p = env_get("TCPFILTERDELIM");
  if (p) delimch = *p;
  p = env_get("TCPFILTERFIELD");
  if (p) {
    i = scan_ulong(p, &u);
    if (i) fieldnr = u;
  }
  p = env_get("TCPFILTERENV");
  if (p) {
    filterenv = p;
    filterenvlen = str_len(filterenv);
  }
  if (env_get("TCPFILTERINVERT")) flaginvert = 1;
  if (env_get("TCPFILTERIP")) flagip = 1;

  if (delimch && !filterenv)
    strerr_die1x(100,"tcprulescheck: env var TCPFILTERENV not set");  

  if (filterenv) {
    fd = open_read(fnrules);
    if (fd == -1) {
        strerr_die3sys(111,"tcprulescheck: fatal: unable to open",fnrules,": ");
    }

    while (match) {
      if (getln(buffer_0,&line,&match,'\n') == -1)
        strerr_die1sys(111,"tcprulescheck: fatal: unable to read input: ");

      x = line.s; len = line.len;

      if (!len) break;
      if (x[0] == '#') continue;
      if (x[0] == '\n') continue;

      while (len) {
        ch = x[len - 1];
        if (ch != '\n') if (ch != ' ') if (ch != '\t') break;
        --len;
      }
      line.len = len;

      if (delimch) {
        if (flaginvert && byte_chr(x, len, delimch) == len) {
          buffer_put(buffer_1, x, len);
          buffer_puts(buffer_1,"\n");
          continue;
        }
        for (currfield = 0; currfield < fieldnr; currfield++) {
          delim = byte_chr(x, len, delimch);
          if (delim == len) break;
          x += delim + 1;
          len -= delim + 1;
        }
        if (len == 0) continue;
        delimend = byte_chr(x, len, delimch);
      } else {
        delimend = len;
      }
      if (!stralloc_copyb(&filterln, x, delimend)) die_mem(); 
      if (!stralloc_0(&filterln)) die_mem();
      switch (rules(found, fd, (flagip) ? filterln.s : ip,
                               (flagip) ? 0 : filterln.s, 0)) {
        case -1:
          strerr_die3sys(111,"tcprulescheck: fatal: unable to read",fnrules,": ");
          break;
      }
    }
  } else {
    fd = open_read(fnrules);
    if ((fd == -1) || (rules(found,fd,ip,host,info) == -1))
      strerr_die3sys(111,"tcprulescheck: fatal: unable to read ",fnrules,": ");
  }

  if (!filterenv) buffer_puts(buffer_1,"default:\nallow connection\n");
  buffer_flush(buffer_1);
  _exit(0);
}

