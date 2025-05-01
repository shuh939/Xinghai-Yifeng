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
	 * ����(���)�����ȡ���Ӷ���.
	 * 
	 * \param pos ���꣬һ�����������
	 * \return ��ȡ�ɹ�����ChessObj ���򷵻�nullptr
	 */
	ChessObj* getChessByPoint(const Point& pos)const;
	ChessObj* getChessByIndex(const Point& idx)const;

	/*
	* ������ǰ�غ�
	*/
	void endCurrentTurn();

	/**
	 * ��������.
	 */
	void saveChessManual(const std::string& path);

	/**
	 * ��������.
	 */
	void loadChessManual(const std::string& path);
private:
	IMAGE* m_chessBoardImg{};
	IMAGE* m_selectedImg{};
	std::vector<std::unique_ptr<ChessObj>> m_chess;		//��������
	ChessObj* m_selectedChess{};						//��ǰѡ�������
	ChessObj::Player m_currentPlayer{ ChessObj::Red };	//��ǰ����
	ChessObj::Player m_blockedPlayer = ChessObj::None; // �������з�״̬
	std::stack<std::unique_ptr<Record>> m_records;
	int m_stepCount = 0;
};