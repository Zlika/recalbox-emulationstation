#pragma once

#include "GuiComponent.h"
#include "components/ImageComponent.h"

class TextCache;
class Font;

// Used to display/edit a value between some min and max values.
class SliderComponent : public GuiComponent
{
public:
	//Minimum value (far left of the slider), maximum value (far right of the slider), increment size (how much just pressing L/R moves by), unit to display (optional).
	SliderComponent(Window* window, float min, float max, float increment, const std::string& suffix = "");

	void setValue(float val);
	float getValue();
	void setColor(unsigned int color);
	inline void setOriginColor(unsigned int color){mOriginColor = color;};
	inline unsigned int getOriginColor() override{return mOriginColor;};

	bool input(InputConfig* config, Input input) override;
	void update(int deltaTime) override;
	void render(const Transform4x4f& parentTrans) override;
	
	void onSizeChanged() override;
	
	std::vector<HelpPrompt> getHelpPrompts() override;

	inline void setSelectedChangedCallback(const std::function<void(const float&)>& callback) {
		mSelectedChangedCallback = callback;
	}

private:
	void onValueChanged();

	float mMin, mMax;
	float mValue;
	float mSingleIncrement;
	float mMoveRate;
	int mMoveAccumulator;
	unsigned int mColor;
	unsigned int mOriginColor;

	ImageComponent mKnob;

	std::string mSuffix;
	std::shared_ptr<Font> mFont;
	std::shared_ptr<TextCache> mValueCache;

	std::function<void(const float&)> mSelectedChangedCallback;
};
