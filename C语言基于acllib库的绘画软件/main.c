#include "acllib.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _data {
    int x;
    int y;
    int key;
    int event;
} Data;

typedef struct _node {
    Data data;
    struct _node* next;
} Node;

Node* head;
ACL_Color c = BLACK;
int saveM = 0;
int readM = 0;
int markt = 0;

void insert(Node* list, int x, int y, int key, int event);
void save();
void read();
void mouseEvent(int x,int y,int key,int event);

void insert(Node* list, int x, int y, int key, int event) {
    Node* t = list;
    while(t->next!=NULL) t = t->next;
    
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = (Data){x, y, key, event};

    node->next = NULL;
    t->next = node;
}

void save() {
    FILE* p = fopen("test.txt", "w");
    Node* t = head;
    while(t->next!=NULL) {
        t = t->next;
        fprintf(p, "%d %d %d %d\n", t->data.x, t->data.y, t->data.key, t->data.event);
    }
    fprintf(p, "-1 -1 -1 -1");
    fclose(p);
    saveM = 0;//防止点击“保存”后还继续读下去
}

void read() {
    markt = 1;
    FILE* d = fopen("test.txt", "r");
    int x = 0, y = 0, key = 0, event = 0;

    fscanf(d, "%d%d%d%d", &x, &y, &key, &event);
    while(x!=-1&&y!=-1&&key!=-1&&event!=-1) {
        mouseEvent(x, y, key, event);
        fscanf(d, "%d%d%d%d", &x, &y, &key, &event);        
    }
    
    fclose(d);
    readM = 0;
}

void draw() {
    beginPaint();
    setPenWidth(2);
    setPenColor(BLACK);
    line(0, 54, 800, 54);
    setBrushColor(EMPTY);
    rectangle(7, 7, 7+70, 7+40);
    rectangle(87, 7, 87+70, 7+40);
    setTextSize(30);
    paintText(10, 10, "保存");
    paintText(90, 10, "读取");
       
    setBrushColor(BLUE);
    rectangle(187, 7, 187+40, 7+40);
           
    setBrushColor(RED);
    rectangle(237, 7, 237+40, 7+40);
        
    setBrushColor(GREEN);
    rectangle(287, 7, 287+40, 7+40);
        
    setBrushColor(SKYBLUE);
    rectangle(337, 7, 337+40, 7+40);
        
    setBrushColor(PURPLE);
    rectangle(387, 7, 387+40, 7+40);
        
    setBrushColor(PINK);
    rectangle(437, 7, 437+40, 7+40);
        
    setBrushColor(BLACK);
    rectangle(487, 7, 487+40, 7+40);
    
    setBrushColor(EMPTY);
    rectangle(547, 7, 547+70, 7+40);
    rectangle(627, 7, 627+70, 7+40);
    setTextSize(30);
    paintText(550, 10, "撤销");
    paintText(630, 10, "重画");
    
    rectangle(707, 7, 707+70, 7+40);
    paintText(710, 10, "清空");  
    
    endPaint();
}

//坐标要记录什么？按下的（按钮区域除外），移动的（按钮区域除外），放的
void mouseEvent(int x,int y,int key,int event) {
	draw();
    static int mark = 0;
    static int ox = 0;
    static int oy = 0; 
   
    if(saveM) save();
    if(readM&&markt==0) {
        read();
        markt= 0;
    } 
//&||||key==RIGHT_BUTTON
    if(key==LEFT_BUTTON&&event==BUTTON_DOWN) {
        if(7<=x&&x<=77&&7<=y&&y<=47) saveM = 1;
        if(87<=x&&x<=157&&7<=y&&y<=47) readM = 1;
        if(!(7<=x&&x<=177&&7<=y&&y<=47)) mark = 1;//if(key==LEFT_BUTTON&&event==BUTTON_DOWN) mark = 1;也要改
        if(187<=x&&x<=227&&7<=y&&y<=47) c = BLUE;
        if(237<=x&&x<=277&&7<=y&&y<=47) c = RED;       
        if(287<=x&&x<=327&&7<=y&&y<=47) c = GREEN;
        if(337<=x&&x<=377&&7<=y&&y<=47) c = SKYBLUE;
        if(387<=x&&x<=427&&7<=y&&y<=47) c = PURPLE;
        if(437<=x&&x<=477&&7<=y&&y<=47) c = PINK;
        if(487<=x&&x<=527&&7<=y&&y<=47) c = BLACK;
        if(707<=x&&x<=777&&7<=y&&y<=47) {
            beginPaint();
            setBrushColor(WHITE);
            setPenColor(WHITE);
            rectangle(0, 54, 800, 600); 
            endPaint();
        }
    }

    if(!(7<=x&&x<=177&&7<=y&&y<=47)) insert(head, x, y, key, event);
          
    if(key==LEFT_BUTTON&&event==BUTTON_UP)  mark = 0;
    
    if(key==RIGHT_BUTTON&&event==BUTTON_DOWN) {
        c = WHITE;
        if(!(7<=x&&x<=177&&7<=y&&y<=47)) mark = 1;
    } 
    
    if(key==RIGHT_BUTTON&&event==BUTTON_UP) {
        c = BLACK;
        mark = 0;
    }   
    
	if(mark==0&&!(7<=x&&x<=177&&7<=y&&y<=47)) {//就这么办，简单
		ox = x;
		oy = y;
	}
    
    if(event==MOUSEMOVE&&mark==1) {
    	beginPaint();
    	setPenWidth(5);
        if(c==WHITE) setPenWidth(20);      
        setPenColor(c);
    	line(ox, oy, x, y);
    	ox = x;
    	oy = y;
        
    	endPaint();
	}
}

int Setup() {
    head = (Node*)malloc(sizeof(Node));
    head->next = NULL;
    initWindow("绘图",DEFAULT,DEFAULT,800,600);
    //开始布局，不放在点击上面，是因为当你一开始鼠标不在这个界面上的时候，它就不会出现     
    draw();
    registerMouseEvent(mouseEvent);
    return 0;
}
