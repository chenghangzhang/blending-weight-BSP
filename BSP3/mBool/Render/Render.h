#pragma once

//////////////////////////////////////////////////////////////////////////
//Packages 中的库
#include "../OpenGL/GLAD/include/glad.h"
#include "../OpenGL/GLFW/include/glfw3.h"

#include "../OpenGL/GLM/glm.hpp"
#include "../OpenGL/GLM/gtc/matrix_transform.hpp"
#include "../OpenGL/GLM/gtc/type_ptr.hpp"

//////////////////////////////////////////////////////////////////////////
//本项目定义的类或功能
#include "Shader/Base/ShaderTemplate.h"
#include "Shader/Base/VertexShader.h"
#include "Shader/Base/FragmentShader.h"
#include "Shader/Base/Shader.h"

#include "Shader/SolidShader/SolidShader.h"
#include "Shader/LitShader/LitShader.h"

#include "Pipline/RenderParas.h"
#include "Pipline/RenderScence.h"

#include "Pipline/Renderer/Base/UnitRenderer.h"
#include "Pipline/Renderer/Base/SingleRenderer.hpp"
#include "Pipline/Renderer/Base/CompositeRenderer.h"

//#include "DataBind/DataBind.h"
#include "DataBind/DataBinder.h"

//#include "Pipline/Renderer/ComponentRenderer/VerticesRenderer/VerticesRenderer.h"
//#include "Pipline/Renderer/ComponentRenderer/EdgesRenderer/EdgesRenderer.h"
//#include "Pipline/Renderer/ComponentRenderer/TrianglesRenderer/TrianglesRenderer.h"

#include "Pipline/Renderer/ComponentRenderer/VerticesRenderer/VerticesRenderer.hpp"
#include "Pipline/Renderer/ComponentRenderer/EdgesRenderer/EdgesRenderer.hpp"
#include "Pipline/Renderer/ComponentRenderer/TrianglesRenderer/TrianglesRenderer.hpp"

#include "Pipline/Renderer/EdgeSetRenderer/EdgeSetRenderer.h"

#include "Pipline/Renderer/MeshRenderer/Mesh3dRenderer.hpp"
//
//#include "Pipline/RenderUnit/RenderMesh/RenderMeshNormal.h"



#include "Camera/Camera.h"

#include "Light/Light.h"

#include "Wnd/RenderWnd.h"

//#include "Renderer/Geometry/2d/SplineRenderer.h"
//#include "Renderer/Geometry/3d/PlaneRenderer.h"

