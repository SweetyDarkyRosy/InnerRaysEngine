#include "stdafx.h"
#include "render.h"


//ENGINE_API IRender_interface*		Render = NULL;


// Destructor
IRender_interface::~IRender_interface()
{};


/*[
	Resources
 ]*/

IRender_Light::~IRender_Light()
{
	::Render->light_destroy(this);
}

IRender_Glow::~IRender_Glow()
{
	::Render->glow_destroy(this);
}
