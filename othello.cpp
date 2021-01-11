#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>
#include <regex>

const int MINIMAX_DEPTH = 5; // depth of the game tree search
const bool DEBUG_MODE = false;

// flips appropriate pieces after a disc is placed down (called after verifying the move isFlippable)
void flip(char (&board)[8][8], int row, int col, char player){
    // declare a list of positions of discs that will be flipped
    // e.g. {{0,1}, {0,2}} means disc at location board[0][1] & board[0][2] will be flipped
    std::vector<std::vector<int>> discs_to_flip;

    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // use deltas to find all 8 surrounding positions
    int surroundingPosDeltas[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, // 3 positions above
                                      {0, -1}, {0, 1}, // 2 positions on same row
                                      {1, -1}, {1, 0}, {1, 1}}; // 3 positions below

    // for every delta representing a neighboring position...
    for(auto deltas : surroundingPosDeltas){
        //std::cout << "deltas: [" << deltas[0] << ", " << deltas[1] << "]" << '\n';

        // save what row/col currently on
        int curr_row = row + deltas[0];
        int curr_col = col + deltas[1];

        // ignore if this goes off of the board
        if(curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
            continue;


        // save character in this position
        char char_in_pos = board[curr_row][curr_col];

        // use this variable to save whether or not a line of pieces should be flipped
        bool flip_this_direction = false;

        // if the character in this delta position is the opponent's piece...
        if(char_in_pos == otherPlayer){
            //std::cout << "Found other player at location: [" << curr_row << ", " << curr_col << "], " << char_in_pos << '\n';

            // continue in this delta position until the next character is no longer the opponent's or you go off the board
            while(char_in_pos == otherPlayer){
                curr_row += deltas[0];
                curr_col += deltas[1];

                // check to see if new position is off board
                if(curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
                    break;

                // save the character
                char_in_pos = board[curr_row][curr_col];
            }

            // if the player's piece is found after traversing over the opponent's piece(s), we know we will be flipping
            if(char_in_pos == player)
                flip_this_direction = true;

            // if we found out we should be flipping...
            if(flip_this_direction){
                // save current position
                curr_row = row + deltas[0];
                curr_col = col + deltas[1];
                char_in_pos = board[curr_row][curr_col];

                // traverse over the opponent's pieces, while saving the positions to the big list to be flipped later
                while(char_in_pos == otherPlayer){
                    //std::cout << "flipping [" << curr_row << ", " << curr_col << "]\n";
                    std::vector<int> disc = {curr_row, curr_col};
                    discs_to_flip.push_back(disc);
                    curr_row += deltas[0];
                    curr_col += deltas[1];

                    // save next character
                    char_in_pos = board[curr_row][curr_col];
                }

            }
        }
    }

    // after we've collecting the row/col of all discs to flipped, flip them to the current player's color/character
    for(auto pos : discs_to_flip)
        board[pos[0]][pos[1]] = player;
}

// a move "isFlippable" if it causes pieces to flip
bool isFlippable(char board[8][8], int row, int col, char player) {
    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // Check all 8 surround positions
    int surroundingPosDeltas[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, // 3 positions above
                                      {0, -1}, {0, 1}, // 2 positions on same row
                                      {1, -1}, {1, 0}, {1, 1}}; // 3 positions below

    // for every delta of the surrounding positions
    for(auto deltas : surroundingPosDeltas){

        // skip if the position is off of game board
        if(row+deltas[0] > 7 || row+deltas[0] < 0 || col+deltas[1] > 7 || col+deltas[1] < 0){
            continue;
        }

        //std::cout << "deltas: [" << deltas[0] << ", " << deltas[1] << "]" << '\n';
        char char_in_pos = board[row+deltas[0]][col+deltas[1]]; // grab the character in that spot

        // if the character in this delta spot is the opponent's piece...
        if(char_in_pos == otherPlayer){
            // save spot's row and col #
            int curr_row = row + deltas[0];
            int curr_col = col + deltas[1];

            //std::cout << "Found other player at location: [" << curr_row << ", " << curr_col << "], " << char_in_pos << '\n';

            //continue along this delta trajectory until you stop seeing the opponent's pieces
            while(char_in_pos == otherPlayer){
                curr_row += deltas[0];
                curr_col += deltas[1];

                // check to see if new position is off board
                if(curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_row < 0)
                    break;

                // save the next character
                char_in_pos = board[curr_row][curr_col];
            }

            // if the player's piece is seen after one (+more) of the opponent's pieces, the original move is a flippable one
            if(char_in_pos == player)
                return true;
        }
    }

    // if no flippable spot is found after checking all surrounding positions, the original move is not a flippable one
    return false;
}

// set board[row][col] to player's piece, and flip appropriate pieces
void makeMove(char (&board)[8][8], int row, int col, char player){
    //std::cout << "Updating row: " << row << " col: " << col << '\n';
    // set provided row/col position to the player's character piece
    board[row][col] = player;

    // flip discs from resulting move
    flip(board, row, col, player);
}

// used to algorithmically calculate legal moves belonging to passed-in player
std::vector<std::vector<int>> calculateLegalMoves(char board[8][8], char player) {

    // declare main move list
    std::vector<std::vector<int>> move_list;

    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            // first make sure the spot is empty
            if(board[i][j] == '-'){

                // check to see if placing a piece there will flip one (+more) of the opponent's pieces
                if(isFlippable(board, i, j, player)){

                    // if so, create a 2-element vector representative of the move and push it to the big move list
                    std::vector<int> move = {i, j};
                    move_list.push_back(move);
                }

            }
        }
    }

    return move_list;

}

// for a given board configuration, determine if a move is legal (searches through a previously generated movelist)
bool isLegalMove(char board[8][8], std::vector<std::vector<int>> move_list, int row, int col, char player) {
    std::vector<int> proposedMove = {row, col};
//    for (int i : proposedMove) {
//        std::cout << i << ' ';
//    }

    //This error should NOT occur, as the regex pattern validates the user's input
    if(row > 7 || row < 0 || col > 7 || col < 0)
        throw std::range_error{"isLegalMove()"};

    // Make sure position is empty
    if(board[row][col] != '-'){
        return false;
    }

    if (std::find(move_list.begin(), move_list.end(), proposedMove) != move_list.end()){
        return true;
    }

    return false;
}

// return a list of all the moves available to black
std::vector<std::vector<int>> getBlackLegalMoves(char board[8][8]) {
    return calculateLegalMoves(board, 'b');
}

// return a list of all the moves available to white
std::vector<std::vector<int>> getWhiteLegalMoves(char board[8][8]) {
    return calculateLegalMoves(board, 'w');
}

// for the passed-in player, print all legal moves (displayed on board update)
void printLegalMoves(char board[8][8], char player){
    if(player == 'b'){
        std::cout << "Black legal moves:\n";
        auto v = getBlackLegalMoves(board);
        for ( const auto &vec : v ) {
            std::cout << "(" << vec[0]  << "," << vec[1] << ")  ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "White legal moves:\n";
        auto x = getWhiteLegalMoves(board);
        for ( const auto &vec : x ) {
            std::cout << "(" << vec[0]  << "," << vec[1] << ")  ";
        }
        std::cout << std::endl;
    }
}

// pass in a generated move list to "pretty print" them
void printLegalMoves(std::vector<std::vector<int>> move_list){
    for ( const auto &vec : move_list ){
        std::cout << "(" << vec[0]  << "," << vec[1] << ")  ";
    }
    std::cout << std::endl;
}

// overload the << operator to "pretty print" the board
std::ostream& operator<<(std::ostream& os, const char board[8][8]){
    std::cout << "   0  1  2  3  4  5  6  7\n";
    for(int i = 0; i < 8; ++i){
        std::cout << (i) << "  ";
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << "  ";
        }
        std::cout << '\n';
    }
    return os;
}

// used to determine if the game is ended, makes sure at least 1 player has a move to make
bool isGameOver(char board[8][8]){
    return getBlackLegalMoves(board).empty() && getWhiteLegalMoves(board).empty();
}

// go through whole board, and count pieces of passed-in player
int getScore(char board[8][8], char player){
    int total = 0;
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if(board[i][j] == player)
                total += 1;

    return total;
}

// "pretty print" the winner of the game at the end of the game loop
void printWinner(char (&board)[8][8]){
    int white_total = getScore(board, 'w');
    int black_total = getScore(board, 'b');

    std::cout << "Black total: " << black_total << '\n';
    std::cout << "White total: " << white_total << '\n';
    if(black_total == white_total){
        std::cout << "TIE GAME\n";
        return;
    }

    std::cout << ((black_total > white_total) ? "Black" : "White") << " wins!\n";
}

int main() 
{
    //**** Initialize Game Board *********
    char board[8][8];
    for(auto & i : board){
        for (char & j : i) {
            j = '-';
        }
    }

    board[3][3] = 'w'; board[3][4] = 'b';
    board[4][3] = 'b'; board[4][4] = 'w';
    //************************************

    int total_moves = 0;
    char player = 'b'; // black always goes first
    std::regex move_input_pattern("[0-7] [0-7]"); // regex for row/col input

        while(!isGameOver(board)){
            std::vector<std::vector<int>> move_list = calculateLegalMoves(board, player);

            std::cout << ((player == 'w') ? "White's Movelist: " : "Black's Movelist: \n");
            printLegalMoves(move_list);
            std::cout << board; // Show board

            if (player == 'b' && getBlackLegalMoves(board).size() == 0){
                //std::cout << "Black is out of moves, PASS to White.\n";
                player = 'w';
                continue;
            }

            if (player == 'w' && getWhiteLegalMoves(board).size() == 0){
                //std::cout << "White is out of moves, PASS to Black.\n";
                player = 'b';
                continue;
            }

            // Print input prompt
            std::cout << ((player == 'w') ? "White's Move: " : "Black's Move: ");

            std::string user_input;
            std::getline(std::cin, user_input);
            //std::cout << "You entered: " << user_input << '\n';

            if(!std::regex_match(user_input, move_input_pattern)){
                std::cout << "\nInvalid input: Moves are inputted as '<row #> <column #>' with numbers [1-8].\n";
                std::cout << "e.g. If you want to place your piece at row #1, column #2 input '1 2'.\n\n";
                continue;
            }

            // user_input = [<some num>, " ", <some num>], nums will be at indices 0 and 2
            // subtract '0's ascii value (48) from the user nums to get the real integer
            int row = user_input[0] - '0';
            int col = user_input[2] - '0';

            try{
                if(isLegalMove(board, move_list, row, col, player)){
                    makeMove(board, row, col, player);
                } else {
                    std::cout << "Illegal move! Try again.\n";
                    continue;
                }
            } catch(std::range_error& e){
                std::cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
                return 1;
            }

            total_moves += 1;
            int white_total = getScore(board, 'w');
            int black_total = getScore(board, 'b');

            std::cout << "Black total: " << black_total << '\n';
            std::cout << "White total: " << white_total << "\n\n";

            // Switch players
            player = (player == 'w') ? 'b' : 'w';
        }


    std::cout << board; // Show final board
    printWinner(board);
    return 0;
}