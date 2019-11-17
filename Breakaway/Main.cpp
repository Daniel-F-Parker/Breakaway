#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "Entities.h"

#define FPS 60
#define BRICK_WIDTH 128
#define BRICK_HEIGHT 32
#define OFFSET_X 64
#define OFFSET_Y 64
#define PADDLE_SPEED 8
#define BALL_SPEED 6
#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define MISS 4
#define HIT 5

static bool keep_running;
static bool lose;
static bool win;

void Cleanup()
{

}

bool CheckWin(Brick bricks[], size_t num_bricks)
{
	for (int i = 0; i < num_bricks; i++)
	{
		if (bricks[i].state == 1)
		{
			return false;
		}
	}
	return true;
}

int CheckBallPaddleCollision(Ball *ball, SDL_Rect *paddle)
{
	if (((ball->x >= paddle->x) && (ball->x <= (paddle->x + paddle->w)) && (ball->y + ball->size >= paddle->y)) ||
		(((ball->x + ball->size) >= paddle->x) && ((ball->x + ball->size) <= (paddle->x + paddle->w)) && (ball->y + ball->size >= paddle->y)))
	{
		/*if (ball->y + ball->size >= paddle->y && ball->y + ball->size <= paddle->y + paddle->h)
		{
			return BOTTOM;
		}
		if (ball->y <= paddle->y + paddle->h && ball->y >= paddle->y)
		{
			return TOP;
		}
		if (ball->x + ball->size >= paddle->x && ball->x + ball->size <= paddle->x + paddle->w)
		{
			return RIGHT;
		}
		if (ball->x <= paddle->x + paddle->w && ball->x >= paddle->x)
		{
			return LEFT;
		}*/
		return HIT;
	}
	return MISS;
}

int CheckBallCollision(SDL_Rect *screen_rect, Brick bricks[], size_t num_bricks, Ball *ball, SDL_Rect *paddle)
{

	for (int i = 0; i < num_bricks; i++)
	{
		if (((ball->x >= bricks[i].rect.x) && (ball->x <= (bricks[i].rect.x + bricks[i].rect.w)) && (ball->y <= (bricks[i].rect.y + bricks[i].rect.h))) && (ball->y >= bricks[i].rect.y ) ||
			(((ball->x + ball->size) >= bricks[i].rect.x) && ((ball->x + ball->size) <= (bricks[i].rect.x + bricks[i].rect.w)) && (ball->y <= (bricks[i].rect.y + bricks[i].rect.h))))
		{
			if (bricks[i].state == 1)
			{
				bricks[i].state = 0;
				if (ball->y + ball->size >= bricks[i].rect.y && ball->y + ball->size <= bricks[i].rect.y + bricks[i].rect.h)
				{
					return BOTTOM;
				}
				if (ball->y <= bricks[i].rect.y + bricks[i].rect.h && ball->y >= bricks[i].rect.y )
				{
					return TOP;
				}
				if (ball->x + ball->size >= bricks[i].rect.x && ball->x + ball->size <= bricks[i].rect.x + bricks[i].rect.w)
				{
					return RIGHT;
				}
				if (ball->x <= bricks[i].rect.x + bricks[i].rect.w && ball->x >= bricks[i].rect.x)
				{
					return LEFT;
				}
			}
		}
	}
	if ((ball->x <= screen_rect->x) || (ball->y <= screen_rect->y) ||
		((ball->size + ball->x) >= (screen_rect->w + screen_rect->x)) || ((ball->size + ball->y) >= (screen_rect->h + screen_rect->y)))
	{
		if (ball->y + ball->size >= screen_rect->y + screen_rect->h)
		{
			lose = true;
			return BOTTOM;
		}
		if (ball->y <= screen_rect->y )
		{
			return TOP;
		}
		if (ball->x + ball->size >= screen_rect->x + screen_rect->w)
		{
			return RIGHT;
		}
		if (ball->x <= screen_rect->x)
		{
			return LEFT;
		}
	}
	
	return MISS;
}

bool CheckBoundaryCollision(SDL_Rect *screen_rect, SDL_Rect *paddle)
{
	if ((paddle->x <= screen_rect->x) || (paddle->y <= screen_rect->y) ||
		((paddle->w + paddle->x) >= (screen_rect->w + screen_rect->x)) || ((paddle->h + paddle->y) >= (screen_rect->h + screen_rect->y)))
	{
		return true;
	}
	return false;
}


void Render(SDL_Renderer *renderer, SDL_Rect *paddle_rect, Ball *ball, Brick bricks[], size_t num_bricks)
{
	int err = SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_RenderClear(renderer);
	err = SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x0, 0xFF);
	SDL_RenderFillRect(renderer, paddle_rect);
	SDL_Rect ball_rect = { ball->x, ball->y, ball->size, ball->size };
	err = SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &ball_rect);
	for (int i = 0; i < num_bricks; i++)
	{
		if (bricks[i].state == 1)
		{
			err = SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderFillRect(renderer, &bricks[i].rect);
		}
	}
	//err = SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}
void UpdateGame(SDL_Rect *screen_rect, SDL_Rect *paddle_rect, Ball *ball, V2 paddle_velocity, V2 *ball_velocity, Brick bricks[], size_t num_bricks)
{
	SDL_Rect temp_paddle = *paddle_rect;
	Ball temp_ball = *ball;
	temp_paddle.x += (int)paddle_velocity.x;
	temp_paddle.y += (int)paddle_velocity.y;
	temp_ball.x += ball_velocity->x;
	temp_ball.y += ball_velocity->y;
	int collide = CheckBallCollision(screen_rect, bricks, num_bricks, &temp_ball, paddle_rect);
	if (collide == MISS)
	{
		collide = CheckBallPaddleCollision(ball, paddle_rect);
		if (collide == MISS)
		{
			*ball = temp_ball;
		}
		else
		{
			V2 vec1 = { temp_ball.x - ball->x  , temp_ball.y - ball->y };
			V2 vec2 = { -vec1.x, vec1.y };
			float mirror_angle = acos(DotProduct(vec1, vec2) / (Magnitude(vec1)*Magnitude(vec2)));
			/*if (collide == TOP)
			{
				ball_velocity->y = BALL_SPEED;
			}*/
			if (collide == HIT)
			{
				int mid_paddle = paddle_rect->x + (paddle_rect->w * 0.5);
				int ans = mid_paddle - temp_ball.x;
				if (ans < 0)
					ball_velocity->y = -BALL_SPEED - (ans*0.1);
				else
					ball_velocity->y = -BALL_SPEED + (ans*0.1);
			}
			/*if (collide == RIGHT)
			{
				ball_velocity->x = -BALL_SPEED;
			}
			else if (collide == LEFT)
			{
				ball_velocity->x = BALL_SPEED;
			}*/
			ball->x += ball_velocity->x;
			ball->y += ball_velocity->y;
			/*V2 vec1 = { temp_ball.x - ball->x  , temp_ball.y - ball->y };
			V2 vec2 = { -vec1.x, vec1.y };
			float mirror_angle = acos(DotProduct(vec1, vec2) / (Magnitude(vec1)*Magnitude(vec2)));
			ball->x += cosf(mirror_angle)*BALL_SPEED;
			ball->y += sinf(mirror_angle)*BALL_SPEED;*/
			/*ball->x += vec1.x*BALL_SPEED;
			ball->y += -vec1.y*BALL_SPEED;*/
		}
		
	}
	else
	{
		if (collide == TOP)
		{
			ball_velocity->y = fabs(ball_velocity->y);
		}
		else if (collide == BOTTOM)
		{
			ball_velocity->y = -ball_velocity->y;
		}
		if (collide == RIGHT)
		{
			ball_velocity->x = -ball_velocity->x;
		}
		else if (collide == LEFT)
		{
			ball_velocity->x = fabs(ball_velocity->x);
		}
		ball->x += ball_velocity->x;
		ball->y += ball_velocity->y;
	}
	collide = CheckBoundaryCollision(screen_rect, &temp_paddle);
	if (!collide)
	{
		*paddle_rect = temp_paddle;
	}

	win = CheckWin(bricks, num_bricks);
}
void HandleEvent(V2 *velocity)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.scancode == SDL_SCANCODE_A)
		{
			velocity->x = -PADDLE_SPEED;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_D)
		{
			velocity->x = PADDLE_SPEED;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			lose = true;
			keep_running = false;
		}
	}
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.scancode == SDL_SCANCODE_A)
		{
			velocity->x = 0;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_D)
		{
			velocity->x = 0;
		}
	}
}

void StartGame(Brick bricks[], size_t num_bricks_x, size_t num_bricks_y, Ball *ball, SDL_Rect *paddle, SDL_Rect *screen_rect, V2 *ball_velocity)
{
	int i = 0;
	int space_x = 0;
	int space_y = 0;
	for (int y = 0; y < num_bricks_y; y++)
	{
		for (int x = 0; x < num_bricks_x; x++)
		{
			bricks[i].state = 1;
			bricks[i].rect.x = x * BRICK_WIDTH+OFFSET_X+x;
			bricks[i].rect.y = y * BRICK_HEIGHT +OFFSET_Y+ y;
			bricks[i].rect.w = BRICK_WIDTH;
			bricks[i].rect.h = BRICK_HEIGHT;
			i++;
		}
	}
	*paddle = { 512, 592, 128, 32 };
	ball->x = 512.0f;
	ball->y = 528.0f;
	ball->size = 32;
	*ball_velocity = { BALL_SPEED, BALL_SPEED };
	*screen_rect = { 0, 0, 1280, 720 };
}

int main(int argc, char *argv[])
{
	int SDL_Init(SDL_INIT_EVERYTHING);
	keep_running = true;
	const char title[] = "Breakaway";
	int window_width = 1280;
	int window_height = 720;
	SDL_Window* window =  SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Ball ball = {};
	SDL_Rect paddle_rect = {};
	SDL_Rect screen_rect = {};
	uint32_t start_time, end_time, delta_time;
	uint32_t frame_delay = 1000 / FPS;
	const size_t num_bricks = 36;
	const size_t num_bricks_x = 9;
	const size_t num_bricks_y = 4;
	Brick bricks[num_bricks];
	V2 paddle_velocity = {};
	V2 ball_velocity = { BALL_SPEED, BALL_SPEED };
	do
	{
		StartGame(bricks, num_bricks_x, num_bricks_y, &ball, &paddle_rect, &screen_rect, &ball_velocity);
		keep_running = true;
		lose = false;
		win = false;
		while (!lose && !win)
		{
			start_time = SDL_GetTicks();

			HandleEvent(&paddle_velocity);
			UpdateGame(&screen_rect, &paddle_rect, &ball, paddle_velocity, &ball_velocity, bricks, num_bricks);
			Render(renderer, &paddle_rect, &ball, bricks, num_bricks);

			end_time = SDL_GetTicks();
			delta_time = end_time - start_time;
			if (frame_delay > delta_time)
				SDL_Delay(frame_delay - delta_time);
		}
	} while (keep_running && !win);

	Cleanup();
	return 0;
}