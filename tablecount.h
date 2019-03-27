char tableresetcounter(char *name, char source, char *label) {
  int tli = tablelabelindex(name, source, label);
  if (tli > 0) {
    char tsr = tablesetrow(name, source, label, "0");
    return (tsr == 's') ? '0' : '-';
  } else {
    char tar = tableaddrow(name, source, label, "0");
    return (tar == 'a') ? '0' : '-';
  }
}

char tablecheckcounter(char *name, char source, char *label) {
  int tli = tablelabelindex(name, source, label);
  if (tli > 0) {
    char tval[TABLEMAXVALUESIZE + 1];
    char tlv = tablelabelvalue(tval, name, source, label);
    if (tlv == 'v') {
      if (tval[0] >= '0' && tval[1] == '\0')
        { return tval[0]; } else { return '-'; }
    } else { return '-'; }
  } else { return '-'; }
}

char tablecountcounter(char *name, char source, char *label) {
  int tli = tablelabelindex(name, source, label);
  if (tli > 0) {
    char tval[TABLEMAXVALUESIZE + 1];
    char tlv = tablelabelvalue(tval, name, source, label);
    if (tlv == 'v') {
      if (tval[0] >= '0' && tval[1] == '\0') {
        tval[0] = (tval[0] >= '9') ? '1' : (tval[0] + 1);
        char tsr = tablesetrow(name, source, label, tval);
        if (tsr == 's') { return tval[0]; } else { return '-'; }
      } else { return '-'; }
    } else { return '-'; }
  } else { return '-'; }
}
