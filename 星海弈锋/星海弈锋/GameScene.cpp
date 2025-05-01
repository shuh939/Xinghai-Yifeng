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
	, m_stepCount(0)  // 初始化步数
{
	ResManager::instance()->setPrefix("Resource/images");

	//绘制棋子
	//const int MARGIN = 50;		//边距
	//const int SIZE = 80;		//格子大小
	//const int ROWS = 10;
	//const int COLS = 9;

	//上面每个棋子的名称
	std::array<const char*, 11> names
	{"bing","bing","bing","king","bing","bing","bing",
	"ma","ma","ma","che"};
	//上面每个棋子的索引
	std::array<Point, 11> indexes{ 
		Point{0,0},{1,0},{2,0},
		{4,0},{6,0},{7,0},{8,0},
		{1,2},{4,3},{7,2},{4,1}
	};
	//创建棋子
	//上面
	for (int i = 0; i < names.size(); i++) { 
		m_chess.emplace_back(std::make_unique
			<ChessObj>(ChessObj::Red, 
				indexes[i], names[i], this));

	}
	//下面
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
	//绘制棋盘
	drawImage(0, 0, m_chessBoardImg);
	//绘制棋子
	for (auto& c : m_chess) {
		c->draw();
	}
	//绘制选中状态
	if (m_selectedChess) {
		drawImage(static_cast<int>(m_selectedChess->x), 
			static_cast<int>(m_selectedChess->y), m_selectedImg);
	}
	//绘制UI
	drawUi();
}

void GameScene::drawUi()
{
	//输出哪方行棋
	SetFont(46, "黑体");
	GUI_LabelFormat({ sApp->width() - 220,0,200,150 }, 
		"%s方行棋", m_currentPlayer == ChessObj::Red 
		? "红" : "黑");

	//绘制按钮
	SetFont(18, "黑体");
	static const char* texts[]
	{ "悔棋","求和","认输","保存棋谱","导入棋谱" };
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
	SetFont(18, "黑体");              // 设置字体（高度24像素）
	settextcolor(RGB(255, 0, 0));             // 设置文字颜色（浅蓝色）
	setbkmode(TRANSPARENT);             // 透明背景模式

	char stepText[100];
	sprintf_s(stepText, "以太哀嚎来临: %d", 10-m_stepCount);

	// 使用GUI_Label函数替代textwidth和outtextxy
	GUI_Label({sApp->width() - 225, 110, 180, 30}, stepText, RGB(255, 0, 0), AlignRight);
}

void GameScene::update()
{
	for (auto& c : m_chess) {
		c->update();
	}

	//获取消息
	auto msg = sApp->msg();
	//鼠标左键弹起
	if (msg->message == WM_LBUTTONDOWN)
	{
		auto chess = getChessByPoint({ msg->x, msg->y });
		//if(chess)
			//printf("index(%d,%d) pos(%f,%f)\n",
			// chess->index.x, chess->index.y, 
			// chess->x, chess->y);
		//第一次点击棋子
		if (!m_selectedChess) {
			//棋子必须存在,而且必须选中自己的棋子
			if (chess && chess->player == m_currentPlayer) {
				m_selectedChess = chess;
			}
		}
		//第二次点击棋子，判断是重新选择、移动还是吃
		else
		{
			//点击位置有棋子	
			if (chess) {
				
				//点击了选中的棋子，则取消选中
				if (chess == m_selectedChess) {
					m_selectedChess = nullptr;
				}
				//而且点击的是己方的棋子,重新选择
				else if (chess->player == 
					m_selectedChess->player) {
					m_selectedChess = chess;
				}
				//吃
				else {
					auto idx =  ChessObj::
						pointToIndex({ msg->x,msg->y });
					if (m_selectedChess->isAbleMove(idx)) {
						printf("eat %s!\n",chess->name.data());
						//把信息加入到回退栈中
						m_records.emplace(new Record
						(m_selectedChess, m_selectedChess->
							index, idx,chess));
			
						//移动棋子
						m_selectedChess->index = chess->index;
						//移除被吃掉的棋子
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
						//结束当前回合
						endCurrentTurn();
						m_stepCount++;
						m_stepCount %= 10;
						if (m_stepCount == 0)
						{
							std::random_device rd;

							// 2. 选择随机数引擎（示例：梅森旋转算法）
							std::mt19937 gen(rd());

							// 3. 定义分布范围（示例：均匀分布的整数）
							std::uniform_int_distribution<> dis(1, 2); // 1~6

							// 生成随机数
							int diceRoll = dis(gen); // 类似掷骰子
							if (diceRoll == 1)
							{
								endCurrentTurn();
							}
						}
					}
				}
			}
			//点击位置没有棋子:移动
			else
			{
				auto idx = ChessObj::pointToIndex({ msg->x,msg->y });
				if (m_selectedChess->isAbleMove(idx)) {
					//把信息加入到回退栈中
					m_records.emplace(new Record(m_selectedChess,m_selectedChess->index,idx));
					//移动棋子
					m_selectedChess->index = idx;
					printf("move %s!\n",m_selectedChess->name.data());
					//结束当前回合
					endCurrentTurn();
					m_stepCount++;
					m_stepCount %= 10;
					if (m_stepCount == 0)
					{
  // 需要包含的头文件

						// 1. 创建随机设备（获取真随机数种子）
						std::random_device rd;

						// 2. 选择随机数引擎（示例：梅森旋转算法）
						std::mt19937 gen(rd());

						// 3. 定义分布范围（示例：均匀分布的整数）
						std::uniform_int_distribution<> dis(1, 2); // 1~6

						// 生成随机数
						int diceRoll = dis(gen); // 类似掷骰子
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
	//悔棋
	if (id == 0) {
		//回退栈非空
		if (!m_records.empty())
		{
			auto& record = m_records.top();
			if(m_stepCount>0)
				m_stepCount--;
			m_stepCount %= 10;
			record->chess->index = record->start;
			//如果有被吃掉的棋子
			if (record->eatedChess) {
				m_chess.emplace_back(record->eatedChess);
			}
			m_records.pop();
			if (m_stepCount == 0)
			{
				std::random_device rd;

				// 2. 选择随机数引擎（示例：梅森旋转算法）
				std::mt19937 gen(rd());

				// 3. 定义分布范围（示例：均匀分布的整数）
				std::uniform_int_distribution<> dis(1, 2); // 1~6

				// 生成随机数
				int diceRoll = dis(gen); // 类似掷骰子
				if (diceRoll == 1)
				{
					endCurrentTurn();
				}
			}
		}
	}
	else if (id == 1) {

	}
	// 认输
	else if (id == 2) {
		// 当前是红方回合，则黑方获胜；当前是黑方回合，则红方获胜
		ResultScene::ResultType resultType = 
			(m_currentPlayer == ChessObj::Red) ? 
			ResultScene::BlackWin : ResultScene::RedWin;
		
		// 跳转到结算页面
		sApp->runScene(new ResultScene(resultType));
	}
	//保存棋谱
	else if (id == 3) {
		saveChessManual("chessManual.txt");
	}
	//加载棋谱
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
	//清空所以棋子
	m_chess.clear();
	//加载新棋子
	std::string str;
	while (std::getline(inFile, str))
	{
		auto chess = ChessObj::fromString(str, this);
		m_chess.emplace_back(chess);
	}
}

