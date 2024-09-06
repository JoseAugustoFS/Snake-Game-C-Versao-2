#include <windows.h>
#include <gl/gl.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#define CORPO 20
#define TAMANHO 0.04
#define TAMANHO_MACA 0.01
#define TAMANHO_BORDA 0.92

//Profiles: Pequeno=0.02|0.005|0.94 Grande=0.04|0.01|0.92

GLfloat movx, movy = 0;
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void Atualizar_Posicao();
void Testar_Colisoes();
void Gerar_Maca_Coordenadas();
void Teclado();
int quantidade=5, quantidade_buffer=5, posicao=2, velocidade=180, atualizar, status=0;
float x[CORPO+5], y[CORPO+5], xmaca=2, ymaca=2;
char placar[13];
HWND texto, hwnd;



int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    int quadrado_atual;
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC, hdctexto;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;
    char vitoria[25], gameover[25];
    PAINTSTRUCT ps;
    WPARAM hFont, hFont2;
    sprintf(placar,"Placar %d/%d",quantidade-5,CORPO);

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;

    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          556,
                          556,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);
    texto=CreateWindow("STATIC",
                        placar,
                        WS_VISIBLE | WS_CHILD,
                        20,0,82,16,
                        hwnd, NULL, NULL, NULL);
    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {


            /* OpenGL animation code goes here */
            if(quantidade-5==CORPO)
            {
                sprintf(vitoria,"Voce Ganhou!");
                texto=CreateWindow("STATIC",
                                    vitoria,
                                    WS_VISIBLE | WS_CHILD,
                                    169.5,260.5,217,35,
                                    hwnd, NULL, NULL, NULL);
                hFont=CreateFont (40, 0, 0, 0,
                                  FW_DONTCARE,
                                  FALSE, FALSE, FALSE,
                                  ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                  DEFAULT_PITCH | FF_SWISS, L"Arial");
                SendMessage (texto, WM_SETFONT, hFont, TRUE);
                status=1;

            }
            if(status==2)
            {
                sprintf(gameover,"GAME OVER!");
                texto=CreateWindow("STATIC",
                                    gameover,
                                    WS_VISIBLE | WS_CHILD,
                                    167,260.5,222,35,
                                    hwnd, NULL, NULL, NULL);
                hFont2=CreateFont (40, 0, 0, 0,
                                    FW_DONTCARE,
                                    FALSE, FALSE, FALSE,
                                    ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                    DEFAULT_PITCH | FF_SWISS, L"Arial");
                SendMessage (texto, WM_SETFONT, hFont2, TRUE);
            }
            if(status==0)
            {

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();

                Teclado();
                if(atualizar==0)
                {
                    Atualizar_Posicao();
                }



                //Borda
                glBegin(GL_QUADS);
                    glColor3f(1,0,0);
                    glVertex2f(1,-1);
                    glVertex2f(1,1);
                    glVertex2f(-1,1);
                    glVertex2f(-1,-1);


                glEnd();



                //Fundo
                glBegin(GL_QUADS);
                    glColor3f(0,0,0);
                    glVertex2f(TAMANHO_BORDA,-TAMANHO_BORDA);
                    glVertex2f(TAMANHO_BORDA,TAMANHO_BORDA);
                    glVertex2f(-TAMANHO_BORDA,TAMANHO_BORDA);
                    glVertex2f(-TAMANHO_BORDA,-TAMANHO_BORDA);
                glEnd();

                x[0]=movx;
                y[0]=movy;

                //Corpo da Cobra
                for(quadrado_atual=quantidade;quadrado_atual>=1;quadrado_atual--)
                {
                    x[quadrado_atual]=x[quadrado_atual-1];
                    y[quadrado_atual]=y[quadrado_atual-1];
                }
                glBegin(GL_QUADS);
                for(quadrado_atual=1;quadrado_atual<=quantidade;quadrado_atual++)
                {
                    glColor3f(0.3,0.3,0.6);
                    glVertex2f(x[quadrado_atual]+TAMANHO,y[quadrado_atual]-TAMANHO);
                    glColor3f(0.2,0.2,0.5);
                    glVertex2f(x[quadrado_atual]+TAMANHO,y[quadrado_atual]+TAMANHO);
                    glColor3f(0.1,0.1,0.4);
                    glVertex2f(x[quadrado_atual]-TAMANHO,y[quadrado_atual]+TAMANHO);
                    glColor3f(0,0,0.3);
                    glVertex2f(x[quadrado_atual]-TAMANHO,y[quadrado_atual]-TAMANHO);

                }
                glEnd();

                //Cabeça da Cobra

                glBegin(GL_QUADS);
                    glColor3f(0.3,0.3,1);
                    glVertex2f(x[0]+TAMANHO,y[0]-TAMANHO);
                    glColor3f(0.2,0.2,0.8);
                    glVertex2f(x[0]+TAMANHO,y[0]+TAMANHO);
                    glColor3f(0.1,0.1,0.6);
                    glVertex2f(x[0]-TAMANHO,y[0]+TAMANHO);
                    glColor3f(0,0,0.4);
                    glVertex2f(x[0]-TAMANHO,y[0]-TAMANHO);
                glEnd();

                //Maçã
                if((xmaca==2)||(ymaca==2))
                {
                    Gerar_Maca_Coordenadas();
                }
                glBegin(GL_QUADS);
                    glColor3f(0,1,0);
                    glVertex2f(xmaca+TAMANHO_MACA,ymaca-TAMANHO_MACA);
                    glVertex2f(xmaca+TAMANHO_MACA,ymaca+TAMANHO_MACA);
                    glVertex2f(xmaca-TAMANHO_MACA,ymaca+TAMANHO_MACA);
                    glVertex2f(xmaca-TAMANHO_MACA,ymaca-TAMANHO_MACA);
                glEnd();



            glPopMatrix();

            SwapBuffers(hDC);
            }
            atualizar=0;
            Testar_Colisoes();
            glFlush();
            Sleep (velocidade);
            if(quantidade!=quantidade_buffer)
            {
                sprintf(placar,"Placar %d/%d",quantidade-5,CORPO);
                texto=CreateWindow("STATIC",
                                    placar,
                                    WS_VISIBLE | WS_CHILD,
                                    20,0,82,16,
                                    hwnd, NULL, NULL, NULL);
                quantidade_buffer=quantidade;
            }
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_CTLCOLORSTATIC:
            if(status==0)
            {
                    SetTextColor((HDC)wParam,RGB(0,0,0));
                    SetBkMode((HDC)wParam,TRANSPARENT);
                return GetClassLong(hwnd, GCL_HBRBACKGROUND);
            }
            else
            if(status==1)
            {
                    SetTextColor((HDC)wParam,RGB(0,255,0));
                    SetBkColor((HDC)wParam,RGB(0,0,0));
                    SetBkMode((HDC)wParam,OPAQUE);
                return GetClassLong(hwnd, GCL_HBRBACKGROUND);
            }
            if(status==2)
            {
                    SetTextColor((HDC)wParam,RGB(255,0,0));
                    SetBkColor((HDC)wParam,RGB(0,0,0));
                    SetBkMode((HDC)wParam,OPAQUE);
                return GetClassLong(hwnd, GCL_HBRBACKGROUND);
            }

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
void Atualizar_Posicao()
{
    switch(posicao)
    {
        case 0: movy+=TAMANHO*2;
                break;
        case 1: movy-=TAMANHO*2;
                break;
        case 2: movx+=TAMANHO*2;
                break;
        case 3: movx-=TAMANHO*2;
                break;
    }
}
void Testar_Colisoes()
{
    int quadrado_atual;
    //Borda
    if((x[0]+TAMANHO>=TAMANHO_BORDA+TAMANHO)||(x[0]-TAMANHO<=-TAMANHO_BORDA-TAMANHO)||(y[0]+TAMANHO>=TAMANHO_BORDA+TAMANHO)||(y[0]-TAMANHO<=-TAMANHO_BORDA-TAMANHO))
    {
        status=2;

    }
    //Maçã
    if((xmaca>=x[0]-TAMANHO)&&(xmaca<=x[0]+TAMANHO)&&(ymaca>=y[0]-TAMANHO)&&(ymaca<=y[0]+TAMANHO))
    {
        Gerar_Maca_Coordenadas();
        quantidade++;
        if(velocidade>80)
        {
            velocidade-=10;
        }
    }
    //Corpo
    for(quadrado_atual=2;quadrado_atual<=quantidade;quadrado_atual++)
    {
        if((x[quadrado_atual]-TAMANHO==x[0]-TAMANHO)&&(x[quadrado_atual]+TAMANHO==x[0]+TAMANHO)&&(y[quadrado_atual]-TAMANHO==y[0]-TAMANHO)&&(y[quadrado_atual]+TAMANHO==y[0]+TAMANHO))
        {
            status=2;

        }
    }
}
void Gerar_Maca_Coordenadas()
{
    int quadrado_atual, parametro;
    parametro=(TAMANHO_BORDA-TAMANHO)*100;
    inicio:
    srand(time(NULL));
    xmaca= (rand()%(parametro*2+1)-parametro);
    ymaca= (rand()%(parametro*2+1)-parametro);
    xmaca=xmaca/100;
    ymaca=ymaca/100;
    for(quadrado_atual=0;quadrado_atual<=quantidade;quadrado_atual++)
    {
        if((xmaca>=x[quadrado_atual]-TAMANHO)&&(xmaca<=x[quadrado_atual]+TAMANHO)&&(ymaca>=y[quadrado_atual]-TAMANHO)&&(ymaca<=y[quadrado_atual]+TAMANHO))
        {
            goto inicio;
        }
    }

}
void Teclado()
{
    if(atualizar==0)
    {
    if ((GetAsyncKeyState(VK_UP))||(GetAsyncKeyState(0x57)))
    {
        if(posicao!=0)
        {
            posicao=0;
            movy+=TAMANHO*2;
            atualizar=1;
            return;
        }
    }

    if ((GetAsyncKeyState(VK_DOWN))||(GetAsyncKeyState(0x53)))
    {
        if(posicao!=1)
        {
            posicao=1;
            movy-=TAMANHO*2;
            atualizar=1;
            return;
        }
    }

    if ((GetAsyncKeyState(VK_RIGHT))||(GetAsyncKeyState(0x44)))
    {
        if(posicao!=2)
        {
            posicao=2;
            movx+=TAMANHO*2;
            atualizar=1;
            return;
        }
    }

    if ((GetAsyncKeyState(VK_LEFT))||(GetAsyncKeyState(0x41)))
    {
        if(posicao!=3)
        {
            posicao=3;
            movx-=TAMANHO*2;
            atualizar=1;
            return;
        }
    }



    }




}
