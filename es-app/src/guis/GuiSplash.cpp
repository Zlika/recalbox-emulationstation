//
// Created by bkg2k on 30/11/18.
//

#include "GuiSplash.h"
#include <Renderer.h>
#include <resources/Font.h>
#include <Locale.h>

GuiSplash::GuiSplash(Window* window)
  : GuiComponent(window),
    mWindow(window),
    mSplash(window, true),
    mLoading(window, _("LOADING..."), window->getMediumFont())
{
  mSplash.setResize(Renderer::getScreenWidth() * 0.6f, 0.0f);
  mSplash.setImage(":/splash.svg");
  mSplash.setPosition((Renderer::getScreenWidth() - mSplash.getSize().x()) / 2, (Renderer::getScreenHeight() - mSplash.getSize().y()) / 2 * 0.6f);

  mLoading.setHorizontalAlignment(Alignment::ALIGN_CENTER);
  mLoading.setSize(Renderer::getScreenWidth(), Renderer::getScreenHeight() / 10.0);
  mLoading.setPosition(0.0, Renderer::getScreenHeight() * 0.7, 0.0);
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

  /*std::string text = ;
  auto& font = mDefaultFonts.at(1);
  TextCache* cache = font->buildTextCache(text, 0, 0, 0x656565FF);
  trans = trans.translate(Eigen::Vector3f(round((Renderer::getScreenWidth() - cache->metrics.size.x()) / 2.0f),
                                          round(Renderer::getScreenHeight() * 0.835f), 0.0f));
  Renderer::setMatrix(trans);
  font->renderTextCache(cache);
  delete cache;*/

  //Renderer::swapBuffers();
}

void GuiSplash::update(int deltaTime)
{
  (void)deltaTime;
/*  GuiComponent::update(deltaTime);
  mBusyAnim.update(deltaTime);

  if (!mRunning) {
    if(mFunc2 != NULL)
      mFunc2(result);
    delete this;
  }*/
}