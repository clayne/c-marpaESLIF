#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <genericStack.h>
#include "config.h" /* For inline */
#include "marpaESLIF/internal/bootstrap_actions.h"
#include "marpaESLIF/internal/bootstrap_types.h"

/* This file contain the definition of all bootstrap actions, i.e. the ESLIF grammar itself */
/* This is an example of how to use the API */

static inline void  _marpaESLIF_bootstrap_rhs_primary_freev(marpaESLIF_bootstrap_rhs_primary_t *rhsPrimaryp);
static inline void  _marpaESLIF_bootstrap_rhs_primary_symbol_freev(marpaESLIF_bootstrap_rhs_primary_symbol_t *symbolp);
static inline void  _marpaESLIF_bootstrap_utf_string_freev(marpaESLIF_bootstrap_utf_string_t *stringp);
static inline void  _marpaESLIF_bootstrap_rhs_freev(genericStack_t *rhsPrimaryStackp);
static inline void  _marpaESLIF_bootstrap_adverb_list_item_freev(marpaESLIF_bootstrap_adverb_list_item_t *adverbListItemp);
static inline void  _marpaESLIF_bootstrap_adverb_list_items_freev(genericStack_t *adverbListItemStackp);

static        void  _marpaESLIF_bootstrap_freeDefaultActionv(void *userDatavp, int contexti, void *p, size_t sizel);

static        short _marpaESLIF_bootstrap_G1_action_symbol_2b(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static        short _marpaESLIF_bootstrap_G1_action_op_declare_1b(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static        short _marpaESLIF_bootstrap_G1_action_rhsb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static        short _marpaESLIF_bootstrap_G1_action_adverb_list_itemsb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static        short _marpaESLIF_bootstrap_G1_action_actionb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static        short _marpaESLIF_bootstrap_G1_action_rhs_primary_2b(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);

/*****************************************************************************/
static inline void  _marpaESLIF_bootstrap_rhs_primary_freev(marpaESLIF_bootstrap_rhs_primary_t *rhsPrimaryp)
/*****************************************************************************/
{
  if (rhsPrimaryp != NULL) {
    switch (rhsPrimaryp->type) {
    case MARPAESLIF_BOOTSTRAP_RHS_PRIMARY_TYPE_SYMBOL:
      _marpaESLIF_bootstrap_rhs_primary_symbol_freev(rhsPrimaryp->u.symbolp);
      break;
    case MARPAESLIF_BOOTSTRAP_RHS_PRIMARY_TYPE_QUOTED_STRING:
      _marpaESLIF_bootstrap_utf_string_freev(rhsPrimaryp->u.quotedStringp);
      break;
    default:
      break;
    }
    free(rhsPrimaryp);
  }
}

/*****************************************************************************/
static inline void  _marpaESLIF_bootstrap_rhs_primary_symbol_freev(marpaESLIF_bootstrap_rhs_primary_symbol_t *symbolp)
/*****************************************************************************/
{
  if (symbolp != NULL) {
    if (symbolp->symbols != NULL) {
      free(symbolp->symbols);
    }
    _marpaESLIF_bootstrap_utf_string_freev(symbolp->lookupGrammarStringp);
    free(symbolp);
  }
}

/*****************************************************************************/
static inline void  _marpaESLIF_bootstrap_utf_string_freev(marpaESLIF_bootstrap_utf_string_t *stringp)
/*****************************************************************************/
{
  if (stringp != NULL) {
    if (stringp->bytep != NULL) {
      free(stringp->bytep);
    }
    free(stringp);
  }
}

/*****************************************************************************/
static inline void _marpaESLIF_bootstrap_rhs_freev(genericStack_t *rhsPrimaryStackp)
/*****************************************************************************/
{
  int   i;

  if (rhsPrimaryStackp != NULL) {
    for (i = 0; i < GENERICSTACK_USED(rhsPrimaryStackp); i++) {
      if (GENERICSTACK_IS_PTR(rhsPrimaryStackp, i)) {
        _marpaESLIF_bootstrap_rhs_primary_freev((marpaESLIF_bootstrap_rhs_primary_t *) GENERICSTACK_GET_PTR(rhsPrimaryStackp, i));
      }
    }
    GENERICSTACK_FREE(rhsPrimaryStackp);
  }
}

/*****************************************************************************/
static inline void  _marpaESLIF_bootstrap_adverb_list_items_freev(genericStack_t *adverbListItemStackp)
/*****************************************************************************/
{
  int i;

  if (adverbListItemStackp != NULL) {
    for (i = 0; i < GENERICSTACK_USED(adverbListItemStackp); i++) {
      if (GENERICSTACK_IS_PTR(adverbListItemStackp, i)) {
        _marpaESLIF_bootstrap_adverb_list_item_freev((marpaESLIF_bootstrap_adverb_list_item_t *) GENERICSTACK_GET_PTR(adverbListItemStackp, i));
      }
    }
    GENERICSTACK_FREE(adverbListItemStackp);
  }
}

/*****************************************************************************/
static inline void _marpaESLIF_bootstrap_adverb_list_item_freev(marpaESLIF_bootstrap_adverb_list_item_t *adverbListItemp)
/*****************************************************************************/
{
  if (adverbListItemp != NULL) {
    switch (adverbListItemp->type) {
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_ACTION:
      if (adverbListItemp->u.actions != NULL) {
        free(adverbListItemp->u.actions);
      }
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_AUTORANK:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_LEFT:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_RIGHT:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_GROUP:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_SEPARATOR:
      if (adverbListItemp->u.separators != NULL) {
        free(adverbListItemp->u.separators);
      }
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_PROPER:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_RANK:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_NULL_RANKING:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_PRIORITY:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_PAUSE:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_LATM:
      break;
    case MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_NAMING:
      if (adverbListItemp->u.naming.bytep != NULL) {
        free(adverbListItemp->u.naming.bytep);
      }
      break;
    default:
      break;
    }
    free(adverbListItemp);
  }
}

/*****************************************************************************/
static void _marpaESLIF_bootstrap_freeDefaultActionv(void *userDatavp, int contexti, void *p, size_t sizel)
/*****************************************************************************/
{
  switch (contexti) {
  case MARPAESLIF_BOOTSTRAP_STACK_TYPE_OP_DECLARE:
    break;
  case MARPAESLIF_BOOTSTRAP_STACK_TYPE_SYMBOL_NAME:
    free(p);
    break;
  case MARPAESLIF_BOOTSTRAP_STACK_TYPE_RHS_PRIMARY:
    _marpaESLIF_bootstrap_rhs_primary_freev((marpaESLIF_bootstrap_rhs_primary_t *) p);
    break;
  case MARPAESLIF_BOOTSTRAP_STACK_TYPE_RHS:
    _marpaESLIF_bootstrap_rhs_freev((genericStack_t *) p);
    break;
  case MARPAESLIF_BOOTSTRAP_STACK_TYPE_ACTION:
    free(p);
    break;
  case MARPAESLIF_BOOTSTRAP_STACK_TYPE_ADVERB_LIST_ITEMS:
    _marpaESLIF_bootstrap_adverb_list_items_freev((genericStack_t *) p);
    break;
  default:
    break;
  }
}

/*****************************************************************************/
static marpaESLIFValueRuleCallback_t _marpaESLIF_bootstrap_ruleActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  marpaESLIF_t                  *marpaESLIFp        = marpaESLIFValue_eslifp(marpaESLIFValuep);
  marpaESLIFGrammar_t           *marpaESLIFGrammarp = marpaESLIFValue_grammarp(marpaESLIFValuep);
  marpaESLIFValueRuleCallback_t  marpaESLIFValueRuleCallbackp;
  int                            grammari;
  int                            leveli;

  if (! marpaESLIFValue_grammarib(marpaESLIFValuep, &grammari)) {
    MARPAESLIF_ERROR(marpaESLIFp, "marpaESLIFValue_grammarib failure");
    goto err;
  }
  marpaESLIFGrammarp = marpaESLIFValue_grammarp(marpaESLIFValuep);
  if (marpaESLIFGrammarp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFp, "marpaESLIFValue_grammarp failure");
    goto err;
  }
  if (! marpaESLIFGrammar_leveli_by_grammarb(marpaESLIFGrammarp, &leveli, grammari, NULL /* descp */)) {
    MARPAESLIF_ERROR(marpaESLIFp, "marpaESLIFGrammar_leveli_by_grammarb failure");
    goto err;
  }
  /* We have only one level here */
  if (leveli != 0) {
    MARPAESLIF_ERRORF(marpaESLIFp, "leveli is %d", leveli);
    goto err;
  }
  /* TO DO */
       if (strcmp(actions, "G1_action_op_declare_1")      == 0) { marpaESLIFValueRuleCallbackp = _marpaESLIF_bootstrap_G1_action_op_declare_1b;      }
  else if (strcmp(actions, "G1_action_rhs")               == 0) { marpaESLIFValueRuleCallbackp = _marpaESLIF_bootstrap_G1_action_rhsb;               }
  else if (strcmp(actions, "G1_action_adverb_list_items") == 0) { marpaESLIFValueRuleCallbackp = _marpaESLIF_bootstrap_G1_action_adverb_list_itemsb; }
  else if (strcmp(actions, "G1_action_action")            == 0) { marpaESLIFValueRuleCallbackp = _marpaESLIF_bootstrap_G1_action_actionb;            }
  else if (strcmp(actions, "G1_action_symbol_2")          == 0) { marpaESLIFValueRuleCallbackp = _marpaESLIF_bootstrap_G1_action_symbol_2b;          }
  else if (strcmp(actions, "G1_action_rhs_primary_2")     == 0) { marpaESLIFValueRuleCallbackp = _marpaESLIF_bootstrap_G1_action_rhs_primary_2b;     }
  else
  {
    MARPAESLIF_ERRORF(marpaESLIFp, "Unsupported action \"%s\"", actions);
    goto err;
  }

  goto done;

 err:
  marpaESLIFValueRuleCallbackp = NULL;
 done:
  return marpaESLIFValueRuleCallbackp;
}

/*****************************************************************************/
static marpaESLIFValueFreeCallback_t _marpaESLIF_bootstrap_freeActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  marpaESLIF_t                   *marpaESLIFp        = marpaESLIFValue_eslifp(marpaESLIFValuep);
  marpaESLIFGrammar_t            *marpaESLIFGrammarp = marpaESLIFValue_grammarp(marpaESLIFValuep);
  marpaESLIFValueFreeCallback_t   marpaESLIFValueFreeCallbackp;
  int                             grammari;
  int                             leveli;

  if (! marpaESLIFValue_grammarib(marpaESLIFValuep, &grammari)) {
    MARPAESLIF_ERROR(marpaESLIFp, "marpaESLIFValue_grammarib failure");
    goto err;
  }
  marpaESLIFGrammarp = marpaESLIFValue_grammarp(marpaESLIFValuep);
  if (marpaESLIFGrammarp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFp, "marpaESLIFValue_grammarp failure");
    goto err;
  }
  if (! marpaESLIFGrammar_leveli_by_grammarb(marpaESLIFGrammarp, &leveli, grammari, NULL /* descp */)) {
    MARPAESLIF_ERROR(marpaESLIFp, "marpaESLIFGrammar_leveli_by_grammarb failure");
    goto err;
  }
  /* We have only one level here */
  if (leveli != 0) {
    MARPAESLIF_ERRORF(marpaESLIFp, "leveli is %d", leveli);
    goto err;
  }

  if (strcmp(actions, "_marpaESLIF_bootstrap_freeDefaultActionv") == 0) {
    marpaESLIFValueFreeCallbackp = _marpaESLIF_bootstrap_freeDefaultActionv;
  } else {
    MARPAESLIF_ERRORF(marpaESLIFp, "Unsupported action \"%s\"", actions);
    goto err;
  }

  goto done;

 err:
  marpaESLIFValueFreeCallbackp = NULL;
 done:
  return marpaESLIFValueFreeCallbackp;
}

/*****************************************************************************/
static short _marpaESLIF_bootstrap_G1_action_symbol_2b(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  /* <symbol name>  ::= <bracketed name> */
  marpaESLIF_t *marpaESLIFp = marpaESLIFValue_eslifp(marpaESLIFValuep);
  char         *barenames   = NULL;
  short         arrayb;
  char         *asciis; /* bare name is only ASCII letters as per the grammar */
  size_t        asciil;
  short         rcb;

  /* Cannot be nullable */
  if (nullableb) {
    MARPAESLIF_ERROR(marpaESLIFp, "Nullable mode is not supported");
    goto err;
  }

  /* Per def, because of the ::shift default action, <bracketed name> is of type array since this is a lexeme */
  if (! _marpaESLIFValue_stack_is_arrayb(marpaESLIFValuep, arg0i, &arrayb)) {
    goto err;
  }
  if (! arrayb) {
    MARPAESLIF_ERRORF(marpaESLIFp, "RHS No %d is of type ARRAY", arg0i);
    goto err;
  }
  if (! _marpaESLIFValue_stack_get_arrayb(marpaESLIFValuep, arg0i, NULL /* contextip */, (void **) &asciis, &asciil, NULL /* shallowbp */)) {
    goto err;
  }
  if ((asciis == NULL) || (asciil <= 0)) {
    /* Should never happen as per the grammar */
    MARPAESLIF_ERROR(marpaESLIFp, "Null bare name");
    goto err;
  }
  if (asciil < 2) {
    /* Should never happen neither as per the grammar */
    MARPAESLIF_ERRORF(marpaESLIFp, "Length of bare name is %ld", (unsigned long) asciil);
    goto err;
  }
  /* We just remove the '<' and '>' around... */
  barenames = (char *) malloc(asciil - 2 + 1);
  if (barenames == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  strncpy(barenames, asciis + 1, asciil - 2);
  barenames[asciil - 2] = '\0';

  if (! marpaESLIFValue_stack_set_ptrb(marpaESLIFValuep, resulti, MARPAESLIF_BOOTSTRAP_STACK_TYPE_SYMBOL_NAME, barenames, 0 /* shallowb */)) {
    goto err;
  }

  /* You will note that we are coherent will ALL the other <symbol name> rules: the outcome is an ASCII NUL terminated string pointer */
  rcb = 1;
  goto done;
 err:
  if (barenames != NULL) {
    free(barenames);
  }
  rcb = 0;
 done:
  return rcb;
}

/*****************************************************************************/
static short _marpaESLIF_bootstrap_G1_action_op_declare_1b(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  marpaESLIF_t *marpaESLIFp = marpaESLIFValue_eslifp(marpaESLIFValuep);
  /* <op declare> ::= <op declare top grammar> */

  /* Cannot be nullable */
  if (nullableb) {
    MARPAESLIF_ERROR(marpaESLIFp, "Nullable mode is not supported");
    return 0;
  }

  return marpaESLIFValue_stack_set_intb(marpaESLIFValuep, resulti, MARPAESLIF_BOOTSTRAP_STACK_TYPE_OP_DECLARE, 1);
}

/*****************************************************************************/
static short _marpaESLIF_bootstrap_G1_action_rhsb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  /* <rhs> ::= <rhs primary>+ */
  marpaESLIF_t                       *marpaESLIFp      = marpaESLIFValue_eslifp(marpaESLIFValuep);
  genericStack_t                     *rhsPrimaryStackp = NULL;
  marpaESLIF_bootstrap_rhs_primary_t *rhsPrimaryp      = NULL;
  int                                 i;
  short                rcb;

  /* Cannot be nullable */
  if (nullableb) {
    MARPAESLIF_ERROR(marpaESLIFp, "Nullable mode is not supported");
    goto err;
  }

  GENERICSTACK_NEW(rhsPrimaryStackp);
  if (GENERICSTACK_ERROR(rhsPrimaryStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "rhsPrimaryStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  for (i = arg0i; i <= argni; i++) {
    if (! marpaESLIFValue_stack_get_ptrb(marpaESLIFValuep, i, NULL /* contextip */, (void **) &rhsPrimaryp, NULL /* shallowbp */)) {
      return 0;
    }
    if (rhsPrimaryp == NULL) {
      MARPAESLIF_ERROR(marpaESLIFp, "An RHS primary is not set");
      goto err;
    }
    if (! marpaESLIFValue_stack_forgetb(marpaESLIFValuep, i)) {
      goto err;
    }

    GENERICSTACK_PUSH_PTR(rhsPrimaryStackp, rhsPrimaryp);
    if (GENERICSTACK_ERROR(rhsPrimaryStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "rhsStackp push failure, %s", strerror(errno));
      goto err;
    }
    rhsPrimaryp = NULL; /* rhsPrimaryp is now in rhsPrimaryStackp */
  }
  if (! marpaESLIFValue_stack_set_ptrb(marpaESLIFValuep, resulti, MARPAESLIF_BOOTSTRAP_STACK_TYPE_RHS, rhsPrimaryStackp, 0 /* shallowb */)) {
    goto err;
  }

  rcb = 1;
  goto done;

 err:
  _marpaESLIF_bootstrap_rhs_primary_freev(rhsPrimaryp);
  _marpaESLIF_bootstrap_rhs_freev(rhsPrimaryStackp);
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short _marpaESLIF_bootstrap_G1_action_adverb_list_itemsb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  /* <adverb list items> ::= <adverb item>* */
  marpaESLIF_t                            *marpaESLIFp          = marpaESLIFValue_eslifp(marpaESLIFValuep);
  genericStack_t                          *adverbListItemStackp = NULL;
  marpaESLIF_bootstrap_adverb_list_item_t *adverbListItemp      = NULL;
  void                                    *p;
  int                                      contexti;
  int                                      i;
  short                                    rcb;

  GENERICSTACK_NEW(adverbListItemStackp);
  if (GENERICSTACK_ERROR(adverbListItemStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "adverbListItemStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  /* In theory, if we are called, this is because there is something on the stack */
  /* In any case, this is okay to have an empty stack -; */
  if (! nullableb) {
    for (i = arg0i; i <= argni; i++) {
      if (! marpaESLIFValue_stack_get_ptrb(marpaESLIFValuep, i, &contexti, &p, NULL /* shallowbp */)) {
        goto err;
      }
      if (p != NULL) { /* NULL is the case of null adverb - ignored */
        adverbListItemp = (marpaESLIF_bootstrap_adverb_list_item_t *) malloc(sizeof(marpaESLIF_bootstrap_adverb_list_item_t));
        if (adverbListItemp == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
          goto err;
        }
        if (! marpaESLIFValue_stack_forgetb(marpaESLIFValuep, i)) {
          goto err;
        }
        adverbListItemp->type = MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_NA;
        switch (contexti) {
        case MARPAESLIF_BOOTSTRAP_STACK_TYPE_ACTION:
          adverbListItemp->type      = MARPAESLIF_BOOTSTRAP_ADVERB_LIST_ITEM_TYPE_ACTION;
          adverbListItemp->u.actions = (char *) p;
          break;
        default:
          MARPAESLIF_ERRORF(marpaESLIFp, "Unsupported adverb list item type %d", contexti);
          goto err;
          break;
        }
        GENERICSTACK_PUSH_PTR(adverbListItemStackp, (void *) adverbListItemp);
        if (GENERICSTACK_ERROR(adverbListItemStackp)) {
          MARPAESLIF_ERRORF(marpaESLIFp, "adverbListItemStackp push failure, %s", strerror(errno));
          goto err;
        }
        adverbListItemp = NULL; /* adverbListItemp is now in adverbListItemStackp */
      }
    }
  }

  if (! marpaESLIFValue_stack_set_ptrb(marpaESLIFValuep, resulti, MARPAESLIF_BOOTSTRAP_STACK_TYPE_ADVERB_LIST_ITEMS, adverbListItemStackp, 0 /* shallowb */)) {
    goto err;
  }

  rcb = 1;
  goto done;

 err:
  _marpaESLIF_bootstrap_adverb_list_item_freev(adverbListItemp);
  _marpaESLIF_bootstrap_adverb_list_items_freev(adverbListItemStackp);
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short _marpaESLIF_bootstrap_G1_action_actionb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  /* action ::= 'action' '=>' <action name> */
  marpaESLIF_t *marpaESLIFp = marpaESLIFValue_eslifp(marpaESLIFValuep);
  char         *actions = NULL;
  short         rcb;

  /* Cannot be nullable */
  if (nullableb) {
    MARPAESLIF_ERROR(marpaESLIFp, "Nullable mode is not supported");
    goto err;
  }

  /* <action name> is the result of ::ascii, i.e. a ptr in any case  */
  if (! marpaESLIFValue_stack_get_ptrb(marpaESLIFValuep, argni, NULL /* contextip */, (void **) &actions, NULL /* shallowbp */)) {
    goto err;
  }
  /* It is a non-sense to not have no action in this case */
  if (actions == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "marpaESLIFValue_stack_get_ptrb at indice %d returned %p", argni, actions);
    goto err;
  }
  /* "Tranfert" to the action stack type */
  if (! marpaESLIFValue_stack_forgetb(marpaESLIFValuep, argni)) {
    goto err;
  }
  if (! marpaESLIFValue_stack_set_ptrb(marpaESLIFValuep, resulti, MARPAESLIF_BOOTSTRAP_STACK_TYPE_ACTION, actions, 0 /* shallowb */)) {
    goto err;
  }
  
  rcb = 1;
  goto done;

 err:
  if (actions != NULL) {
    free(actions);
  }
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short _marpaESLIF_bootstrap_G1_action_rhs_primary_2b(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  /* <rhs primary> ::= <quoted string> */
  marpaESLIF_bootstrap_rhs_primary_t *rhsPrimaryp = NULL;
  marpaESLIF_t                       *marpaESLIFp = marpaESLIFValue_eslifp(marpaESLIFValuep);
  void                               *bytep;
  size_t                              bytel;
  short                               rcb;

  /* Cannot be nullable */
  if (nullableb) {
    MARPAESLIF_ERROR(marpaESLIFp, "Nullable mode is not supported");
    goto err;
  }

  /* action is the result of ::shift, i.e. a lexeme in any case  */
  if (! marpaESLIFValue_stack_get_arrayb(marpaESLIFValuep, arg0i, NULL /* contextip */, &bytep, &bytel, NULL /* shallowbp */)) {
    goto err;
  }
  /* It is a non-sense to not have valid information */
  if ((bytep == NULL) || (bytel <= 0)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "marpaESLIFValue_stack_get_arrayb at indice %d returned {%p,%ld}", argni, bytep, (unsigned long) bytel);
    goto err;
  }

  /* Make that an rhs primary structure */
  rhsPrimaryp = (marpaESLIF_bootstrap_rhs_primary_t *) malloc(sizeof(marpaESLIF_bootstrap_rhs_primary_t));
  if (rhsPrimaryp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  rhsPrimaryp->type = MARPAESLIF_BOOTSTRAP_RHS_PRIMARY_TYPE_NA;

  rhsPrimaryp->u.quotedStringp = (marpaESLIF_bootstrap_utf_string_t *) malloc(sizeof(marpaESLIF_bootstrap_utf_string_t));
  if (rhsPrimaryp->u.quotedStringp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  rhsPrimaryp->type                   = MARPAESLIF_BOOTSTRAP_RHS_PRIMARY_TYPE_QUOTED_STRING;
  rhsPrimaryp->u.quotedStringp->bytep = NULL;
  rhsPrimaryp->u.quotedStringp->bytel = 0;

  rhsPrimaryp->u.quotedStringp->bytep = (char *) malloc(bytel);
  if (rhsPrimaryp->u.quotedStringp->bytep == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  memcpy(rhsPrimaryp->u.quotedStringp->bytep, bytep, bytel);
  rhsPrimaryp->u.quotedStringp->bytel = bytel;

  if (! marpaESLIFValue_stack_set_ptrb(marpaESLIFValuep, resulti, MARPAESLIF_BOOTSTRAP_STACK_TYPE_RHS_PRIMARY, rhsPrimaryp, 0 /* shallowb */)) {
    goto err;
  }

  rcb = 1;
  goto done;

 err:
  _marpaESLIF_bootstrap_rhs_primary_freev(rhsPrimaryp);
  rcb = 0;

 done:
  return rcb;
}
