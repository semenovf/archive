/*
 * sigslot_mapping.hpp
 *
 *  Created on: Feb 21, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */


#ifndef __CWT_UI_SIGSLOT_MAPPING_HPP__
#define __CWT_UI_SIGSLOT_MAPPING_HPP__

#include <cwt/cwt.h>
#include <cwt/sigslot.hpp>
#include <cwt/petaloid.hpp>

CWT_NS_BEGIN

class sigslot_mapping_t
{
public:
	virtual ~sigslot_mapping_t() {}
	virtual void connectAll() = 0;
	virtual void appendEmitter(Emitter *e) = 0;
	virtual void appendDetector(Petaloid *petaloid, Detector d) = 0;
};

template <typename _emitterT, typename _detectorT>
struct _base_sigslot_mapping_t : public sigslot_mapping_t
{
	typedef Vector<_emitterT*>   emitter_vector_t;
	typedef Vector<DetectorPair> detector_vector_t;

	emitter_vector_t  emitters;
	detector_vector_t detectors;

	virtual void connectAll();
	virtual void appendEmitter(Emitter *e) { emitters.append(reinterpret_cast<_emitterT*>(e)); }
	virtual void appendDetector(Petaloid *p, Detector d) { detectors.append(DetectorPair (p, d));}
};

template <typename _emitterT, typename _detectorT>
void _base_sigslot_mapping_t<_emitterT, _detectorT>::connectAll()
{
	if( emitters.size() == 0 || detectors.size() == 0 )
		return;

    typename emitter_vector_t::const_iterator itEnd = emitters.end();
	typename detector_vector_t::const_iterator itdEnd = detectors.end();

	for( typename emitter_vector_t::const_iterator it = emitters.begin(); it != itEnd; it++ ) {
		for( typename detector_vector_t::const_iterator itd = detectors.begin(); itd != itdEnd; itd++ ) {
			(*it)->connect(itd->petaloid, reinterpret_cast<_detectorT>(itd->detector));
		}
	}
}

/*template <template typename notused = NULL>*/
struct sigslot_mapping0_t : public _base_sigslot_mapping_t<signal0<>, void (Petaloid::*)()> {};

template <typename a0>
struct sigslot_mapping1_t : public _base_sigslot_mapping_t<signal1<a0>, void (Petaloid::*)(a0)> {};

template <typename a0, typename a1>
struct sigslot_mapping2_t : public _base_sigslot_mapping_t<signal2<a0, a1>, void (Petaloid::*)(a0, a1)> {};

template <typename a0, typename a1, typename a2>
struct sigslot_mapping3_t : public _base_sigslot_mapping_t<signal3<a0, a1, a2>, void (Petaloid::*)(a0, a1, a2)> {};

template <typename a0, typename a1, typename a2, typename a3>
struct sigslot_mapping4_t : public _base_sigslot_mapping_t<signal4<a0, a1, a2, a3>, void (Petaloid::*)(a0, a1, a2, a3)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4>
struct sigslot_mapping5_t : public _base_sigslot_mapping_t<signal5<a0, a1, a2, a3, a4>, void (Petaloid::*)(a0, a1, a2, a3, a4)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5>
struct sigslot_mapping6_t : public _base_sigslot_mapping_t<signal6<a0, a1, a2, a3, a4, a5>, void (Petaloid::*)(a0, a1, a2, a3, a4, a5)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6>
struct sigslot_mapping7_t : public _base_sigslot_mapping_t<signal7<a0, a1, a2, a3, a4, a5, a6>, void (Petaloid::*)(a0, a1, a2, a3, a4, a5, a6)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7>
struct sigslot_mapping8_t : public _base_sigslot_mapping_t<signal8<a0, a1, a2, a3, a4, a5, a6, a7>, void (Petaloid::*)(a0, a1, a2, a3, a4, a5, a6, a7)> {};

CWT_NS_END

#endif /* __CWT_UI_SIGSLOT_MAPPING_HPP__ */
