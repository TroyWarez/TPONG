#pragma once


extern HINSTANCE hInst;

/* 1972 Paddle Lenght PlayerPaddleX = 138.0f, PlayerPaddleX2 = 273.0f;CPUPaddle.x = 138.0f, CPUPaddle.x2 = 273.0f,*/

#define PlayerPaddleY 113.0f
#define PlayerPaddleY2 122.0f

#define BallSpeedinc 0.175f
#define NegativeBallSpeedinc -0.175f

#define HalfBallSpeedinc 0.20f
#define NegativeHalfBallSpeedinc -0.050f

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define xKEY_DOWN(vk_code) ((XInputGetKeystroke(vk_code) & 0x8000) ? 1 : 0)

#define VK_W 0x57
#define VK_S 0x53

static const float RandomSpawnA[24] = { 20.0f, 45.0f, 70.0f, 90.0f, 120.0f, 145.0f, 170.0f, 195.0f, 220.0f, 245.0f, 270.0f, 295.0f, 320.0f, 345.0f, 370.0f, 395.0f, 420.0f, 445.0f, 470.0f, 495.0f, 520.0f, 545.0f, 570.0f, 595.0f };

class GameLogic
{
public:
	XINPUT_STATE Xstate;
	_XINPUT_VIBRATION Xvib;
	unsigned int PlayerScore, CPUScore,

		RandomNumber, SpawnRandomNumber, BallRandomNumber, 
		NumModeSelect, NumModeSelect1, NumModeSelect2;

	float PlayerPaddleX, PlayerPaddleX2, RealPlayerPaddleX, RealPlayerPaddleX2,

		  Postive_Speed, Negative_Speed, HalfPostive_Speed, HalfNegative_Speed, BallSpeedM;

	DWORD rc = 0;

	struct {
		float x, y, x2, y2;
	} typedef Paddle;
	struct {
		float Speed1, Speed2, Speed3, Speed4;
	} typedef Velocity;

	struct {
		float x, y, x2, y2;
	} typedef Ball;
		bool CanGameBeReset,
			 IsInputReset,
			 IsTimeSaved,
			 IsTimeSaved2, 
			 IsBallSpawned, 
			 IsGameSetup, 
			 IdleBallSpawned, 
			 HasCPUScored, 
			 HasPlayerScored,
			 IdleModeBallSpawned,
			 StartGame,
			 TwoPlayerMode,
			 SlowBallMode,
			 OnePlayerMode;
	Ball Ball;
	Velocity BallVelocity;
	Paddle CPUPaddle;
public:
	GameLogic();
	~GameLogic();

	void UpdateState();

	int GetRandomNumber(unsigned int RandomNumber, unsigned int mod);

private:
	long long SpawnTime, ResetTime;


private:
	void ModeSelect();

	void GameDeconstructor();

	void IdleMode();

	void SetupGame();

	void CollisionDetection();
	void FindArea();
	void CompareArea();

	void CPU_AI();

	void BallPhysics();

	void SpawnBall();

	void BallTimer();
	void InputTimer();
};