/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>
//tự thêm vào
#include "../game/setup.h"
#include "../utils/config.h"
static const int FONT_SIZE = 32;
static const int FONT_SIZE_SMALL = 20;

static void drawTxt(SDL_Renderer* r, TTF_Font* font, const std::string& text, int cx, int y, SDL_Color c) {
    if (!font || !r) return;
    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), c);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    if (tex) {
        SDL_Rect d = {cx - surf->w/2, y, surf->w, surf->h};
        SDL_SetTextureColorMod(tex, 0, 0, 0);
        SDL_SetTextureAlphaMod(tex, 200);
        SDL_Rect outline = d;
        outline.x -= 1;
        SDL_RenderCopy(r, tex, nullptr, &outline);
        outline = d;
        outline.x += 1;
        SDL_RenderCopy(r, tex, nullptr, &outline);
        outline = d;
        outline.y -= 1;
        SDL_RenderCopy(r, tex, nullptr, &outline);
        outline = d;
        outline.y += 1;
        SDL_RenderCopy(r, tex, nullptr, &outline);
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
        SDL_RenderCopy(r, tex, nullptr, &d);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}



/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer() {
}

/**
 * Mô tả: Destructor của SDLRenderer.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không (việc giải phóng thực hiện trong close()).
 */
SDLRenderer::~SDLRenderer() {
}

/**
 * Mô tả: Khởi tạo SDL, window và renderer.
 * Đầu vào: config - cấu hình màn hình và layout.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khởi tạo SDL subsystem.
 *   - Tạo window và renderer.
 *   - Thiết lập chế độ blend.
 * NOTE: Phải gọi trước khi render.
 */

void SDLRenderer::init(const RunConfig& config) {
    // read config
    int screenWidth = config.screenWidth;
    int screenHeight = config.screenHeight;

    // int boardPadding = config.boardPadding;
    // init(...)

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "TicTacToe SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,  
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    



    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface* backgroundImage = SDL_LoadBMP("assets/background.bmp");
    if (backgroundImage) {
        background = SDL_CreateTextureFromSurface(renderer, backgroundImage);
        SDL_FreeSurface(backgroundImage);
    }

    // load font
    // font = TTF_OpenFont("assets/font.ttf", font_size);
    s_font = TTF_OpenFont("assets/font.ttf", FONT_SIZE);
    s_font_small = TTF_OpenFont("assets/font.ttf", FONT_SIZE_SMALL);   // cỡ chữ dòng dưới
    ;


}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  // dark background
    SDL_RenderClear(renderer);
    drawBackground();
}

void SDLRenderer::drawBackground() {
    if (!renderer || !background) return;

    int w, h;
    int textureWidth, textureHeight;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    SDL_QueryTexture(background, nullptr, nullptr, &textureWidth, &textureHeight);

    SDL_Rect source = {0, 0, textureWidth, textureHeight};
    const float targetAspect = static_cast<float>(w) / h;
    const float imageAspect = static_cast<float>(textureWidth) / textureHeight;
    if (imageAspect > targetAspect) {
        source.w = static_cast<int>(textureHeight * targetAspect);
        source.x = (textureWidth - source.w) / 2;
    } else {
        source.h = static_cast<int>(textureWidth / targetAspect);
        source.y = (textureHeight - source.h) / 2;
    }

    SDL_Rect destination = {0, 0, w, h};
    SDL_RenderCopy(renderer, background, &source, &destination);
}

/**
 * Mô tả: Hiển thị frame đã render lên màn hình.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Swap buffer để hiển thị nội dung.
 */
void SDLRenderer::renderPresent() {
    SDL_RenderPresent(renderer);
}

/**
 * Mô tả: Vẽ hình chữ nhật lên renderer.
 * Đầu vào:
 *   - x, y: tọa độ.
 *   - w, h: kích thước.
 *   - color: màu sắc.
 *   - filled: true nếu fill, false nếu vẽ viền.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ trực tiếp lên renderer.
 */
void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

/**
 * Mô tả: Hiển thị menu lựa chọn.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ UI lên màn hình.
 * TODO:
 *   - Bước 1: Xác định loại menu.
 *   - Bước 2: Render text tương ứng.
 *   - Trường hợp biên: selectType không hợp lệ.
 */
void SDLRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: Render menu UI tương ứng với selectType
    clearScreen();
    SDL_Color white = {255, 255, 255, 255};
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    switch(selectType){
        case SelectType::TITLE_UI: 
        drawTxt(renderer, s_font, "TIC TAC TOE", w/2, 250, white);
        drawTxt(renderer,s_font,"Chon phim bat ki de bat dau",w/2,300,white);
        break;
        case SelectType::SIZE_UI: break;
        case SelectType::GOAL_UI: break;
        case SelectType::BOT_LEVEL_UI: drawTxt(renderer,s_font,"Chon cap do bot",w/2,h*3/10,white);
                                       drawTxt(renderer,s_font,"1:Easy",w/2,h*3/8,white);
                                       drawTxt(renderer,s_font,"2:Medium",w/2,h*9/20,white);
                                       drawTxt(renderer,s_font,"3:Hard",w/2,h*21/40,white);
                                       break;
        case SelectType::GAME_MODE_UI: drawTxt(renderer,s_font,"Chon che do choi",w/2,h*3/10,white);
                                       drawTxt(renderer,s_font,"1:PVP",w/2,h*3/8,white);
                                       drawTxt(renderer,s_font,"2:PVE",w/2,h*9/20,white);
                                       drawTxt(renderer,s_font,"3:EVE",w/2,h*21/40,white);
                                       break;
        case SelectType::MUL_BOT_LEVEL_UI: drawTxt(renderer,s_font,std::format("Chon cap do cua Bot {}", context + 1)
                                             ,w/2,240,white);
                                       drawTxt(renderer,s_font,"1:Easy",w/2,h*3/8,white);
                                       drawTxt(renderer,s_font,"2:Medium",w/2,h*9/20,white);
                                       drawTxt(renderer,s_font,"3:Hard",w/2,h*21/40,white);
                                       break;
        case SelectType::PLAYER_UI: break;
    }
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông báo lỗi.
 * TODO:
 *   - Bước 1: Xác định loại lỗi.
 *   - Bước 2: Render text cảnh báo.
 */
void SDLRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: Render thông báo lỗi
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị xác nhận.
 * TODO:
 *   - Bước 1: Xác định loại selection.
 *   - Bước 2: Render thông báo thành công.
 */
void SDLRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: Render thông báo thành công
    renderPresent();
}

void SDLRenderer::setPlayer(int player,bool is_bot){
    g_player =player;
    g_is_bot =is_bot;
}

void SDLRenderer::redrawBoard(){
    if (!renderer || !window || s_board_size == 0) return;
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    int pxSize = (std::min(w, h) - 400) / s_board_size;
    int X = (w - pxSize * s_board_size) / 2;
    int Y = (h - pxSize * s_board_size) / 2;
    SDL_Rect boardBackground = {X, Y, pxSize * s_board_size, pxSize * s_board_size};
    SDL_SetRenderDrawColor(renderer, 8, 16, 24, 155);
    SDL_RenderFillRect(renderer, &boardBackground);

    SDL_SetRenderDrawColor(renderer, 235, 245, 250, 230);
    for (int i = 1; i < s_board_size; i++) {
        SDL_RenderDrawLine(renderer, X+i*pxSize, Y, X+i*pxSize, Y+s_board_size*pxSize);
        SDL_RenderDrawLine(renderer, X, Y+i*pxSize, X+s_board_size*pxSize, Y+i*pxSize);
    }
    SDL_Rect border = {X, Y, pxSize * s_board_size, pxSize * s_board_size};
    SDL_RenderDrawRect(renderer, &border);

    
    for (int row = 0; row < s_board_size; row++) {
        for (int col = 0; col < s_board_size; col++) {
            int x = X + col*pxSize, y = Y + row*pxSize;
            if (s_board_data[row][col] == 'X') {
                SDL_SetRenderDrawColor(renderer, 255, 105, 80, 255);
                int inset = pxSize / 6;
                for (int offset = -2; offset <= 2; offset++) {
                    SDL_RenderDrawLine(renderer, x+inset+offset, y+inset,
                                       x+pxSize-inset+offset, y+pxSize-inset);
                    SDL_RenderDrawLine(renderer, x+pxSize-inset+offset, y+inset,
                                       x+inset+offset, y+pxSize-inset);
                }
            } else if (s_board_data[row][col] == 'O') {
                SDL_SetRenderDrawColor(renderer, 75, 220, 255, 255);
                int cx = x + pxSize / 2, cy = y + pxSize / 2;
                int radius = pxSize / 2 - pxSize / 8;
                for (int thickness = 0; thickness < 4; thickness++) {
                    int px = 0, py = radius - thickness, d = 1 - (radius - thickness);
                    while (px <= py) {
                        SDL_RenderDrawPoint(renderer, cx+px, cy+py);
                        SDL_RenderDrawPoint(renderer, cx-px, cy+py);
                        SDL_RenderDrawPoint(renderer, cx+px, cy-py);
                        SDL_RenderDrawPoint(renderer, cx-px, cy-py);
                        SDL_RenderDrawPoint(renderer, cx+py, cy+px);
                        SDL_RenderDrawPoint(renderer, cx-py, cy+px);
                        SDL_RenderDrawPoint(renderer, cx+py, cy-px);
                        SDL_RenderDrawPoint(renderer, cx-py, cy-px);
                        if (d < 0) d += 2*px+3;
                        else { d += 2*(px-py)+5; py--; }
                        px++;
                    }
                }
            }
        }
    }
}

/**
 * Mô tả: Vẽ bàn cờ lên màn hình.
 * Đầu vào: board, size.
 * Đầu ra: Không.
 * Tác dụng phụ: Render grid và ký hiệu.
 * TODO:
 *   - Bước 1: Tính toán layout ô.
 *   - Bước 2: Vẽ grid.
 *   - Bước 3: Vẽ X/O.
 */
void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // TODO: Render board
    s_board_size = size;
    memcpy(s_board_data, board, sizeof(s_board_data));

    redrawBoard();
}

/**
 * Mô tả: Hiển thị nước đi vừa thực hiện.
 * Đầu vào: row, col.
 * Đầu ra: Không.
 * Tác dụng phụ: Highlight ô.
 * TODO:
 *   - Bước 1: Xác định vị trí ô.
 *   - Bước 2: Vẽ highlight.
 */
void SDLRenderer::showMove(const int row, const int col) {
    
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị lỗi.
 * TODO:
 *   - Bước 1: Render thông báo lỗi.
 */
void SDLRenderer::showInvalidMove() {
    // TODO: Render invalid move message
    renderPresent();
}
/**
 * Mô tả: Hiển thị người chơi hiện tại.
 * Đầu vào: player, is_bot.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông tin turn.
 * TODO:
 *   - Bước 1: Xác định text.
 *   - Bước 2: Render lên màn hình.
 */
void SDLRenderer::showPlayer(const int player, const bool is_bot) {
    // TODO: Render player info
    setPlayer(player, is_bot);
    SDL_Color white = {255, 255, 255, 255};
    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    drawTxt(renderer,s_font,"TIC TAC TOE",w/2,70,white);
    if(is_bot){ 
        drawTxt(renderer,s_font,std::format("Bot {} is thinking...", player+1),w/2,h-120,white) ;
        renderPresent();
    }
    else drawTxt(renderer,s_font,std::format("Player {}'s turn", player+1),w/2,h-120,white);
    
}

/**
 * Mô tả: Hiển thị kết quả game.
 * Đầu vào: winner, is_bot, winLine.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
 * TODO:
 *   - Bước 1: Kiểm tra draw hoặc win.
 *   - Bước 2: Render text.
 *   - Bước 3: Highlight winLine nếu có.
 */
void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: Render result
    SDL_Color white = {255, 255, 255, 255};
    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    if(winner==-1) drawTxt(renderer,s_font,"It's draw!",w/2,h-100,white);
    else if(is_bot) drawTxt(renderer,s_font,std::format("Bot {} wins!",winner+1),w/2,h-100,white);
    else drawTxt(renderer,s_font,std::format("Player {} wins!",winner+1),w/2,h-100,white);
    renderPresent();
}

/**
 * Mô tả: In kết quả ra stdout (judge mode).
 * Đầu vào: gameResult.
 * Đầu ra: Không.
 * Tác dụng phụ: In console.
 * TODO:
 *   - Bước 1: Format output.
 *   - Bước 2: In ra std::cout.
 */
void SDLRenderer::printResult(const GameResult& gameResult) {
    // TODO: Print result
    std::cout << gameResult.winner<< " " << gameResult.turns << "\n";
}

/**
 * Mô tả: Giải phóng tài nguyên SDL.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Destroy renderer và window.
 *   - Shutdown SDL subsystem.
 */
void SDLRenderer::close() {
    if (background) {
        SDL_DestroyTexture(background);
        background = nullptr;
    }

    // if (font) {
    //     TTF_CloseFont(font);
    //     font = nullptr;
    // }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}