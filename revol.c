#include "revol.h"

static const char * tag_name[] = {
  [FREE] = "FREE",
  [LITERAL] = "LITERAL",
  [ASSOC] = "ASSOC",
  [GET] = "GET"
};

static const char seperators[] = {
  ' ', '\n',
  '[', ']', '(', ')', '{', '}',
  '"', ':', ';', '/',
  '\0'
};

static bool is_seperator(char c) {
  for (size_t i = 0; i < strlen(seperators); i++) {
    if (c == '\0' || seperators[i] == c) {
      return true;
    }
  }
  
  return false;
}

static char content[255];

static rev_word * read_word(char * source, bool * skipped) {
  char c = source[0];
  rev_word_tag tag = FREE;
  size_t len = 0;

  if (c == ':') {
    tag = GET;
    source++;
    c = source[0];
    *skipped = true;
  } else if (c == '\'') {
    tag = LITERAL;
    source++;
    c = source[0];
    *skipped = true;
  }

  while (!is_seperator(c)) {
    content[len] = c;
    c = source[++len];
  }

  if (c == ':') {
    tag = ASSOC;
    *skipped = true;
  }

  content[len] = '\0';

  rev_word * word = malloc(sizeof(rev_word));
  word->tag = tag;
  word->content = malloc(sizeof(char) * len);
  memcpy(word->content, content, len);

  return word;
}

static rev_string * read_string(char * source) {
  size_t len = 0;

  do { len++; } while (source[len] == '\\' || source[len] != '"');

  len--;

  rev_string * string = malloc(sizeof(rev_string));
  char * content = malloc(sizeof(char) * len);
  memcpy(content, ++source, len);
  content[len] = '\0';
  string->content = content;

  return string;
}

static char * read_number(char * source) {
  size_t len = 0;

  while (source[len] == '.' || ('0' <= source[len] && source[len] <= '9')) {
    len++;
  };

  char * result = malloc(sizeof(char) * len);
  result[len] = '\0';
  memcpy(result, source, len);

  return result;
}

int main(int argc, char *argv[]) {
  FILE *input;
  int length;
  char *source;

  if (argc < 2) {
    fprintf(stderr, "usage: revol <input>\n");
    exit(1);
  }

  input = fopen(argv[1], "r");
  if (input == NULL) {
    fprintf(stderr, "invalid input file\n");
    exit(1);
  }

  fseek(input, 0, SEEK_END);
  length = (int)ftell(input);
  fseek(input, 0, SEEK_SET);
  source = malloc(length + 1);
  fread(source, 1, length, input);
  source[length] = '\0';
  fclose(input);

  char * slice = source;
  while (slice != &(source[length])) {
    switch (slice[0]) {
      case ' ': case '\n': case '\t': {
        slice++;
        break;
      }
      case '"':
        rev_string * string = read_string(slice);
        fprintf(stdout, string->content);
        fprintf(stdout, "\t \"STRING\"\n");
        slice = &(slice[strlen(string->content) + 2]);
        break;
      default: {
        if ('0' <= slice[0] && slice[0] <= '9') {
          char * number = read_number(slice);
          double n = strtod(number, NULL);
          fprintf(stdout, number);
          fprintf(stdout, "\t \"NUMBER\"\n");
          slice = &(slice[strlen(number)]);
          break;
        };
        bool skipped = false;
        rev_word * word = read_word(slice, &skipped);
        fprintf(stdout, word->content);
        fprintf(stdout, "\t");
        fprintf(stdout, tag_name[word->tag]);
        fprintf(stdout, "\n");
        slice = &(slice[strlen(word->content) + skipped]);
        break;
      }
    }
  }

  slice = NULL;
  free(source);
  return 0;
}