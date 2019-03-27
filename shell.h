#define SHELLOUTPATHNAMESIZE		1024

// srcdir no longer includes trailing / !
char shellexec(char *cmdname, char *srcdir, char *label) {
  char nameca[SHELLOUTPATHNAMESIZE];
  sprintf(nameca, "%s", cmdname);
  char outname[SHELLOUTPATHNAMESIZE];
  sprintf(outname, "%s/@%s", srcdir, label);
  char *args[6];
  args[0] = args[1] = args[2] = NULL;
  args[3] = args[4] = args[5] = NULL;
  int ncl = -1;
  while (nameca[++ncl] != '\0');
  if (nameca[ncl - 1] != ']')
    { return ']'; }
  nameca[ncl - 1] = '\0';
  int ncleast = ncl;
  while (--ncl >= 0 && nameca[ncl] != ',');
  if (ncl >= 0) {
    args[1] = &nameca[ncl + 1];
    nameca[ncl] = '\0';
    ncleast = ncl;
    while (--ncl >= 0 && nameca[ncl] != ',');
    if (ncl >= 0) {
      args[2] = args[1];
      args[1] = &nameca[ncl + 1];
      nameca[ncl] = '\0';
      ncleast = ncl;
      while (--ncl >= 0 && nameca[ncl] != ',');
      if (ncl >= 0) {
        args[3] = args[2];
        args[2] = args[1];
        args[1] = &nameca[ncl + 1];
        nameca[ncl] = '\0';
        ncleast = ncl;
        while (--ncl >= 0 && nameca[ncl] != ',');
        if (ncl >= 0) {
          args[4] = args[3];
          args[3] = args[2];
          args[2] = args[1];
          args[1] = &nameca[ncl + 1];
          nameca[ncl] = '\0';
          ncleast = ncl;
        }
      }
    }
  }
//  args[1] = &nameca[ncl + 1];
//  nameca[ncl] = '\0';
  while (--ncleast >= 0 && nameca[ncleast] != '[');
  if (ncleast < 0 || nameca[ncleast] != '[')
    { return '['; }
  args[0] = &nameca[ncleast + 1];
  if (args[2] == NULL && ncleast > 0)
    { args[2] = &nameca[0]; args[3] = NULL; }
  char popname[SHELLOUTPATHNAMESIZE];
  if (args[4] != NULL) {
    sprintf(popname, "%s %s %s %s %s > %s", args[0], args[1], args[2], args[3], args[4], outname);
  } else if (args[3] != NULL) {
    sprintf(popname, "%s %s %s %s > %s", args[0], args[1], args[2], args[3], outname);
  } else if (args[2] != NULL) {
    sprintf(popname, "%s %s %s > %s", args[0], args[1], args[2], outname);
  } else if (args[1] != NULL) {
    sprintf(popname, "%s %s > %s", args[0], args[1], outname);
  } else if (args[0] != NULL) {
    sprintf(popname, "%s > %s", args[0], outname);
  } else { return 'p'; }
  int pid = fork();
  if (pid < 0) { return 'e'; }
  else if (pid == 0) {
//    int stdwut = dup(1);
//    int fd = open(outname, O_RDWR | O_CREAT | O_TRUNC);
//    if (fd < 0) { _exit(1); }
//    dup2(fd, 1);
//    fcntl(fd, F_SETFD, FD_CLOEXEC);
//    execvp(args[0], args);
//    dup2(stdwut, 1);
//    close(stdwut);
//    close(fd);
    FILE *fd = popen(popname, "r");
    pclose(fd);
    _exit(0);
  } else {
    int status;
    waitpid(pid, &status, 0);
    return (status == 0) ? 'y' : 'f';
  }
  return 'd';
}
//  if (!stayhome) { }
//  else { you.grinch(); }
