#include "ScraperCmdLine.h"
#include <iostream>
#include <vector>
#include "SystemData.h"
#include "Settings.h"
#include <signal.h>
#include "Log.h"

std::ostream& out = std::cout;

void handle_interrupt_signal(int p)
{
	(void)p;

	sleep(50);

	LOG(LogInfo) << "Interrupt received during scrape...";

	SystemData::deleteSystems();

	exit(1);
}

int run_scraper_cmdline()
{
	out << "EmulationStation scraper\n";
	out << "========================\n";
	out << "\n";

	signal(SIGINT, handle_interrupt_signal);

	//==================================================================================
	//filter
	//==================================================================================
	enum FilterChoice
	{
		FILTER_MISSING_IMAGES,
		FILTER_ALL
	};

	int filter_choice;
	do {
		out << "Select filter for games to be scraped:\n";
		out << FILTER_MISSING_IMAGES << " - games missing images\n";
		out << FILTER_ALL << " - all games period, can overwrite existing metadata\n";

		std::cin >> filter_choice;
		std::cin.ignore(1, '\n'); //skip the unconsumed newline
	} while(filter_choice < FILTER_MISSING_IMAGES || filter_choice > FILTER_ALL);

	out << "\n";

	//==================================================================================
	//platforms
	//==================================================================================

	std::vector<SystemData*> systems;

	out << "You can scrape only specific platforms, or scrape all of them.\n";
	out << "Would you like to scrape all platforms? (y/n)\n";

	std::string system_choice;
	std::getline(std::cin, system_choice);
	
	if(system_choice == "y" || system_choice == "Y")
	{
		out << "Will scrape all platforms.\n";
		for (auto& i : SystemData::sSystemVector)
		{
			out << "   " << i->getName() << " (" << i->getGameCount() << " games)\n";
			systems.push_back(i);
		}

	}else{
		std::string sys_name;

		out << "Enter the names of the platforms you would like to scrape, one at a time.\n";
		out << "Type nothing and press enter when you are ready to continue.\n";

		do {
			for (auto& i : SystemData::sSystemVector)
			{
				if(std::find(systems.begin(), systems.end(), i) != systems.end())
					out << " C ";
				else
					out << "   ";

				out << "\"" << i->getName() << "\" (" << i->getGameCount() << " games)\n";
			}

			std::getline(std::cin, sys_name);
			
			if(sys_name.empty())
				break;

			bool found = false;
			for (auto& i : SystemData::sSystemVector)
			{
				if(i->getName() == sys_name)
				{
					systems.push_back(i);
					found = true;
					break;
				}
			}

			if(!found)
				out << "System not found.\n";

		} while(true);
	}

	//==================================================================================
	//manual mode
	//==================================================================================

	out << "\n";
	out << "You can let the scraper try to automatically choose the best result, or\n";
	out << "you can manually approve each result. This \"manual mode\" is much more accurate.\n";
	out << "It is highly recommended you use manual mode unless you have a very large collection.\n";
	out << "Scrape in manual mode? (y/n)\n";

	std::string manual_mode_str;
	std::getline(std::cin, manual_mode_str);

	//bool manual_mode = false;

	if(manual_mode_str == "y" || manual_mode_str == "Y")
	{
		//manual_mode = true;
		out << "Scraping in manual mode!\n";
	}else{
		out << "Scraping in automatic mode!\n";
	}

	//==================================================================================
	//scraping
	//==================================================================================
	out << "\n";
	out << "Alright, let's do this thing!\n";
	out << "=============================\n";

	/*
	std::shared_ptr<Scraper> scraper = Settings::getInstance()->getScraper();
	for(auto sysIt = systems.begin(); sysIt != systems.end(); sysIt++)
	{
		FileData::List files = (*sysIt)->getRootFolder()->getFilesRecursive(GAME);

		ScraperSearchParams params;
		params.system = (*sysIt);

		for(auto gameIt = files.begin(); gameIt != files.end(); gameIt++)
		{
			params.nameOverride = "";
			params.game = *gameIt;

			//print original search term
			out << getCleanFileName(params.game->getPath()) << "...\n";

			//need to take into account filter_choice
			if(filter_choice == FILTER_MISSING_IMAGES)
			{
				if(!params.game->metadata.get("image").empty()) //maybe should also check if the image file exists/is a URL
				{
					out << "   Skipping, metadata \"image\" entry is not empty.\n";
					continue;
				}
			}

			//actually get some results
			do {
				std::vector<MetaDataList> mdls = scraper->getResults(params);

				//no results
				if(mdls.size() == 0)
				{
					if(manual_mode)
					{
						//in manual mode let the user enter a custom search
						out << "   NO RESULTS FOUND! Enter a new name to search for, or nothing to skip.\n";

						std::getline(std::cin, params.nameOverride);

						if(params.nameOverride.empty())
						{
							out << "   Skipping...\n";
							break;
						}

						continue;

					}else{
						out << "   NO RESULTS FOUND! Skipping...\n";
						break;
					}
				}

				//some results
				if(manual_mode)
				{
					//print list of choices
					for(unsigned int i = 0; i < mdls.size(); i++)
					{
						out << "   " << i << " - " << mdls.at(i).get("name") << "\n";
					}

					int choice = -1;
					std::string choice_str;
					
					out << "Your choice: ";

					std::getline(std::cin, choice_str);
					std::stringstream choice_buff(choice_str); //convert to int
					choice_buff >> choice;

					if(choice >= 0 && choice < (int)mdls.size())
					{
						params.game->metadata = mdls.at(choice);
						break;
					}else{
						out << "Invalid choice.\n";
						continue;
					}

				}else{
					//automatic mode
					//always choose the first choice
					out << "   name -> " << mdls.at(0).get("name") << "\n";
					params.game->metadata = mdls.at(0);
					break;
				}

			} while(true);

			out << "===================\n";
		}
	}

	out << "\n\n";
	out << "Downloading boxart...\n";

	for(auto sysIt = systems.begin(); sysIt != systems.end(); sysIt++)
	{
		FileData::List files = (*sysIt)->getRootFolder()->getAllItemsRecursively(GAME);

		for(auto gameIt = files.begin(); gameIt != files.end(); gameIt++)
		{
			FileData* game = *gameIt;
			const std::vector<MetaDataDecl>& mdd = game->metadata.getMDD();
			for(auto i = mdd.begin(); i != mdd.end(); i++)
			{
				std::string key = i->key;
				std::string url = game->metadata.get(key);

				if(i->type == MD_IMAGE_PATH && HttpReq::isUrl(url))
				{
					std::string urlShort = url.substr(0, url.length() > 35 ? 35 : url.length());
					if(url.length() != urlShort.length()) urlShort += "...";

					out << "   " << game->metadata.get("name") << " [from: " << urlShort << "]...\n";

					ScraperSearchParams p;
					p.game = game;
					p.system = *sysIt;
					game->metadata.set(key, downloadImage(url, getSaveAsPath(p, key, url)));
					if(game->metadata.get(key).empty())
					{
						out << "     FAILED! Skipping.\n";
						game->metadata.set(key, url); //result URL to what it was if download failed, retry some other time
					}
				}
			}
		}
	}


	out << "\n\n";
	out << "==============================\n";
	out << "SCRAPE COMPLETE!\n";
	out << "==============================\n";
	*/

	out << "\n\n";
	out << "ACTUALLY THIS IS STILL TODO\n";

	return 0;
}
