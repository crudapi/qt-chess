#pragma once

#include "chessbase.h"
//Âí
class Horse : public ChessBase {
public:
	Horse(int sidetag, int _id, int value, const Coordinate& newlocation);
	~Horse(void);
	void GetMove(const Matrix& chessboard, moves& temp) const;
	void GetCaptureValue(const Matrix& chessboard) const;
	bool CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const;
};