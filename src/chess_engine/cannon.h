#pragma once

#include "chessbase.h"

class  Cannon : public ChessBase {
public:
	Cannon(int sidetag, int _id, int value, const Coordinate& newlocation);
	~Cannon(void);
	void GetMove(const Matrix& chessboard, moves& temp) const;
	void GetCaptureValue(const Matrix& chessboard) const;
	bool CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const;
};