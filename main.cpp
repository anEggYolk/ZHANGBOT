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
Bit 16–19	4	Promotion Pie=ce	0: none, 1: queen, 2: rook, 3: bishop, 4: knight
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

void FindCapture()
{
    list<uint32_t> PutCapture;
    list<int> PawnMoves={7,9};
    uint32_t tempCaptureStore=0ULL;
    for (int i=0;i<=63;i++)
    {
        if (wKnightBit&(1ULL<<i)!=0ULL)
        {

        }
        if ((wPawnBit&(1ULL<<i))!=0ULL) // add promtion
        {

        }
    }
}
void ValidPawn() {
    
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
