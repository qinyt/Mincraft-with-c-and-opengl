#include"World.h"
#include<chrono>
#include"Game.h"
#include"Application.h"


#define sleep_millisecons(x) std::this_thread::sleep_for(std::chrono::milliseconds(50))

std::unordered_map<math::VectorXZ_t, ChunkCylinder> World::map;

#if 1
#define LOCK _chunk_mtx.lock();

#define UNLOCK _chunk_mtx.unlock();
#else
#define LOCK 
#define UNLOCK 
#endif

#define RENDER_LIMITE 6


World::World() :
	_render_distance(2),
	_chunk_renderer(),
	_chunk_building_thd([&]()
{
	int posX, posZ;
	while (App::is_running) 
	{
		posX = (Game::player.get_position()->x) / CHUNK_WIDTH_SIZE;
		posZ = (Game::player.get_position()->z) / CHUNK_WIDTH_SIZE;
		LOCK;
		// must add chunk first, THEN mesh. Otherwise, the meshing process is wrong.
		for (int i = -_render_distance; i < _render_distance; ++i)
			for (int j = -_render_distance; j < _render_distance; ++j)
			{
				int x = posX + j;
				int z = posZ + i;
				math::VectorXZ_t key = { x, z };
				if (map.find(key) == map.end()) 
				{
					ChunkCylinder chunk_cylinder(x, z);
					push_chunk(chunk_cylinder);
				};
				auto& chunk_cylinder = map.at(key);
				if (chunk_cylinder.is_meshed() == false)
				{
					chunk_cylinder.mesh();
				}
			}
		if (++_render_distance == RENDER_LIMITE) _render_distance = 2;
		UNLOCK;
		sleep_millisecons(10);
	}
})
{}

World::~World() 
{
	_chunk_building_thd.join();
}

std::unordered_map<math::VectorXZ_t, ChunkCylinder>* World::get_map()
{
	return &map;
}

bool World::is_chunk_cylinder_exist(int x, int y) 
{
	math::VectorXZ_t key = { x, y};
	return (map.find(key) != map.end());
}

void World::push_chunk(ChunkCylinder& chunk) 
{
	map.emplace(chunk.get_pos(), chunk);
}

BlockType World::get_block_type(int x, int y, int z) 
{
	int world_posX = static_cast<int>(x / CHUNK_WIDTH_SIZE);
	int world_posZ = static_cast<int>(z / CHUNK_WIDTH_SIZE);

	math::VectorXZ_t key = { world_posX, world_posZ };
	if (map.find(key) == map.end())
		return BlockType::AIR;
	ChunkCylinder& cy = map.at(key);	

	int ix = x - world_posX * CHUNK_WIDTH_SIZE;
	int iz = z - world_posZ * CHUNK_WIDTH_SIZE;

	return cy.get_block_within(ix, y, iz);
}

void World::set_chunk_renderer(ChunkRenderer* renderer) 
{
	_chunk_renderer = renderer;
}


void World::update() 
{

}

void World::render() 
{
	int posX = (Game::player.get_position()->x) / CHUNK_WIDTH_SIZE;
	int posZ = (Game::player.get_position()->z) / CHUNK_WIDTH_SIZE;
	LOCK;
	for (auto iter = map.begin(); iter != map.end();)
	{
		auto& pos = iter->first;
		int min_x = posX - RENDER_LIMITE;
		int min_z = posZ - RENDER_LIMITE;
		int max_x = posX + RENDER_LIMITE;
		int max_z = posZ + RENDER_LIMITE;

		bool out = (pos.x < min_x || pos.z < min_z || pos.x > max_x || pos.z > max_z);
		if(out)
		{
			for(auto& chunk: iter->second.get_chunks()) 
				chunk.get_model()->clear_data();
			iter = map.erase(iter);
			continue;
		}
		for (auto& chunk : iter->second.get_chunks()) 
		{
			//if (App::keyboard.is_key_down(sf::Keyboard::Key::Space)) __debugbreak();
			chunk.add_data_to_GPU();
			_chunk_renderer->add_chunk(&chunk);
		}
		++iter;
	}
	UNLOCK;
}

void World::load_chunk(int x, int z) 
{
	if (!is_chunk_cylinder_exist(x, z))
	{
		//TODO: async the process
		ChunkCylinder chunk_cylinder(x, z);
		push_chunk(chunk_cylinder);
	}
}
