/*这是一个菜单，鼠标悬停的时候可以高亮，但是我还没有学会鼠标命中测试和悬停高亮，我大概在明天补充完*/



#include <graphics.h>
#include <windows.h>
#include <string.h>

// 菜单的结构
typedef struct MenuItem {
    char text[64];
    RECT rect;
    int hovered;
} MenuItem;

//构造一个矩形
static RECT MakeRect(int x, int y, int w, int h) {
    RECT r; r.left = x; r.top = y; r.right = x + w; r.bottom = y + h; return r;
}
static int HitTest(const MenuItem* it, int x, int y) {
    POINT pt; pt.x = x; pt.y = y; return ::PtInRect(&it->rect, pt) != 0;
}
//绘制按钮
static void DrawButton(const MenuItem* it) {
    if (it->hovered) { setfillcolor(RGB(80, 160, 255)); setlinecolor(RGB(255, 255, 255)); settextcolor(RGB(255, 255, 255)); }
    else { setfillcolor(RGB(40, 80, 160));  setlinecolor(RGB(200, 200, 200)); settextcolor(RGB(230, 230, 230)); }
    solidrectangle(it->rect.left, it->rect.top, it->rect.right, it->rect.bottom);
    rectangle(it->rect.left, it->rect.top, it->rect.right, it->rect.bottom);
    setbkmode(TRANSPARENT);
    settextstyle(24, 0, "宋体");
    RECT tr = it->rect; drawtext(it->text, &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
//高亮
static void UpdateHover(MenuItem* items, int count, int mouseX, int mouseY, int* selected) {
    int anyHover = 0;
    for (int i = 0; i < count; ++i) {
        int h = HitTest(&items[i], mouseX, mouseY);
        items[i].hovered = h;
        if (h) { *selected = i; anyHover = 1; }
    }
    if (!anyHover) {
        for (int i = 0; i < count; ++i) items[i].hovered = (i == *selected);
    }
}

int main() {
    const int W = 480, H = 360;
    initgraph(W, H);
    setbkmode(TRANSPARENT);

    //菜单


    //初始状态


        //鼠标消息


        //绘制
    setfillcolor();
    solidrectangle(0, 0, W, H);
    for ()
        DrawButton();
    settextstyle);
    settextcolor();
    outtextxy();

    FlushBatchDraw();
    Sleep(16);
}
}