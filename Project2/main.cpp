#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "Header.h"

//ZMIENNE GLOBALNE
const int WIDTH = 1200;
const int HEIGHT = 600;
const int NUM_OBST = 25;
const int NUM_PNT = 3;
const int NUM_PNTM = 2;
const int NUM_PNTL = 1;
const int NUM_LIV = 1;
const int NUM_HIT = 25;
int distance = 0;
int i = 0;
int j = 4;
int move = -1;

enum OKNA{START, MENUP, MENUH, MENUG, MENUQ, HIGHSCORE, GUIDE, PAUZA, RESTART, GRA, DEATH};
enum keys {UP, DOWN, LEFT, RIGHT, SPACE, ESCAPE, Y, N};
bool keys[6] = { false,false,false,false,false,false };

SuperSheep sheep;
Tlo SBG;
Tlo MG;
Tlo FG;
Tlo BG;
Tlo MNU;
Tlo MNU1;
Obstacle obstacles[NUM_OBST];
SheepHit hits[NUM_HIT];
Live lives[NUM_LIV];
Point points[NUM_PNT];
PointM pointsm[NUM_PNTM];
PointL pointsl[NUM_PNTL];

ALLEGRO_SAMPLE *kolizja = NULL;
ALLEGRO_SAMPLE *pointsup = NULL;
ALLEGRO_SAMPLE *limit = NULL;
ALLEGRO_SAMPLE *lose = NULL;
ALLEGRO_SAMPLE_INSTANCE *limits = NULL;

void InitSheep(SuperSheep &sheep, ALLEGRO_BITMAP *image);
void DrawSheep(SuperSheep &sheep);
void ResetAnimacji(SuperSheep &sheep, int position);
void MovementUp(SuperSheep &sheep);
void MovementDown(SuperSheep &sheep);
void MovementLeft(SuperSheep &sheep);
void MovementRight(SuperSheep &sheep);

void InitObstacles(Obstacle obstacles[], int size, ALLEGRO_BITMAP *image);
void DrawObstacles(Obstacle obstacles[], int size);
void StartObstacles(Obstacle obstacles[], int size);
void UptadeObstacles(Obstacle obstacles[], int size);
void ColideObstacles(Obstacle obstacles[], int bsize, SuperSheep &sheep, SheepHit hits[], int hSize);

void InitHitS(SheepHit hits[], int size, ALLEGRO_BITMAP *image);
void DrawHitS(SheepHit hits[], int size);
void StartHitS(SheepHit hits[], int size, int x, int y);
void UptadeHitS(SheepHit hits[], int size);

void InitLives(Live lives[], int size, ALLEGRO_BITMAP *image);
void DrawLives(Live lives[], int size);
void StartLives(Live lives[], int size);
void UptadeLives(Live lives[], int size);
void ColideLives(Live lives[], int bsize, SuperSheep &sheep);

void InitPoints(Point points[], int size, ALLEGRO_BITMAP *image);
void DrawPoints(Point points[], int size);
void StartPoints(Point points[], int size);
void UptadePoints(Point points[], int size);
void ColidePoints(Point points[], int bsize, SuperSheep &sheep);

void InitPointsM(PointM pointsm[], int size, ALLEGRO_BITMAP *image);
void DrawPointsM(PointM pointsm[], int size);
void StartPointsM(PointM pointsm[], int size);
void UptadePointsM(PointM pointsm[], int size);
void ColidePointsM(PointM pointsm[], int bsize, SuperSheep &sheep);

void InitPointsL(PointL pointsl[], int size, ALLEGRO_BITMAP *image);
void DrawPointsL(PointL pointsl[], int size);
void StartPointsL(PointL pointsl[], int size);
void UptadePointsL(PointL pointsl[], int size);
void ColidePointsL(PointL pointsl[], int bsize, SuperSheep &sheep);

void InitTlo(Tlo &back, float x, float y, float velX, float velY, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image);
void DrawTlo(Tlo &back);
void UptadeTlo(Tlo &back);

void ZmianaOkna(int &okno, int noweOkno);

int main(void)
{

	bool done = false;
	bool redraw = true;
	const int FPS = 60;
	int okno = -1;

	//Allegro
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font1 = NULL;
	ALLEGRO_FONT *font2 = NULL;
	ALLEGRO_FONT *font3 = NULL;
	ALLEGRO_BITMAP *SheepImage;
	ALLEGRO_BITMAP *HitSImage;
	ALLEGRO_BITMAP *PointsImage;
	ALLEGRO_BITMAP *PointsImageM;
	ALLEGRO_BITMAP *PointsImageL;
	ALLEGRO_BITMAP *ObstaclesImage;
	ALLEGRO_BITMAP *LiveImage;
	ALLEGRO_BITMAP *bgImage;
	ALLEGRO_BITMAP *mgImage;
	ALLEGRO_BITMAP *fgImage;
	ALLEGRO_BITMAP *startImage;
	ALLEGRO_BITMAP *menuImage;
	ALLEGRO_BITMAP *menu1Image;

	if (!al_init())										
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			

	if (!display)										
		return -1;

	al_init_acodec_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_image_addon();

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	SheepImage = al_load_bitmap("lot.png");
	al_convert_mask_to_alpha(SheepImage, al_map_rgb(128, 128, 0));

	HitSImage = al_load_bitmap("hitbird.png");
	al_convert_mask_to_alpha(HitSImage, al_map_rgb(128, 128, 0));

	PointsImage = al_load_bitmap("pkt1.png");
	al_convert_mask_to_alpha(PointsImage, al_map_rgb(128, 128, 0));

	PointsImageM = al_load_bitmap("pkt2.png");
	al_convert_mask_to_alpha(PointsImageM, al_map_rgb(128, 128, 0));

	PointsImageL = al_load_bitmap("pkt3.png");
	al_convert_mask_to_alpha(PointsImageL, al_map_rgb(128, 128, 0));

	ObstaclesImage = al_load_bitmap("enemy.png");
	al_convert_mask_to_alpha(ObstaclesImage, al_map_rgb(128, 128, 0));

	LiveImage = al_load_bitmap("live.png");
	al_convert_mask_to_alpha(LiveImage, al_map_rgb(128, 128, 0));

	startImage = al_load_bitmap("start.png");

	bgImage = al_load_bitmap("BG.png");
	mgImage = al_load_bitmap("BG1.png");
	fgImage = al_load_bitmap("BG2.png");
	menuImage = al_load_bitmap("menu.png");
	menu1Image = al_load_bitmap("menu1.png");
	al_convert_mask_to_alpha(menu1Image, al_map_rgb(128, 128, 0));
	al_convert_mask_to_alpha(menuImage, al_map_rgb(128, 128, 0));
	al_convert_mask_to_alpha(mgImage, al_map_rgb(128, 128, 0));
	al_convert_mask_to_alpha(fgImage, al_map_rgb(128, 128, 0));

	al_reserve_samples(10);
	lose = al_load_sample("Fail.ogg");
	kolizja = al_load_sample("D'Oh.ogg");
	pointsup = al_load_sample("Woohooo.ogg");
	limit = al_load_sample("Push It To The Limit.ogg");

	limits = al_create_sample_instance(limit);
	al_set_sample_instance_playmode(limits, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(limits, al_get_default_mixer());

	srand(time(NULL));

	ZmianaOkna(okno, START);

	InitSheep(sheep, SheepImage);
	InitObstacles(obstacles, NUM_OBST, ObstaclesImage);
	InitHitS(hits, NUM_HIT, HitSImage);
	InitLives(lives, NUM_LIV, LiveImage);
	InitPoints(points, NUM_PNT,PointsImage);
	InitPointsM(pointsm, NUM_PNTM, PointsImageM);
	InitPointsL(pointsl, NUM_PNTL, PointsImageL);
	InitTlo(SBG, 0, 0, 0, 0, 1200, 600, move, 1, startImage);
	InitTlo(BG, 0, 0, 0, 0, 1200, 600, move, 1, bgImage);
	InitTlo(MG, 0, 0, 1, 0, 2400, 600, move, 1, mgImage);
	InitTlo(FG, 0, 0, 2, 0, 3600, 600, move, 1, fgImage);
	InitTlo(MNU, 0, 0, 0, 0, 1200, 600, move, 1, menuImage);
	InitTlo(MNU1, 0, 0, 0, 0, 1200, 600, move, 1, menu1Image);


	font1 = al_load_font("lowrider.ttf", 56, 0);
	font2 = al_load_font("lowrider.ttf", 32, 0);
	font3 = al_load_font("lowrider.ttf", 20, 0);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));


	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;


			if (okno == START)
			{
				UptadeTlo(SBG);
			}
			else if (okno == MENUP)
			{

				redraw = true;
				if (keys[UP])
					MovementUp(sheep);

				else if (keys[DOWN])
					MovementDown(sheep);
				else
					ResetAnimacji(sheep, 1);

				if (keys[LEFT])
					MovementLeft(sheep);

				else if (keys[RIGHT])
					MovementRight(sheep);
				else
					ResetAnimacji(sheep, 1);

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);
				UptadeTlo(MNU);
			}
			else if (okno == MENUH)
			{

				if (keys[UP])
					MovementUp(sheep);

				else if (keys[DOWN])
					MovementDown(sheep);
				else
					ResetAnimacji(sheep, 1);

				if (keys[LEFT])
					MovementLeft(sheep);

				else if (keys[RIGHT])
					MovementRight(sheep);
				else
					ResetAnimacji(sheep, 1);

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);
				UptadeTlo(MNU);
			}
			else if (okno == MENUG)
			{

				if (keys[UP])
					MovementUp(sheep);

				else if (keys[DOWN])
					MovementDown(sheep);
				else
					ResetAnimacji(sheep, 1);

				if (keys[LEFT])
					MovementLeft(sheep);

				else if (keys[RIGHT])
					MovementRight(sheep);
				else
					ResetAnimacji(sheep, 1);

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);
				UptadeTlo(MNU);
			}
			else if (okno == MENUQ)
			{

				if (keys[UP])
					MovementUp(sheep);

				else if (keys[DOWN])
					MovementDown(sheep);
				else
					ResetAnimacji(sheep, 1);

				if (keys[LEFT])
					MovementLeft(sheep);

				else if (keys[RIGHT])
					MovementRight(sheep);
				else
					ResetAnimacji(sheep, 1);

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);
				UptadeTlo(MNU);
			}
			else if (okno == GUIDE)
			{
				if (keys[UP])
					MovementUp(sheep);

				else if (keys[DOWN])
					MovementDown(sheep);
				else
					ResetAnimacji(sheep, 1);

				if (keys[LEFT])
					MovementLeft(sheep);

				else if (keys[RIGHT])
					MovementRight(sheep);
				else
					ResetAnimacji(sheep, 1);

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);
				UptadeTlo(MNU1);
			}
			else if (okno == HIGHSCORE)
			{

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);
				UptadeTlo(MNU);
			}
			else if (okno == GRA)
			{

				if (keys[UP])
					MovementUp(sheep);

				else if (keys[DOWN])
					MovementDown(sheep);
				else
					ResetAnimacji(sheep, 1);

				if (keys[LEFT])
				{
					MovementLeft(sheep);
				}

				else if (keys[RIGHT])
					MovementRight(sheep);
				else
					ResetAnimacji(sheep, 1);

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);


				StartObstacles(obstacles, NUM_OBST);
				UptadeObstacles(obstacles, NUM_OBST);
				ColideObstacles(obstacles, NUM_OBST, sheep, hits, NUM_HIT);


				UptadeHitS(hits,NUM_HIT);

				StartLives(lives, NUM_LIV);
				UptadeLives(lives, NUM_LIV);
				ColideLives(lives, NUM_LIV, sheep);


				StartPoints(points, NUM_PNT);
				UptadePoints(points, NUM_PNT);
				ColidePoints(points, NUM_PNT, sheep);

				StartPointsM(pointsm, NUM_PNTM);
				UptadePointsM(pointsm, NUM_PNTM);
				ColidePointsM(pointsm, NUM_PNTM, sheep);

				StartPointsL(pointsl, NUM_PNTL);
				UptadePointsL(pointsl, NUM_PNTL);
				ColidePointsL(pointsl, NUM_PNTL, sheep);

				distance++;

				if (sheep.lives <= 0)

					ZmianaOkna(okno, DEATH);
			}
			else if (okno == PAUZA)
			{ }
			else if (okno == RESTART)
			{


				if (keys[UP])
					MovementUp(sheep);

				else if (keys[DOWN])
					MovementDown(sheep);
				else
					ResetAnimacji(sheep, 1);

				if (keys[LEFT])
					MovementLeft(sheep);

				else if (keys[RIGHT])
					MovementRight(sheep);
				else
					ResetAnimacji(sheep, 1);

				UptadeTlo(BG);
				UptadeTlo(MG);
				UptadeTlo(FG);


				StartObstacles(obstacles, NUM_OBST);
				UptadeObstacles(obstacles, NUM_OBST);
				ColideObstacles(obstacles, NUM_OBST, sheep, hits, NUM_HIT);

				UptadeHitS(hits, NUM_HIT);

				StartLives(lives, NUM_LIV);
				UptadeLives(lives, NUM_LIV);
				ColideLives(lives, NUM_LIV, sheep);


				StartPoints(points, NUM_PNT);
				UptadePoints(points, NUM_PNT);
				ColidePoints(points, NUM_PNT, sheep);

				StartPointsM(pointsm, NUM_PNTM);
				UptadePointsM(pointsm, NUM_PNTM);
				ColidePointsM(pointsm, NUM_PNTM, sheep);

				StartPointsL(pointsl, NUM_PNTL);
				UptadePointsL(pointsl, NUM_PNTL);
				ColidePointsL(pointsl, NUM_PNTL, sheep);

				distance++;
				if (sheep.lives <= 0)

					ZmianaOkna(okno, DEATH);
			}
			else if (okno == DEATH)
			{ 
				ResetAnimacji(sheep, 2);
			}
			
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
					keys[ESCAPE] = false;
					if (okno == GRA)
						ZmianaOkna(okno, PAUZA);

					else if (okno == DEATH)
						ZmianaOkna(okno, MENUP);

					else if (okno == RESTART)
						ZmianaOkna(okno, PAUZA);

					else if (okno == HIGHSCORE)
						ZmianaOkna(okno, MENUH);

					else if (okno == GUIDE)
						ZmianaOkna(okno, MENUG);

					else if (okno == PAUZA)
						ZmianaOkna(okno, RESTART);
					
					else
						done = true;
					break;

			case ALLEGRO_KEY_Y:
				if (okno == PAUZA)
				{
					distance = 0;
					ZmianaOkna(okno, MENUP);
					break;
				}
			case ALLEGRO_KEY_N:
				if (okno == PAUZA)
					ZmianaOkna(okno, RESTART);
					break;
			case ALLEGRO_KEY_UP:
				if (okno == GRA)
					keys[UP] = true;

				else if (okno == RESTART)
						keys[UP] = true;

				else if (okno == MENUP)
					ZmianaOkna(okno, MENUQ);

				else if (okno == MENUQ)
					ZmianaOkna(okno, MENUG);

				else if (okno == MENUG)
					ZmianaOkna(okno, MENUH);

				else if (okno == MENUH)
					ZmianaOkna(okno, MENUP);

				else
					keys[UP] = false;
					break;

			case ALLEGRO_KEY_DOWN:
				if (okno == GRA)
					keys[DOWN] = true;

				else if (okno == RESTART)
					keys[DOWN] = true;

				else if (okno == MENUP)
					ZmianaOkna(okno, MENUH);

				else if (okno == MENUQ)
					ZmianaOkna(okno, MENUP);

				else if (okno == MENUH)
					ZmianaOkna(okno, MENUG);

				else if (okno == MENUG)
					ZmianaOkna(okno, MENUQ);
				else
					keys[DOWN] = false;
					break;

			case ALLEGRO_KEY_LEFT:
				if (okno == GRA)
					keys[LEFT] = true;

				else if (okno == RESTART)
					keys[LEFT] = true;
				else
					keys[LEFT] = false;
					break;

			case ALLEGRO_KEY_RIGHT:
				if (okno == GRA)
					keys[RIGHT] = true;

				else if(okno == RESTART)
					keys[RIGHT] = true;
				else
					keys[RIGHT] = false;
					break;

			case ALLEGRO_KEY_SPACE:
					keys[SPACE] = true;
					if (okno == START)
						ZmianaOkna(okno, MENUP);

					else if (okno == MENUP)
						ZmianaOkna(okno, GRA);

					else if (okno == MENUH)
						ZmianaOkna(okno, HIGHSCORE);

					else if (okno == MENUG)
						ZmianaOkna(okno, GUIDE);
					
					else if (okno == MENUQ)
						done = true;

					else if (okno == PAUZA)
						ZmianaOkna(okno, RESTART);

					else if (okno == DEATH)
					{
						distance = 0;
						ZmianaOkna(okno, GRA);
					}
					break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				keys[ESCAPE] = false;
				break;

			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;

			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		if (redraw && al_is_event_queue_empty(event_queue))

		{
			if (okno == START)
			{
				DrawTlo(SBG);
			}

			else if (okno == MENUP)
			{

				redraw = false;
				sheep.x = WIDTH / 3 + 60;
				sheep.y = HEIGHT / 4 + 20;
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawTlo(MNU);
				DrawSheep(sheep);
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "PLAY!");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 90, ALLEGRO_ALIGN_CENTRE, "HIGHSCORE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 180, ALLEGRO_ALIGN_CENTRE, "GUIDE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 270, ALLEGRO_ALIGN_CENTRE, "QUIT");
			}
			else if (okno == MENUH)
			{

				redraw = false;
				sheep.x = WIDTH / 3;
				sheep.y = HEIGHT / 4 + 110;
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawTlo(MNU);
				DrawSheep(sheep);
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "PLAY!");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 90, ALLEGRO_ALIGN_CENTRE, "HIGHSCORE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 180, ALLEGRO_ALIGN_CENTRE, "GUIDE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 270, ALLEGRO_ALIGN_CENTRE, "QUIT");
			}
			else if (okno == MENUQ)
			{

				redraw = false;
				sheep.x = WIDTH / 3 + 75;
				sheep.y = HEIGHT / 4 + 290;
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawTlo(MNU);
				DrawSheep(sheep);
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "PLAY!");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 90, ALLEGRO_ALIGN_CENTRE, "HIGHSCORE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 180, ALLEGRO_ALIGN_CENTRE, "GUIDE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 270, ALLEGRO_ALIGN_CENTRE, "QUIT");
			}
			else if (okno == MENUG)
			{

				redraw = false;
				sheep.x = WIDTH / 3 + 60;
				sheep.y = HEIGHT / 4 + 200;
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawTlo(MNU);
				DrawSheep(sheep);
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4, ALLEGRO_ALIGN_CENTRE, "PLAY!");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 90, ALLEGRO_ALIGN_CENTRE, "HIGHSCORE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 180, ALLEGRO_ALIGN_CENTRE, "GUIDE");
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 270, ALLEGRO_ALIGN_CENTRE, "QUIT");
			}
			else if (okno == GUIDE)
			{

				redraw = false;
				sheep.x = WIDTH / 3 + 10;
				sheep.y = HEIGHT / 4;

				obstacles[0].x = WIDTH / 3 + 10;
				obstacles[0].y = HEIGHT / 4 + 80;
				obstacles[0].live = true;

				lives[0].x = WIDTH / 3 + 10;
				lives[0].y = HEIGHT / 4 + 150;
				lives[0].live = true;

				points[0].x = WIDTH / 3 + 10;
				points[0].y = HEIGHT / 4 + 220;
				points[0].live = true;

				pointsm[0].x = WIDTH / 3 + 10;
				pointsm[0].y = HEIGHT / 4 + 290;
				pointsm[0].live = true;

				pointsl[0].x = WIDTH / 3 + 10;
				pointsl[0].y = HEIGHT / 4 + 360;
				pointsl[0].live = true;

				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawTlo(MNU1);
				DrawSheep(sheep);
				DrawObstacles(obstacles, 1);
				DrawLives(lives, 1);
				DrawPoints(points, 1);
				DrawPointsM(pointsm, 1);
				DrawPointsL(pointsl, 1);

				if (obstacles[0].live) {
					if (++obstacles[0].frameCount >= obstacles[0].frameDelay)
					{
						obstacles[0].curFrame += obstacles[0].animationDirection;
						if (obstacles[0].curFrame >= obstacles[0].maxFrame)
							obstacles[0].curFrame = 0;
						else if (obstacles[0].curFrame <= 0)
							obstacles[0].curFrame = obstacles[0].maxFrame - 1;
						obstacles[0].frameCount = 0;
					}
				}

				if (lives[0].live) {
					if (++lives[0].frameCount >= lives[0].frameDelay)
					{
						lives[0].curFrame += lives[0].animationDirection;
						if (lives[0].curFrame >= lives[0].maxFrame)
							lives[0].curFrame = 0;
						else if (lives[0].curFrame <= 0)
							lives[0].curFrame = lives[0].maxFrame - 1;
						lives[0].frameCount = 0;
					}
				}

				if (points[0].live) {
					if (++points[0].frameCount >= points[0].frameDelay)
					{
						points[0].curFrame += points[0].animationDirection;
						if (points[0].curFrame >= points[0].maxFrame)
							points[0].curFrame = 0;
						else if (points[0].curFrame <= 0)
							points[0].curFrame = points[0].maxFrame - 1;
						points[0].frameCount = 0;
					}
				}

				if (pointsm[0].live) {
					if (++pointsm[0].frameCount >= pointsm[0].frameDelay)
					{
						pointsm[0].curFrame += pointsm[0].animationDirection;
						if (pointsm[0].curFrame >= pointsm[0].maxFrame)
							pointsm[0].curFrame = 0;
						else if (pointsm[0].curFrame <= 0)
							pointsm[0].curFrame = pointsm[0].maxFrame - 1;
						pointsm[0].frameCount = 0;
					}
				}
				if (pointsl[0].live) {
					if (++pointsl[0].frameCount >= pointsl[0].frameDelay)
					{
						pointsl[0].curFrame += pointsl[0].animationDirection;
						if (pointsl[0].curFrame >= pointsl[0].maxFrame)
							pointsl[0].curFrame = 0;
						else if (obstacles[0].curFrame <= 0)
							pointsl[0].curFrame = pointsl[0].maxFrame - 1;
						pointsl[0].frameCount = 0;
					}
				}
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, 60, ALLEGRO_ALIGN_CENTRE, "GUIDE");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 - 10, ALLEGRO_ALIGN_CENTRE, "PLAYER");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 65, ALLEGRO_ALIGN_CENTRE, "ENEMY");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 140, ALLEGRO_ALIGN_CENTRE, "EKSTRA LIVE");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 210, ALLEGRO_ALIGN_CENTRE, "5 POINTS");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 280, ALLEGRO_ALIGN_CENTRE, "15 POINTS");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 4 + 350, ALLEGRO_ALIGN_CENTRE, "50 POINTS");
			}
			else if (okno == HIGHSCORE)
			{

				redraw = false;
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawTlo(MNU);
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, 110, ALLEGRO_ALIGN_CENTRE, "HIGHSCORE");
			}
			else if (okno == GRA)
			{
				redraw = false;
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawSheep(sheep);
				DrawHitS(hits, NUM_HIT);
				DrawObstacles(obstacles, NUM_OBST);
				DrawLives(lives, NUM_LIV);
				DrawPoints(points, NUM_PNT);
				DrawPointsM(pointsm, NUM_PNTM);
				DrawPointsL(pointsl, NUM_PNTL);

				al_draw_textf(font3, al_map_rgb(255, 0, 0), 800, 10, 0, "LIVES: %i  POINTS: %i  DISTANCE: %i", sheep.lives, sheep.score, distance);
			}
			else if (okno == PAUZA)
			{
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawSheep(sheep);
				DrawHitS(hits,NUM_HIT);
				DrawObstacles(obstacles, NUM_OBST);
				DrawLives(lives, NUM_LIV);
				DrawPoints(points, NUM_PNT);
				DrawPointsM(pointsm, NUM_PNTM);
				DrawPointsL(pointsl, NUM_PNTL);
				al_draw_textf(font1, al_map_rgb(255, 0, 0), WIDTH / 2, 80, ALLEGRO_ALIGN_CENTRE, "PAUSE");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 - 120, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU?");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 - 60, ALLEGRO_ALIGN_CENTRE, "(Y)ES/(N)O");
			}
			else if (okno == RESTART)
			{
				redraw = false;
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawSheep(sheep);
				DrawHitS(hits,NUM_HIT);
				DrawObstacles(obstacles, NUM_OBST);
				DrawLives(lives, NUM_LIV);
				DrawPoints(points, NUM_PNT);
				DrawPointsM(pointsm, NUM_PNTM);
				DrawPointsL(pointsl, NUM_PNTL);

				al_draw_textf(font3, al_map_rgb(255, 0, 0), 810, 10, 0, "LIVES: %i     POINTS: %i     DISTANCE: %i", sheep.lives, sheep.score, distance);
			}
			else if (okno == DEATH)
			{
				DrawTlo(BG);
				DrawTlo(MG);
				DrawTlo(FG);
				DrawTlo(MNU);
				DrawSheep(sheep);
				sheep.x = WIDTH / 2;
				sheep.y = 3 * HEIGHT / 4 ;
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 - 135, ALLEGRO_ALIGN_CENTRE, "!! GAME OVER !!");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 - 95, ALLEGRO_ALIGN_CENTRE, "Final score: %i", sheep.score);
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 - 55, ALLEGRO_ALIGN_CENTRE, "Distance: %im", distance);
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 + 25, ALLEGRO_ALIGN_CENTRE, "PRESS SPACEBAR TO START AGAIN");
				al_draw_textf(font2, al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT / 2 + 75, ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO BACK TO MENU");
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

		}
	}
	al_destroy_sample(kolizja);
	al_destroy_sample(pointsup);
	al_destroy_sample(limit);
	al_destroy_sample(lose);
	al_destroy_sample_instance(limits);
	al_destroy_bitmap(SheepImage);
	al_destroy_bitmap(HitSImage);
	al_destroy_bitmap(PointsImage);
	al_destroy_bitmap(PointsImageM);
	al_destroy_bitmap(PointsImageL);
	al_destroy_bitmap(ObstaclesImage);
	al_destroy_bitmap(LiveImage);
	al_destroy_bitmap(menuImage);
	al_destroy_bitmap(startImage);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(mgImage);
	al_destroy_bitmap(fgImage);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						

	return 0;
}
void ResetAnimacji(SuperSheep &sheep, int position)
{
	if (position == 1)
	{
		if (++sheep.frameCount >= sheep.frameDelay)
		{
			
			sheep.animationRow = i;
			i++;
			
			if (i > 2)
			{
				i = 0;
			}

			sheep.frameCount = 1;
		}
	}

	else if(position == 2)
	{
		if (sheep.animationRow < 7) 
		{
			if (++sheep.frameCount >= sheep.frameDelay)
			{

				sheep.animationRow = j;
				j++;

				if (j > 7)
				{
					j = 4;
				}
				sheep.frameCount = 1;
			}
		}
	}
}

void InitSheep(SuperSheep &sheep, ALLEGRO_BITMAP *image = NULL)
{
	sheep.x = 46;
	sheep.y = HEIGHT / 2;
	sheep.ID = PLAYER;
	sheep.lives = 3;
	sheep.speed = 6;
	sheep.boundx = 35;
	sheep.boundy = 23;
	sheep.score = 0;

	sheep.maxFrame = 8;
	sheep.curFrame = 0;
	sheep.frameCount = 1;
	sheep.frameDelay = 20;
	sheep.frameWidth = 91;
	sheep.frameHeight = 64;
	sheep.animationColumns = 1;
	sheep.animationDirection = 1;

	sheep.animationRow = 0;

	if (image != NULL)
	sheep.image = image;
}
void DrawSheep(SuperSheep &sheep)
{
	int fx = (sheep.curFrame % sheep.animationColumns) * sheep.frameWidth;
	int fy = sheep.animationRow * sheep.frameHeight;

	al_draw_bitmap_region(sheep.image, fx, fy, sheep.frameWidth, sheep.frameHeight, sheep.x - sheep.frameWidth / 2, sheep.y - sheep.frameHeight / 2 , 0);
	/*al_draw_filled_rectangle(sheep.x - sheep.boundx, sheep.y - sheep.boundy, sheep.x + sheep.boundx, sheep.y + sheep.boundy, al_map_rgba(255, 0, 255,100));*/
}
void MovementUp(SuperSheep &sheep)
{
	sheep.animationRow = 0;
	sheep.y -= sheep.speed;
	if (sheep.y < 40)
		sheep.y = 40;
}
void MovementDown(SuperSheep &sheep)
{
	sheep.animationRow = 1;
	sheep.y += sheep.speed;
	if (sheep.y > HEIGHT-30)
		sheep.y = HEIGHT-30;

}
void MovementLeft(SuperSheep &sheep)
{
	sheep.animationRow = 3;
	sheep.x -= sheep.speed + 1;
	if (sheep.x < 45)
		sheep.x = 45;

}
void MovementRight(SuperSheep &sheep)
{
	sheep.animationRow = 0;
	sheep.x += sheep.speed;
	if (sheep.x > WIDTH-50)
		sheep.x = WIDTH-50;

}


void InitObstacles(Obstacle obstacles[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		obstacles[i].ID = ENEMY;
		obstacles[i].live = false;
		obstacles[i].speed = 7;
		obstacles[i].boundx = 18;
		obstacles[i].boundy = 18;


		obstacles[i].maxFrame = 5;
		obstacles[i].curFrame = 0;
		obstacles[i].frameCount = 1;
		obstacles[i].frameDelay = 5;
		obstacles[i].frameWidth = 70;
		obstacles[i].frameHeight = 76;
		obstacles[i].animationColumns = 5;
		obstacles[i].animationDirection = 1;

		if (image != NULL)
		obstacles[i].image = image;
		
	}
}
void DrawObstacles(Obstacle obstacles[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (obstacles[i].live)
		{
			int fx = (obstacles[i].curFrame % obstacles[i].animationColumns) * obstacles[i].frameWidth;
			int fy = (obstacles[i].curFrame / obstacles[i].animationColumns) * obstacles[i].frameHeight;

			al_draw_bitmap_region(obstacles[i].image, fx, fy, obstacles[i].frameWidth, obstacles[i].frameHeight, obstacles[i].x - obstacles[i].frameWidth / 2, obstacles[i].y - obstacles[i].frameHeight / 2, 0);
			/*al_draw_filled_rectangle(obstacles[i].x - obstacles[i].boundx, obstacles[i].y - obstacles[i].boundy, obstacles[i].x + obstacles[i].boundx, obstacles[i].y + obstacles[i].boundy, al_map_rgba(255, 0, 255, 100));*/
		}

	}
}
void StartObstacles(Obstacle obstacles[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!obstacles[i].live)
		{
			if (rand() % 500 == 0)
			{
				obstacles[i].live = true;
				obstacles[i].x = WIDTH;
				obstacles[i].y = 30 + rand() % (HEIGHT - 60);
				
				break;
			}
		}
	}
}
void UptadeObstacles(Obstacle obstacles[], int size)
{
	for (int i = 0;i < size;i++)
	{
		if (distance % 2000 == 0)
			obstacles[i].speed = obstacles[i].speed + 1;
		if (obstacles[i].live)
		{
			if (++obstacles[i].frameCount >= obstacles[i].frameDelay)
			{
				obstacles[i].curFrame += obstacles[i].animationDirection;
				if (obstacles[i].curFrame >= obstacles[i].maxFrame)
					obstacles[i].curFrame = 0;
				else if (obstacles[i].curFrame <= 0)
					obstacles[i].curFrame = obstacles[i].maxFrame - 1;
				obstacles[i].frameCount = 0;
			}
			obstacles[i].x -= obstacles[i].speed;
			if (obstacles[i].x < 0)
				obstacles[i].live = false;
		}
	}
}
void ColideObstacles(Obstacle obstacles[], int bsize, SuperSheep &sheep, SheepHit hits[], int hSize)
{
	for (int i = 0; i < bsize; i++)
	{
		if (obstacles[i].live)
		{
			if (obstacles[i].x - obstacles[i].boundx < sheep.x + sheep.boundx &&
				obstacles[i].x + obstacles[i].boundx > sheep.x - sheep.boundx &&
				obstacles[i].y - obstacles[i].boundy < sheep.y + sheep.boundy &&
				obstacles[i].y + obstacles[i].boundy > sheep.y - sheep.boundy)
			{
				sheep.lives--;
				obstacles[i].live = false;

				StartHitS(hits, hSize, obstacles[i].x, obstacles[i].y);

				al_play_sample(kolizja, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}

		}
	}
}

void InitHitS(SheepHit hits[],int size, ALLEGRO_BITMAP *image)
{
	for (int i = 0;i < size;i ++)
	{
		hits[i].live = false;

		hits[i].maxFrame = 3;
		hits[i].curFrame = 0;
		hits[i].frameCount = 0;
		hits[i].frameDelay = 4;
		hits[i].frameWidth = 57;
		hits[i].frameHeight = 54;
		hits[i].animationColumns = 3;
		hits[i].animationDirection = 1;

		hits[i].image = image;
	}
};
void DrawHitS(SheepHit hits[], int size)
{
	for (int i = 0;i < size;i++)
	{
		if (hits[i].live)
		{
			int fx = (hits[i].curFrame % hits[i].animationColumns) * hits[i].frameWidth;
			int fy = (hits[i].curFrame / hits[i].animationColumns) * hits[i].frameHeight;

			al_draw_bitmap_region(hits[i].image, fx, fy, hits[i].frameWidth, hits[i].frameHeight, hits[i].x - hits[i].frameWidth / 2, hits[i].y - hits[i].frameHeight / 2, 0);
		}
	}
};
void StartHitS(SheepHit hits[], int size, int x, int y)
{
	for (int i = 0; i < size; i++)

	{
		if (!hits[i].live)
		{
			hits[i].live = true;
			hits[i].x = x;
			hits[i].y = y;
			break;
		}
	}
};
void UptadeHitS(SheepHit hits[],int size)
{
	for (int i = 0;i < size;i++)
	{
		if (hits[i].live)
		{
			hits[i].x -= 7;
			hits[i].y += 4;
			if (++hits[i].frameCount >= hits[i].frameDelay)
			{
				hits[i].curFrame += hits[i].animationDirection;
				if (hits[i].curFrame >= hits[i].maxFrame)
				{
					hits[i].curFrame = 0;
					hits[i].live = false;
				}
				hits[i].frameCount = 0;
			}
		}
	}
};

void InitLives(Live lives[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		lives[i].ID = LIVES;
		lives[i].live = false;
		lives[i].speed = 7;
		lives[i].boundx = 18;
		lives[i].boundy = 18;


		lives[i].maxFrame = 4;
		lives[i].curFrame = 0;
		lives[i].frameCount = 1;
		lives[i].frameDelay = 8;
		lives[i].frameWidth = 54;
		lives[i].frameHeight = 45;
		lives[i].animationColumns = 4;
		lives[i].animationDirection = 1;

		if (image != NULL)
		lives[i].image = image;


	}
}
void DrawLives(Live lives[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (lives[i].live)
		{
			int fx = (lives[i].curFrame % lives[i].animationColumns) * lives[i].frameWidth;
			int fy = (lives[i].curFrame / lives[i].animationColumns) * lives[i].frameHeight;

			al_draw_bitmap_region(lives[i].image, fx, fy, lives[i].frameWidth, lives[i].frameHeight, lives[i].x - lives[i].frameWidth / 2, lives[i].y - lives[i].frameHeight / 2, 0);
			/*al_draw_filled_rectangle(lives[i].x - lives[i].boundx, lives[i].y - lives[i].boundy, lives[i].x + lives[i].boundx, lives[i].y + lives[i].boundy, al_map_rgba(255, 0, 255, 100));*/
		}

	}
}
void StartLives(Live lives[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!lives[i].live)
		{
			if (rand() % 2000 == 0)
			{
				lives[i].live = true;
				lives[i].x = WIDTH;
				lives[i].y = 30 + rand() % (HEIGHT - 60);

				break;
			}
		}
	}
}
void UptadeLives(Live lives[], int size)
{
	for (int i = 0;i < size;i++)
	{
		if (lives[i].live)
		{
			if (++lives[i].frameCount >= lives[i].frameDelay)
			{
				lives[i].curFrame += lives[i].animationDirection;
				if (lives[i].curFrame >= lives[i].maxFrame)
					lives[i].curFrame = 0;
				else if (lives[i].curFrame <= 0)
					lives[i].curFrame = lives[i].maxFrame - 1;
				lives[i].frameCount = 0;
			}
			lives[i].x -= lives[i].speed;

			if (lives[i].x < 0)
				lives[i].live = false;
		}
	}
}
void ColideLives(Live lives[], int bsize, SuperSheep &sheep)
{
	for (int i = 0; i < bsize; i++)
	{
		if (lives[i].live)
		{
			if (lives[i].x - lives[i].boundx < sheep.x + sheep.boundx &&
				lives[i].x + lives[i].boundx > sheep.x - sheep.boundx &&
				lives[i].y - lives[i].boundy < sheep.y + sheep.boundy &&
				lives[i].y + lives[i].boundy > sheep.y - sheep.boundy)
			{
				sheep.lives++;
				lives[i].live = false;
				al_play_sample(pointsup, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}
	}
}

void InitPoints(Point points[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		points[i].ID = POINTS;
		points[i].live = false;
		points[i].speed = 5;
		points[i].boundx = 20;
		points[i].boundy = 20;

		points[i].maxFrame = 4;
		points[i].curFrame = 0;
		points[i].frameCount = 1;
		points[i].frameDelay = 8;
		points[i].frameWidth = 50;
		points[i].frameHeight = 48;
		points[i].animationColumns = 4;
		points[i].animationDirection = 1;

		if (image != NULL)
		points[i].image = image;

	}
}
void DrawPoints(Point points[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (points[i].live)
		{
			int fx = (points[i].curFrame % points[i].animationColumns) * points[i].frameWidth;
			int fy = (points[i].curFrame / points[i].animationColumns) * points[i].frameHeight;

			al_draw_bitmap_region(points[i].image, fx, fy, points[i].frameWidth, points[i].frameHeight, points[i].x - points[i].frameWidth / 2, points[i].y - points[i].frameHeight / 2, 0);
			/*al_draw_filled_rectangle(points[i].x - points[i].boundx, points[i].y - points[i].boundy, points[i].x + points[i].boundx, points[i].y + points[i].boundy, al_map_rgba(255, 0, 255,100));*/
		}

	}
}
void StartPoints(Point points[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!points[i].live)
		{
			if (rand() % 500 == 0)
			{
				points[i].live = true;
				points[i].x = WIDTH;
				points[i].y = 30 + rand() % (HEIGHT - 60);

				break;
			}
		}
	}
}
void UptadePoints(Point points[], int size)
{
	for (int i = 0;i < size;i++)
	{
		if (points[i].live)
		{
			if (++points[i].frameCount >= points[i].frameDelay)
			{
				points[i].curFrame += points[i].animationDirection;
				if (points[i].curFrame >= points[i].maxFrame)
					points[i].curFrame = 0;
				else if (points[i].curFrame <= 0)
					points[i].curFrame = points[i].maxFrame - 1;
				points[i].frameCount = 0;
			}
			points[i].x -= points[i].speed;

			if (points[i].x < 0)
				points[i].live = false;
		}
	}
}
void ColidePoints(Point points[], int bsize, SuperSheep &sheep)
{
	for (int i = 0; i < bsize; i++)
	{
		if (points[i].live)
		{
			if (points[i].x - points[i].boundx < sheep.x + sheep.boundx &&
				points[i].x + points[i].boundx > sheep.x - sheep.boundx &&
				points[i].y - points[i].boundy < sheep.y + sheep.boundy &&
				points[i].y + points[i].boundy > sheep.y - sheep.boundy)
			{
				sheep.score=sheep.score+5;
				points[i].live = false;
				al_play_sample(pointsup, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}
	}
}

void InitPointsM(PointM pointsm[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		pointsm[i].ID = POINTSM;
		pointsm[i].live = false;
		pointsm[i].speed = 6;
		pointsm[i].boundx = 20;
		pointsm[i].boundy = 20;

		pointsm[i].maxFrame = 4;
		pointsm[i].curFrame = 0;
		pointsm[i].frameCount = 1;
		pointsm[i].frameDelay = 8;
		pointsm[i].frameWidth = 50;
		pointsm[i].frameHeight = 48;
		pointsm[i].animationColumns = 4;
		pointsm[i].animationDirection = 1;

		if (image != NULL)
		pointsm[i].image = image;

	}
}
void DrawPointsM(PointM pointsm[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (pointsm[i].live)
		{
			int fx = (pointsm[i].curFrame % pointsm[i].animationColumns) * pointsm[i].frameWidth;
			int fy = (pointsm[i].curFrame / pointsm[i].animationColumns) * pointsm[i].frameHeight;

			al_draw_bitmap_region(pointsm[i].image, fx, fy, pointsm[i].frameWidth, pointsm[i].frameHeight, pointsm[i].x - pointsm[i].frameWidth / 2, pointsm[i].y - pointsm[i].frameHeight / 2, 0);
			/*al_draw_filled_rectangle(pointsm[i].x - pointsm[i].boundx, pointsm[i].y - pointsm[i].boundy, pointsm[i].x + pointsm[i].boundx, pointsm[i].y + pointsm[i].boundy, al_map_rgba(255, 0, 255,100));*/
		}

	}
}
void StartPointsM(PointM pointsm[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!pointsm[i].live)
		{
			if (rand() % 1000 == 0)
			{
				pointsm[i].live = true;
				pointsm[i].x = WIDTH;
				pointsm[i].y = 30 + rand() % (HEIGHT - 60);

				break;
			}
		}
	}
}
void UptadePointsM(PointM pointsm[], int size)
{
	for (int i = 0;i < size;i++)
	{
		if (pointsm[i].live)
		{
			if (++pointsm[i].frameCount >= pointsm[i].frameDelay)
			{
				pointsm[i].curFrame += pointsm[i].animationDirection;
				if (pointsm[i].curFrame >= pointsm[i].maxFrame)
					pointsm[i].curFrame = 0;
				else if (pointsm[i].curFrame <= 0)
					pointsm[i].curFrame = pointsm[i].maxFrame - 1;
				pointsm[i].frameCount = 0;
			}
			pointsm[i].x -= pointsm[i].speed;

			if (pointsm[i].x < 0)
				pointsm[i].live = false;
		}
	}
}
void ColidePointsM(PointM pointsm[], int bsize, SuperSheep &sheep)
{
	for (int i = 0; i < bsize; i++)
	{
		if (pointsm[i].live)
		{
			if (pointsm[i].x - pointsm[i].boundx < sheep.x + sheep.boundx &&
				pointsm[i].x + pointsm[i].boundx > sheep.x - sheep.boundx &&
				pointsm[i].y - pointsm[i].boundy < sheep.y + sheep.boundy &&
				pointsm[i].y + pointsm[i].boundy > sheep.y - sheep.boundy)
			{
				sheep.score = sheep.score + 15;
				pointsm[i].live = false;
				al_play_sample(pointsup, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}
	}
}

void InitPointsL(PointL pointsl[], int size, ALLEGRO_BITMAP *image = NULL)
{
	for (int i = 0; i < size; i++)
	{
		pointsl[i].ID = POINTSL;
		pointsl[i].live = false;
		pointsl[i].speed = 7;
		pointsl[i].boundx = 22;
		pointsl[i].boundy = 27;

		pointsl[i].maxFrame = 4;
		pointsl[i].curFrame = 0;
		pointsl[i].frameCount = 1;
		pointsl[i].frameDelay = 8;
		pointsl[i].frameWidth = 64;
		pointsl[i].frameHeight = 62;
		pointsl[i].animationColumns = 4;
		pointsl[i].animationDirection = 1;

		if (image != NULL)
		pointsl[i].image = image;

	}
}
void DrawPointsL(PointL pointsl[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (pointsl[i].live)
		{
			int fx = (pointsl[i].curFrame % pointsl[i].animationColumns) * pointsl[i].frameWidth;
			int fy = (pointsl[i].curFrame / pointsl[i].animationColumns) * pointsl[i].frameHeight;

			al_draw_bitmap_region(pointsl[i].image, fx, fy, pointsl[i].frameWidth, pointsl[i].frameHeight, pointsl[i].x - pointsl[i].frameWidth / 2, pointsl[i].y - pointsl[i].frameHeight / 2, 0);
			/*al_draw_filled_rectangle(pointsl[i].x - pointsl[i].boundx, pointsl[i].y - pointsl[i].boundy, pointsl[i].x + pointsl[i].boundx, pointsl[i].y + pointsl[i].boundy, al_map_rgba(255, 0, 255,100));*/
		}

	}
}
void StartPointsL(PointL pointsl[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!pointsl[i].live)
		{
			if (rand() % 1500 == 0)
			{
				pointsl[i].live = true;
				pointsl[i].x = WIDTH;
				pointsl[i].y = 30 + rand() % (HEIGHT - 60);

				break;
			}
		}
	}
}
void UptadePointsL(PointL pointsl[], int size)
{
	for (int i = 0;i < size;i++)
	{
		if (pointsl[i].live)
		{
			if (++pointsl[i].frameCount >= pointsl[i].frameDelay)
			{
				pointsl[i].curFrame += pointsl[i].animationDirection;
				if (pointsl[i].curFrame >= pointsl[i].maxFrame)
					pointsl[i].curFrame = 0;
				else if (pointsl[i].curFrame <= 0)
					pointsl[i].curFrame = pointsl[i].maxFrame - 1;
				pointsl[i].frameCount = 0;
			}
			pointsl[i].x -= pointsl[i].speed;

			if (pointsl[i].x < 0)
				pointsl[i].live = false;
		}
	}
}
void ColidePointsL(PointL pointsl[], int bsize, SuperSheep &sheep)
{
	for (int i = 0; i < bsize; i++)
	{
		if (pointsl[i].live)
		{
			if (pointsl[i].x - pointsl[i].boundx < sheep.x + sheep.boundx &&
				pointsl[i].x + pointsl[i].boundx > sheep.x - sheep.boundx &&
				pointsl[i].y - pointsl[i].boundy < sheep.y + sheep.boundy &&
				pointsl[i].y + pointsl[i].boundy > sheep.y - sheep.boundy)
			{
				sheep.score = sheep.score + 50;
				pointsl[i].live = false;
				al_play_sample(pointsup, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
		}
	}
}

void InitTlo(Tlo &back, float x, float y, float velx, float vely, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image = NULL)
{

		back.x = x;
		back.y = y;
		back.velX = velx;
		back.velY = vely;
		back.width = width;
		back.height = height;
		back.dirX = dirX;
		back.dirY = dirY;

		if(image != NULL)
			back.image = image;

	
}
void UptadeTlo(Tlo &back)
{
	back.x += back.velX * back.dirX;
		if (back.x + back.width <= 0)
		{
			back.x = 0;
		}
	
}
void DrawTlo(Tlo &back)
{
		al_draw_bitmap(back.image, back.x, back.y, 0);

		if (back.x + back.width < WIDTH)
		{
			al_draw_bitmap(back.image, back.x + back.width, back.y, 0);
		}

	
}

void ZmianaOkna(int &okno, int noweOkno)
{
	if (okno == START)
	{
		InitTlo(SBG, 0, 0, 0, 0, 1200, 600, move, 1);

	}
	else if(okno == MENUP)
	{ }
	else if (okno == MENUH)
	{ }
	else if (okno == MENUQ)
	{ }
	else if (okno == MENUG)
	{ }
	else if (okno == GUIDE)
	{ }
	else if (okno == HIGHSCORE)
	{ }
	else if(okno == GRA)
	{ }
	else if (okno == PAUZA)
	{ }
	else if (okno == RESTART)
	{ }
	else if(okno == DEATH)
	{ }

	okno = noweOkno;

	if (okno == START)
	{
		InitTlo(SBG, 0, 0, 0, 0, 1200, 600, move, 1);
		al_play_sample_instance(limits);
	}
	else if(okno == MENUP)
	{
		al_play_sample_instance(limits);
	}
	else if (okno == MENUH)
	{ }
	else if (okno == MENUQ)
	{ }
	else if (okno == MENUG)
	{ }
	else if (okno == GUIDE)
	{ }
	else if (okno == HIGHSCORE)
	{ }
	else if(okno == GRA)
	{
		InitSheep(sheep);
		InitObstacles(obstacles, NUM_OBST);
		InitLives(lives, NUM_LIV);
		InitPoints(points, NUM_PNT);
		InitPointsM(pointsm, NUM_PNTM);
		InitPointsL(pointsl, NUM_PNTL);
		InitTlo(BG, 0, 0, 1, 0, 1200, 600, move, 1);
		InitTlo(MG, 0, 0, 3, 0, 2400, 600, move, 1);
		InitTlo(FG, 0, 0, 5, 0, 3600, 600, move, 1);
		al_play_sample_instance(limits);
	}
	else if (okno == PAUZA)
	{ }
	else if (okno == RESTART)
	{ }
	else if(okno == DEATH)
	{
		al_stop_sample_instance(limits);
		al_play_sample(lose, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	}

}
