#include<iostream>
#include <stdint.h>
#include <bits/stdc++.h>
#include <bitset>
#include"Tests.h"
#include <list>
#include <unordered_map>
using namespace std;
uint64_t Board;
uint64_t BlackPieces;
uint64_t WhitePieces;
uint64_t emptySquares;
uint64_t wPawnBit,wKnightBit,wBishopBit,wRookBit,wQueenBit,wKingBit=0ULL;//instatnie white pieces
uint64_t bPawnBit,bKnightBit,bBishopBit,bRookBit,bQueenBit,bKingBit=0ULL; //instantie black pieces
uint32_t HashMove=0ULL;
list<uint32_t> StoreCaptures;
/* Bit 0–5	6	From Square	0 to 63
Bit 6–11	6	To Square	0 to 63
Bit 12–15	4	Move Type	0: quiet, 1: capture, 2: en passant, 3: castle, etc.
Bit 16–19	4	Promotion Piece	0: none, 1: queen, 2: rook, 3: bishop, 4: knight
*/

unordered_map<string, int> MovePriority;
void representBoard()
{
    for (int i=8;i<=15;i++) {
        //white pawns
        wPawnBit|=(1ULL<<i);
    }
    //white pieces
    wKnightBit|=(1ULL<<1)|(1ULL<<6);
    wBishopBit|=(1ULL<<2)|(1ULL<<5);
    wRookBit|=(1ULL<<0)|(1ULL<<7);
    wQueenBit|=(1ULL<<3);
    wKingBit|=(1ULL<<4);
    //merge into white pieces
    WhitePieces =wPawnBit|wKingBit|wBishopBit|wRookBit|wQueenBit|wKnightBit;
    for (int i=48;i<=55;i++) //black pawns
    {
         bPawnBit|=(1ULL<<i);
    }
    // Black major pieces (rank 8)
    bRookBit   |= (1ULL << 56) | (1ULL << 63); // a8, h8
    bKnightBit |= (1ULL << 57) | (1ULL << 62); // b8, g8
    bBishopBit |= (1ULL << 58) | (1ULL << 61); // c8, f8
    bQueenBit  |= (1ULL << 59);                // d8
    bKingBit   |= (1ULL << 60);
    //merge into black pices
    BlackPieces =bPawnBit|bKingBit|bBishopBit|bRookBit|bQueenBit|bKnightBit;
    //merge into complete board
    Board= WhitePieces | BlackPieces;
    emptySquares=~Board;
}

bool Evaluate(uint32_t Move) //true menas best and false means bad
{

}
int GetFile(int GridPos)
{
    return (GridPos % 8) + 1;
}
list<uint32_t> FindCaptureForWhite()
{
    list<uint32_t> PutCapture;
    list<int> PawnMoves={7,9};
    list<int> KnightMoves={17, 10, -6, -15,-17, -10, 6, 15};
    list<int> BishopMoveStepSize={-9,9,7,-7}; //use recursion for this kind of stuff
    list<int> RookMoveStepSize={-8, -1, 1, 8};
    list<int> QueenMoveStepSize={-8,-1,8,-9,-7,  7,  9  };
    for (int i=0;i<=63;i++)
    {
        if (wQueenBit&(1ULL<<i))
        {
            for (auto ii :QueenMoveStepSize)
            {
                int z=ii+i;
                while (z>=0&&z<=63)
                {
                    if (abs(GetFile(z) - GetFile(z - ii)) > 1) break;

                    {
                        if (BlackPieces&(1ULL<<z))
                        {
                            uint32_t wQueenMove=0ULL;
                            wQueenMove|=(i&0x3F);
                            wQueenMove|=(z&0x3F)<<6;
                            wQueenMove|=(1&0x3F)<<12;
                            wQueenMove|=(0&0x3F)<<16;
                            PutCapture.push_back(wQueenMove);
                            break;
                        }
                    }
                    z+=ii;
                }
            }
        }
        if (wRookBit&(1ULL<<i))
        {
            for (auto ii:RookMoveStepSize)
            {
                int z=ii+i;
                while (z>=0&&z<=63)
                {
                    if (abs(GetFile(z) - GetFile(z - ii)) > 1) break;


                    {
                        if (BlackPieces&(1ULL<<z))
                        {
                            uint32_t wRookMove=0ULL;
                            wRookMove|=(i&0x3F);
                            wRookMove|=(z&0x3F)<<6;
                            wRookMove|=(1&0x3F)<<12;
                            wRookMove|=(0&0x3F)<<16;
                            PutCapture.push_back(wRookMove);
                            break;
                        }
                        z+=ii;
                    }
                }
            }
        }
        if (wBishopBit&(1ULL<<i))
        {
            for (auto y:BishopMoveStepSize)
            {
                int z=i+y;
                while (z>=0&&z<=63)
                {
                    if (abs(GetFile(z) - GetFile(z - y)) > 1) break;

                    {
                        if (BlackPieces&(1ULL<<z))
                        {
                            uint32_t WmoveBishop=0ULL;
                            WmoveBishop|=(i&0x3F);
                            WmoveBishop|=(z&0x3F)<<6;
                            WmoveBishop|=(1&0xF)<<12;
                            WmoveBishop|=(1&0xF)<<16;
                            PutCapture.push_back(WmoveBishop);
                            break;
                        }
                        z += y;
                    }
                }
            }
        }
        if ((wKnightBit&(1ULL<<i))!=0ULL)
        {
            for (auto ii:KnightMoves)
            {
                if (BlackPieces & (1ULL << (i + ii)) != 0ULL)

                {
                    if ((i+ii)>=0&&(i+ii)<=63)
                    {
                            if (abs(GetFile(i + ii) - GetFile(i)) == 1 || 2)
                            {
                                uint32_t WmoveKnight=0ULL;
                                WmoveKnight |=(i&0x3F);
                                WmoveKnight|=((i+ii)&0x3F)<<6;
                                WmoveKnight|=(1&0xF)<<12;
                                WmoveKnight|=(0&0xF)<<16;
                                PutCapture.push_back(WmoveKnight);
                            }
                    }
                }
            }
        }
        if ((wPawnBit&(1ULL<<i))!=0ULL)//make promtoion more then  just gurrantee queen
        {
            for (auto ii:PawnMoves) {
                if (BlackPieces&(1ULL<<(i+ii))!=0ULL)
                {
                    if (i+ii>0&&i+ii<=63)
                    {

                            if (abs(GetFile(i + ii) - GetFile(i)) == 1)
                            {
                                uint32_t WmovePawn =0ULL;
                                WmovePawn |=(i&0x3F);
                                WmovePawn |= ((i+ii)&0x3F)<<6;
                                WmovePawn |=(1&0xF)<<12;
                                WmovePawn |=(1&0xF)<<16;
                                PutCapture.push_back(WmovePawn);
                            }

                    }
                }
            }
        }
    }
    return PutCapture;
}
uint32_t MovePicker(int beta)//beta is just like minum for mvoe to be cosndiered strong
{
    if (HashMove!=0ULL)
    {
        int score=Evaluate(HashMove);
        if (score >= beta)
        {
            return HashMove;
        }
    }

}


int main()
{
    MovePriority["Hash"]    = 10000;  // Highest priority: always try known good move first
    MovePriority["Capture"] = 8000;   // Try captures early—may lead to a cutoff
    MovePriority["Killer"]  = 6000;   // Killer quiet moves from previous cutoffs
    MovePriority["Quiet"]   = 1000;   // Regular non-capture, non-killer moves
    representBoard();
    printTestBoard(wPawnBit,wKnightBit,wBishopBit,wRookBit,wQueenBit,wKingBit,bPawnBit,bKnightBit,bBishopBit,bRookBit,bQueenBit,bKingBit);//generate board test
    return 0;
}
