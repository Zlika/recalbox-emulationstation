#pragma once

#include "GuiComponent.h"

namespace GridFlags
{
	enum UpdateType
	{
		UPDATE_ALWAYS,
		UPDATE_WHEN_SELECTED,
		UPDATE_NEVER
	};

	enum Border : unsigned int
	{
		BORDER_NONE = 0,

		BORDER_TOP = 1,
		BORDER_BOTTOM = 2,
		BORDER_LEFT = 4,
		BORDER_RIGHT = 8
	};
};

// Used to arrange a bunch of components in a spreadsheet-esque grid.
class ComponentGrid : public GuiComponent
{
public:
	ComponentGrid(Window* window, const Vector2i& gridDimensions);
	~ComponentGrid() override;

	bool removeEntry(const std::shared_ptr<GuiComponent>& comp);

	void setEntry(const std::shared_ptr<GuiComponent>& comp, const Vector2i& pos, bool canFocus, bool resize = true,
		const Vector2i& size = Vector2i(1, 1), unsigned int border = GridFlags::BORDER_NONE, GridFlags::UpdateType updateType = GridFlags::UPDATE_ALWAYS);

	void textInput(const char* text) override;
	bool input(InputConfig* config, Input input) override;
	void update(int deltaTime) override;
	void render(const Transform4x4f& parentTrans) override;
	void onSizeChanged() override;

	void resetCursor();
	bool cursorValid();

	float getColWidth(int col);
	float getRowHeight(int row);

	void setColWidthPerc(int col, float width, bool update = true); // if update is false, will not call an onSizeChanged() which triggers a (potentially costly) repositioning + resizing of every element
	void setRowHeightPerc(int row, float height, bool update = true); // if update is false, will not call an onSizeChanged() which triggers a (potentially costly) repositioning + resizing of every element

	bool moveCursor(Vector2i dir);
	void setCursorTo(const std::shared_ptr<GuiComponent>& comp);

	inline void setUnhandledInputCallback(const std::function<bool(InputConfig* config, Input input)>& func) { mUnhandledInputCallback = func; }

	inline std::shared_ptr<GuiComponent> getSelectedComponent()
	{
		GridEntry* e = getCellAt(mCursor);
		if(e)
			return e->component;
		else
			return nullptr;
	}

	void onFocusLost() override;
	void onFocusGained() override;

	std::vector<HelpPrompt> getHelpPrompts() override;

private:
	class GridEntry
	{
	public:
		Vector2i pos;
		Vector2i dim;
		std::shared_ptr<GuiComponent> component;
		bool canFocus;
		bool resize;
		GridFlags::UpdateType updateType;
		unsigned int border;

		explicit GridEntry(const Vector2i& p = Vector2i::Zero(), const Vector2i& d = Vector2i::Zero(),
			std::shared_ptr<GuiComponent> cmp = nullptr, bool f = false, bool r = true,
			GridFlags::UpdateType u = GridFlags::UPDATE_ALWAYS, unsigned int b = GridFlags::BORDER_NONE) : 
			pos(p), dim(d), component(std::move(cmp)), canFocus(f), resize(r), updateType(u), border(b)
		{};

		explicit operator bool() const
		{
			return component != nullptr;
		}
	};

	float* mRowHeights;
	float* mColWidths;
	
	struct Vert
	{
		explicit Vert(float xi = 0, float yi = 0) : x(xi), y(yi) {};
		float x;
		float y;
	};

	std::vector<Vert> mLines;
	std::vector<unsigned int> mLineColors;

	// Update position & size
	void updateCellComponent(const GridEntry& cell);
	void updateSeparators();

	GridEntry* getCellAt(int x, int y);
	inline GridEntry* getCellAt(const Vector2i& pos) { return getCellAt(pos.x(), pos.y()); }
	
	Vector2i mGridSize;

	std::vector<GridEntry> mCells;

	void onCursorMoved(Vector2i from, Vector2i to);
	Vector2i mCursor;

	std::function<bool(InputConfig* config, Input input)> mUnhandledInputCallback;
};
