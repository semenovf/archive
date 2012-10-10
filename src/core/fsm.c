/**
 * @file   fsm.c
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Implementation of
 */

#include <cwt/fsm.h>
#include <string.h>
#include <cwt/str.h>

static ssize_t __fsm_exec(CwtFsm *fsm, const void *data, size_t len);

static CwtFsmNS __cwtFsmNS = {
	__fsm_exec
};


/*
static void __beginTransition(int tid);
static void __nextTransition(void);
static void __hasMoreTranstions(void);
*/


DLL_API_EXPORT CwtFsmNS* cwtFsmNS(void)
{
	return &__cwtFsmNS;
}

#define _BELONG_CHAR(char_type,ch,subset,n)                           \
	size_t i = 0;                                                     \
	while( i < (n) ) {                                                \
		if( ((const char_type*)(subset))[i++] == *((char_type*)(ch))) \
			return TRUE;                                              \
	}                                                                 \
	return FALSE;

/*
#define _EXACT_STR(char_type,s1,n1,s2,n2)                       \
	size_t i = 0;                                               \
	if( (n1) != (n2) )                                          \
		return FALSE;                                           \
	while( i < (n1)                                             \
		&& (*((char_type*)(s1)++) == *((char_type*)(s2)++)) ) { \
		i++;                                                    \
	}                                                           \
	return i == (n1) ? TRUE : FALSE;
*/
#define _EXACT_STR(char_type,s1,n1,s2,n2)                       \
	if( (n1) != (n2) ) return FALSE;                            \
	return memcmp(s1, s2, (n1) * sizeof(char_type)) == 0        \
		? TRUE : FALSE;

DLL_API_EXPORT BOOL cwtBelongChar(void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(char, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactChar(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(char, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwtBelongCwtChar(void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(CWT_CHAR, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactCwtChar(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(CWT_CHAR, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwtBelongInt(void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(int, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactInt(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(int, s, n1, seq, n2);
}


static ssize_t __fsm_exec(CwtFsm *fsm, const void *data, size_t len)
{
	CWT_UNUSED(fsm);
	CWT_UNUSED(data);
	CWT_UNUSED(len);
#ifdef __COMMENT__
	int tid = 0;
	int nchars_processed = 0;
	/*int in_accept = 0;*/

	  /* all possible states are numbered positively */
	while( tid >= 0) {
		CwtFsmTransition *trans;

/*
	    int nbytes_used_transing;
	    int successful_trans = 0;
*/

	    /* walk the action table, looking for the first transition where
	       both the current_state matches, and the character / string
	       matching or the FSM execution succeeds */
	    for(trans = action_table;
		current_trans->current_state != -1;
		current_trans++) {

	      /* printf("attempting to transition from %s at state %d\n", *data, current_state); */

	      /* check to see if the current state is right, and the
		 transition condition succeeds */
	      if(current_state == current_trans->current_state) {
		/* we need a copy of the data pointer because, if a failing
		   transition happens where some of the data is processed in
		   another FSM, we can not have that sub-FSM moving our data
		   pointer, so we give it a copy, and only let ourselves move it
		   based on the returned amount of processed bytes */
		char *data_copy = *data;


		/* if we are in a transition moving from our current state.. */
		if((nbytes_used_transing = run_transition(current_trans, &data_copy, context, dup_context, free_context)) >= 0) {
		  /* successful transition! run the function to be executed on
		     transition (if there is one), then move forward the number
		     of bytes processed in the input stream */
		  /* printf("run_transition success\n"); */
		  if(current_trans->transfn != NULL) {
		    current_trans->transfn(data, nbytes_used_transing, (context == NULL) ? NULL : *context, current_trans->local_context);
		  }

		  /* move forward the number of bytes used transitioning */
		  nbytes_processed += nbytes_used_transing;
		  *data += nbytes_used_transing;

		  /* change the state to the success state */
		  current_state = current_trans->state_pass;

		  /* if the target of this transition was an accept state,
		     mark that, otherwise, clear the in_accept variable */
		  in_accept = 0;
		  if(current_trans->type == ACCEPT) {
		    in_accept = 1;
		  } else if (current_trans->type == REJECT) {
		    /* if we are in a reject state, then we immediately abort */
		    return -1;
		  } else {
		    /* our target was a normal state, nothing special to do */
		  }


		  /* printf("transition done, data aligned at %s\n", *data); */

		  /* finally, mark this as a successful transition, and break
		     from the transition-hunting for loop */
		  successful_trans = 1;
		  break;
		} else {
		  /* the transition failed. check to see if the state_fail is
		     positive indicating that there is a state to move to if
		     this transition fails. if the state_fail is negative, it
		     just gets ignored */
		  if(current_trans->state_fail >= 0) {
		    current_state = current_trans->state_fail;
		  }
		}
	      }
	    }

	    /* if we are done with the for loop, and there was NOT a
	       successful transition, then move to the error state, -1 */
	    if(successful_trans == 0) {
	      current_state = -1;
	    }


	  }

	  /* return 0 or more if we landed in an ACCEPT state, and -1 if we
	     were unable to use the FSM to parse the input */
	  return (in_accept == 1) ? nbytes_processed : -1;

#endif
	return (ssize_t)-1;
}
