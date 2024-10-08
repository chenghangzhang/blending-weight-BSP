#include "mPch.h"
#include "ShaderTemplate.h"


#define GLSL_VERSION 330

	const char* VertexShaderScript =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
		"}\0";

	const char* VertexWithNormalShaderScript =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"

		"out vec3 position;\n"
		"out vec3 normal;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"   position = vec3(model * vec4(aPos, 1.0));\n"
		"   normal = aNormal;\n"

		"   gl_Position = projection * view * vec4(position, 1.0);\n"
		"}\n\0";

	//VertexShader* GetDefaultVs()
	//{
	//	static VertexShader VsDefault = VertexShader(VsDefaultTemplate);
	//	return &VsDefault;
	//}

	//////////////////////////////////////////////////////////////////////////////

	const char* SolidFragmentShader =
		"#version 330 core\n"
		"out vec4 Color;\n"

		"uniform vec3 objectColor;\n"

		"void main()\n"
		"{\n"
		"   Color = vec4(objectColor, 1.0);\n"
		"}\n\0";


	//////////////////////////////////////////////////////////////////////////////
	const char* LitFragmentShaderScript =
		"#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 normal;\n"
		"in vec3 position;\n"
		"uniform vec3 lightPosition;\n"
		"uniform vec3 lightColor;\n"
		"uniform vec3 objectColor;\n"
		"void main()\n"
		"{\n"
		"	float ambientStrength = 0.5;\n"
		"   vec3 lightAmbient = ambientStrength * lightColor;\n"

		"   vec3 lightDiriction = normalize(lightPosition - position);\n"
		"   float diffuse = max(dot(normal, lightDiriction), 0.0);\n"
		"   vec3 lightDiffuse = diffuse * lightColor;\n"

		"   vec3 result = (lightAmbient + lightDiffuse) * objectColor;\n"
		"	color = vec4(result, 1.0);\n"
		"}\n\0";

	//FragmentShader* GetDefaultFs()
	//{
	//	static FragmentShader FsDefault = FragmentShader(FsTemplate);
	//	return &FsDefault;
	//}


