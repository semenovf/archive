/*
 * abnf-norm.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: wladt
 */

#include "abnf_p.hpp"

CWT_NS_BEGIN


/**
 *
 * @param element
 * @param rule
 * @param uniqn unique counter inside the rule
 * @param nestingValue
 * @return
 */
bool AbnfRuleSet::normalizeElement (AbnfAbstractContainer & container, AbnfRule & rule, int & uniqn, int nesting)
{
	bool r = true;

	switch (container.type()) {
	case Abnf_Altern:
		break;
	case Abnf_Concat:
		break;
	case Abnf_Rpt:
		break;
	case Abnf_Group:
		break;
	case Abnf_Option:
		break;
	default:
		break;
	}

	return r;
}

bool AbnfRuleSet::normalizeRule (AbnfRule & rule)
{
	bool r = true;
	const Vector<AbnfAbstractElement *> * elements = & rule.elements();

	Vector<AbnfAbstractElement *>::iterator it = elements->begin();
	Vector<AbnfAbstractElement *>::iterator itEnd = elements->end();

	for (; r && it != itEnd; ++it) {
		int uniqn = 0;
		int nesting = 0;
		AbnfAbstractElement * element = (*it);

		// AbnfComment is scalar too
		if (element->isScalar())
			continue;

		r = normalizeElement(*dynamic_cast<AbnfAbstractContainer *>(element), rule, uniqn, nesting);

		// May be new elements has been appended
		it = elements->begin();
		itEnd = elements->end();
	}

	return r;
}

bool AbnfRuleSet::normalize ()
{
	bool r = true;
	Vector<AbnfRule *>::iterator it = m_rules.begin();
	Vector<AbnfRule *>::iterator itEnd = m_rules.end();

	for (; r && it != itEnd; ++it) {
		AbnfRule * rule = *it;
		r = normalizeRule(*rule);
	}

	return true;
}

CWT_NS_END


