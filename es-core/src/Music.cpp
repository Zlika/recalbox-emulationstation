#include "Music.h"
#include "Log.h"
#include "Settings.h"
#include "ThemeData.h"
#include "AudioManager.h"
#include "RecalboxConf.h"

std::map< std::string, std::shared_ptr<Music> > Music::sMap;


std::shared_ptr<Music> Music::get(const std::string& path)
{
	auto it = sMap.find(path);
	if(it != sMap.end())
		return it->second;
	std::shared_ptr<Music> music = std::shared_ptr<Music>(new Music(path));
	sMap[path] = music;
        AudioManager::getInstance()->registerMusic(music);

	return music;
}

std::shared_ptr<Music> Music::getFromTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element)
{
	LOG(LogInfo) << " req music [" << view << "." << element << "]";
	const ThemeData::ThemeElement* elem = theme->getElement(view, element, "sound");
	if(!elem || !elem->has("path"))
	{
		LOG(LogInfo) << "   (missing)";
		return nullptr;
	}
	return get(elem->get<std::string>("path"));
}

Music::Music(const std::string & path) : music(nullptr), playing(false)
{
	mPath = path;
	initMusic();
}

Music::~Music()
{
	deinitMusic();
}

std::string Music::getName()
{
	boost::filesystem::path p(mPath);
	return p.stem().string();
}


void Music::initMusic()
{
	if(music != nullptr)
		deinitMusic();

	if(mPath.empty())
		return;

	//load wav file via SDL
        Mix_Music *gMusic = nullptr;
        gMusic = Mix_LoadMUS( mPath.c_str() );
        if(gMusic == nullptr){
            LOG(LogError) << "Error loading sound \"" << mPath << "\"!\n" << "	" << SDL_GetError();
            return;
        }else {
            music = gMusic;
        }
}

void Music::deinitMusic()
{
	playing = false;
        if(music != nullptr){
            Mix_FreeMusic( music );
            music = nullptr;
        }
}

void Music::play(bool repeat, void (* callback)())
{
    if(music == nullptr)
		return;
	if (!playing)
	{
		playing = true;
	}
    LOG(LogInfo) << "playing";
    if(Mix_FadeInMusic(music, repeat ? -1 : 1, 1000) == -1){
        LOG(LogInfo) << "Mix_PlayMusic: " << Mix_GetError();
		return;
    }
	if(!repeat){
		Mix_HookMusicFinished(callback);
	}
}