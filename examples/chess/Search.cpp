#include "Search.h"

#include "Heuristics.h"
#include "algorithm"
#include "pieces/Pieces.h"

#include <iostream>

auto Search::ListMoves(WorldState& state, PieceColor turn) -> std::vector<Move>
{
    vector<Move> moves;
    for (auto line = 0; line < 8; line++)
    {
        for (auto column = 0; column < 8; column++)
        {
            Point2D const location = {column, line};
            auto p = state.PieceAtPosition(location);
            // continue if it is not a valid piece
            if (p.Piece() == PieceType::NONE || turn != p.Color())
                continue;
            vector<Move> toAdd;
            // todo: improve this switch
            switch (p.Piece())
            {
            case PieceType::Rook:
                toAdd = Move::GenerateListOfMoves(p, location, Rook::AttackMoves(state, location));
                moves.insert(moves.end(), toAdd.begin(), toAdd.end());
                break;
            case PieceType::Bishop:
                toAdd = Move::GenerateListOfMoves(p, location, Bishop::AttackMoves(state, location));
                moves.insert(moves.end(), toAdd.begin(), toAdd.end());
                break;
            case PieceType::Pawn:
                toAdd = Move::GenerateListOfMoves(p, location, Pawn::PossibleMoves(state, location));
                moves.insert(moves.end(), toAdd.begin(), toAdd.end());
                break;
            case PieceType::Queen:
                toAdd = Move::GenerateListOfMoves(p, location, Queen::AttackMoves(state, location));
                moves.insert(moves.end(), toAdd.begin(), toAdd.end());
                break;
            case PieceType::Knight:
                toAdd = Move::GenerateListOfMoves(p, location, Knight::AttackMoves(state, location));
                moves.insert(moves.end(), toAdd.begin(), toAdd.end());
                break;
            case PieceType::King:
                toAdd = Move::GenerateListOfMoves(p, location, King::AttackMoves(state, location));
                moves.insert(moves.end(), toAdd.begin(), toAdd.end());
            default:
                break;
            }
        }
    }
    return moves;
}

auto Search::ListPlacesKingCannotGo(WorldState& state, PieceColor turn) -> unordered_set<Point2D>
{
    unordered_set<Point2D> moves;
    for (auto line = 0; line < 8; line++)
    {
        for (auto column = 0; column < 8; column++)
        {
            Point2D const location = {column, line};
            auto p = state.PieceAtPosition(location);
            // continue if it is not a valid piece
            if (p.Piece() == PieceType::NONE || p.Piece() == PieceType::WRONG)
                continue;
            if (turn == p.Color())
                continue;
            // todo: improve this switch
            switch (p.Piece())
            {
            case PieceType::Rook:
                moves.merge(Rook::CoverMoves(state, location));
                break;
            case PieceType::Bishop:
                moves.merge(Bishop::CoverMoves(state, location));
                break;
            case PieceType::Pawn:
                moves.merge(Pawn::CoverMoves(state, location));
                break;
            case PieceType::Queen:
                moves.merge(Queen::CoverMoves(state, location));
                break;
            case PieceType::Knight:
                moves.merge(Knight::CoverMoves(state, location));
                break;
            case PieceType::King:
                moves.merge(King::CoverMovesNaive(state, location));
                break;
            default:
                break;
            }
        }
    }

    return moves;
}

pair<int, int> minMax(int currentDepth, int index, bool getMax, int maxDepth, vector<MoveState>& moveStates, WorldState& state)
{
    string currentTurn = (state.GetTurn() == PieceColor::White) ? "White" : "Black";

    if (King::IsInCheck(state, state.GetTurn()) > 0)
        cout << "KING IS IN CHECK FOR COLOR: " << currentTurn << endl;

    // Get all moves
    auto moves = Search::ListMoves(state, state.GetTurn());

    if (currentDepth == maxDepth)
    {
        //Get the score of where we are
        int score = Heuristics::MaterialScore(&state);

        //Push back the score and move state
        moveStates.push_back({state, {moves[index]}, score});

        // Return the pair of score and index
        return std::make_pair(score, index);
    }

    //For every move
    for (size_t i = 0; i < moves.size(); i++)
    {
        WorldState newState = state;                   // Get the staete
        newState.Move(moves[i].From(), moves[i].To()); // Move

        if (getMax)
            return max(minMax(currentDepth + 1, index * 2, false, maxDepth, moveStates, newState),
                       minMax(currentDepth + 1, index * 2 + 1, false, maxDepth, moveStates, newState));
        else
            return min(minMax(currentDepth + 1, index * 2, true, maxDepth, moveStates, newState),
                       minMax(currentDepth + 1, index * 2 + 1, true, maxDepth, moveStates, newState));
    }
}

Move Search::NextMove(WorldState& state)
{
    // generate states
    vector<MoveState> moveStates;

    pair<int, int> pair = minMax(0, 0, true, 4, moveStates, state);

    for (auto state : moveStates)
    {
        cout << state.score << endl;
    }

    // Return best move
    return moveStates[pair.second].moves[0];
}
