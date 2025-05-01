#pragma once
#include "Scene.h"
#include <vector>
#include <stack>
#include "ChessObj.h"
#include "Record.h"
#include <tchar.h>

class GameScene : public Scene
{
public:
	GameScene();
	void draw() override;
	void drawUi();
	void update() override;
	void onButtonClicked(int id);

	/**
	 * 根据(鼠标)坐标获取棋子对象.
	 * 
	 * \param pos 坐标，一般是鼠标坐标
	 * \return 获取成功返回ChessObj 否则返回nullptr
	 */
	ChessObj* getChessByPoint(const Point& pos)const;
	ChessObj* getChessByIndex(const Point& idx)const;

	/*
	* 结束当前回合
	*/
	void endCurrentTurn();

	/**
	 * 保存棋谱.
	 */
	void saveChessManual(const std::string& path);

	/**
	 * 加载棋谱.
	 */
	void loadChessManual(const std::string& path);
private:
	IMAGE* m_chessBoardImg{};
	IMAGE* m_selectedImg{};
	std::vector<std::unique_ptr<ChessObj>> m_chess;		//所有棋子
	ChessObj* m_selectedChess{};						//当前选择的棋子
	ChessObj::Player m_currentPlayer{ ChessObj::Red };	//当前棋手
	ChessObj::Player m_blockedPlayer = ChessObj::None; // 新增禁行方状态
	std::stack<std::unique_ptr<Record>> m_records;
	int m_stepCount = 0;
};