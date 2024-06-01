#pragma once
#include "chessbase.h"
//帅、将
class  King : public ChessBase
{
public:
	King(int sidetag, int _id, int value, const Coordinate& newlocation);
	~King(void);
	void GetMove(const Matrix& chessboard, moves& temp) const;
	void GetCaptureValue(const Matrix& chessboard) const;
	bool CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const;

private:
	//将_id走到_moveToPosition位置后会不会导致将帅照面
    bool IsKingKillKing(const IdType _id, const int _xTo, const int _yTo) const;
};
