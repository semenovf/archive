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
#include <pfs/module.hpp>

namespace pfs {

class sigslot_mapping_t
{
public:
	virtual ~sigslot_mapping_t() {}
	virtual void connect_all () = 0;
	virtual void disconnect_all () = 0;
	virtual void append_emitter (emitter *e) = 0;
	virtual void append_detector (module * m, detector d) = 0;
};

template <typename _emitterT, typename _detectorT>
struct sigslot_mapping_base : public sigslot_mapping_t
{
	typedef vector<_emitterT*>    emitter_vector_t;
	typedef vector<detector_pair> detector_vector_t;

	emitter_vector_t  emitters;
	detector_vector_t detectors;

	virtual void connect_all ();
	virtual void disconnect_all ();

	virtual void append_emitter (emitter * e)
	{
		emitters.push_back(reinterpret_cast<_emitterT*>(e));
	}

	virtual void append_detector (module * m, detector d)
	{
		detectors.push_back(detector_pair (m, d));
	}
};

template <typename _emitterT, typename _detectorT>
void sigslot_mapping_base<_emitterT, _detectorT>::connect_all()
{
	if( emitters.size() == 0 || detectors.size() == 0 )
		return;

    typename emitter_vector_t::const_iterator itEnd = emitters.cend();
	typename detector_vector_t::const_iterator itdEnd = detectors.cend();

	for( typename emitter_vector_t::const_iterator it = emitters.cbegin(); it != itEnd; it++ ) {
		for( typename detector_vector_t::const_iterator itd = detectors.cbegin(); itd != itdEnd; itd++ ) {
			_emitterT * em = *it;
			em->connect(itd->_module, reinterpret_cast<_detectorT>(itd->_detector));
		}
	}
}

template <typename _emitterT, typename _detectorT>
void sigslot_mapping_base<_emitterT, _detectorT>::disconnect_all()
{
    typename emitter_vector_t::const_iterator itEnd = emitters.cend();

	for( typename emitter_vector_t::const_iterator it = emitters.cbegin(); it != itEnd; it++ ) {
		_emitterT * em = *it;
		em->disconnect_all();
	}
}


/*template <template typename notused = NULL>*/
struct sigslot_mapping0_t : public sigslot_mapping_base<signal0<>, void (module::*)()> {};

template <typename a0>
struct sigslot_mapping1_t : public sigslot_mapping_base<signal1<a0>, void (module::*)(a0)> {};

template <typename a0, typename a1>
struct sigslot_mapping2_t : public sigslot_mapping_base<signal2<a0, a1>, void (module::*)(a0, a1)> {};

template <typename a0, typename a1, typename a2>
struct sigslot_mapping3_t : public sigslot_mapping_base<signal3<a0, a1, a2>, void (module::*)(a0, a1, a2)> {};

template <typename a0, typename a1, typename a2, typename a3>
struct sigslot_mapping4_t : public sigslot_mapping_base<signal4<a0, a1, a2, a3>, void (module::*)(a0, a1, a2, a3)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4>
struct sigslot_mapping5_t : public sigslot_mapping_base<signal5<a0, a1, a2, a3, a4>, void (module::*)(a0, a1, a2, a3, a4)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5>
struct sigslot_mapping6_t : public sigslot_mapping_base<signal6<a0, a1, a2, a3, a4, a5>, void (module::*)(a0, a1, a2, a3, a4, a5)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6>
struct sigslot_mapping7_t : public sigslot_mapping_base<signal7<a0, a1, a2, a3, a4, a5, a6>, void (module::*)(a0, a1, a2, a3, a4, a5, a6)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7>
struct sigslot_mapping8_t : public sigslot_mapping_base<signal8<a0, a1, a2, a3, a4, a5, a6, a7>, void (module::*)(a0, a1, a2, a3, a4, a5, a6, a7)> {};

} // pfs

#endif /* __PFS_SIGSLOT_MAPPING_HPP__ */
