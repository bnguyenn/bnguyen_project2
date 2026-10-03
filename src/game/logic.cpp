/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            board[i][j]='-';
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    if(row < 0||row>=size||col<0||col>=size){
        return false;
    }
    else if(board[row][col]!='-'){
        return false;
    }
    return true;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    // TODO: implement
    board[row][col]=symbol;
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
    if(x<0||x>=size||y<0||y>=size){
        return true;
    }
    else if(board[x][y]=='-'){
        return true;
    }
    else if(board[x][y]==symbol){
        return true;
    }

    return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    int dx[] = {0, 1, 1, 1};
    int dy[] = {1, 0, 1, -1};
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int d = 0; d < 4; d++) {
                int count = 0;
                for (int k = 0; k < goal; k++) {
                    int x = i + dx[d] * k;
                    int y = j + dy[d] * k;
                    if (x < 0 || x >= size || y < 0 || y >= size) break;
                    if (board[x][y] != symbol) break;
                    count++;
                }
                if (count == goal) {
                    bool head1 = isEmptyHead(board, size, i - dx[d], j - dy[d], symbol);
                    bool head2 = isEmptyHead(board, size, i + dx[d] * goal, j + dy[d] * goal, symbol);
                    if (head1 && head2) return true;
                }
            }
        }
    }
    return false;
}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for(int i=0;i<size;i++ ){
        for(int j=0;j<size;j++){
            if(board[i][j]=='-'){
                return false;
            }
        }
    }

    return true;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {
    int dx[] = {0, 1, 1, 1};
    int dy[] = {1, 0, 1, -1};
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int d = 0; d < 4; d++) {
                int count = 0;
                for (int k = 0; k < goal; k++) {
                    int x = i + dx[d] * k;
                    int y = j + dy[d] * k;
                    if (x < 0 || x >= size || y < 0 || y >= size) break;
                    if (board[x][y] != symbol) break;
                    count++;
                }
                if (count == goal) {
                    bool head1 = isEmptyHead(board, size, i - dx[d], j - dy[d], symbol);
                    bool head2 = isEmptyHead(board, size, i + dx[d] * goal, j + dy[d] * goal, symbol);
                    bool valid = (rule == EndRule::NONE) ||
                                 (rule == EndRule::OPEN_ONE && (head1 || head2)) ||
                                 (rule == EndRule::OPEN_TWO && head1 && head2);
                    if (valid) {
                        WinLine wl;
                        for (int l = 0; l < goal; l++)
                            wl.cells.push_back({i + dx[d] * l, j + dy[d] * l});
                        return wl;
                    }
                }
            }
        }
    }
    return std::nullopt;
}