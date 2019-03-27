/* reads from [stream] into [buffer] until terminated by
 * \r, \n or EOF, or [lastnullindex] is reached. Returns
 * the number of characters read excluding the terminating
 * character. [lastnullindex] refers to the uppermost index
 * of the [buffer] array. If an error occurs or non-text
 * characters (below space ' ' or above tilde '~') are
 * detected, the buffer will be emptied and 0 returned.
^^ needs update, return -1 on error, -2 on EOF
 */
int readline(FILE *stream, char *buffer, int lastnullindex, int htmlsafe) {
  if (!stream) return -1;
  if (!buffer) return -1;
  if (lastnullindex < 0) return -1;
  int inch = EOF;
  int chi = 0;
  int eof = 0;
  while (chi < lastnullindex) {
    inch = fgetc(stream);
    if (inch == EOF) {
      buffer[chi] = '\0';
      eof = 1;
      break;
    } else if (inch == '\n' || inch == '\r') {
      buffer[chi] = '\0';
      break;
    } else if (inch >= ' ' && inch <= '~') {
      if (htmlsafe == 1) {
        if (inch == '<') { inch = '#'; }
        if (inch == '>') { inch = '#'; }
        if (inch == '"') { inch = '#'; }
      }
      buffer[chi] = (char)inch;
      chi++;
    } else {
      buffer[chi] = '#';
      chi++;
//      buffer[0] = '\0';
//      return -1;
    }
  }
  if (chi < 0 || chi > lastnullindex) {
    buffer[0] = '\0';
    return -1;
  } else {
    buffer[chi] = '\0';
    if (eof == 1)
      { return -2 - chi; }
    else { return chi; }
  }
}

