#define PERL_NO_GET_CONTEXT 1     /* we want efficiency */
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include <marpaESLIF.h>
#include <genericLogger.h>

/* Creating marpaESLIF includes genericLogger */
typedef struct MarpaX_ESLIF {
  SV              *Perl_loggerInterfacep;
  genericLogger_t *genericLoggerp;
  marpaESLIF_t    *marpaESLIFp;
} MarpaX_ESLIF_t;
/* Nothing special for the other types */
typedef marpaESLIFGrammar_t MarpaX_ESLIF_Grammar_t;

/* For typemap */
typedef MarpaX_ESLIF_t         *MarpaX_ESLIF;
typedef MarpaX_ESLIF_Grammar_t *MarpaX_ESLIF_Grammar;

/* ----------------------- */
/* generic Logger Callback */
/* ----------------------- */
static void marpaESLIF_genericLoggerCallback(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs) {
  SV   *Perl_loggerInterfacep = (SV *) userDatavp;
  char *method;

  switch (logLeveli) {
  case GENERICLOGGER_LOGLEVEL_TRACE:     method = "trace";     break;
  case GENERICLOGGER_LOGLEVEL_DEBUG:     method = "debug";     break;
  case GENERICLOGGER_LOGLEVEL_INFO:      method = "info";      break;
  case GENERICLOGGER_LOGLEVEL_NOTICE:    method = "notice";    break;
  case GENERICLOGGER_LOGLEVEL_WARNING:   method = "warning";   break;
  case GENERICLOGGER_LOGLEVEL_ERROR:     method = "error";     break;
  case GENERICLOGGER_LOGLEVEL_CRITICAL:  method = "critical";  break;
  case GENERICLOGGER_LOGLEVEL_ALERT:     method = "alert";     break;
  case GENERICLOGGER_LOGLEVEL_EMERGENCY: method = "emergency"; break;
  default:                               method = NULL;        break;
  }

  if (method != NULL) {
    dTHX;
    dSP;

    PUSHMARK(SP);
    EXTEND(SP, 2);
    PUSHs(Perl_loggerInterfacep);
    PUSHs(sv_2mortal(newSVpv(msgs,0)));
    PUTBACK;

    call_method(method, G_DISCARD);
  }
}

=for comment
  /* ======================================================================= */
  /* MarpaX::ESLIF                                                           */
  /* ======================================================================= */
=cut

MODULE = MarpaX::ESLIF            PACKAGE = MarpaX::ESLIF

PROTOTYPES: ENABLE

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::new                                                      */
  /* ----------------------------------------------------------------------- */
=cut

MarpaX_ESLIF
new(Perl_packagep, ...)
  SV *Perl_packagep;
PREINIT:
  SV *Perl_loggerInterfacep = &PL_sv_undef;
CODE:
  MarpaX_ESLIF       MarpaX_ESLIFp;
  marpaESLIFOption_t marpaESLIFOption;

  if(items > 1) {
    Perl_loggerInterfacep = ST(1);
  }

  Newx(MarpaX_ESLIFp, 1, MarpaX_ESLIF_t);
  MarpaX_ESLIFp->Perl_loggerInterfacep = &PL_sv_undef;
  MarpaX_ESLIFp->genericLoggerp        = NULL;
  MarpaX_ESLIFp->marpaESLIFp           = NULL;

  /* ------------- */
  /* genericLogger */
  /* ------------- */
  if (Perl_loggerInterfacep != &PL_sv_undef) {
    MarpaX_ESLIFp->Perl_loggerInterfacep = SvREFCNT_inc(Perl_loggerInterfacep);
    MarpaX_ESLIFp->genericLoggerp        = genericLogger_newp(marpaESLIF_genericLoggerCallback,
                                                                        MarpaX_ESLIFp->Perl_loggerInterfacep,
                                                                        GENERICLOGGER_LOGLEVEL_TRACE);
    if (MarpaX_ESLIFp->genericLoggerp == NULL) {
      croak(strerror(errno));
    }
  }

  /* ---------- */
  /* marpaESLIF */
  /* ---------- */
  marpaESLIFOption.genericLoggerp = MarpaX_ESLIFp->genericLoggerp;
  MarpaX_ESLIFp->marpaESLIFp = marpaESLIF_newp(&marpaESLIFOption);
  if (MarpaX_ESLIFp->marpaESLIFp == NULL) {
      croak(strerror(errno));
  }

  RETVAL = MarpaX_ESLIFp;
OUTPUT:
  RETVAL

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::DESTROY                                                  */
  /* ----------------------------------------------------------------------- */
=cut

void
DESTROY(MarpaX_ESLIFp)
  MarpaX_ESLIF MarpaX_ESLIFp;
CODE:
  if (MarpaX_ESLIFp->Perl_loggerInterfacep != &PL_sv_undef) {
    SvREFCNT_dec(MarpaX_ESLIFp->Perl_loggerInterfacep);
  }
  marpaESLIF_freev(MarpaX_ESLIFp->marpaESLIFp);
  genericLogger_freev(&(MarpaX_ESLIFp->genericLoggerp));
  Safefree(MarpaX_ESLIFp);

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::version                                                  */
  /* ----------------------------------------------------------------------- */
=cut

const char *
version(MarpaX_ESLIFp)
  MarpaX_ESLIF MarpaX_ESLIFp;
CODE:
  /* In reality MarpaX_ESLIFp is not needed -; */
  RETVAL = marpaESLIF_versions();
OUTPUT:
  RETVAL

=for comment
  /* ======================================================================= */
  /* MarpaX::ESLIF::Grammar                                                  */
  /* ======================================================================= */
=cut

MODULE = MarpaX::ESLIF            PACKAGE = MarpaX::ESLIF::Grammar

PROTOTYPES: ENABLE

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::Grammar::new                                             */
  /* ----------------------------------------------------------------------- */
=cut

MarpaX_ESLIF_Grammar
new(Perl_packagep, MarpaX_ESLIFp, Perl_grammarp)
  SV           *Perl_packagep;
  MarpaX_ESLIF  MarpaX_ESLIFp;
  SV           *Perl_grammarp;
CODE:
  MarpaX_ESLIF_Grammar       MarpaX_ESLIF_Grammarp;
  marpaESLIFGrammarOption_t  marpaESLIFGrammarOption;
  marpaESLIFGrammar_t       *marpaESLIFGrammarp;
  char                      *utf8p;
  STRLEN                     utf8l;

  /* The grammar has to be a valid string, whatever its encoding */
  utf8p = SvPVutf8(Perl_grammarp, utf8l);

  marpaESLIFGrammarOption.bytep               = (void *) utf8p;
  marpaESLIFGrammarOption.bytel               = (size_t) utf8l;
  marpaESLIFGrammarOption.encodings           = "UTF-8";
  marpaESLIFGrammarOption.encodingl           = strlen("UTF-8");
  marpaESLIFGrammarOption.encodingOfEncodings = "ASCII";

  marpaESLIFGrammarp = marpaESLIFGrammar_newp(MarpaX_ESLIFp->marpaESLIFp, &marpaESLIFGrammarOption);
  if (marpaESLIFGrammarp == NULL) {
    croak(strerror(errno));
  }
  RETVAL = marpaESLIFGrammarp;
OUTPUT:
  RETVAL

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::Grammar::DESTROY                                         */
  /* ----------------------------------------------------------------------- */
=cut

void
DESTROY(MarpaX_ESLIF_Grammarp)
  MarpaX_ESLIF_Grammar MarpaX_ESLIF_Grammarp;
CODE:
  marpaESLIFGrammar_freev(MarpaX_ESLIF_Grammarp);

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::Grammar::ngrammar                                        */
  /* ----------------------------------------------------------------------- */
=cut

IV
ngrammar(MarpaX_ESLIF_Grammarp)
  MarpaX_ESLIF_Grammar MarpaX_ESLIF_Grammarp;
CODE:
  int ngrammari;

  if (! marpaESLIFGrammar_ngrammarib(MarpaX_ESLIF_Grammarp, &ngrammari)) {
    croak("marpaESLIFGrammar_ngrammarib failure");
  }

  RETVAL = (IV) ngrammari;
OUTPUT:
  RETVAL

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::Grammar::currentLevel                                    */
  /* ----------------------------------------------------------------------- */
=cut

IV
currentLevel(MarpaX_ESLIF_Grammarp)
  MarpaX_ESLIF_Grammar MarpaX_ESLIF_Grammarp;
CODE:
  int leveli;

  if (! marpaESLIFGrammar_grammar_currentb(MarpaX_ESLIF_Grammarp, &leveli, NULL)) {
    croak("marpaESLIFGrammar_grammar_currentb failure");
  }
  RETVAL = (IV) leveli;
OUTPUT:
  RETVAL

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::Grammar::currentDescription                              */
  /* ----------------------------------------------------------------------- */
=cut

SV *
currentDescription(MarpaX_ESLIF_Grammarp)
  MarpaX_ESLIF_Grammar MarpaX_ESLIF_Grammarp;
CODE:
  marpaESLIFString_t  *descp;
  SV                  *sv;

  if (! marpaESLIFGrammar_grammar_currentb(MarpaX_ESLIF_Grammarp, NULL, &descp)) {
    croak("marpaESLIFGrammar_grammar_currentb failure");
  }
  /* We enforced UTF-8 when parsing the grammar, so description is also in UTF-8 */
  RETVAL = newSVpvn_utf8(descp->bytep, descp->bytel, 1);
OUTPUT:
  RETVAL

=for comment
  /* ----------------------------------------------------------------------- */
  /* MarpaX::ESLIF::Grammar::descriptionByLevel                              */
  /* ----------------------------------------------------------------------- */
=cut

SV *
descriptionByLevel(MarpaX_ESLIF_Grammarp, Perl_leveli)
  MarpaX_ESLIF_Grammar MarpaX_ESLIF_Grammarp;
  IV  Perl_leveli;
CODE:
  marpaESLIFString_t  *descp;
  SV                  *sv;

  if (! marpaESLIFGrammar_grammar_by_levelb(MarpaX_ESLIF_Grammarp, (int) Perl_leveli, NULL, NULL, &descp)) {
    croak("marpaESLIFGrammar_grammar_by_levelb failure");
  }
  /* We enforced UTF-8 when parsing the grammar, so description is also in UTF-8 */
  RETVAL = newSVpvn_utf8(descp->bytep, descp->bytel, 1);
OUTPUT:
  RETVAL
