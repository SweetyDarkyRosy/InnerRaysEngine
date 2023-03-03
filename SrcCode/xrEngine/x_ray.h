#ifndef __X_RAY_H__
#define __X_RAY_H__

// References
class ENGINE_API CGameFont;

#include "../Include/xrRender/FactoryPtr.h"
#include "../Include/xrRender/ApplicationRender.h"


class ENGINE_API CApplication :
	public pureFrame,
	public IEventReceiver
{
public:
	CApplication();
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

#endif // !__X_RAY_H__
