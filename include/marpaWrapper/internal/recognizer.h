#ifndef MARPAWRAPPER_INTERNAL_RECOGNIZER_H
#define MARPAWRAPPER_INTERNAL_RECOGNIZER_H

#include <stddef.h>
#include "marpaWrapper/recognizer.h"
#include "marpa.h"

typedef struct marpaWrapperRecognizerAlternative {
  int symboli;
  int valuei;
  int lengthi;
} marpaWrapperRecognizerAlternative_t;

struct marpaWrapperRecognizer {
  Marpa_Recognizer               marpaRecognizerp;
  marpaWrapperGrammar_t         *marpaWrapperGrammarp;
  marpaWrapperRecognizerOption_t marpaWrapperRecognizerOption;

  /* Storage of symbols for expected terminals */
  size_t                        sizeSymboll;        /* Allocated size */
  size_t                        nSymboll;           /* Used size      */
  int                          *symbolip;

  /* Storage of symbols given as alternative */
  size_t                               sizeAlternativel; /* Allocated size */
  size_t                               nAlternativel;    /* Used size      */
  marpaWrapperRecognizerAlternative_t *alternativeip;

  /* Temporary storage of accepted symbols given as alternative */
  size_t                               sizeAlternativeokl; /* Allocated size */
  size_t                               nAlternativeokl;    /* Used size      */
  marpaWrapperRecognizerAlternative_t *alternativeokip;

  /* Progress storage */
  size_t                               sizeProgressl; /* Allocated size */
  size_t                               nProgressl;    /* Used size      */
  marpaWrapperRecognizerProgress_t    *progressp;
};

#endif /* MARPAWRAPPER_INTERNAL_RECOGNIZER_H */
