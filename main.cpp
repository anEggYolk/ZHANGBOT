#include <iostream>
#include <stdint.h>
#include <vector>
#include <string>
#include <bitset>
#include "tests.h"
#include <list>
#include <unordered_map>

using namespace std;

// Board + Peices + Empty Squares
uint64_t Board;
uint64_t BlackPieces;
uint64_t WhitePieces;
uint64_t emptySquares;

// White Pieces
uint64_t wPawnBit = 0ULL;
uint64_t wKnightBit = 0ULL;
uint64_t wBishopBit = 0ULL;
uint64_t wRookBit = 0ULL;
uint64_t wQueenBit = 0ULL;
uint64_t wKingBit = 0ULL;

// Black Pieces
uint64_t bPawnBit = 0ULL;
uint64_t bKnightBit = 0ULL;
uint64_t bBishopBit = 0ULL;
uint64_t bRookBit = 0ULL;
uint64_t bQueenBit = 0ULL;
uint64_t bKingBit = 0ULL;

// Move Hash
uint32_t HashMove = 0ULL;

// Capture "List"
list<uint32_t> StoreCaptures;

/*
Encoding: PPPPMMMMTTTTTTTTFFFFFF
Bit 0–5     6   From Square 0 to 63
Bit 6–11    6   To Square   0 to 63
Bit 12–15   4   Move Type   0: quiet, 1: capture, 2: en passant, 3: castle, etc.
Bit 16–19   4   Promotion Piece 0: none, 1: queen, 2: rook, 3: bishop, 4: knight
*/

// L---s add spaces pls

unordered_map<string, int> MovePriority;
void representBoard()
{
    for (int i = 8; i <= 15; i++)
    {
        //white pawns
        wPawnBit |= (1ULL << i);
    }
    //white pieces
    wKnightBit |= (1ULL << 1) | (1ULL << 6);
    wBishopBit |= (1ULL << 2) | (1ULL << 5);
    wRookBit |= (1ULL << 0) | (1ULL << 7);
    wQueenBit |= (1ULL << 3);
    wKingBit |= (1ULL << 4);
    //merge into white pieces
    WhitePieces = wPawnBit | wKingBit | wBishopBit | wRookBit | wQueenBit | wKnightBit;
    for (int i = 48; i <= 55; i++) //black pawns
    {
        bPawnBit |= (1ULL << i);
    }
    // Black major pieces (rank 8)
    bRookBit |= (1ULL << 56) | (1ULL << 63); // a8, h8
    bKnightBit |= (1ULL << 57) | (1ULL << 62); // b8, g8
    bBishopBit |= (1ULL << 58) | (1ULL << 61); // c8, f8
    bQueenBit |= (1ULL << 59);                // d8
    bKingBit |= (1ULL << 60);
    //merge into black pices
    BlackPieces = bPawnBit | bKingBit | bBishopBit | bRookBit | bQueenBit | bKnightBit;
    //merge into complete board
    Board = WhitePieces | BlackPieces;
    emptySquares = ~Board;
}

bool Evaluate(uint32_t Move) //true menas best and false means bad
{
    // todo.
    return false; // error/warn handle
}

int GetFile(int GridPos)
{
    return (GridPos % 8) + 1;
}

void FindCapture()
{
    list<uint32_t> PutCapture;
    list<int> PawnMoves = {7, 9};
    list<int> KnightMoves = {17, 10, -6, -15, -17, -10, 6, 15};
    list<int> BishopMoveStepSize = {-9, 9, 7, -7}; //use recursion for this kind of stuff
    for (int i = 0; i <= 63; i++)
    {
        if (wBishopBit & (1ULL << i))
        {
            list<int> DiffernceFileBishop = {1, -1};
            for (auto y : BishopMoveStepSize)
            {
                int z = i + y;
                while (z >= 0 && z <= 63)
                {
                    if (BlackPieces & (1ULL << (i + y)))
                    {
                    }
                    if ((z + y) < 0 || (z + y) > 63) break;
                    z += y;
                }
            }
        }
        if ((wKnightBit & (1ULL << i)) != 0ULL)
        {
            list<int> DiffernceFileKnight = {1, 2, -1, -2};
            for (auto ii : KnightMoves)
            {
                if ((BlackPieces & (1ULL << i)) != 0ULL)
                {
                    if ((i + ii) >= 0 && (i + ii) <= 63)
                    {
                        for (auto iii : DiffernceFileKnight)
                        {
                            if (GetFile(abs((i + ii))) - GetFile(abs(i)) == iii)
                            {
                                uint32_t WmoveKnight = 0ULL;
                                WmoveKnight |= (i & 0x3F);
                                WmoveKnight |= ((i + ii) & 0x3F) << 6;
                                WmoveKnight |= (1 << 12); // Assuming move type is capture (1)
                                WmoveKnight |= (0 << 16); // Assuming no promotion for knight captures
                                PutCapture.push_back(WmoveKnight);
                            }
                        }
                    }
                }
            }
        }
        if ((wPawnBit & (1ULL << i)) != 0ULL)//make promtoion more then just gurrantee queen
        {
            list<int> DiffernceFilePawn = {1, -1};
            for (auto ii : PawnMoves) {
                if ((BlackPieces & (1ULL << (i + ii))) != 0ULL)
                {
                    if (i + ii > 0 && i + ii <= 63)
                    {
                        for (auto iii : DiffernceFilePawn)
                        {
                            if (abs(GetFile(i + ii) - GetFile(i)) == iii)
                            {
                                uint32_t WmovePawn = 0ULL;
                                WmovePawn |= (i & 0x3F);
                                WmovePawn |= ((i + ii) & 0x3F) << 6;
                                WmovePawn |= (1 << 12);
                                WmovePawn |= (1 << 16);
                                PutCapture.push_back(WmovePawn);
                            }
                        }
                    }
                }
            }
        }
    }
}
uint32_t MovePicker(int beta)//beta is just like minum for mvoe to be cosndiered strong
{
    if (HashMove != 0ULL)
    {
        int score = Evaluate(HashMove);
        if (score >= beta)
        {
            return HashMove;
        }
    }
    // Return a default value or handle the case where HashMove is 0 or its score is not >= beta
    return 0;
}


int main()
{
    MovePriority["Hash"] = 10000;  // Highest priority: always try known good move first
    MovePriority["Capture"] = 8000;   // Try captures early—may lead to a cutoff
    MovePriority["Killer"] = 6000;   // Killer quiet moves from previous cutoffs
    MovePriority["Quiet"] = 1000;   // Regular non-capture, non-killer moves
    representBoard();
    printTestBoard(wPawnBit, wKnightBit, wBishopBit, wRookBit, wQueenBit, wKingBit, bPawnBit, bKnightBit, bBishopBit, bRookBit, bQueenBit, bKingBit);//generate board test
    return 0;
}