////////////////////////////////////////////////////////////////////////////
//	Module 		: smart_cover_animation_selector_inline.h
//	Created 	: 07.09.2007
//	Author		: Alexander Dudin
//	Description : Animation selector for smart covers inline functions
////////////////////////////////////////////////////////////////////////////

#ifndef __INNER_RAYS_ENGINE_SMART_COVER_ANIMATION_SELECTOR_CLASS_INLINE_IMPLEMENTATIONS_H__
#define __INNER_RAYS_ENGINE_SMART_COVER_ANIMATION_SELECTOR_CLASS_INLINE_IMPLEMENTATIONS_H__

namespace smart_cover
{
	IC CPropertyStorage* animation_selector::property_storage()
	{
		return m_storage;
	}

	IC animation_planner& animation_selector::planner()
	{
		return *m_planner;
	}

} // !namespace smart_cover

#endif // !__INNER_RAYS_ENGINE_SMART_COVER_ANIMATION_SELECTOR_CLASS_INLINE_IMPLEMENTATIONS_H__
