#pragma once

#include "GuiComponent.h"
#include "resources/Font.h"
#include "Renderer.h"
#include "Window.h"
#include "components/TextComponent.h"
#include "components/ImageComponent.h"
#include "components/MenuComponent.h"
#include <sstream>
#include "Log.h"
#include "Locale.h"
#include "MenuThemeData.h"

using namespace boost::locale;


//Used to display a list of options.
//Can select one or multiple options.

// if !multiSelect
// * <- curEntry ->

// always
// * press a -> open full list

#define CHECKED_PATH ":/checkbox_checked.svg"
#define UNCHECKED_PATH ":/checkbox_unchecked.svg"

template<typename T>
class OptionListComponent : public GuiComponent
{
private:
	struct OptionListData
	{
		std::string name;
		T object;
		bool selected;
	};

	class OptionListPopup : public GuiComponent
	{
	private:
		MenuComponent mMenu;
		OptionListComponent<T>* mParent;

	public:
		OptionListPopup(Window* window, OptionListComponent<T>* parent, const std::string& title) : GuiComponent(window),
			mMenu(window, title.c_str()), mParent(parent)
		{
			auto menuTheme = MenuThemeData::getInstance()->getCurrentTheme();
			auto font = menuTheme->menuText.font;
			auto color = menuTheme->menuText.color;
			ComponentListRow row;

			// for select all/none
			std::vector<ImageComponent*> checkboxes;

			for (auto& e : mParent->mEntries)
			{
				row.elements.clear();
				row.addElement(std::make_shared<TextComponent>(mWindow, strToUpper(e.name), font, color), true);

				if(mParent->mMultiSelect)
				{
					// add checkbox
					auto checkbox = std::make_shared<ImageComponent>(mWindow);
					checkbox->setImage(e.selected ? CHECKED_PATH : UNCHECKED_PATH);
					checkbox->setResize(0, font->getLetterHeight());
					checkbox->setColor(color);
					row.addElement(checkbox, false);

					// input handler
					// update checkbox state & selected value
					row.makeAcceptInputHandler([this, &e, checkbox, color]
					{
						e.selected = !e.selected;
						checkbox->setImage(e.selected ? CHECKED_PATH : UNCHECKED_PATH);
						checkbox->setColor(color);
						mParent->onSelectedChanged();
					});

					// for select all/none
					checkboxes.push_back(checkbox.get());
				}else{
					// input handler for non-multiselect
					// update selected value and close
					row.makeAcceptInputHandler([this, &e]
					{
						mParent->mEntries.at(mParent->getSelectedId()).selected = false;
						e.selected = true;
						mParent->onSelectedChanged();
						delete this;
					});
				}

				// also set cursor to this row if we're not multi-select and this row is selected
				mMenu.addRow(row, (!mParent->mMultiSelect && e.selected));
			}

			mMenu.addButton(_("BACK"), "accept", [this] { delete this; });

			if(mParent->mMultiSelect)
			{
			  mMenu.addButton(_("SELECT ALL"), "select all", [this, checkboxes, color] {
					for (unsigned int i = 0; i < mParent->mEntries.size(); i++)
					{
						mParent->mEntries.at(i).selected = true;
						checkboxes.at(i)->setImage(CHECKED_PATH);
						checkboxes.at(i)->setColor(color);
					}
					mParent->onSelectedChanged();
				});

			  mMenu.addButton(_("SELECT NONE"), "select none", [this, checkboxes, color] {
					for (unsigned int i = 0; i < mParent->mEntries.size(); i++)
					{
						mParent->mEntries.at(i).selected = false;
						checkboxes.at(i)->setImage(UNCHECKED_PATH);
						checkboxes.at(i)->setColor(color);
					}
					mParent->onSelectedChanged();
				});
			}

			mMenu.setPosition((Renderer::getScreenWidth() - mMenu.getSize().x()) / 2, (Renderer::getScreenHeight() - mMenu.getSize().y()) / 2);
			addChild(&mMenu);
		}

		bool input(InputConfig* config, Input input) override
		{
			if(config->isMappedTo("a", input) && input.value != 0)
			{
				delete this;
				return true;
			}

			return GuiComponent::input(config, input);
		}

		std::vector<HelpPrompt> getHelpPrompts() override
		{
			auto prompts = mMenu.getHelpPrompts();
			prompts.push_back(HelpPrompt("a", _("BACK")));
			return prompts;
		}
	};

public:
	OptionListComponent(Window* window, const std::string& name, bool multiSelect = false, unsigned int font_size = FONT_SIZE_MEDIUM) : GuiComponent(window), mMultiSelect(multiSelect), mName(name),
		 mText(window), mLeftArrow(window), mRightArrow(window)
	{
		std::shared_ptr<Font> font = nullptr;
		unsigned int color;
		auto menuTheme = MenuThemeData::getInstance()->getCurrentTheme();
		if (font_size == FONT_SIZE_SMALL)
			font = menuTheme->menuTextSmall.font;
		else
			font = menuTheme->menuText.font;
		color = menuTheme->menuText.color;
		mOriginColor = color;
			
		mText.setFont(font);
		mText.setColor(color);
		mText.setHorizontalAlignment(ALIGN_CENTER);
		addChild(&mText);

		if(mMultiSelect)
		{
			mRightArrow.setImage(menuTheme->iconSet.arrow);
			mRightArrow.setColorShift(color);
			addChild(&mRightArrow);
		}else{
			mLeftArrow.setImage(menuTheme->iconSet.option_arrow);
			mLeftArrow.setColorShift(color);
			mLeftArrow.setFlipX(true);
			addChild(&mLeftArrow);

			mRightArrow.setImage(menuTheme->iconSet.option_arrow);
			mRightArrow.setColorShift(color);
			addChild(&mRightArrow);
		}

		setSize(mLeftArrow.getSize().x() + mRightArrow.getSize().x(), font->getHeight());
	}

	// handles positioning/resizing of text and arrows
	void onSizeChanged() override
	{
		mLeftArrow.setResize(0, mText.getFont()->getLetterHeight());
		mRightArrow.setResize(0, mText.getFont()->getLetterHeight());

        /*if(mSize.x() < (mLeftArrow.getSize().x() + mRightArrow.getSize().x())) {
			LOG(LogWarning) << "OptionListComponent too narrow!";
        }*/

		mText.setSize(mSize.x() - mLeftArrow.getSize().x() - mRightArrow.getSize().x(), mText.getFont()->getHeight());

		// position
		mLeftArrow.setPosition(0, (mSize.y() - mLeftArrow.getSize().y()) / 2);
		mText.setPosition(mLeftArrow.getPosition().x() + mLeftArrow.getSize().x(), (mSize.y() - mText.getSize().y()) / 2);
		mRightArrow.setPosition(mText.getPosition().x() + mText.getSize().x(), (mSize.y() - mRightArrow.getSize().y()) / 2);
	}

	bool input(InputConfig* config, Input input) override
	{
		if(input.value != 0)
		{
			if(config->isMappedTo("b", input))
			{
				open();
				return true;
			}
			if(!mMultiSelect)
			{
				if(config->isMappedTo("left", input))
				{
					// move selection to previous
					unsigned int i = getSelectedId();
					int next = (int)i - 1;
					if(next < 0)
						next += mEntries.size();

					mEntries.at(i).selected = false;
					mEntries.at(next).selected = true;
					onSelectedChanged();
					return true;

				}else if(config->isMappedTo("right", input))
				{
					// move selection to next
					unsigned int i = getSelectedId();
					int next = (i + 1) % mEntries.size();
					mEntries.at(i).selected = false;
					mEntries.at(next).selected = true;
					onSelectedChanged();
					return true;

				}
			}
		}
		return GuiComponent::input(config, input);
	}

	std::vector<T> getSelectedObjects()
	{
		std::vector<T> ret;
		for (auto& comp : mEntries)
		{
			if(comp.selected)
				ret.push_back(comp.object);
		}

		return ret;
	}

        
	T getSelected()
	{
		assert(mMultiSelect == false);
		auto selected = getSelectedObjects();
		if(selected.size() == 1){
                    return selected.at(0);
                }else {
                    return T();
                }
	}
        
	std::string getSelectedName()
	{
                assert(mMultiSelect == false);
		for (auto& entry : mEntries)
		{
			if(entry.selected)
				return entry.name;
		}
                return "";
	}

	void select(T object) {
	    bool hasChanged = false;
	    bool selectable = true;
		for (auto& entry : mEntries) {
		    const bool previous = entry.selected;
			entry.selected = selectable && (entry.object == object);
			if (entry.selected && !mMultiSelect) {
                selectable = false;
			}
            hasChanged = hasChanged || (entry.selected != previous);
		}
		if (hasChanged) {
            onSelectedChanged();
		}
	}
        
	void clear() { 
		mEntries.clear();
	}

	void add(const std::string& name, const T& obj, bool selected)
	{
		OptionListData e;
		e.name = name;
		e.object = obj;
		e.selected = selected;
		if (selected) {
			if (!mMultiSelect) {
				for (auto& entry : mEntries) {
					entry.selected = false;
				}
			}
			firstSelected = obj;
		}

		mEntries.push_back(e);
		onSelectedChanged();
	}

	inline void invalidate() {
		onSelectedChanged();
	}

	inline void setSelectedChangedCallback(const std::function<void(const T&)>& callback) {
		mSelectedChangedCallback = callback;
	}

	bool changed(){
		return firstSelected != getSelected();
	}


private:
	unsigned int getSelectedId()
	{
		assert(mMultiSelect == false);
		for (unsigned int i = 0; i < mEntries.size(); i++)
		{
			if(mEntries.at(i).selected)
				return i;
		}

		LOG(LogWarning) << "OptionListComponent::getSelectedId() - no selected element found, defaulting to 0";
		return 0;
	}

	void open()
	{
		mWindow->pushGui(new OptionListPopup(mWindow, this, mName));
	}

	void onSelectedChanged()
	{
		if(mMultiSelect)
		{
			// display # selected
		  	char strbuf[256];
			int x = getSelectedObjects().size();
		  	snprintf(strbuf, 256, ngettext("%i SELECTED", "%i SELECTED", x).c_str(), x);
			mText.setText(strbuf);
			mText.setSize(0, mText.getSize().y());
			setSize(mText.getSize().x() + mRightArrow.getSize().x() + 24, mText.getSize().y());
			if(mParent) // hack since theres no "on child size changed" callback atm...
				mParent->onSizeChanged();
		}else{
			// display currently selected + l/r cursors
			for (const auto& entry : mEntries)
			{
				if(entry.selected)
				{
					mText.setText(strToUpper(entry.name));
					mText.setSize(0, mText.getSize().y());
					setSize(mText.getSize().x() + mLeftArrow.getSize().x() + mRightArrow.getSize().x() + 24, mText.getSize().y());
					if(mParent) // hack since theres no "on child size changed" callback atm...
						mParent->onSizeChanged();
					break;
				}
			}
		}

		if (mSelectedChangedCallback) {
			mSelectedChangedCallback(mEntries.at(getSelectedId()).object);
		}
	}

	std::vector<HelpPrompt> getHelpPrompts() override
	{
		std::vector<HelpPrompt> prompts;
		if(!mMultiSelect)
		  prompts.push_back(HelpPrompt("left/right", _("CHANGE")));
		
		prompts.push_back(HelpPrompt("b", _("SELECT")));
		return prompts;
	}

	void setColor(unsigned int color)
	{
		mText.setColor(color);
		mRightArrow.setColorShift(color);
		mLeftArrow.setColorShift(color);
	}


	inline void setOriginColor(unsigned int color){mOriginColor = color;};
	inline unsigned int getOriginColor() override{return mOriginColor;};

	bool mMultiSelect;

	std::string mName;
	T firstSelected;
	TextComponent mText;
	ImageComponent mLeftArrow;
	ImageComponent mRightArrow;
	unsigned int mOriginColor;

	std::vector<OptionListData> mEntries;
	std::function<void(const T&)> mSelectedChangedCallback;
};
