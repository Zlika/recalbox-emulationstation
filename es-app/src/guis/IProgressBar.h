#pragma once

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
    virtual void setMaximum(int maxvalue) = 0;
    /*!
     * Set progress bar current value
     * @param value Current value, from 0 to maxvalue (included).
     */
    virtual void setProgression(int value) = 0;
};
