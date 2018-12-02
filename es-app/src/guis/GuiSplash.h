#pragma once

#include "GuiComponent.h"
#include "IProgressBar.h"
#include "Window.h"
#include "components/TextComponent.h"
#include "components/ImageComponent.h"

#include <boost/thread.hpp>

class GuiSplash : public GuiComponent, public IProgressBar
{
  private:
    Window& mWindow;
    ImageComponent mSplash;
    TextComponent mLoading;

    boost::recursive_mutex mCriticalSection;
    int mProgressBarMaximumValue;
    int mProgressBarCurrentValue;

  public:
    explicit GuiSplash(Window& window);

    /*
     * IGuiSplashProgressBar Implementation
     */

    void setMaximum(int maxvalue) override;
    void setProgression(int value) override;
    void resetProgression() override;
    void incProgression() override;

    /*
     * GuiComponent overrides
     */

    void render(const Eigen::Affine3f &parentTrans) override;
    bool input(InputConfig *config, Input input) override;
    std::vector<HelpPrompt> getHelpPrompts() override;
    void update(int deltaTime) override;
};


