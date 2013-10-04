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
bool AbnfRuleSet::normalizeElement (AbnfAbstractContainer & container, AbnfRule & rule, int & uniqn)
{
	bool r = true;

	Vector<AbnfAbstractElement *> * elements = & container.m_elements;
	Vector<AbnfAbstractElement *>::iterator it = elements->begin();
	Vector<AbnfAbstractElement *>::iterator itEnd = elements->end();

	for (; it != itEnd; ++it) {

		// Ignore repetition of scalar
		if ((*it)->type() == Abnf_Rpt) {
			const Vector<AbnfAbstractElement *> & rpt_elements = dynamic_cast<const AbnfAbstractContainer *>(*it)->elements();
			if (rpt_elements.size() == 1 && rpt_elements[0]->isScalar())
				continue;
		}

		if (! (*it)->isScalar()) {
			String newRuleName = rule.name() + '_' + String::number(uniqn++);
			newRule(newRuleName).add(**it);
			*it = & Abnf::newRuleRef(newRuleName);
		}
	}

	return r;
}

bool AbnfRuleSet::normalizeRule (AbnfRule & rule)
{
	bool r = true;
	Vector<AbnfAbstractElement *> * elements = & rule.m_elements;

	// each iteration elements may changes the size (grows after appending new rules)
	for (size_t i = 0; r && i < elements->size(); ++i) {
		int uniqn = 0;
		AbnfAbstractElement * element = (*elements)[i];

		// AbnfComment is scalar too
		if (element->isScalar())
			continue;

		r = normalizeElement(*dynamic_cast<AbnfAbstractContainer *>(element), rule, uniqn);
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


