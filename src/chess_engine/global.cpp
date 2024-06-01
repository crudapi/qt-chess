#include <cassert>

#include "global.h"
#include "chessbase.h"
Global g_global;
Evaluate g_evaluate;			


Global::Global(void):bigKingId(32), smallKingId(16)
, g_chessmanName(48, "  ")
, killStep(0, 0, 0, 0, 0)
, engineCannotUseStep(-1, -1, 0, 0, 0)
, g_randomer(std::chrono::system_clock::now().time_since_epoch().count())
{
	g_xStart = 3;
	g_yStart = 3;
	g_xEnd = 11 + 1;
	g_yEnd = 12 + 1;
}

void Global::InitChessManIDArray(void)
{
    std::array<IdType, 16> smallId = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    for (size_t i = 0; i < smallId.size(); ++i)
	{
		Global::smallManIDs.data()[i] = smallId[i];
	}
    std::array<IdType, 16> bigId =   { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
    for (size_t i = 0; i < bigId.size(); ++i)
	{
		Global::bigManIDs.data()[i] = bigId[i];
	}
}
void Global::Clear(void)
{
	//清空棋盘
	for (int i = 3; i <= 12; i++)
	{
		for (int j = 3; j <= 11; j++)
		{
            g_chessboard[i][j] = 0;
		}
	}
	//修改棋子
	for (int i = 16; i < 48; i++)
	{
		Global::g_chessman[i]->SetAlive(false);
	}
}

void Evaluate::Clear(void)
{
	sideValue[0] = sideValue[1] = 0;
	for (size_t i = 16; i < 48; i++)
	{
		manBeKilled[i] = manBeProtected[i] = 0;
	}
}

std::string Global::to_string(const OneStep& _step)
{
	char _Buf[2 * 100] = {0};

	sprintf_s(_Buf, "[(%d,%d),(%d,%d),%d]", _step.move.from.x, _step.move.from.y, _step.move.to.x, _step.move.to.y, _step.targetId);
	return (std::string(_Buf));
}
std::string Global::to_string(const Matrix& _matrix)
{
	std::string str;
    for (size_t i = 0; i < g_chessboard.size(); ++i)
	{
        for (size_t j = 0; j < g_chessboard[i].size(); ++j)
		{
			//str.append(std::to_string(g_chessboard[i][j]) + " ");
		}
		str.append("\n");
	}
	return str;
}

bool Global::BeProtected(bool _isTargetBigMan, int _x, int _y)
{
	if (_isTargetBigMan)
	{
		for (auto itr = Global::bigManIDs.cbegin(); itr != Global::bigManIDs.cend(); ++itr)
		{
			moves mvs;
            Global::g_chessman[*itr]->GetMove(g_chessboard, mvs);
			for (auto itrInner = mvs.cbegin(); itrInner != mvs.cend(); ++itrInner)
			{
				if (itrInner->move.to == Coordinate(_x, _y))
				{
					return true;
				}
			}
		}
	}
	else
	{
		for (auto itr = Global::smallManIDs.cbegin(); itr != Global::smallManIDs.cend(); ++itr)
		{
			moves mvs;
            Global::g_chessman[*itr]->GetMove(g_chessboard, mvs);
			for (auto itrInner = mvs.cbegin(); itrInner != mvs.cend(); ++itrInner)
			{
				if (itrInner->move.to == Coordinate(_x, _y))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool good_step(const OneStep& s1, const OneStep& s2)											///<	严格弱排序排序,相等的返回false
{
	
	if ((s1.targetId == 16 || s1.targetId == 32) && !(s2.targetId == 16 || s2.targetId == 32))			///<	将军走法优先
	{
		return true;
	}
	
	if (s1.targetId != 0 && s2.targetId == 0)														///<	吃子走法优先
	{
		return true;
	}
	return false;
}
//根据m所指示的一个走法，走子，改变棋盘数组和棋子数组，并返回这个走法对应的走子
OneStep Global::GoAhead(const OneStep& m)
{
	//获取起点棋子和终点棋子
    int id_from = g_chessboard[m.move.from.x][m.move.from.y];   //起点棋子的坐标
    int id_to = g_chessboard[m.move.to.x][m.move.to.y];   //终点棋子的坐标
	assert(id_from != 0);
	if (id_from == 0)
	{
		throw std::runtime_error("id_from == 0, onestep:" + to_string(m));
	}
    g_chessboard[m.move.from.x][m.move.from.y] = 0;
    g_chessboard[m.move.to.x][m.move.to.y] = id_from;
	Global::g_chessman[id_from]->SetLocation(m.move.to);

	if (id_to != 0)
	{
		Global::g_chessman[id_to]->SetAlive(false);
	}
	return m;
}
void Global::GoBack(const OneStep& one)
{
	Coordinate fromLocation = one.move.from,
		toLocation = one.move.to;
	//修改移动棋子
    int idHaveToBack = g_chessboard[toLocation.x][toLocation.y];
	assert(idHaveToBack != 0);
    assert(g_chessboard[fromLocation.x][fromLocation.y] == 0);
    g_chessboard[fromLocation.x][fromLocation.y] = idHaveToBack;
	Global::g_chessman[idHaveToBack]->SetLocation(fromLocation);
	//修改被吃棋子
	int idBeKilled = one.targetId;
    g_chessboard[toLocation.x][toLocation.y] = idBeKilled;
	if (idBeKilled != 0)
	{
		Global::g_chessman[idBeKilled]->SetAlive(true);
	}
}
