#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GameData.h"

bool ResourceManager::Init()
{
	//fs::path curPath  = fs::current_path();
	//// todo: release build path
	//m_resourcePath = curPath.parent_path() / L"Output\\build\\Resource\\";

	wchar_t buf[MAX_PATH] = {}; // windows 최대 경로 길이
	::GetModuleFileNameW(nullptr, buf, MAX_PATH); // 현재 실행중인 exe 경로 buf에 저장   
	fs::path exeDir = fs::path(buf).parent_path();                //  buf 전체 경로를 path 객체로 가서 디렉토리만 추출
	fs::path resourceDir = exeDir.parent_path() / L"build" / L"Resource\\"; // release모드일때 build 한번더 붙이는거 무시
	m_resourcePath = resourceDir.native();
	if (!RegisterFontFile(L"Font\\나눔손글씨 암스테르담.ttf"))
		return false;
	RegisterGDI();
	RegisterTexture();

	FMOD::System_Create(&m_pSoundSystem); // 시스템 생성
	if (m_pSoundSystem != nullptr)
		m_pSoundSystem->init(64, FMOD_INIT_NORMAL, nullptr);
	RegisterSound();

	auto data = GameData::GetInstance();
	m_bgmVolume = data->bgmVolume;
	m_effectVolume = data->effectVolume;

	return true;
}

void ResourceManager::FmodUpdate()
{
	if (m_pSoundSystem)
		m_pSoundSystem->update();
	m_elapsedTime += fDT;
}

void ResourceManager::Release()
{
	std::unordered_map<wstring, Texture*>::iterator iter;
	for (iter = m_mapTexture.begin(); iter != m_mapTexture.end(); ++iter)
		SAFE_DELETE(iter->second);
	m_mapTexture.clear();
	ReleaseGDI();
	ReleaseFonts();

	std::unordered_map<wstring, SoundInfo*>::iterator iterSound;
	for (iterSound = m_mapSounds.begin(); iterSound != m_mapSounds.end(); ++iterSound)
		SAFE_DELETE(iterSound->second);
	m_mapSounds.clear();

	m_pSoundSystem->close();
	m_pSoundSystem->release();
}

void ResourceManager::RegisterGDI()
{
	// BRUSH
	m_Brushs[(UINT)BrushType::HOLLOW] = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	m_Brushs[(UINT)BrushType::RED] = (HBRUSH)::CreateSolidBrush(RGB(255, 167, 167));
	m_Brushs[(UINT)BrushType::GREEN] = (HBRUSH)::CreateSolidBrush(RGB(134, 229, 134));

	// PEN 
	m_Pens[(UINT)PenType::RED] = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_Pens[(UINT)PenType::GREEN] = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	// 폰트 등록
	RegisterFont(FontType::TITLE, L"나눔손글씨 암스테르담", 30);
	RegisterFont(FontType::TITLE_BIG, L"나눔손글씨 암스테르담", 75);
}

void ResourceManager::ReleaseGDI()
{
	for (int i = 0; i < (UINT)PenType::END; ++i)
		::DeleteObject(m_Pens[i]);
	for (int i = 1; i < (UINT)BrushType::END; ++i)
		// Hollow 제외하고
		::DeleteObject(m_Brushs[i]);
	for (int i = 0; i < (UINT)FontType::END; ++i)
		::DeleteObject(m_Fonts[i]);
}


bool ResourceManager::RegisterFontFile(const wstring& _path)
{
	wstring fontPath = m_resourcePath;
	fontPath += _path;
	if (!(AddFontResourceExW(fontPath.c_str(), FR_PRIVATE, 0) > 0))
		return false;
	m_vecFontFiles.push_back(fontPath);
	return true;
}

void ResourceManager::ReleaseFonts()
{
	for (const auto& path : m_vecFontFiles)
		::RemoveFontResourceExW(path.c_str(), FR_PRIVATE, 0);
	m_vecFontFiles.clear();
}

void ResourceManager::LoadSound(const wstring& _key, const wstring& _path, bool _isLoop)
{
	if (FindSound(_key) || !m_pSoundSystem)
		return;
	wstring strFilePath = m_resourcePath;
	strFilePath += _path;

	// wstring to string
	std::string str;
	str.assign(strFilePath.begin(), strFilePath.end());

	// 루프할지 말지 결정
	FMOD_MODE eMode = FMOD_LOOP_NORMAL; // 반복 출력
	if (!_isLoop)
		eMode = FMOD_DEFAULT; // 사운드 1번만 출력
	FMOD::Sound* p = nullptr;

	// BGM면 stream, 아니면 sound
	// 팩토리함수
	//// 사운드 객체를 만드는 것은 system임.
	//						//파일경로,  FMOD_MODE, NULL, &sound
	FMOD_RESULT r = _isLoop
		? m_pSoundSystem->createStream(str.c_str(), eMode, nullptr, &p)
		: m_pSoundSystem->createSound(str.c_str(), eMode, nullptr, &p);

	if (r != FMOD_OK || !p)
		return;

	SoundInfo* pSound = new SoundInfo;
	pSound->IsLoop = _isLoop;
	pSound->pSound = p;
	m_mapSounds.insert({ _key, pSound });

}

void ResourceManager::Play(const wstring& _key)
{
	SoundInfo* pSound = FindSound(_key);
	if (!pSound || !m_pSoundSystem)
		return;

	SOUND_CHANNEL eChannel = pSound->IsLoop ? SOUND_CHANNEL::BGM : SOUND_CHANNEL::EFFECT;
	UINT idx = (UINT)eChannel;

	//EFFECT 사운드는 쿨타임 검사 (같은 키 기준)
	if (!pSound->IsLoop)
	{
		const float COOLDOWN = 0.3f; // 1초에 한 번
		float now = m_elapsedTime;

		auto it = m_lastEffectPlayTime.find(_key);
		if (it != m_lastEffectPlayTime.end())
		{
			if (now - it->second < COOLDOWN)
			{
				// 아직 쿨 안 돌았으면 재생 안 함
				return;
			}
		}

		m_lastEffectPlayTime[_key] = now;
	}

	//BGM 처리
	if (pSound->IsLoop)
	{
		// 같은 BGM이면 그냥 무시
		if (m_curBGMKey == _key)
			return;

		if (m_pChannel[idx])
		{
			bool isPlaying = false;
			m_pChannel[idx]->isPlaying(&isPlaying);
			if (isPlaying)
				m_pChannel[idx]->stop();
		}

		m_curBGMKey = _key;
	}

	FMOD_RESULT r = m_pSoundSystem->playSound(pSound->pSound, nullptr, false, &m_pChannel[idx]);
	if (r != FMOD_OK || !m_pChannel[idx])
		return;

	// 새 채널에 저장된 볼륨 적용
	float vol = GetSavedVolume(eChannel);
	m_pChannel[idx]->setVolume(vol);
}

void ResourceManager::Stop(SOUND_CHANNEL _channel)
{
	FMOD::Channel* ch = m_pChannel[(UINT)_channel];
	if (ch)
		ch->stop();
}

void ResourceManager::Volume(SOUND_CHANNEL _channel, float _vol)
{
	_vol = std::clamp(_vol, 0.f, 1.f);

	// 1) 내부 상태 저장
	switch (_channel)
	{
	case SOUND_CHANNEL::BGM:
		m_bgmVolume = _vol;
		break;
	case SOUND_CHANNEL::EFFECT:
		m_effectVolume = _vol;
		break;
	default:
		break;
	}

	// 2) GameData에도 반영 (세이브용)
	auto data = GameData::GetInstance();
	data->bgmVolume = m_bgmVolume;
	data->effectVolume = m_effectVolume;

	// 3) 현재 재생 중인 FMOD 채널에도 적용
	FMOD::Channel* ch = m_pChannel[(UINT)_channel];
	if (ch)
		ch->setVolume(_vol);
}

void ResourceManager::Pause(SOUND_CHANNEL _channel, bool _ispause)
{
	FMOD::Channel* ch = m_pChannel[(UINT)_channel];
	if (ch)
		ch->setPaused(_ispause);
}

SoundInfo* ResourceManager::FindSound(const wstring& _key)
{
	std::unordered_map<wstring, SoundInfo*>::iterator iter = m_mapSounds.find(_key);

	if (iter == m_mapSounds.end())
		return nullptr;
	return iter->second;
}

void ResourceManager::RegisterSound()
{
	LoadSound(L"TITLEBGM", L"Sound\\TitleBgm.mp3", true);
	LoadSound(L"BOSSSELECT", L"Sound\\BossSelect.mp3", true);
	LoadSound(L"BOSS1BGM", L"Sound\\Boss1Bgm.mp3", true);
	LoadSound(L"BOSS2BGM", L"Sound\\Boss2Bgm.mp3", true);
	LoadSound(L"BOSS3BGM", L"Sound\\Boss3Bgm.mp3", true);

	LoadSound(L"PlayerDie", L"Sound\\PlayerDie.mp3", false);
	LoadSound(L"GameClear", L"Sound\\GameClear.mp3", false);
	LoadSound(L"EnemyShoot", L"Sound\\EnemyGun.mp3", false);
	LoadSound(L"Laser", L"Sound\\Laser.mp3", false);
	LoadSound(L"SubcoreDestroy", L"Sound\\SubcoreDestroy.mp3", false);
	LoadSound(L"SubcoreReturn", L"Sound\\SubcoreReturn.mp3", false);
	LoadSound(L"BossCoreDestroy", L"Sound\\BossCoreDestroy.mp3", false);
	LoadSound(L"BossDie", L"Sound\\BossDie.mp3", false);
	LoadSound(L"UIButton", L"Sound\\Button.mp3", false);

	LoadSound(L"PlayerDash", L"Sound\\PlayerDash.mp3", false);
	LoadSound(L"PlayerJump", L"Sound\\PlayerJump.mp3", false);
	LoadSound(L"PlayerShoot", L"Sound\\PlayerShoot.mp3", false);
	LoadSound(L"PlayerShield", L"Sound\\PlayerShield.mp3", false);


}

float ResourceManager::GetSavedVolume(SOUND_CHANNEL ch) const
{
	switch (ch)
	{
	case SOUND_CHANNEL::BGM:    return m_bgmVolume;
	case SOUND_CHANNEL::EFFECT: return m_effectVolume;
	default:                    return 1.f;
	}
}

void ResourceManager::StopAllSounds()
{
	for (int i = 0; i < (int)SOUND_CHANNEL::END; ++i)
	{
		if (m_pChannel[i])
			m_pChannel[i]->stop();
	}

	m_lastEffectPlayTime.clear();

	m_curBGMKey.clear();
}

void ResourceManager::RegisterFont(FontType _type, const wstring& _name, int _height, int _weight, bool _italic, int _quality)
{

	HFONT h = ::CreateFont(_height, 0, 0, 0, _weight, _italic, false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, _quality, DEFAULT_PITCH || FF_DONTCARE, _name.c_str());
	m_Fonts[(UINT)_type] = h;
}



void ResourceManager::RegisterTexture()
{
	LoadTexture(L"plane", L"Texture\\plane.bmp");
	LoadTexture(L"bullet", L"Texture\\Bullet.bmp");
	LoadTexture(L"jiwoo", L"Texture\\jiwoo.bmp");
	LoadTexture(L"boss1", L"Texture\\boss1.bmp");
	LoadTexture(L"boss1Icon", L"Texture\\boss1Icon.bmp");
	LoadTexture(L"boss1Core", L"Texture\\Boss1_Break-Sheet.bmp");
	LoadTexture(L"boss1CoreBreaking", L"Texture\\Boss1_Breaking-Sheet.bmp");

	LoadTexture(L"LaserVertical", L"Texture\\Laser_Vectical.bmp");
	LoadTexture(L"LaserHorizontal", L"Texture\\Laser_Horizontal.bmp");
	LoadTexture(L"SmallExplosion", L"Texture\\small-explosion.bmp");
	LoadTexture(L"BigExplosion", L"Texture\\big-explosion.bmp");

	LoadTexture(L"ShieldIdle150", L"Texture\\ShieldIdle150.bmp");
	LoadTexture(L"ShieldIdle250", L"Texture\\ShieldIdle250.bmp");
	LoadTexture(L"ShieldIdle300", L"Texture\\ShieldIdle300.bmp");
	LoadTexture(L"ShieldIdle350", L"Texture\\ShieldIdle350.bmp");
	LoadTexture(L"Shield150", L"Texture\\Shield150.bmp");
	LoadTexture(L"Shield250", L"Texture\\Shield250.bmp");
	LoadTexture(L"Shield300", L"Texture\\Shield300.bmp");
	LoadTexture(L"Shield350", L"Texture\\Shield350.bmp");

	LoadTexture(L"boss2", L"Texture\\boss2.bmp");
	LoadTexture(L"boss2Icon", L"Texture\\boss2Icon.bmp");
	LoadTexture(L"boss2Subcore", L"Texture\\Boss2_SubCore.bmp");
	LoadTexture(L"boss2SubcoreDeath", L"Texture\\Boss2_SubCore_Death-sheet.bmp");
	LoadTexture(L"boss2SubcoreBreak", L"Texture\\Boss2_SubCore_Break-sheet.bmp");
	LoadTexture(L"boss2SubcoreBreaking", L"Texture\\Boss2_SubCore_Breaking-Sheet.bmp");
	LoadTexture(L"boss2SubcoreReturn", L"Texture\\Boss2_SubCore_ReSpawn-sheet.bmp");
	LoadTexture(L"boss2Maincore", L"Texture\\Boss2_Break-sheet.bmp");
	LoadTexture(L"boss2Death", L"Texture\\Boss2_Breaking-sheet.bmp");
	LoadTexture(L"bossBullet", L"Texture\\bossBullet.bmp");

	LoadTexture(L"boss3", L"Texture\\Boss3_Idle.bmp");
	LoadTexture(L"boss3Icon", L"Texture\\Boss3Icon.bmp");
	LoadTexture(L"boss3Attack", L"Texture\\Boss3_IdleAttack.bmp");
	LoadTexture(L"boss3Fold", L"Texture\\Boss3_Fold.bmp");
	LoadTexture(L"boss3UnFold", L"Texture\\Boss3_UnFold.bmp");

	LoadTexture(L"BackTest1", L"Texture\\backtest1.bmp");
	LoadTexture(L"BackTest2", L"Texture\\backtest2.bmp");
	LoadTexture(L"BackTest3", L"Texture\\backtest3.bmp");
	LoadTexture(L"BackTest4", L"Texture\\backtest4.bmp");
	LoadTexture(L"Backtest5", L"Texture\\backtest5.bmp");
	LoadTexture(L"Backtest6", L"Texture\\backtest6.bmp");

	LoadTexture(L"Ground", L"Texture\\Ground.bmp");

	LoadTexture(L"Speaker_On", L"Texture\\Speaker_N.bmp");
	LoadTexture(L"Speaker_Off", L"Texture\\Speaker_S.bmp");
	LoadTexture(L"BackBtn_Normal", L"Texture\\Pannel1.bmp");
	LoadTexture(L"BackBtn_Hover", L"Texture\\Pannel2.bmp");

	LoadTexture(L"PlayerMove", L"Texture\\PlayerMove.bmp");
	LoadTexture(L"PlayerMove_L", L"Texture\\PlayerMove_Left.bmp");
	LoadTexture(L"Player_Back", L"Texture\\Player_Back.bmp");
	LoadTexture(L"PlayerBullet", L"Texture\\PlayerBullet.bmp");

	LoadTexture(L"Button", L"Texture\\Button.bmp");
	LoadTexture(L"Platform", L"Texture\\Platform.bmp");
}



void ResourceManager::LoadTexture(const wstring& _key, const wstring& _path)
{
	Texture* pTex = GetTexture(_key);
	// 못찾았으면 리턴
	if (nullptr != pTex)
		return;
	// 처음에 없을거니 경로 찾아서
	wstring texPath = m_resourcePath;
	texPath += _path;
	// 만들어서
	pTex = new Texture;
	pTex->Load(texPath); // 텍스처 자체 로드
	pTex->SetKey(_key); // 키 경로 세팅
	pTex->SetRelativePath(texPath);
	m_mapTexture.insert({ _key,pTex }); // 맵에 저장
}

Texture* ResourceManager::GetTexture(const wstring& _key)
{
	auto iter = m_mapTexture.find(_key);
	if (iter != m_mapTexture.end())
		return iter->second;
	return nullptr;
}


