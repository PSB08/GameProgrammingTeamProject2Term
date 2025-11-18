#pragma once
#include "fmod.hpp"
#pragma comment(lib, "fmod_vc")
enum class SOUND_CHANNEL //사운드마다 채널
{
	BGM, EFFECT, END
};
struct SoundInfo
{
	FMOD::Sound* pSound; // 실제 사운드 메모리
	bool IsLoop;        // 사운드마다 루프할지말지
};
class Texture;
class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);
public:
	bool Init();
	void FmodUpdate();
	void Release();
public:
	void LoadTexture(const wstring& _key, const wstring& _path);
	Texture* GetTexture(const wstring& _key);
	void RegisterGDI();
	void ReleaseGDI();
	bool RegisterFontFile(const wstring& _path);
	void ReleaseFonts();

	// Sound
	void LoadSound(const wstring& _key, const wstring& _path, bool _isLoop);
	void Play(const wstring& _key);
	void Stop(SOUND_CHANNEL _channel);
	void Volume(SOUND_CHANNEL _channel, float _vol);
	void Pause(SOUND_CHANNEL _channel, bool _ispause);
public:
	void RegisterFont(FontType _type, const wstring& _name, int _height,
		int _weight = FW_NORMAL, bool _italic = false, int _quality = CLEARTYPE_QUALITY);
	const HBRUSH& GetBrush(BrushType _eType)
	{
		return m_Brushs[(UINT)_eType];
	}
	const HPEN& GetPen(PenType _eType)
	{
		return m_Pens[(UINT)_eType];
	}
	const HFONT& GetFont(FontType _eType)
	{
		return m_Fonts[(UINT)_eType];
	}
private:
	void RegisterTexture();
	SoundInfo* FindSound(const wstring& _key);
	void RegisterSound();
public:
	const wstring& GetResPath() const
	{
		return m_resourcePath.native();
	}
private:
	//std::filesystem::path m_resourcePath;
	fs::path m_resourcePath;
	std::unordered_map<wstring, Texture*> m_mapTexture;
	HBRUSH m_Brushs[(UINT)BrushType::END] = {};
	HPEN m_Pens[(UINT)PenType::END] = {};
	HFONT m_Fonts[(UINT)FontType::END] = {};
	vector<wstring> m_vecFontFiles;
	std::unordered_map<wstring, SoundInfo*> m_mapSounds;
	FMOD::System* m_pSoundSystem; // 사운드 시스템
	FMOD::Channel* m_pChannel[(UINT)SOUND_CHANNEL::END]; // 오디오 채널
};

