#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Headers.h"

static const float epsilon = 1e-6;
static const float pi = acos(-1.0f);

// 角色状态
enum State {
	STAND, 
	MOVELEFT, 
	MOVERIGHT,
	ATTACK,
	DEAD, 
	SMITTEN, 
	DEFENSE
};

// 随机数生成器
static std::mt19937 rnd(time(0));

// 字体
static const std::string TIPS_FNT_PATH = "fonts/futura-48.fnt";

// 背景图
static const std::string START_BACKGROUND_PATH = "backgrounds/StartBackground.png";
static const std::string LEVEL1_BACKGROUND_PATH = "backgrounds/Level1Background.png";
static const std::string LEVEL2_BACKGROUND_PATH = "backgrounds/Level2Background.png";
static const std::string LEVEL3_BACKGROUND_PATH = "backgrounds/Level3Background.png";
static const std::string END_BACKGROUND_PATH = "backgrounds/EndBackground.png";

// 剧情/提示
static const float HINT_DELTA_PERCENT = 0.01;  // 剧情/提示出现速度
static const std::string HINT_BACKGROUND_PATH = "hints/HintBackground.png";
static const int START_AND_PAUSE_HINT_TAG = 301;
static const std::string START_AND_PAUSE_HINT = "Press SPACE to start or P to pause/resume.";
static const std::string REPLAY_AND_NEXT_HINT = "Press R to replay or N to the next level.";
static const std::string START_STORY_PATH = "hints/StartStory.png";
static const std::string END_STORY_PATH = "hints/EndStory.png";

// 云背景
static const int CLOUD_BACKGROUND_TAG = 100;
static const std::string CLOUD_BACKGROUND_PATH = "cloud.png";

// 右下角关闭按钮
static const std::string CLOSE_BUTTON_PATH = "CloseButton/CloseNormal.png";
static const std::string CLOSE_BUTTON_SELECTED_PATH = "CloseButton/Selected.png";

// 摇杆
static const float JOYSTICK_RADIUS = 63.0;
static const std::string JOYSTICK_PATH = "JoyStick/joystick.png";
static const std::string JOYSTICK_BUTTON_PATH = "JoyStick/joystick_button.png";

// 血条
static const std::string HERO_BLOOD_BAR_PATH = "BloodBar/HeroBloodBar.png";
static const std::string HERO_BLOOD_BAR_BACKGROUND_PATH = "BloodBar/HeroBloodBarBackground.png";
static const std::string TAUREN_BLOOD_BAR_PATH = "BloodBar/TaurenBloodBar.png";
static const std::string TAUREN_BLOOD_BAR_BACKGROUND_PATH = "BloodBar/TaurenBloodBarBackground.png";

// 骨骼动画
static const std::string ANIMATION_HERO_PATH = "Hero/Hero.ExportJson";
static const std::string ANIMATION_TAUREN_PATH = "tauren/tauren.ExportJson";

// 技能按钮
static const int ATTACK_BUTTON_TAG = 201;
static const std::string ATTACK_BUTTON = "SkillButtons/AttackButton.png";
static const std::string ATTACK_BUTTON_PRESSED = "SkillButtons/AttackButtonPressed.png";
static const int DEFENSE_BUTTON_TAG = 202;
static const std::string DEFENSE_BUTTON = "SkillButtons/DefenseButton.png";
static const std::string DEFENSE_BUTTON_PRESSED = "SkillButtons/DefenseButtonPressed.png";
static const int KNOCKBACK_BUTTON_TAG = 203;
static const std::string KNOCKBACK_BUTTON = "SkillButtons/KnockbackButton.png";
static const std::string KNOCKBACK_BUTTON_PRESSED = "SkillButtons/KnockbackButtonPressed.png";
static const int BARRAGE_BUTTON_TAG = 204;
static const std::string BARRAGE_BUTTON = "SkillButtons/BarrageButton.png";
static const std::string BARRAGE_BUTTON_PRESSED = "SkillButtons/BarrageButtonPressed.png";

// HERO 技能 CD
static const int ATTACK_CD = 60;
static const int DEFENSE_CD = 50;
static const int KNOCKBACK_CD = 200;
static const int BARRAGE_CD = 500;

// 技能贴图
static const std::string TAUREN_SHIELD_PATH = "SkillItems/TaurenShield.png";
static const std::vector<std::string> BULLET_PATHS = {
	"SkillItems/bullet1.png",
	"SkillItems/bullet2.png",
	"SkillItems/bullet3.png",
	"SkillItems/bullet4.png"
};
static const std::string BARRAGE_EMITTER_PATH = "SkillItems/emitter.png";

// Tauren 技能 CD
static const int TAUREN_DEFENSE_CD = 400;
static const int TAUREN_STEALTH_CD = 500;
static const int TAUREN_STRIKE_FLY_CD = 600;

// 参数
static const int BASIC_DAMAGE = 10;  // 平 A 基础攻击力
static const int DEFENSE_DAMAGE = 5;  // Hero 防御抵消的攻击力
static const int SPEED_X = 4;  // 角色移动速度
static const int HERO_SMITTEN_LOCK = 50;  // Hero 相邻两次受伤的最小时间间隔
static int TAUREN_SWITCH_ACTION = 300;  // Tauren 切换动作序列的时间间隔
static const int BULLET_BASIC_DAMAGE = 10;  // 子弹的基础伤害
static const int BULLET_SPEED = 10;  // 子弹速度的模
static const float BULLET_FORCE = 5;  // 子弹击退距离是其速度的 x 分量的倍数
static const int TAUREN_STEALTH_TIME = 5;  // Tauren 隐身时间 (单位: s)
static const int BARRAGE_EMITTER_ROTATE_SPEED = 60;  // 弹幕发射器旋转速度 (0.1 s 相差的角度)
static const int HERO_BARRAGE_TIME = 300;  // 弹幕持续时间
static const int BARRAGE_DELTA_TIME = 50;  // 弹幕的发射时间间隔
static const int BARRAGE_DELTA_ANGLE = 30;  // 相邻弹幕的发射角度差
static const int BARRAGE_BASIC_DAMAGE = 1;  // 弹幕的基础伤害
static const float STRUCK_FLY_BASIC_SPEED = 50;  // 击飞的初速度 (最大)
static const float g = 0.98;  // 重力加速度
static const int STRUCK_FLY_BASIC_DAMAGE = 30;  // 击飞的基础伤害

// bgm
static const std::string MOFA_PATH = "sounds/mofa.mp3";
static const std::string DOKU_PATH = "sounds/doku.mp3";
static const std::string FAIRY_PATH = "sounds/fairy.mp3";

// 音效
static const std::string HIT_METAL_PATH = "sounds/hitMetal.mp3";
static const std::string GLASS_BREAK_PATH = "sounds/glassBreak.mp3";

// 计分板
static int heroScore = 0;
static int taurenScore = 0;

// 比较浮点数大小
int doubleCompare(double a, double b);

// 随机生成区间 [l, r] 内的整数
int generateRandomInteger(int l, int r);

#endif