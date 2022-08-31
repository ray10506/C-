#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <opencv/highgui.h>
#include <conio.h>
#include <time.h>


IplImage * imgCanvas=NULL;
IplImage * imgstart=NULL;
IplImage * imgKid=NULL;
IplImage * imgOver=NULL;
IplImage * imglife=NULL;
IplImage * imgupteeth=NULL;
IplImage * imgdownteeth=NULL;
IplImage * imgboard=NULL;
IplImage * imgleftboard=NULL;
IplImage * imgrightboard=NULL;


int kidX=100,kidY=50;
int boardX[14]={0}, boardY[14]={390}, boardv=3;
bool onboard=false;

int Gameover(IplImage * Over)
{
    cvShowImage("Game", Over);
    cvWaitKey(0);
    Sleep(1000);
    return 0;
}

int Move(IplImage * imgCanvas, IplImage * imgKid)
{
    int key=cvWaitKey(30);
    if(key=='a' && kidX-10>10)
        kidX-=15;
    else if(key=='a' && kidX-10<=10)
    {
        kidX=11;
    }

    if(key=='d' && kidX+10+imgKid->width < (imgCanvas->width-10))
        kidX+=15;
    else if(key=='d' && kidX+10+imgKid->width >= imgCanvas->width)
    {
        kidX=270;
    }
    return 1;
}


void myPutImage(IplImage * canvas, IplImage *s,int x,int y)
{
    cvSetImageROI(canvas, cvRect(x,y,s->width, s->height));
    cvCopy(s,canvas);
    cvResetImageROI(canvas);
}

int main()
{
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,1.0f,1.0f,0,1,CV_AA);
    srand(time(NULL));
    imgCanvas = cvCreateImage(cvSize(300,400),IPL_DEPTH_8U,3);
    imgstart = cvLoadImage("start.png");
    imgKid = cvLoadImage("kid.png");
    imgOver = cvLoadImage("gameover.png");
    imglife = cvLoadImage("life.png");
    imgupteeth = cvLoadImage("teeth2.png");
    imgdownteeth = cvLoadImage("teeth.png");
    imgleftboard = cvLoadImage("leftboard.png");
    imgrightboard = cvLoadImage("rightboard.png");
    imgboard = cvLoadImage("board.png");

    int life=10,times=1,boardnum=1,hurt=0,time=0,run=0;
    char word[5];
    for(int i=0; i<10; i++){
        boardX[i]=rand()%263;
    }

    cvShowImage("Game", imgstart);
    cvWaitKey(0);
    Sleep(500);

    while(1)
    {
        cvSetZero(imgCanvas);
        ///清理畫布

        time=run/8;
        cvPutText(imgCanvas,"Time: ",cvPoint(210,27),&font,CV_RGB(0,255,255));
        cvPutText(imgCanvas,itoa(time,word,10),cvPoint(260,27),&font,CV_RGB(0,255,255));
        Sleep(6);
        run+=1;

        myPutImage(imgCanvas, imgKid, kidX, kidY);
        for(int i=0 ; i<life ; i++)
        {
            myPutImage(imgCanvas, imglife,10+i*(imglife->width),15);
        }
        for(int i=0 ; i<10 ; i++)
        {
            myPutImage(imgCanvas, imgupteeth, i*(imgupteeth->width), 0);
        }
        if(times%50==0)
        {
            if(boardnum<13)
                 boardnum+=1;
        }
        for(int i=0; i<boardnum; i++)
        {
            if(i%2==0)
                myPutImage(imgCanvas, imgboard, boardX[i], boardY[i]);
            else
                myPutImage(imgCanvas, imgdownteeth, boardX[i], boardY[i]);
        }
        for(int i=0;i<8;i++)
        {
            myPutImage(imgCanvas, imgleftboard, 0, i*(imgleftboard->height ));
        }
        for(int i=0;i<8;i++)
        {
            myPutImage(imgCanvas, imgrightboard, 290, i*(imgrightboard->height ));
        }
        cvShowImage("Game", imgCanvas);
        ///以上載入圖片

        if(Move(imgCanvas, imgKid)==0)
            return 0;
        ///移動

        for(int i=0; i<boardnum; i++)
        {
            if((kidY + imgKid->height == boardY[i]) || ((boardY[i] <= (kidY + imgKid->height+4)) && ( (kidY + imgKid->height + 4) <= boardY[i]+boardv ) ) )
            {
                if(kidX >= boardX[i] + imgboard->width || kidX + imgKid->width <= boardX[i])
                {
                    onboard=false;
                    hurt=0;
                }
                else if(i%2==0)
                {
                    onboard=true;
                }
                else
                {
                    if(hurt==0){
                        life-=1;
                        hurt=1;
                    }
                    if(life==0)
                    {
                        if(Gameover(imgOver)==0)
                            return 0;
                    }
                    onboard=true;
                }
            }
            if(boardY[i]-boardv <= 20 )
            {
                boardY[i]=390;
                boardX[i]=rand()%263;
            }
            else{
                boardY[i]-=boardv;
            }
        }
        ///看人物有沒有站在任何一塊板子上

        if(onboard==false){
            kidY+=3;
            if(kidY+imgKid->height >= imgCanvas->height)
            {
                if(Gameover(imgOver)==0)
                    return 0;
            }
        }
        else if(onboard==true){
            kidY-=boardv;
            if(life>0 && kidY<=20)
            {
                life-=1;
                kidY=100;
                onboard=false;
                hurt=0;
                kidY+=1;
                if(life==0)
                {
                    if(Gameover(imgOver)==0)
                        return 0;
                }
            }
        }


        times+=1;
        if(times%500==0 && boardv<=12){
            boardv+=1;
        }
        if(times%750==0 && boardnum>3){
            boardnum-=1;
        }
    }
    return 0;
}

