// EngineAPI.h: interface for the CEngineAPI class.
//
//****************************************************************************
// Support for extension DLLs
//****************************************************************************

#ifndef __INNER_RAYS_ENGINE_ENGINE_API_CLASS_H__
#define __INNER_RAYS_ENGINE_ENGINE_API_CLASS_H__
#pragma once

// Abstract 'Pure' class for DLL interface
class ENGINE_API DLL_Pure
{
public:
	CLASS_ID CLS_ID;

	DLL_Pure(void* params) { CLS_ID = 0; };
	DLL_Pure() { CLS_ID = 0; };
	virtual	DLL_Pure* _construct() { return this; }
	virtual ~DLL_Pure() {};

};

// Class creation/destroying interface
extern "C"
{
	typedef DLL_API DLL_Pure* __cdecl Factory_Create(CLASS_ID CLS_ID);
	typedef DLL_API void __cdecl Factory_Destroy(DLL_Pure* O);
};

// Tuning interface
extern "C"
{
	typedef void __cdecl VTPause(void);
	typedef void __cdecl VTResume(void);
};

class ENGINE_API CEngineAPI
{
public:
	// Constructor
	CEngineAPI();
	// Destructor
	~CEngineAPI();

public:
	Factory_Create* pCreate;
	Factory_Destroy* pDestroy;
	BOOL tune_enabled;
	VTPause* tune_pause;
	VTResume* tune_resume;
	void Initialize();
	void Destroy();

	void CreateRendererList();

private:
	HMODULE hGame;
	HMODULE hRender;
	HMODULE hTuner;

};

#define NEW_INSTANCE(a)		Engine.External.pCreate(a)
#define DEL_INSTANCE(a)		{ Engine.External.pDestroy(a); a = NULL; }

#endif // !__INNER_RAYS_ENGINE_ENGINE_API_CLASS_H__
