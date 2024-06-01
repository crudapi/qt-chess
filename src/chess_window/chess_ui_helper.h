#pragma once

#include"CrChessPlay.h"
#include<algorithm>
#include<ctime>
#include <list>
#include <map>
#include <memory>
#include <QPoint>

class CchessUI;

struct Rect
{
	Rect(){}
	Rect(int, int, int, int);
	int Height(void) const;
	int Width(void) const;
public:
	int left;
	int right;
	int top;
	int bottom;
};

class ElementRect
{
	friend class CchessUI;
	//Rect toolBarRect;//l t r b
	Rect chessBoard;
	Rect dlgRect;//整个对话框的客户区大小
};

// CchessUI 对话框
class CchessUI
{
// 构造
public:
	CchessUI(int nToolBarElement);	// 标准构造函数
    ~CchessUI();
public:
    QPoint ChessManCoordinateToUIPoint(const Coordinate& p) const;
    Coordinate UIPointToChessManCoordinate(const QPoint& point)const;

	bool CheckPositionIsOnBoard(Coordinate p) const;
	OneStep GoAhead(const OneStep& m);
	void GoBack(void);
	//检查从当前位置是否可达目标位置(point)
    std::pair<bool, std::wstring> CanMoveTo(QPoint point) const;
    std::pair<bool, std::string> CanMoveFrom(QPoint point) const;
protected:
    std::unique_ptr< CChessPlay> m_chessObject;
    //UI related
    QPoint m_userManPointStart;//用户走棋的起点
    QPoint m_userManPointTo;//用户走棋的终点
    QPoint m_AIManPointFrom;//计算机走棋的起点
    QPoint m_AIManPointTo;//计算机走棋的终点
    struct SizeConfig
    {
        int uiXBase = 0;
        int uiYBase = 0;
        int uiChessManSize = 57;
    };
    enum struct State
    {
        EMPTY,
        AFTER_SELECTED_START,
        AFTER_SELECTED_TO,
        AFTER_AI_SELECTED_START_AND_TO,
    };

    SizeConfig m_sizeConfig;
    State m_selectState = State::EMPTY;

	MoveOnce m_moveOnceAISelected;
	bool m_swaped;//该变量仅仅控制界面上红黑棋子的交换，跟内部走棋逻辑毫无关系，需要其他变量来控制。
	bool m_aiUseRed;//计算机执红棋，否则就是执黑棋：CChessPlay内部总是红棋在下方（小号棋子，大写字母）,默认情况下，计算机在上方大棋子黑棋
private:
	std::ofstream m_fout;
protected:
  //the only object contained the data of playing  chess
private:
	//about ucci 
	bool m_engineOK;
	bool m_useOtherEngine;
	bool m_useSelfEngine;
	//UCCIHelper m_ucciUiHelper;//该成员负责加载外部引擎
};
