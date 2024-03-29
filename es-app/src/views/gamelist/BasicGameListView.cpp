#include <RecalboxConf.h>
#include "components/TextListComponent.h"
#include "components/IList.h"
#include "views/gamelist/BasicGameListView.h"
#include "views/ViewController.h"
#include "Renderer.h"
#include "Window.h"
#include "ThemeData.h"
#include "SystemData.h"
#include "FileSorts.h"
#include "Settings.h"
#include "Locale.h"
#include <boost/assign.hpp>
#include <recalbox/RecalboxSystem.h>

static const std::map<std::string, const char*> favorites_icons_map = boost::assign::map_list_of
        ("snes"        , "\uF25e ")
        ("3do"         , "\uF28a ")
        ("x68000"      , "\uF28b ")
        ("amiga600"    , "\uF244 ")
        ("amiga1200"   , "\uF245 ")
        ("nds"         , "\uF267 ")
        ("c64"         , "\uF24c ")
        ("nes"         , "\uF25c ")
        ("n64"         , "\uF260 ")
        ("gba"         , "\uF266 ")
        ("gbc"         , "\uF265 ")
        ("gb"          , "\uF264 ")
        ("fds"         , "\uF25d ")
        ("virtualboy"  , "\uF25f ")
        ("gw"          , "\uF278 ")
        ("dreamcast"   , "\uF26e ")
        ("megadrive"   , "\uF26b ")
        ("segacd"      , "\uF26d ")
        ("sega32x"     , "\uF26c ")
        ("mastersystem", "\uF26a ")
        ("gamegear"    , "\uF26f ")
        ("sg1000"      , "\uF269 ")
        ("psp"         , "\uF274 ")
        ("psx"         , "\uF275 ")
        ("pcengine"    , "\uF271 ")
        ("pcenginecd"  , "\uF273 ")
        ("supergrafx"  , "\uF272 ")
        ("scummvm"     , "\uF27a ")
        ("dos"         , "\uF24a ")
        ("fba"         , "\uF252 ")
        ("fba_libretro", "\uF253 ")
        ("mame"        , "\uF255 ")
        ("neogeo"      , "\uF257 ")
        ("colecovision", "\uF23f ")
        ("atari2600"   , "\uF23c ")
        ("atari7800"   , "\uF23e ")
        ("lynx"        , "\uF270 ")
        ("ngp"         , "\uF258 ")
        ("ngpc"        , "\uF259 ")
        ("wswan"       , "\uF25a ")
        ("wswanc"      , "\uF25b ")
        ("prboom"      , "\uF277 ")
        ("vectrex"     , "\uF240 ")
        ("lutro"       , "\uF27d ")
        ("cavestory"   , "\uF276 ")
        ("atarist"     , "\uF248 ")
        ("amstradcpc"  , "\uF246 ")
        ("msx"         , "\uF24d ")
        ("msx1"        , "\uF24e ")
        ("msx2"        , "\uF24f ")
        ("odyssey2"    , "\uF241 ")
        ("zx81"        , "\uF250 ")
        ("zxspectrum"  , "\uF251 ")
        ("moonlight"   , "\uF27e ")
        ("apple2"      , "\uF247 ")
        ("gamecube"    , "\uF262 ")
        ("wii"         , "\uF263 ")
        ("imageviewer" , "\uF27b ");

BasicGameListView::BasicGameListView(Window* window, FolderData* root)
	: ISimpleGameListView(window, root),
	  mList(window),
    mEmptyListItem(root->getSystem()),
	  listingOffset(0)
{
	mList.setSize(mSize.x(), mSize.y() * 0.8f);
	mList.setPosition(0, mSize.y() * 0.2f);
	mList.setDefaultZIndex(20);
	addChild(&mList);

	mEmptyListItem.Metadata().SetName(_("EMPTY LIST"));

	populateList(root);
}

void BasicGameListView::onThemeChanged(const std::shared_ptr<ThemeData>& theme)
{
	ISimpleGameListView::onThemeChanged(theme);
	using namespace ThemeFlags;
	mList.applyTheme(theme, getName(), "gamelist", ALL);
	sortChildren();
}

void BasicGameListView::onFileChanged(FileData* file, FileChangeType change)
{
	ISimpleGameListView::onFileChanged(file, change);

	if(change == FileChangeType::MetadataChanged)
	{
		// might switch to a detailed view
		ViewController::get()->reloadGameListView(this);
		return;
	}
}

const char * BasicGameListView::getItemIcon(FileData* item)
{
	if (item->Metadata().Hidden()) return "\uF070 ";
	if ((item->isGame()) && (item->Metadata().Favorite()))
	{
		auto icon = favorites_icons_map.find(item->getSystem()->getName());
		return (icon != favorites_icons_map.end()) ? (*icon).second : "\uF006 ";
	}

	return nullptr;
}

void BasicGameListView::populateList(const FolderData* folder)
{
	mPopulatedFolder = folder;
	mList.clear();
	mHeaderText.setText(mSystem->getFullName());

  // Default filter
  FileData::Filter filter = FileData::Filter::Normal | FileData::Filter::Favorite;
  // Add hidden?
  if (Settings::getInstance()->getBool("ShowHidden"))
    filter |= FileData::Filter::Hidden;
  // Favorites only?
  if (mFavoritesOnly)
    filter = FileData::Filter::Favorite;

  // Get items
  bool flatfolders = (RecalboxConf::getInstance()->getBool(getRoot()->getSystem()->getName() + ".flatfolder"));
  int count = flatfolders ? folder->countFilteredItemsRecursively(filter, false) : folder->countFilteredItems(filter, true);
  if (count == 0) filter |= FileData::Filter::Normal;
	FileData::List items = flatfolders ? folder->getFilteredItemsRecursively(filter, false) : folder->getFilteredItems(filter, true);
	// Check emptyness
	if (items.empty())
  {
	  // Insert "EMPTY SYSTEM" item
    items.push_back(&mEmptyListItem);
  }

  // Sort
  const FileSorts::SortType& sortType = mSystem->getSortType(mSystem->isFavorite());
  FolderData::Sort(items, sortType.comparisonFunction, sortType.ascending);

  // Add to list
  //mList.reserve(items.size()); // TODO: Reserve memory once
  for (FileData* fd : items)
	{
  	// Get name
  	std::string name = fd->getName();
  	// Select fron icon
    const char* icon = getItemIcon(fd);
    if (icon != nullptr)
    	name = icon + name;
    // Store
		mList.add(name, fd, fd->isFolder() ? 1 : 0, false);
	}
}

void BasicGameListView::refreshList()
{
    populateList(mPopulatedFolder);
}

FileData::List BasicGameListView::getFileDataList()
{
	FileData::List objects = mList.getObjects();
	FileData::List slice;
  for (auto it = objects.begin() + listingOffset; it != objects.end(); it++)
  {
    slice.push_back(*it);
  }
  return slice;
}

FileData* BasicGameListView::getCursor() {
	return mList.getSelected();
}

void BasicGameListView::setCursorIndex(int index)
{
  if (index >= mList.size()) index = mList.size() - 1;
  if (index < 0) index = 0;

  RecalboxSystem::getInstance()->NotifyGame(*getCursor(), false, false);
	mList.setCursorIndex(index);
}

int BasicGameListView::getCursorIndex()
{
  return mList.getCursorIndex();
}

int BasicGameListView::getCursorIndexMax(){
	return mList.size() - 1;
}

void BasicGameListView::setCursor(FileData* cursor)
{
	if(!mList.setCursor(cursor, listingOffset))
	{
		populateList(mRoot);
		mList.setCursor(cursor);

		// update our cursor stack in case our cursor just got set to some folder we weren't in before
		if(mCursorStack.empty() || mCursorStack.top() != cursor->getParent())
		{
			std::stack<FolderData*> tmp;
			FolderData* ptr = cursor->getParent();
			while(ptr && ptr != mRoot)
			{
				tmp.push(ptr);
				ptr = ptr->getParent();
			}
			
			// flip the stack and put it in mCursorStack
			mCursorStack = std::stack<FolderData*>();
			while(!tmp.empty())
			{
				mCursorStack.push(tmp.top());
				tmp.pop();
			}
		}
	}
  RecalboxSystem::getInstance()->NotifyGame(*getCursor(), false, false);
}

void BasicGameListView::launch(FileData* game) {
	ViewController::get()->launch(game);
}