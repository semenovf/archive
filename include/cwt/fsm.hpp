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
#include <cwt/string.hpp>
#include <cstring>

CWT_NS_BEGIN

/* There are common predefined macros.
 */
#define FSM_MATCH_SEQ(n)          FsmMatch<String>(new FsmMatchSeq<String>(n))
#define FSM_MATCH_RANGE(f,t)      FsmMatch<String>(new FsmMatchRange<String>(f,t))
#define FSM_MATCH_FSM(tr)         FsmMatch<String>(new FsmMatchFsm<String>(tr))
#define FSM_MATCH_OPT_FSM(tr)     FsmMatch<String>(new FsmMatchRpt<String>(FsmMatch<String>(new FsmMatchFsm<String>(tr)),0,1))
#define FSM_MATCH_FUNC(f,pcont)   FsmMatch<String>(new FsmMatchFunc<String>(f,pcont))
#define FSM_MATCH_RPT_FSM(tr,f,t) FsmMatch<String>(new FsmMatchRpt<String>(FsmMatch<String>(new FsmMatchFsm<String>(tr)),f,t))
#define FSM_MATCH_NOTHING         FsmMatch<String>(new FsmMatchNothing<String>)
#define FSM_MATCH_CHAR(s)         FsmMatch<String>(new FsmMatchChar<String>(s))
#define FSM_MATCH_STR(s)          FsmMatch<String>(new FsmMatchStr<String>(s))
#define FSM_MATCH_RPT_STR(s,f,t)  FsmMatch<String>(new FsmMatchRpt<String>(FSM_MATCH_STR(s),f,t))
#define FSM_MATCH_OPT_STR(s)      FsmMatch<String>(new FsmMatchRpt<String>(FSM_MATCH_STR(s),0,1))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch<String>(new FsmMatchRpt<String>(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_OPT_CHAR(s)     FsmMatch<String>(new FsmMatchRpt<String>(FSM_MATCH_CHAR(s),0,1))

#define FSM_NORMAL  0
#define FSM_REJECT  1
#define FSM_ACCEPT  2

template <typename _P>
struct FsmContext;

template <typename _P>
struct FsmTransition;

struct FsmRptBounds {
	int from, to;
};

template <typename _P>
class FsmMatchBase
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchBase() : ref(1) {}
	virtual ~FsmMatchBase() {}
	virtual ssize_t match(FsmContext<_P> *fsm, const const_iterator & begin, const const_iterator & end) const = 0;
	int ref;
};

template <typename _P>
class FsmMatch
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;
	typedef ssize_t (*func_type)(FsmContext<_P> *fsm, void *fn_context, const typename _P::const_iterator & begin, const typename _P::const_iterator & end);
private:
	FsmMatch() : m_match(nullptr) {}
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
	FsmMatch(FsmMatchBase<_P> * match) : m_match(match) { CWT_ASSERT(m_match); }
	FsmMatch(const FsmMatch &match) {
		CWT_ASSERT(match.m_match);
		m_match = match.m_match;
		m_match->ref++; }
	~FsmMatch() { deref(); }

	ssize_t operator () (FsmContext<_P> *fsm, const const_iterator & begin, const const_iterator & end) const {
		return m_match->match(fsm, begin, end);
	}
private:
	FsmMatchBase<_P>* m_match;
};

template <typename _P>
struct FsmTransition
{
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	int state_next;
	int state_fail;
	FsmMatch<_P> match;
	int status; /* last entry in the chain of ...*/
	bool (*action)(const const_iterator & begin, const const_iterator & end, void *context, void *action_args);
	void *action_args;
};

template <typename _P>
struct FsmContext {
	const FsmTransition<_P> *trans_tab;
	void *context;               /* user context */
};


template <typename _P>
class Fsm
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

public:
	Fsm();
	Fsm(const FsmTransition<_P> *initialTrans, void *context);
	~Fsm();

	void setTransitionTable(FsmTransition<_P> *trans) { m_fsmContext->trans_tab = trans; }
	void setUserContext(void * context) { m_fsmContext->context = context; }
	ssize_t exec(int state_cur, const const_iterator & begin, const const_iterator & end);

public:
	static bool belongsChar(char_type ch, const const_iterator & begin, const const_iterator & end);
	static bool containsChars(const const_iterator & needle_begin, const const_iterator & needle_end
			, const const_iterator & haystack_begin, const const_iterator & haystack_end);
	static bool rangeChar(char_type ch, char_type from, char_type to);

private:
	FsmContext<_P> *m_fsmContext;
};


/** @brief Checks if character 'ch' belongs to the set of characters subset
 * */
template <typename _P>
bool Fsm<_P>::belongsChar(char_type ch, const const_iterator & begin, const const_iterator & end)
{
	const_iterator it(begin);
	while (it < end) {
		if (ch == it.value())
			return true;
		++it;
	}
	return false;
}

/** @brief Checks if string 's' exactly equals to sequence of characters 'seq'
 * */
template <typename _P>
bool Fsm<_P>::containsChars(const const_iterator & needle_begin, const const_iterator & needle_end
		, const const_iterator & haystack_begin, const const_iterator & haystack_end)
{
	const_iterator it_needle(needle_begin);
	const_iterator it_haystack(haystack_begin);

	if (needle_begin == needle_end)
		return false;

	while(it_needle < needle_end && it_haystack < haystack_end) {
		if (it_needle.value() != it_haystack.value())
			break;
		++it_needle;
		++it_haystack;
	}

	return it_needle == needle_end ? true : false;
}

/** @brief Checks if character 'ch' belongs to the specified range
 * */
template <typename _P>
inline bool Fsm<_P>::rangeChar(char_type ch, char_type from, char_type to)
{
	return ch >= from && ch <= to ? true : false;
}

template <typename _P>
Fsm<_P>::Fsm()
	: m_fsmContext(new FsmContext<_P>)
{
	m_fsmContext->trans_tab     = nullptr;
	m_fsmContext->context       = nullptr;
}

template <typename _P>
Fsm<_P>::Fsm( const FsmTransition<_P> *initialTrans, void *context)
	: m_fsmContext(new FsmContext<_P>)
{
	m_fsmContext->trans_tab     = initialTrans;
	m_fsmContext->context       = context;
}

template <typename _P>
Fsm<_P>::~Fsm()
{
	if (m_fsmContext) {
		delete m_fsmContext;
	}
}

CWT_NS_END

#include <cwt/fsm_match.hpp>
#include <cwt/fsm_exec.hpp>

#endif /* __CWT_FSM_H__ */
