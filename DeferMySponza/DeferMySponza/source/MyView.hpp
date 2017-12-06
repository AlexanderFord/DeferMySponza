#pragma once

#include <sponza/sponza_fwd.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tgl/tgl.h>
#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <map>

class MyView : public tygra::WindowViewDelegate
{
public:

    MyView();

    ~MyView();

    void setScene(const sponza::Context * scene);

private:

	void gBufferPass(glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position);

	void directionalLightPass(glm::vec3 camera_position);

	void pointLightPass(glm::mat4 projection_xform, glm::mat4 view_xform, glm::vec3 camera_position);

	void spotlightPass(glm::mat4 projection_xform, glm::mat4 view_xform, glm::vec3 camera_position);

    void windowViewWillStart(tygra::Window * window) override;

    void windowViewDidReset(tygra::Window * window,
                            int width,
                            int height) override;

    void windowViewDidStop(tygra::Window * window) override;

    void windowViewRender(tygra::Window * window) override;

	struct directional_lights
	{
		glm::vec3 directional_light_direction;
		glm::vec3 directional_light_intensity;
	};

	struct point_lights
	{
		glm::vec3 point_light_position;
		float point_light_range;
		glm::vec3 point_light_intensity;
	};

	struct PerDrawUniforms
	{
		glm::mat4 projection_view_model_xform;
		glm::mat4 model_xform;
	};

	struct PerFrameUniforms
	{
		glm::mat4 view_xform;
		glm::mat4 projection_xform;
		glm::vec3 camera_position;
		float pad0;
	};

	PerDrawUniforms per_draw_uniforms;
	PerFrameUniforms per_frame_uniforms;
	GLuint per_draw_ubo_;
	GLuint per_frame_ubo_;

	//gbuffer
	GLuint gbuffer_fbo;
	GLuint gbuffer_position_texture;
	GLuint gbuffer_normal_texture;
	GLuint gbuffer_material_texture;
	GLuint depth_stencil_rbo;

	//lbuffer
	GLuint lbuffer_fbo;
	GLuint lbuffer_colour_rbo;

	//Shader Programs
	GLuint gbuffer_prog_;
	GLuint direction_light_prog_;
	GLuint point_light_prog_;
	GLuint spotlight_prog_;

	struct MeshGL
	{
		GLuint position_vbo{ 0 };
		GLuint normal_vbo{ 0 };
		GLuint element_vbo{ 0 };
		GLuint texcoord_vbo{ 0 };
		GLuint tangent_vbo{ 0 };
		GLuint vao{ 0 };
		int element_count;
	};

	struct Mesh
	{
		GLuint vertex_vbo{ 0 };
		GLuint element_vbo{ 0 };
		GLuint vao{ 0 };
		int element_count{ 0 };
	};

	Mesh light_quad_mesh_;
	Mesh light_sphere_mesh_;
	Mesh light_cone_mesh_;

	std::map<sponza::MeshId, MeshGL> sponza_mesh_;
    const sponza::Context * scene_;
};
