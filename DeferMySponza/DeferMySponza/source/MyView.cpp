#include "MyView.hpp"

#include <sponza/sponza.hpp>
#include <tygra/FileHelper.hpp>
#include <tsl/shapes.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>

MyView::MyView()
{
}

MyView::~MyView() {
}

void MyView::setScene(const sponza::Context * scene)
{
    scene_ = scene;
}

void MyView::windowViewWillStart(tygra::Window * window)
{
    assert(scene_ != nullptr);

	/*Shaders are generated here*/

	GLint compile_status = 0;

	/*GBUFFER SHADERS*/
	{
		//Vertex Shader
		compile_status = 0;

		GLuint gbuffer_vs = glCreateShader(GL_VERTEX_SHADER);
		std::string gbuffer_vs_string = tygra::createStringFromFile("resource:///gbuffer_vs.glsl");
		const char *gbuffer_vs_code = gbuffer_vs_string.c_str();
		glShaderSource(gbuffer_vs, 1, (const GLchar **)&gbuffer_vs_code, NULL);
		glCompileShader(gbuffer_vs);
		glGetShaderiv(gbuffer_vs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(gbuffer_vs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		//Fragment Shader
		GLint compile_status = 0;

		GLuint gbuffer_fs = glCreateShader(GL_FRAGMENT_SHADER);
		std::string gbuffer_fs_string = tygra::createStringFromFile("resource:///gbuffer_fs.glsl");
		const char *gbuffer_fs_code = gbuffer_fs_string.c_str();
		glShaderSource(gbuffer_fs, 1, (const GLchar **)&gbuffer_fs_code, NULL);
		glCompileShader(gbuffer_fs);
		glGetShaderiv(gbuffer_fs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(gbuffer_fs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		gbuffer_prog_ = glCreateProgram();

		//Attach the data to the shader program
		glAttachShader(gbuffer_prog_, gbuffer_vs);
		glBindAttribLocation(gbuffer_prog_, 0, "vertex_position");
		glBindAttribLocation(gbuffer_prog_, 1, "vertex_normal");
		glDeleteShader(gbuffer_vs);

		glAttachShader(gbuffer_prog_, gbuffer_fs);
		glBindFragDataLocation(gbuffer_prog_, 0, "fragment_colour");
		glDeleteShader(gbuffer_fs);

		glLinkProgram(gbuffer_prog_);

		GLint link_status = 0;
		glGetProgramiv(gbuffer_prog_, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetProgramInfoLog(gbuffer_prog_, string_length, NULL, log);
			std::cerr << log << std::endl;
		}
	}
	/*DIRECTIONAL LIGHT SHADERS*/
	{
		//Vertex Shader
		GLint compile_status = 0;

		GLuint directional_light_vs = glCreateShader(GL_VERTEX_SHADER);
		std::string directional_light_vs_string = tygra::createStringFromFile("resource:///directional_light_vs.glsl");
		const char *directional_light_vs_code = directional_light_vs_string.c_str();
		glShaderSource(directional_light_vs, 1, (const GLchar **)&directional_light_vs_code, NULL);
		glCompileShader(directional_light_vs);
		glGetShaderiv(directional_light_vs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(directional_light_vs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		//Fragment Shader
		compile_status = 0;

		GLuint directional_light_fs = glCreateShader(GL_FRAGMENT_SHADER);
		std::string directional_light_fs_string = tygra::createStringFromFile("resource:///directional_light_fs.glsl");
		const char *directional_light_fs_code = directional_light_fs_string.c_str();
		glShaderSource(directional_light_fs, 1, (const GLchar **)&directional_light_fs_code, NULL);
		glCompileShader(directional_light_fs);
		glGetShaderiv(directional_light_fs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(directional_light_fs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		direction_light_prog_ = glCreateProgram();

		//Attach the data to the shader program
		glAttachShader(direction_light_prog_, directional_light_vs);
		glBindAttribLocation(direction_light_prog_, 0, "vertex_position");
		glDeleteShader(directional_light_vs);

		glAttachShader(direction_light_prog_, directional_light_fs);
		glBindFragDataLocation(direction_light_prog_, 0, "fragment_colour");
		glDeleteShader(directional_light_fs);

		glLinkProgram(direction_light_prog_);

		GLint link_status = 0;
		glGetProgramiv(direction_light_prog_, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetProgramInfoLog(direction_light_prog_, string_length, NULL, log);
			std::cerr << log << std::endl;
		}
	}

	/*POINT LIGHT SHADERS*/
	{
		//Vertex Shader
		GLint compile_status = 0;

		GLuint point_light_vs = glCreateShader(GL_VERTEX_SHADER);
		std::string point_light_vs_string = tygra::createStringFromFile("resource:///point_light_vs.glsl");
		const char *point_light_vs_code = point_light_vs_string.c_str();
		glShaderSource(point_light_vs, 1, (const GLchar **)&point_light_vs_code, NULL);
		glCompileShader(point_light_vs);
		glGetShaderiv(point_light_vs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(point_light_vs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		//Fragment Shader
		compile_status = 0;

		GLuint point_light_fs = glCreateShader(GL_FRAGMENT_SHADER);
		std::string point_light_fs_string = tygra::createStringFromFile("resource:///point_light_fs.glsl");
		const char *point_light_fs_code = point_light_fs_string.c_str();
		glShaderSource(point_light_fs, 1, (const GLchar **)&point_light_fs_code, NULL);
		glCompileShader(point_light_fs);
		glGetShaderiv(point_light_fs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(point_light_fs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		point_light_prog_ = glCreateProgram();

		//Attach the data to the shader program
		glAttachShader(point_light_prog_, point_light_vs);
		glBindAttribLocation(point_light_prog_, 0, "vertex_position");
		glDeleteShader(point_light_vs);

		glAttachShader(point_light_prog_, point_light_fs);
		glBindFragDataLocation(point_light_prog_, 0, "fragment_colour");
		glDeleteShader(point_light_fs);

		glLinkProgram(point_light_prog_);

		GLint link_status = 0;
		glGetProgramiv(point_light_prog_, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetProgramInfoLog(point_light_prog_, string_length, NULL, log);
			std::cerr << log << std::endl;
		}
	}

	/*SPOTLIGHT SHADERS*/
	{
		//Vertex Shader
		GLint compile_status = 0;

		GLuint spotlight_vs = glCreateShader(GL_VERTEX_SHADER);
		std::string spotlight_vs_string = tygra::createStringFromFile("resource:///spotlight_vs.glsl");
		const char *spotlight_vs_code = spotlight_vs_string.c_str();
		glShaderSource(spotlight_vs, 1, (const GLchar **)&spotlight_vs_code, NULL);
		glCompileShader(spotlight_vs);
		glGetShaderiv(spotlight_vs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(spotlight_vs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		//Fragment Shader
		compile_status = 0;

		GLuint spotlight_fs = glCreateShader(GL_FRAGMENT_SHADER);
		std::string spotlight_fs_string = tygra::createStringFromFile("resource:///spotlight_fs.glsl");
		const char *spotlight_fs_code = spotlight_fs_string.c_str();
		glShaderSource(spotlight_fs, 1, (const GLchar **)&spotlight_fs_code, NULL);
		glCompileShader(spotlight_fs);
		glGetShaderiv(spotlight_fs, GL_COMPILE_STATUS, &compile_status);

		if (compile_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetShaderInfoLog(spotlight_fs, string_length, NULL, log);
			std::cerr << log << std::endl;
		}

		spotlight_prog_ = glCreateProgram();

		//Attach the data to the shader program
		glAttachShader(spotlight_prog_, spotlight_vs);
		glBindAttribLocation(spotlight_prog_, 0, "vertex_position");
		glDeleteShader(spotlight_vs);

		glAttachShader(spotlight_prog_, spotlight_fs);
		glBindFragDataLocation(spotlight_prog_, 0, "fragment_colour");
		glDeleteShader(spotlight_fs);

		glLinkProgram(spotlight_prog_);

		GLint link_status = 0;
		glGetProgramiv(spotlight_prog_, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
		{
			const int string_length = 1024;
			GLchar log[string_length] = "";
			glGetProgramInfoLog(spotlight_prog_, string_length, NULL, log);
			std::cerr << log << std::endl;
		}
	}

	//Retrieve sponza geometry from the data file
	sponza::GeometryBuilder sponza;

	const auto& scene_meshes = sponza.getAllMeshes();

	for (const auto& scene_mesh : scene_meshes)
	{
		MeshGL &newMesh_ = sponza_mesh_[scene_mesh.getId()];

		const auto& positions = scene_mesh.getPositionArray();
		const auto& normals = scene_mesh.getNormalArray();
		const auto& elements = scene_mesh.getElementArray();
		const auto& texcoords = scene_mesh.getTextureCoordinateArray();
		const auto& tangents = scene_mesh.getTangentArray();

		/*SPONZA*/

		/*VERTEX BUFFER OBJECTS*/
		{
			//Position VBO
			glGenBuffers(1, &newMesh_.position_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.position_vbo);
			glBufferData(GL_ARRAY_BUFFER,
				positions.size() * sizeof(glm::vec3),
				positions.data(),
				GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//Normal VBO
			glGenBuffers(1, &newMesh_.normal_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.normal_vbo);
			glBufferData(GL_ARRAY_BUFFER,
				normals.size() * sizeof(glm::vec3),
				normals.data(),
				GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//Element VBO
			glGenBuffers(1, &newMesh_.element_vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh_.element_vbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				elements.size() * sizeof(unsigned int),
				elements.data(),
				GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			newMesh_.element_count = elements.size();

			//Texture Coordinates VBO
			glGenBuffers(1, &newMesh_.texcoord_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.texcoord_vbo);
			glBufferData(GL_ARRAY_BUFFER,
				texcoords.size() * sizeof(glm::vec2),
				texcoords.data(),
				GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//Tangent VBO
			glGenBuffers(1, &newMesh_.tangent_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.tangent_vbo);
			glBufferData(GL_ARRAY_BUFFER,
				tangents.size() * sizeof(glm::vec3),
				tangents.data(),
				GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		/*VERTEX ARRAY OBJECTS*/
		{
			//VAO
			glGenVertexArrays(1, &newMesh_.vao);
			glBindVertexArray(newMesh_.vao);

			//Positions
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.position_vbo);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

			//Normals
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.normal_vbo);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
				sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

			//Elements
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh_.element_vbo);

			//Texture Coordinates
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.texcoord_vbo);
			if (texcoords.size() <= 0)
				glDisableVertexAttribArray(2);
			else
				glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(glm::vec2), TGL_BUFFER_OFFSET(0));

			//Tangents
			glBindBuffer(GL_ARRAY_BUFFER, newMesh_.tangent_vbo);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
				sizeof(glm::vec3), TGL_BUFFER_OFFSET(0));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	/*DIRECTIONAL LIGHTS*/
	{
		//Create a fullscreen quad
		std::vector<glm::vec2> vertices(4);
		vertices[0] = glm::vec2(-1, -1);
		vertices[1] = glm::vec2(1, -1);
		vertices[2] = glm::vec2(1, 1);
		vertices[3] = glm::vec2(-1, 1);

		//Light Quad Mesh VBO
		glGenBuffers(1, &light_quad_mesh_.vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_quad_mesh_.vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(glm::vec2),
			vertices.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Light Quad Mesh VAO
		glGenVertexArrays(1, &light_quad_mesh_.vao);
		glBindVertexArray(light_quad_mesh_.vao);
		glBindBuffer(GL_ARRAY_BUFFER, light_quad_mesh_.vertex_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec2), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	/*POINT LIGHTS*/
	{
		//Create a sphere
		tsl::IndexedMeshPtr mesh = tsl::createSpherePtr(1.f, 12);
		mesh = tsl::cloneIndexedMeshAsTriangleListPtr(mesh.get());

		light_sphere_mesh_.element_count = mesh->indexCount();

		//Light Sphere Mesh Vertex VBO
		glGenBuffers(1, &light_sphere_mesh_.vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_sphere_mesh_.vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			mesh->vertexCount() * sizeof(glm::vec3),
			mesh->positionArray(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Light Sphere Mesh Element VBO
		glGenBuffers(1, &light_sphere_mesh_.element_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_sphere_mesh_.element_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->indexCount() * sizeof(unsigned int),
			mesh->indexArray(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Light Sphere Mesh VAO
		glGenVertexArrays(1, &light_sphere_mesh_.vao);
		glBindVertexArray(light_sphere_mesh_.vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_sphere_mesh_.element_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_sphere_mesh_.vertex_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	/*SPOTLIGHTS*/
	{
		tsl::IndexedMeshPtr mesh = tsl::createConePtr(1.f, 1.f, 12);
		mesh = tsl::cloneIndexedMeshAsTriangleListPtr(mesh.get());

		light_cone_mesh_.element_count = mesh->indexCount();

		//Light Cone Mesh Vertex VBO
		glGenBuffers(1, &light_cone_mesh_.vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_cone_mesh_.vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			mesh->vertexCount() * sizeof(glm::vec3),
			mesh->positionArray(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Light Sphere Mesh Element VBO
		glGenBuffers(1, &light_cone_mesh_.element_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_cone_mesh_.element_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->indexCount() * sizeof(unsigned int),
			mesh->indexArray(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//Light Sphere Mesh VAO
		glGenVertexArrays(1, &light_cone_mesh_.vao);
		glBindVertexArray(light_cone_mesh_.vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_cone_mesh_.element_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, light_cone_mesh_.vertex_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	/*FRAMEBUFFER OBJECTS*/
	{
		//Gbuffer
		glGenFramebuffers(1, &gbuffer_fbo);				//Gbuffer
		glGenTextures(1, &gbuffer_position_texture);	//Gbuffer position texture
		glGenTextures(1, &gbuffer_normal_texture);		//Gbuffer normal texture
		glGenTextures(1, &gbuffer_material_texture);	//Gbuffer material texture
										
		glGenFramebuffers(1, &lbuffer_fbo);				//Lbuffer
		glGenRenderbuffers(1, &lbuffer_colour_rbo);		//Lbuffer renderbuffer to hold colour
		glGenRenderbuffers(1, &depth_stencil_rbo);		//Depth/Stencil renderbuffer
	}

	/*UNIFORM BUFFER OBJECTS*/
	{
		//Per Draw UBO
		glGenBuffers(1, &per_draw_ubo_);
		glBindBuffer(GL_UNIFORM_BUFFER, per_draw_ubo_);
		glBufferData(GL_UNIFORM_BUFFER,
			sizeof(PerDrawUniforms),
			nullptr,
			GL_STREAM_DRAW);

		//Per Frame UBO
		glGenBuffers(1, &per_frame_ubo_);
		glBindBuffer(GL_UNIFORM_BUFFER, per_frame_ubo_);
		glBufferData(GL_UNIFORM_BUFFER,
			sizeof(PerFrameUniforms),
			nullptr,
			GL_STREAM_DRAW);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
}

void MyView::windowViewDidReset(tygra::Window * window,
                                int width,
                                int height)
{
    glViewport(0, 0, width, height);

	/*Create the texture objects and bind them to the framebuffer*/
	/*GBUFFER*/
	{
		//Gbuffer Position texture
		glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_texture);
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);

		//Gbuffer Normal Texture
		glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_texture);
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);

		//Gbuffer Material Texture
		glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_material_texture);
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);

		//Gbuffer Depth/Stencil Texture
		glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//Attach data to the gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, gbuffer_position_texture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, gbuffer_normal_texture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, gbuffer_material_texture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_rbo);

		GLenum gbuffer_status = 0;
		gbuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (gbuffer_status != GL_FRAMEBUFFER_COMPLETE) 
		{
			tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, "gbuffer not complete");
		}

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, buffers);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	/*LBUFFER*/
	{
		//Lbuffer colour data renderbuffer
		glBindRenderbuffer(GL_RENDERBUFFER, lbuffer_colour_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//Attach data to the lbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, lbuffer_colour_rbo);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_rbo);

		GLenum lbuffer_status = 0;
		lbuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (lbuffer_status != GL_FRAMEBUFFER_COMPLETE)
		{
			tglDebugMessage(GL_DEBUG_SEVERITY_HIGH_ARB, "lbuffer not complete");
		}
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void MyView::windowViewDidStop(tygra::Window * window)
{
	//Delete the buffers upon exit
	sponza::GeometryBuilder builder;

	const auto& scene_meshes = builder.getAllMeshes();

	for (const auto& mesh_ : scene_meshes)
	{
		const MeshGL& mesh = sponza_mesh_[mesh_.getId()];

		glDeleteBuffers(1, &mesh.position_vbo);
		glDeleteBuffers(1, &mesh.normal_vbo);
		glDeleteBuffers(1, &mesh.element_vbo);
		glDeleteBuffers(1, &mesh.texcoord_vbo);
		glDeleteBuffers(1, &mesh.tangent_vbo);
		glDeleteBuffers(1, &mesh.vao);
	}

	glDeleteBuffers(1, &light_quad_mesh_.vertex_vbo);
	glDeleteBuffers(1, &light_quad_mesh_.element_vbo);
	glDeleteBuffers(1, &light_quad_mesh_.vao);

	glDeleteBuffers(1, &light_sphere_mesh_.vertex_vbo);
	glDeleteBuffers(1, &light_sphere_mesh_.element_vbo);
	glDeleteBuffers(1, &light_sphere_mesh_.vao);

	glDeleteBuffers(1, &light_cone_mesh_.vertex_vbo);
	glDeleteBuffers(1, &light_cone_mesh_.element_vbo);
	glDeleteBuffers(1, &light_cone_mesh_.vao);

	glDeleteBuffers(1, &per_frame_ubo_);
	glDeleteBuffers(1, &per_draw_ubo_);

	glDeleteFramebuffers(1, &gbuffer_fbo);
	glDeleteTextures(1, &gbuffer_position_texture);
	glDeleteTextures(1, &gbuffer_normal_texture);
	glDeleteTextures(1, &gbuffer_material_texture);

	glDeleteFramebuffers(1, &lbuffer_fbo);
	glDeleteRenderbuffers(1, &lbuffer_colour_rbo);
	glDeleteRenderbuffers(1, &depth_stencil_rbo);

	glDeleteProgram(gbuffer_prog_);
	glDeleteProgram(direction_light_prog_);
	glDeleteProgram(point_light_prog_);
	glDeleteProgram(spotlight_prog_);
}

void MyView::windowViewRender(tygra::Window * window)
{
    assert(scene_ != nullptr);

	GLint viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);
	const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

	//Transformation Matrices
	glm::vec3 camera_position = (const glm::vec3&)scene_->getCamera().getPosition();
	glm::vec3 camera_direction = (const glm::vec3&)scene_->getCamera().getDirection();
	glm::mat4 view_xform = glm::lookAt(camera_position, camera_position + camera_direction, (const glm::vec3&)scene_->getUpDirection());
	glm::mat4 projection_xform = glm::perspective(glm::radians(75.0f), aspect_ratio, 1.f, 1000.f);
	
	//Perform the gbuffer pass
	gBufferPass(view_xform, projection_xform, camera_position);

	//Perform the directional light pass
	directionalLightPass(camera_position);

	//Perform the point light pass
	pointLightPass(projection_xform, view_xform, camera_position);

	//Perform the point light pass
	spotlightPass(projection_xform, view_xform, camera_position);

	//Blit the lbuffer colour data to the screen
	glBindFramebuffer(GL_READ_FRAMEBUFFER, lbuffer_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, viewport_size[2], viewport_size[3], 0, 0, viewport_size[2], viewport_size[3], GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MyView::gBufferPass(glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position)
{
	//Bind the gbuffer and clear the buffers
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	glClearColor(108.f/255.f, 39.f/255.f, 135.f/255.f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Set up the stencil to protect the background
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//Enable the depth test
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glDisable(GL_BLEND);

	glUseProgram(gbuffer_prog_);
	
	/*TRANSFORMATION MATRICES*/
	{
		glUniformMatrix4fv(glGetUniformLocation(gbuffer_prog_, "view_xform"), 1, GL_FALSE, glm::value_ptr(view_xform));
		glUniformMatrix4fv(glGetUniformLocation(gbuffer_prog_, "projection_xform"), 1, GL_FALSE, glm::value_ptr(projection_xform));
		glUniform3fv(glGetUniformLocation(gbuffer_prog_, "camera_position"), 1, glm::value_ptr(camera_position));

		//Model Matrix
		for (const auto& instance : scene_->getAllInstances())
		{
			const MeshGL& mesh_ = sponza_mesh_[instance.getMeshId()];
			const auto& material = scene_->getMaterialById(instance.getMaterialId());

			//Get the model transformation matrix and send it to the shader
			glm::mat4 model_xform = glm::mat4((const glm::mat4x3&)instance.getTransformationMatrix());
			glUniformMatrix4fv(glGetUniformLocation(gbuffer_prog_, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));

			//Get the diffuse colour and send it to the shader
			glm::vec3 diffuse_colour = (glm::vec3 &)material.getDiffuseColour();
			glUniform3fv(glGetUniformLocation(gbuffer_prog_, "diffuse_colour"), 1, glm::value_ptr(diffuse_colour));

			//Draw
			glBindVertexArray(mesh_.vao);
			glDrawElements(GL_TRIANGLES, mesh_.element_count, GL_UNSIGNED_INT, 0);
		}
	}
}

void MyView::directionalLightPass(glm::vec3 camera_position)
{	
	//Bind the lbuffer and clear the colour
	glBindFramebuffer(GL_FRAMEBUFFER, lbuffer_fbo);
	glClear(GL_COLOR_BUFFER_BIT);

	//Configure the stencil test
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//Disable depth test and blender
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glUseProgram(direction_light_prog_);

	//Pass the gbuffer position as a texture to the shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_texture);
	glUniform1i(glGetUniformLocation(direction_light_prog_, "sampler_world_position"), 0);

	//Pass the gbuffer normal as a texture to the shader
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_texture);
	glUniform1i(glGetUniformLocation(direction_light_prog_, "sampler_world_normal"), 1);

	//Pass the gbuffer material as a texture to the shader
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_material_texture);
	glUniform1i(glGetUniformLocation(direction_light_prog_, "sampler_world_material"), 2);

	//Get directional light data
	const auto& directionalLights = scene_->getAllDirectionalLights();

	for (int i = 0; i < directionalLights.size(); ++i)
	{
		if (i > 0)
		{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);
		}

		glm::vec3 directional_light_direction = (const glm::vec3&)directionalLights[i].getDirection();
		glm::vec3 directional_light_intensity = (const glm::vec3&)directionalLights[i].getIntensity();
		glUniform3fv(glGetUniformLocation(direction_light_prog_, "directional_light_direction"), 1, glm::value_ptr(directional_light_direction));
		glUniform3fv(glGetUniformLocation(direction_light_prog_, "directional_light_intensity"), 1, glm::value_ptr(directional_light_intensity));


		//Draw a the lbuffer as a fullscreen quad
		glBindVertexArray(light_quad_mesh_.vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	glDisable(GL_BLEND);
}

void MyView::pointLightPass(glm::mat4 projection_xform, glm::mat4 view_xform, glm::vec3 camera_position)
{
	glUseProgram(point_light_prog_);

	//Enable the blender for additive lighting
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	//Pass the gbuffer position as a texture to the shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_texture);
	glUniform1i(glGetUniformLocation(point_light_prog_, "sampler_world_position"), 0);

	//Pass the gbuffer normal as a texture to the shader
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_texture);
	glUniform1i(glGetUniformLocation(point_light_prog_, "sampler_world_normal"), 1);
	
	//Pass the gbuffer material as a texture to the shader
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_material_texture);
	glUniform1i(glGetUniformLocation(point_light_prog_, "sampler_world_material"), 2);

	glUniformMatrix4fv(glGetUniformLocation(point_light_prog_, "projection_xform"), 1, GL_FALSE, glm::value_ptr(projection_xform));
	glUniformMatrix4fv(glGetUniformLocation(point_light_prog_, "view_xform"), 1, GL_FALSE, glm::value_ptr(view_xform));
	glUniform3fv(glGetUniformLocation(point_light_prog_, "camera_position"), 1, glm::value_ptr(camera_position));

	//Get point light data
	const auto& pointLights = scene_->getAllPointLights();

	for (int i = 0; i < pointLights.size(); ++i)
	{

		glm::vec3 point_light_position = (const glm::vec3&)pointLights[i].getPosition();
		float point_light_range = (const float&)pointLights[i].getRange();
		glm::vec3 point_light_intensity = (const glm::vec3&)pointLights[i].getIntensity();

		glUniform3fv(glGetUniformLocation(point_light_prog_, "point_light_position"), 1, glm::value_ptr(point_light_position));
		glUniform1f(glGetUniformLocation(point_light_prog_, "point_light_range"), point_light_range);
		glUniform3fv(glGetUniformLocation(point_light_prog_, "point_light_intensity"), 1, glm::value_ptr(point_light_intensity));

		//Create a model matrix
		glm::mat4 model_xform = glm::mat4(1);
		model_xform = glm::translate(model_xform, point_light_position);
		model_xform = glm::scale(model_xform, glm::vec3(point_light_range));

		glUniformMatrix4fv(glGetUniformLocation(point_light_prog_, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));

		//Draw the light spheres
		glBindVertexArray(light_sphere_mesh_.vao);
		glDrawElements(GL_TRIANGLES, light_sphere_mesh_.element_count, GL_UNSIGNED_INT, nullptr);
	}
}

void MyView::spotlightPass(glm::mat4 projection_xform, glm::mat4 view_xform, glm::vec3 camera_position)
{
	glUseProgram(spotlight_prog_);

	//Pass the gbuffer position as a texture to the shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_position_texture);
	glUniform1i(glGetUniformLocation(spotlight_prog_, "sampler_world_position"), 0);

	//Pass the gbuffer normal as a texture to the shader
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_normal_texture);
	glUniform1i(glGetUniformLocation(spotlight_prog_, "sampler_world_normal"), 1);

	//Pass the gbuffer material as a texture to the shader
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, gbuffer_material_texture);
	glUniform1i(glGetUniformLocation(spotlight_prog_, "sampler_world_material"), 2);

	glUniformMatrix4fv(glGetUniformLocation(spotlight_prog_, "projection_xform"), 1, GL_FALSE, glm::value_ptr(projection_xform));
	glUniformMatrix4fv(glGetUniformLocation(spotlight_prog_, "view_xform"), 1, GL_FALSE, glm::value_ptr(view_xform));
	glUniform3fv(glGetUniformLocation(spotlight_prog_, "camera_position"), 1, glm::value_ptr(camera_position));

	const auto& spotlights = scene_->getAllSpotLights();

	for (int i = 0; i < spotlights.size(); ++i)
	{
		/*if (i > spotlights.size())
		{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);
		}*/
		glm::vec3 spotlight_intensity = (const glm::vec3&)spotlights[i].getIntensity();
		glm::vec3 spotlight_direction = (const glm::vec3&)spotlights[i].getDirection();
		glm::vec3 spotlight_position = (const glm::vec3&)spotlights[i].getPosition();
		float spotlight_cone_angle = (const float&)spotlights[i].getConeAngleDegrees();
		float spotlight_range = (const float&)spotlights[i].getRange();
		bool spotlight_castshadow = (const bool&)spotlights[i].getCastShadow();
		//float spotlight_radius = spotlight_range * tanf(spotlight_cone_angle / 2);
		//glm::vec3 spotlight_pivot = glm::vec3(0, 0, -1.f);

		glUniform3fv(glGetUniformLocation(spotlight_prog_, "spotlight_intensity"), 1, glm::value_ptr(spotlight_intensity));
		glUniform3fv(glGetUniformLocation(spotlight_prog_, "spotlight_direction"), 1, glm::value_ptr(spotlight_direction));
		glUniform3fv(glGetUniformLocation(spotlight_prog_, "spotlight_position"), 1, glm::value_ptr(spotlight_position));
		glUniform1f(glGetUniformLocation(spotlight_prog_, "spotlight_cone_angle"), spotlight_cone_angle);
		glUniform1f(glGetUniformLocation(spotlight_prog_, "spotlight_range"), spotlight_range);
		glUniform1f(glGetUniformLocation(spotlight_prog_, "spotlight_castshadow"), spotlight_castshadow);

		//Create a model matrix
		//glm::mat4 model_xform = glm::mat4(1);
		//model_xform = glm::translate(model_xform, spotlight_position);
		//model_xform = glm::scale(model_xform, glm::vec3(spotlight_range));
		//model_xform = glm::inverse(glm::lookAt(spotlight_position, spotlight_position + spotlight_direction, glm::vec3(0, 1, 0)))*model_xform;

		//glm::mat4 lightProjection = glm::perspective(spotlight_cone_angle, 1.f, 1.f, 1000.f);
		//glm::mat4 lightView = glm::lookAt(spotlight_position, spotlight_position + spotlight_direction, glm::vec3(0, 1, 0));
		float spotlight_raduis = spotlight_range * glm::tan(spotlight_cone_angle / 2);
		glm::mat4 model_xform = glm::mat4();
		model_xform = glm::translate(model_xform, spotlight_position + spotlight_direction * spotlight_range);
		//model_xform *= directionLookat(glm::vec3(0, 0, -1), spotlight_direction);
		float angle = glm::dot(glm::vec3(0, 0, -1), spotlight_direction);
		glm::vec3 axis = glm::cross(glm::vec3(0, 0, -1), spotlight_direction);
		glm::quat rotation = glm::angleAxis(glm::acos(angle), axis);
		model_xform = model_xform * glm::mat4(rotation);
		model_xform = glm::scale(model_xform, glm::vec3(spotlight_raduis, spotlight_raduis, spotlight_range));


		glUniformMatrix4fv(glGetUniformLocation(spotlight_prog_, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));

		//Draw the light cones
		glBindVertexArray(light_cone_mesh_.vao);
		glDrawElements(GL_TRIANGLES, light_cone_mesh_.element_count, GL_UNSIGNED_INT, nullptr);
	}
	
}

//#ifdef TDK_NVTX
//#include<nvToolsEtx.h>
//#endif