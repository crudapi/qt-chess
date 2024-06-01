#include <fstream>
#include <array>
#include <cassert>
#include "chess_ui_helper.h"
#include "global.h"
#include "CrChessPlay.h"
#include <iostream>
#include <functional>
using namespace std;

CchessUI::CchessUI(int nToolBarElement)
: m_chessObject(new CChessPlay)
{
    m_userManPointStart.setX(0);
    m_userManPointStart.setY(0);

	//about ucci
	m_useOtherEngine = true;
	m_useSelfEngine = false;
	m_engineOK = false;
	assert(nToolBarElement > 0);

	m_swaped = false;
	m_aiUseRed = false;//默认：计算机执黑棋在上方
}
QPoint CchessUI::ChessManCoordinateToUIPoint(const Coordinate& p) const
{
    QPoint point;
	//数据转换 （x,y）->UI(x1,y1)
	//由棋盘数组中的坐标值得到窗口棋盘中的左上坐标值
    assert(m_sizeConfig.uiXBase >= 0 && m_sizeConfig.uiYBase >= 0);
    point.setX(m_sizeConfig.uiXBase + (p.y - 3) * m_sizeConfig.uiChessManSize);//棋盘方格小正方形的边长62
    point.setY(m_sizeConfig.uiYBase + (p.x - 3) * m_sizeConfig.uiChessManSize);
    return point;
}
CchessUI::~CchessUI()
{
}

//数据转换 UI(x1,y1)->棋盘数组（x,y）
Coordinate CchessUI::UIPointToChessManCoordinate(const QPoint& point) const
{
    Coordinate p;
    p.y = (point.x() - m_sizeConfig.uiXBase) / m_sizeConfig.uiChessManSize + 3;//除数取整
    p.x = (point.y() - m_sizeConfig.uiYBase) / m_sizeConfig.uiChessManSize + 3;//除数取整
    return p;
}

bool CchessUI::CheckPositionIsOnBoard(Coordinate p) const
{
  return (3<=p.x && p.x<=12 &&
          3<=p.y && p.y<=11);
}

std::pair<bool, std::string> CchessUI::CanMoveFrom(QPoint point) const
{
    Coordinate positionstart = UIPointToChessManCoordinate(point);
    moves m;
    //检查是否出界，起点棋子是不是用户的棋子
    if (CheckPositionIsOnBoard(positionstart) && m_chessObject->ChessBoard(positionstart) / 16 == 1)
    {
        return std::make_pair(true, "");
    }
    return std::make_pair(false, "走法违规！");
}
std::pair<bool, std::wstring> CchessUI::CanMoveTo(QPoint point) const
{
    Coordinate positionFrom = UIPointToChessManCoordinate(m_userManPointStart);
    Coordinate positionTarget = UIPointToChessManCoordinate(point);
	moves m;
	//检查是否出界，起点棋子是不是用户的棋子
	IdType idTarget = m_chessObject->ChessBoard(positionTarget);
    bool checkFrom = CheckPositionIsOnBoard(positionFrom) ;
    bool checkTo = CheckPositionIsOnBoard(positionTarget);
    bool canMoveTo = m_chessObject->IsUserCanMoveTo(OneStep(positionFrom, positionTarget, idTarget));
    if ( checkFrom && checkTo && canMoveTo /*&& !isSameSideBetweenFromAndTarget*/)
	{
			return std::make_pair(true, L"可以到达那里！");
	}
	else
	{
		return std::make_pair(false, L"走法违规！");
	}
}
OneStep CchessUI::GoAhead(const OneStep& m)
{
	m_chessObject->GoAheadAndRecordHistory(m);
    return m;
}

void CchessUI::GoBack(void)
{
	m_chessObject->GoBackAndRemoveHistory();
	//Invalidate();
}

Rect::Rect(int left1, int top1, int right1, int bottom1)
	:left(left1)
	,right(right1)
	,top(top1)
	,bottom(bottom1)
{
}

int Rect::Height(void) const
{
	assert(bottom >= 0);
	assert(top >= 0);

	return bottom - top;
}

int Rect::Width(void) const
{
	assert(left >= 0);
	assert(right >= 0);

	return right - left;
}
