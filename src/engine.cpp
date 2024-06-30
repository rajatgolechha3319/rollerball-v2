
#include <algorithm>
#include <random>
#include <iostream>
#include <thread>
#include <climits>
#include <ctime>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <map>
#include <string>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

using namespace std;

random_device rd;
mt19937 rng(rd());

std::unordered_map<std::string, int> exist_move;
std::unordered_map<std::string, std::string> move_to_play;

PlayerColor pcGlobal = WHITE;
int moves_done = 0;

void fill(){
    
    string m1 = "73212041403031454625263635w";
    exist_move[m1] = 100; 
    move_to_play[m1] = "c2b3";

    m1 = "73122041403031554625263635w";
    exist_move[m1] = 100;
    move_to_play[m1] = "e2f2";

    m1 = "73122041403031564625263635w";
    exist_move[m1] = 100;
    move_to_play[m1] = "e2f2";

    m1 = "73112041400331454615263635b";
    exist_move[m1] = 100;
    move_to_play[m1] = "e6f6";

    m1 = "73122041403031454625263635b";
    exist_move[m1] = 100;
    move_to_play[m1] = "e6f6";

    m1 = "73112041403031454625263635b";
    exist_move[m1] = 100;
    move_to_play[m1] = "c6b6";

    // opening for 8-4 white
    m1 = "8421205150414030312627565736374746w";
    exist_move[m1] = 100;
    move_to_play[m1] = "c1b2";

    m1 = "8421115150414030312627565736374746b";
    exist_move[m1] = 100;
    move_to_play[m1] = "f8g7";

    m1 = "8421105150414030312627565736374746b";
    exist_move[m1] = 100;
    move_to_play[m1] = "f8g7";

    m1 = "8412205150414030312627565736374746b";
    exist_move[m1] = 100;
    move_to_play[m1] = "f8g7";

    m1 = "8411205150414030312627565736374746b";
    exist_move[m1] = 100;
    move_to_play[m1] = "f8g7";

    //opening database for 8-2 white
    m1 = "822120225251504241313256572555262735364645w";
    exist_move[m1] = 100;
    move_to_play[m1] = "d2c4";

    m1 = "822120225251504241233256572555262735364645b";
    exist_move[m1] = 100;
    move_to_play[m1] = "c7b7";

    // m1 = "822120225251504241233256572555162735364645w";
    // exist_move[m1] = 100;
    // move_to_play[m1] = "c1b2";

}

double eval_board_73(const Board& b1,int depth){
    double score = 0.0;
    // add piece weights for white pieces
    double PawnValue = 250.0;
    double RookValue = 550.0;
    double BishopValue = 450.0;
    // if(moves_done < 3){
    //     RookValue = 450.0;
    //     BishopValue = 550.0;
    // }
    double KingValue = 10000.0;
    if( b1.data.w_rook_1 != DEAD){
        score += RookValue;
    }
    if( b1.data.w_rook_2 != DEAD){
        score += RookValue;
    }
    if( b1.data.w_king != DEAD){
        score += KingValue;
    }
    if( b1.data.w_bishop != DEAD){
        score += BishopValue;
    }
    if( b1.data.w_pawn_1 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_1] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_1] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        int r = getx(b1.data.w_pawn_1);
        int c = gety(b1.data.w_pawn_1);
        int dist = min((abs(r-5)+abs(c-6)),abs(r-5)+abs(c-7));
        score -= 5*dist;
    }
    if( b1.data.w_pawn_2 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_2] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_2] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        int r = getx(b1.data.w_pawn_2);
        int c = gety(b1.data.w_pawn_2);
        int dist = min((abs(r-5)+abs(c-6)),abs(r-5)+abs(c-7));
        score -= 5*dist;
    }
    // add piece weights for black pieces
    if( b1.data.b_rook_1 != DEAD){
        score -= RookValue;
    }
    if( b1.data.b_rook_2 != DEAD){
        score -= RookValue;
    }
    if( b1.data.b_king != DEAD){
        score -= KingValue;
    }
    if( b1.data.b_bishop != DEAD){
        score -= BishopValue;
    }
    if( b1.data.b_pawn_1 != DEAD){
        score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_1] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_1] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        int r = getx(b1.data.b_pawn_1);
        int c = gety(b1.data.b_pawn_1);
        int dist = min((abs(r-2)+abs(c-0)),abs(r-2)+abs(c-1));
        score += 5*dist;
    }
    if( b1.data.b_pawn_2 != DEAD){
        score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_2] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_2] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        int r = getx(b1.data.b_pawn_2);
        int c = gety(b1.data.b_pawn_2);
        int dist = min((abs(r-2)+abs(c-0)),abs(r-2)+abs(c-1));
        score += 5*dist;
    }
    // add weights for stalemate, checkmate, moveset size, tie breakers

    PlayerColor pc = b1.data.player_to_play;
    if(depth > 0){
    auto moveset = b1.get_legal_moves();
    if(moveset.size()==0){
        if(b1.in_check()){
            if(pc == WHITE){
                score = -1000000 + 1000*(depth);
            }
            else{
                score = 1000000 - 1000*(depth);
            }
        }
        else{
            score = 0;
        }
    }
    }
    // if(pc == WHITE){
    //     score+=0.1*moveset.size();
    // }
    // else{
    //     score-=0.1*moveset.size();
    // }
    
    return score;
}

double eval_board_82(const Board& b1,int depth){
    double score = 0.0;
    double PawnValue = 250.0;
    double RookValue = 610.0;
    double BishopValue = 450.0;
    double KnightValue = 450.0;
    double KingValue = 10000.0;
    // add piece weights for white pieces
    if( b1.data.w_rook_1 != DEAD){
        score += RookValue;
    }
    if( b1.data.w_rook_2 != DEAD){
        score += RookValue;
    }
    if( b1.data.w_king != DEAD){
        score += KingValue;
    }
    if( b1.data.w_bishop != DEAD){
        score += BishopValue;
    }
    if( b1.data.w_knight_1 != DEAD){
        score += KnightValue;
    }
    if( b1.data.w_knight_2 != DEAD){
        score += KnightValue;
    }
    if( b1.data.w_pawn_1 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_1] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_1] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        int r = getx(b1.data.w_pawn_1);
        int c = gety(b1.data.w_pawn_1);
        int dist = min((abs(r-4)+abs(c-5)),min(abs(r-4)+abs(c-6), abs(r-4)+abs(c-7)));
        score -= 8*dist;
    }
    if( b1.data.w_pawn_2 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_2] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_2] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        int r = getx(b1.data.w_pawn_2);
        int c = gety(b1.data.w_pawn_2);
        int dist = min((abs(r-4)+abs(c-5)),min(abs(r-4)+abs(c-6), abs(r-4)+abs(c-7)));
        score -= 8*dist;
    }
    if( b1.data.w_pawn_3 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_3] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_3] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        int r = getx(b1.data.w_pawn_3);
        int c = gety(b1.data.w_pawn_3);
        int dist = min((abs(r-4)+abs(c-5)), min(abs(r-4)+abs(c-6), abs(r-4)+abs(c-7)));
        score -= 8*dist;
    
    }
    if( b1.data.w_pawn_4 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_3] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_3] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        // int r = getx(b1.data.w_pawn_3);
        // int c = gety(b1.data.w_pawn_3);
        // int dist = min((abs(r-4)+abs(c-5)), min(abs(r-4)+abs(c-6), abs(r-4)+abs(c-7)));
        // score -= 8*dist;
    }
    // add piece weights for black pieces
    if( b1.data.b_rook_1 != DEAD){
        score -= RookValue;
    }
    if( b1.data.b_rook_2 != DEAD){
        score -= RookValue;
    }
    if( b1.data.b_king != DEAD){
        score -= KingValue;
    }
    if( b1.data.b_bishop != DEAD){
        score -= BishopValue;
    }
    if( b1.data.b_knight_1 != DEAD){
        score -= KnightValue;
    }
    if( b1.data.b_knight_2 != DEAD){
        score -= KnightValue;
    }
    if( b1.data.b_pawn_1 != DEAD){
        score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_1] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_1] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        int r = getx(b1.data.b_pawn_1);
        int c = gety(b1.data.b_pawn_1);
        int dist = min((abs(r-3)+abs(c-0)),min(abs(r-3)+abs(c-1), abs(r-3)+abs(c-2)));
        score += 8*dist;
    }
    if( b1.data.b_pawn_2 != DEAD){
        score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_2] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_2] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        int r = getx(b1.data.b_pawn_2);
        int c = gety(b1.data.b_pawn_2);
        int dist = min((abs(r-3)+abs(c-0)),min(abs(r-3)+abs(c-1), abs(r-3)+abs(c-2)));
        score += 8*dist;
    }
    if( b1.data.b_pawn_3 != DEAD){
        score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_3] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_3] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        // int r = getx(b1.data.b_pawn_3);
        // int c = gety(b1.data.b_pawn_3);
        // int dist = min((abs(r-3)+abs(c-0)),min(abs(r-3)+abs(c-1), abs(r-3)+abs(c-2)));
        // score += 8*dist;
    }
    if( b1.data.b_pawn_4 != DEAD){
       score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_4] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_4] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        int r = getx(b1.data.b_pawn_4);
        int c = gety(b1.data.b_pawn_4);
        int dist = min((abs(r-3)+abs(c-0)),min(abs(r-3)+abs(c-1), abs(r-3)+abs(c-2)));
        score += 8*dist;

    }
    // add weights for stalemate, checkmate, moveset size, tie breakers
    PlayerColor pc = b1.data.player_to_play;
    if(depth > 0){
    auto moveset = b1.get_legal_moves();
    if(moveset.size()==0){
        if(b1.in_check()){
            if(pc == WHITE){
                score = -1000000 + 1000*(depth);
            }
            else{
                score = 1000000 - 1000*(depth);
            }
        }
        else{
            score = 0;
        }
    }
    }
    // if(pc == WHITE){
    //     score+=0.1*moveset.size();
    // }
    // else{
    //     score-=0.1*moveset.size();
    // }
    
    return score;
}

double eval_board_84(const Board& b1,int depth){
    double score = 0.0;
    // add piece weights for white pieces
    double PawnValue = 250.0;
    double RookValue = 600.0;
    double BishopValue = 450.0;
    double KingValue = 10000.0;
    if( b1.data.w_rook_1 != DEAD){
        score += RookValue;
    }
    if( b1.data.w_rook_2 != DEAD){
        score += RookValue;
    }
    if( b1.data.w_king != DEAD){
        score += KingValue;

    }
    if( b1.data.w_bishop != DEAD){
        score += BishopValue;
        // int r = getx(b1.data.w_bishop);
        // int c = gety(b1.data.w_bishop);
        // if((r==1 && c==6))
        // {
        //     score-=150;
        // }
    }
    if( b1.data.w_pawn_1 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_1] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_1] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        int r = getx(b1.data.w_pawn_1);
        int c = gety(b1.data.w_pawn_1);
        int dist = min((abs(r-5)+abs(c-7)),abs(r-5)+abs(c-6));
        score -= 8*dist;
    }
    if( b1.data.w_pawn_2 != DEAD){
        score += PawnValue;
        if( b1.data.board_0[b1.data.w_pawn_2] == PAWN_ROOK){
            score += RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.w_pawn_2] == PAWN_BISHOP){
            score += BishopValue - PawnValue;
        }
        int r = getx(b1.data.w_pawn_2);
        int c = gety(b1.data.w_pawn_2);
        int dist = min((abs(r-5)+abs(c-7)),abs(r-5)+abs(c-6));
        score -= 8*dist;
        
    }
    if( b1.data.w_pawn_3 != DEAD){
        score += PawnValue;
    }
    if( b1.data.w_pawn_4 != DEAD){
        score += PawnValue;
    }
    // add piece weights for black pieces
    if( b1.data.b_rook_1 != DEAD){
        score -= RookValue;
    }
    if( b1.data.b_rook_2 != DEAD){
        score -= RookValue;
    }
    if( b1.data.b_king != DEAD){
        score -= KingValue;
    }
    if( b1.data.b_bishop != DEAD){
        score -= BishopValue;
        // int r = getx(b1.data.b_bishop);
        // int c = gety(b1.data.b_bishop);
        // if((r==6 && c==1))
        // {
        //     score+=150;
        // }
    }
    if( b1.data.b_pawn_4 != DEAD){
        score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_4] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_4] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        int r = getx(b1.data.b_pawn_4);
        int c = gety(b1.data.b_pawn_4);
        int dist = min((abs(r-2)+abs(c-0)),abs(r-2)+abs(c-1));
        score += 8*dist;
    }
    if( b1.data.b_pawn_3 != DEAD){
        score -= PawnValue;
        if( b1.data.board_0[b1.data.b_pawn_3] == PAWN_ROOK){
            score -= RookValue - PawnValue;
        }
        if (b1.data.board_0[b1.data.b_pawn_3] == PAWN_BISHOP){
            score -= BishopValue - PawnValue;
        }
        int r = getx(b1.data.b_pawn_3);
        int c = gety(b1.data.b_pawn_3);
        // std::cout<<r<<' '<<c<<endl; 
        int dist = min((abs(r-2)+abs(c-0)),abs(r-2)+abs(c-1));
        // std::cout<<dist<<endl;
        score += 8*dist;
    }
    if( b1.data.b_pawn_2 != DEAD){
        score -= PawnValue;

    }
    if( b1.data.b_pawn_1 != DEAD){
        score -= PawnValue;
    }
    // add weights for stalemate, checkmate, moveset size, tie breakers
    PlayerColor pc = b1.data.player_to_play;
    auto moveset = b1.get_legal_moves();
    if(depth > 0){
    if(moveset.size()==0){
        if(b1.in_check()){
            if(pc == WHITE){
                score = -1000000 + 1000*(depth);
            }
            else{
                score = 1000000 - 1000*(depth);
            }
        }
        else{
            score = 0;
        }
    }
    }
    // if(pc == WHITE){
    //     score+=0.1*moveset.size();
    // }
    // else{
    //     score-=0.1*moveset.size();
    // }
    
    return score;
}

double eval(const Board& b1,int depth, int btype){
    if(btype == 1)
    {
        return eval_board_73(b1, depth);
    }
    else if(btype == 2)
    {
        return eval_board_84(b1, depth);
    }
    else 
    {
        return eval_board_82(b1, depth);
    }
}

double minimax(const Board&b,int depth, bool isMax, double alpha, double beta, float time_lt, std::chrono::time_point<std::chrono::system_clock>& now){
    std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(curr - now);
    // curr = std::time(&curr);
    if(milliseconds.count() > time_lt){
        std::cout<<"Time Used in milliseconds = "<<milliseconds.count()<<std::endl;
        return 0;
    }
    if(depth==0){
        return eval(b, depth, b.data.board_type);
    }
    auto moveset = b.get_legal_moves();
    if(moveset.size()==0){
        return eval(b, depth, b.data.board_type);
    }
    vector<U16> moves(moveset.begin(), moveset.end());
    std::shuffle(moves.begin(), moves.end(),rng);

    if(isMax){
        double bestVal = INT_MIN;
        for(auto m : moves){
            Board* c = new Board(b);
            c->do_move_(m);
            double val = minimax(*c, depth-1, false, alpha, beta,time_lt,now);
            // cout<<"DEPTH: "<<depth<<' '<<move_to_str(m)<<" Val: "<<val<<endl;
            if(val > bestVal){
                bestVal = val;
            }
            alpha = std::max(alpha, bestVal);
            delete c;
            if(beta <= alpha){
                break;
            }
        }
        return bestVal;
    }
    else{
        double bestVal = INT_MAX;
        for(auto m : moves){
            Board* c = new Board(b);
            c->do_move_(m);
            double val = minimax(*c, depth-1, true, alpha, beta,time_lt,now);
            // cout<<"DEPTH: "<<depth<<' '<<move_to_str(m)<<" Val: "<<val<<endl;
            if(val < bestVal){
                bestVal = val;
            }
            beta = std::min(beta, bestVal);
            delete c;
            if(beta <= alpha){
                break;
            }
        }
        return bestVal;
    }
}

string lastmove = "";

string CRHF(Board& b){
    string res = "";
    if(b.data.board_type == EIGHT_TWO)
    {
        res = "82";
        if(b.data.w_pawn_1 != DEAD){
            res += to_string(getx(b.data.w_pawn_1));
            res += to_string(gety(b.data.w_pawn_1));
        }
        else{
            res += "xx";
        }
        if(b.data.w_pawn_2 != DEAD){
            res += to_string(getx(b.data.w_pawn_2));
            res += to_string(gety(b.data.w_pawn_2));
        }
        else{
            res += "xx";
        }
        if(b.data.w_pawn_3 != DEAD){
            res += to_string(getx(b.data.w_pawn_3));
            res += to_string(gety(b.data.w_pawn_3));
        }
        else{
            res += "xx";
        }
        if(b.data.w_pawn_4 != DEAD){
            res += to_string(getx(b.data.w_pawn_4));
            res += to_string(gety(b.data.w_pawn_4));
        }
        else{
            res += "xx";
        }
        if(b.data.w_rook_1 != DEAD){
            res += to_string(getx(b.data.w_rook_1));
            res += to_string(gety(b.data.w_rook_1));
        }
        else{
            res += "xx";
        }
        if(b.data.w_rook_2 != DEAD){
            res += to_string(getx(b.data.w_rook_2));
            res += to_string(gety(b.data.w_rook_2));
        }
        else{
            res += "xx";
        }
        if(b.data.w_bishop != DEAD){
            res += to_string(getx(b.data.w_bishop));
            res += to_string(gety(b.data.w_bishop));
        }
        else{
            res += "xx";
        }
        if(b.data.w_king != DEAD){
            res += to_string(getx(b.data.w_king));
            res += to_string(gety(b.data.w_king));
        }
        else{
            res += "xx";
        }
        if(b.data.w_knight_1 != DEAD){
            res += to_string(getx(b.data.w_knight_1));
            res += to_string(gety(b.data.w_knight_1));
        }
        else{
            res += "xx";
        }
        if(b.data.w_knight_2 != DEAD){
            res += to_string(getx(b.data.w_knight_2));
            res += to_string(gety(b.data.w_knight_2));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_1 != DEAD){
            res += to_string(getx(b.data.b_pawn_1));
            res += to_string(gety(b.data.b_pawn_1));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_2 != DEAD){
            res += to_string(getx(b.data.b_pawn_2));
            res += to_string(gety(b.data.b_pawn_2));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_3 != DEAD){
            res += to_string(getx(b.data.b_pawn_3));
            res += to_string(gety(b.data.b_pawn_3));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_4 != DEAD){
            res += to_string(getx(b.data.b_pawn_4));
            res += to_string(gety(b.data.b_pawn_4));
        }
        else{
            res += "xx";
        }
        if(b.data.b_rook_1 != DEAD){
            res += to_string(getx(b.data.b_rook_1));
            res += to_string(gety(b.data.b_rook_1));
        }
        else{
            res += "xx";
        }
        if(b.data.b_rook_2 != DEAD){
            res += to_string(getx(b.data.b_rook_2));
            res += to_string(gety(b.data.b_rook_2));
        }
        else{
            res += "xx";
        }
        if(b.data.b_bishop != DEAD){
            res += to_string(getx(b.data.b_bishop));
            res += to_string(gety(b.data.b_bishop));
        }
        else{
            res += "xx";
        }
        if(b.data.b_king != DEAD){
            res += to_string(getx(b.data.b_king));
            res += to_string(gety(b.data.b_king));
        }
        else{
            res += "xx";
        }
        if(b.data.b_knight_1 != DEAD){
            res += to_string(getx(b.data.b_knight_1));
            res += to_string(gety(b.data.b_knight_1));
        }
        else{
            res += "xx";
        }
        if(b.data.b_knight_2 != DEAD){
            res += to_string(getx(b.data.b_knight_2));
            res += to_string(gety(b.data.b_knight_2));
        }
        else{
            res += "xx";
        }
    }
    else if(b.data.board_type == EIGHT_FOUR)
    {
        res = "84";
        if(b.data.w_pawn_1 != DEAD){
            res += to_string(getx(b.data.w_pawn_1));
            res += to_string(gety(b.data.w_pawn_1));
        }
        else{
            res += "xx";
        }
        if(b.data.w_pawn_2 != DEAD){
            res += to_string(getx(b.data.w_pawn_2));
            res += to_string(gety(b.data.w_pawn_2));
        }
        else{
            res += "xx";
        }
        if(b.data.w_pawn_3 != DEAD){
            res += to_string(getx(b.data.w_pawn_3));
            res += to_string(gety(b.data.w_pawn_3));
        }
        else{
            res += "xx";
        }
        if(b.data.w_pawn_4 != DEAD){
            res += to_string(getx(b.data.w_pawn_4));
            res += to_string(gety(b.data.w_pawn_4));
        }
        else{
            res += "xx";
        }
        if(b.data.w_rook_1 != DEAD){
            res += to_string(getx(b.data.w_rook_1));
            res += to_string(gety(b.data.w_rook_1));
        }
        else{
            res += "xx";
        }
        if(b.data.w_rook_2 != DEAD){
            res += to_string(getx(b.data.w_rook_2));
            res += to_string(gety(b.data.w_rook_2));
        }
        else{
            res += "xx";
        }
        if(b.data.w_bishop != DEAD){
            res += to_string(getx(b.data.w_bishop));
            res += to_string(gety(b.data.w_bishop));
        }
        else{
            res += "xx";
        }
        if(b.data.w_king != DEAD){
            res += to_string(getx(b.data.w_king));
            res += to_string(gety(b.data.w_king));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_1 != DEAD){
            res += to_string(getx(b.data.b_pawn_1));
            res += to_string(gety(b.data.b_pawn_1));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_2 != DEAD){
            res += to_string(getx(b.data.b_pawn_2));
            res += to_string(gety(b.data.b_pawn_2));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_3 != DEAD){
            res += to_string(getx(b.data.b_pawn_3));
            res += to_string(gety(b.data.b_pawn_3));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_4 != DEAD){
            res += to_string(getx(b.data.b_pawn_4));
            res += to_string(gety(b.data.b_pawn_4));
        }
        else{
            res += "xx";
        }
        if(b.data.b_rook_1 != DEAD){
            res += to_string(getx(b.data.b_rook_1));
            res += to_string(gety(b.data.b_rook_1));
        }
        else{
            res += "xx";
        }
        if(b.data.b_rook_2 != DEAD){
            res += to_string(getx(b.data.b_rook_2));
            res += to_string(gety(b.data.b_rook_2));
        }
        else{
            res += "xx";
        }
        if(b.data.b_bishop != DEAD){
            res += to_string(getx(b.data.b_bishop));
            res += to_string(gety(b.data.b_bishop));
        }
        else{
            res += "xx";
        }
        if(b.data.b_king != DEAD){
            res += to_string(getx(b.data.b_king));
            res += to_string(gety(b.data.b_king));
        }
        else{
            res += "xx";
        }
    }
    else
    {
        res = "73";
        if(b.data.w_pawn_1 != DEAD){
            res += to_string(getx(b.data.w_pawn_1));
            res += to_string(gety(b.data.w_pawn_1));
        }
        else{
            res += "xx";
        }
        if(b.data.w_pawn_2 != DEAD){
            res += to_string(getx(b.data.w_pawn_2));
            res += to_string(gety(b.data.w_pawn_2));
        }
        else{
            res += "xx";
        }
        if(b.data.w_rook_1 != DEAD){
            res += to_string(getx(b.data.w_rook_1));
            res += to_string(gety(b.data.w_rook_1));
        }
        else{
            res += "xx";
        }
        if(b.data.w_rook_2 != DEAD){
            res += to_string(getx(b.data.w_rook_2));
            res += to_string(gety(b.data.w_rook_2));
        }
        else{
            res += "xx";
        }
        if(b.data.w_bishop != DEAD){
            res += to_string(getx(b.data.w_bishop));
            res += to_string(gety(b.data.w_bishop));
        }
        else{
            res += "xx";
        }
        if(b.data.w_king != DEAD){
            res += to_string(getx(b.data.w_king));
            res += to_string(gety(b.data.w_king));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_1 != DEAD){
            res += to_string(getx(b.data.b_pawn_1));
            res += to_string(gety(b.data.b_pawn_1));
        }
        else{
            res += "xx";
        }
        if(b.data.b_pawn_2 != DEAD){
            res += to_string(getx(b.data.b_pawn_2));
            res += to_string(gety(b.data.b_pawn_2));
        }
        else{
            res += "xx";
        }
        if(b.data.b_rook_1 != DEAD){
            res += to_string(getx(b.data.b_rook_1));
            res += to_string(gety(b.data.b_rook_1));
        }
        else{
            res += "xx";
        }
        if(b.data.b_rook_2 != DEAD){
            res += to_string(getx(b.data.b_rook_2));
            res += to_string(gety(b.data.b_rook_2));
        }
        else{
            res += "xx";
        }
        if(b.data.b_bishop != DEAD){
            res += to_string(getx(b.data.b_bishop));
            res += to_string(gety(b.data.b_bishop));
        }
        else{
            res += "xx";
        }
        if(b.data.b_king != DEAD){
            res += to_string(getx(b.data.b_king));
            res += to_string(gety(b.data.b_king));
        }
        else{
            res += "xx";
        }

    }
    if(b.data.player_to_play == BLACK){
        res += 'b';
    }
    else{
        res += 'w';
    }
    return res;
}

void Engine::find_best_move(const Board& b) {

    // time_t start = std::time(&start);
    bool random_flag = false;
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    // std::chrono::time_point<std::chrono::system_clock> foo = now + std::chrono::milliseconds(3500);
    if(moves_done == 0){
        fill();
    }

    float t_left = time_left.count();
    float time_lt = 4*t_left/100; // used for less than 30s
    cout<<"t_left = "<<t_left<<endl;
    if(b.data.board_type == EIGHT_TWO)
    {
        if(t_left/1000 > 100)
        {
            time_lt = 10000;
        }
        else if(t_left/1000 > 33){
            time_lt = 5000;
        }
        else if(t_left/1000 > 5){
            time_lt = 2000;
        }
        else{
            random_flag = true;
        }
    }
    else if(b.data.board_type == EIGHT_FOUR)
    {   
        if(t_left/1000 > 120)
        {
            time_lt = 6000;
        }
        else if(t_left/1000 > 60)
        {
            time_lt = 7000;
        }
        else if(t_left/1000 > 20){
            time_lt = 1600;
        }
        else if(t_left/1000 > 5)
        {
            time_lt = 800;
        }
        else{
            random_flag = true;
        }
    }
    else{
        if(t_left/1000 > 5){
            time_lt = 2000;
        }
        else{
            random_flag = true;
        }
    }

    
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        this->best_move = 0;
    }
    else {
        std::vector<U16> moves(moveset.begin(), moveset.end());
        std::shuffle(moves.begin(), moves.end(),rng);
        std::cout << std::endl;
        U16 best_move = *moveset.begin();
        PlayerColor pc = b.data.player_to_play;
        double alpha = INT_MIN;
        double beta = INT_MAX;
        Board* c1 = new Board(b);
        string mad_angle = CRHF(*c1);
        delete c1;
        if(exist_move[mad_angle]==100){
            std::cout<<"CRHF = "<<mad_angle<<endl;
            best_move = str_to_move(move_to_play[mad_angle]);
            this->best_move = best_move;
            lastmove = move_to_str(best_move);
            return;
        }
        if(random_flag){
            best_move = moves[rand()%moves.size()];
            this->best_move = best_move;
            lastmove = move_to_str(best_move);
            return;
        }
        if(pc == BLACK){
            pcGlobal = BLACK;
        }
        else
        {
            pcGlobal = WHITE;
        }

        if(pc==WHITE){
            moves_done++;
            for(int iter_depth=2; iter_depth<=10; iter_depth+=1){
                double bestVal = INT_MIN;
                U16 iter_best_move = *moveset.begin();
                for(auto m : moves){

                    std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
                    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(curr - now);
                    // curr = std::time(&curr);
                    if(milliseconds.count() > time_lt){
                        // std::cout<<"Time Used in milliseconds = "<<milliseconds.count()<<std::endl;
                        break;
                    }


                    // if(search == false) break;
                    // add if time brea
                    Board* c = new Board(b);
                    c->do_move_(m);
                    double val = minimax(*c, iter_depth, false, alpha, beta,time_lt,now);
                    string ms = move_to_str(m);
                    cout<<"MOVE: "<<ms<<" xx val="<<100*val<<endl;
                    string ms_rev = ms.substr(2);
                    ms_rev+=ms.substr(0,2);
                    if(((ms_rev == lastmove) && val > 0) && moves.size()>1) continue;
                    if(val > bestVal){
                        bestVal = val;
                        iter_best_move = m;
                    }
                    delete c;
                }
                std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(curr - now);
                // curr = std::time(&curr);
                if(milliseconds.count() > time_lt){
                    // std::cout<<"Time Used in milliseconds = "<<milliseconds.count()<<std::endl;
                    break;
                }
                best_move = iter_best_move;
                
                this->best_move = best_move;
                
                cout<<" iterdepth: "<<iter_depth<<" curbest_val: "<<bestVal<<" move: "<<move_to_str(best_move)<<'\n';
                if(b.data.board_type == EIGHT_TWO && iter_depth == 4)
                {
                    if(milliseconds.count() < time_lt * 0.2 && (t_left > 33000)){
                        int rrr = 123;
                    }
                    else{
                    break;
                    }
                }
                // else if(b.data.board_type == SEVEN_THREE && t_left/1000 < 60 && t_left/1000 > 30 && iter_depth == 4)
                // {
                //     break;
                // }
                else if(b.data.board_type == EIGHT_FOUR)
                {
                    if(iter_depth == 6) break;
                    // else if(t_left/1000 < 60 && iter_depth == 4) break;
                }
                if(bestVal > 50000){
                    break;
                }
            }
        }
        else{
            moves_done++;
            for(int iter_depth = 2; iter_depth <=10; iter_depth+=1)
            {
                if(b.data.board_type != SEVEN_THREE && iter_depth%2) continue;
                double bestVal = INT_MAX;
                U16 iter_best_move = *moveset.begin();
                for(auto m : moves){

                    std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
                    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(curr - now);
                    // curr = std::time(&curr);
                    if(milliseconds.count() > time_lt){
                        // std::cout<<"Time Used in milliseconds = "<<milliseconds.count()<<std::endl;
                        break;
                    }

                    // if(search == false) break;
                    Board* c = new Board(b);
                    c->do_move_(m);
                    double val = minimax(*c, iter_depth, true, alpha, beta,time_lt,now);
                    string ms = move_to_str(m);
                    cout<<"MOVE: "<<ms<<" xx val="<<100*val<<endl;
                    string ms_rev = ms.substr(2);
                    ms_rev+=ms.substr(0,2);
                    if((ms_rev == lastmove) && moves.size()>1 && val<0) continue;
                    if(val < bestVal){
                        bestVal = val;
                        iter_best_move = m;
                    }
                    delete c;
                }
                std::chrono::time_point<std::chrono::system_clock> curr = std::chrono::system_clock::now();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(curr - now);
                // curr = std::time(&curr);
                if(milliseconds.count() > time_lt)
                {
                    break;
                }
                best_move = iter_best_move;
                cout<<" iterdepth: "<<iter_depth<<" curbest_val: "<<bestVal<< " move: "<<move_to_str(best_move)<<endl;
                this->best_move = best_move;
                if(b.data.board_type == EIGHT_TWO && iter_depth == 4)
                {
                    if(milliseconds.count() < time_lt * 0.2 && (t_left > 33000)){
                        int rrr = 123;
                    }
                    else{
                    break;
                    }
                }
                // else if(b.data.board_type == SEVEN_THREE && t_left/1000 < 60 && t_left/1000 > 30 && iter_depth == 4)
                // {
                //     break;
                // }
                else if(b.data.board_type == EIGHT_FOUR)
                {
                    if(iter_depth == 6) break;
                    // else if(t_left/1000 < 60 && iter_depth == 4) break;
                }
                // if(search == false) break;
                
                if(bestVal < -50000){
                    break;
                }
            }
            
        }
    }
    // std::cout<<board_to_str(&b.data);
    Board* x = new Board(b);
    string bingo = CRHF(*x);
    cout<<"CRHF = "<<bingo<<endl;
    x->do_move_(best_move);
    cout<<lastmove<<endl;
    
    std::cout<<board_to_str(&x->data);
    lastmove = move_to_str(best_move);
    cout<<"cur move: "<<lastmove<<endl;
    delete x;
    // just for debugging, to slow down the moves
    // std::this_thread::sleep_for(std::chrono::milliseconds(250));
}