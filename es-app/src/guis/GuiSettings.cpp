#include "guis/GuiSettings.h"
#include "Window.h"
#include "Settings.h"
#include "views/ViewController.h"
#include "Locale.h"

GuiSettings::GuiSettings(Window* window, const char* title) : GuiComponent(window), mMenu(window, title)
{
	addChild(&mMenu);

	mMenu.addButton(_("CLOSE"), _("CLOSE"), [this] { delete this; });

	setSize((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
}

GuiSettings::~GuiSettings()
{
	if(doSave) save();
}

void GuiSettings::save()
{
	if(!mSaveFuncs.size())
		return;

	for (auto& saveFunc : mSaveFuncs)
		saveFunc();

	Settings::getInstance()->saveFile();
}

bool GuiSettings::input(InputConfig* config, Input input)
{
	if(config->isMappedTo("a", input) && input.value != 0)
	{
		delete this;
		return true;
	}

	if(config->isMappedTo("start", input) && input.value != 0)
	{
		// close everything
		Window* window = mWindow;
		while(window->peekGui() && window->peekGui() != ViewController::get())
			delete window->peekGui();
		return true;
	}
	
	return GuiComponent::input(config, input);
}

std::vector<HelpPrompt> GuiSettings::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();

	prompts.push_back(HelpPrompt("a", _("BACK")));
	prompts.push_back(HelpPrompt("start", _("CLOSE")));

	return prompts;
}
