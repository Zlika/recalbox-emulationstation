#pragma once

#include "views/gamelist/IGameListView.h"

#include "components/TextComponent.h"
#include "components/ImageComponent.h"

class ISimpleGameListView : public IGameListView
{
public:
	ISimpleGameListView(Window* window, FolderData* root);
	virtual ~ISimpleGameListView() {}

	// Called when a new file is added, a file is removed, a file's metadata changes, or when file sort changed
	virtual void onFileChanged(FileData* file, FileChangeType change);

	// Called whenever the theme changes.
	virtual void onThemeChanged(const std::shared_ptr<ThemeData>& theme);

	virtual FileData* getCursor() = 0;
	virtual int getCursorIndex() = 0;
	virtual int getCursorIndexMax() = 0;
	virtual void setCursor(FileData*) = 0;
	virtual void setCursorIndex(int) = 0;

	virtual bool input(InputConfig* config, Input input) override;

	virtual inline void updateInfoPanel() override {}

	virtual inline void populateList(const FolderData* folder) override { (void)folder; }
	virtual inline void refreshList() override {};

	virtual std::vector<HelpPrompt> getHelpPrompts() override;
	virtual std::vector<std::string> getAvailableLetters() override;
	virtual void jumpToLetter(char letter) override;

	void jumpToNextLetter(int increment);

protected:
	virtual void launch(FileData* game) = 0;

	virtual FileData* getEmptyListItem() = 0;

	TextComponent mHeaderText;
	ImageComponent mHeaderImage;
	ImageComponent mBackground;

	ThemeExtras mThemeExtras;

	std::stack<FolderData*> mCursorStack;
	bool mFavoritesOnly;

private:
  bool mIsFavoriteSystem;
  int mFavoritesCount;
};
