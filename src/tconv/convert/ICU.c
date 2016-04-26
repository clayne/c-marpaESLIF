#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unicode/uconfig.h>
#include <unicode/ucnv.h>
#include <unicode/utrans.h>
#include <unicode/uset.h>
#include <unicode/ustring.h>

#include "tconv/convert/ICU.h"
/* Because this is a built-in, it can take advantage of TCONV_TRACE macro */
#include "tconv_config.h"

#define TCONV_ICU_IGNORE   "//IGNORE"
#define TCONV_ICU_TRANSLIT "//TRANSLIT"

/* Default option */
#define TCONV_ENV_CONVERT_ICU_UCHARLENGTH "TCONV_ENV_CONVERT_ICU_UCHARLENGTH"
#define TCONV_ENV_CONVERT_ICU_FALLBACK  "TCONV_ENV_CONVERT_ICU_FALLBACK"

tconv_convert_ICU_option_t tconv_convert_icu_option_default = {
  1024*1024, /* uCharLengthl */
  0          /* fallbackb */
};

/* Context */
typedef struct tconv_convert_ICU_context {
  UConverter                 *uConverterFromp;  /* Input => UChar  */
  UChar                      *uCharBufp;        /* UChar buffer    */
  size_t                      uCharLengthl;     /* Size of intermediary buffer. Default: 1024*1024 */
  UConverter                 *uConverterTop;    /* UChar => Output */
  UTransliterator            *uTransliteratorp; /* Transliteration */
} tconv_convert_ICU_context_t;

/*****************************************************************************/
void  *tconv_convert_ICU_new(tconv_t tconvp, const char *tocodes, const char *fromcodes, void *voidp)
/*****************************************************************************/
{
  static const char            funcs[]          = "tconv_convert_ICU_new";
  tconv_convert_ICU_option_t  *optionp          = (tconv_convert_ICU_option_t *) voidp;
  UBool                        ignoreb          = FALSE;
  UBool                        translitb        = FALSE;
  char                        *realToCodes      = NULL;
  tconv_convert_ICU_context_t *contextp         = NULL;
  char                        *ignorep          = NULL;
  char                        *endIgnorep       = NULL;
  char                        *translitp        = NULL;
  char                        *endTranslitp     = NULL;
  UConverter                  *uConverterFromp  = NULL;
  UChar                       *uCharBufp        = NULL;
  UTransliterator             *uTransliteratorp = NULL;
  UConverter                  *uConverterTop    = NULL;
  UConverterFromUCallback      fromUCallbackp   = NULL;
  const void                  *fromuContextp    = NULL;
  UConverterToUCallback        toUCallbackp     = NULL;
  const void                  *toUContextp      = NULL;
  UBool                        fallbackb        = FALSE;
  int32_t                      uSetPatternFroml = 0;
  UChar                       *uSetPatternFroms = NULL;
  USet                        *uSetFromp        = NULL;
  int32_t                      uSetPatternTol   = 0;
  UChar                       *uSetPatternTos   = NULL;
  USet                        *uSetTop          = NULL;
  int32_t                      uSetPatternl     = 0;
  UChar                       *uSetPatterns     = NULL;
  USet                        *uSetp            = NULL;
  UErrorCode                   uErrorCode;
  char                        *p, *q;
  UConverterUnicodeSet         whichSet;
#define universalTransliteratorsLength 22
  U_STRING_DECL(universalTransliterators, "Any-Latin; Latin-ASCII", universalTransliteratorsLength);

  U_STRING_INIT(universalTransliterators, "Any-Latin; Latin-ASCII", universalTransliteratorsLength);

  if ((tocodes == NULL) || (fromcodes == NULL)) {
    errno = EINVAL;
    goto err;
  }

  /* ----------------------------------------------------------- */
  /* Duplicate tocodes and manage //IGNORE and //TRANSLIT option */
  /* ----------------------------------------------------------- */
  TCONV_TRACE(tconvp, "%s - strdup(\"%s\")", funcs, tocodes);
  realToCodes = strdup(tocodes);
  if (realToCodes == NULL) {
    goto err;
  }
  ignorep   = strstr(realToCodes, TCONV_ICU_IGNORE);
  translitp = strstr(realToCodes, TCONV_ICU_TRANSLIT);
  /* They must end the string or be followed by another (maybe) option */
  if (ignorep != NULL) {
    endIgnorep = ignorep + strlen(TCONV_ICU_IGNORE);
    if ((*endIgnorep == '\0') || (*(endIgnorep + 1) == '/')) {
      ignoreb = TRUE;
    }
  }
  if (translitp != NULL) {
    endTranslitp = translitp + strlen(TCONV_ICU_TRANSLIT);
    if ((*endTranslitp == '\0') || (*(endTranslitp + 1) == '/')) {
      translitb = TRUE;
    }
  }
  /* ... Remove options from realToCodes  */
  for (p = q = realToCodes; *p != '\0'; ++p) {    /* Note that a valid charset cannot contain \0 */
    if ((ignoreb == TRUE) && ((p >= ignorep) && (p < endIgnorep))) {
      continue;
    }
    if ((translitb == TRUE) && ((p >= translitp) && (p < endTranslitp))) {
      continue;
    }
    if (p != q) {
      *q++ = *p;
    } else {
      q++;
    }
  }
  *q = '\0';
  TCONV_TRACE(tconvp, "%s - realToCodes is now \"%s\"", funcs, realToCodes);

  /* ----------------------------------------------------------- */
  /* Get options                                                 */
  /* ----------------------------------------------------------- */
  if (optionp == NULL) {
    optionp = &tconv_convert_icu_option_default;
    /* This can be overwriten with environment variables */
    TCONV_TRACE(tconvp, "%s - getenv(\"%s\")", funcs, TCONV_ENV_CONVERT_ICU_UCHARLENGTH);
    p = getenv(TCONV_ENV_CONVERT_ICU_UCHARLENGTH);
    if (p != NULL) {
      TCONV_TRACE(tconvp, "%s - atoi(\"%s\")", funcs, p);
      optionp->uCharLengthl = atoi(p);
      if (optionp->uCharLengthl <= 0) {
	errno = EINVAL;
	goto err;
      }
    }
    TCONV_TRACE(tconvp, "%s - getenv(\"%s\")", funcs, TCONV_ENV_CONVERT_ICU_FALLBACK);
    p = getenv(TCONV_ENV_CONVERT_ICU_FALLBACK);
    if (p != NULL) {
      TCONV_TRACE(tconvp, "%s - atoi(\"%s\")", funcs, p);
      optionp->fallbackb = (atoi(p) != 0) ? 1 : 0;
    }
  }
  fallbackb = (optionp->fallbackb != 0) ? TRUE : FALSE;

  /* ----------------------------------------------------------- */
  /* Setup the from converter                                    */
  /* ----------------------------------------------------------- */
  fromUCallbackp = (ignoreb == TRUE) ? UCNV_FROM_U_CALLBACK_SKIP : UCNV_FROM_U_CALLBACK_STOP;
  fromuContextp  = NULL;

  uErrorCode = U_ZERO_ERROR;
  TCONV_TRACE(tconvp, "%s - ucnv_open(\"%s\", %p)", funcs, fromcodes, &uErrorCode);
  uConverterFromp = ucnv_open(fromcodes, &uErrorCode);
  if (U_FAILURE(uErrorCode)) {
    errno = ENOSYS;
    goto err;
  }
  TCONV_TRACE(tconvp, "%s - ucnv_open returned %p", funcs, uConverterFromp);

  uErrorCode = U_ZERO_ERROR;
  TCONV_TRACE(tconvp, "%s - ucnv_setFromUCallBack(%p, %p, %p, %p, %p)", funcs, uConverterFromp, fromUCallbackp, fromuContextp, NULL, NULL, &uErrorCode);
  ucnv_setFromUCallBack(uConverterFromp, fromUCallbackp, fromuContextp, NULL, NULL, &uErrorCode);
  if (U_FAILURE(uErrorCode)) {
    errno = ENOSYS;
    goto err;
  }

  TCONV_TRACE(tconvp, "%s - ucnv_setFallback(%p, %d)", funcs, uConverterFromp, (int) fallbackb);
  ucnv_setFallback(uConverterFromp, fallbackb);

  /* ----------------------------------------------------------- */
  /* Setup the proxy unicode buffer                              */
  /* ----------------------------------------------------------- */
  TCONV_TRACE(tconvp, "%s - malloc(%lld)", funcs, (unsigned long long) (optionp->uCharLengthl * sizeof(UChar)));
  uCharBufp = (UChar *) malloc(optionp->uCharLengthl * sizeof(UChar));
  if (uCharBufp == NULL) {
    goto err;
  }
  
  /* ----------------------------------------------------------- */
  /* Setup the to converter                                      */
  /* ----------------------------------------------------------- */
  toUCallbackp   = (ignoreb == TRUE) ? UCNV_TO_U_CALLBACK_SKIP   : UCNV_TO_U_CALLBACK_STOP;
  toUContextp    = NULL;

  uErrorCode = U_ZERO_ERROR;
  TCONV_TRACE(tconvp, "%s - ucnv_open(\"%s\", %p)", funcs, realToCodes, &uErrorCode);
  uConverterTop = ucnv_open(realToCodes, &uErrorCode);
  if (U_FAILURE(uErrorCode)) {
    errno = ENOSYS;
    goto err;
  }
  TCONV_TRACE(tconvp, "%s - ucnv_open returned %p", funcs, uConverterTop);

  uErrorCode = U_ZERO_ERROR;
  TCONV_TRACE(tconvp, "%s - ucnv_setToUCallBack(%p, %p, %p, %p, %p)", funcs, uConverterTop, toUCallbackp, toUContextp, NULL, NULL, &uErrorCode);
  ucnv_setToUCallBack(uConverterTop, toUCallbackp, toUContextp, NULL, NULL, &uErrorCode);
  if (U_FAILURE(uErrorCode)) {
    errno = ENOSYS;
    goto err;
  }

  TCONV_TRACE(tconvp, "%s - ucnv_setFallback(%p, %d)", funcs, uConverterTop, (int) fallbackb);
  ucnv_setFallback(uConverterTop, fallbackb);

  /* ----------------------------------------------------------- */
  /* Setup the transliterator                                    */
  /* ----------------------------------------------------------- */
  if (translitb == TRUE) {
#if UCONFIG_NO_TRANSLITERATION
    TCONV_TRACE(tconvp, "%s - UCONFIG_NO_TRANSLITERATION", funcs);
    errno = ENOSYS;
    goto err;
#endif
    whichSet = (fallbackb == TRUE) ? UCNV_ROUNDTRIP_AND_FALLBACK_SET : UCNV_ROUNDTRIP_SET;

    /* Transliterator is generated on-the-fly using the unicode */
    /* sets from the two converters.                            */
    
    /* --------------------------- */
    /* Uset for the from converter */
    /* --------------------------- */
    TCONV_TRACE(tconvp, "%s - getting \"from\" USet", funcs);
    TCONV_TRACE(tconvp, "%s - uset_openEmpty()", funcs);
    uSetFromp = uset_openEmpty();
    if (uSetFromp == NULL) { /* errno ? */
      errno = ENOSYS;
      goto err;
    }
    TCONV_TRACE(tconvp, "%s - uset_openEmpty returned %p", funcs, uSetFromp);

    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - ucnv_getUnicodeSet(%p, %p, %s, %p)", funcs, uConverterFromp, uSetFromp, (fallbackb == TRUE) ? "UCNV_ROUNDTRIP_AND_FALLBACK_SET" : "UCNV_ROUNDTRIP_SET", &uErrorCode);
    ucnv_getUnicodeSet(uConverterFromp, uSetFromp, whichSet, &uErrorCode);
    if (U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }

#ifndef TCONV_NTRACE
    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - uset_toPattern(%p, NULL, 0, TRUE, %p)", funcs, uSetFromp, &uErrorCode);
    uSetPatternFroml = uset_toPattern(uSetFromp, NULL, 0, TRUE, &uErrorCode);
    if (uErrorCode != U_BUFFER_OVERFLOW_ERROR) {
      errno = ENOSYS;
      goto err;
    }
    uSetPatternFroms = malloc((uSetPatternFroml + 1) * sizeof(UChar));
    if (uSetPatternFroms == NULL) {
      goto err;
    }
    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - uset_toPattern(%p, %p, %lld, TRUE, %p)", funcs, uSetFromp, uSetPatternFroms, (unsigned long long) (uSetPatternFroml + 1), &uErrorCode);
    uset_toPattern(uSetFromp, uSetPatternFroms, uSetPatternFroml, TRUE, &uErrorCode);
    if (U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }
    /* Make sure uSetPatternFroms is NULL terminated (a-la UTF-16) */
    p = (char *) (uSetPatternFroms + uSetPatternFroml);
    *p++ = '\0';
    *p   = '\0';
#endif

    /* ------------------------- */
    /* Uset for the to converter */
    /* ------------------------- */
    TCONV_TRACE(tconvp, "%s - getting \"to\" USet", funcs);
    TCONV_TRACE(tconvp, "%s - uset_openEmpty()", funcs);
    uSetTop = uset_openEmpty();
    if (uSetTop == NULL) { /* errno ? */
      errno = ENOSYS;
      goto err;
    }
    TCONV_TRACE(tconvp, "%s - uset_openEmpty returned %p", funcs, uSetTop);

    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - ucnv_getUnicodeSet(%p, %p, %s, %p)", funcs, uConverterTop, uSetTop, (fallbackb == TRUE) ? "UCNV_ROUNDTRIP_AND_FALLBACK_SET" : "UCNV_ROUNDTRIP_SET", &uErrorCode);
    ucnv_getUnicodeSet(uConverterTop, uSetTop, whichSet, &uErrorCode);
    if (U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }

#ifndef TCONV_NTRACE
    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - uset_toPattern(%p, NULL, 0, TRUE, %p)", funcs, uSetTop, &uErrorCode);
    uSetPatternTol = uset_toPattern(uSetTop, NULL, 0, TRUE, &uErrorCode);
    if (uErrorCode != U_BUFFER_OVERFLOW_ERROR) {
      errno = ENOSYS;
      goto err;
    }
    uSetPatternTos = malloc((uSetPatternTol + 1) * sizeof(UChar));
    if (uSetPatternTos == NULL) {
      goto err;
    }
    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - uset_toPattern(%p, %p, %lld, TRUE, %p)", funcs, uSetTop, uSetPatternTos, (unsigned long long) (uSetPatternTol + 1), &uErrorCode);
    uset_toPattern(uSetTop, uSetPatternTos, uSetPatternTol, TRUE, &uErrorCode);
    if (U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }
    /* Make sure uSetPatternTos is NULL terminated (a-la UTF-16) */
    p = (char *) (uSetPatternTos + uSetPatternTol);
    *p++ = '\0';
    *p   = '\0';
#endif

    /* ---------------------------------------------------------------------------- */
    /* Interset the two usets (I could have used uset_retainAll(uSetFromp, uSetTop) */
    /* ---------------------------------------------------------------------------- */
    TCONV_TRACE(tconvp, "%s - intersecting \"from\" and \"to\" USet's", funcs);
    TCONV_TRACE(tconvp, "%s - uset_openEmpty()", funcs);
    uSetp = uset_openEmpty();
    if (uSetp == NULL) { /* errno ? */
      errno = ENOSYS;
      goto err;
    }
    TCONV_TRACE(tconvp, "%s - uset_openEmpty returned %p", funcs, uSetp);

    uset_addAll(uSetp, uSetFromp);
    uset_retainAll(uSetp, uSetTop);

    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - uset_toPattern(%p, NULL, 0, TRUE, %p)", funcs, uSetp, &uErrorCode);
    uSetPatternl = uset_toPattern(uSetp, NULL, 0, TRUE, &uErrorCode);
    if (uErrorCode != U_BUFFER_OVERFLOW_ERROR) {
      errno = ENOSYS;
      goto err;
    }
    uSetPatterns = malloc((uSetPatternl + 1) * sizeof(UChar));
    if (uSetPatterns == NULL) {
      goto err;
    }
    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - uset_toPattern(%p, %p, %lld, TRUE, %p)", funcs, uSetp, uSetPatterns, (unsigned long long) (uSetPatternl + 1), &uErrorCode);
    uset_toPattern(uSetp, uSetPatterns, uSetPatternl, TRUE, &uErrorCode);
    if (U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }
    /* Make sure uSetPatterns is NULL terminated (a-la UTF-16) */
    p = (char *) (uSetPatterns + uSetPatternl);
    *p++ = '\0';
    *p   = '\0';

    /* ---------------------------------------------------------------------------- */
    /* Create transliterator                                                        */
    /* ---------------------------------------------------------------------------- */
    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - uTransliteratorp(%p, %d, UTRANS_FORWARD, NULL, 0, NULL, %p)", funcs, universalTransliterators, universalTransliteratorsLength, &uErrorCode);
    uTransliteratorp = utrans_openU(universalTransliterators,
                                    universalTransliteratorsLength,
                                    UTRANS_FORWARD,
                                    NULL,
                                    0,
                                    NULL,
                                    &uErrorCode);
    if (U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }

    /* Add a filter to this transliterator using the intersected USet's */
    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - utrans_setFilter(%p, %p, %lld, %p)", funcs, uSetPatterns, (unsigned long long) uSetPatternl, &uErrorCode);
    utrans_setFilter(uTransliteratorp,
                     uSetPatterns,
                     uSetPatternl,
                     &uErrorCode);
    if (U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }

      /* Cleanup */
    uset_close(uSetFromp);
    uset_close(uSetTop);
    uset_close(uSetp);
    uSetFromp = NULL;
    uSetTop   = NULL;
    uSetp     = NULL;

#ifndef TCONV_NTRACE
    free(uSetPatternFroms);
    free(uSetPatternTos);
    uSetPatternFroms = NULL;
    uSetPatternTos   = NULL;
#endif
    free(uSetPatterns);
    uSetPatterns     = NULL;
  }

  /* ----------------------------------------------------------- */
  /* Setup the context                                           /
  /* ----------------------------------------------------------- */
  TCONV_TRACE(tconvp, "%s - malloc(%lld)", funcs, (unsigned long long) sizeof(tconv_convert_ICU_context_t));
  contextp = (tconv_convert_ICU_context_t *) malloc(sizeof(tconv_convert_ICU_context_t));
  if (contextp == NULL) {
    goto err;
  }

  contextp->uConverterFromp  = uConverterFromp;
  contextp->uCharBufp        = uCharBufp;
  contextp->uCharLengthl     = optionp->uCharLengthl;
  contextp->uConverterTop    = uConverterTop;
  contextp->uTransliteratorp = uTransliteratorp;

  TCONV_TRACE(tconvp, "%s - return %p", funcs, contextp);
  return contextp;

 err:
  {
    int errnol = errno;
    if (realToCodes != NULL) {
      free(realToCodes);
    }
    if (U_FAILURE(uErrorCode)) {
      tconv_error_set(tconvp, u_errorName(uErrorCode));
    }
    if (uConverterFromp != NULL) {
      ucnv_close (uConverterFromp);
    }
    if (uCharBufp != NULL) {
      free(uCharBufp);
    }
    if (uConverterTop != NULL) {
      ucnv_close (uConverterTop);
    }
    if (uSetPatternFroms == NULL) {
      free(uSetPatternFroms);
    }
    if (uSetFromp != NULL) {
      uset_close(uSetFromp);
    }
    if (uSetPatternTos == NULL) {
      free(uSetPatternTos);
    }
    if (uSetTop != NULL) {
      uset_close(uSetTop);
    }
    if (uSetp != NULL) {
      uset_close(uSetp);
    }
    if (uTransliteratorp != NULL) {
      utrans_close(uTransliteratorp);
    }
    if (contextp != NULL) {
      free(contextp);
    }
    errno = errnol;
  }
  TCONV_TRACE(tconvp, "%s - return -1", funcs);
  return (void *)-1;
}

/*****************************************************************************/
size_t tconv_convert_ICU_run(tconv_t tconvp, void *voidp, char **inbufsp, size_t *inbytesleftlp, char **outbufsp, size_t *outbytesleftlp)
/*****************************************************************************/
{
  static const char            funcs[]      = "tconv_convert_ICU_run";
  tconv_convert_ICU_context_t *contextp     = (tconv_convert_ICU_context_t *) voidp;
  UBool                        flushb       = ((inbufsp == NULL) || (*inbufsp == NULL)) ? TRUE : FALSE;
  UChar                       *targetp      = contextp->uCharBufp;
  UChar                       *targetLimitp = contextp->uCharBufp + contextp->uCharLengthl; /* in units of UChar */
  static const char           *dummys       = "";
  char                        *sourcep      = (inbufsp != NULL) ? *inbufsp : dummys;
  char                        *sourceLimitp = ((inbufsp != NULL) && (inbytesleftlp != NULL)) ? (*inbufsp + *inbytesleftlp) : dummys;
  UErrorCode                   uErrorCode;
  size_t                       rcl;
  /* Logic here directly inherited from uconv.cpp */
  size_t                       rd;
  size_t                       wr;
  uint32_t                     infoffset;
  uint32_t                     outfoffset;
  UBool                        willexit;
  UBool                        fromSawEndOfBytes;
  UBool                        toSawEndOfUnicode;

  infoffset = 0;
  outfoffset = 0;
  rd = 0;
  do {

    willexit = FALSE;
    infoffset += rd;

    uErrorCode = U_ZERO_ERROR;
    TCONV_TRACE(tconvp, "%s - ucnv_toUnicode(%p, %p, %p, %p, %p, NULL, %d, %p)", funcs, contextp->uConverterFromp, &targetp, targetLimitp, &sourcep, sourceLimitp, (int) flushb, &uErrorCode);
    ucnv_toUnicode(contextp->uConverterFromp,
                   &targetp,
                   targetLimitp,
                   &sourcep,
                   sourceLimitp,
                   NULL,
                   flushb,
                   &uErrorCode);
    if ((uErrorCode != U_BUFFER_OVERFLOW_ERROR) && U_FAILURE(uErrorCode)) {
      errno = ENOSYS;
      goto err;
    }

  } while (fluhsb == FALSE)

  return 0;

 err:
  if (U_FAILURE(uErrorCode)) {
    tconv_error_set(tconvp, u_errorName(uErrorCode));
  }
  return (size_t)-1;
}

/*****************************************************************************/
int tconv_convert_ICU_free(tconv_t tconvp, void *voidp)
/*****************************************************************************/
{
  static const char funcs[] = "tconv_convert_ICU_free";

  return 0;
}
