#include "GameScene.h"
#include "ResManager.h"
#include "Geometry.h"
#include "Application.h"
#include "gui/Gui.h"
#include <array>
#include <fstream>
#include <random>
#include <stdlib.h>
#include "ResultScene.h"
#pragma comment(lib,"Winmm.lib")

GameScene::GameScene()
	: m_chessBoardImg(ResManager::instance()->cacheImage
	("chessBoard", "Resource/images/chessBoard.png"))
	, m_selectedImg(ResManager::instance()->cacheImage
	("selected", "Resource/images/selected.png"))
	, m_stepCount(0)  // ��ʼ������
{
	ResManager::instance()->setPrefix("Resource/images");

	//��������
	//const int MARGIN = 50;		//�߾�
	//const int SIZE = 80;		//���Ӵ�С
	//const int ROWS = 10;
	//const int COLS = 9;

	//����ÿ�����ӵ�����
	std::array<const char*, 11> names
	{"bing","bing","bing","king","bing","bing","bing",
	"ma","ma","ma","che"};
	//����ÿ�����ӵ�����
	std::array<Point, 11> indexes{ 
		Point{0,0},{1,0},{2,0},
		{4,0},{6,0},{7,0},{8,0},
		{1,2},{4,3},{7,2},{4,1}
	};
	//��������
	//����
	for (int i = 0; i < names.size(); i++) { 
		m_chess.emplace_back(std::make_unique
			<ChessObj>(ChessObj::Red, 
				indexes[i], names[i], this));

	}
	//����
	for (int i = 0; i < names.size(); i++) {
		auto& ptr = m_chess.emplace_back
		(std::make_unique<ChessObj>
			(ChessObj::Black, indexes[i],
				names[i], this));
		ptr->reverse();
	}
}

void GameScene::draw()
{
	//��������
	drawImage(0, 0, m_chessBoardImg);
	//��������
	for (auto& c : m_chess) {
		c->draw();
	}
	//����ѡ��״̬
	if (m_selectedChess) {
		drawImage(static_cast<int>(m_selectedChess->x), 
			static_cast<int>(m_selectedChess->y), m_selectedImg);
	}
	//����UI
	drawUi();
}

void GameScene::drawUi()
{
	//����ķ�����
	SetFont(46, "����");
	GUI_LabelFormat({ sApp->width() - 220,0,200,150 }, 
		"%s������", m_currentPlayer == ChessObj::Red 
		? "��" : "��");

	//���ư�ť
	SetFont(18, "����");
	static const char* texts[]
	{ "����","���","����","��������","��������" };
	static int size = sizeof(texts) / sizeof(texts[0]);
	static int btnW = 150;
	static int btnH = 40;
	static int totalH = size * btnH;
	
	for (int i = 0; i < size; i++) {
		if (GUI_TextButton(GUID + i, 
			{ sApp->width() - 180,(sApp->height() - totalH)
			/ 2 + i * btnH ,btnW,btnH }, texts[i])) {
			//printf("id:%d\n",i);
			onButtonClicked(i);
		}
	}
	SetFont(18, "����");              // �������壨�߶�24���أ�
	settextcolor(RGB(255, 0, 0));             // ����������ɫ��ǳ��ɫ��
	setbkmode(TRANSPARENT);             // ͸������ģʽ

	char stepText[100];
	sprintf_s(stepText, "��̫��������: %d", 10-m_stepCount);

	// ʹ��GUI_Label�������textwidth��outtextxy
	GUI_Label({sApp->width() - 225, 110, 180, 30}, stepText, RGB(255, 0, 0), AlignRight);
}

void GameScene::update()
{
	for (auto& c : m_chess) {
		c->update();
	}

	//��ȡ��Ϣ
	auto msg = sApp->msg();
	//����������
	if (msg->message == WM_LBUTTONDOWN)
	{
		auto chess = getChessByPoint({ msg->x, msg->y });
		//if(chess)
			//printf("index(%d,%d) pos(%f,%f)\n",
			// chess->index.x, chess->index.y, 
			// chess->x, chess->y);
		//��һ�ε������
		if (!m_selectedChess) {
			//���ӱ������,���ұ���ѡ���Լ�������
			if (chess && chess->player == m_currentPlayer) {
				m_selectedChess = chess;
			}
		}
		//�ڶ��ε�����ӣ��ж�������ѡ���ƶ����ǳ�
		else
		{
			//���λ��������	
			if (chess) {
				
				//�����ѡ�е����ӣ���ȡ��ѡ��
				if (chess == m_selectedChess) {
					m_selectedChess = nullptr;
				}
				//���ҵ�����Ǽ���������,����ѡ��
				else if (chess->player == 
					m_selectedChess->player) {
					m_selectedChess = chess;
				}
				//��
				else {
					auto idx =  ChessObj::
						pointToIndex({ msg->x,msg->y });
					if (m_selectedChess->isAbleMove(idx)) {
						printf("eat %s!\n",chess->name.data());
						//����Ϣ���뵽����ջ��
						m_records.emplace(new Record
						(m_selectedChess, m_selectedChess->
							index, idx,chess));
			
						//�ƶ�����
						m_selectedChess->index = chess->index;
						//�Ƴ����Ե�������
						auto it = std::remove_if(m_chess.begin(),
							m_chess.end(), [=](auto& ptr)->bool
							{
								if (ptr.get() == chess) {
									
									ptr.release();
									return true;
								}
								return false;	
							});
						m_chess.erase(it,m_chess.end());
						//������ǰ�غ�
						endCurrentTurn();
						m_stepCount++;
						m_stepCount %= 10;
						if (m_stepCount == 0)
						{
							std::random_device rd;

							// 2. ѡ����������棨ʾ����÷ɭ��ת�㷨��
							std::mt19937 gen(rd());

							// 3. ����ֲ���Χ��ʾ�������ȷֲ���������
							std::uniform_int_distribution<> dis(1, 2); // 1~6

							// ���������
							int diceRoll = dis(gen); // ����������
							if (diceRoll == 1)
							{
								endCurrentTurn();
							}
						}
					}
				}
			}
			//���λ��û������:�ƶ�
			else
			{
				auto idx = ChessObj::pointToIndex({ msg->x,msg->y });
				if (m_selectedChess->isAbleMove(idx)) {
					//����Ϣ���뵽����ջ��
					m_records.emplace(new Record(m_selectedChess,m_selectedChess->index,idx));
					//�ƶ�����
					m_selectedChess->index = idx;
					printf("move %s!\n",m_selectedChess->name.data());
					//������ǰ�غ�
					endCurrentTurn();
					m_stepCount++;
					m_stepCount %= 10;
					if (m_stepCount == 0)
					{
  // ��Ҫ������ͷ�ļ�

						// 1. ��������豸����ȡ����������ӣ�
						std::random_device rd;

						// 2. ѡ����������棨ʾ����÷ɭ��ת�㷨��
						std::mt19937 gen(rd());

						// 3. ����ֲ���Χ��ʾ�������ȷֲ���������
						std::uniform_int_distribution<> dis(1, 2); // 1~6

						// ���������
						int diceRoll = dis(gen); // ����������
						if (diceRoll ==1 )
						{
							endCurrentTurn();
						}
					}
				}
			}
		}
	}
}

void GameScene::onButtonClicked(int id)
{
	//����
	if (id == 0) {
		//����ջ�ǿ�
		if (!m_records.empty())
		{
			auto& record = m_records.top();
			if(m_stepCount>0)
				m_stepCount--;
			m_stepCount %= 10;
			record->chess->index = record->start;
			//����б��Ե�������
			if (record->eatedChess) {
				m_chess.emplace_back(record->eatedChess);
			}
			m_records.pop();
			if (m_stepCount == 0)
			{
				std::random_device rd;

				// 2. ѡ����������棨ʾ����÷ɭ��ת�㷨��
				std::mt19937 gen(rd());

				// 3. ����ֲ���Χ��ʾ�������ȷֲ���������
				std::uniform_int_distribution<> dis(1, 2); // 1~6

				// ���������
				int diceRoll = dis(gen); // ����������
				if (diceRoll == 1)
				{
					endCurrentTurn();
				}
			}
		}
	}
	else if (id == 1) {

	}
	// ����
	else if (id == 2) {
		// ��ǰ�Ǻ췽�غϣ���ڷ���ʤ����ǰ�Ǻڷ��غϣ���췽��ʤ
		ResultScene::ResultType resultType = 
			(m_currentPlayer == ChessObj::Red) ? 
			ResultScene::BlackWin : ResultScene::RedWin;
		
		// ��ת������ҳ��
		sApp->runScene(new ResultScene(resultType));
	}
	//��������
	else if (id == 3) {
		saveChessManual("chessManual.txt");
	}
	//��������
	else if (id == 4) {
		loadChessManual("chessManual.txt");
	}
}

ChessObj* GameScene::getChessByPoint(const Point& pos) const
{
	return getChessByIndex(ChessObj::pointToIndex(pos));
}

ChessObj* GameScene::getChessByIndex(const Point& idx) const
{
	auto it = std::find_if(m_chess.begin(), m_chess.end(), [=](auto& ptr)
		{
			return ptr->index == idx;
		});
	if (it != m_chess.end())
		return it->get();
	return nullptr;
}

void GameScene::endCurrentTurn()
{
	m_currentPlayer = m_currentPlayer == ChessObj::Red ? ChessObj::Black : ChessObj::Red;
	m_selectedChess = nullptr;
}

void GameScene::saveChessManual(const std::string& path)
{
	std::ofstream outFile(path, std::ios::ate);
	if (!outFile) {
		printf("oepn %s failed\n", path.data());
		return;
	}

	for (auto& c : m_chess) {
		outFile << c->toString() << "\n";
	}
}

void GameScene::loadChessManual(const std::string& path)
{
	std::ifstream inFile(path);
	if (!inFile) {
		printf("oepn %s failed\n", path.data());
		return;
	}
	//�����������
	m_chess.clear();
	//����������
	std::string str;
	while (std::getline(inFile, str))
	{
		auto chess = ChessObj::fromString(str, this);
		m_chess.emplace_back(chess);
	}
}

