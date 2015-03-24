/*
 * sigslot_mapping.hpp
 *
 *  Created on: Feb 21, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */


#ifndef __PFS_SIGSLOT_MAPPING_HPP__
#define __PFS_SIGSLOT_MAPPING_HPP__

#include <pfs/sigslot.hpp>
#include <pfs/petaloid.hpp>

namespace pfs {

class sigslot_mapping_t
{
public:
	virtual ~sigslot_mapping_t() {}
	virtual void connectAll () = 0;
	virtual void disconnectAll () = 0;
	virtual void appendEmitter (emitter *e) = 0;
	virtual void appendDetector (petaloid * petaloid, detector d) = 0;
};

template <typename _emitterT, typename _detectorT>
struct _base_sigslot_mapping_t : public sigslot_mapping_t
{
	typedef vector<_emitterT*>    emitter_vector_t;
	typedef vector<detector_pair> detector_vector_t;

	emitter_vector_t  emitters;
	detector_vector_t detectors;

	virtual void connectAll ();
	virtual void disconnectAll ();
	virtual void appendEmitter (emitter *e) { emitters.append(reinterpret_cast<_emitterT*>(e)); }
	virtual void appendDetector (petaloid *p, detector d) { detectors.append(detector_pair (p, d));}
};

template <typename _emitterT, typename _detectorT>
void _base_sigslot_mapping_t<_emitterT, _detectorT>::connectAll()
{
	if( emitters.size() == 0 || detectors.size() == 0 )
		return;

    typename emitter_vector_t::const_iterator itEnd = emitters.cend();
	typename detector_vector_t::const_iterator itdEnd = detectors.cend();

	for( typename emitter_vector_t::const_iterator it = emitters.cbegin(); it != itEnd; it++ ) {
		for( typename detector_vector_t::const_iterator itd = detectors.cbegin(); itd != itdEnd; itd++ ) {
			_emitterT * em = *it;
			em->connect(itd->_petaloid, reinterpret_cast<_detectorT>(itd->_detector));
		}
	}
}

template <typename _emitterT, typename _detectorT>
void _base_sigslot_mapping_t<_emitterT, _detectorT>::disconnectAll()
{
    typename emitter_vector_t::const_iterator itEnd = emitters.cend();

	for( typename emitter_vector_t::const_iterator it = emitters.cbegin(); it != itEnd; it++ ) {
		_emitterT * em = *it;
		em->disconnect_all();
	}
}


/*template <template typename notused = NULL>*/
struct sigslot_mapping0_t : public _base_sigslot_mapping_t<signal0<>, void (petaloid::*)()> {};

template <typename a0>
struct sigslot_mapping1_t : public _base_sigslot_mapping_t<signal1<a0>, void (petaloid::*)(a0)> {};

template <typename a0, typename a1>
struct sigslot_mapping2_t : public _base_sigslot_mapping_t<signal2<a0, a1>, void (petaloid::*)(a0, a1)> {};

template <typename a0, typename a1, typename a2>
struct sigslot_mapping3_t : public _base_sigslot_mapping_t<signal3<a0, a1, a2>, void (petaloid::*)(a0, a1, a2)> {};

template <typename a0, typename a1, typename a2, typename a3>
struct sigslot_mapping4_t : public _base_sigslot_mapping_t<signal4<a0, a1, a2, a3>, void (petaloid::*)(a0, a1, a2, a3)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4>
struct sigslot_mapping5_t : public _base_sigslot_mapping_t<signal5<a0, a1, a2, a3, a4>, void (petaloid::*)(a0, a1, a2, a3, a4)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5>
struct sigslot_mapping6_t : public _base_sigslot_mapping_t<signal6<a0, a1, a2, a3, a4, a5>, void (petaloid::*)(a0, a1, a2, a3, a4, a5)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6>
struct sigslot_mapping7_t : public _base_sigslot_mapping_t<signal7<a0, a1, a2, a3, a4, a5, a6>, void (petaloid::*)(a0, a1, a2, a3, a4, a5, a6)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7>
struct sigslot_mapping8_t : public _base_sigslot_mapping_t<signal8<a0, a1, a2, a3, a4, a5, a6, a7>, void (petaloid::*)(a0, a1, a2, a3, a4, a5, a6, a7)> {};

} // pfs

#endif /* __PFS_SIGSLOT_MAPPING_HPP__ */
