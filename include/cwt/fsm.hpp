/**
 * @file   fsm.hpp
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Header file for Finite State Machine definitions.
 */

#ifndef __CWT_FSM_HPP__
#define __CWT_FSM_HPP__

#include <cwt/cwt.h>
#include <cwt/char.hpp>
#include <cwt/string.hpp>
#include <cstring>

CWT_NS_BEGIN

/* There are common predefined macros.
 * There are no predefined macros FSM_MATCH_STR and FSM_MATCH_CHAR.
 * They are specific for sample storage (e.g. plain array or String)
 * and can/must be defined separately.
 */
#define FSM_MATCH_SEQ(n)          FsmMatch(new FsmMatchSeq(n))
#define FSM_MATCH_RANGE(f,t)      FsmMatch(new FsmMatchRange(f,t))
#define FSM_MATCH_FSM(tr)         FsmMatch(new FsmMatchFsm(tr))
#define FSM_MATCH_OPT_FSM(tr)     FsmMatch(new FsmMatchRpt(FsmMatch(new FsmMatchFsm(tr)),0,1))
#define FSM_MATCH_FUNC(f,pcont)   FsmMatch(new FsmMatchFunc(f,pcont))
#define FSM_MATCH_RPT_FSM(tr,f,t) FsmMatch(new FsmMatchRpt(FsmMatch(new FsmMatchFsm(tr)),f,t))
#define FSM_MATCH_NOTHING         FsmMatch(new FsmMatchNothing)

#define FSM_NORMAL  0
#define FSM_REJECT  1
#define FSM_ACCEPT  2

struct FsmMatchBase;
struct FsmContext;
struct FsmTransition;

struct FsmRptBounds {
	int from, to;
};
typedef ssize_t (*FsmFunc)(FsmContext *fsm, void *fn_context, const void *data, size_t len);

struct FsmMatchBase
{
	FsmMatchBase() : ref(1) {}
	virtual ~FsmMatchBase() {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const = 0;
	int ref;
};

class FsmMatch
{
private:
	FsmMatch() : m_match(NULL) {}
	void deref() {
		if (m_match) {
			CWT_ASSERT(m_match->ref > 0);
			m_match->ref--;
			if(m_match->ref == 0)
				delete m_match;
			m_match = NULL;
		}
	}
public:
	FsmMatch(FsmMatchBase *match) : m_match(match) { CWT_ASSERT(m_match); }
	FsmMatch(const FsmMatch &match) {
		CWT_ASSERT(match.m_match);
		m_match = match.m_match;
		m_match->ref++; }
	~FsmMatch() { deref(); }

	ssize_t operator () (FsmContext *fsm, const void *data, size_t len) const {
		return m_match->match(fsm, data, len);
	}

private:
	FsmMatchBase* m_match;
};

/* Matches sequence of characters */
struct FsmMatchSeq : public FsmMatchBase
{
	FsmMatchSeq(size_t len) : m_len(len) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	size_t m_len;
};

/* Matches string of characters */
struct FsmMatchStr : public FsmMatchBase
{
	FsmMatchStr(const void *str, size_t len) : m_len(len), m_str(str) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	size_t m_len;
	const void *m_str;
};

/* Matches string of characters from String object */
struct FsmMatchStringStr : public FsmMatchBase
{
	FsmMatchStringStr(const String &str) : m_str(str) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	String m_str;
};


/* Matches one character from characters */
struct FsmMatchChar : public FsmMatchBase
{
	FsmMatchChar(const void *chars, size_t count) : m_count(count), m_chars(chars) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	size_t m_count;
	const void *m_chars;
};

/* Matches one character from String object */
struct FsmMatchStringChar : public FsmMatchBase
{
	FsmMatchStringChar(const String &str) : m_str(str) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	String m_str;
};


struct FsmMatchRange : public FsmMatchBase
{
	FsmMatchRange( const void *from, const void *to) : m_from(from), m_to(to) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	const void *m_from, *m_to;
};

struct FsmMatchFsm : public FsmMatchBase
{
	FsmMatchFsm(const FsmTransition *trans) : m_trans(trans) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	const FsmTransition *m_trans;
};

struct FsmMatchFunc : public FsmMatchBase
{
	FsmMatchFunc(FsmFunc fn, void *fnContext) : m_fn(fn), m_fnContext(fnContext) {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;
	FsmFunc m_fn;
	void *m_fnContext;
};

class FsmMatchRpt : public FsmMatchBase
{
public:
	FsmMatchRpt(const FsmMatch &match, int from, int to) : m_match(match) {
		m_bounds.from = from; m_bounds.to = to;
	}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const;

private:
	FsmMatch     m_match;
	FsmRptBounds m_bounds;
};

struct FsmMatchNothing : public FsmMatchBase
{
	FsmMatchNothing() {}
	virtual ssize_t match(FsmContext *fsm, const void *data, size_t len) const {
		CWT_UNUSED3(fsm, data, len);
		return 0;
	}
};

struct FsmTransition {
	int state_next;
	int state_fail;
	FsmMatch match;
	int status; /* last entry in the chain of ...*/
	bool (*action)(const void *data, size_t len, void *context, void *action_args);
	void *action_args;
};

/** @brief Checks if character 'ch' belongs to the set of characters subset */
typedef bool (*FsmBelongCharFn) (const void *ch, const void *subset, size_t n);

/** @brief Checks if string 's' exactly equals to sequence of characters 'seq' */
typedef bool (*FsmExactCharFn)  (const void *s, size_t n1, const void *seq, size_t n2);

/** @brief Checks if character 'ch' belongs to the specified range */
typedef bool (*FsmRangeCharFn)  (const void *ch, const void *from, const void *to);

struct FsmContext {
	const FsmTransition *trans_tab;
	int   sizeof_char;           /* size of character */
	void *context;               /* user context */

	FsmBelongCharFn belongCharFn;
	FsmExactCharFn  exactCharFn;
	FsmRangeCharFn  rangeCharFn;
};

extern ssize_t execFsmContext (FsmContext *fsmContext, int state_cur, const void *data, size_t len);

template<typename char_type>
class Fsm {
private:
	Fsm() : m_fsmContext(NULL) {}
public:
	Fsm(const FsmTransition *initialTrans, void *context);
	~Fsm();

	void setTransitionTable(FsmTransition *trans) { if (m_fsmContext) m_fsmContext->trans_tab = trans; }
	ssize_t exec(int state_cur, const void *data, size_t len) { return execFsmContext(m_fsmContext, state_cur, data, len); }

private:
	FsmContext *m_fsmContext;
};

template<typename char_type>
bool belongCharType(const void *ch, const void *subset, size_t n)
{
	size_t i = 0;
	const char_type *ss = (const char_type*)subset;
	char_type c = *(char_type*)ch;
	while (i < n) {
		if (ss[i++] == c)
			return true;
	}
	return false;
}

template<typename char_type>
inline bool exactCharType(const void *s, size_t n1, const void *seq, size_t n2)
{
	return n1 != n2
		? false
		: memcmp(s, seq, n1 * sizeof(char_type)) == 0
		  	  ? true : false;
}

template<typename char_type>
inline bool rangeCharType(const void *ch, const void *from, const void *to)
{
	return *((const char_type*)ch) >= *((const char_type*)from)
		&& *((const char_type*)ch) <= *((const char_type*)to)
		? true : false;
}

template<typename char_type>
Fsm<char_type>::Fsm( const FsmTransition *initialTrans, void *context )
	: m_fsmContext(new FsmContext)
{
	m_fsmContext->trans_tab     = initialTrans;
	m_fsmContext->sizeof_char   = sizeof(char_type);
	m_fsmContext->context       = context;
	m_fsmContext->belongCharFn  = belongCharType<char_type>;
	m_fsmContext->exactCharFn   = exactCharType<char_type>;
	m_fsmContext->rangeCharFn   = rangeCharType<char_type>;
}

template<typename char_type>
Fsm<char_type>::~Fsm()
{
	if (m_fsmContext) {
		delete m_fsmContext;
	}
}

CWT_NS_END

#endif /* __CWT_FSM_H__ */
