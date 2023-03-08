/*
	InnerRaysEngine game engine source code. Unlimited Snow Independent Studio. 2023.

	This file is part of the xrEngine module.

	@Name:			SSoundProcessor.h
	@Created:		08.03.2023
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)

	Sound processor structure. */

#ifndef __INNER_RAYS_ENGINE_SOUND_PROCESSOR_STRUCTURE_H__
#define __INNER_RAYS_ENGINE_SOUND_PROCESSOR_STRUCTURE_H__

#include "device.h"


// Sound processor structure
struct SSoundProcessor : public pureFrame
{
	virtual void OnFrame()
	{
		Device.Statistic->Sound.Begin();
		::Sound->update(Device.vCameraPosition, Device.vCameraDirection, Device.vCameraTop);
		Device.Statistic->Sound.End();
	}

};

#endif // !__INNER_RAYS_ENGINE_SOUND_PROCESSOR_STRUCTURE_H__
