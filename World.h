#pragma once

#include<unordered_map>
#include"ChunkCylinder.h"
#include"Math.h"
#include <thread>
#include<mutex>
//#include"ChunkRenderer.h"

class ChunkRenderer;

#define CHUNK_CENTER_X 501
#define CHUNK_CENTER_Z 501
#define PLAYER_SPAWN_X(x) (x+CHUNK_CENTER_X) * CHUNK_WIDTH_SIZE
#define PLAYER_SPAWN_Y(y) y + CHUNK_WIDTH_SIZE
#define PLAYER_SPAWN_Z(z) (z+CHUNK_CENTER_Z) * CHUNK_WIDTH_SIZE

#define WATER_LEVEL 5

#define RENDER_DISTANCE 1

class World 
{
public:
	World();
	~World();
	void push_chunk(ChunkCylinder& chunk);
	static BlockType get_block_type(int x, int y, int z);
	bool is_chunk_cylinder_exist(int x, int y);
	void render();
	void update();
	void set_chunk_renderer(ChunkRenderer* renderer);
	static std::unordered_map<math::VectorXZ_t, ChunkCylinder>* get_map();
private:
	static std::unordered_map<math::VectorXZ_t, ChunkCylinder> map;
	std::thread _chunk_building_thd;
	ChunkRenderer* _chunk_renderer;
	std::mutex _chunk_mtx;
};