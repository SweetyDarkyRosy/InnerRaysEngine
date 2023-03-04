////////////////////////////////////////////////////////////////////////////
//	Module 		: space_restriction_manager_inline.h
//	Created 	: 17.08.2004
//  Modified 	: 27.08.2004
//	Author		: Dmitriy Iassenev
//	Description : Space restriction manager inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "space_restriction.h"


#ifdef DEBUG
IC	const CSpaceRestrictionManager::SPACE_RESTRICTIONS &CSpaceRestrictionManager::restrictions	() const
{
	return								(m_space_restrictions);
}
#endif

IC CSpaceRestrictionManager::CRestrictionPtr CSpaceRestrictionManager::restriction(ALife::_OBJECT_ID id)
{
	CLIENT_RESTRICTIONS::iterator I = m_clients->find(id);
	VERIFY(m_clients->end() != I);
	return (*I).second.m_restriction;
}

template <typename T1, typename T2>
IC	void CSpaceRestrictionManager::add_border					(ALife::_OBJECT_ID id, T1 p1, T2 p2)
{
	CRestrictionPtr						client_restriction = restriction(id);
	if (client_restriction)
		client_restriction->add_border	(p1,p2);
}

IC bool CSpaceRestrictionManager::restriction_presented(shared_str restrictions, shared_str restriction) const
{
	string4096 m_temp;
	for (u32 i = 0, n = _GetItemCount(*restrictions); i < n; ++i)
	{
		if (!xr_strcmp(restriction, _GetItem(*restrictions, i, m_temp)))
		{
			return true;
		}
	}

	return false;
}
