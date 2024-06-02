#include "CrChessPlay.h"

#include<algorithm>
#include <cassert>
#include <numeric>
#include <sstream>
#include<vector>
#include<iostream>
using namespace std;

#include "../FEN//CrFenHelper.h"

CChessPlay::CChessPlay(void) 
:m_isAIGo(false)//开局总是人先走，计算机后走
, m_currentZobristKey(0)
{
    InitLog();
	//构造计算机的棋子(大号棋子)
	g_global.g_chessman.assign(48,nullptr);
	//构造两个车
	g_global.g_chessman[39] = new Chariot(1, 39, 90, Coordinate(3, 3));
	g_global.g_chessman[40] = new Chariot(1, 40, 90, Coordinate(3, 11));
	g_global.g_chessmanValue[39] = g_global.g_chessmanValue[40] = 90;
	//构造两个马
	g_global.g_chessman[37] = new Horse(1, 37, 40, Coordinate(3, 4));
	g_global.g_chessman[38] = new Horse(1, 38, 40, Coordinate(3, 10));
	g_global.g_chessmanValue[37] = g_global.g_chessmanValue[38] = 40;
	//构造两个炮
	g_global.g_chessman[41] = new Cannon(1, 41, 45, Coordinate(5, 4));
	g_global.g_chessman[42] = new Cannon(1, 42, 45, Coordinate(5, 10));
	g_global.g_chessmanValue[41] = g_global.g_chessmanValue[42] = 45;
	//构造将
	g_global.g_chessman[32] = new King(1, 32, 1000, Coordinate(3, 7));
	g_global.g_chessmanValue[32] = 1000;
	//构造两个士
	g_global.g_chessman[33] = new Official(1, 33, 20, Coordinate(3, 6));
	g_global.g_chessman[34] = new Official(1, 34, 20, Coordinate(3, 8));
	g_global.g_chessmanValue[33] = g_global.g_chessmanValue[34] = 20;
	//构造两个相
	g_global.g_chessman[35] = new Minister(1, 35, 20, Coordinate(3, 5));
	g_global.g_chessman[36] = new Minister(1, 36, 20, Coordinate(3, 9));
	g_global.g_chessmanValue[35] = g_global.g_chessmanValue[36] = 20;
	//构造五个兵
	g_global.g_chessman[43] = new Soldier(1, 43, 8, Coordinate(6, 3));
	g_global.g_chessman[44] = new Soldier(1, 44, 15, Coordinate(6, 5));
	g_global.g_chessman[45] = new Soldier(1, 45, 20, Coordinate(6, 7));//中兵
	g_global.g_chessman[46] = new Soldier(1, 46, 15, Coordinate(6, 9));
	g_global.g_chessman[47] = new Soldier(1, 47, 8, Coordinate(6, 11));
	g_global.g_chessmanValue[43] = g_global.g_chessmanValue[44] = g_global.g_chessmanValue[45] = g_global.g_chessmanValue[46] = g_global.g_chessmanValue[47] = 10;

	//构造小号棋子，在棋盘下方
	//构造将
	g_global.g_chessman[16] = new King(0, 16, 1000, Coordinate(12, 7));
	g_global.g_chessmanValue[16] = 1000;
	//构造两个士
	g_global.g_chessman[17] = new Official(0, 17, 20, Coordinate(12, 6));
	g_global.g_chessman[18] = new Official(0, 18, 20, Coordinate(12, 8));
	g_global.g_chessmanValue[17] = g_global.g_chessmanValue[18] = 20;
	//构造两个相
	g_global.g_chessman[19] = new Minister(0, 19, 20, Coordinate(12, 5));
	g_global.g_chessman[20] = new Minister(0, 20, 20, Coordinate(12, 9));
	g_global.g_chessmanValue[19] = g_global.g_chessmanValue[20] = 20;
	//构造两个马
	g_global.g_chessman[21] = new Horse(0, 21, 40, Coordinate(12, 4));
	g_global.g_chessman[22] = new Horse(0, 22, 40, Coordinate(12, 10));
	g_global.g_chessmanValue[21] = g_global.g_chessmanValue[22] = 40;
	//构造两个车
	g_global.g_chessman[23] = new Chariot(0, 23, 90, Coordinate(12, 3));
	g_global.g_chessman[24] = new Chariot(0, 24, 90, Coordinate(12, 11));
	g_global.g_chessmanValue[23] = g_global.g_chessmanValue[24] = 90;
	//构造两个炮
	g_global.g_chessman[25] = new Cannon(0, 25, 45, Coordinate(10, 4));
	g_global.g_chessman[26] = new Cannon(0, 26, 45, Coordinate(10, 10));
	g_global.g_chessmanValue[25] = g_global.g_chessmanValue[26] = 45;
	//构造五个兵
	g_global.g_chessman[27] = new Soldier(0, 27, 8, Coordinate(9, 3));
	g_global.g_chessman[28] = new Soldier(0, 28, 15, Coordinate(9, 5));
	g_global.g_chessman[29] = new Soldier(0, 29, 20, Coordinate(9, 7));
	g_global.g_chessman[30] = new Soldier(0, 30, 15, Coordinate(9, 9));
	g_global.g_chessman[31] = new Soldier(0, 31, 8, Coordinate(9, 11));
	g_global.g_chessmanValue[27] = g_global.g_chessmanValue[28] = g_global.g_chessmanValue[29] = g_global.g_chessmanValue[30] = g_global.g_chessmanValue[31] = 10;
}
CChessPlay::~CChessPlay(void)
{
    CloseLog();

	for (vector<ChessBase*>::size_type i = 0; i < g_global.g_chessman.size(); ++i)
	{
		auto& pItr = g_global.g_chessman[i];
		if (pItr != nullptr)
		{
			delete pItr;
		}
	}
}
void CChessPlay::InitChessPlay(void)
{
	m_moveHistory.clear();//清空历史走法
	//构造两个车
	g_global.g_chessman[39]->SetLocation(Coordinate(3,3));
	g_global.g_chessman[40]->SetLocation(Coordinate(3,11));
//	g_global.g_chessmanName[39] = g_global.g_chessmanName[40] = "車";
	//构造两个马
	g_global.g_chessman[37]->SetLocation(Coordinate(3,4));
	g_global.g_chessman[38]->SetLocation(Coordinate(3,10));
//	g_global.g_chessmanName[37] = g_global.g_chessmanName[38] = "馬";
	////构造两个炮
	g_global.g_chessman[41]->SetLocation(Coordinate(5,4));
	g_global.g_chessman[42]->SetLocation(Coordinate(5,10));
//	g_global.g_chessmanName[41] = g_global.g_chessmanName[42] = "砲";
	//构造将
	g_global.g_chessman[32]->SetLocation(Coordinate(3,7));
//	g_global.g_chessmanName[32] = "帥";
	//构造两个士
	g_global.g_chessman[33]->SetLocation(Coordinate(3,6));
	g_global.g_chessman[34]->SetLocation(Coordinate(3,8));
//	g_global.g_chessmanName[33] = g_global.g_chessmanName[34] = "仕";
	//构造两个相
	g_global.g_chessman[35]->SetLocation(Coordinate(3,5));
	g_global.g_chessman[36]->SetLocation(Coordinate(3,9));
//	g_global.g_chessmanName[35] = g_global.g_chessmanName[36] = "相";
	//构造五个兵
	g_global.g_chessman[43]->SetLocation(Coordinate(6,3 ));
	g_global.g_chessman[44]->SetLocation(Coordinate(6,5 ));
	g_global.g_chessman[45]->SetLocation(Coordinate(6,7 ));
	g_global.g_chessman[46]->SetLocation(Coordinate(6,9 ));
	g_global.g_chessman[47]->SetLocation(Coordinate(6,11));
//	g_global.g_chessmanName[43] = g_global.g_chessmanName[44]
//		= g_global.g_chessmanName[45] = g_global.g_chessmanName[46] = g_global.g_chessmanName[47] = "兵";

	//构造小号棋子，在棋盘下方
	//构造将
	g_global.g_chessman[16]->SetLocation(Coordinate(12,7));
//	g_global.g_chessmanName[16] = "将";
	//构造两个士
	g_global.g_chessman[17]->SetLocation(Coordinate(12,6));
	g_global.g_chessman[18]->SetLocation(Coordinate(12,8));
//	g_global.g_chessmanName[17] = g_global.g_chessmanName[18] = "士";
	//构造两个相
	g_global.g_chessman[19]->SetLocation(Coordinate(12,5));
	g_global.g_chessman[20]->SetLocation(Coordinate(12,9));
//	g_global.g_chessmanName[19] = g_global.g_chessmanName[20] = "象";
	//构造两个马
	g_global.g_chessman[21]->SetLocation(Coordinate(12,4));
	g_global.g_chessman[22]->SetLocation(Coordinate(12,10));
//	g_global.g_chessmanName[21] = g_global.g_chessmanName[22] = "马";
	//构造两个车
	g_global.g_chessman[23]->SetLocation(Coordinate(12,3));
	g_global.g_chessman[24]->SetLocation(Coordinate(12,11));
//	g_global.g_chessmanName[23] = g_global.g_chessmanName[24] = "车";
	//构造两个炮
	g_global.g_chessman[25]->SetLocation(Coordinate(10,4));
	g_global.g_chessman[26]->SetLocation(Coordinate(10,10));
//	g_global.g_chessmanName[25] = g_global.g_chessmanName[26] = "炮";
	////构造五个兵
	g_global.g_chessman[27]->SetLocation(Coordinate(9,3 ));
	g_global.g_chessman[28]->SetLocation(Coordinate(9,5 ));
	g_global.g_chessman[29]->SetLocation(Coordinate(9,7 ));
	g_global.g_chessman[30]->SetLocation(Coordinate(9,9 ));
	g_global.g_chessman[31]->SetLocation(Coordinate(9,11));
//	g_global.g_chessmanName[27] = g_global.g_chessmanName[28]
//		= g_global.g_chessmanName[29] = g_global.g_chessmanName[30] = g_global.g_chessmanName[31] = "卒";

	//初始化bigManIds,smallManIds
	g_global.InitChessManIDArray();
    //构造棋盘数组
    //All zeros in g_global.g_chessboard[i][j] = id at begin
    g_global.g_chessboard.clear();
    vector<int> tempLine(16,0);
    for(int i=0;i<16;++i)
	{
        g_global.g_chessboard.push_back(tempLine);
	}
	//往棋盘上放置棋子
    for(int i=16;i<=47;i++)
    {    
		int x = g_global.g_chessman[i]->location().x;
		int y = g_global.g_chessman[i]->location().y;
		g_global.g_chessboard[x][y]=i;
		g_global.g_chessman[i]->SetAlive(true);
	}

	g_evaluate.Clear();
	//初始化合法位置
    ChessBase::InitLegalPosition();
	//初始化Zobrist相关
	InitZobrist();
}

void CChessPlay::InitZobrist(void)
{
	//构造Zobrist随机数组，为每个棋子的每个坐标位置都对应一个随机数
	for (size_t i = 0; i < 48; ++i)
	{
		for (size_t j = 0; j < 13; ++j)
		{
			for (size_t k = 0; k < 12; ++k)
			{
				g_global.g_zobristMatrix[i][j][k] = g_global.g_randomer();
			}
		}
	}
	//初始化开局的ZobristKey
	m_currentZobristKey ^= m_isAIGo ? 1 : 0;
	for (size_t i = 16; i <= 47; ++i)
	{
		if (g_global.g_chessman[i]->GetAlive())
		{
			m_currentZobristKey ^= g_global.g_zobristMatrix[i][g_global.g_chessman[i]->location().x][g_global.g_chessman[i]->location().y];
		}
    }
}

void CChessPlay::InitLog()
{
    m_fout.open("log.txt");
    assert(m_fout.is_open());
}

void CChessPlay::CloseLog()
{
    m_fout.close();
}

void CChessPlay::ClearChessMan(void)
{
	for (size_t i = 16; i <= 47; ++i)
	{
		g_global.g_chessman[i]->SetAlive(false);
	}
}
void CChessPlay::ClearChessBoard(void)
{
	for (size_t i =3;i <= 12;++i)
	{
		for (size_t j=3;j <= 11;++j)
		{
			g_global.g_chessboard[i][j] = 0;
		}
	}
}
IdType CChessPlay::ChessBoard(const Coordinate& p) const
{
    return g_global.g_chessboard[p.x][p.y];
}
void CChessPlay::SetChessBoard(const Coordinate& p, int _id)
{
	if (g_global.g_chessboard[p.x][p.y] != 0)
	{
		int oldId = g_global.g_chessboard[p.x][p.y];
		g_global.g_chessman[oldId]->SetAlive(false);
	}
	g_global.g_chessboard[p.x][p.y] = _id;
	g_global.g_chessman[_id]->SetAlive(true);
	g_global.g_chessman[_id]->SetLocation(p);
}
bool CChessPlay::BeKilled(bool _aiUseRed)
{
	moves m; 
	_aiUseRed ? GetMoveSmallMan(m) : GetMoveBigMan(m);
	if (m.size() == 0)
	{
		return true;
	}
	return false;
}

//生成小号棋子的全部走法
void CChessPlay::GetMoveSmallMan(moves& m)
{
	for (vector<ChessBase*>::size_type i = 16; i <= 31; ++i)
	{
		if (g_global.g_chessman[i] != (ChessBase*)0 && g_global.g_chessman[i]->GetAlive())
		{
			g_global.g_chessman[i]->GetMove(g_global.g_chessboard, m);
		}
	}
}
//生成(电脑的)大号棋子的全部走法
void CChessPlay::GetMoveBigMan(moves& m)
{
    for(vector<ChessBase*>::size_type i=32;i<=47;++i)
    {
        if(g_global.g_chessman[i]!=(ChessBase*)0 && g_global.g_chessman[i]->GetAlive())
	    {
		    g_global.g_chessman[i]->GetMove(g_global.g_chessboard,m);
	    }
    }
}
void CChessPlay::GetMoveOneMan(int id, moves& m) const
{
	g_global.g_chessman[id]->GetMove(g_global.g_chessboard,m);
}

vector<Coordinate> CChessPlay::AliveMan(void)
{
  vector<Coordinate> aliveman;
  for (vector<ChessBase*>::size_type i = 16; i <= 47; ++i)
  {
	  if (g_global.g_chessman[i] != nullptr && g_global.g_chessman[i]->GetAlive())
	  {
		  aliveman.push_back(g_global.g_chessman[i]->location());
	  }
  }

  return aliveman;
}
//计算机走棋,试图找到一个min局面
OneStep CChessPlay::KillSortAlfaBataSearch(int depth)
{
	//计算机走棋,计算机只需要找到min局面
	m_isAIGo = true;

	int min, tempMin;
	tempMin = min = 10000;
	moves m;
	GetMoveBigMan(m);
	//无路可走：输棋
	if (m.size() == 0)
	{
		MoveOnce once(Coordinate(0, 0), Coordinate(0, 0));
		OneStep oncestep(once, 0);
		return oncestep;
	}
	//吃子走法优先走
	sort(m.begin(), m.end(), good_step);
	int beststep = 0;
	for (moves::size_type i = 0; i < m.size(); i++)
	{
		g_global.GoAhead(m[i]);
		bool isKillStep = g_global.IsKillStep(m[i]);//如果是个吃子走法
		//用已经找到的最小值更新接下来的搜索,使得搜索窗口缩小,更容易发生剪枝
		tempMin = KillSortAlfaBataMaxUserSearch(depth - 1, tempMin, isKillStep);
		//找到一个更好的走法(min值会更低)
		if (tempMin < min)
		{
			min = tempMin;
			beststep = i;
		}
		g_global.GoBack(m[i]);
	}

    return std::move(m[beststep]);
}
//baseMin代表上层已经找到的局面最小值,而上层要的就是最小值
//用户走棋  MaxSearch要的是最大值,只要找到了一个最大值比上层的baseMin大,就退出
int CChessPlay::KillSortAlfaBataMaxUserSearch(int _depth,int _baseMin, bool _isKillStep)
{
	//人走棋
	m_isAIGo = false;
	
	if (_depth == 0)
	{
        m_evaluate = Evaluate();
        return m_evaluate;
	}

	int finalMax, tempMax;
	finalMax = tempMax = -10000;
	moves m;
	GetMoveSmallMan(m);
	//吃子走法优先走
	sort(m.begin(),m.end(),good_step);

	for(moves::size_type i=0;i<m.size();++i)
	{
		g_global.GoAhead(m[i]);
		bool isKillStep = m[i].targetId != 0;//如果是个吃子走法
		tempMax = KillSortAlfaBataMinAISearch(_depth - 1, tempMax, isKillStep);
		if (tempMax > _baseMin)
		{
			finalMax = tempMax;
			g_global.GoBack(m[i]);
			break;
		}
		else if (tempMax > finalMax)
		{
			finalMax = tempMax;
		}
		g_global.GoBack(m[i]);
	}
	return finalMax;
}
//计算机走棋
int CChessPlay::KillSortAlfaBataMinAISearch(int depth, int _baseMax, bool _isKillStep)
{
	//计算机走棋
	m_isAIGo = true;

	if(depth==0)
	{
        m_evaluate = Evaluate();
        return m_evaluate;
	}

	int finalMin, tempMin;
	finalMin = tempMin = 10000;
	moves m;
	GetMoveBigMan(m);
	//吃子走法优先走
	sort(m.begin(),m.end(),good_step);

	for(moves::size_type i=0;i<m.size();i++)
	{
		g_global.GoAhead(m[i]);
		bool isKillStep = m[i].targetId != 0;
		tempMin = KillSortAlfaBataMaxUserSearch(depth - 1, tempMin, isKillStep);
		//发生剪枝
		if (tempMin<_baseMax)
		{
			finalMin = tempMin;
			g_global.GoBack(m[i]);
			break;
		}
		else if (tempMin < finalMin)
		{
			finalMin = tempMin;
		}
		g_global.GoBack(m[i]);
	}
	return finalMin;
}

int CChessPlay::Evaluate(void)
{
	m_evaluate = EvaluateSmallUser() - EvaluateMaxAI();

	int smallValue = 0, bigValue = 0;
	for (auto itr = g_global.smallManIDs.cbegin(); itr != g_global.smallManIDs.cend(); ++itr)
	{
		if (g_global.g_chessman[*itr]->GetAlive())
		{
			//smallValue += (g_evaluate.manBeProtected[*itr] > g_evaluate.manBeKilled[*itr] ? 1 : 0) * g_global.g_chessmanValue[*itr];
			smallValue -= (g_evaluate.manBeProtected[*itr] < g_evaluate.manBeKilled[*itr] ? 1 : 0)  * g_global.g_chessmanValue[*itr];
		}
	}
	for (auto itr = g_global.bigManIDs.cbegin(); itr != g_global.bigManIDs.cend(); ++itr)
	{
		if (g_global.g_chessman[*itr]->GetAlive())
		{
			//bigValue += (g_evaluate.manBeProtected[*itr] > g_evaluate.manBeKilled[*itr] ? 1 : 0) * g_global.g_chessmanValue[*itr];
			bigValue -= (g_evaluate.manBeProtected[*itr] < g_evaluate.manBeKilled[*itr] ? 1 : 0)  * g_global.g_chessmanValue[*itr];
		}
	}
	m_evaluate = smallValue - bigValue;

	g_evaluate.Clear();

    OutStatus(m_fout);

	return m_evaluate;
}
int CChessPlay::EvaluateSmallUser(void)
{
	//位置价值+棋子价值
	int smallvalue = 0;
	for (auto itr = g_global.smallManIDs.cbegin(); itr != g_global.smallManIDs.cend(); ++itr)
	{
		if (g_global.g_chessman[*itr]->GetAlive())
		{
            smallvalue += g_global.g_chessman[*itr]->Value();// 棋子价值
            smallvalue += g_global.g_chessman[*itr]->PositionValue();// 位置价值
            g_global.g_chessman[*itr]->GetCaptureValue(g_global.g_chessboard);// 攻击防御价值
		}
	}
	return smallvalue;
}
int CChessPlay::EvaluateMaxAI(void)
{
	//位置价值+棋子价值
	int bigvalue = 0;
	for (auto itr = g_global.bigManIDs.cbegin(); itr != g_global.bigManIDs.cend(); ++itr)
	{
		if (g_global.g_chessman[*itr]->GetAlive())
		{
			bigvalue += g_global.g_chessman[*itr]->Value();
			bigvalue += g_global.g_chessman[*itr]->PositionValue();
			g_global.g_chessman[*itr]->GetCaptureValue(g_global.g_chessboard);
		}
	}
	return bigvalue;
}

void CChessPlay::SaveSituation(const std::string& _fullPathFile) const
{
	wofstream fout;
	fout.open(_fullPathFile);
	if (!fout)
	{
		return;
	}
	for (int i=16;i<48;i++)
	{
		if (g_global.g_chessman[i]->GetAlive())
		{
			fout<<i<<" "
				<<g_global.g_chessman[i]->location().x<<" "
				<<g_global.g_chessman[i]->location().y<<endl;
		}
	}
}
void CChessPlay::LoadSituationFromFen(const std::string& _fenStr)
{
	std::string redBlack;
	int notKillCount = 0;
	int totalCount = 0;
	CCrFENHelper::LoadSituationFromFen(_fenStr, redBlack, notKillCount, totalCount);
}
std::string CChessPlay::GetSituationFen(bool _aiUseRed)
{
	return CCrFENHelper::GetSituationFen(_aiUseRed);
}
void CChessPlay::LoadSituationFromFile(const std::string& _fullPathFile)
{
    std::string narrowStr = _fullPathFile;
	std::ifstream fin;
	fin.open(_fullPathFile);
	vector<vector<int> > onesituation;
	while (fin)
	{
		int id,x,y;
		vector<int> oneman;
		fin>>id>>x>>y;
		oneman.push_back(id);
		oneman.push_back(x);
		oneman.push_back(y);
		onesituation.push_back(oneman);
	}
	fin.close();

	g_global.Clear();

	for (auto itr = onesituation.begin(); itr != onesituation.end(); itr++)
	{
		g_global.g_chessman[(*itr)[0]]->SetAlive(true);
		g_global.g_chessman[(*itr)[0]]->SetLocation(Coordinate((*itr)[1], (*itr)[2]));
		g_global.g_chessboard[(*itr)[1]][(*itr)[2]] = (*itr)[0];
	}
}

OneStep CChessPlay::GoAheadAndRecordHistory(const OneStep& _oneStep)
{
	g_global.GoAhead(_oneStep);
    m_moveHistory.push_back(_oneStep);

    return _oneStep;
}

bool CChessPlay::IsNowRedGo(void)
{
	bool ret = m_moveHistory.size() % 2 == 0;
	return ret;
}

bool CChessPlay::IsNowBlackGo(void)
{
	bool ret = IsNowRedGo();
	ret = !ret;
	return ret;
}
bool CChessPlay::IsUserFailed(void)
{
	//判断计算机走棋会不会导致人被困毙
	moves m;
	bool userFailed = true;
	GetMoveSmallMan(m);
	for (auto& i : m)
	{
		if (IsUserCanMoveTo(i))
		{
			userFailed = false;
		}
	}
	return userFailed;
}
void CChessPlay::GoBackAndRemoveHistory(void)
{
	//双方各退后一步，退后之前判断是否已经退到最开始
    if (!IsMoveHistoryEmpty())
    {
		g_global.GoBack(m_moveHistory.back());
        m_moveHistory.pop_back();
	}
	//双方各退后一步，退后之前判断是否已经退到最开始。
	//如果是加载局面走棋，可能只走了一步就将死了，所以悔棋只能悔一步
	if (!IsMoveHistoryEmpty())
	{
		g_global.GoBack(m_moveHistory.back());
		m_moveHistory.pop_back();
	}
}
bool CChessPlay::IsUserCanMoveTo(const OneStep& _oneStep) const
{
	//获取所有可能的走法
	auto from = _oneStep.move.from;
	moves m;
	auto idFrom = ChessBoard(from);
	if (idFrom == 0)
	{
		return false;
	}
	GetMoveOneMan(idFrom, m);
	auto iter = find(m.begin(), m.end(), _oneStep);
	if (iter != m.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Out(ostream& os, initializer_list<string> _il)
{
    for (auto itr = _il.begin(); itr != _il.end(); ++itr)
    {
        os<<setw(2) << *itr;
    }
}
void PrintLine(ostream& _os)
{
	_os << "\n";
}
template<typename T>
void PrintVal(ostream& os, T _t)
{
	os<<setw(2) << _t;
	os.flush();
}
//5 子力价值
void OutValue(ostream& os)
{
	os << "[value]:\n";
	int smallManValue = 0, bigManValue = 0;
	for (size_t i = 16; i <= 31; ++i)
	{
		smallManValue += g_global.g_chessman[i]->GetAlive() ? g_global.g_chessman[i]->Value() : 0;
	}
	os << "smallMan("<< smallManValue<<") :";
	for (size_t i = 16; i <= 31; ++i)
	{
        Out(os, { to_string(i), g_global.g_chessmanName[i] + "(", to_string(g_global.g_chessman[i]->GetAlive() ? g_global.g_chessman[i]->Value() : 0), ") " });
	}
	PrintLine(os);
	for (size_t i = 32; i <= 47; ++i)
	{
		bigManValue += g_global.g_chessman[i]->GetAlive() ? g_global.g_chessman[i]->Value() : 0;
	}
	os << "bigMan  ("<<bigManValue<<") :";
	for (size_t i = 32; i <= 47; ++i)
	{
        Out(os, { to_string(i), g_global.g_chessmanName[i] + "(", to_string(g_global.g_chessman[i]->GetAlive() ? g_global.g_chessman[i]->Value() : 0), ") " });
	}
	PrintLine(os);
}
//6 位置价值
void OutPositionValue(ostream& os)
{
	int smallManValue = 0, bigManValue = 0;
	os << "[position value]:\n";
	for (size_t i = 16; i <= 31; ++i)
	{
		smallManValue += g_global.g_chessman[i]->GetAlive() ? g_global.g_chessman[i]->PositionValue() : 0;
	}
	os << "smallMan(" << smallManValue << ") :";
	for (auto itr = g_global.smallManIDs.cbegin(); itr != g_global.smallManIDs.cend(); ++itr)
	{
        Out(os, { to_string(*itr), g_global.g_chessmanName[*itr] + "(", to_string(g_global.g_chessman[*itr]->GetAlive() ? g_global.g_chessman[*itr]->PositionValue() : 0), ") " });
	}
	PrintLine(os);
	for (size_t i = 32; i <= 47; ++i)
	{
		bigManValue += g_global.g_chessman[i]->GetAlive() ? g_global.g_chessman[i]->PositionValue() : 0;
	}
	os << "bigMan  (" << bigManValue << ") :";
	for (auto itr = g_global.bigManIDs.cbegin(); itr != g_global.bigManIDs.cend(); ++itr)
	{
        Out(os, { to_string(*itr), g_global.g_chessmanName[*itr] + "(", to_string(g_global.g_chessman[*itr]->GetAlive() ? g_global.g_chessman[*itr]->PositionValue() : 0), ") " });
	}
	PrintLine(os);
}
void OutCaptureValue(ostream& os)
{
	int smallManValue = 0, bigManValue = 0;
	os << "[capture value]:\n";
	for (auto itr = g_global.smallManIDs.cbegin(); itr != g_global.smallManIDs.cend(); ++itr)
	{
		smallManValue -= (g_evaluate.manBeProtected[*itr] < g_evaluate.manBeKilled[*itr] ? 1 : 0)  * g_global.g_chessmanValue[*itr];
	}
	os << "smallMan(" << smallManValue << ") :";
	for (auto itr = g_global.smallManIDs.cbegin(); itr != g_global.smallManIDs.cend(); ++itr)
	{
		int value = 0;
		value -= (g_evaluate.manBeProtected[*itr] < g_evaluate.manBeKilled[*itr] ? 1 : 0)  * g_global.g_chessmanValue[*itr];
        Out(os, { to_string(*itr), g_global.g_chessmanName[*itr] + "(", to_string(value), ") " });
	}
	os << "\n";
	for (auto itr = g_global.bigManIDs.cbegin(); itr != g_global.bigManIDs.cend(); ++itr)
	{
		bigManValue -= (g_evaluate.manBeProtected[*itr] < g_evaluate.manBeKilled[*itr] ? 1 : 0)  * g_global.g_chessmanValue[*itr];
	}
	os << "bigMan  (" << bigManValue << ") :";
	for (auto itr = g_global.bigManIDs.cbegin(); itr != g_global.bigManIDs.cend(); ++itr)
	{
		int value = 0;
		value -= (g_evaluate.manBeProtected[*itr] < g_evaluate.manBeKilled[*itr] ? 1 : 0)  * g_global.g_chessmanValue[*itr];
        Out(os, { to_string(*itr), g_global.g_chessmanName[*itr] + "(", to_string(value), ") " });
	}
	os << "\n";
}
void OutChessBoard(ostream& os)
{
	//2 输出整个棋盘
	PrintVal(os, "  ");
	for (size_t i = 3; i <= 11; ++i)
	{
		os << setw(2) << i;
	}
	PrintLine(os);
	for (size_t i = 3; i <= 11; ++i)
	{
		char c = 'a' + (short)(i - 3);
		os << setw(2) << c;
	}
	PrintLine(os);
	for (size_t hight = 3; hight <= 12; ++hight)
	{
		PrintVal(os, hight);
		for (size_t j = 3; j <= 11; ++j)
		{
			int id = g_global.g_chessboard[hight][j];
			
			os << g_global.g_chessmanName[id] << "";
		}
		os << "   ";
		for (size_t j = 3; j <= 11; ++j)
		{
			int id = g_global.g_chessboard[hight][j];
			if (id == 0)
			{
				os << "   ";
			}
			else
			{
				os << id << " ";
			}
		}
		PrintLine(os);
	}
}

int CChessPlay::GetEvaluate(void) const
{
	return m_evaluate;
}
void CChessPlay::OutStatus(ostream& os)
{
	static int count_now = 0;
	//1 输出主要信息
	os << "CChessPlay::OutStatus ChessBoard[" << ++count_now << "]" << "  evaluate[" << m_evaluate << "] ";
	PrintLine(os);
	//2 输出整个棋盘
	OutChessBoard(os);
	//5 子力价值
	OutValue(os);
	//6 位置价值
	OutPositionValue(os);
	//3 进攻贡献明细
	OutCaptureValue(os);

	os << "\n" << endl;
}
