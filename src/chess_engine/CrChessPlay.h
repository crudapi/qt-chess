#pragma once

#include <cassert>
#include <fstream>
#include"chessbase.h"
#include"cannon.h"
#include"chariot.h"
#include"horse.h"
#include"king.h"
#include"minister.h"
#include"official.h"
#include"soldier.h"
#include"global.h"

/*
作者：QQ3508551694
*/

class CChessPlay
{
public:
	CChessPlay(void);
	~CChessPlay(void);
	IdType ChessBoard(const Coordinate& p) const;
	ChessBase* ChessMan(IdType _id)  { assert(g_global.IsValidId(_id));  return g_global.g_chessman[_id]; }
public:
    void InitChessPlay(void);
	void ClearChessBoard(void);
	void ClearChessMan(void);

    //FEN
    void LoadSituationFromFile(const std::string& _fullPathFile);
    void LoadSituationFromFen(const std::string& _fenStr);//about ucci
	std::string GetSituationFen(bool _aiUseRed);
	
    void SaveSituation(const std::string& _fullPathFile) const;
	std::vector<Coordinate> AliveMan(void);
	bool IsUserCanMoveTo(const OneStep& _oneStep) const;
	int GetChessBoard(const Coordinate& p) const;
	void SetChessBoard(const Coordinate& p, int _id);
	//杀法优先的Alfa-Bata搜索
	OneStep KillSortAlfaBataSearch(int depth);
	// 获取当前棋子id的所有走法
	void GetMoveOneMan(int id, moves& m) const;
    bool IsMoveHistoryEmpty(void) const { return (m_moveHistory.size() == 0);}
	bool IsNowRedGo(void);
	bool IsNowBlackGo(void);
	bool IsUserFailed(void);
    OneStep GoAheadAndRecordHistory(const OneStep& );
    void GoBackAndRemoveHistory(void);
	int GetEvaluate(void) const;
	//输出到控制台
    void OutStatus(std::ostream& os);
	bool BeKilled(bool _aiUseRed);
	void GetMoveSmallMan(moves& m);
	void GetMoveBigMan(moves& m);

private:
	//杀法优先的Alfa-Bata搜索
	int KillSortAlfaBataMaxUserSearch(int _depth, int _baseMin, bool _isKillStep);
	int KillSortAlfaBataMinAISearch(int _depth, int _baseMax, bool _isKillStep);
	//静态搜索算法
	int QuiesMinAISearch(int _baseMax);
	int QuiesMaxUserSearch(int _baseMin);
	int Quies(int alpha,int beta);

	//评价函数相关
private:
	int Evaluate();
	int EvaluateMaxAI();
	int EvaluateSmallUser();

private:
	//Zobrist
	void InitZobrist(void);
    void InitLog(void);
    void CloseLog(void);
private:
	//Zobrist
	bool m_isAIGo;//当前轮到谁走棋，也是局面的一部分（0表示FALSE，1表示TRUE）
	int m_currentZobristKey;

private:
    int m_evaluate;
    std::list<OneStep> m_moveHistory;
    std::ofstream m_fout;
private:
	static CChessPlay* s_instance;
};
