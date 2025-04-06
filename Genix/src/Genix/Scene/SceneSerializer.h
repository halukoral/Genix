#pragma once

class Scene;

class SceneSerializer
{
public:
	SceneSerializer(const Ref<Scene>& scene);

	void Serialize(const std::string& filepath) const;
	void SerializeRuntime(const std::string& filepath);

	bool Deserialize(const std::string& filepath) const;
	bool DeserializeRuntime(const std::string& filepath);
private:
	Ref<Scene> m_Scene;
};
