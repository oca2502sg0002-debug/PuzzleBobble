#ifndef __CONST_H__
#define __CONST_H__

// 画面のサイズ
const int WINDOW_WIDTH = 1069;
const int WINDOW_HEIGHT = 800;

const int MAX_ROW = 8;  // 横
const int MAX_COL = 11;  // 縦

class Float2
{
public:
	float x;
	float y;
};

//-----------------------------------------------------------------
// Ballの設定
//-----------------------------------------------------------------
class Ball
{
public:
	Float2 pos;	// X座標,Y座標
	bool mode;	// 飛んでいる状態
	float vx;
	float vy;
	int color;	// 現在のボールの色 (1～4)
	int nextColor;	// 次のボールの色 (1～4)

	void Init();		// 初期設定
	void Nextball();	// 次にセットするボール
};
Ball ball;

//-----------------------------------------------------------------
// 大砲の設定
//-----------------------------------------------------------------
class Player
{
public:
	Float2 pos;	// X座標, Y座標
	float rot;	// 角度
};
Player player;


// ボールの座標からグリッド座標を計算する
inline bool GetGridIndex(float x, float y, int& outRow, int& outCol)
{
	// グリッドの左上基準
	float baseX = 343.0f;
	float baseY = 104.0f;

	// 仮の列計算
	int col = (int)((y - baseY + 25.0f) / 50.0f); // 四捨五入気味に補正
	if (col < 0 || col >= MAX_COL) return false;

	// 偶数列と奇数列でXの基準が違う
	float offsetX = (col % 2 == 1) ? 54.0f / 2.0f : 0.0f;

	int row = (int)((x - baseX - offsetX + 27.0f) / 54.0f);

	// 列によって最大数が違う
	int rowMax = (col % 2 == 1) ? 7 : 8;
	if (row < 0 || row >= rowMax) return false;

	outRow = row;
	outCol = col;
	return true;
}

#endif
