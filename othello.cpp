#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>
#include <regex>
#include<stdlib.h>
using namespace std;

const int MINIMAX_DEPTH = 5; // depth of the game tree search
const bool DEBUG_MODE = false;

// flips appropriate pieces after a disc is placed down (called after verifying the move isFlippable)
void flip(char (&board)[8][8], int row, int col, char player){
    // declare a list of positions of discs that will be flipped
    // e.g. {{0,1}, {0,2}} means disc at location board[0][1] & board[0][2] will be flipped
    vector<vector<int>> discs_to_flip;

    char otherPlayer = (player == 'b') ? 'w' : 'b';

    // use deltas to find all 8 surrounding positions
    int surroundingPosDeltas[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, // 3 positions above
                                      {0, -1}, {0, 1}, // 2 positions on same row
                                      {1, -1}, {1, 0}, {1, 1}}; // 3 positions below

    // for every delta representing a neighboring position...
    for(auto deltas : surroundingPosDeltas){
        //cout << "deltas: [" << deltas[0] << ", " << deltas[1] << "]" << '\n';

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
            //cout << "Found other player at location: [" << curr_row << ", " << curr_col << "], " << char_in_pos << '\n';

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
                    //cout << "flipping [" << curr_row << ", " << curr_col << "]\n";
                    vector<int> disc = {curr_row, curr_col};
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

        //cout << "deltas: [" << deltas[0] << ", " << deltas[1] << "]" << '\n';
        char char_in_pos = board[row+deltas[0]][col+deltas[1]]; // grab the character in that spot

        // if the character in this delta spot is the opponent's piece...
        if(char_in_pos == otherPlayer){
            // save spot's row and col #
            int curr_row = row + deltas[0];
            int curr_col = col + deltas[1];

            //cout << "Found other player at location: [" << curr_row << ", " << curr_col << "], " << char_in_pos << '\n';

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
    //cout << "Updating row: " << row << " col: " << col << '\n';
    // set provided row/col position to the player's character piece
    board[row][col] = player;

    // flip discs from resulting move
    flip(board, row, col, player);
}

// used to algorithmically calculate legal moves belonging to passed-in player
vector<vector<int>> calculateLegalMoves(char board[8][8], char player) {

    // declare main move list
    vector<vector<int>> move_list;

    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            // first make sure the spot is empty
            if(board[i][j] == '-'){

                // check to see if placing a piece there will flip one (+more) of the opponent's pieces
                if(isFlippable(board, i, j, player)){

                    // if so, create a 2-element vector representative of the move and push it to the big move list
                    vector<int> move = {i, j};
                    move_list.push_back(move);
                }

            }
        }
    }

    return move_list;

}

bool isLegalMove(char board[8][8], vector<vector<int>> move_list, int row, int col, char player) {
    vector<int> proposedMove = {row, col};
//    for (int i : proposedMove) {
//        cout << i << ' ';
//    }

    if(row > 7 || row < 0 || col > 7 || col < 0)
        throw range_error{"isLegalMove()"};

    if(board[row][col] != '-'){
        return false;
    }

    if (find(move_list.begin(), move_list.end(), proposedMove) != move_list.end()){
        return true;
    }

    return false;
}

vector<vector<int>> getBlackLegalMoves(char board[8][8]) {
    return calculateLegalMoves(board, 'b');
}

vector<vector<int>> getWhiteLegalMoves(char board[8][8]) {
    return calculateLegalMoves(board, 'w');
}

void printLegalMoves(char board[8][8], char player){
    if(player == 'b'){
        cout << "Black legal moves:\n";
        auto v = getBlackLegalMoves(board);
        for ( const auto &vec : v ) {
            cout << "(" << vec[0]  << "," << vec[1] << ")  ";
        }
        cout << endl;
    } else {
        cout << "White legal moves:\n";
        auto x = getWhiteLegalMoves(board);
        for ( const auto &vec : x ) {
            cout << "(" << vec[0]  << "," << vec[1] << ")  ";
        }
        cout << endl;
    }
}

void printLegalMoves(vector<vector<int>> move_list){
    for ( const auto &vec : move_list ){
        cout << "(" << vec[0]  << "," << vec[1] << ")  ";
    }
    cout << endl;
}

ostream& operator<<(ostream& os, const char board[8][8]){
    cout << "   0  1  2  3  4  5  6  7\n";
    for(int i = 0; i < 8; ++i){
        cout << (i) << "  ";
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j] << "  ";
        }
        cout << '\n';
    }
    return os;
}

bool isGameOver(char board[8][8]){
    return getBlackLegalMoves(board).empty() && getWhiteLegalMoves(board).empty();
}

int getScore(char board[8][8], char player){
    int total = 0;
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if(board[i][j] == player)
                total += 1;

    return total;
}

void printWinner(char (&board)[8][8]){
    int white_total = getScore(board, 'w');
    int black_total = getScore(board, 'b');

    cout << "Black total: " << black_total << '\n';
    cout << "White total: " << white_total << '\n';
    if(black_total == white_total){
        cout << "TIE GAME\n";
        return;
    }

    cout << ((black_total > white_total) ? "Black" : "White") << " wins!\n";
}

int heuristic(char board[8][8]){

    int b_total = 0;
    int w_total = 0;

    b_total += getBlackLegalMoves(board).size();
    w_total += getWhiteLegalMoves(board).size();

    b_total += getScore(board, 'b');
    w_total += getScore(board, 'w');

    if(board[0][0] == 'w'){
        w_total += 10;
    }
    if(board[0][0] == 'b'){
        b_total += 10;
    }
    if(board[7][0] == 'w'){
        w_total += 10;
    }
    if(board[7][0] == 'b'){
        b_total += 10;
    }
    if(board[0][7] == 'w'){
        w_total += 10;
    }
    if(board[0][7] == 'b'){
        b_total += 10;
    }
    if(board[7][7] == 'w'){
        w_total += 10;
    }
    if(board[7][7] == 'b'){
        b_total += 10;
    }

    return (b_total-w_total);
}

struct Node
{
    Node ** children;
    int child_count;
    vector<vector<int>> move_list;
    char state[8][8];
    int val;
};

Node * CreateTree(char board[8][8], int depth, char player)
{
    Node * node = new Node();

    node->move_list = (player == 'w') ? getWhiteLegalMoves(board) : getBlackLegalMoves(board);

    node->child_count = node->move_list.size();

    memcpy(node->state, board, 8 * 8 * sizeof(char));

    char other_player = (player == 'w') ? 'b' : 'w';

    if (depth > 0 && node->child_count > 0) {
        node->children = new Node * [node->child_count];

        for (int i = 0; i < node->child_count; ++i){
            char tmp_board[8][8];
            memcpy(tmp_board, board, 8 * 8 * sizeof(char));

            makeMove(tmp_board, node->move_list[i][0], node->move_list[i][1], player);

            node->children[i] = CreateTree(tmp_board, depth - 1, other_player);
        }
    } else {
        node->children = NULL;
    }

    return node;
}

int minimax(Node *position, int depth, int alpha, int beta, bool maximizing_player){

    if(depth == 0 || isGameOver(position->state)){
        //cout<< "returning heursitic: " << heuristic(position->state) << '\n';
        return heuristic(position->state);
    }

    if(maximizing_player){
        int max_eval = -9999999; 

        for(int i = 0; i < position->child_count; ++i){
            int eval = minimax(position->children[i], depth - 1, alpha, beta, false);
            max_eval = max(max_eval, eval); 

            alpha = max(alpha, eval);
            if(beta <= alpha) {
                if (DEBUG_MODE) {
                    cout << "DEBUG: PRUNED " << (position->child_count - (i+1)) << " children.\n";
                }
                break;
            }
        }
        position->val = max_eval;
        return max_eval;
    } else {
        int min_eval = 9999999;
        for(int i = 0; i < position->child_count; ++i){
            int eval = minimax(position->children[i], depth -1, alpha, beta, true);
            min_eval = min(min_eval, eval); 

            beta = min(beta, eval);
            if(beta <= alpha)
                break;
        }
        position->val = min_eval; 
        return min_eval;
    }
}


int main() 
{
    system ("clear");

    char board[8][8];
    for(auto & i : board){
        for (char & j : i) {
            j = '-';
        }
    }

    board[3][3] = 'w'; board[3][4] = 'b';
    board[4][3] = 'b'; board[4][4] = 'w';

    int total_moves = 0;
    char player = 'b';
    regex move_input_pattern("[0-7] [0-7]");

        regex player_selection_pattern("w|b"); 
        cout << "Enter 'b' to play as black or 'w' to play as white: ";
        string selected_player;
        while(true)
        {
            getline(cin, selected_player);
            if(!regex_match(selected_player, player_selection_pattern)){
                system ("clear");
                cout << "\nInvalid input: \"Enter 'b' to be black or 'w' to be white. \n";

continue;
            }
            break;
        }

        char player_char = selected_player[0];
        cout << "You have chosen to play as " << ((player_char == 'w') ? "white" : "black") << "!\n\n";

        char ai_char = ((player_char == 'w') ? 'b' : 'w');
        int count = 0;

        while(!isGameOver(board))
        {

            vector<vector<int>> move_list = calculateLegalMoves(board, player);

            if (player == 'b' && getBlackLegalMoves(board).empty()){
                player = 'w';
                continue;
            }

            if (player == 'w' && getWhiteLegalMoves(board).empty()){
                player = 'b';
                continue;
            }

            int white_total = getScore(board, 'w');
            int black_total = getScore(board, 'b');

            cout << "Black total: " << black_total << '\n';
            cout << "White total: " << white_total << '\n';

            cout << board; 
            cout << '\n';
            if(player == player_char)
            {
                printLegalMoves(board, player_char);

                string user_input;
                while(true){
                    cout << ((player == 'w') ? "Your move (w): " : "Your move (b): ");
                    getline(cin, user_input);

                    if(!regex_match(user_input, move_input_pattern))
                    {
                        cout << "\nInvalid input: Moves are inputted as '<row #> <column #>' with numbers [0-7].\n";
                        cout << "e.g. If you want to place your piece at row #1, column #2 input '1 2'.\n\n";
                        continue;
                    } 

                    else
                    {
                        int row = user_input[0] - '0';
                        int col = user_input[2] - '0';



                        try{
                            if(isLegalMove(board, move_list, row, col, player)){
                                makeMove(board, row, col, player);
                            } else {
                                cout << "Illegal move! Try again.\n";
                                continue;
                            }
                        } catch(range_error& e){
                            cout << e.what() << " - attempted access to element outside of game board, modification after initial input";
                            return 1;
                        }
                        break;
                    }

                }

            } 
            else 
            {
                    auto gametree = CreateTree(board, MINIMAX_DEPTH, player);
                    bool maximizer = (player == 'b') ? true : false;

                    int optimial_val = minimax(gametree, MINIMAX_DEPTH, -99999999, 99999999, maximizer);

                    if(DEBUG_MODE){
                        cout << "DEBUG: AI considered " << gametree->child_count << " initial moves for this board configuration.\n";
                        printLegalMoves(gametree->move_list);
                        for(int i = 0; i < gametree->child_count; ++i){
                            cout << "\t" << i << "th node's heuristic value = " << gametree->children[i]->val << '\n';
                        }
                        cout << '\n';
                    }

                    for(int i = 0; i < gametree->child_count; ++i){
                        //cout << gametree->children[i]->val << '\n';
                        if(gametree->children[i]->val == optimial_val){
                            bool same_config = true;
                            for(int j = 0; j < 7; ++j){
                                for(int k = 0; k < 7; ++k){
                                    if(gametree->children[i]->state[j][k] != board[j][k])
                                        same_config = false;
                                }
                            }
                            //cout << "the " << i << "th child of gametree has the optimizal value.\n";

                            if(!same_config)
                                memcpy(board, gametree->children[i]->state, 8 * 8 * sizeof(char));
                            else{
                                makeMove(board, move_list[0][0], move_list[0][1], player);
                            }
                            break;
                        }
                    }
            }

            total_moves += 1;
            //cout << '\n' << gb;

            player = (player == 'w') ? 'b' : 'w';

        }
    cout << board;
    printWinner(board);
    return 0;
}
