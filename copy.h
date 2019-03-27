int copy(char *from, char *to) {
  FILE *ffrom = fopen(from, "r");
  if (!ffrom) return 0;
  FILE *fto = fopen(to, "w");
  if (!fto) return 0;
  int inch;
  while (inch = fgetc(ffrom)) {
    if (inch == EOF) break;
    if (fputc(inch, fto) == EOF) break;
  }
  fclose(fto);
  fclose(ffrom);
  return 1;
}
