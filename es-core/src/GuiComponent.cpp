#include "GuiComponent.h"
#include "Window.h"
#include "Log.h"
#include "Renderer.h"
#include "animations/AnimationController.h"
#include "ThemeData.h"
#include "Settings.h"

GuiComponent::GuiComponent(Window* window)
  : mOpacity(255),
		mWindow(window),
		mParent(nullptr),
	  mPosition(Vector3f::Zero()),
	  mOrigin(Vector2f::Zero()),
	  mRotationOrigin(0.5, 0.5),
	  mSize(Vector2f::Zero()),
	  mIsProcessing(false),
    mTransform(Transform4x4f::Identity())
{
	for (auto& i : mAnimationMap)
		i = nullptr;
}

GuiComponent::~GuiComponent()
{
	mWindow->removeGui(this);

	cancelAllAnimations();

	if(mParent)
		mParent->removeChild(this);

	for (unsigned int i = 0; i < getChildCount(); i++)
		getChild(i)->setParent(nullptr);
}

bool GuiComponent::input(InputConfig* config, Input input)
{
	for (unsigned int i = 0; i < getChildCount(); i++)
	{
		if(getChild(i)->input(config, input))
			return true;
	}

	return false;
}

void GuiComponent::updateSelf(int deltaTime)
{
	for (unsigned char i = 0; i < MAX_ANIMATIONS; i++)
		advanceAnimation(i, deltaTime);
}

void GuiComponent::updateChildren(int deltaTime)
{
	for (unsigned int i = 0; i < getChildCount(); i++)
	{
		getChild(i)->update(deltaTime);
	}
}

void GuiComponent::update(int deltaTime)
{
	updateSelf(deltaTime);
	updateChildren(deltaTime);
}

void GuiComponent::render(const Transform4x4f& parentTrans)
{
	Transform4x4f trans = parentTrans * getTransform();
	renderChildren(trans);
}

void GuiComponent::renderChildren(const Transform4x4f& transform) const
{
	for (unsigned int i = 0; i < getChildCount(); i++)
	{
		getChild(i)->render(transform);
	}
}

const Vector3f& GuiComponent::getPosition() const
{
	return mPosition;
}

void GuiComponent::setNormalisedPosition(float x, float y, float z)
{
    Vector2f pos = denormalise(x, y);
    setPosition(pos.x(), pos.y(), z);
}

void GuiComponent::setPosition(float x, float y, float z)
{
	mPosition.Set(x, y, z);
	onPositionChanged();
}

const Vector2f& GuiComponent::getOrigin() const
{
	return mOrigin;
}

void GuiComponent::setOrigin(float x, float y)
{
	mOrigin.Set(x, y);
	onOriginChanged();
}

const Vector2f& GuiComponent::getRotationOrigin() const
{
	return mRotationOrigin;
}

void GuiComponent::setRotationOrigin(float x, float y)
{
	mRotationOrigin.Set(x, y);
}

const Vector2f& GuiComponent::getSize() const
{
	return mSize;
}

void GuiComponent::setSize(float w, float h)
{
	mSize.Set(w, h);
    onSizeChanged();
}

float GuiComponent::getRotation() const
{
	return mRotation;
}

void GuiComponent::setRotation(float rotation)
{
	mRotation = rotation;
}

float GuiComponent::getScale() const
{
	return mScale;
}

void GuiComponent::setScale(float scale)
{
	mScale = scale;
}

float GuiComponent::getZIndex() const
 {
 	return mZIndex;
 }
 
 void GuiComponent::setZIndex(float z)
 {
 	mZIndex = z;
 }
 
 float GuiComponent::getDefaultZIndex() const
 {
 	return mDefaultZIndex;
 }
 
 void GuiComponent::setDefaultZIndex(float z)
 {
 	mDefaultZIndex = z;
 }

Vector2f GuiComponent::getCenter() const
{
	return Vector2f(mPosition.x() - (getSize().x() * mOrigin.x()) + getSize().x() / 2,
						      mPosition.y() - (getSize().y() * mOrigin.y()) + getSize().y() / 2);
}

//Children stuff.
void GuiComponent::addChild(GuiComponent* cmp)
{
	mChildren.push_back(cmp);

	if(cmp->getParent())
		cmp->getParent()->removeChild(cmp);

	cmp->setParent(this);
}

void GuiComponent::removeChild(GuiComponent* cmp)
{
	if(!cmp->getParent())
		return;

	if(cmp->getParent() != this)
	{
		LOG(LogError) << "Tried to remove child from incorrect parent!";
	}

	cmp->setParent(nullptr);

	for (auto i = mChildren.begin(); i != mChildren.end(); i++)
	{
		if(*i == cmp)
		{
			mChildren.erase(i);
			return;
		}
	}
}

void GuiComponent::clearChildren()
{
	mChildren.clear();
}

void GuiComponent::sortChildren()
{
	std::stable_sort(mChildren.begin(), mChildren.end(),  [](GuiComponent* a, GuiComponent* b) {
		return b->getZIndex() > a->getZIndex();
	});
}

unsigned int GuiComponent::getChildCount() const
{
	return mChildren.size();
}

GuiComponent* GuiComponent::getChild(unsigned int i) const
{
	return mChildren.at(i);
}

void GuiComponent::setParent(GuiComponent* parent)
{
	mParent = parent;
}

GuiComponent* GuiComponent::getParent() const
{
	return mParent;
}

unsigned char GuiComponent::getOpacity() const
{
	return mOpacity;
}

void GuiComponent::setOpacity(unsigned char opacity)
{
	mOpacity = opacity;
	for (auto& it : mChildren)
	{
		it->setOpacity(opacity);
	}
}

const Transform4x4f& GuiComponent::getTransform()
{
	mTransform = Transform4x4f::Identity();
	mTransform.translate(mPosition);
	if (mScale != 1.0)
	{
		mTransform.scale(Vector3f(mScale));
	}
	if (mRotation != 0.0)
	{
		// Calculate offset as difference between origin and rotation origin
		float xOff = (mOrigin.x() - mRotationOrigin.x()) * mSize.x();
		float yOff = (mOrigin.y() - mRotationOrigin.y()) * mSize.y();

		// transform to offset point
		if (xOff != 0.0 || yOff != 0.0)
			mTransform.translate(Vector3f(xOff * -1, yOff * -1, 0.0f));

		// apply rotation transorm
		mTransform.rotate(mRotation, Vector3f::UnitZ());

		// Tranform back to original point
		if (xOff != 0.0 || yOff != 0.0)
			mTransform.translate(Vector3f(xOff, yOff, 0.0f));
	}
	mTransform.translate(Vector3f(mOrigin.x() * mSize.x() * -1, mOrigin.y() * mSize.y() * -1, 0.0f));
	return mTransform;
}

void GuiComponent::setValue(const std::string& value)
{
	(void)value;
}

void GuiComponent::setColor(unsigned int color)
{
	(void)color;
}

unsigned int GuiComponent::getOriginColor()
{
	return 0;
}

std::string GuiComponent::getValue() const
{
	return "";
}

void GuiComponent::textInput(const char* text)
{
	for (auto& iter : mChildren)
	{
		iter->textInput(text);
	}
}

void GuiComponent::setAnimation(Animation* anim, int delay, std::function<void()> finishedCallback, bool reverse, unsigned char slot)
{
	assert(slot < MAX_ANIMATIONS);

	AnimationController* oldAnim = mAnimationMap[slot];
	mAnimationMap[slot] = new AnimationController(anim, delay, std::move(finishedCallback), reverse);

	delete oldAnim;
}

bool GuiComponent::stopAnimation(unsigned char slot)
{
	assert(slot < MAX_ANIMATIONS);
	if(mAnimationMap[slot])
	{
		delete mAnimationMap[slot];
		mAnimationMap[slot] = nullptr;
		return true;
	}else{
		return false;
	}
}

bool GuiComponent::cancelAnimation(unsigned char slot)
{
	assert(slot < MAX_ANIMATIONS);
	if(mAnimationMap[slot])
	{
		mAnimationMap[slot]->removeFinishedCallback();
		delete mAnimationMap[slot];
		mAnimationMap[slot] = nullptr;
		return true;
	}else{
		return false;
	}
}

bool GuiComponent::finishAnimation(unsigned char slot)
{
	assert(slot < MAX_ANIMATIONS);
	if(mAnimationMap[slot])
	{
		// skip to animation's end
		const bool done = mAnimationMap[slot]->update(mAnimationMap[slot]->getAnimation()->getDuration() - mAnimationMap[slot]->getTime());
    (void)done;
		assert(done);

		delete mAnimationMap[slot]; // will also call finishedCallback
		mAnimationMap[slot] = nullptr;
		return true;
	}else{
		return false;
	}
}

bool GuiComponent::advanceAnimation(unsigned char slot, unsigned int time)
{
	assert(slot < MAX_ANIMATIONS);
	AnimationController* anim = mAnimationMap[slot];
	if(anim)
	{
		bool done = anim->update((int)time);
		if(done)
		{
			mAnimationMap[slot] = nullptr;
			delete anim;
		}
		return true;
	}else{
		return false;
	}
}

void GuiComponent::stopAllAnimations()
{
	for (unsigned char i = 0; i < MAX_ANIMATIONS; i++)
		stopAnimation(i);
}

void GuiComponent::cancelAllAnimations()
{
	for (unsigned char i = 0; i < MAX_ANIMATIONS; i++)
		cancelAnimation(i);
}

bool GuiComponent::isAnimationPlaying(unsigned char slot) const
{
	return mAnimationMap[slot] != nullptr;
}

bool GuiComponent::isAnimationReversed(unsigned char slot) const
{
	assert(mAnimationMap[slot] != nullptr);
	return mAnimationMap[slot]->isReversed();
}

int GuiComponent::getAnimationTime(unsigned char slot) const
{
	assert(mAnimationMap[slot] != nullptr);
	return mAnimationMap[slot]->getTime();
}

Vector2f GuiComponent::denormalise(float x, float y) {
    Vector2f value(x, y);
    return denormalise(value);
}

Vector2f GuiComponent::denormalise(const Vector2f& value) {
    Vector2f scale = getParent() ? getParent()->getSize() : Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());
    return value * scale;
}

void GuiComponent::applyTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element, unsigned int properties)
{
	Vector2f scale = getParent() ? getParent()->getSize() : Vector2f((float)Renderer::getScreenWidth(), (float)Renderer::getScreenHeight());

	const ThemeData::ThemeElement* elem = theme->getElement(view, element, "");
	if(!elem)
		return;

	using namespace ThemeFlags;
	if(properties & POSITION && elem->has("pos"))
	{
		Vector2f denormalized = elem->get<Vector2f>("pos") * scale;
		setPosition(Vector3f(denormalized.x(), denormalized.y(), 0));
	}

	if(properties & ThemeFlags::SIZE && elem->has("size"))
		setSize(elem->get<Vector2f>("size") * scale);

	// position + size also implies origin
	if((properties & ORIGIN || (properties & POSITION && properties & ThemeFlags::SIZE)) && elem->has("origin"))
		setOrigin(elem->get<Vector2f>("origin"));

	if(properties & ThemeFlags::ROTATION) {
		if(elem->has("rotation"))
			setRotationDegrees(elem->get<float>("rotation"));
		if(elem->has("rotationOrigin"))
			setRotationOrigin(elem->get<Vector2f>("rotationOrigin"));
	}
	
	if(properties & ThemeFlags::Z_INDEX && elem->has("zIndex"))
		setZIndex(elem->get<float>("zIndex"));
	else
		setZIndex(getDefaultZIndex());
}

void GuiComponent::updateHelpPrompts()
{
	if(getParent())
	{
		getParent()->updateHelpPrompts();
		return;
	}

	std::vector<HelpPrompt> prompts = getHelpPrompts();

	if(mWindow->peekGui() == this)
		mWindow->setHelpPrompts(prompts, getHelpStyle());
}

HelpStyle GuiComponent::getHelpStyle()
{
	HelpStyle style = HelpStyle();
	if (Settings::getInstance()->getBool("ThemeHasMenuView"))
		style.applyTheme(ThemeData::getCurrent(), "menu");
	else
		style.applyTheme(ThemeData::getCurrent(), "system");
	return style;
}

bool GuiComponent::isProcessing() const
{
	return mIsProcessing;
}

void GuiComponent::onShow()
{
    for (unsigned int i = 0; i < getChildCount(); i++)
        getChild(i)->onShow();
}

void GuiComponent::onHide()
{
	for (unsigned int i = 0; i < getChildCount(); i++)
        getChild(i)->onHide();
}
