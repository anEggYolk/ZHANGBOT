// Created by lucas on 12/06/2025

#include <iostream>
#include "tests.h"

void printTestBoard(uint64_t wPawnBit, uint64_t wKnightBit, uint64_t wBishopBit,
                    uint64_t wRookBit, uint64_t wQueenBit, uint64_t wKingBit,
                    uint64_t bPawnBit, uint64_t bKnightBit, uint64_t bBishopBit,
                    uint64_t bRookBit, uint64_t bQueenBit, uint64_t bKingBit) {

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            uint64_t mask = (1ULL << square);
            char pieceChar = '.';

            if (wPawnBit   & mask) pieceChar = 'P';
            if (wKnightBit & mask) pieceChar = 'N';
            if (wBishopBit & mask) pieceChar = 'B';
            if (wRookBit   & mask) pieceChar = 'R';
            if (wQueenBit  & mask) pieceChar = 'Q';
            if (wKingBit   & mask) pieceChar = 'K';

            if (bPawnBit   & mask) pieceChar = 'p';
            if (bKnightBit & mask) pieceChar = 'n';
            if (bBishopBit & mask) pieceChar = 'b';
            if (bRookBit   & mask) pieceChar = 'r';
            if (bQueenBit  & mask) pieceChar = 'q';
            if (bKingBit   & mask) pieceChar = 'k';

            std::cout << pieceChar << " ";
        }
        std::cout << std::endl;
    }
}
