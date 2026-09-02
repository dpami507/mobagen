#include "Heuristics.h"
#include "WorldState.h"
#include "pieces/Pieces.h"

Heuristics Heuristics::BoardAnalysis(WorldState* state) {
  // todo: write your own heuristic here
  return {};
}

// https://www.chessprogramming.org/Simplified_Evaluation_Function
// We did some borrowing from this handy dandy website
const int PSQT_pawn[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5, 5, 10, 25, 25, 10, 5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, -5, -10, 0, 0, -10, -5, 5,
    5, 10, 10, -20, -20, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0
};

const int PSQT_knight[] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50}
;

const int PSQT_bishop[] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};

const int PSQT_rook[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 5, 5, 0, 0, 0
};

const int PSQT_queen[] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20
};

const int PSQT_king[] = {
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, 20, 0, 0, 0, 0, 20, 20,
    20, 30, 10, 0, 0, 10, 30, 20
};

int Heuristics::MaterialScore(WorldState* state) {
  int score = 0;
  for (auto line = 0; line < 8; line++) {
    for (auto column = 0; column < 8; column++) {
      auto location = Point2D(column, line);
      auto piece = state->PieceAtPosition(location);
      auto pieceScore = 0;
      int moves;
      switch (piece.Piece()) {
        case PieceType::King:
          pieceScore += 20000;
          pieceScore += PSQT_king[line * 8 + column];
          break;
        case PieceType::Queen:
          pieceScore += 900;
          pieceScore += PSQT_queen[line * 8 + column];
          break;
        case PieceType::Rook:
          pieceScore += 500;
          pieceScore += PSQT_rook[line * 8 + column];
          break;
        case PieceType::Knight:
          pieceScore += 320;
          pieceScore += PSQT_knight[line * 8 + column];
          break;
        case PieceType::Bishop:
          pieceScore += 330;
          pieceScore += PSQT_bishop[line * 8 + column];
          break;
        case PieceType::Pawn:
          pieceScore += 100;
          pieceScore += PSQT_pawn[line * 8 + column];
          break;
        default:
          continue;
      }
      if (piece.Color() == PieceColor::Black)
        score -= pieceScore;
      else
        score += pieceScore;
    }
  }
  return score;
}
int Heuristics::distanceToCenter(Point2D location) {
  // todo: improve. I am unsure if this is the best way. I am doubling because I am targeting the center.
  auto doubled = Point2D(location.x * 2 - 7, location.y * 2 - 7);
  auto maxed = 3 - (std::min(std::abs(doubled.x), std::abs(doubled.y)) - 1) / 2;
  return maxed;
}
