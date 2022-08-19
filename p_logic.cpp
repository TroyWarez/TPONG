#include "p_pchmain.h"
#include "p_audio.h"
#include "p_graphics.h"
#include "p_logic.h"
#include "p_resource.h"

Audio audio;
extern Graphics graphics;

GameLogic::~GameLogic()
{}

GameLogic::GameLogic()
{
	BallSpeedM = BallRandomNumber = RandomNumber = SpawnRandomNumber = 0;
	RealPlayerPaddleX = 0.0f;
	RealPlayerPaddleX2 = 0.0f;
	Postive_Speed = 2.5f, Negative_Speed = -2.5f, HalfPostive_Speed = 3.5f, HalfNegative_Speed = -1.0f;

	PlayerScore = CPUScore = 1;

	NumModeSelect = NumModeSelect1 = NumModeSelect2 = 0;

	CPUPaddle.x = CPUPaddle.x2 = CPUPaddle.y = CPUPaddle.y2 = 8000.0f;

	PlayerPaddleX = PlayerPaddleX2 = -100.0f;

	BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;

	Ball.x = Ball.x2 = 255.0f, Ball.y = Ball.y2 = 135.0f;

	    CanGameBeReset =
		IsTimeSaved =
		IsBallSpawned =
		IsGameSetup =
		IdleBallSpawned =
		HasCPUScored =
		HasPlayerScored =
		IdleModeBallSpawned =
		StartGame =
		OnePlayerMode =
		TwoPlayerMode =
		SlowBallMode = false;

	    SpawnTime = 0;

		ZeroMemory(&Xstate, sizeof(XINPUT_STATE));
		ZeroMemory(&Xvib, sizeof(_XINPUT_VIBRATION));
}
void GameLogic::SetupGame()
{
	Ball.x = Ball.x2 = -1.0f, Ball.y = Ball.y2 = 2.0f;

	PlayerScore = 0, CPUScore = 0;

	PlayerPaddleX = 138.0f, PlayerPaddleX2 = 223.0f;

	CPUPaddle.x = 138.0f, CPUPaddle.x2 = 223.0f,
	CPUPaddle.y = 644.0f, CPUPaddle.y2 = 654.0f;

	SpawnTime = 0;

	BallSpeedM = 1;
	IsBallSpawned = false;

	CanGameBeReset = false;

	IsTimeSaved = false;

	IsGameSetup = true;

}
void GameLogic::GameDeconstructor()
{
	BallSpeedM = 0;
	Postive_Speed = 5.0f, Negative_Speed = -5.0f, HalfPostive_Speed = 5.0f, HalfNegative_Speed = -2.5f;

	CPUPaddle.x = CPUPaddle.x2 = CPUPaddle.y = CPUPaddle.y2 = 8000.0f;

	PlayerPaddleX = PlayerPaddleX2 = -100.0f;

	BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;

	Ball.x = Ball.x2 = 255.0f, Ball.y = Ball.y2 = 135.0f;

	    IsTimeSaved =
		IsTimeSaved2 =
		IsBallSpawned =
		IsGameSetup =
		IdleBallSpawned =
		HasCPUScored =
		HasPlayerScored =
		IdleModeBallSpawned =
		StartGame = false;

		IsInputReset = true;

		BallSpeedM = SpawnTime = ResetTime = 0;
}
void GameLogic::UpdateState()
{
	DWORD Er = XInputGetState(0, &Xstate);
	CollisionDetection();
	ModeSelect();
	InputTimer();
	if ((OnePlayerMode) || (TwoPlayerMode) || (SlowBallMode)) {
		if (CanGameBeReset && IsInputReset) {
			if (KEY_DOWN(VK_RETURN) || Xstate.Gamepad.wButtons == XINPUT_GAMEPAD_BACK) {
				SetupGame();
			}
		}
	}
	if (OnePlayerMode) {
		CPU_AI();
	}
	if (TwoPlayerMode) {
		if (KEY_DOWN(VK_UP)) {
			CollisionDetection();
			CPUPaddle.x -= 6.0f, CPUPaddle.x2 -= 6.0f;
		}
		if (KEY_DOWN(VK_DOWN)) {
			CollisionDetection();
			CPUPaddle.x += 6.0f, CPUPaddle.x2 += 6.0f;
		}
	}
	if (KEY_DOWN(VK_W) || Xstate.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP) {
		CollisionDetection();
		PlayerPaddleX -= 8.0f, PlayerPaddleX2 -= 8.0f;
	}
	if (KEY_DOWN(VK_S) || Xstate.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN) {
		CollisionDetection();
		PlayerPaddleX += 8.0f, PlayerPaddleX2 += 8.0f;
	}
	CollisionDetection();
	if (!StartGame) {
	  IdleMode();
	}
	else if (!IsGameSetup) {
		SetupGame();
	}
	else if (!IsBallSpawned) {
		BallTimer();
		CollisionDetection();
	}
	else if (IsBallSpawned)
	{
		BallPhysics();
	}
}
void GameLogic::ModeSelect()
{
	if (KEY_DOWN(0x31) || Xstate.Gamepad.wButtons == XINPUT_GAMEPAD_START)
	{
		NumModeSelect++;
		switch (NumModeSelect)
		{
		case 1:
		{
			if (SlowBallMode)
			{
				SlowBallMode = false;
			}
			NumModeSelect1 = NumModeSelect2 = 0;
			OnePlayerMode = true;
			TwoPlayerMode = false;
			StartGame = true;
			Postive_Speed = 5.0f, Negative_Speed = -5.0f, HalfPostive_Speed = 5.0f, HalfNegative_Speed = -2.5f;
			SetupGame();
			break;
		}
		case 2:
		{
			NumModeSelect = 2;
			break;
		}
		default:
		{
			NumModeSelect = 2;
			break;
		}
		}
	}
	if (KEY_DOWN(0x32))
	{
		NumModeSelect1++;
		switch (NumModeSelect1)
		{
		case 1:
		{
			if (SlowBallMode)
			{
				SlowBallMode = false;
			}
			NumModeSelect = NumModeSelect2 = 0;
			OnePlayerMode = false;
			TwoPlayerMode = true;
			StartGame = true;
			Postive_Speed = 5.0f, Negative_Speed = -5.0f, HalfPostive_Speed = 5.0f, HalfNegative_Speed = -2.5f;
			SetupGame();
			break;
		}
		case 2:
		{
			NumModeSelect1 = 2;
			break;
		}
		default:
		{
			NumModeSelect1 = 2;
			break;
		}
		}
	}
	if (KEY_DOWN(0x33))
	{
		NumModeSelect2++;
		switch (NumModeSelect2)
		{
		case 1:
		{
			if ((!OnePlayerMode) && (!TwoPlayerMode))
			{
				OnePlayerMode = true;
			}
			NumModeSelect = NumModeSelect1 = 0;
			SlowBallMode = true;
			StartGame = true;
			Postive_Speed = 2.5f, Negative_Speed = -2.5f, HalfPostive_Speed = 2.5f, HalfNegative_Speed = -1.75f;
			SetupGame();
			break;
		}
		case 2:
		{
			NumModeSelect2 = 2;
			break;
		}
		default:
		{
			NumModeSelect2 = 1;
			break;
		}
		}
	}
}
void GameLogic::CollisionDetection()
{
	if (PlayerPaddleX <= 23.0f)
		PlayerPaddleX = PlayerPaddleX + 6.0f, PlayerPaddleX2 = PlayerPaddleX2 + 6.0f;
	if (PlayerPaddleX2 >= 577.0f)
		PlayerPaddleX = PlayerPaddleX - 6.0f, PlayerPaddleX2 = PlayerPaddleX2 - 6.0f;
	if (CPUPaddle.x <= 23.0f)
		CPUPaddle.x = CPUPaddle.x + 6.0f, CPUPaddle.x2 = CPUPaddle.x2 + 6.0f;
	if (CPUPaddle.x2 >= 577.0f)
		CPUPaddle.x = CPUPaddle.x - 6.0f, CPUPaddle.x2 = CPUPaddle.x2 - 6.0f;
	if (Ball.y < PlayerPaddleY2 && Ball.y > PlayerPaddleY && Ball.x > PlayerPaddleX && Ball.x < PlayerPaddleX2 ||
		Ball.y2 < PlayerPaddleY2 && Ball.y2 > PlayerPaddleY && Ball.x2 > PlayerPaddleX && Ball.x2 < PlayerPaddleX2) { //Player Paddle Sound
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInst, SND_ASYNC | SND_RESOURCE);
		if(BallSpeedM < 0.1f)
		BallSpeedM = BallSpeedM + 0.1f;
		audio.PlayResource(IDR_WAVE1);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;

		GetRandomNumber(BallRandomNumber, 4);
		switch (BallRandomNumber)
		{
		case 0: {
				BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
				break;
		}
		case 1: {
			BallVelocity.Speed1 = BallVelocity.Speed2 = 0.0f, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
			break;
		}
		case 2: {
			BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
			break;
		}
		case 3: {
			BallVelocity.Speed1 = HalfPostive_Speed, BallVelocity.Speed2 = HalfPostive_Speed, BallVelocity.Speed3 = HalfPostive_Speed, BallVelocity.Speed4 = HalfPostive_Speed;
			break;
		}
		case 4: {
			BallVelocity.Speed1 = HalfNegative_Speed, BallVelocity.Speed2 = HalfNegative_Speed, BallVelocity.Speed3 = HalfPostive_Speed, BallVelocity.Speed4 = HalfPostive_Speed;
			break;
		}
			default:
				break;
		}
		
	}
	if (Ball.y2 > CPUPaddle.y && Ball.y2 < CPUPaddle.y2 && Ball.x2 > CPUPaddle.x && Ball.x2 < CPUPaddle.x2 ||
		Ball.y > CPUPaddle.y && Ball.y < CPUPaddle.y2 && Ball.x > CPUPaddle.x && Ball.x < CPUPaddle.x2) {
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInst, SND_ASYNC | SND_RESOURCE);
		if (BallSpeedM < 0.1f)
			BallSpeedM = BallSpeedM + 0.1f;
		audio.PlayResource(IDR_WAVE1);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;
		GetRandomNumber(BallRandomNumber, 4);
		switch (BallRandomNumber)
		{
		case 0: {
			BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			break;
		}
		case 1: {
			BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			break;
		}
		case 2:
		{
			BallVelocity.Speed1 = BallVelocity.Speed2 = 0.0f, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			break;
		}
		case 3: {
			BallVelocity.Speed1 = HalfNegative_Speed, BallVelocity.Speed2 = HalfNegative_Speed, BallVelocity.Speed3 = HalfNegative_Speed, BallVelocity.Speed4 = HalfNegative_Speed;
			break;
		}
		case 4: {
			BallVelocity.Speed1 = HalfPostive_Speed, BallVelocity.Speed2 = HalfPostive_Speed, BallVelocity.Speed3 = HalfNegative_Speed, BallVelocity.Speed4 = HalfNegative_Speed;
			break;
		}
		default:
			break;
		}
	}
}
void GameLogic::FindArea()
{

}
void GameLogic::CompareArea()
{

}
void GameLogic::CPU_AI()
{
	CollisionDetection();
	if (Ball.y2 >= 400.0f) {
		if ((CPUPaddle.x + 40.0f) <= Ball.x) {
			CPUPaddle.x += 5.0f, CPUPaddle.x2 += 5.0f;
		}
		else if ((CPUPaddle.x2 + 40.0f) >= Ball.x2) {
			CPUPaddle.x += -5.0f, CPUPaddle.x2 += -5.0f;
		}
	}
	else if (CPUPaddle.x < 143.0f)
	{
		if (CPUPaddle.x == 143.0f)
			int w = 0;
		CPUPaddle.x += 3.0f, CPUPaddle.x2 += 3.0f;
	}
	else if (CPUPaddle.x > 143.0f)
	{
		if (CPUPaddle.x == 143.0f)
			int w = 0;
		CPUPaddle.x += -3.0f, CPUPaddle.x2 += -3.0f;
	}

}
void GameLogic::BallPhysics()
{
	if (Ball.y < 0.0f) {
		if (SlowBallMode) {
			Postive_Speed = 2.5f, Negative_Speed = -2.5f, HalfPostive_Speed = 2.5f, HalfNegative_Speed = -1.75f;
			BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
		}
		BallSpeedM = 1;
		CPUScore++;
		Ball.x = 590.0f, Ball.x2 = 590.0f, Ball.y = 335.0f, Ball.y2 = 335.0f;
		IsBallSpawned = false;
		HasCPUScored = true;
		HasPlayerScored = false;
		if (CPUScore == 11 || PlayerScore == 11)
			GameDeconstructor();
		else
		{
			//PlaySound(MAKEINTRESOURCE(IDR_WAVE3), hInst, SND_ASYNC | SND_RESOURCE);
			audio.PlayResource(IDR_WAVE3);
			Xvib.wLeftMotorSpeed = 65535;
			Xvib.wRightMotorSpeed = 65535;
			XInputSetState(0, &Xvib);
		}
	
	}
	if (Ball.y2 > 800.0f) {
		if (SlowBallMode) {
			Postive_Speed = 2.5f, Negative_Speed = -2.5f, HalfPostive_Speed = 2.5f, HalfNegative_Speed = -1.75f;
			BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
		}
		BallSpeedM = 1;
		PlayerScore++;
		Ball.x = 590.0f, Ball.x2 = 590.0f, Ball.y = 335.0f, Ball.y2 = 335.0f;
		IsBallSpawned =  false;
		HasCPUScored = false;
		HasPlayerScored = true;
        if (CPUScore == 11 || PlayerScore == 11)
			GameDeconstructor();
		else
		{
			//PlaySound(MAKEINTRESOURCE(IDR_WAVE3), hInst, SND_ASYNC | SND_RESOURCE);
			audio.PlayResource(IDR_WAVE3);
			Xvib.wLeftMotorSpeed = 65535;
			Xvib.wRightMotorSpeed = 65535;
			XInputSetState(0, &Xvib);
		}
	
	}
	if (Ball.x < 0.0f && BallVelocity.Speed4 == HalfNegative_Speed) {
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), hInst, SND_ASYNC | SND_RESOURCE);
		audio.PlayResource(IDR_WAVE2);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;
		BallVelocity.Speed1 = HalfPostive_Speed, BallVelocity.Speed2 = HalfPostive_Speed, BallVelocity.Speed3 = HalfNegative_Speed, BallVelocity.Speed4 = HalfNegative_Speed;
	}
	else if (Ball.x < 0.0f && BallVelocity.Speed4 == Negative_Speed) {
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), hInst, SND_ASYNC | SND_RESOURCE);
		audio.PlayResource(IDR_WAVE2);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;
		BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
	}
	else if (Ball.x < 0.0f) {
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), hInst, SND_ASYNC | SND_RESOURCE);
		audio.PlayResource(IDR_WAVE2);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;
		BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
	}

	if (Ball.x2 > 600.0f && BallVelocity.Speed4 == HalfNegative_Speed) {
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), hInst, SND_ASYNC | SND_RESOURCE);
		audio.PlayResource(IDR_WAVE2);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;
		BallVelocity.Speed1 = HalfNegative_Speed, BallVelocity.Speed2 = HalfNegative_Speed, BallVelocity.Speed3 = HalfNegative_Speed, BallVelocity.Speed4 = HalfNegative_Speed;
	}
	else if (Ball.x2 > 600.0f && BallVelocity.Speed4 == Negative_Speed) {
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), hInst, SND_ASYNC | SND_RESOURCE);
		audio.PlayResource(IDR_WAVE2);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;
		BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
	}
	else if (Ball.x2 > 600.0f) {
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), hInst, SND_ASYNC | SND_RESOURCE);
		audio.PlayResource(IDR_WAVE2);
		if (SlowBallMode)
			Postive_Speed += BallSpeedinc, Negative_Speed += NegativeBallSpeedinc, HalfNegative_Speed += NegativeHalfBallSpeedinc, HalfPostive_Speed += HalfBallSpeedinc;
		BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
	}
	if (IsBallSpawned) {
			Ball.x += (BallVelocity.Speed1 * BallSpeedM), Ball.x2 += (BallVelocity.Speed2 * BallSpeedM), Ball.y += (BallVelocity.Speed3 * BallSpeedM), Ball.y2 += (BallVelocity.Speed4 * BallSpeedM);

	}
	else if (!IsBallSpawned) {
		BallTimer();
	}
}
int GameLogic::GetRandomNumber(unsigned int RandomNumber, unsigned int mod)
{
	srand((unsigned int)time(NULL));
	RandomNumber = rand() % mod;

	srand((unsigned int)time(NULL));
	BallRandomNumber = rand() % mod;

	srand((unsigned int)time(NULL));
	SpawnRandomNumber = rand() % mod;

	return RandomNumber;
}
void GameLogic::SpawnBall()
{
	if ((!HasCPUScored) && (!HasPlayerScored)) {
		GetRandomNumber(RandomNumber, 3);
		switch (RandomNumber)
		{
		case 0: {
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;

		}
		case 1: { 
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;
		}
		case 2: {
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;
		}
		case 3:{
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;
		
		}
		default:
			break;
		}
	}
	if (HasPlayerScored) {
		GetRandomNumber(RandomNumber, 1);
		switch (RandomNumber)
		{
		case 0: {
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;
		}
		case 1: {
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;
		}
		default:
			break;
		}
	}
	if (HasCPUScored) {
		GetRandomNumber(RandomNumber, 1);
		switch (RandomNumber)
		{
		case 0: {
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;
		}
		case 1: {
			GetRandomNumber(RandomNumber, 24);
			BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
			Ball.x = (RandomSpawnA[SpawnRandomNumber]), Ball.x2 = Ball.x + 10, Ball.y = 335.0f, Ball.y2 = 345.0f;
			IsBallSpawned = true;
			break;
		}
		default:
			break;
		}
	}
}
void GameLogic::InputTimer()
{
	time_t CurrentTime;
	time(&CurrentTime);
	if (!IsTimeSaved) {
		ResetTime = CurrentTime + 1;
		IsTimeSaved2 = true;
		IsInputReset = false;
	}
	if (CurrentTime >= ResetTime) {
		ResetTime = 0;
		IsTimeSaved2 = false;
		IsInputReset = true;
	}
}
void GameLogic::BallTimer()
{
	time_t CurrentTime;
	time(&CurrentTime);

	if (!IsTimeSaved) {
		SpawnTime = CurrentTime + 4;
		IsTimeSaved = true;
	}
	if (CurrentTime >= SpawnTime) {
		SpawnTime = 0;
		IsTimeSaved = false;
		CanGameBeReset = true;
		SpawnBall();
	}
	else if (CurrentTime >= SpawnTime - 3)
	{
		Xvib.wLeftMotorSpeed = 0;
		Xvib.wRightMotorSpeed = 0;
		XInputSetState(0, &Xvib);
	}
}
void GameLogic::IdleMode()
{
	if (!IdleBallSpawned) {
		Ball.x = RandomSpawnA[4], Ball.x2 = Ball.x + 10.0f, Ball.y = 335.0f, Ball.y2 = 345.0f;
		IdleBallSpawned = true;
	}
	if (Ball.y < 0.0f && BallVelocity.Speed1 == Postive_Speed) {
		BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
	}
	else if (Ball.y < 0.0f) {
		BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
	}

	if (Ball.x < 0.0f && BallVelocity.Speed4 == Negative_Speed) {
		BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
	}
	else if (Ball.x < 0.0f) {
		BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
	}

	if (Ball.x2 > 600.0f && BallVelocity.Speed4 == Negative_Speed) {
		BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
	}
	else if (Ball.x2 > 600.0f) {
		BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Postive_Speed, BallVelocity.Speed4 = Postive_Speed;
	}

	if (Ball.y2 > 800.0f && BallVelocity.Speed1 == Postive_Speed) {
		BallVelocity.Speed1 = Postive_Speed, BallVelocity.Speed2 = Postive_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
	}
	else if (Ball.y2 > 800.0f) {
		BallVelocity.Speed1 = Negative_Speed, BallVelocity.Speed2 = Negative_Speed, BallVelocity.Speed3 = Negative_Speed, BallVelocity.Speed4 = Negative_Speed;
	}

	Ball.x += BallVelocity.Speed1, Ball.x2 += BallVelocity.Speed2,

	Ball.y += BallVelocity.Speed3, Ball.y2 += BallVelocity.Speed4;

}