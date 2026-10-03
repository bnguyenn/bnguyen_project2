/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {
    // TODO: init state
}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer() {
    // TODO: cleanup nếu cần
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig& config) {
    // TODO: init renderer theo config
    
}

/**
 * Mô tả:
 *   Xóa màn hình terminal.
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen() {
    // TODO: clear screen
    std::cout << "\033[2J\033[H";
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;

        case SelectType::SIZE_UI:
            // TODO: display board size selection
            // Example: "Size Input (NxN, 3 <= N <= BOARD_N_MAX)"
            std::cout<< "Size Input(NxN,3 <= N <= 12): ";
            break;

        case SelectType::GOAL_UI:
            // TODO: display goal selection
            // Example: "Goal Input (3 - 5, goal <= size)"
            std::cout<<"Goal Input(3 - 5, goal <= size): ";
            break;

        case SelectType::GAME_MODE_UI:
            // TODO: display game mode selection
            // Example: "(1) PvP | (2) PvE | (3) EvE"
            std::cout<<"(1) PvP | (2) PvE | (3) EvE: ";
            break;

        case SelectType::BOT_LEVEL_UI:
            // TODO: display bot level selection
            // Example: "(1) EASY | (2) MEDIUM | (3) HARD"
            std::cout<<"(1) EASY | (2) MEDIUM | (3) HARD: ";
            break;

        case SelectType::PLAYER_UI:
            // TODO: display player move prompt
            // Example input format: "(row, col)"
            std::cout<<"Move (row col): ";
            break;

        case SelectType::MUL_BOT_LEVEL_UI:
            // TODO: display multiple bot level selection
            // Example input format: "(bot1_level, bot2_level)"
            std::cout<<"Bot "<< context+1 <<" level (1) EASY | (2) MEDIUM | (3) HARD: ";
            break;

        default:
            break;
    }
    return;
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: render invalid message
    std::cout << "Invalid input! Try again.\n";
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: render success message
    
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // TODO: render board
    std::cout<<"  ";
    for (int col=0; col <size;col++){
        std::cout<<" "<<col;
    }
    std::cout<<"\n";
    
    std::cout<<"  ";
    for (int col=0;col< size*2+1;col++){
        std::cout<<"-";
    }
    std::cout<<"\n";

    for(int row=0; row< size;row++){
        std::cout<< row <<" |";
        for (int col=0;col<size;col++){
            if(board[row][col] == 'X')
                std::cout << "\033[31m" << board[row][col] << "\033[0m" << " ";
            else
                std::cout << board[row][col] << " ";
        }
        std::cout<<"\n";
    }
}
/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot) {
    // TODO: render player info
    if(is_bot){
        std::cout<<"Bot(Player "<< player+1 <<") is thinking...\n";
    }
    else{
        std::cout<<"Player "<< player+1 <<"'s turn\n";
    }
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col) {
    // TODO: render move
    std::cout<<"Move placed at ("<<row<<", "<<col<<")"<<"\n";
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove() {
    // TODO: render invalid move message
    std::cout<<"Invalid Move\n";
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: render result
    if(winner==-1){
        std::cout<<"It's a draw!\n";
    }
    else if(is_bot){
        std::cout<<"Bot "<<winner+1<<" wins!\n";
    }
    else{
        std::cout<<"Player "<<winner+1<<" wins!\n";
    }
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult& gameResult) {
    // TODO: print result đơn giản
    std::cout<<gameResult.winner<<" "<<gameResult.turns<< "\n";
}

/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close() {
    // TODO: cleanup renderer
}