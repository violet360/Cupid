#include <bits/stdc++.h>
#define FAST_CODE                     \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);
typedef long long ll;
using namespace std;

class Game
{

public:
    void initBoard(char board[][8])
    {
        for (ll i = 0; i < 8; i++)
            for (ll j = 0; j < 8; j++)
                board[i][j] = '.';
        board[3][3] = 'X';
        board[3][4] = 'O';
        board[4][3] = 'O';
        board[4][4] = 'X';
    }

    void displayBoard(char board[][8])
    {
        for (ll y = 7; y >= 0; y--)
        {
            cout << y << " ";
            for (ll x = 0; x < 8; x++)
                cout << " " << board[x][y];
            cout << endl;
        }
        cout << "   0 1 2 3 4 5 6 7" << endl;
    }

    bool checkFlip(char board[][8], ll x, ll y, ll deltaX, ll deltaY,
                   char myPiece, char opponentPiece)
    {
        if (board[x][y] == opponentPiece)
        {
            while ((x >= 0) && (x < 8) && (y >= 0) && (y < 8))
            {
                x += deltaX;
                y += deltaY;

                if ((x >= 0) && (x < 8) && (y >= 0) && (y < 8))
                {
                    if (board[x][y] == '.')
                        return false;
                    if (board[x][y] == myPiece)
                        return true;
                    else
                    {
                    }
                }
            }
        }
        return false;
    }

    void flipPieces(char board[][8], ll x, ll y, ll deltaX, ll deltaY, char myPiece, char opponentPiece)
    {
        while (board[x][y] == opponentPiece)
        {
            board[x][y] = myPiece;
            x += deltaX;
            y += deltaY;
        }
    }

    void makeMove(char board[][8], ll x, ll y, char piece)
    {
        board[x][y] = piece;

        char opponent = 'O';
        if (piece == 'O')
            opponent = 'X';

        if (checkFlip(board, x - 1, y, -1, 0, piece, opponent))
            flipPieces(board, x - 1, y, -1, 0, piece, opponent);

        if (checkFlip(board, x + 1, y, 1, 0, piece, opponent))
            flipPieces(board, x + 1, y, 1, 0, piece, opponent);

        if (checkFlip(board, x, y - 1, 0, -1, piece, opponent))
            flipPieces(board, x, y - 1, 0, -1, piece, opponent);

        if (checkFlip(board, x, y + 1, 0, 1, piece, opponent))
            flipPieces(board, x, y + 1, 0, 1, piece, opponent);

        if (checkFlip(board, x - 1, y - 1, -1, -1, piece, opponent))
            flipPieces(board, x - 1, y - 1, -1, -1, piece, opponent);

        if (checkFlip(board, x + 1, y - 1, 1, -1, piece, opponent))
            flipPieces(board, x + 1, y - 1, 1, -1, piece, opponent);

        if (checkFlip(board, x - 1, y + 1, -1, 1, piece, opponent))
            flipPieces(board, x - 1, y + 1, -1, 1, piece, opponent);

        if (checkFlip(board, x + 1, y + 1, 1, 1, piece, opponent))
            flipPieces(board, x + 1, y + 1, 1, 1, piece, opponent);
    }

    bool validMove(char board[][8], ll x, ll y, char piece)
    {
        if (board[x][y] != '.')
            return false;

        char opponent = 'O';
        if (piece == 'O')
            opponent = 'X';

        if (checkFlip(board, x - 1, y, -1, 0, piece, opponent))
            return true;

        if (checkFlip(board, x + 1, y, 1, 0, piece, opponent))
            return true;

        if (checkFlip(board, x, y - 1, 0, -1, piece, opponent))
            return true;

        if (checkFlip(board, x, y + 1, 0, 1, piece, opponent))
            return true;

        if (checkFlip(board, x - 1, y - 1, -1, -1, piece, opponent))
            return true;

        if (checkFlip(board, x + 1, y - 1, 1, -1, piece, opponent))
            return true;

        if (checkFlip(board, x - 1, y + 1, -1, 1, piece, opponent))
            return true;

        if (checkFlip(board, x + 1, y + 1, 1, 1, piece, opponent))
            return true;

        return false;
    }

    void getMoveList(char board[][8], ll moveX[], ll moveY[], ll &numMoves, char piece)
    {
        numMoves = 0;

        for (ll x = 0; x < 8; x++)
            for (ll y = 0; y < 8; y++)
            {
                if (validMove(board, x, y, piece))
                {
                    moveX[numMoves] = x;
                    moveY[numMoves] = y;
                    numMoves++;
                }
            }
    }

    bool gameOver(char board[][8])
    {
        ll XMoveX[60], XMoveY[60], OMoveX[60], OMoveY[60];
        ll numXMoves, numOMoves;

        getMoveList(board, XMoveX, XMoveY, numXMoves, 'X');
        getMoveList(board, OMoveX, OMoveY, numOMoves, 'O');
        if ((numXMoves == 0) && (numOMoves == 0))
            return true;
        return false;
    }

    void getMove(char board[][8], ll &x, ll &y, char piece)
    {

        ll moveX[60], moveY[60], numMoves;
        getMoveList(board, moveX, moveY, numMoves, piece);
        if (numMoves == 0)
        {
            x = -1;
            y = -1;
        }
        else
        {
            cout << "their are " << numMoves << " moves"
                 << "\n";
            for (ll i = 0; i < numMoves; ++i)
            {
                cout << " (" << moveX[i] << "," << moveY[i] << ") "
                     << "\n";
            }

            cout << "enter a valid move \n";

            ll des = 0;

            do
            {
                                    cout << "---"<<endl;

                cin >> x >> y;
                des = 0;

                des = 0;

                for (ll i = 0; i < numMoves; ++i)
                {
                    if (moveX[i] == x)
                    {
                        des = 1;
                        break;
                    }
                }

            } while (des != 1);
        }
    }

    ll score(char board[][8], char piece)
    {
        ll total = 0;
        for (ll x = 0; x < 8; x++)
            for (ll y = 0; y < 8; y++)
            {
                if (board[x][y] == piece)
                    total++;
            }
        return total;
    }

    void copyBoard(char src[][8], char dest[][8])
    {
        memcpy(dest, src, 8 * 8 * sizeof(char));
    }

    ll heuristic(char board[][8], char whoseTurn)
    {
        char opponent = 'O';
        if (whoseTurn == 'O')
            opponent = 'X';
        ll ourScore = score(board, whoseTurn);
        ll opponentScore = score(board, opponent);
        return (ourScore - opponentScore);
    }

    void makeDes(char board[][8], char whoseTurn, ll &x, ll &y, bool desicion)
    {
        if (desicion)
        {
            minimaxDecision(board, whoseTurn, x, y);
        }

        else
        {
            getMove(board, x, y, whoseTurn);
        }
    }

    void minimaxDecision(char board[][8], char whoseTurn, ll &x, ll &y)
    {
        ll moveX[60], moveY[60];
        ll numMoves;
        char opponent = 'X';
        if (whoseTurn == 'X')
            opponent = 'O';

        getMoveList(board, moveX, moveY, numMoves, whoseTurn);
        if (numMoves == 0) // if no moves return -1
        {
            x = -1;
            y = -1;
        }
        else
        {
            ll bestMoveVal = -99999;
            ll bestX = moveX[0], bestY = moveY[0];

            for (ll i = 0; i < numMoves; i++)
            {
                char tempBoard[8][8];
                copyBoard(board, tempBoard);
                makeMove(tempBoard, moveX[i], moveY[i], whoseTurn);
                // Recursive call
                ll alpha = -99999999;
                ll beta = 99999999;
                ll val = minimaxValue(tempBoard, alpha, beta, whoseTurn, opponent, 1);
                // Remember best move
                if (val > bestMoveVal)
                {
                    bestMoveVal = val;
                    bestX = moveX[i];
                    bestY = moveY[i];
                }
            }
            // Return the best x/y
            x = bestX;
            y = bestY;
        }
    }

    ll minimaxValue(char board[][8], ll alpha, ll beta, char originalTurn, char currentTurn, ll searchPly)
    {
        if ((searchPly == 5) || gameOver(board))
        {
            return heuristic(board, originalTurn);
        }
        ll moveX[60], moveY[60];
        ll numMoves;
        char opponent = 'X';
        if (currentTurn == 'X')
            opponent = 'O';

        getMoveList(board, moveX, moveY, numMoves, currentTurn);
        if (numMoves == 0)
        {
            return minimaxValue(board, alpha, beta, originalTurn, opponent, searchPly + 1);
        }
        else
        {
            ll bestMoveVal = -99999;
            if (originalTurn != currentTurn)
                bestMoveVal = 99999;

            for (ll i = 0; i < numMoves; i++)
            {

                char tempBoard[8][8];
                copyBoard(board, tempBoard);
                makeMove(tempBoard, moveX[i], moveY[i], currentTurn);

                ll val = minimaxValue(tempBoard, alpha, beta, originalTurn, opponent, searchPly + 1);

                if (originalTurn == currentTurn)
                {

                    if (val > bestMoveVal)
                        bestMoveVal = val;

                    alpha = max(alpha, val);
                    if (beta <= alpha)
                        break;
                }
                else
                {
                    if (val < bestMoveVal)
                        bestMoveVal = val;

                    beta = min(beta, val);
                    if (beta <= alpha)
                        break;
                }
            }
            return bestMoveVal;
        }
        return -1;
    }
};

int main()
{
    FAST_CODE
    srand(time(NULL));
    char board[8][8];
    char curPlayer = 'X';

    Game g;
    g.initBoard(board);

    while (!(g.gameOver(board)))
    {
        g.displayBoard(board);
        cout << "It is player " << curPlayer << "'s turn." << endl;
        cout << "Enter move..." << endl;


        ll x, y;

        if (curPlayer == 'O')
        {
            //cin >> x >> y;
            g.makeDes(board, curPlayer, x, y, true); //true if you want AI to play
            // g.minimaxDecision(board, 'O', x, y);
        }
        else if (curPlayer == 'X')
        {


            g.getMove(board, x, y, 'X');
        }
        // g.makeDes()
        //minimaxDecision(board, 'X', x, y);
        if (g.validMove(board, x, y, curPlayer) || (x == -1))
        {
            cout << "Moving to " << x << " " << y << endl;
            // Use -1 if no move possible
            if (x != -1)
                g.makeMove(board, x, y, curPlayer);
            if (curPlayer == 'X')
                curPlayer = 'O';
            else
                curPlayer = 'X';
        }
        else
        {
            cout << "Invalid move.  Enter move again. " << endl;
        }
    }
    cout << endl
         << "The game is over!" << endl;
    g.displayBoard(board);
    cout << "X's score: " << g.score(board, 'X') << endl;
    cout << "O's score: " << g.score(board, 'O') << endl;
    system("pause");
}
