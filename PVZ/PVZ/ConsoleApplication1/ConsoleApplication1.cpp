#include <iostream>
#include<graphics.h>
#include<conio.h>
#include<string>
#include<algorithm>
#include<bits/stdc++.h>
#include<vector>
#include<stdio.h>
#include<Mmsystem.h>
#include<Windows.h>
#include <random>
//一些参数
const int WindowWidth = 1200;
const int WindowHeight = 700;
const int plant_width = 70;
const int plant_height = 70;
const int Zombies_width = 90; 
const int Zombies_height = 90;
const int top_plant_x = 400, top_plant_y = 70;
const int zombie_v = 1;			//僵尸移动速度
const int plant_hp[6] = {0,100,200,30000,1,6000};			//植物血量
const int zombie_hp = 200;			//僵尸血量
const int zombie_init_x = 1080;			//初始僵尸横坐标
const int move_y = 20;						//僵尸向上的偏移量,
const int move_x = 40;						//僵尸向右的偏移量,
//偏移量的解释：若僵尸的图像输出位置是x，y。则实际碰撞体积为x + move_x , y - move_y
//和豌豆绘画的偏移量共用
const int plant_nxt_frame[5] = { 0,200,30,0,0 };	//植物行为间隔
const int hurt_plant = 20;
const int hurt_zombie = 4;
//地图信息
const int HY[9] = { 0,136,202,269,348,425,503,580,1066};
const int LX[11] = { 0,273,352,441,527,609,699,790,888,985,1066};
const int magma[8][5]{	{},
						{3,5,6,9},
						{1,9},
						{4,3,4,8,9},
						{0},
						{2,3,4},
						{3,4,6,9},
						{1,9} };
//阳光系统
int sun = 75;
const int sun_cost[6] = { 0,50,100,25,75,50 };
//冷却系统
const int cool_time[6] = { 0,100,100,75,150 ,300};
int cool[6] = { 0,50,50,50,125 ,125};


/******************************************************************************/
void music();
/******************************************************************************/
ExMessage msg;

unsigned seed;
//生成值域[L,R]的随机数
int rand_number(const int& l, const int& r) {
	if (l >= r)	return l;
	return rand() % (r - l + 1)+l;
}



//纵坐标变行数，在外面就返回0
int yh(const int& y) {
	if (y > HY[8] || y < HY[1])	return 0;
	int l = 1, r = 7;
	while(l+1 < r) {		//lower_bound莫名用不了，手搓二分
		int mid = (l + r) / 2;
		if (HY[mid] <= y) {
			l = mid;
		}
		else {
			r = mid;
		}
	}
	if (y < HY[r])	return l;
	else return r;
}

//横坐标变列数，在外面就返回0
int xl(const int& x) {
	if (x > 1066 || x < 276)	return 0;
	int l = 1, r = 9;
	while (l+1<r) {		//lower_bound莫名用不了，手搓二分
		int mid = (l + r) / 2;
		if (LX[mid] <= x) {
			l = mid;
		}
		else {
			r = mid;
		}
	}
	if (x < LX[r])	return l;
	else return r;

}

const int plant1_nxt_frame = 120;
const int plant5_nxt_frame = 100;

IMAGE mapp;                         //雪顶火山背景图
IMAGE p1[2], p2[2], p3[3], p4[2];   //向日葵，豌豆射手，棉花糖，冰淇淋
IMAGE p5[2], p6[2];                 //阳光，豌豆
IMAGE p7[2];						//吐司面包
IMAGE q1[30],q2[30],q3[14];
int len1=22, len2=21, len3=15;			//帧数

void put_plant(const IMAGE* p, const int& x, const int& y,int op=1) {                 //IMAGE指针，横纵坐标
	putimage(x, y, p + 0, SRCAND);                                           //先白底黑图
	putimage(x, y, p + op, SRCPAINT);                                         //再黑底彩图
}

void drawAlpha(IMAGE* image, int x, int y, int width, int height, int pic_x, int pic_y, double AA = 1)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();			// GetImageBuffer() 函数，用于获取绘图设备的显存指针， EasyX 自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(image);		// 获取 picture 的显存指针
	int imageWidth = image->getwidth();		// 获取图片宽度
	int imageHeight = image->getheight();	// 获取图片宽度
	int dstX = 0;							// 在 绘图区域 显存里像素的角标
	int srcX = 0;							// 在 image 显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < height; iy++)
	{
		for (int ix = 0; ix < width; ix++)
		{
			// 防止越界
			if (ix + pic_x >= 0 && ix + pic_x < imageWidth && iy + pic_y >= 0 && iy + pic_y < imageHeight &&
				ix + x >= 0 && ix + x < WindowWidth && iy + y >= 0 && iy + y < WindowHeight)
			{
				// 获取像素角标
				int srcX = (ix + pic_x) + (iy + pic_y) * imageWidth;
				dstX = (ix + x) + (iy + y) * WindowWidth;

				int sa = ((src[srcX] & 0xff000000) >> 24) * AA;			// 0xAArrggbb; AA 是透明度
				int sr = ((src[srcX] & 0xff0000) >> 16);				// 获取 RGB 里的 R
				int sg = ((src[srcX] & 0xff00) >> 8);					// G
				int sb = src[srcX] & 0xff;								// B

				// 设置对应的绘图区域像素信息
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}
void PutNumber(
	long x,
	long y,
	long number,
	int TextSize = 10,
	int TextProportion = 2,
	int TextInterval = 0,
	COLORREF tc = 0x000000,
	LPCTSTR font = "宋体"
) {


	int max;
	long int one_data;
	settextcolor(tc);
	setbkmode(TRANSPARENT);
	settextstyle(TextSize * TextProportion, TextSize, font);
	if (!TextInterval)
		TextInterval = TextSize;
	for (max = 0; max < 10; max++)
		if (number / pow(10, max) < 10)
			break;
	while (number >= 0) {

		if (max < 0 || x>getwidth())
			return;
		one_data = number / (long)pow(10, max);
		outtextxy(x, y, (char)(one_data + 48));
		number -= one_data * (long)pow(10, max);
		--max;
		x += TextInterval;
	}
}

bool on_magma(const int &h,const int &l) {
	for (int i = 1; i <= magma[h][0]; i++) {
		if (l == magma[h][i]) {
			return true;
		}
	}
	return false;
}

void must_put() {				//每一帧都要画的东西
	loadimage(NULL, _T(".\\素材\\图片\\背景\\background.png"), 1200, 700, true);
	//加载顶部图片框
	for (int i = 1; i <= 5; i++) {
		PutNumber(top_plant_x + plant_width *(i-1), top_plant_y + 30, sun_cost[i], 20, 2, 0, 0XFF00FF);
	}
	
	if (cool[1] <= 0 && sun>=sun_cost[1]) {
		putimage(top_plant_x, 0, &p1[1]);
	}
	else {
		putimage(top_plant_x, 0, &p1[0]);
		PutNumber(top_plant_x, top_plant_y, cool[1],20,2,0, 0X00FFFF);
		cool[1]--;
	}

	if (cool[2] <= 0 && sun >= sun_cost[2]) {
		putimage(top_plant_x + plant_width, 0, &p2[1]);
	}
	else {
		putimage(top_plant_x + plant_width, 0, &p2[0]);
		PutNumber(top_plant_x + plant_width, top_plant_y, cool[2], 20, 2, 0, 0X00FFFF);
		cool[2]--;
	}

	if (cool[3] <= 0 && sun >= sun_cost[3]) {
		putimage(top_plant_x + plant_width * 2, 0, &p3[1]);
	}
	else {
		putimage(top_plant_x + plant_width * 2, 0, &p3[0]);
		PutNumber(top_plant_x + plant_width * 2, top_plant_y, cool[3], 20, 2, 0, 0X00FFFF);
		cool[3]--;
	}

	if (cool[4] <= 0 && sun >= sun_cost[4]) {
		putimage(top_plant_x + plant_width * 3, 0, &p4[1]);
	}
	else {
		putimage(top_plant_x + plant_width * 3, 0, &p4[0]);
		PutNumber(top_plant_x + plant_width * 3, top_plant_y, cool[4], 20, 2, 0, 0X00FFFF);
		cool[4]--;
	}

	if (cool[5] <= 0 && sun >= sun_cost[5]) {
		putimage(top_plant_x + plant_width * 4, 0, &p7[1]);
	}
	else {
		putimage(top_plant_x + plant_width * 4, 0, &p7[0]);
		PutNumber(top_plant_x + plant_width * 4, top_plant_y, cool[5], 20, 2, 0, 0X00FFFF);
		cool[5]--;
	}

	/*
	putimage(top_plant_x + plant_width, 0, &p2[1]);
	putimage(top_plant_x + plant_width * 2, 0, &p3[1]);
	putimage(top_plant_x + plant_width * 3, 0, &p4[1]);
	*/
}


struct Mouse_State {
	bool is_choose;
	int choose;						//选择了什么
	/*
	1	向日葵
	2	豌豆射手
	3	棉花糖
	4	冰淇淋
	5	阳光
	*/
}mouse_state;
int choose_top(const int& x, const int& y) {			//
	//p
	if (x< top_plant_x || x > plant_width * 5 + top_plant_x || y > plant_height)	return 0;
	else {
		if (x < top_plant_x + plant_width)	return 1;
		else if (x < top_plant_x + plant_width * 2)	return 2;
		else if (x < top_plant_x + plant_width * 3)	return 3;
		else if (x < top_plant_x + plant_width * 4)	return 4;
		else return 5;
	}
}

void put_sun() {
	PutNumber(0, 0, sun, 50,2,0, 0x0000FF);
	return;
}




/***********************************************************************/
struct Bullet {
	int h, x;
	void draw() {
		put_plant(p6, x+move_x, HY[h]-move_y);
	}
	void move() {
		x += 20;
	}
};
std::vector<Bullet>bullet[8];
/*********************************************************************/
struct plant {
	int choose;
	/*
	1	向日葵
	2	豌豆射手
	3	棉花糖
	4	冰淇淋
	*/
	bool is_candy;				//有没有棉花糖
	int hp_candy;
	int h, l;
	int hp;
	int nxt_frame;				
	//初始化
	void init(const int &op) {
		if (op == 0) {		//清除上面的植物，不包括棉花糖
			choose = 0,hp = 0, nxt_frame = 0;
		}
		else {
			if (op == 3) {
				is_candy = true;
				hp_candy = plant_hp[3];
			}
			else if (op == 4) {
				cool[choose] = 0;
			}
			else {
				hp = plant_hp[op];
				nxt_frame = plant_nxt_frame[op];
				choose = op;
			}
		}
	}
	void make() {
		if (is_candy) {
			hp_candy--;
		}
		if (hp_candy <= 0 && is_candy) {
			is_candy = false;
			init(0);
		}
		if (choose == 0) {
			//init(0);
			return;
		}
		if (nxt_frame <= 0) {	//要执行 行为了
			nxt_frame = plant_nxt_frame[choose];
			switch (choose) {
			case 1:		//向日葵
				sun += 25;
				break;
			case 2:		//豌豆射手
				bullet[h].push_back(Bullet{ h,LX[l] });
				nxt_frame = plant_nxt_frame[choose];
				break;
			case 3:
				//is_candy = false;
				break;
			}
		}
		else {
			nxt_frame--;
		}
	}
	void draw_plant() {
		if (is_candy && l != 0 && h !=0 ) {
			if (hp_candy < plant_hp[3] / 4 * 3) {
				put_plant(p3, LX[l], HY[h], 2);
			}
			else {
				put_plant(p3, LX[l], HY[h]);
			}
		}
		if (choose == 0) {
			return;
		}
		if (hp <= 0) {
			init(0);
			return;
		}
		if (on_magma(h, l) == true && is_candy == false && choose!=0 &&choose!=3) {
			init(0);
			return;
		}
		if (choose == 1) {
			if (nxt_frame <= 0 ) {					//向日葵+太阳
				put_plant(p1, LX[l], HY[h]);
				put_plant(p5, LX[l], HY[h]);
			}
			else {
				put_plant(p1, LX[l], HY[h]);		//向日葵
			}
		}else if (choose == 2) {
			put_plant(p2, LX[l], HY[h]);			//豌豆射手
		}
		else if (choose == 5) {						//土司面包
			put_plant(p7, LX[l], HY[h]);
		}
		else {

		}
		return;
	}

}mp[8][10];

void draw_plant() {					//画植物的函数
	for (int i = 1; i <= 7; i++) {
		for (int j = 1; j <= 9; j++) {
			mp[i][j].draw_plant();
		}
	}
}
void plant_action() {				//植物行为
	for (int i = 1; i <= 7; i++) {
		for (int j = 1; j <= 9; j++) {
			mp[i][j].make();
		}
	}
}
/**********************************************************************************/
bool end = false;
struct zom {
	int type;
	/*
	1	普通	22
	2	塑料	22
	3	铁桶	14
	*/
	int hp;
	int h, x;	//行数	横坐标
	int frame = 0;
	void draw_zom() {
		if (x < 170) {
			end = true;
			return;
		}
		if (mp[h][xl(x + move_x)].choose == 0) {		//没有植物
			if (on_magma(h, xl(x + move_x)) && mp[h][xl(x + move_x)].is_candy == false) {
				x-= zombie_v*2;	//在没有棉花糖的岩浆上移动
			}
			else {
				x -= zombie_v;	//僵尸移动
			}
		}
		else {
			mp[h][xl(x + move_x)].hp -= hurt_zombie;
			//调试用
			//PutNumber(100, 100, mp[h][xl(x + move_x)].hp, 50);
		}
		switch (type) {

		case 1:
			//普通僵尸
			drawAlpha(q1 + frame, x, HY[h] - move_y, Zombies_width, Zombies_height, 0, 0);
			if (frame >= 21) {
				frame = 0;
			}
			else {
				frame++;
			}
			break;
		case 2:
			//塑料僵尸
			drawAlpha(q2 + frame, x, HY[h] - move_y, Zombies_width, Zombies_height, 0, 0);
			if (frame >= 20) {
				frame = 0;
			}
			else {
				frame++;
			}
			break;
		case 3:
			//铁桶僵尸
			drawAlpha(q3 + frame, x, HY[h] - move_y, Zombies_width, Zombies_height, 0, 0);
			if (frame >= 13) {
				frame = 0;
			}
			else {
				frame++;
			}
			break;
		}
	}
};
//存僵尸位置，血量的动态数组
std::vector<zom>zombie[10];			//按行数归类，方便写和子弹碰撞的函数
const int num_zom_wave = 25;
int zom_wave[num_zom_wave + 1][4] = { {0,},
	{0,1} ,//1
	{0,1} ,//2
	{0,2,2} ,//3
	{0,3,2} ,//4
	{0,3,2} ,//5
	{0,3,2} ,//6
	{0,4,2},//7
	{0,4,3} ,//8
	{0,5,4} ,//9
	{0,5.5} ,//10
	{0,5,5,1} ,//11
	{0,5,5,2} ,//12
	{0,5,5,2} ,//13
	{0,5,5,2} ,//14
	{0,5,5,2} ,//15
	{0,5,5,3} ,//16
	{0,6,5,3} ,//17
	{0,6,5,4},//18
	{0,6,5,5},//19
	{0,7,10,5},//20
	{0,8,10,5},//21
	{0,8,10,5},//22
	{0,8,10,5},//23
	{0,8,10,5},//24
	{0,8,10,5} };//25
int zom_wave_time[num_zom_wave+1] = {0,	500,	1000,	1500,	2000,	2500,
										2900,	3300,	3700,	4100,	4500,
										4900,	5200,	5500,	5800,	6100,
										6400,	6700,	7000,	7300,	7600,
										7900,	8200,	8500,	8800,	9100};
int cur_wave = 1;
int after_basic = 280;

//随机生成僵尸的函数
//随机生成生成一个僵尸并存到zombie动态数组中
/*
op=0	随机生成
op=1	生成普通僵尸
op=2	生成塑料僵尸
op=3	生成铁桶僵尸

cnt： 生成数量
*/
void make_zom(int op=0,int cnt=1) {
	if (cnt == 0)	return;
	if (op == 0) {
		/*
		unsigned seed;
		seed = time(0);
		srand(seed);
		*/
		while (cnt--) {
			zom new_zom;
			new_zom.frame = 0;
			new_zom.x = zombie_init_x;
			//new_zom.h = (rand() % 7) + 1;
			new_zom.h = rand_number(1, 7);
			new_zom.type = rand_number(1, 3);
			new_zom.hp = zombie_hp * new_zom.type;
			zombie[new_zom.h].push_back(new_zom);
		}
		return;
	}
	else {
		while (cnt--) {
			zom new_zom;
			new_zom.frame = 0;
			new_zom.x = zombie_init_x;
			new_zom.h = rand_number(1, 7);
			new_zom.type = op;
			new_zom.hp = zombie_hp * new_zom.type;
			zombie[new_zom.h].push_back(new_zom);
		}
	}
	
}

//画僵尸
void draw_zombie() {
	for (int i = 1; i <= 7; i++) {
		for (int j = 0; j < zombie[i].size(); j++) {
			if (zombie[i][j].hp <= 0) {
				zombie[i].erase(zombie[i].begin() + j);	//死了，删除
				continue;
			}
			else {
				zombie[i][j].draw_zom();
			}
		}
	}
}

/******************************************************************************************************************************************/

void bullet_draw() {		//画子弹
	for (int i = 1; i <= 7; i++) {
		for (int j = 0; j < bullet[i].size(); j++) {
			bullet[i][j].draw();
		}
	}
}
void bullet_move() {		//子弹移动
	for (int i = 1; i <= 7; i++) {
		for (int j = 0; j < bullet[i].size(); j++) {
			bullet[i][j].move();
		}
	}
}
void bullet_crash() {		//子弹碰撞检测,出界检测，碰撞岩浆检测
	for (int i = 1; i <= 7; i++) {
		for (int j = 0; j < bullet[i].size(); j++) {
			bool flag = false;
			for (int k = 0; k < zombie[i].size(); k++) {
				if (abs(bullet[i][j].x - zombie[i][k].x)<25) {	//撞僵尸检测
					zombie[i][k].hp -= hurt_plant;
					bullet[i].erase(bullet[i].begin() + j);
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}
			if (on_magma(i, xl(bullet[i][j].x)) && mp[i][xl(bullet[i][j].x)].is_candy == false) {									//岩浆检测
				bullet[i].erase(bullet[i].begin() + j);
				break;
			}
			else if (bullet[i][j].x > 1080) {
				bullet[i].erase(bullet[i].begin() + j);
				break;
			}
		}
	}
}
/****************************************************************************************************************************/

// 绘图函数，补充透明度 AA

void loadres();						//加载素材

void init() {
	sun = 75;
	for (int i = 1; i <= 6; i++) {
		cool[i] = cool_time[i];
	}
	end = false;
	seed = time(0);
	srand(seed);
	cur_wave = 1;
    initgraph(640, 480);    //初始化图像绘制参数
    //loadimage(NULL, _T(".\\素材\\图片\\背景\\background.png"), 1200, 700, true);
	loadres();				//加载素材
	//加载背景
	loadimage(NULL, _T(".\\素材\\图片\\背景\\background.png"), 1200, 700, true);
	//加载顶部图片框
	putimage(top_plant_x,0,&p1[1]);
	putimage(top_plant_x + plant_width, 0, &p2[1]);
	putimage(top_plant_x + plant_width * 2, 0, &p3[1]);
	putimage(top_plant_x + plant_width * 3, 0, &p4[1]);

	int xx = 100, yy = 100;
	
}

void mouse_action() {	//鼠标活动
	while (peekmessage(&msg)) {
		
		if (msg.lbutton) {
			//PutNumber(0, 0, 100000);
			//调试代码
			if (mouse_state.is_choose) {	//已经选了植物
				int l = xl(msg.x);
				int h = yh(msg.y);
				if (l == 0 || h == 0 || (mouse_state.choose == 3 && !on_magma(h,l))) {		//选在了无效位置上
					return;
				}
				mp[h][l].h = h;
				mp[h][l].l = l;
				switch (mouse_state.choose) {
					
				case 1://向日葵
					mp[h][l].choose = 1;
					mp[h][l].init(mouse_state.choose);
					break;
				case 2://豌豆射手
					mp[h][l].choose = 2;
					mp[h][l].init(mouse_state.choose);
					break;
				case 3://棉花糖
					if (!on_magma(h, l)) {
						return;
					}
					mp[h][l].init(mouse_state.choose);
					break;
				case 4://冰淇凌
					//mp[h][l].choose = 4;
					mp[h][l].init(mouse_state.choose);
					break;
				case 5://吐司面包
					mp[h][l].init(mouse_state.choose);
					break;
				}
				//鼠标状态初始化
				mouse_state.is_choose = false;
				mouse_state.choose = 0;
			}
			else {							//没选植物
				mouse_state.choose = choose_top(msg.x, msg.y);
				if (mouse_state.choose && sun>=sun_cost[ mouse_state.choose ] && cool[mouse_state.choose] <= 0) {
					//选中植物了
					cool[mouse_state.choose] = cool_time[mouse_state.choose];	//重置冷却时间
					sun -= sun_cost[mouse_state.choose];						//花费阳光
					mouse_state.is_choose = true;								//标记为选中
				}
			}
		}
	}
	//调试用
	//PutNumber(0, 0, msg.x);
	//PutNumber(100, 0, msg.y);
	//PutNumber(0, 200, yh(msg.y));
	//PutNumber(100, 200, xl(msg.x));
	//PutNumber(0, 300, HY[yh(msg.y)]);
	//PutNumber(100, 300, LX[xl(msg.x)]);
	return;
}



void game() {
	/*
	zom new_zom;
	new_zom.frame = 0;
	new_zom.x = zombie_init_x;
	new_zom.h = 1;
	new_zom.type = 3;
	new_zom.hp = zombie_hp;
	zombie.push_back(new_zom);
	*/
	BeginBatchDraw();
	unsigned long long int step = 0;	//计步器 
	while (true) {
		music();
		Sleep(30);

		must_put();
		
		//putimage(0, 0, p7 + 0);
		mouse_action();		//鼠标活动

		if (cur_wave <= num_zom_wave) {
			if (step == zom_wave_time[cur_wave]) {	//生成僵尸
				make_zom(1, zom_wave[cur_wave][1]);
				make_zom(2, zom_wave[cur_wave][2]);
				make_zom(3, zom_wave[cur_wave][3]);
				(cur_wave)++;
			}
			else {
				step++;
			}
		}
		else {
			if (step % after_basic == 0) {
				make_zom(1, zom_wave[num_zom_wave][1]);
				make_zom(2, zom_wave[num_zom_wave][2]);
				make_zom(3, zom_wave[num_zom_wave][3]);
				step = 0;
				after_basic -= 10;
				cur_wave++;
			}
			else {
				step++;
			}
		}
		

		PutNumber(100, 100, cur_wave, 50, 2, 0, 0xFF1493);
		if (step % 250 == 0) {
			sun += 25;
		}
		put_sun();

		draw_plant();	//画植物
		plant_action();

		bullet_crash();
		bullet_move();
		bullet_draw();

		draw_zombie();	//画僵尸
		FlushBatchDraw();
		if (end == true) {
			return;
		}
		cleardevice();
		
	}
	EndBatchDraw();

}
/*******************************************************************/
//音乐相关
const std::string path = ".\\素材\\音乐\\";	//相对目录
const int music_num = 15;
std::string music_name[100];
//C++ stl大法吼！！！！！！！！！！！！！！！！！！
//这辈子不再用char[]
std::string is_play_name;
bool is_play;
char music_length[100];	//总长
char cur[100];			//播放位置	

std::string cur_music;		//每次查询播放位置用的参数
//windows提供的脑残接口，拿字符串当参数是真**
std::string status = "status ";
std::string position = " position";
std::string length = " length";

std::string num_to_char(int num) {		
	int k = (int)log10(num)+1;
	std::string ret;
	for (int i = k; i >= 1; i--) {
		char p = (num / (int)pow(10, i - 1)) % 10 + '0';
		ret += p;
	}
	return ret;
}
unsigned long long int char_to_num(std::string s) {
	unsigned long long int ret = 0;
	for (int i = s.length() - 1; i >= 0; i--) {
		ret *= 10;
		ret += s[i] - '0';
	}
	return ret;
}
void load_music() {				//加载music_name数组
	for (int i = 1; i <= music_num; i++) {
		music_name[i]=num_to_char(i);
		music_name[i] += ".mp3";
	}
	
	return;
}
bool cmp(char *x, char *y) {
	int len1 = strlen(x), len2 = strlen(y);
	if (strlen(x) != strlen(y)) {
		return false;
	}
	int len = strlen(x);
	for (int i = 0; i < strlen(x); i++) {
		if (x[i] != y[i]) {
			return false;
		}
	}
	return true;
}

void play_music(std::string music_name) {
	is_play_name = music_name;	//目前播放的歌曲名字
	is_play = true;	//标记为播放状态
	cur_music = status;
	cur_music += path;
	cur_music += music_name;
	//cur_music += length;
	//mciSendString(&s[0], &music_length[0], 100, NULL);	//歌曲总长度
	std::string s = cur_music + length;
	mciSendString(&s[0], &music_length[0], 100, NULL);
	//std::cout << music_length[0];
	cur_music += position;
	
	std::string open = "open ";
	std::string play = "play ";
	//strcat(open, path);
	//strcat(open, music_name);
	open += path;
	open += music_name;
	//strcat(play, path);
	//strcat(play, music_name);
	play += path;
	play += music_name;
	mciSendString(&open[0], NULL, 0, NULL);
	mciSendString(&play[0], NULL, 0, NULL);
}

void music() {
	//load_music();
	//放在loadres函数里面
	if (is_play == false) {
		play_music(music_name[rand_number(1, music_num)]);
		//play_music(music_name[3]);
	}
	else {
		mciSendString(&cur_music[0], &cur[0], 100, NULL);
		if (cmp(music_length,cur)) {
			is_play = false;
		}
	}
}


int main()
{
	init();
	game();
    while (1) {
    }
}

void loadres() {
	load_music();		//加载音乐

	loadimage(p1 + 1, _T(".\\素材\\图片\\植物\\向日葵1.jpg"), plant_width, plant_height, false);     //先黑底彩图
	loadimage(p1 + 0, _T(".\\素材\\图片\\植物\\向日葵0.jpg"), plant_width, plant_height, false);      //再白底黑图

	loadimage(p2 + 1, _T(".\\素材\\图片\\植物\\豌豆射手1.jpg"), plant_width, plant_height, false);     //先黑底彩图
	loadimage(p2 + 0, _T(".\\素材\\图片\\植物\\豌豆射手0.jpg"), plant_width, plant_height, false);      //再白底黑图

	loadimage(p3 + 1, _T(".\\素材\\图片\\植物\\棉花糖1.jpg"), plant_width, plant_height, false);     //先黑底彩图
	loadimage(p3 + 2, _T(".\\素材\\图片\\植物\\棉花糖2.jpg"), plant_width, plant_height, false);
	loadimage(p3 + 0, _T(".\\素材\\图片\\植物\\棉花糖0.jpg"), plant_width, plant_height, false);      //再白底黑图
	
	loadimage(p4 + 1, _T(".\\素材\\图片\\植物\\冰淇淋1.jpg"), plant_width, plant_height, false);     //先黑底彩图
	loadimage(p4 + 0, _T(".\\素材\\图片\\植物\\冰淇淋0.jpg"), plant_width, plant_height, false);      //再白底黑图
	

	loadimage(p5 + 1, _T(".\\素材\\图片\\植物\\阳光1.jpg"), plant_width, plant_height, false);     //先黑底彩图
	loadimage(p5 + 0, _T(".\\素材\\图片\\植物\\阳光0.jpg"), plant_width, plant_height, false);      //再白底黑图

	loadimage(p6 + 1, _T(".\\素材\\图片\\植物\\豌豆1.jpg"), plant_width, plant_height, false);     //先黑底彩图
	loadimage(p6 + 0, _T(".\\素材\\图片\\植物\\豌豆0.jpg"), plant_width, plant_height, false);		//再白底黑图

	loadimage(p7 + 1, _T(".\\素材\\图片\\植物\\吐司面包1.jpg"), plant_width, plant_height, false);     //先黑底彩图
	loadimage(p7 + 0, _T(".\\素材\\图片\\植物\\土司面包0.jpg"), plant_width, plant_height, false);


	loadimage(q1 + 0, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (1).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 1, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (2).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 2, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (3).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 3, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (4).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 4, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (5).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 5, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (6).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 6, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (7).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 7, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (8).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 8, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (9).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 9, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (10).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 10, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (11).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 11, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (12).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 12, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (13).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 13, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (14).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 14, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (15).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 15, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (16).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 16, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (17).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 17, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (18).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 18, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (19).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 19, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (20).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 20, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (21).gif"), Zombies_width, Zombies_height, false);
	loadimage(q1 + 21, _T(".\\素材\\图片\\动物\\普通僵尸\\普通僵尸 (22).gif"), Zombies_width, Zombies_height, false);

	loadimage(q2 + 0, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (1).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q2 + 1, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (2).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 2, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (3).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 3, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (4).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 4, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (5).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 5, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (6).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 6, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (7).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 7, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (8).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 8, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (9).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 9, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (10).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 10, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (11).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 11, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (12).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 12, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (13).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 13, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (14).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 14, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (15).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 15, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (16).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 16, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (17).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 17, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (18).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 18, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (19).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 19, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (20).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 20, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (21).gif"), Zombies_width, Zombies_height, false);
	loadimage(q2 + 21, _T(".\\素材\\图片\\动物\\塑料僵尸\\塑料僵尸 (22).gif"), Zombies_width, Zombies_height, false);

	loadimage(q3 + 0, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (1).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 1, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (2).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 2, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (3).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 3, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (4).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 4, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (5).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 5, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (6).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 6, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (7).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 7, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (8).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 8, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (9).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 9, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (10).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 10, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (11).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 11, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (12).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 12, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (13).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
	loadimage(q3 + 13, _T(".\\素材\\图片\\动物\\铁桶僵尸\\铁桶僵尸 (14).gif"), Zombies_width, Zombies_height, false);      //再白底黑图
}