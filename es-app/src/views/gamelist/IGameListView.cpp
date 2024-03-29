#include "views/gamelist/IGameListView.h"
#include "Window.h"
#include "guis/GuiMetaDataEd.h"
#include "guis/GuiMenu.h"
#include "views/ViewController.h"
#include "Settings.h"
#include "Log.h"
#include "Sound.h"

bool IGameListView::input(InputConfig* config, Input input)
{
	if(Settings::getInstance()->getBool("Debug") && config->getDeviceId() == DEVICE_KEYBOARD &&
		(SDL_GetModState() & (KMOD_LCTRL | KMOD_RCTRL)) && input.id == SDLK_r && input.value != 0)
	{
		LOG(LogDebug) << "reloading view";
		ViewController::get()->reloadGameListView(this, true);
		return true;
	}

	return GuiComponent::input(config, input);
}

void IGameListView::setTheme(const std::shared_ptr<ThemeData>& theme)
{
	mTheme = theme;
	onThemeChanged(theme);
}

HelpStyle IGameListView::getHelpStyle()
{
	HelpStyle style;
	style.applyTheme(mTheme, getName());
	return style;
}
