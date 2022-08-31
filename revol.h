#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum rev_word_tag {
  FREE,
  LITERAL,
  ASSOC,
  GET
} rev_word_tag;

typedef struct rev_word {
  rev_word_tag tag;
  char * content;
} rev_word;

typedef struct rev_string {
  const char * content;
} rev_string;

typedef struct rev_number {
  double value;
} rev_number;

typedef struct rev_block {
  struct rev_value * values;
} rev_block;

typedef enum rev_tag {
  WORD,
  BLOCK,
  STRING,
  NUMBER
} rev_tag;

typedef struct rev_value {
  rev_tag tag;
  union {
    rev_word word;
    rev_block block;
    rev_string string;
    rev_number number;
  } u;
} rev_value;