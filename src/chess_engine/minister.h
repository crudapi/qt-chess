#pragma once

#include "chessbase.h"
//�ࡢ��
class  Minister : public ChessBase {
public:
	Minister(int sidetag, int _id, int value, const Coordinate& newlocation);
	~Minister(void);
	void GetMove(const Matrix& chessboard, moves& temp) const;
	void GetCaptureValue(const Matrix& chessboard) const;
	bool CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const;
};