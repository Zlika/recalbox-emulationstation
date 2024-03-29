#include "Vector2f.h"
#include "Vector2i.h"

Vector2i Vector2f::toInt() const
{
  return Vector2i((int)mX, (int)mY);
}

Vector2f& Vector2f::round()
{
	mX = (float)(int)(mX + 0.5f);
	mY = (float)(int)(mY + 0.5f);

	return *this;

} // round

Vector2f& Vector2f::lerp(const Vector2f& _start, const Vector2f& _end, const float _fraction)
{
	mX = Math::lerp(_start.x(), _end.x(), _fraction);
	mY = Math::lerp(_start.y(), _end.y(), _fraction);

	return *this;

} // lerp
