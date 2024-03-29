#include "guis/GuiScraperStart.h"
#include "guis/GuiScraperMulti.h"
#include "guis/GuiMsgBox.h"
#include "views/ViewController.h"

#include "components/TextComponent.h"
#include "components/OptionListComponent.h"
#include "components/SwitchComponent.h"
#include "Locale.h"
#include "Settings.h"


GuiScraperStart::GuiScraperStart(Window* window) : GuiComponent(window),
  mMenu(window, _("SCRAPE NOW").c_str())
{
	addChild(&mMenu);

	// add filters (with first one selected)
	mFilters = std::make_shared< OptionListComponent<GameFilterFunc> >(mWindow, _("SCRAPE THESE GAMES"), false);
	mFilters->add(_("All Games"), 
		[](SystemData*, FileData*) -> bool { return true; }, false);
	mFilters->add(_("Only missing image"), 
		[](SystemData*, FileData* g) -> bool { return g->Metadata().Image().empty(); }, true);
	mMenu.addWithLabel(mFilters, _("FILTER"));

	// add systems (all with a platformid specified selected)
	mSystems = std::make_shared< OptionListComponent<SystemData*> >(mWindow, _("SCRAPE THESE SYSTEMS"), true);
	for (auto& it : SystemData::sSystemVector)
	{
		if(!it->hasPlatformId(PlatformIds::PlatformId::PLATFORM_IGNORE))
			mSystems->add(it->getFullName(), it, !it->getPlatformIds().empty());
	}
	mMenu.addWithLabel(mSystems, _("SYSTEMS"));

	// add mix images option (if scraper = screenscraper)
	std::string scraperName = Settings::getInstance()->getString("Scraper");

	if(scraperName == "Screenscraper") {
		mMixImages = std::make_shared<SwitchComponent>(mWindow);
		mMixImages->setState(true);
		mMenu.addWithLabel(mMixImages, _("USE COMPOSED VISUALS"));
	}

	mApproveResults = std::make_shared<SwitchComponent>(mWindow);
	mApproveResults->setState(false);
	mMenu.addWithLabel(mApproveResults, _("USER DECIDES ON CONFLICTS"));

	mMenu.addButton(_("START"), "start", std::bind(&GuiScraperStart::pressedStart, this));
	mMenu.addButton(_("BACK"), "back", [&] { delete this; });

	mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
}

void GuiScraperStart::pressedStart()
{
	std::vector<SystemData*> sys = mSystems->getSelectedObjects();
	for (auto& sy : sys)
	{
		if(sy->getPlatformIds().empty())
		{
			mWindow->pushGui(new GuiMsgBox(mWindow, 
				_("WARNING: SOME OF YOUR SELECTED SYSTEMS DO NOT HAVE A PLATFORM SET. RESULTS MAY BE EVEN MORE INACCURATE THAN USUAL!\nCONTINUE ANYWAY?"), 
						       _("YES"), std::bind(&GuiScraperStart::start, this), 
						       _("NO"), nullptr));
			return;
		}
	}

	start();
}

void GuiScraperStart::start()
{
	std::queue<ScraperSearchParams> searches = getSearches(mSystems->getSelectedObjects(), mFilters->getSelected());
	if(Settings::getInstance()->getString("Scraper") == "Screenscraper") {
		Settings::getInstance()->setBool("MixImages", mMixImages->getState());
	}

	if(searches.empty())
	{
		mWindow->pushGui(new GuiMsgBox(mWindow,
					       _("NO GAMES FIT THAT CRITERIA.")));
	}else{
		GuiScraperMulti* gsm = new GuiScraperMulti(mWindow, searches, mApproveResults->getState());
		mWindow->pushGui(gsm);
		delete this;
	}
}

std::queue<ScraperSearchParams> GuiScraperStart::getSearches(std::vector<SystemData*> systems, GameFilterFunc selector)
{
	std::queue<ScraperSearchParams> queue;
	for (auto& system : systems)
	{
		FileData::List games = system->getRootFolder()->getAllItemsRecursively(false);
		for (auto& game : games)
		{
			if(selector(system, game))
			{
				ScraperSearchParams search;
				search.game = game;
				search.system = system;
				
				queue.push(search);
			}
		}
	}

	return queue;
}

bool GuiScraperStart::input(InputConfig* config, Input input)
{
	bool consumed = GuiComponent::input(config, input);
	if(consumed)
		return true;
	
	if(input.value != 0 && config->isMappedTo("a", input))
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
	}


	return false;
}

std::vector<HelpPrompt> GuiScraperStart::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts = mMenu.getHelpPrompts();
	prompts.push_back(HelpPrompt("a", _("BACK")));
	prompts.push_back(HelpPrompt("start", _("CLOSE")));
	return prompts;
}
