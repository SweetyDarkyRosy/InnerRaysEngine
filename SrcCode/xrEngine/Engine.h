// Engine.h: interface for the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __INNER_RAYS_ENGINE_ENGINE_CLASS_H__
#define __INNER_RAYS_ENGINE_ENGINE_CLASS_H__
#pragma once

#include "engineAPI.h"
#include "eventAPI.h"
#include "../xrCPU_Pipe/xrCPU_Pipe.h"
#include "xrSheduler.h"


class ENGINE_API CEngine
{
	HMODULE hPSGP;

public:
	// Constructor
	CEngine();
	// Destructor
	~CEngine();

public:
	void Initialize();
	void Destroy();

public:
	CEngineAPI External;
	CEventAPI Event;
	CSheduler Sheduler;

};

ENGINE_API extern xrDispatchTable		PSGP;
ENGINE_API extern CEngine				Engine;

#endif // !__INNER_RAYS_ENGINE_ENGINE_CLASS_H__
