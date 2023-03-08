/*
	InnerRaysEngine game engine source code. Unlimited Snow Independent Studio. 2023.

	This file is part of the xrEngine module.

	@Name:			CApplication.h
	@Created:		08.03.2023
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)

	Game engine application class. */

#ifndef __INNER_RAYS_ENGINE_APPLICATION_CLASS_H__
#define __INNER_RAYS_ENGINE_APPLICATION_CLASS_H__

#include "../Include/xrRender/FactoryPtr.h"
#include "../Include/xrRender/ApplicationRender.h"


// References
class ENGINE_API CGameFont;

class ENGINE_API CApplication :
	public pureFrame,
	public IEventReceiver
{
public:
	// Constructors
	CApplication();
	// Destructors
	~CApplication();

public:
	void Level_Scan();
	int Level_ID(LPCSTR name, LPCSTR ver, bool bSet);
	void Level_Set(u32 ID);
	void LoadAllArchives();
	CInifile* GetArchiveHeader(LPCSTR name, LPCSTR ver);

	// Loading
	void LoadBegin();
	void LoadEnd();
	void LoadTitleInt(LPCSTR str);
	void LoadSwitch();
	void LoadDraw();

	virtual void OnEvent(EVENT E, u64 P1, u64 P2);

	virtual void OnFrame();
	void load_draw_internal();
	void destroy_loading_shaders();

private:
	friend class dxApplicationRender;

	// Levels
	struct sLevelInfo
	{
		char* folder;
		char* name;
	};

	string256 app_title;

	FactoryPtr<IApplicationRender> m_pRender;

	int max_load_stage;
	int load_stage;
	u32 ll_dwReference;

	EVENT eQuit;
	EVENT eStart;
	EVENT eStartLoad;
	EVENT eDisconnect;
	EVENT eConsole;

	void Level_Append(LPCSTR lname);

public:
	CGameFont* pFontSystem;

	// Levels
	xr_vector<sLevelInfo> Levels;
	u32 Level_Current;

};

extern ENGINE_API CApplication*		pApp;

#endif // !__INNER_RAYS_ENGINE_APPLICATION_CLASS_H__
