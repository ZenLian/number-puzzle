#include <ncurses.h>
#include <algorithm>
#include <vector>
#include <random>
#include <cstring>
#include <cstdlib>
// for debug
//#include <fstream>

using std::vector;

static inline int myrandom(int i)
{
    std::random_device rd;
    return rd()%i;
}

class Puzzle {
public:
    Puzzle(int _size = 3);
    void start();
private:
    WINDOW *puzzle_window;
    int size; // 拼图大小
    int blank_index; // 空白块位置
    vector<int> pieces; // 存储拼图数字
    // for debug
    //std::ofstream dbg;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void displayPiece(int index);
    bool isComplete();

    int getY(int index)          { return index / size + 1; }
    int getX(int index)          { return (index % size) * 2 + 1 ; }
    int getIndexUp(int index)    { return (index / size) ? (index - size) : -1; }
    int getIndexDown(int index)  { return (index / size != size - 1) ? (index + size) : -1; }
    int getIndexLeft(int index)  { return (index % size) ? (index - 1) : -1; }
    int getIndexRight(int index) { return (index % size != size - 1) ? (index + 1) : -1; }
};

Puzzle::Puzzle(int _size) : size(std::max(_size, 3))
{
    //dbg.open("debug.out");

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0); // 隐藏光标
    printw("Press q to quit");
    refresh();

    // 创建拼图窗口，并绘制边框
    //dbg << "[debug] create window..." << std::endl;
    puzzle_window = newwin(size+2, size*2+1, 10, 10);
    box(puzzle_window, 0, 0);
    //getch();
    wrefresh(puzzle_window);

    // 生成数字，并随机打乱
    //dbg << "[debug] generate numbers..." << std::endl;
    for (int x = 1; x < size * size; ++x)
        pieces.push_back(x);
    std::random_shuffle(pieces.begin(), pieces.end(), myrandom);
    std::swap(pieces[0], pieces[1]); // 打乱后无解，必须交换
    pieces.push_back(0);
    blank_index = pieces.size() - 1;

    for (int i = 0; i < pieces.size(); ++i) {
        dbg << "pieces[" << i << "] = " << pieces[i] << std::endl;
    }

    // 将数字显示到相应位置
    for (int i = 0; i < pieces.size(); ++i)
        displayPiece(i);

}

// 读取用户输入，移动拼图
void Puzzle::start()
{
    int c;
    while ((c = getch()) != 'q') {
        switch (c) {
        case KEY_UP:
            moveUp();
            break;
        case KEY_DOWN:
            moveDown();
            break;
        case KEY_LEFT:
            moveLeft();
            break;
        case KEY_RIGHT:
            moveRight();
            break;
        default:
            break;
        }
        // 检查是否完成拼图
        if (isComplete()) {
            char str[] = "Congratulations!";
            int len = strlen(str);
            mvaddstr(size + 2, 0, str);
            getch();
            return;
        }
    }
}

void Puzzle::moveUp()
{
    int move_index = getIndexDown(blank_index);
    if (move_index == -1)
        return;

    std::swap(pieces[blank_index], pieces[move_index]);
    std::swap(blank_index, move_index);
    displayPiece(blank_index);
    displayPiece(move_index);
    wrefresh(puzzle_window);
}

void Puzzle::moveDown()
{
    int move_index = getIndexUp(blank_index);
    if (move_index == -1)
        return;

    std::swap(pieces[blank_index], pieces[move_index]);
    std::swap(blank_index, move_index);
    displayPiece(blank_index);
    displayPiece(move_index);
}

void Puzzle::moveLeft()
{
    int move_index = getIndexRight(blank_index);
    if (move_index == -1)
        return;

    std::swap(pieces[blank_index], pieces[move_index]);
    std::swap(blank_index, move_index);
    displayPiece(blank_index);
    displayPiece(move_index);
}

void Puzzle::moveRight()
{
    int move_index = getIndexLeft(blank_index);
    if (move_index == -1)
        return;

    std::swap(pieces[blank_index], pieces[move_index]);
    std::swap(blank_index, move_index);
    displayPiece(blank_index);
    displayPiece(move_index);
}

bool Puzzle::isComplete()
{
    for (int i = 0; i < pieces.size() - 1; ++i)
        if (pieces[i] != i+1)
            return false;
    return true;
}

void Puzzle::displayPiece(int index)
{
    //dbg << "[debug] in displayPiece(" << index << ")" << std::endl;
    int y = getY(index);
    int x = getX(index);
    //dbg << "index " << index << " at " << "(" << y << ", " << x << ")" << std::endl;

    if (pieces[index]) {
    // 非空白块
        chtype ch = pieces[index] + '0';

        mvwaddch(puzzle_window, y, x, ch);
    } else {
    // 空白块
        //wstandout(puzzle_window);
        mvwaddch(puzzle_window, y, x, ' ');
        //wstandend(puzzle_window);
    }
    wrefresh(puzzle_window);
}

int main(int argc, char const* argv[])
{
    Puzzle puzzle(3); // 3*3的拼图

    puzzle.start();
    return 0;
}
