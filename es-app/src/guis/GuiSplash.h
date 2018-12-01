#pragma once

#include <GuiComponent.h>
#include <Window.h>
#include <components/TextComponent.h>
#include <components/ImageComponent.h>

class GuiSplash : public GuiComponent
{
  private:
    Window* mWindow;
    ImageComponent mSplash;
    TextComponent mLoading;

  public:
    /*!
     * Progress bar interface
     */
    class IProgressBar
    {
      public:
        virtual ~IProgressBar() {}

        /*!
         * Set the progress bar maximum value
         * @param maxvalue Maximum value (included)
         */
        virtual void serMaximum(int maxvalue) = 0;
        /*!
         * Set progress bar current value
         * @param value Current value, from 0 to maxvalue (included).
         */
        virtual void setProgression(int value) = 0;
    };

    GuiSplash(Window* window);

    /*
     * GuiComponent
     */

    void render(const Eigen::Affine3f &parentTrans) override;
    bool input(InputConfig *config, Input input) override;
    std::vector<HelpPrompt> getHelpPrompts() override;
    void update(int deltaTime) override;
};


