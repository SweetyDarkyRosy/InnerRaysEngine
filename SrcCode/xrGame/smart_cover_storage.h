////////////////////////////////////////////////////////////////////////////
//	Module 		: smart_cover.h
//	Created 	: 16.08.2007
//	Author		: Alexander Dudin
//	Description : Smart cover storage class
////////////////////////////////////////////////////////////////////////////

#ifndef __INNER_RAYS_ENGINE_SMART_COVER_STORAGE_CLASS_H__
#define __INNER_RAYS_ENGINE_SMART_COVER_STORAGE_CLASS_H__

#include <boost/noncopyable.hpp>
#include "smart_cover.h"


namespace smart_cover
{
	class storage : private boost::noncopyable
	{
	public:
		typedef xr_vector<smart_cover::description*>		Descriptions;
		typedef cover::DescriptionPtr						DescriptionPtr;

	private:
		Descriptions m_descriptions;

	public:
		// Destructor
		~storage();

	public:
		DescriptionPtr description(shared_str const& table_id);
		void collect_garbage();

	};

} // !namespace smart_cover

#endif // !__INNER_RAYS_ENGINE_SMART_COVER_STORAGE_CLASS_H__
