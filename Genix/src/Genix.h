#pragma once

#include "Genix/Core/Core.h"

#include "Genix/Core/Application.h"
#include "Genix/Core/Log.h"

#include "Genix/Input/Input.h"
#include "Genix/Input/KeyCodes.h"
#include "Genix/Input/MouseCodes.h"

#include "Genix/Actors/Actor.h"
#include "Genix/Actors/CameraActor.h"
#include "Genix/Actors/StaticMeshActor.h"
#include "Genix/Actors/LightActor.h"

#include "Genix/ECS/Entity.h"
#include "Genix/ECS/EntityManager.h"

#include "Genix/ECS/Components/Camera/CameraComponent.h"
#include "Genix/ECS/Components/Camera/CameraControllerComponent.h"

#include "Genix/ECS/Components/PBRMaterialComponent.h"
#include "Genix/ECS/Components/LightComponent.h"
#include "Genix/ECS/Components/TagComponent.h"
#include "Genix/ECS/Components/TransformComponent.h"
#include "Genix/ECS/Components/StaticMeshComponent.h"

#include "Genix/Common/PrimitiveTypes.h"
#include "Genix/Common/TimeStep.h"
#include "Genix/Events/KeyEvent.h"
#include "Genix/Events/MouseEvent.h"
#include "Genix/Layer/Layer.h"

#include "Genix/Utils/PlatformUtils.h"

#include "Genix/Math/Math.h"

#include "Genix/Scene/Scene.h"
#include "Genix/Scene/SceneSerializer.h"

#include "Genix/ImGui/ImGuiLayer.h"

// ---Renderer------------------------
#include "Genix/Renderer/Renderer.h"
#include "Genix/Renderer/RenderCommand.h"

#include "Genix/Renderer/VertexBuffer.h"
#include "Genix/Renderer/FrameBuffer.h"
#include "Genix/Renderer/Mesh.h"
#include "Genix/Renderer/Shader.h"
#include "Genix/Renderer/Texture.h"
#include "Genix/Renderer/VertexArray.h"
// -----------------------------------
