#pragma once

#include <string>
#include <memory>
#include "platform.h"
#include <mutex>
#include "platform_gl.h"
#include <nanosvg/nanosvg.h>

class TextureResource;

class TextureData
{
public:
	TextureData(bool tile);
  TextureData();
	~TextureData();

	// These functions populate mDataRGBA but do not upload the texture to VRAM

	//!!!! Needs to be canonical path. Caller should check for duplicates before calling this
	void initFromPath(const std::string& path);
	bool initSVGFromMemory(const unsigned char* fileData, size_t length);
	bool initImageFromMemory(const unsigned char* fileData, size_t length);
	bool initFromRGBA(const unsigned char* dataRGBA, size_t width, size_t height);

  bool updateFromRGBA(const unsigned char* dataRGBA, size_t width, size_t height);

    // Read the data into memory if necessary
	bool load();

	bool isLoaded();

	// Upload the texture to VRAM if necessary and bind. Returns true if bound ok or
	// false if either not loaded
	bool uploadAndBind();

	// Release the texture from VRAM
	void releaseVRAM();

	// Release the texture from conventional RAM
	void releaseRAM();

	// Reset texture
	void reset();

	// Get the amount of VRAM currenty used by this texture
	size_t getVRAMUsage();

	size_t width();
	size_t height();
	float sourceWidth();
	float sourceHeight();
	void setSourceSize(float width, float height);

	bool tiled() { return mTile; }

private:
	std::mutex		mMutex;
	bool			mTile;
	std::string		mPath;
	GLuint 			mTextureID;
	unsigned char*	mDataRGBA;
	size_t			mWidth;
	size_t			mHeight;
	float			mSourceWidth;
	float			mSourceHeight;
	bool			mScalable;
	bool			mReloadable;
	NSVGimage*		mSVGImage;
};
