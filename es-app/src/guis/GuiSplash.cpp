//
// Created by bkg2k on 30/11/18.
//

#include "GuiSplash.h"
#include <Renderer.h>
#include <resources/Font.h>
#include <Locale.h>

GuiSplash::GuiSplash(Window& window)
  : GuiComponent(&window),
    mWindow(window),
    mSplash(&window, true),
    mLoading(&window, _("LOADING..."), window.getMediumFont()),
    mProgressBarMaximumValue(0),
    mProgressBarCurrentValue(0)
{
  mSplash.setResize(Renderer::getScreenWidth() * 0.6f, 0.0f);
  mSplash.setImage(":/splash.svg");
  mSplash.setPosition((Renderer::getScreenWidth() - mSplash.getSize().x()) / 2, (Renderer::getScreenHeight() - mSplash.getSize().y()) / 2 * 0.6f);

  mLoading.setHorizontalAlignment(Alignment::ALIGN_CENTER);
  mLoading.setSize(Renderer::getScreenWidth(), Renderer::getScreenHeight() / 10.0f);
  mLoading.setPosition(0.0f, Renderer::getScreenHeight() * 0.7f, 0.0f);
}

bool GuiSplash::input(InputConfig *config, Input input)
{
  (void)config;
  (void)input;
  return false;
}

std::vector<HelpPrompt> GuiSplash::getHelpPrompts()
{
  return std::vector<HelpPrompt>();
}

void GuiSplash::render(const Eigen::Affine3f &parentTrans)
{
  Eigen::Affine3f trans = parentTrans * getTransform();

  Renderer::setMatrix(trans);
  Renderer::drawRect(0, 0, Renderer::getScreenWidth(), Renderer::getScreenHeight(), 0xFFFFFFFF);

  mSplash.render(trans);
  mLoading.render(trans);

  int w = (int)(Renderer::getScreenWidth() / 10.0f);
  int h = (int)(Renderer::getScreenHeight() / 50.0f);
  int x = Renderer::getScreenWidth() - (w / 2);
  int y = (int)(Renderer::getScreenHeight() * 0.9f);
  Renderer::drawRect(x, y, w, h, 0xFF808080);
}

void GuiSplash::update(int deltaTime)
{
  (void)deltaTime;
}

void GuiSplash::setMaximum(int maxvalue)
{
  // Check bounds
  if (maxvalue < 0) maxvalue = 0;
  // Assign and refresh
  mProgressBarMaximumValue = maxvalue;
  setProgression(mProgressBarCurrentValue);
}

void GuiSplash::setProgression(int value)
{
  // Lock the whole  method
  boost::lock_guard<boost::recursive_mutex> locker(mCriticalSection);

  // Check bounds
  if (value < 0) value = 0;
  if (value > mProgressBarMaximumValue) value = mProgressBarMaximumValue;
  mProgressBarCurrentValue = value;

  // force rendering
  mWindow.render();
  Renderer::swapBuffers();
}
