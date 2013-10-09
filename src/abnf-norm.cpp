/*
 * abnf-norm.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: wladt
 */

#include "abnf_p.hpp"

CWT_NS_BEGIN

/*
 *
 * @param element
 * @param rule
 * @param uniqn unique counter inside the rule
 * @return
 */
bool AbnfRuleList::normalizeElement (AbnfElement & element, AbnfRule & rule, int & uniqn)
{
	bool r = true;

	if (element.isScalar()) {
		return true;
	}

	AbnfContainer & container = dynamic_cast<AbnfContainer &>(element);

	Vector<AbnfElement *> * elements = & container.m_elements;
	Vector<AbnfElement *>::iterator it = elements->begin();
	Vector<AbnfElement *>::iterator itEnd = elements->end();

	for (; it != itEnd; ++it) {
		AbnfElement * elem = *it;

		if (elem->isScalar())
			continue;

		// Replaceable element.
		// For repetition it is a child element (which is a container).
		Vector<AbnfElement *>::iterator itRepl(it);

		if (elem->type() == Abnf_Rpt) {
			AbnfRpt * rpt = dynamic_cast<AbnfRpt *>(elem);
			Vector<AbnfElement *> & rpt_elements = rpt->m_elements;

			CWT_ASSERT(rpt_elements.size() == 1);

			if (rpt_elements[0]->isScalar())
				continue;

			itRepl = rpt_elements.begin();
			elem = rpt_elements[0];
		}

		String newRuleName = rule.name() + '-' + String::number(uniqn++);
		newRule(newRuleName).add(*elem);
		*itRepl = & Abnf::newRuleRef(newRuleName);

		r = normalizeElement(*elem, rule, uniqn);
		if (!r)
			break;
	}

	return r;
}

bool AbnfRuleList::normalizeRule (AbnfRule & rule)
{
	bool r = true;
	Vector<AbnfElement *> * elements = & rule.m_elements;

	// Rule has only one element (top-level container: Altern, Concat or Rpt)
	CWT_ASSERT(elements->size() == 1);

	int uniqn = 0;
	AbnfElement * element = (*elements)[0];

	r = normalizeElement(*element, rule, uniqn);

	return r;
}

bool AbnfRuleList::normalize ()
{
	bool r = true;

	Vector<AbnfRule *> & rulesRef = rules();

	for (size_t i = 0; r && i < rulesRef.size(); ++i) {
		r = normalizeRule(*rulesRef[i]);
	}

	return true;
}

CWT_NS_END


