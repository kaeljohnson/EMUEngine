#pragma once

#include "box2d/box2d.h"
#include "../../include/TileMap/TileMap.h"
#include "../../include/Physics/Physics.h"
#include "../../include/Scenes/Scene.h"
#include "../../Public/Logger.h"
#include "../../Public/MathUtil.h"
#include "../../include/ECS/ECS.h"
#include "../../Public/EMUTime.h"
#include "../../include/AppState.h"

#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace fs = std::filesystem;

static json rulesJson; // Only one rules file per game for now so this will work.
static json sceneRules;
static json worldLayers;
static json sceneAssets;
static json componentTemplates;
static json characterRules;

namespace Engine
{
	// j must out live the pointer returned.
	static const json* getJson(const json& j, const std::string& key)
	{
		auto it = j.find(key);
		if (it == j.end())
		{
			return nullptr;
		}
		return &(*it);
	}

	template<typename T>
	static Math2D::Point2D<T> ExtractPoint2DFromJSON(const json& j, const std::string& key, Math2D::Point2D<T> ioVec)
	{
		if (!j.contains(key))
		{
			ENGINE_WARN("Field Not Found: {}", key);
			return ioVec;
		}

		const auto& arr = j.at(key);
		if (!arr.is_array() || arr.size() != 2) return ioVec;

		if constexpr (std::is_floating_point_v<T>)
		{
			if (arr[0].is_number()) ioVec.X = static_cast<T>(arr[0].get<double>());
			if (arr[1].is_number()) ioVec.Y = static_cast<T>(arr[1].get<double>());
		}
		else if constexpr (std::is_integral_v<T>)
		{
			if (arr[0].is_number_integer()) ioVec.X = static_cast<T>(arr[0].get<long long>());
			else if (arr[0].is_number()) ioVec.X = static_cast<T>(arr[0].get<long long>());
			if (arr[1].is_number_integer()) ioVec.Y = static_cast<T>(arr[1].get<long long>());
			else if (arr[1].is_number()) ioVec.Y = static_cast<T>(arr[1].get<long long>());
		}
		return ioVec;
	}

	static size_t ExtractSizeTFromJSON(const json& j, const std::string& key, size_t defaultValue)
	{
		if (!j.contains(key))
		{
			ENGINE_WARN("Field Not Found: {}", key);
			return defaultValue;
		}

		const auto& value = j.at(key);
		if (value.is_number_unsigned()) return value.get<size_t>();
		else if (value.is_number_integer())
		{
			int intValue = value.get<int>();
			if (intValue >= 0) return static_cast<size_t>(intValue);
		}
		else if (value.is_number())
		{
			double doubleValue = value.get<double>();
			if (doubleValue >= 0.0) return static_cast<size_t>(doubleValue);
		}
		return defaultValue;
	}

	static std::array<int, 3> ExtractColorArrayFromJSON(const json& j, const std::string& key, std::array<int, 3> defaultValue)
	{
		if (!j.contains(key))
		{
			ENGINE_ERROR("Invalid Rules File. Field Not Found: {}.", key);
			std::exit(1);
		}
		const auto& arr = j.at(key);
		if (!arr.is_array() || arr.size() != 3) return defaultValue;
		std::array<int, 3> colorArray = defaultValue;
		for (size_t i = 0; i < 3; ++i)
		{
			if (arr[i].is_number_unsigned()) colorArray[i] = arr[i].get<int>();
			else if (arr[i].is_number_integer())
			{
				int intValue = arr[i].get<int>();
				if (intValue >= 0) colorArray[i] = static_cast<int>(intValue);
			}
			else if (arr[i].is_number())
			{
				double doubleValue = arr[i].get<double>();
				if (doubleValue >= 0.0) colorArray[i] = static_cast<int>(doubleValue);
			}
		}
		return colorArray;
	}
	static std::string ExtractStringFromJSON(const json& j, const std::string& key, const std::string& defaultValue)
	{
		if (!j.contains(key))
		{
			ENGINE_ERROR("Invalid Rules File. Field Not Found: {}.", key);
			std::exit(1);
		}

		const auto& value = j.at(key);

		if (!value.is_string())
		{
			ENGINE_ERROR("Invalid Rules File. Field '{}' is not a string.", key);
			return defaultValue;
		}

		return value.get<std::string>();
	}

	Scene::Scene(std::string rulesFileName, ECS& refECS, AssetManager& refAssetManager, IOEventSystem& refIOEventSystem)
		: m_refECS(refECS), m_rulesFileName(rulesFileName),
		m_physicsSimulation(refECS), m_refAssetManager(refAssetManager), m_refIOEventSystem(refIOEventSystem),
		m_cameraSystem(refECS)
	{
		m_entities.reserve(50000);

		// Open and parse the rules file
		std::ifstream inFile(m_rulesFileName);
		if (!inFile.is_open())
		{
			ENGINE_ERROR("Failed to open rules file.");
			std::exit(1);
		}

		try
		{
			inFile >> rulesJson;
		}
		catch (const json::parse_error& e)
		{
			ENGINE_ERROR("Failed to parse rules JSON: {}", e.what());
			std::exit(1);
		}

		auto& sceneName = rulesJson.begin().key();
		ENGINE_LOG_D("Loading audio files for scene: {}", sceneName);

		sceneRules = *getJson(rulesJson, sceneName);

		worldLayers = *getJson(sceneRules, "WorldLayers");

		sceneAssets = *getJson(sceneRules, "Assets");

		componentTemplates = *getJson(sceneRules, "ComponentTemplates");

		characterRules = *getJson(sceneRules, "CharacterRules");

		// TEMP
		const json* physicsLayer = getJson(worldLayers, "Physics");

		if (physicsLayer == nullptr)
		{
			ENGINE_ERROR("Invalid Rules File. No Physics Layer Found.");
			std::exit(1);
		}

		for (auto& [key, value] : physicsLayer->items())
		{
			ENGINE_CRITICAL_D("Physics Layer Properties: {}", key);
		}

		const std::string tileMapFileName = ExtractStringFromJSON(*physicsLayer, "TileMapPath", "");
		if (tileMapFileName != "")
		{
			ENGINE_INFO("Tilemap found.");
			// no tile map in scene.
			AddTileMaps();
		}
	}

	Scene::~Scene()
	{
		m_physicsSimulation.Cleanup();
	}
	
	void Scene::RegisterContactCallback(ContactType contactType, const size_t entityA, const size_t entityB, ContactCallback callback)
	{
		m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, entityA, entityB, callback);
	}

	void Scene::RegisterContactCallback(ContactType contactType, const size_t entity, ContactCallback callback)
	{
		m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, entity, callback);
	}

	void Scene::RegisterOnScenePlayEvent(std::function<void()> func)
	{
		// Register a function to be called when the scene starts playing.
		// This can be used to initialize things that need to be set up when the scene starts.
		m_clientOnScenePlayEvents.push_back(std::move(func));
	}

	void Scene::RegisterOnSceneEndEvent(std::function<void()> func)
	{
		m_clientOnSceneEndEvents.push_back(std::move(func));
	}

	void Scene::OnScenePlay()
	{
		// Order matters here.

		// 1. Create the world.
		m_physicsSimulation.CreateWorld();

		// 2. Load audio files for the scene.
		loadAudioFiles();

		// 3. Load the entities associated with the characters in the tile map.
		//	  Adds components defined in the rules file and adds them to the ECS.
		//    This function also activates the entites in the ECS.
		loadSceneEntitiesFromTileMap();

		// 4. Activate entities with camera first.
		for (auto& pair : m_cameraOrder)
		{
			Activate(pair.second);
		}

		// 5. Activate the rest of the entities
		for (auto& pair : m_entities)
		{
			if (pair.second == true) // entity active on start.
				Activate(pair.first);
		}

		// 6. Frame the cameras
		m_cameraSystem.Frame(Math2D::Point2D<int>(141, 22)); // TEMP
		
		// 7. Physics bodies need to be added to the world after they are activated and pooled.
		m_physicsSimulation.AddPhysicsBodiesToWorld(m_entities);
		m_physicsSimulation.AddChainCollidersToWorld();

		// 8. Contact callbacks need to be activated.
		m_physicsSimulation.ActivateContactCallbacks();

		// 9. Deactivate all components that should not be active at the start of the scene?

		// process items client wants to do.
		for (auto& func : m_clientOnScenePlayEvents)
		{
			func();
		}

		AppState::IN_SCENE = true;
	}

	void Scene::OnSceneEnd()
	{
		// Could be problematic if this is called mid frame.
		AppState::IN_SCENE = false;

		// Call client defined OnSceneEndEvents.
		for (auto& func : m_clientOnSceneEndEvents)
		{
			func();
		}

		m_physicsSimulation.Cleanup();

		// Should unload assets here to potentially be reloaded next scene?
		// Or should there be a more detailed check so assets that might transfer
		// to next scene are not unloaded? This takes awhile.
		m_refAssetManager.UnloadTextures();
		for (auto& animations : m_refECS.GetHotComponents<Sprite>())
		{
			// Free texture pointer associated with sprite.
			animations.m_ptrLoadedTexture = nullptr;
		}

		for (auto& eventType : m_sceneRuntimeIOEvents)
		{
			m_refIOEventSystem.UnRegisterIOEventListener(eventType);
		}

		m_refAssetManager.UnloadSounds();

		// Deactivate all entities and destroy all components.
		m_refECS.DeactivateEntities();

		for (auto& pair : m_entities)
			m_refECS.DestroyComponents(pair.first);
	}

	void Scene::AddTileMaps()
	{
		// count up all the layers by iterating throught the items in the world layers then create
		// a tile map for each layer.
		for (auto& [layerName, layerJson] : worldLayers.items())
		{
			const int layerId = layerJson["id"].get<int>();

			const float parallaxFactor =
				layerJson.contains("ParallaxFactor")
				? static_cast<float>(layerJson["ParallaxFactor"].get<double>())
				: 1.0f;

			const std::string tileMapPath =
				layerJson.contains("TileMapPath")
				? ExtractStringFromJSON(layerJson, "TileMapPath", "")
				: "";

			bool hasTileMap = !tileMapPath.empty();

			auto [it, inserted] = m_layers.emplace(
				layerId,
				Layer{ hasTileMap, TileMap(&m_refECS), parallaxFactor }
			);

			if (!hasTileMap)
				continue;

			Layer& layer = it->second;

			layer.m_tileMap.CreateMap(tileMapPath);

			m_physicsSimulation.AddPhysicsTileMap(&layer.m_tileMap);

			for (auto& [coords, info] : layer.m_tileMap.GetMap())
			{
				add(info.first);
			}
		}
	}

	void Scene::add(Entity entity)
	{
		if (m_entities.contains(entity))
		{
			ENGINE_LOG("Entity already exists in the scene: {}", entity);
			return;
		}

		m_entities.emplace(entity, true);
	}

	void Scene::Activate(Entity entity)
	{
		if (!m_entities.contains(entity))
		{
			ENGINE_LOG("Entity does not exist in the current scene: {}", entity);
			return;
		}

		m_refECS.Activate(entity);

		activatePhysics(entity);
	}

	void Scene::activatePhysics(Entity entity)
	{
		m_physicsSimulation.ActivateBody(entity);
		m_physicsSimulation.ActivateChains(entity);
	}

	void Scene::Deactivate(Entity entity)
	{
		if (!m_entities.contains(entity))
		{
			ENGINE_LOG("Entity does not exist in the current scene: {}", entity);
			return;
		}

		// @todo temp solution to avoid deactivating entities with cameras. 
		// Real solution might include making cameras separate from components.
		if (m_refECS.HasComponent<Camera>(entity))
		{
			ENGINE_CRITICAL("Cannot deactivate entity with camera component: {}. Consider creating a separate entity which has the camera attached and follows the entity you are trying to deactivate.", entity);
			return;
		}

		deactivatePhysics(entity);

		m_refECS.Deactivate(entity);
	}

	void Scene::deactivatePhysics(Entity entity)
	{
		m_physicsSimulation.DeactivateBody(entity);
		m_physicsSimulation.DeactivateChains(entity);
	}

	//void Scene::Remove(Entity entity)
	//{
	//	// Remove entity from the scene. Do not remove the entity from the ECS, just deactivate it.
	//	m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
	//	m_refECS.Deactivate(entity);
	//}

	void Scene::UpdatePhysics()
	{
		m_physicsSimulation.Update();
	}

	void Scene::UpdateCamera(AssetManager& refAssetManager)
	{
		m_cameraSystem.Update(refAssetManager);
	}

	void Scene::AddIOEvent(IOEventType type)
	{
		if (m_sceneRuntimeIOEvents.contains(type) != 0)
		{
			ENGINE_WARN("Event of type {} is already associated with a client callback function in scene.", static_cast<int>(type));
			return;
		}

		m_sceneRuntimeIOEvents.insert(type);
	}

	void Scene::RemoveIOEvent(IOEventType type)
	{
		if (m_sceneRuntimeIOEvents.contains(type) == 0)
		{
			ENGINE_INFO("Event of type {} is not present in current runtime io events.");
			return;
		}

		m_sceneRuntimeIOEvents.erase(type);
	}

	void Scene::SetGravity(const Math2D::Point2D<float> gravity)
	{
		ENGINE_INFO_D("Setting gravity: {}, {}", gravity.X, gravity.Y);

		m_physicsSimulation.UpdateGravity(gravity);
	}

	const Entity Scene::GetTileMapEntity(size_t tileId) const
	{
		return m_layers.at(1).m_tileMap.GetEntity(tileId);
	}

	void Scene::loadAudioFiles()
	{
		const json* assetsJson = getJson(sceneRules, "Assets");
		if (!assetsJson)
		{
			ENGINE_CRITICAL("Assets section not found in rules file. Continuing without.");
			return;
		}

		const json* audioJson = getJson(*assetsJson, "Audio");
		if (!audioJson)
		{
			ENGINE_CRITICAL("Audio section not found in rules file. Continuing without.");
			return;
		}

		const json* audioFilePathJson = getJson(*audioJson, "PathToAudioFiles");
		if (!audioFilePathJson)
		{
			ENGINE_ERROR("PathToAudioFiles not found in rules file.");
			std::exit(1);
		}

		std::string audioFilePath = audioFilePathJson->get<std::string>();

		const json* soundsJson = getJson(*audioJson, "Sounds");
		if (!soundsJson)
		{
			ENGINE_ERROR("Sounds section not found in rules file.");
			std::exit(1);
		}

		json j = json::parse(soundsJson->dump());

		m_refAssetManager.PrepareSoundStorage(j.size());

		for (auto& [file, idx] : j.items())
		{
			int idxInt = idx.get<int>();
			std::string fullPath = audioFilePath + file;
			m_refAssetManager.LoadSound(idxInt, fullPath);
		}
	}

	static void addTransformComponent(ECS& refECS, Entity entity, int layer, float parallaxFactor, const json& transformTemplates, std::string templateKey, int x, int y, size_t numUnitsPerTile)
	{
		const json* entityTransformTemplate = getJson(transformTemplates, templateKey);

		bool drawDebug = entityTransformTemplate->contains("DrawDebug");
		std::string debugColor = entityTransformTemplate->value("DrawDebug", "red");

		DebugColor debugColorEnum;

		if (debugColor == "green")
		{
			debugColorEnum = DebugColor::Green;
		}
		else if (debugColor == "blue")
		{
			debugColorEnum = DebugColor::Blue;
		}
		else if (debugColor == "black")
		{
			debugColorEnum = DebugColor::Black;
		}
		else
		{
			debugColorEnum = DebugColor::Red;
		}

		refECS.AddComponent<Transform>(
			entity,
			Math2D::Point2D<float>(x * (float)numUnitsPerTile, y * (float)numUnitsPerTile),
			1.0f, 1, layer, parallaxFactor, drawDebug, debugColorEnum
		);
	}

	static std::pair<size_t, Entity> addCameraComponent(ECS& refECS, Entity entity, const json& cameraTemplate, const std::string& cameraTemplateKey, const size_t numLayers)
	{
		const json* entityCameraTemplate = getJson(cameraTemplate, cameraTemplateKey);

		Math2D::Point2D<float> size = { 0.0f, 0.0f }; // size determined by engine 
		size_t pixelsPerUnit = ExtractSizeTFromJSON(*entityCameraTemplate, "PixelsPerUnit", 0);

		bool clampingOn = entityCameraTemplate->contains("ClampingOn") ? getJson(*entityCameraTemplate, "ClampingOn")->get<bool>() : false;

		bool borderOn = entityCameraTemplate->contains("border") ? getJson(*entityCameraTemplate, "border")->get<bool>() : false;
		
		size_t order = entityCameraTemplate->contains("order") ? getJson(*entityCameraTemplate, "order")->get<size_t>() : 1;

		std::array<int, 3> backgroundColor = ExtractColorArrayFromJSON(*entityCameraTemplate, "backgroundColor", { 0, 0, 0 });
		ENGINE_CRITICAL_D("Camera Background Color RGB: {}, {}, {}", backgroundColor[0], backgroundColor[1], backgroundColor[2]);

		Math2D::Point2D<float> screenRatio = { 1.0f, 1.0f };
		Math2D::Point2D<float> position = { 0.0f, 0.0f };

		if (const json* windowJson = getJson(*entityCameraTemplate, "Viewport"))
		{
			position.X = windowJson->value("X", 0.0f);
			position.Y = windowJson->value("Y", 0.0f);
			screenRatio.X = windowJson->value("Width", 1.0f);
			screenRatio.Y = windowJson->value("Height", 1.0f);
		}

		ENGINE_INFO_D("Adding camera component with pixelsPerUnit: {}, Position: {}x{}, ScreenRatio: {}x{}", pixelsPerUnit, position.X, position.Y, screenRatio.X, screenRatio.Y);

		refECS.AddComponent<Camera>(entity, size, screenRatio, position, pixelsPerUnit, clampingOn, borderOn, backgroundColor, numLayers);

		return std::make_pair(order, entity);
	}

	static void createEdge(TileMap& refTileMap, std::unordered_set<size_t>& isMap, int x, int y, Entity tileEntity, std::vector<Math2D::Edge>& refEdges)
	{
		
		auto isTileSolid = [&](int x, int y) -> bool
				{
					const auto* tile = refTileMap.GetTile(x, y);

					return tile && isMap.find(tile->second) != isMap.end();
				};
		
		bool hasTileAbove = y > 0 && isTileSolid(x, y - 1);
		bool hasTileLeft = x > 0 && isTileSolid(x - 1, y);
		bool hasTileBelow = y < refTileMap.GetHeight() - 1 && isTileSolid(x, y + 1);
		bool hasTileRight = x < refTileMap.GetWidth() - 1 && isTileSolid(x + 1, y);

		bool hasTileDiagonalLeftAbove = x > 0 && y > 0 && isTileSolid(x - 1, y - 1);
		bool hasTileDiagonalLeftBelow = x > 0 && y < refTileMap.GetHeight() - 1 && isTileSolid(x - 1, y + 1);
		bool hasTileDiagonalRightAbove = x < refTileMap.GetWidth() - 1 && y > 0 && isTileSolid(x + 1, y - 1);
		bool hasTileDiagonalRightBelow = x < refTileMap.GetWidth() - 1 && y < refTileMap.GetHeight() - 1 && isTileSolid(x + 1, y + 1);

		//adjacent tiles must include if a tile size is larger than 1x1 units.
			
		if (!hasTileAbove)
		{
			// TODO: Need to organize by catergory and mask.

			float ghostX0, ghostY0;
			float x1 = (float)x;
			float y1 = (float)y;
			float x2 = x + 1.0f;
			float y2 = (float)y;
			float ghostX3, ghostY3;

			if (!hasTileRight) { ghostX3 = (float)x + 1.0f; ghostY3 = (float)y + 1.0f; }
			else if (hasTileRight && hasTileDiagonalRightAbove) { ghostX3 = (float)x + 1.0f; ghostY3 = (float)y - 1.0f; }
			else if (hasTileRight) { ghostX3 = (float)x + 2.0f; ghostY3 = (float)y; }

			if (!hasTileLeft) { ghostX0 = (float)x; ghostY0 = (float)y + 1.0f; }
			else if (hasTileLeft && hasTileDiagonalLeftAbove) { ghostX0 = (float)x; ghostY0 = (float)y - 1.0f; }
			else if (hasTileLeft) { ghostX0 = (float)x - 1.0f; ghostY0 = (float)y; }		

			refEdges.emplace_back(Math2D::Point2D<float>(ghostX0, ghostY0), Math2D::Point2D<float>(x1, y1), Math2D::Point2D<float>(x2, y2), Math2D::Point2D<float>(ghostX3, ghostY3));
		}

		if (!hasTileLeft)
		{
			// TODO: Need to organize by catergory and mask.

			float ghostX0, ghostY0;
			float x1 = (float)x;
			float y1 = y + 1.0f;
			float x2 = (float)x;
			float y2 = (float)y;
			float ghostX3, ghostY3;

			if (!hasTileAbove) { ghostX3 = x + 1.0f; ghostY3 = (float)y; }
			else if (hasTileAbove && hasTileDiagonalLeftAbove) { ghostX3 = (float)x - 1.0f; ghostY3 = (float)y; }
			else if (hasTileAbove) { ghostX3 = (float)x; ghostY3 = (float)y - 1.0f; }

			if (!hasTileBelow) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y + 1.0f; }
			else if (hasTileBelow && hasTileDiagonalLeftBelow) { ghostX0 = (float)x - 1.0f; ghostY0 = (float)y + 1.0f; }
			else if (hasTileBelow) { ghostX0 = (float)x; ghostY0 = (float)y + 2.0f; }

			refEdges.emplace_back(Math2D::Point2D<float>(ghostX0, ghostY0), Math2D::Point2D<float>(x1, y1), Math2D::Point2D<float>(x2, y2), Math2D::Point2D<float>(ghostX3, ghostY3));
		}

		if (!hasTileBelow)
		{
			// TODO: Need to organize by catergory and mask.

			float ghostX0, ghostY0;
			float x1 = x + 1.0f;
			float y1 = y + 1.0f;
			float x2 = (float)x;
			float y2 = y + 1.0f;
			float ghostX3, ghostY3;

			if (!hasTileLeft) { ghostX3 = (float)x; ghostY3 = (float)y; }
			else if (hasTileLeft && hasTileDiagonalLeftBelow) { ghostX3 = (float)x; ghostY3 = (float)y + 2.0f; }
			else if (hasTileLeft) { ghostX3 = (float)x - 1.0f; ghostY3 = (float)y + 1.0f; }

			if (!hasTileRight) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y; }
			else if (hasTileRight && hasTileDiagonalRightBelow) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y + 2.0f; }
			else if (hasTileRight) { ghostX0 = (float)x + 2.0f; ghostY0 = (float)y + 1.0f; }

			refEdges.emplace_back(Math2D::Point2D<float>(ghostX0, ghostY0), Math2D::Point2D<float>(x1, y1), Math2D::Point2D<float>(x2, y2), Math2D::Point2D<float>(ghostX3, ghostY3));
		}

		if (!hasTileRight)
		{
			// TODO: Need to organize by catergory and mask.

			float ghostX0, ghostY0;
			float x1 = x + 1.0f;
			float y1 = (float)y;
			float x2 = x + 1.0f;
			float y2 = y + 1.0f;
			float ghostX3, ghostY3;

			if (!hasTileAbove) { ghostX0 = (float)x; ghostY0 = (float)y; }
			else if (hasTileAbove && hasTileDiagonalRightAbove) { ghostX0 = (float)x + 2.0f; ghostY0 = (float)y; }
			else if (hasTileAbove) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y - 1.0f; }

			if (!hasTileBelow) { ghostX3 = (float)x; ghostY3 = (float)y + 1.0f; }
			else if (hasTileBelow && hasTileDiagonalRightBelow) { ghostX3 = (float)x + 2.0f; ghostY3 = (float)y + 1.0f; }
			else if (hasTileBelow) { ghostX3 = (float)x + 1.0f; ghostY3 = (float)y + 2.0f; }

			refEdges.emplace_back(Math2D::Point2D<float>(ghostX0, ghostY0), Math2D::Point2D<float>(x1, y1), Math2D::Point2D<float>(x2, y2), Math2D::Point2D<float>(ghostX3, ghostY3));
		}
	}

	static void addPhysicsComponent(ECS& refECS, TileMap& refTileMap, Entity tileEntity, const size_t tileId, const json& physicsComponentTemplate,
		const std::string& physicsTemplateKey, int x, int y, size_t numUnitsPerTile, std::unordered_set<size_t>& isMap, std::vector<Math2D::Edge>& edges)
	{
		// Add Physics components.
		BodyType bodyType = STATIC;
		Filter category = ALL;
		Filter mask = ALL;
		Math2D::Point2D<float> size = Math2D::Point2D<float>(static_cast<float>(numUnitsPerTile), static_cast<float>(numUnitsPerTile));
		bool gravityOn = false;
		bool checkSimpleContacts = false;
		bool fillRect = false;
		bool drawDebug = false;
		DebugColor debugColor = DebugColor::NoColor;

		if (physicsComponentTemplate.contains(physicsTemplateKey))
		{
			json characterPhysicsRulesJson = physicsComponentTemplate[physicsTemplateKey];

			// Physics library needs to know if the body is enabled or not.
			const bool enabled = characterPhysicsRulesJson.contains("enabled") ? characterPhysicsRulesJson["enabled"].get<bool>() : true;

			if (const json* categoryJson = getJson(characterPhysicsRulesJson, "Category"))
			{
				std::string categoryStr = categoryJson->get<std::string>();
				if (categoryStr == "NONE") category = NONE;
				else if (categoryStr == "PLAYER") category = PLAYER;
				else if (categoryStr == "MAP") category = MAP;
				else if (categoryStr == "ALL") category = ALL;
			}
			if (const json* maskJson = getJson(characterPhysicsRulesJson, "Mask"))
			{
				std::string maskStr = maskJson->get<std::string>();
				if (maskStr == "NONE") mask = NONE;
				else if (maskStr == "PLAYER") mask = PLAYER;
				else if (maskStr == "MAP") mask = MAP;
				else if (maskStr == "ALL") mask = ALL;
			}
			if (const json* bodyTypeJson = getJson(characterPhysicsRulesJson, "BodyType"))
			{
				std::string bodyTypeStr = bodyTypeJson->get<std::string>();

				if (bodyTypeStr == "STATIC") bodyType = STATIC;
				else if (bodyTypeStr == "DYNAMIC") bodyType = DYNAMIC;
				else if (bodyTypeStr == "KINEMATIC") bodyType = KINEMATIC;
				else if (bodyTypeStr == "SENSOR") bodyType = SENSOR;
			}

			if (const json* fillRectJson = getJson(characterPhysicsRulesJson, "FillRect"))
			{
				fillRect = fillRectJson->get<bool>();
			}

			if (const json* gravityOnJson = getJson(characterPhysicsRulesJson, "GravityOn"))
			{
				bool gravityOnStr = gravityOnJson->get<bool>();
				if (gravityOnStr == true)
				{
					gravityOn = true;
				}
			}
			if (const json* checkSimpleContactsJson = getJson(characterPhysicsRulesJson, "CheckSimpleContacts"))
			{
				bool checkSimpleContactsStr = checkSimpleContactsJson->get<bool>();
				if (checkSimpleContactsStr == true)
				{
					checkSimpleContacts = true;
				}
			}

			if (const json* physicsSizeJson = getJson(characterPhysicsRulesJson, "SizeInUnits"))
			{
				if (physicsSizeJson->is_array() && physicsSizeJson->size() == 2)
				{

					if ((*physicsSizeJson)[0].is_number() && (*physicsSizeJson)[1].is_number())
					{
						size.X = (*physicsSizeJson)[0].get<float>();
						size.Y = (*physicsSizeJson)[1].get<float>();
					}
					else
					{
						ENGINE_CRITICAL("Invalid transform size values. Using default size.");
					}
				}
			}

			if (const json* drawDebugJson = getJson(characterPhysicsRulesJson, "DrawDebug"))
			{
				drawDebug = true;
				const std::string& debugColorString = drawDebugJson->get<std::string>();
				if (debugColorString == "green")
				{
					debugColor = DebugColor::Green;
				}
				else if (debugColorString == "blue")
				{
					debugColor = DebugColor::Blue;
				}
				else if (debugColorString == "black")
				{
					debugColor = DebugColor::Black;
				}
				else if (debugColorString == "red")
				{
					debugColor = DebugColor::Red;
				}
			}

			if (bodyType == STATIC && category == MAP) // only add to map chain if part of map category for now.
			{
				createEdge(refTileMap, isMap, x, y, tileEntity, edges);
			}
			else // Create a regular physics body for the tiles that don't use chains.
			{
				refECS.AddComponent<PhysicsBody>(tileEntity, enabled, bodyType, category, mask,
					Math2D::Point2D<float>(size.X, size.Y),
					Math2D::Point2D<float>(static_cast<float>(x) * static_cast<float>(numUnitsPerTile), static_cast<float>(y) * static_cast<float>(numUnitsPerTile)),
					0.0f, gravityOn, checkSimpleContacts, drawDebug, fillRect, debugColor);
			}
		}
	}

	static void addSpriteComponent(ECS& refECS, AssetManager& refAssetManager, Entity entity, 
		const json& spriteSheetsTemplate, const std::string& spriteSheetKey, const json& spriteSettings)
	{
		std::string path = getJson(spriteSettings, "PathToSpriteSheets")->get<std::string>();

		if (path.empty())
		{
			ENGINE_CRITICAL("No path for sprite sheet found.");
			return;
		}

		const json* entitySpriteSheetJson = getJson(spriteSheetsTemplate, spriteSheetKey);
		if (!entitySpriteSheetJson) 
		{
			ENGINE_CRITICAL("No sprite sheet template found for key: {}", spriteSheetKey);
			return;
		}

		std::string spriteSheetPathKey = getJson(*entitySpriteSheetJson, "Path")->get<std::string>();

		const json* texturesJson = getJson(spriteSettings, "Textures");
		if (!texturesJson)
		{
			ENGINE_CRITICAL("No textures found in sprite paths.");
			return;
		}

		std::string spriteSheetName = getJson(*texturesJson, spriteSheetPathKey)->get<std::string>();

		void* ptrLoadedTexture = refAssetManager.LoadTexture(entity, path + spriteSheetName);

		Math2D::Point2D<float> sizeInUnits = ExtractPoint2DFromJSON<float>(*entitySpriteSheetJson, "SizeInUnits", { 1.0f, 1.0f });
		Math2D::Point2D<int> pixelsPerFrame = ExtractPoint2DFromJSON<int>(*entitySpriteSheetJson, "PixelsPerFrame", { 32, 32 });
		Math2D::Point2D<float> offsetFromTransform = ExtractPoint2DFromJSON<float>(*entitySpriteSheetJson, "OffsetFromTransform", { 0.0f, 0.0f });
		Math2D::Point2D<size_t> dimensions = { entitySpriteSheetJson->value("Width", (size_t)1), entitySpriteSheetJson->value("Height", (size_t)1) };
 
		DebugColor debugColorEnum = DebugColor::NoColor;
		bool drawDebug = false;

		if (const json* drawDebugJson = getJson(*entitySpriteSheetJson, "DrawDebug"))
		{
			drawDebug = true;
			std::string debugColor = drawDebugJson->get<std::string>();
			if (debugColor == "green")
			{
				debugColorEnum = DebugColor::Green;
			}
			else if (debugColor == "blue")
			{
				debugColorEnum = DebugColor::Blue;
			}
			else if (debugColor == "black")
			{
				debugColorEnum = DebugColor::Black;
			}
			else if (debugColor == "red")
			{
				debugColorEnum = DebugColor::Red;
			}
		}

		refECS.AddComponent<Sprite>(entity, ptrLoadedTexture, pixelsPerFrame, offsetFromTransform,
			dimensions, sizeInUnits, drawDebug, debugColorEnum);
	}

	static void addAnimationsComponent(ECS& refECS, AssetManager& refAssetManager, Entity entity, const json& animationsTemplate,
		const std::string& animationTemplateKey)
	{
		const json* entityAnimationsTemplate = getJson(animationsTemplate, animationTemplateKey);

		std::unordered_map<size_t, Animation> animations;
		json j = json::parse(entityAnimationsTemplate->dump());

		for (auto& [name, value] : j.items())
		{
			size_t id = ExtractSizeTFromJSON(value, "id", -1);

			animations.emplace(id, Animation(id, value.at("Frames").get<std::vector<int>>(), value.at("FrameTime").get<int>(), value.at("Loop").get<bool>()));
		}

		refECS.AddComponent<Animations>(entity, animations);
	}

	static void verifyAssetPaths(const json& assetsRules)
	{
		// Verify Sprites
		if (assetsRules.contains("Sprites"))
		{
			const auto& spriteRules = getJson(assetsRules, "Sprites");
			std::string spriteDir = spriteRules->value("PathToSpriteSheets", "");

			if (spriteDir.empty() || !fs::exists(spriteDir) || !fs::is_directory(spriteDir))
			{
				ENGINE_ERROR("Invalid or missing sprite sheet directory: {}", spriteDir);
				std::exit(1);
			}

			if (spriteRules->contains("Textures"))
			{
				const auto& textures = getJson(*spriteRules, "Textures");
				for (const auto& [textureName, textureFile] : textures->items())
				{
					std::string fullPath = spriteDir + textureFile.get<std::string>();
					if (!fs::exists(fullPath))
					{
						ENGINE_ERROR("Missing texture file: {}", fullPath);
						std::exit(1);
					}
				}
			}
			else
			{
				ENGINE_ERROR("Missing 'Textures' section in 'Sprites'.");
				std::exit(1);
			}
		}

		// Verify Audio
		if (assetsRules.contains("Audio"))
		{
			const auto& audioRules = getJson(assetsRules, "Audio");
			std::string audioDir = audioRules->value("PathToAudioFiles", "");

			if (audioDir.empty() || !fs::exists(audioDir) || !fs::is_directory(audioDir))
			{
				ENGINE_ERROR("Invalid or missing audio directory: {}", audioDir);
				std::exit(1);
			}

			if (audioRules->contains("Sounds"))
			{
				const auto& sounds = getJson(*audioRules, "Sounds");
				for (const auto& [soundFile, _] : sounds->items())
				{
					std::string fullPath = audioDir + soundFile;
					if (!fs::exists(fullPath))
					{
						ENGINE_ERROR("Missing sound file: {}", fullPath);
						std::exit(1);
					}
				}
			}
			else
			{
				ENGINE_ERROR("Missing 'Sounds' section in 'Audio'.");
				std::exit(1);
			}
		}
	}

	static std::unordered_set<size_t> determineMapTiles(const json& characterRules, const json& componentTemplates)
	{
		std::unordered_set<size_t> isMap;

		for (const auto& item : characterRules.items())
		{
			const std::string key = item.key();
			const json& value = item.value();

			// Convert key -> size_t
			size_t tileId = 0;
			try
			{
				// nlohmann::json stores object keys as strings, convert safely
				tileId = static_cast<size_t>(std::stoull(key));
			}
			catch (const std::exception& e)
			{
				ENGINE_ERROR("Invalid tile ID key in character rules: {}. Error: {}", key, e.what());
				std::exit(1);
			}

			// Look for Physics -> template -> Category == "MAP"
			if (const json* characterPhysicsJson = getJson(value, "Physics"))
			{
				std::string physicsTemplateKey = characterPhysicsJson->get<std::string>();
				if (const json* physicsTemplates = getJson(componentTemplates, "Physics"))
				{
					if (const json* entityPhysicsTemplate = getJson(*physicsTemplates, physicsTemplateKey))
					{
						if (const json* categoryJson = getJson(*entityPhysicsTemplate, "Category"))
						{
							std::string categoryString = categoryJson->get<std::string>();
							if (categoryString == "MAP")
							{
								isMap.emplace(tileId);
							}
						}
					}
				}
			}
		}

		return isMap;
	}

	void Scene::createChainColliders(std::vector<Math2D::Chain>& refChains)
	{
		std::vector<ChainCollider> chainColliders;

		for (auto& refChain : refChains)
		{
			Entity chainEntity = m_refECS.CreateEntity();

			m_refECS.AddComponent<ChainCollider>(chainEntity, refChain,
				true, MAP, ALL, true, DebugColor::Red);

			// refECS.Activate(chainEntity);
			add(chainEntity);
		}
	}

	void Scene::loadSceneEntitiesFromTileMap()
	{
		const std::string& sceneName = rulesJson.begin().key();
		ENGINE_LOG("Loading scene entities for scene: {}", sceneName);


		// Load the physics rules.
		size_t numUnitsPerTile = 1;
		size_t numLayers = 5;

		const json* physicsRules = getJson(worldLayers, "Physics");
		if (!physicsRules)
		{
			ENGINE_ERROR("No physics layer found for world in scene: {}", sceneName);
			std::exit(1);
		}

		SetGravity(ExtractPoint2DFromJSON<float>(*physicsRules, "Gravity", { 0.0f, 0.0f }));
		numUnitsPerTile = ExtractSizeTFromJSON(*physicsRules, "NumUnitsPerTile", 1);

		// Load Assets.

		
		verifyAssetPaths(sceneAssets);

		const json* characterRules = getJson(sceneRules, "CharacterRules");
		if (!characterRules)
		{
			ENGINE_ERROR("No character rules found for scene: {}", sceneName);
			std::exit(1);
		}

		const json* componentTemplates = getJson(sceneRules, "ComponentTemplates");
		if (!componentTemplates)
		{
			ENGINE_ERROR("No component templates found for scene: {}", sceneName);
			std::exit(1);
		}

		std::unordered_set<size_t> isMap = determineMapTiles(*characterRules, *componentTemplates);
		std::vector<Math2D::Edge> edges;

		for (auto& layer : m_layers)
		{
			const float parallaxFactor = layer.second.m_parallaxFactor;
			const int layerIndex = layer.first;

			// if theres no tilemap in this layer, skip it.
			if (!layer.second.m_hasTileMap)
				continue;

			TileMap& refTileMap = layer.second.m_tileMap;

			for (auto& [coords, info] : layer.second.m_tileMap.GetMap())
			{
				const size_t tileId = info.second;
				const int x = coords.first;
				const int y = coords.second;
				Entity tileEntity = info.first;

				std::string tileKey = std::to_string(tileId);

				const json* characterComponents = nullptr;
				if (!(characterComponents = getJson(*characterRules, tileKey)))
				{
					ENGINE_INFO("Tile ID {} at ({}, {}) has no character rules defined. Skipping entity creation.", tileId, x, y);
					continue;
				}

				const json* characterTransformJson = nullptr;
				if (characterTransformJson = getJson(*characterComponents, "Transform"))
				{
					std::string transformTemplateKey = characterTransformJson->get<std::string>();
					const json* transformTemplates = getJson(*componentTemplates, "Transforms");
					if (transformTemplates)
						addTransformComponent(m_refECS, tileEntity, layerIndex, parallaxFactor, *transformTemplates, transformTemplateKey, x, y, numUnitsPerTile);
				}
				else ENGINE_ERROR("Transform component is required for all entities. Missing for tile: " + tileKey);

				if (const json* characterCameraJson = getJson(*characterComponents, "Camera"))
				{
					std::string cameraTemplateKey = characterCameraJson->get<std::string>();
					const json* cameraTemplates = getJson(*componentTemplates, "Camera");
					if (cameraTemplates)
						m_cameraOrder.emplace(addCameraComponent(m_refECS, tileEntity, *cameraTemplates, cameraTemplateKey, numLayers));
				}
				if (const json* characterPhysicsJson = getJson(*characterComponents, "Physics"))
				{
					std::string physicsTemplateKey = characterPhysicsJson->get<std::string>();
					const json* physicsTemplates = getJson(*componentTemplates, "Physics");
					std::string transformTemplateKey = characterTransformJson->get<std::string>();
					const json* transformTemplates = getJson(*componentTemplates, "Transforms");
					if (physicsTemplates)
						addPhysicsComponent(m_refECS, refTileMap, tileEntity, tileId, *physicsTemplates, physicsTemplateKey, x, y, numUnitsPerTile, isMap, edges);
				}
				if (const json* characterSpriteSheetJson = getJson(*characterComponents, "SpriteSheet"))
				{
					std::string spriteSheetTemplateKey = characterSpriteSheetJson->get<std::string>();
					const json* spriteSheetTemplates = getJson(*componentTemplates, "SpriteSheets");

					if (spriteSheetTemplates)
						addSpriteComponent(m_refECS, m_refAssetManager, tileEntity, *spriteSheetTemplates, spriteSheetTemplateKey, *getJson(sceneAssets, "Sprites"));
				}
				if (const json* characterAnimationsJson = getJson(*characterComponents, "Animations"))
				{
					std::string animationsTemplateKey = characterAnimationsJson->get<std::string>();
					const json* animationsTemplate = getJson(*componentTemplates, "Animations");
					if (animationsTemplate)
						addAnimationsComponent(m_refECS, m_refAssetManager, tileEntity, *animationsTemplate, animationsTemplateKey);
				}
			}
		}

		m_staticChains = Math2D::MergeGridLinesIntoChains(edges);
		createChainColliders(m_staticChains);
	}
}