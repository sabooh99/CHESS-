// LEGACY CODE (NOT USED IN BUILD)
// This file is preserved for development history and reference only.
#include<iostream>
#include<string>
using namespace std;

class Piece {
	string Colour;
	bool IsCaptured;
	int Row, Col;

public:
	Piece(string C, int R, int Column) : Colour(C), Row(R), Col(Column) {
		IsCaptured = false;
	}

	virtual bool IsValidMove(int GoToRow, int GoToCol, class Board& B) = 0;

	void SetPosition(int R, int C) {
		Row = R;
		Col = C;
	}

	void MarkCaptured() {
		IsCaptured = true;
	}

	string GetColor() {
		return Colour;
	}

	int GetRow() {
		return Row;
	}

	int GetCol() {
		return Col;
	}

	bool GetIsCaptured() {
		return IsCaptured;
	}

	virtual string GetName() {
		return "piece";
	}

	virtual ~Piece() {}
};

class Board {
private:
	Piece* Grid[8][8];

public:
	Board() {
		for (int R = 0; R < 8; R++)
			for (int C = 0; C < 8; C++)
				Grid[R][C] = nullptr;
	}

	~Board() {
		for (int R = 0; R < 8; R++)
			for (int C = 0; C < 8; C++)
				if (Grid[R][C] != nullptr)
					delete Grid[R][C];
	}

	void MovePieceTo(int FromRow, int FromCol, int ToRow, int ToCol) {
		if (FromRow == ToRow && FromCol == ToCol)
			return;

		if (Grid[ToRow][ToCol] != nullptr) {
			Grid[ToRow][ToCol]->MarkCaptured();
			delete Grid[ToRow][ToCol];
		}

		Grid[ToRow][ToCol] = Grid[FromRow][FromCol];
		Grid[FromRow][FromCol] = nullptr;
		Grid[ToRow][ToCol]->SetPosition(ToRow, ToCol);
	}

	Piece* GetPiece(int Row, int Col) {
		return Grid[Row][Col];
	}

	bool IsInCheck(string Color) {
		int KingRow = -1, KingCol = -1;
		bool KingFound = false;

		for (int R = 0; R < 8 && !KingFound; R++) {
			for (int C = 0; C < 8 && !KingFound; C++) {
				Piece* P = Grid[R][C];

				if (P != nullptr && P->GetColor() == Color && P->GetName() == "king") {
					KingRow = R;
					KingCol = C;
					KingFound = true;
				}
			}
		}

		for (int R = 0; R < 8; R++) {
			for (int C = 0; C < 8; C++) {
				Piece* P = Grid[R][C];

				if (P != nullptr && P->GetColor() != Color) {
					if (P->IsValidMove(KingRow, KingCol, *this))
						return true;
				}
			}
		}

		return false;
	}

	void SimulateMove(int FromRow, int FromCol, int ToRow, int ToCol) {
		Grid[ToRow][ToCol] = Grid[FromRow][FromCol];
		Grid[FromRow][FromCol] = nullptr;
		Grid[ToRow][ToCol]->SetPosition(ToRow, ToCol);
	}

	void UndoMove(int FromRow, int FromCol, int ToRow, int ToCol, Piece* Captured) {
		Grid[FromRow][FromCol] = Grid[ToRow][ToCol];
		Grid[ToRow][ToCol] = nullptr;
		Grid[FromRow][FromCol]->SetPosition(FromRow, FromCol);
		Grid[ToRow][ToCol] = Captured;
	}

	bool IsCheckmate(string Color) {
		if (!IsInCheck(Color))
			return false;

		for (int R = 0; R < 8; R++) {
			for (int C = 0; C < 8; C++) {
				Piece* P = Grid[R][C];

				if (P == nullptr || P->GetColor() != Color)
					continue;

				for (int TR = 0; TR < 8; TR++) {
					for (int TC = 0; TC < 8; TC++) {
						if (!P->IsValidMove(TR, TC, *this))
							continue;

						Piece* Captured = Grid[TR][TC];
						SimulateMove(R, C, TR, TC);

						bool StillInCheck = IsInCheck(Color);

						UndoMove(R, C, TR, TC, Captured);

						if (!StillInCheck)
							return false;
					}
				}
			}
		}

		return true;
	}

	bool IsStalemate(string Color) {
		if (IsInCheck(Color))
			return false;

		for (int R = 0; R < 8; R++) {
			for (int C = 0; C < 8; C++) {
				Piece* P = Grid[R][C];

				if (P == nullptr || P->GetColor() != Color)
					continue;

				for (int TR = 0; TR < 8; TR++) {
					for (int TC = 0; TC < 8; TC++) {
						if (!P->IsValidMove(TR, TC, *this))
							continue;

						Piece* Captured = Grid[TR][TC];
						SimulateMove(R, C, TR, TC);

						bool StillInCheck = IsInCheck(Color);

						UndoMove(R, C, TR, TC, Captured);

						if (!StillInCheck)
							return false;
					}
				}
			}
		}

		return true;
	}
};

