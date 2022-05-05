
#ifndef AUTOCHESS_AI_H
#define AUTOCHESS_AI_H

enum PlayerTurn {
    WHITE = true, BLACK = false
};

class AI {
private:

    PlayerTurn &turn;

public:
    AI(PlayerTurn &turn);

    int minimax(int position, int depth, double alpha, double beta, PlayerTurn playerTurn);
};


#endif //AUTOCHESS_AI_H
