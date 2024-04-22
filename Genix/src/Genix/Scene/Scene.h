#pragma once
#include "Genix/Common/PrimitiveTypes.h"

class Framebuffer;
class TextureCube;
class Texture2D;
class Shader;
class Entity;
class UUID;
class KeyPressedEvent;
class Actor;
class CameraActor;
class CameraComponent;
class EditorCamera;
class EditorCameraActor;
class EditorCameraControllerComponent;
class Event;
class TimeStep;

enum class SceneState
{
	Edit = 0, Play = 1, Simulate = 2
};

class Scene
{
public:
	Scene();
	~Scene();

	void DestroyLevel();
	
	static Ref<Scene> Copy(const Ref<Scene>& other);
	
	template<typename T, typename... Args>
	Ref<T> CreateActor(Args&&... args)
	{
		Ref<T> actor = CreateRef<T>(std::forward<Args>(args)...);
		m_Actors.push_back(actor);
		return actor;
	}

	Ref<Actor> GetActorForEditor(const uint32& value) const;
	std::vector<Ref<Actor>> GetActors() const { return m_Actors; }

	SceneState GetSceneState() const { return m_SceneState; }
	
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	
	void OnEvent(Event& e);
	void OnResize();
	void OnResize(uint32 width, uint32 height);

	void OnScenePlay();
	void OnSceneSimulate();
	void OnSceneStop();
	
	void OnUpdate(TimeStep deltaTime);

	void DestroyActor(Ref<Actor> actor);
	Ref<Actor> DuplicateActor(const Ref<Actor>& actor);

	bool IsRunning() const { return m_IsRunning; }
	bool IsPaused() const { return m_IsPaused; }

	void SetPaused(bool paused) { m_IsPaused = paused; }

	void Step(int frames = 1);

	Ref<Entity> GetCamera();
	Ref<Framebuffer> GetFrameBuffer() const { return m_Framebuffer; }
	
private:
	friend class SceneSerializer;

	void OnPhysicsStart();
	void OnPhysicsStop();

private:
	SceneState m_SceneState = SceneState::Edit;

	EditorCameraActor* m_EditorCamera;
	
	Ref<CameraActor> m_RuntimeCamera;
	std::vector<Ref<Actor>> m_Actors;

	bool m_IsRunning = false;
	bool m_IsPaused = false;
	uint32 m_StepFrames = 0;
	
	uint32 m_ViewportWidth;
	uint32 m_ViewportHeight;

	Ref<Framebuffer> m_Framebuffer;

	Ref<TextureCube> m_EnvCubeMap;
	Ref<TextureCube> m_IrradianceMap;
	Ref<TextureCube> m_PrefilterMap;
	Ref<Texture2D> m_BrdfLUTTexture;

	Ref<Shader> m_BRDFShader;
	Ref<Shader> m_BackgroundShader;
	Ref<Shader> m_IrradianceShader;
	Ref<Shader> m_PrefilterShader;
	Ref<Shader> m_EquirectangularToCubemapShader;
};
