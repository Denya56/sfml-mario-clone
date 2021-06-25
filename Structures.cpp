#include "Structures.h"

platform::platform(Texture& image, float x, float y, char block)
{
	this->block = block;
	this->sprite.setTexture(image);
	this->rect = FloatRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
	this->sprite.setPosition(rect.left, rect.top);
}

enemy::enemy(Texture& image, float x, float y)
{
	this->sprite.setTexture(image);
	this->rect = FloatRect(x, y, ENEMY_WIDTH, ENEMY_HEIGHT);
	this->sprite.setPosition(this->rect.left, this->rect.top);
	this->i = -1;
	this->vx = this->vy = 0;
}

void enemy::update(vector<platform> vp)
{
	if (this->i == -1)
	{
		this->vx = -ENEMY_SPEED + this->sh;
	}

	if (this->i == 1)
	{
		this->vx = ENEMY_SPEED + this->sh;
	}
	if (!this->onGround)
		this->vy += GRAVITY;

	this->onGround = false;
	this->rect.top += this->vy;
	if (this->rect.top > 480)
		this->alive = false;
	collide(0, vp);


	this->rect.left += this->vx;
	collide(1, vp);

	this->sprite.setPosition(this->rect.left, this->rect.top);
}

void enemy::collide(int num, vector<platform> vp)
{
	for (platform pl : vp)
	{
		if (this->rect.intersects(pl.rect))
		{
			if (this->vx < 0 && num == 1)
			{
				this->rect.left = pl.rect.left + BLOCK_SIZE;
				this->i *= -1;
			}
			else if (this->vx > 0 && num == 1)
			{
				this->rect.left = pl.rect.left - ENEMY_WIDTH;
				this->i *= -1;
			}
			if (this->vy > 0 && num == 0)
			{
				this->onGround = true;
				this->rect.top = pl.rect.top - ENEMY_HEIGHT;
				this->vy = 0;
			}
		}
	}
}

player::player(Texture& image)
{
	this->sprite.setTexture(image);
	this->rect = FloatRect(0.f, 382.f, PLAYER_WIDTH, PLAYER_HEIGHT);
	this->vx = this->vy = 0.f;
}

int player::update(vector<platform> vp, vector<enemy*> ve)
{
	if (!this->alive)
		return 1;
	if (this->up)
		if (this->onGround)
			this->vy = -JUMP;
	if (this->left)
	{
		this->vx = -SPEED;
		if (this->rect.left < 5.f)
			this->left = false;
	}
	if (this->right)
	{
		this->vx = SPEED;
		if (this->rect.left > 660.f)
			this->right = false;
	}
	if (!(this->left || this->right))
	{
		this->vx = 0;
	}
	if (!this->onGround)
		this->vy += GRAVITY;

	this->onGround = false;
	this->rect.top += this->vy;
	if (this->rect.top > 480)
		return 1;
	collide(0, this->vy, vp);

	this->rect.left += this->vx;
	collide(this->vx, 0, vp);

	this->sprite.setPosition(this->rect.left, this->rect.top);

	for (enemy* en : ve)
	{
		if (en->alive)
		{
			if (this->rect.intersects(en->rect))
			{
				if (this->vy > 0)
				{
					en->alive = false;
					this->rect.top = en->rect.top - PLAYER_HEIGHT;
				}
				else
				{
					this->alive = false;
				}
			}
		}
	}
}

void player::collide(float vx, float vy, vector<platform> vp)
{
	for (platform pl : vp)
	{
		if (this->rect.intersects(pl.rect))
		{
			if (vx > 0)
				this->rect.left = pl.rect.left - PLAYER_WIDTH;
			if (vx < 0)
				this->rect.left = pl.rect.left + BLOCK_SIZE;
			if (vy > 0)
			{
				this->rect.top = pl.rect.top - PLAYER_HEIGHT;
				this->onGround = true;
				this->vy = 0;
			}
			if (vy < 0)
			{
				if (pl.block == '?')
					++this->coins;
				this->rect.top += PLAYER_HEIGHT;
				this->vy = 0;
			}
		}
	}
}