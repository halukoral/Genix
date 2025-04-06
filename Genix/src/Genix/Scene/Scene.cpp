#include "gxpch.h"
#include "Genix/Scene/Scene.h"

#include "Genix/Actors/CameraActor.h"
#include "Genix/Actors/EditorCameraActor.h"
#include "Genix/Common/TimeStep.h"
#include "Genix/Core/Application.h"

#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"
#include "Genix/ECS/Components/TagComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/ECS/Systems/RenderingManager.h"
#include "Genix/Renderer/FrameBuffer.h"
#include "Genix/Renderer/RenderCommand.h"

#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/Shader.h"
#include "Genix/Renderer/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

template<typename Component>
static void CopyComponent(const Ref<Entity>& dst, const Ref<Entity>& src)
{
	dst->AddComponent<Component>(*(src->GetComponent<Component>()));
}

template<typename Component>
static void CopyComponentIfExists(const Ref<Entity>& dst, const Ref<Entity>& src)
{
	if (src->HasComponent<Component>())
	{
		dst->AddOrReplaceComponent<Component>(*(src->GetComponent<Component>()));
	}
}

Scene::Scene()
{
	// Camera
	m_EditorCamera = EditorCameraActor::Get();
	m_EditorCamera->GetEntity()->GetComponent<TransformComponent>()->Position = {0.f, 0.f, 5.f};
	m_RuntimeCamera = CreateRef<CameraActor>("Runtime Camera Actor");

	// FrameBuffer
	FramebufferSpecification fbSpec;
	fbSpec.Attachments =
	{
		FramebufferTextureFormat::RGBA8,
		FramebufferTextureFormat::RED_INTEGER,
		FramebufferTextureFormat::Depth
	};
	fbSpec.Width = 2560;
	fbSpec.Height = 1440;
	m_Framebuffer = Framebuffer::Create(fbSpec);

	//Shaders
	m_BRDFShader = Shader::Create(
		"Assets/Shaders/BRDF.vert",
		"Assets/Shaders/BRDF.frag"
	);
	
	m_BackgroundShader = Shader::Create(
		"Assets/Shaders/Background.vert",
		"Assets/Shaders/Background.frag"
	);

	m_IrradianceShader = Shader::Create(
		"Assets/Shaders/Cubemap.vert",
		"Assets/Shaders/IrradianceConvolution.frag"
	);

	m_PrefilterShader = Shader::Create(
		"Assets/Shaders/Cubemap.vert",
		"Assets/Shaders/Prefilter.frag"
	);

	m_EquirectangularToCubemapShader = Shader::Create(
		"Assets/Shaders/Cubemap.vert",
		"Assets/Shaders/EquirectangularToCubemap.frag"
	 );
	
	m_BackgroundShader->Bind();
	m_BackgroundShader->GLSetUniform_Int("u_EnvironmentMap", 0);


    // pbr: setup framebuffer for environment
    // --------------------------------------
	// fbSpec.Attachments = { FramebufferTextureFormat::Depth };
	// fbSpec.Width = 512;
	// fbSpec.Height = 512;
	//m_Framebuffer = Framebuffer::Create(fbSpec);
    unsigned int captureFBO;
    glGenFramebuffers(1, &captureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    unsigned int captureRBO;
	glGenRenderbuffers(1, &captureRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // pbr: load the HDR environment map
    // ---------------------------------
	Ref<Texture2D> hdrTexture = Texture2D::Create("Assets/Textures/HDR/environment.hdr");

    // pbr: setup cube map to render to and attach to framebuffer
    // ---------------------------------------------------------
	TextureProperties properties;
	properties.Format = TextureFormat::RGB;
	properties.InternalFormat = TextureFormat::RGB16F;
	properties.Width = 512;
	properties.Height = 512;
	m_EnvCubeMap = TextureCube::Create(properties);

	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    m_EquirectangularToCubemapShader->Bind();
    m_EquirectangularToCubemapShader->GLSetUniform_Int("u_EquirectangularMap", 0);
    m_EquirectangularToCubemapShader->GLSetUniform_Mat4("u_Projection", captureProjection);
	RenderCommand::SetActiveTexture(0);
	hdrTexture->Bind(0);

	// don't forget to configure the viewport to the capture dimensions.
	RenderCommand::SetViewport(0, 0, 512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_EquirectangularToCubemapShader->GLSetUniform_Mat4("u_View", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubeMap->GetId(), 0);
        RenderCommand::Clear();
    	RenderCommand::RenderCube();
    }
    RenderCommand::ResetFrameBuffer();

    // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
	m_EnvCubeMap->Bind(0);
	m_EnvCubeMap->GenerateMipMap();

    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------	
	properties.Format = TextureFormat::RGB;
	properties.InternalFormat = TextureFormat::RGB16F;
	properties.Width = 32;
	properties.Height = 32;
	m_IrradianceMap = TextureCube::Create(properties);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    m_IrradianceShader->Bind();
    m_IrradianceShader->GLSetUniform_Int("u_EnvironmentMap", 0);
    m_IrradianceShader->GLSetUniform_Mat4("u_Projection", captureProjection);
	RenderCommand::SetActiveTexture(0);
	m_EnvCubeMap->Bind(0);

	// don't forget to configure the viewport to the capture dimensions.
	RenderCommand::SetViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_IrradianceShader->GLSetUniform_Mat4("u_View", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap->GetId(), 0);
		RenderCommand::Clear();
    	RenderCommand::RenderCube();
    }
    RenderCommand::ResetFrameBuffer();

    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    // --------------------------------------------------------------------------------
	properties.Format = TextureFormat::RGB;
	properties.InternalFormat = TextureFormat::RGB16F;
	properties.Width = 128;
	properties.Height = 128;
	m_PrefilterMap = TextureCube::Create(properties);

    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    // ----------------------------------------------------------------------------------------------------
    m_PrefilterShader->Bind();
    m_PrefilterShader->GLSetUniform_Int("u_EnvironmentMap", 0);
    m_PrefilterShader->GLSetUniform_Mat4("u_Projection", captureProjection);
	RenderCommand::SetActiveTexture(0);
	m_EnvCubeMap->Bind(0);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        auto mipWidth  = (GLsizei)(128 * std::pow(0.5, mip));
        auto mipHeight = (GLsizei)(128 * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);

    	// don't forget to configure the viewport to the capture dimensions.
    	RenderCommand::SetViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        m_PrefilterShader->GLSetUniform_Float("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_PrefilterShader->GLSetUniform_Mat4("u_View", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap->GetId(), mip);

        	RenderCommand::Clear();
    		RenderCommand::RenderCube();
        }
    }
    RenderCommand::ResetFrameBuffer();

    // pbr: generate a 2D LUT from the BRDF equations used.
    // ----------------------------------------------------
	properties.Format = TextureFormat::RG;
	properties.InternalFormat = TextureFormat::RG16F;
	properties.Width = 512;
	properties.Height = 512;
	m_BrdfLUTTexture = Texture2D::Create(properties);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BrdfLUTTexture->GetId(), 0);
	
	// don't forget to configure the viewport to the capture dimensions.
	RenderCommand::SetViewport(0, 0, 512, 512);
    m_BRDFShader->Bind();
	RenderCommand::Clear();

	RenderCommand::SetBlendFunc(BlendFuncArgument::OneMinusSrcAlpha, BlendFuncArgument::SrcAlpha);
	RenderCommand::RenderQuad();
	RenderCommand::SetBlendFunc(BlendFuncArgument::SrcAlpha, BlendFuncArgument::OneMinusSrcAlpha);

    RenderCommand::ResetFrameBuffer();

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    m_BackgroundShader->Bind();
    m_BackgroundShader->GLSetUniform_Mat4("u_Projection", GetCamera()->GetComponent<CameraComponent>()->GetProjectionMatrix(CameraType::Perspective));

	const FramebufferSpecification spec = GetFrameBuffer()->GetSpecification();
	m_Framebuffer->Resize(spec.Width, spec.Height);
	OnResize(spec.Width, spec.Height);
}

void Scene::OnUpdate(TimeStep deltaTime)
{
	m_Framebuffer->Bind();
	
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	// Clear our entity ID attachment to -1
	m_Framebuffer->ClearAttachment(1, -1);
	Renderer::BeginScene(GetCamera());

	// bind pre-computed IBL data
	RenderCommand::SetActiveTexture(0);
	m_IrradianceMap->Bind(0);

	RenderCommand::SetActiveTexture(1);
	m_PrefilterMap->Bind(1);

	RenderCommand::SetActiveTexture(2);
	m_BrdfLUTTexture->Bind(2);
	
	if (!m_IsPaused || m_StepFrames-- > 0)
	{
		// Physics	
	}
	
	if (m_SceneState == SceneState::Edit)
	{
		RenderingManager::Update();
		m_EditorCamera->OnUpdate(deltaTime);
	}
	else if (m_SceneState == SceneState::Play)
	{
		RenderingManager::Update();
		m_RuntimeCamera->OnUpdate(deltaTime);
	}
	else if (m_SceneState == SceneState::Simulate)
	{
		// Physics update
	}

	// render skybox (render as last to prevent overdraw)
	m_BackgroundShader->Bind();
	m_BackgroundShader->GLSetUniform_Mat4("u_View", GetCamera()->GetComponent<CameraComponent>()->GetViewMatrix());

	RenderCommand::SetActiveTexture(0);
	m_IrradianceMap->Bind(0);
	//m_EnvCubeMap->Bind(0);
	//m_PrefilterMap->Bind(0);
	RenderCommand::RenderCube();
	Renderer::EndScene();
	
	m_Framebuffer->Unbind();
}

Scene::~Scene()
{
	DestroyLevel();
}

void Scene::DestroyLevel()
{
	m_RuntimeCamera.reset();
	for ( auto actor : m_Actors)
	{
		actor.reset();
	}
	m_Actors.clear();
}

Ref<Scene> Scene::Copy(const Ref<Scene>& other)
{
	Ref<Scene> newScene = CreateRef<Scene>();
	newScene->m_SceneState = other->m_SceneState;
	newScene->m_ViewportWidth = other->m_ViewportWidth;
	newScene->m_ViewportHeight = other->m_ViewportHeight;
	*newScene->m_RuntimeCamera = *other->m_RuntimeCamera;

	for (const auto& actor : other->m_Actors)
	{
		const Ref<Actor> newActor = newScene->CreateActor<Actor>(actor->GetEntity()->GetUUID(), actor->GetEntity()->GetName() + "_Runtime");
		newActor->GetEntity()->SetIdForMousePicking(actor->GetEntity()->GetIdForMousePicking());
		
		CopyComponentIfExists<CameraComponent>(newActor->GetEntity(), actor->GetEntity());
		CopyComponentIfExists<CameraControllerComponent>(newActor->GetEntity(), actor->GetEntity());
		CopyComponentIfExists<StaticMeshComponent>(newActor->GetEntity(), actor->GetEntity());		
		CopyComponentIfExists<TagComponent>(newActor->GetEntity(), actor->GetEntity());
		CopyComponentIfExists<TransformComponent>(newActor->GetEntity(), actor->GetEntity());
	}
	return newScene;
}

Ref<Actor> Scene::GetActorForEditor(const uint32& value) const
{
	for(const auto& actor : m_Actors)
	{
		if (actor->GetEntity()->GetIdForMousePicking() == value)
		{
			return actor;
		}
	}
	return nullptr;
}

glm::mat4 Scene::GetViewMatrix() const
{
	if (m_SceneState == SceneState::Edit)
	{
		return m_EditorCamera->GetViewMatrix();
	}
	else
	{
		return m_RuntimeCamera->GetViewMatrix();
	}
}

glm::mat4 Scene::GetProjectionMatrix() const
{
	if (m_SceneState == SceneState::Edit)
	{
		return m_EditorCamera->GetProjectionMatrix(CameraType::Perspective);
	}
	else
	{
		return m_RuntimeCamera->GetProjectionMatrix(CameraType::Perspective);
	}
}

void Scene::OnEvent(Event& e)
{
	if (m_SceneState == SceneState::Edit)
	{
		m_EditorCamera->OnEvent(e);
	}
	else
	{
		m_RuntimeCamera->OnEvent(e);
	}
}

void Scene::OnResize()
{
	if (m_SceneState == SceneState::Edit)
	{
		m_EditorCamera->OnResize(m_ViewportWidth,m_ViewportHeight);
	}
	else
	{
		m_RuntimeCamera->OnResize(m_ViewportWidth,m_ViewportHeight);
	}
}

void Scene::OnResize(const uint32 width, const uint32 height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;
	
	if (m_SceneState == SceneState::Edit)
	{
		m_EditorCamera->OnResize(width,height);
	}
	else
	{
		m_RuntimeCamera->OnResize(width,height);
	}
}

void Scene::OnScenePlay()
{
	m_IsRunning = true;
	m_SceneState = SceneState::Play;
	Application::Get().GetWindow().DisableCursor();
}

void Scene::OnSceneSimulate()
{
	m_SceneState = SceneState::Simulate;
	OnPhysicsStart();
}

void Scene::OnSceneStop()
{
	m_IsRunning = false;
	m_SceneState = SceneState::Edit;
	Application::Get().GetWindow().EnableCursor();
}

void Scene::DestroyActor(Ref<Actor> actor)
{
	auto it = std::find_if(
		m_Actors.begin(),
		m_Actors.end(),
		[actor](const Ref<Actor>& act) -> bool { return  act->GetEntity()->GetUUID() == actor->GetEntity()->GetUUID(); });

	if (it != m_Actors.end())
	{
		EntityManager::Get()->RemoveEntity((*it)->GetEntity()->GetUUID());
		m_Actors.erase(it);
	}
}

Ref<Actor> Scene::DuplicateActor(const Ref<Actor>& actor)
{
	std::string name = actor->GetEntity()->GetName();
	const Ref<Actor> newActor = CreateActor<Actor>(name);
	
	CopyComponentIfExists<CameraComponent>(newActor->GetEntity(), actor->GetEntity());
	CopyComponentIfExists<CameraControllerComponent>(newActor->GetEntity(), actor->GetEntity());
	CopyComponentIfExists<StaticMeshComponent>(newActor->GetEntity(), actor->GetEntity());
	CopyComponentIfExists<TagComponent>(newActor->GetEntity(), actor->GetEntity());
	CopyComponentIfExists<TransformComponent>(newActor->GetEntity(), actor->GetEntity());

	return newActor;
}

void Scene::Step(int frames)
{
	m_StepFrames = frames;
}

Ref<Entity> Scene::GetCamera()
{
	if (m_SceneState == SceneState::Edit)
	{
		return m_EditorCamera->GetEntity();
	}
	else if (m_SceneState == SceneState::Play)
	{
		return m_RuntimeCamera->GetEntity();
	}
	return {};
}

void Scene::OnPhysicsStart()
{
}

void Scene::OnPhysicsStop()
{
}
