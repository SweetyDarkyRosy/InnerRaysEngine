////////////////////////////////////////////////////////////////////////////
//	Module 		: smart_cover_default_behaviour_planner_inline.hpp
//	Created 	: 15.11.2007
//	Author		: Alexander Dudin
//	Description : Default behaviour planner for target selector inline functions
////////////////////////////////////////////////////////////////////////////

#ifndef __INNER_RAYS_ENGINE_SMART_COVER_DEFAULT_BEHAVIOUR_PLANNER_CLASS_INLINE_IMPLEMENTATIONS_HPP__
#define __INNER_RAYS_ENGINE_SMART_COVER_DEFAULT_BEHAVIOUR_PLANNER_CLASS_INLINE_IMPLEMENTATIONS_HPP__

namespace smart_cover
{
	IC u32 const& default_behaviour_planner::idle_time() const
	{
		return m_idle_time;
	}

	IC void default_behaviour_planner::idle_time(u32 const& value)
	{
		m_idle_time = value;
	}

	IC u32 const& default_behaviour_planner::lookout_time() const
	{
		return m_lookout_time;
	}

	IC void default_behaviour_planner::lookout_time(u32 const& value)
	{
		m_lookout_time = value;
	}

} // !namespace smart_cover

#endif // !__INNER_RAYS_ENGINE_SMART_COVER_DEFAULT_BEHAVIOUR_PLANNER_CLASS_INLINE_IMPLEMENTATIONS_HPP__
