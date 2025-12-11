#pragma once
#include <windows.h>
#include <graphics.h>   
#include <stddef.h>    
#include <wchar.h>     
#include <io.h>         

#ifdef __cplusplus
extern "C" {
#endif


    //按钮的结构
    typedef struct MenuItem
    {
        wchar_t text[64];
        RECT rect;
        int hovered;
    } MenuItem;

    //画出来玩家需要xy坐标
    typedef struct PlayerVisual
    {
        float x;
        float y;



    }PlayerVisual;

    //敌人同理
    typedef struct EnemyVisual {

        float x;
        float y;

    }EnemyVisual;

    //子弹的位置
    typedef struct BulletVisual {
        float x;
        float y;
        int formPlayerOrEnemy;//子弹来自玩家还是敌人 如果是玩家的话就是1，敌人就是0

    }BulletVisual;

    typedef struct GamePlayVisualState //
    {
        int width;
        int height;
        int score;              //在底部显示当前分数
        int showWasted;  //如果玩家死了，在屏幕上显示WASTED并且显示死亡原因（）
        const wchar_t* deathReason;
        PlayerVisual         player;
        const EnemyVisual* enemies;
        size_t               enemyCount;
        const BulletVisual* bullets;
        size_t               bulletCount;

    }GamePlayVisualState;

    //用来存放我的图片，如果加载图片没成功，也不会崩溃
    typedef struct RenderTextures
    {
        IMAGE menuBg;
        IMAGE gameBg;
        IMAGE player;
        IMAGE enemy;
        IMAGE playerBullet;
        IMAGE enemyBullet;
        int   menuBgOk;
        int   gameBgOk;
        int   playerOk;
        int   enemyOk;
        int   playerBulletOk;
        int   enemyBulletOk;
    } RenderTextures;

    extern RenderTextures g_renderTextures;

    //分别负责打开窗口，关上窗口
    int  RenderInitialize(int width, int height, const wchar_t* title);
    void RenderShutdown(void);

    //加载资源
    int RenderLoadMenuTexture(const wchar_t* menuBgPath);
    int RenderLoadGameplayTextures(const wchar_t* gameBgPath,
        const wchar_t* playerPath,
        const wchar_t* enemyPath,
        const wchar_t* playerBulletPath,
        const wchar_t* enemyBulletPath);


    //渲染菜单和游戏画面的接口
    int  RenderShowMainMenu(int width, int height, int bestScore);   //就是之前Menu.h里面的ShowMainMenu
    void RenderDrawGameplay(const GamePlayVisualState* state);   //把游戏里的这一帧对应的信息绘制出来、

    const wchar_t* ResolveAssetPath(const wchar_t* relativePath);
    static inline int LoadTextureInternal(IMAGE* img, int* flag, const wchar_t* path);

    // 在一个编译单元里真正定义纹理实例，其他文件只会引用 extern 声明。
    RenderTextures g_renderTextures = { 0 };

    int RenderInitialize(int width, int height, const wchar_t* title)
    {
        // 创建 EasyX 窗口
        initgraph(width, height);
        setbkmode(TRANSPARENT);

        if (title != NULL && title[0] != L'\0')
        {
            SetWindowTextW(GetHWnd(), title);
        }

        return 1;
    }

    void RenderShutdown(void)
    {
        //关闭窗口
        closegraph();
    }

    int RenderLoadMenuTexture(const wchar_t* menuBgPath)
    {
        // 如果没有菜单背景图片就用纯色的窗口
        return LoadTextureInternal(&g_renderTextures.menuBg,
            &g_renderTextures.menuBgOk,
            menuBgPath);
    }

    int RenderLoadGameplayTextures(const wchar_t* gameBgPath,
        const wchar_t* playerPath,
        const wchar_t* enemyPath,
        const wchar_t* playerBulletPath,
        const wchar_t* enemyBulletPath)
    {
        // 游戏内所有贴图的批量加载
        int ok1 = LoadTextureInternal(&g_renderTextures.gameBg, &g_renderTextures.gameBgOk, gameBgPath);
        int ok2 = LoadTextureInternal(&g_renderTextures.player, &g_renderTextures.playerOk, playerPath);
        int ok3 = LoadTextureInternal(&g_renderTextures.enemy, &g_renderTextures.enemyOk, enemyPath);
        int ok4 = LoadTextureInternal(&g_renderTextures.playerBullet, &g_renderTextures.playerBulletOk, playerBulletPath);
        int ok5 = LoadTextureInternal(&g_renderTextures.enemyBullet, &g_renderTextures.enemyBulletOk, enemyBulletPath);

        return ok1 & ok2 & ok3 & ok4 & ok5;
    }

    //下面包含构造矩形，命中测试，绘制按钮等，也是之前Menu.h里的
    static inline RECT MenuMakeRect(int x, int y, int w, int h)
    {
        RECT r = { x, y, x + w, y + h };
        return r;
    }

    static inline int MenuHitTest(const MenuItem* item, int x, int y)
    {
        POINT pt = { x, y };
        return PtInRect(&item->rect, pt);
    }

    static inline void MenuCopyLabel(wchar_t* dst, size_t cap, const wchar_t* src)
    {
        if (dst == NULL || cap == 0)
        {
            return;
        }

#if defined(_MSC_VER)
        wcsncpy_s(dst, cap, (src != NULL) ? src : L"", _TRUNCATE);
#else
        wcsncpy(dst, (src != NULL) ? src : L"", cap - 1);
        dst[cap - 1] = L'\0';
#endif
    }

    static inline void MenuDrawButton(const MenuItem* button)//绘制菜单按钮
    {
        // 根据 hovered 状态渲染不同的按钮样式。
        if (button->hovered)
        {
            setfillcolor(RGB(80, 160, 255));
            setlinecolor(RGB(255, 255, 255));
            settextcolor(RGB(255, 255, 255));
        }
        else
        {
            setfillcolor(RGB(40, 80, 160));
            setlinecolor(RGB(200, 200, 200));
            settextcolor(RGB(230, 230, 230));
        }

        solidrectangle(button->rect.left, button->rect.top, button->rect.right, button->rect.bottom);
        rectangle(button->rect.left, button->rect.top, button->rect.right, button->rect.bottom);

        settextstyle(24, 0, L"宋体");
        RECT textRect = button->rect;
        drawtext(button->text, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    static void MenuRenderFrame(const MenuItem* buttons, size_t buttonCount, int width, int height, int bestScore)
    {
        wchar_t bestText[64];

        BeginBatchDraw();

        // 如果贴图没加载成功，则使用纯色背景。
        if (g_renderTextures.menuBgOk)
        {
            putimage(0, 0, &g_renderTextures.menuBg);
        }
        else
        {
            setfillcolor(RGB(10, 20, 60));
            solidrectangle(0, 0, width, height);
        }

        settextstyle(36, 0, L"宋体");
        settextcolor(RGB(255, 255, 200));
        outtextxy(width / 2 - 110, height / 4 - 40, L"飞机大战");

        settextstyle(20, 0, L"宋体");
        settextcolor(RGB(200, 200, 200));
        _snwprintf_s(bestText, sizeof(bestText) / sizeof(bestText[0]), L"最高分：%d", bestScore);
        outtextxy(width / 2 - textwidth(bestText) / 2, height / 4 + 10, bestScore);

        for (size_t i = 0; i < buttonCount; ++i)
        {
            MenuDrawButton(&buttons[i]);
        }

        settextstyle(16, 0, L"宋体");
        settextcolor(RGB(180, 180, 180));
        outtextxy(10, height - 36, L"鼠标悬停选择，左键点击确认");

        FlushBatchDraw();
    }
    //主菜单
    int RenderShowMainMenu(int width, int height, int bestScore)
    {

        const wchar_t* labels[] = { L"Start Game", L"Options", L"Exit" };
        const size_t buttonCount = sizeof(labels) / sizeof(labels[0]);
        const int buttonWidth = 240;
        const int buttonHeight = 56;
        const int spacing = 12;
        const int centerX = (width - buttonWidth) / 2;
        const int centerY = height / 2 - (int)((buttonHeight * buttonCount + spacing * (buttonCount - 1)) / 2);

        MenuItem buttons[3];

        for (size_t i = 0; i < buttonCount; ++i)
        {
            MenuCopyLabel(buttons[i].text, sizeof(buttons[i].text) / sizeof(buttons[i].text[0]), labels[i]);
            buttons[i].rect = MenuMakeRect(centerX, centerY + (int)i * (buttonHeight + spacing), buttonWidth, buttonHeight);
            buttons[i].hovered = 0;
        }

        for (;;)
        {
            ExMessage msg;
            while (peekmessage(&msg, EM_MOUSE, TRUE))
            {
                if (msg.message == WM_MOUSEMOVE)
                {
                    for (size_t i = 0; i < buttonCount; ++i)
                    {
                        buttons[i].hovered = MenuHitTest(&buttons[i], msg.x, msg.y);
                    }
                }
                else if (msg.message == WM_LBUTTONDOWN)
                {
                    for (size_t i = 0; i < buttonCount; ++i)
                    {
                        if (MenuHitTest(&buttons[i], msg.x, msg.y))
                        {
                            return (int)i;
                        }
                    }
                }
            }

            MenuRenderFrame(buttons, buttonCount, width, height, bestScore);
            Sleep(16);
        }

        return (int)(buttonCount - 1);
    }



    //游戏画面的渲染

    void RenderDrawGameplay(const GamePlayVisualState* state)
    {
        if (state == NULL)
        {
            return;
        }

        BeginBatchDraw();

        if (g_renderTextures.gameBgOk)
        {
            putimage(0, 0, &g_renderTextures.gameBg);
        }
        else
        {
            setfillcolor(RGB(5, 15, 40));
            solidrectangle(0, 0, state->width, state->height);
        }

        if (g_renderTextures.playerOk)
        {
            putimage((int)state->player.x, (int)state->player.y, &g_renderTextures.player);
        }

        if (g_renderTextures.enemyOk)
        {
            for (size_t i = 0; i < state->enemyCount; ++i)
            {
                putimage((int)state->enemies[i].x, (int)state->enemies[i].y, &g_renderTextures.enemy);
            }
        }

        for (size_t i = 0; i < state->bulletCount; ++i)
        {
            const BulletVisual* b = &state->bullets[i];
            IMAGE* tex = b->formPlayerOrEnemy ? &g_renderTextures.playerBullet : &g_renderTextures.enemyBullet;
            int ok = b->formPlayerOrEnemy ? g_renderTextures.playerBulletOk : g_renderTextures.enemyBulletOk;

            if (ok)
            {
                putimage((int)b->x, (int)b->y, tex);
            }
        }

        settextstyle(22, 0, L"宋体");
        settextcolor(RGB(255, 255, 255));

        {
            wchar_t scoreText[64];
            _snwprintf_s(scoreText, sizeof(scoreText) / sizeof(scoreText[0]), L"SCORE %d", state->score);
            outtextxy(state->width / 2 - textwidth(scoreText) / 2, state->height - 32, scoreText);
        }

        if (state->showWasted)
        {
            const wchar_t* deathReason = (state->deathReason != NULL) ? state->deathReason : L"";
            const wchar_t* prompt = L"按任意键返回标题界面...";

            setfillcolor(RGB(60, 60, 60));
            solidrectangle(0, 0, state->width, state->height);

            settextstyle(72, 0, L"Impact");
            settextcolor(RGB(220, 220, 220));
            outtextxy(state->width / 2 - textwidth(L"WASTED") / 2, state->height / 2 - 120, L"WASTED");

            settextstyle(28, 0, L"宋体");
            outtextxy(state->width / 2 - textwidth(deathReason) / 2, state->height / 2 - 40, deathReason);

            settextstyle(20, 0, L"宋体");
            outtextxy(state->width / 2 - textwidth(prompt) / 2, state->height / 2 + 60, prompt);
        }

        FlushBatchDraw();
    }




    const wchar_t* ResolveAssetPath(const wchar_t* relativePath)
    {
        static wchar_t resolved[MAX_PATH * 4];
        wchar_t modulePath[MAX_PATH];
        wchar_t base[MAX_PATH];
        wchar_t temp[MAX_PATH * 4];
        wchar_t* lastSlash;

        const wchar_t* parents[] = { L"", L"..\\", L"..\\..\\", L"..\\..\\..\\" };

        if (relativePath == NULL || relativePath[0] == L'\0')
        {
            resolved[0] = L'\0';
            return resolved;
        }

        if ((relativePath[0] == L'\\' && relativePath[1] == L'\\') ||
            (relativePath[1] == L':'))
        {
            wcsncpy_s(resolved, relativePath, sizeof(resolved) / sizeof(resolved[0]));
            resolved[(sizeof(resolved) / sizeof(resolved[0])) - 1] = L'\0';
            return resolved;
        }

        GetModuleFileNameW(NULL, modulePath, MAX_PATH);
        wcscpy_s(base, modulePath);

        lastSlash = wcsrchr(base, L'\\');
        if (lastSlash != NULL)
        {
            *(lastSlash + 1) = L'\0';
        }

        for (size_t i = 0; i < sizeof(parents) / sizeof(parents[0]); ++i)
        {
            _snwprintf_s(temp, sizeof(temp) / sizeof(temp[0]), L"%ls%ls%ls", base, parents[i], relativePath);

            if (_waccess(temp, 0) == 0)
            {
                wcsncpy_s(resolved, temp, sizeof(resolved) / sizeof(resolved[0]));
                resolved[(sizeof(resolved) / sizeof(resolved[0])) - 1] = L'\0';
                return resolved;
            }
        }

        _snwprintf_s(resolved, sizeof(resolved) / sizeof(resolved[0]), L"%ls%ls", base, relativePath);
        resolved[(sizeof(resolved) / sizeof(resolved[0])) - 1] = L'\0';
        return resolved;
    }

    static inline int LoadTextureInternal(IMAGE* img, int* flag, const wchar_t* path)
    {
        const wchar_t* resolved = ResolveAssetPath(path);
        int ok = (resolved[0] != L'\0' && loadimage(img, resolved) == 0);

        if (flag != NULL)
        {
            *flag = ok;
        }

        if (!ok)
        {
            wchar_t buffer[512];
            _snwprintf_s(buffer, sizeof(buffer) / sizeof(buffer[0]),
                L"加载图片失败：%ls\n尝试路径：%ls",
                (path != NULL) ? path : L"(null)", resolved);

            OutputDebugStringW(buffer);
            MessageBoxW(GetHWnd(), buffer, L"资源加载失败", MB_ICONERROR | MB_OK);
        }

        return ok;
    }



#ifdef __cplusplus
}
#endif
