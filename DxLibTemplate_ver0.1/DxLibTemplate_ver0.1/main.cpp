#include "dxlib.h"
#include "const.h"
#include "keyManager.h"
#include "math.h"
#define PI 3.14159265f

const int POS_X = WINDOW_WIDTH / 2;
const int POS_Y = WINDOW_HEIGHT - 25;

double angle = PI / 2.0f;

Ball ball;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	// 描画先画面を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//---------------------------------------
	// 変数の作成や初期化、その他初期設定
	// ↓ システム初期化 ↓
	initKeyManager();
	// ↑ システム初期化 ↑
	//---------------------------------------

	// プレイヤーの初期設定
	player.pos.x = POS_X;
	player.pos.y = POS_Y;

	// ボールの初期設定
	ball.pos.x = WINDOW_WIDTH / 2;
	ball.pos.y = WINDOW_HEIGHT - 50;
	ball.mode = false;

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//---------------------------------------
		// 更新
		// ↓ システム更新 ↓
		updateKeyState();
		// ↑ システム更新 ↑
		//---------------------------------------

		// プレイヤーの方向変更
		// 左に傾く
		if (CheckHitKey(KEY_INPUT_LEFT))  angle += 0.05;
		// 右に傾く
		if (CheckHitKey(KEY_INPUT_RIGHT)) angle -= 0.05;

		// ボールを跳ばす処理
		// スペースキーを押したら時、ボールの状態がまだ跳んでいない状態なら
		// ボールを飛ばしていい状態にする
		if (pushHitKey(KEY_INPUT_SPACE) && ball.mode != true) {
			ball.mode = true;
			ball.vx = cos(angle) * 5;
			ball.vy = -sin(angle) * 5;
		}
		// ボールを飛ばしたら、プレイヤーの向いている方向に球を出す
		if (ball.mode == true) {

			ball.pos.x += ball.vx;
			// マイナス値の理由、プラス値だと、反対に跳び下方向に跳ぶためマイナス値で、上に上がるため。
			ball.pos.y += ball.vy;
			// 2. 左右の壁の判定 (画面幅を 640 と想定)
			if (ball.pos.x < 0 || ball.pos.x > WINDOW_WIDTH) {
				ball.pos.x *= -1; // X方向の移動向きを反転
			}
		}

		if (ball.pos.y <= -20.0) {
			ball.mode = false;

		}

		//---------------------------------------
		// 描画
		// ↓ 画面消去 ↓
		clsDx();
		ClearDrawScreen();
		// ↑ 画面消去 ↑
		//---------------------------------------

		//printfDx("ball.mode = %d" , ball.mode );
		DrawCircle(ball.pos.x, ball.pos.y, 16, GetColor(255, 0, 0), TRUE);

		DrawLine(player.pos.x, player.pos.y, player.pos.x + (int)(cos(angle) * 50), player.pos.y - (int)(sin(angle) * 50), GetColor(255, 255, 255));

		ScreenFlip();
	}


	DxLib_End();

	return 0;
}