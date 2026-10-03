/**
 * SDL Interaction cpp implementation
 *
 */

#include "interaction.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>
#include <SDL2/SDL_ttf.h>

/* ---------- Definitions ---------- */
static void itxt(SDL_Renderer* rend, TTF_Font* font, const std::string& s, int cx, int y, SDL_Color c) {
    if (!font || !rend) return;
    SDL_Surface* surf = TTF_RenderText_Blended(font, s.c_str(), c);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
    if (tex) {
        SDL_Rect d = {cx - surf->w/2, y, surf->w, surf->h};
        SDL_SetTextureColorMod(tex, 0, 0, 0);
        SDL_SetTextureAlphaMod(tex, 200);
        SDL_Rect outline = d;
        outline.x -= 1;
        SDL_RenderCopy(rend, tex, nullptr, &outline);
        outline = d;
        outline.x += 1;
        SDL_RenderCopy(rend, tex, nullptr, &outline);
        outline = d;
        outline.y -= 1;
        SDL_RenderCopy(rend, tex, nullptr, &outline);
        outline = d;
        outline.y += 1;
        SDL_RenderCopy(rend, tex, nullptr, &outline);
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
        SDL_RenderCopy(rend, tex, nullptr, &d);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}
static void itxt_small(SDL_Renderer* rend, TTF_Font* font_small, const std::string& s, int cx, int y, SDL_Color c) {    //hàm này là dòng chữ nhỏ ở bên dưới chỗ gọi chọn size board,goal
    if (!font_small || !rend) return;
    SDL_Surface* surf = TTF_RenderText_Blended(font_small, s.c_str(), c);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
    if (tex) {
        SDL_Rect d = {cx - surf->w/2, y, surf->w, surf->h};    // xác định vị trí và kích thước của text
        SDL_RenderCopy(rend, tex, nullptr, &d);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}

static void renderInput(SDLRenderer* r, const std::string& title, const std::string& sub, const std::string& buf) {
    SDL_Renderer* rend = r->getSDLRenderer();
    TTF_Font* font = r->getFont();
    TTF_Font* font_small = r->getFontSmall();
    if (!rend) return;
    SDL_SetRenderDrawColor(rend, 20, 20, 20, 255);
    SDL_RenderClear(rend);
    r->drawBackground();
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray  = {160, 160, 160, 255};
    itxt(rend, font, title, 400, 140, white);
    itxt(rend, font, sub,   400, 185, gray);
    std::string box = "[" + buf + std::string(std::max(0, 8-(int)buf.size()), '_') + "]";
    itxt(rend, font, box,400, 295, white);
    itxt(rend, font_small, "INPUT USING KEYBOARD",  400, 390, white);
    itxt(rend, font_small, "<ENTER> DE DONG Y",    400, 430, white);
    itxt(rend, font_small, "<BACKSPACE> DE XOA", 400, 470, white);
    SDL_RenderPresent(rend);
}

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 *   - e: SDL_Event hiện tại.
 * Đầu ra:
 *   - false nếu không phải quit event.
 * Tác dụng phụ:
 *   - Ném QuitException nếu nhận được sự kiện thoát.
 */

bool SDLInteraction::waitForQuit(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        throw QuitException();
    }
    return false;
}

/**
 * Mô tả: Khởi tạo đối tượng SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::SDLInteraction(SDLRenderer* r) : renderer(r) {
}
/**
 * Mô tả: Destructor của SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::~SDLInteraction() {
}

/**
 * Mô tả: Khởi tạo hệ thống interaction cho SDL.
 * Đầu vào:
 *   - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Thiết lập trạng thái ban đầu cho input SDL.
 */
void SDLInteraction::init(const RunConfig& config) {
    // TODO:
    // - Khởi tạo các thành phần cần thiết cho input SDL
    // - Có thể reset event queue hoặc trạng thái input
}

/**
 * Mô tả: Tạm dừng chương trình trong SDL.
 *        - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 *        - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 *   - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Có thể block thread hiện tại.
 *   - Có thể ném QuitException nếu người dùng đóng cửa sổ.
 */
void SDLInteraction::pause(int timeout) {
    // Nếu có timeout cụ thể -> delay trực tiếp bằng SDL
    if (timeout > 0) {
        SDL_Delay(timeout);
        return;
    }

    SDL_FlushEvents(SDL_KEYDOWN, SDL_KEYDOWN);
    SDL_FlushEvents(SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONDOWN);

    bool waiting = true;
    SDL_Event event;

    // Vòng lặp chờ sự kiện từ người dùng
    while (waiting) {
        // SDL_WaitEvent sẽ block cho tới khi có event
        if (SDL_WaitEvent(&event)) {
            // Nếu người dùng đóng cửa sổ -> thoát game
            if (waitForQuit(event)) {
            }

            // tương tự
            // if (event.type == SDL_QUIT) {
            //     throw QuitException();
            // }

            // Nếu có tương tác (nhấn phím hoặc click chuột) -> kết thúc pause
            if ((event.type == SDL_KEYDOWN && event.key.repeat == 0) ||
                event.type == SDL_MOUSEBUTTONDOWN) {
                waiting = false;
            }
        }
    }
}

/**
 * Mô tả: Lấy input kích thước bàn cờ từ người dùng qua SDL.
 * Đầu vào:
 *   - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 *   - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật giá trị tại size nếu thành công.
 */
bool SDLInteraction::selectSize(int* size) {
    // TODO:
    // - Lắng nghe event từ SDL (keyboard/mouse)
    // - Parse input thành số nguyên
    // - Kiểm tra điều kiện hợp lệ (BOARD_N_MIN <= size <= BOARD_N_MAX)
    // - Trả về true nếu hợp lệ, ngược lại false
    std::string buffer="";
    renderInput(renderer,"Chon kich thuoc bang",std::format(("Tu {} den {}"),BOARD_N_MIN,BOARD_N_MAX),buffer);
    SDL_Event e;
    while(SDL_WaitEvent(&e)){
        waitForQuit(e);
        if(e.type!=SDL_KEYDOWN) continue;
        if(e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9){
                buffer+=(char)e.key.keysym.sym;
        }
        else if(e.key.keysym.sym==SDLK_BACKSPACE && !buffer.empty()){
                buffer.pop_back();
            }
        else if(e.key.keysym.sym==SDLK_RETURN && !buffer.empty()){
                int val=std::stoi(buffer);
                if (BOARD_N_MIN<= val&&val<= BOARD_N_MAX){
                    *size=val;
                    return true;
                }
                return false;
            }
        renderInput(renderer,"Chon kich thuoc bang",std::format(("Tu {} den {}"),BOARD_N_MIN,BOARD_N_MAX),buffer);
    }
    return false;
}

/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 *   - goal: con trỏ lưu giá trị goal.
 *   - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật goal nếu hợp lệ.
 */
bool SDLInteraction::selectGoal(int* goal, const int size) {
    // TODO:
    // - Lấy input từ SDL
    // - Parse thành số nguyên
    // - Kiểm tra điều kiện (3 <= goal <= size)
    // - Trả về true nếu hợp lệ
    
    std::string buffer="";
    renderInput(renderer,"Chon goal",std::format("Tu 3 den {}", GOAL_MAX),buffer);
    SDL_Event e;
    while(SDL_WaitEvent(&e)){
        waitForQuit(e);
        if(e.type != SDL_KEYDOWN) continue;
            if(e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9){
                buffer+=(char)e.key.keysym.sym;
            }
            else if(e.key.keysym.sym==SDLK_BACKSPACE && !buffer.empty()){
                buffer.pop_back();
            }
            else if(e.key.keysym.sym==SDLK_RETURN && !buffer.empty()){
                int val=std::stoi(buffer);
                if (val >= 3 && val <= GOAL_MAX && val <= size){
                    *goal=val;
                    return true;
                }
                return false;
            }
        renderInput(renderer,"Chon goal",std::format("Tu 3 den {}", GOAL_MAX),buffer);
    }
    return false;
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 *   - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    // TODO:
    // - Lắng nghe input từ SDL (phím số hoặc click)
    // - Map input sang GameMode tương ứng
    // - Validate giá trị (1-3)
    SDL_Event e;
    while(SDL_WaitEvent(&e)){
        waitForQuit(e);
        if(e.type==SDL_KEYDOWN){
            if(e.key.keysym.sym==SDLK_1){
                *mode=GameMode::PVP;
                return true;
            }
            else if(e.key.keysym.sym==SDLK_2){
                *mode=GameMode::PVE;
                return true;
            }
            else if(e.key.keysym.sym==SDLK_3){
                *mode=GameMode::EVE;
                return true;
            }
            else return false;
        }
    }
    return false;
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 *   - levels: mảng lưu cấp độ bot.
 *   - index: vị trí bot cần gán.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
    // TODO:
    // - Kiểm tra index hợp lệ (0 hoặc 1)
    // - Lấy input từ SDL
    // - Map sang BotLevel (EASY, MEDIUM, HARD)
    // - Trả về true nếu hợp lệ
    SDL_Event e;
    while(SDL_WaitEvent(&e)){
        waitForQuit(e);
        if(e.type==SDL_KEYDOWN){
            if(e.key.keysym.sym==SDLK_1){
                levels[index]=BotLevel::EASY;
                return true;
            }
            else if(e.key.keysym.sym==SDLK_2){
                levels[index]=BotLevel::MEDIUM;
                return true;
            }
            else if(e.key.keysym.sym==SDLK_3){
                levels[index]=BotLevel::HARD;
                return true;
            }
            else return false;
        }
    }
    return false;
}
static void renderMove(SDLRenderer* r, const std::string& buf_row, const std::string& buf_col) {
    SDL_Renderer* rend = r->getSDLRenderer();
    TTF_Font* font = r->getFont();
    TTF_Font* font_small = r->getFontSmall();
    if (!rend) return;
    r->clearScreen();
    r->redrawBoard();

    SDL_Rect bg = {0, 610, 800, 220};
    SDL_SetRenderDrawColor(rend, 20, 20, 20, 255);
    SDL_RenderFillRect(rend, &bg);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray  = {160, 160, 160, 255};
    itxt(rend, font, "TIC TAC TOE", 400, 70, white);
    std::string playerText;
    if(r->getIsBot()) playerText = std::format("Bot {} is thinking...", r->getPlayer()+1);
    else playerText = std::format("Player {} turn", r->getPlayer()+1);
    itxt(rend, font, playerText, 400, 630, white);
    itxt(rend, font, "Enter move:", 400, 680, white);
    itxt(rend, font, "Move: [" + buf_row + (buf_col.empty() ? "" : ", " + buf_col) + "]", 400, 730, white);
    itxt(rend, font_small, "<ENTER> DE DONG Y", 400, 780, white);
    SDL_RenderPresent(rend);
}


/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Đầu vào:
 *   - row: con trỏ lưu hàng.
 *   - col: con trỏ lưu cột.
 * Đầu ra:
 *   - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 *   - Cập nhật row, col nếu hợp lệ.
 */
bool SDLInteraction::getPlayerMove(int* row, int* col) {
    // TODO:
    // - Lắng nghe mouse click hoặc keyboard input
    // - Chuyển đổi tọa độ click thành (row, col)
    // - Validate phạm vi hợp lệ
    SDL_Event e;
    std::string buffer_row="";
    std::string buffer_col="";
    bool nhaprow=true;
    renderMove(renderer,buffer_row,buffer_col);
    while(SDL_WaitEvent(&e)){
        waitForQuit(e);
        if(e.type==SDL_KEYDOWN){
            if(e.key.keysym.sym >=SDLK_0 && e.key.keysym.sym <=SDLK_9){
                if(nhaprow){
                    buffer_row+=(char)e.key.keysym.sym;      //chuyển Sdl qua kí tự
                }
                else buffer_col+=(char)e.key.keysym.sym; 
            }
            if(e.key.keysym.sym==SDLK_BACKSPACE){
                if(nhaprow && !buffer_row.empty()) buffer_row.pop_back();
                else if(!nhaprow && !buffer_col.empty()) buffer_col.pop_back();
            }
            if(e.key.keysym.sym==SDLK_RETURN){
                if(nhaprow && !buffer_row.empty()){
                    *row=stoi(buffer_row);
                    nhaprow=false;
                }
                else if(!nhaprow && !buffer_col.empty()){
                    *col=stoi(buffer_col);
                    return true;
                }
            }
        }
        renderMove(renderer,buffer_row,buffer_col);

    }
    
    return false;
}

/**
 * Mô tả: Dọn dẹp tài nguyên liên quan đến SDL interaction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Giải phóng hoặc reset trạng thái input nếu cần.
 */
void SDLInteraction::close() {
    // TODO:
    // - Dọn dẹp trạng thái, nếu có resource thì giải phóng
}
