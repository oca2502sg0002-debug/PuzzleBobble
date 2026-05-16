#include "dxlib.h"
#include "const.h"
#include "keyManager.h"
#include "math.h"
#define PI 3.14159265f

const int POS_X = WINDOW_WIDTH / 2;
const int POS_Y = WINDOW_HEIGHT - 150;
const int BALL_SPEED = 10;

double angle = PI / 2.0f;

const int row = 8;	// 横
const int col = 7;	// 縦
int grid[row][col]; // グリッド

int stage1;

void Ball::Init()
{
	// ボールの初期設定
	ball.pos.x = WINDOW_WIDTH / 2;
	ball.pos.y = WINDOW_HEIGHT - 150;
	ball.mode = false;
}

void Ball::Nextball()
{
	// 次でるボールの設定
	ball.nextPos.x = 400;
	ball.nextPos.y = WINDOW_HEIGHT - 100;
}




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

	stage1 = LoadGraph("Stege1.png");

	// プレイヤーの初期設定
	player.pos.x = POS_X;
	player.pos.y = POS_Y;

	ball.Init();


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

		// 角度の制限（真横や下には撃てないようにする）
		if (angle < 0.2) angle = 0.2;
		if (angle > 3.141592 - 0.2) angle = 3.141592 - 0.2;

		// ボールを跳ばす処理
		// スペースキーを押したら時、ボールの状態がまだ跳んでいない状態なら
		// ボールを飛ばしていい状態にする
		if (pushHitKey(KEY_INPUT_SPACE) && ball.mode != true)
		{
			ball.mode = true;
			ball.vx = cos(angle) * BALL_SPEED;
			ball.vy = -sin(angle) * BALL_SPEED;
		}
		// ボールを飛ばしたら、プレイヤーの向いている方向に球を出す
		if (ball.mode == true)
		{

			ball.pos.x += ball.vx;
			// マイナス値の理由、プラス値だと、反対に跳び下方向に跳ぶためマイナス値で、上に上がるため。
			ball.pos.y += ball.vy;

			double ReftWall = 343;
			double RightWall = ReftWall + (8 * 54) - 27;
			if (ball.pos.x <= ReftWall || ball.pos.x >= RightWall)
			{
				ball.vx *= -1;
			}
		}

		// ballが飛んでいる状態なら
		if (ball.mode)
		{
			// ボールが天井または、隣接するボールに当たった時移動を停止させる。
			// 天井に当たったら
			if (ball.pos.y <= 104) {
				// モードを切り替える
				ball.mode = false;
				int r, c;
				if (GetGridIndex(ball.pos.x, ball.pos.y, r, c)) {
					grid[r][c] = 1; // ボールを置く
				}
			}
			// ボールに当たったら
			for (int c = 0; c < 7; c++)
			{
				int rowMax = (c % 2 == 1) ? 7 : 8;

				for (int r = 0; r < rowMax; r++)
				{
					if (grid[r][c] == 1)
					{
						// グリッド上のボールの座標を計算
						float gx = 343 + r * 54 + ((c % 2 == 1) ? 27 : 0);
						float gy = 104 + c * 50;

						float dx = ball.pos.x - gx;
						float dy = ball.pos.y - gy;
						float dist = sqrt(dx * dx + dy * dy);

						if (dist <= 54) // 直径分
						{
							ball.mode = false;

							int nr, nc;
							if (GetGridIndex(ball.pos.x, ball.pos.y, nr, nc)) {
								grid[nr][nc] = 1;
							}
							break;
						}
					}
				}
			}

		}

		// ボールが停止したら
		if (!ball.mode)
		{
			// 次のボールを発射位置に移す
			ball.pos.x = ball.nextPos.x;
			ball.pos.y = ball.nextPos.y;

			// 新しい次のボールを準備
			ball.Nextball();
		}

		//---------------------------------------
		// 描画
		// ↓ 画面消去 ↓
		clsDx();
		ClearDrawScreen();
		// ↑ 画面消去 ↑
		//---------------------------------------

		DrawGraph(0, 0, stage1, TRUE);

		// 縦列
		for (int col = 0; col < 7; col++)
		{
			// 偶数列なら横の数を1つ減らす
			// 二列目なら７個１列目なら８個並べる
			int rowMax = (col % 2 == 1) ? 7 : 8;

			// 横列
			for (int row = 0; row < rowMax; row++)
			{
				// X座標に54ずつ並べる
				float x = row * 54;
				// 縦列が偶数列なった場合
				if (col % 2 == 1)
				{
					// 円のX座標を右にずらす
					x += 54 / 2;
				}
				// Y座標に50ずつ並べる
				float y = col * 50;
				// 円の描画
				DrawCircle(x + 343, y + 104, 27, GetColor(255, 255, 255), FALSE);
			}
		}

		// 発射するボールの描画
		DrawCircle(ball.pos.x, ball.pos.y, 26, GetColor(255, 0, 0), TRUE);
		// 次のボールの描画
		DrawCircle(ball.nextPos.x, ball.nextPos.y, 26, GetColor(0, 255, 0), TRUE);


		DrawLine(player.pos.x, player.pos.y, player.pos.x + (int)(cos(angle) * 50), player.pos.y - (int)(sin(angle) * 50), GetColor(255, 255, 255));



		ScreenFlip();
	}


	DxLib_End();

	return 0;
}