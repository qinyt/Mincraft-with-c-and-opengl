#include"ChunkRenderer.h"
#include"Model.h"
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include"World.h"
#include"Application.h"

ChunkRenderer::ChunkRenderer(Camera* camera) :
	_tex("./Textures/DefaultPack2.png")
{
	_shader.set_project_mat(camera->get_proj_mat());
}

ChunkRenderer::~ChunkRenderer() 
{
	for (auto model : _models) 
	{
		delete model;
	}
}

void ChunkRenderer::add_model(Model* model)
{
	_models.emplace_back(model->get_render_info());
}

void ChunkRenderer::render(Camera* camera) 
{	
	if (_models.empty()) return;

	_shader.bind();
	_shader.set_model_view_mat(*camera->get_model_view_mat());
	_tex.bind();
	glEnable(GL_CULL_FACE);
		
	for (auto model : _models) 
	{
		glBindVertexArray(model->vao);
		glDrawElements(GL_TRIANGLES, model->indices_count, GL_UNSIGNED_INT, nullptr);
	}
	_models.clear();
}