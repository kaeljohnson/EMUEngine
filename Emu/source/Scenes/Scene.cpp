#pragma once

#include "box2d/box2d.h"
#include "../../include/TileMap/TileMap.h"
#include "../../include/Physics/Physics.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/MathUtil.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Time.h"
#include "../../include/AppState.h"

#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace fs = std::filesystem;

static json rulesJson; // Only one rules file per game for now so this will work.

namespace Engine
{
	Scene::Scene(ECS& refECS, AssetManager& refAssetManager)
		: m_refECS(refECS), m_levelDimensionsInUnits(32, 32), m_hasTileMap(false), m_tileMap(m_refECS), 
		m_physicsSimulation(refECS, m_tileMap), m_refAssetManager(refAssetManager),
		m_cameraSystem(refECS)
	{
		m_entities.reserve(50000);
	}

	Scene::~Scene()
	{
		m_physicsSimulation.Cleanup();
	}
	
	void Scene::RegisterContactCallback(ContactType contactType, const char entityA, const char entityB, ContactCallback callback)
	{
		m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, entityA, entityB, callback);
	}

	void Scene::RegisterContactCallback(ContactType contactType, const char entity, ContactCallback callback)
	{
		m_physicsSimulation.m_contactSystem.RegisterContactCallback(contactType, entity, callback);
	}

	void Scene::RegisterOnScenePlay(std::function<void()> func)
	{
		// Register a function to be called when the scene starts playing.
		// This can be used to initialize things that need to be set up when the scene starts.
		m_clientOnScenePlay = func;
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

		// 5. Frame the cameras
		m_cameraSystem.Frame(Vector2D<int>(m_levelDimensionsInUnits.X, m_levelDimensionsInUnits.Y));
		
		// 6. Physics bodies need to be added to the world after they are activated and pooled.
		m_physicsSimulation.AddPhysicsBodiesToWorld(m_entities);
		m_physicsSimulation.AddLineCollidersToWorld(m_entities);

		// 7. Contact callbacks need to be activated.
		m_physicsSimulation.ActivateContactCallbacks();

		// 8. Deactivate all components that should not be active at the start of the scene?

		ENGINE_CRITICAL_D("Num transforms: {}, Num bodies: {}, Num line colliders: {}, Num cameras: {}, Num tile map entities: {}", 
			m_refECS.GetHotComponents<Transform>().size(), m_refECS.GetHotComponents<PhysicsBody>().size(),
			m_refECS.GetHotComponents<ChainCollider>().size(), m_refECS.GetHotComponents<Camera>().size(),
			m_tileMap.GetMap().size());

		// process items client wants to do.
		if (m_clientOnScenePlay) m_clientOnScenePlay();

		AppState::IN_SCENE = true;
	}

	void Scene::OnSceneEnd()
	{
		// Could be problematic if this is called mid frame.
		AppState::IN_SCENE = false;

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

		m_refAssetManager.UnloadSounds();

		// Deactivate all entities and destroy all components.
		m_refECS.DeactivateEntities();
		m_refECS.DestroyComponents(m_entities);
	}

	void Scene::AddTileMap(std::string mapFileName, std::string rulesFileName)
	{
		m_mapFileName = mapFileName;
		m_rulesFileName = rulesFileName;

		m_tileMap.CreateMap(mapFileName);

		m_levelDimensionsInUnits = Vector2D<int>(m_tileMap.GetWidth(), m_tileMap.GetHeight());

		ENGINE_CRITICAL_D("Map width: {}, Map height: {}", m_levelDimensionsInUnits.X, m_levelDimensionsInUnits.Y);

		m_hasTileMap = true;

		for (auto& [coords, info] : m_tileMap.GetMap())
		{	
			add(info.first);
		}
	}

	void Scene::add(Entity entity)
	{
		if (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end()) // SLOW. Temp for now.
		{
			ENGINE_INFO("Entity already exists in the scene: {}", entity);
			return;
		}

		m_entities.push_back(entity);
	}

	void Scene::Activate(Entity entity)
	{
		if (std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end()) // SLOW. Temp for now.
		{
			ENGINE_INFO("Entity does not exist in the current scene: {}", entity);
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
		if (std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end()) // SLOW. Temp for now.
		{
			ENGINE_INFO("Entity does not exist in the current scene: {}", entity);
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

	void Scene::SetLevelDimensions(const Vector2D<int> levelDimensions)
	{
		if (m_hasTileMap)
		{
			ENGINE_INFO_D("Scene already has a tile map. Cannot override map dimensions!");
			return;
		}

		m_levelDimensionsInUnits = levelDimensions;
	}

	void Scene::UpdatePhysics()
	{
		m_physicsSimulation.Update();
	}

	void Scene::UpdateCamera(AssetManager& refAssetManager)
	{
		m_cameraSystem.Update(refAssetManager);
	}

	void Scene::SetGravity(const Vector2D<float> gravity)
	{
		ENGINE_INFO_D("Setting gravity: {}, {}", gravity.X, gravity.Y);

		m_physicsSimulation.UpdateGravity(gravity);
	}

	const Entity Scene::GetTileMapEntity(char tileChar) const
	{
		return m_tileMap.GetEntity(tileChar);
	}

	// This function should only be called if the json should be guaranteed to exist.
	// j must out live the pointer returned.
	static const json* getJson(const json& j, const std::string& key)
	{
		auto it = j.find(key);
		if (it == j.end())
		{
			ENGINE_CRITICAL_D("Field not found in rules file: {}. Returning nullptr.", key);
			return nullptr;
		}
		return &(*it);
	}


	void Scene::loadAudioFiles()
	{
		// Open and parse the rules file
		std::ifstream inFile(m_rulesFileName);
		if (!inFile.is_open())
		{
			throw std::runtime_error("Failed to open rules file: " + m_rulesFileName);
		}

		try
		{
			inFile >> rulesJson;
		}
		catch (const json::parse_error& e)
		{
			throw std::runtime_error("Failed to parse rules JSON: " + std::string(e.what()));
		}

		auto& sceneName = rulesJson.begin().key();
		ENGINE_INFO_D("Loading scene entities for scene: {}", sceneName);

		const json* sceneRules = getJson(rulesJson, sceneName);

		const json* assetsJson = getJson(*sceneRules, "Assets");
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
		if (!audioFilePathJson) throw std::runtime_error("PathToAudioFiles not found in rules file.");

		std::string audioFilePath = audioFilePathJson->get<std::string>();

		const json* soundsJson = getJson(*audioJson, "Sounds");
		if (!soundsJson) throw std::runtime_error("Sounds section not found in rules file.");

		json j = json::parse(soundsJson->dump());

		m_refAssetManager.PrepareSoundStorage(j.size());

		for (auto& [file, idx] : j.items())
		{
			int idxInt = idx.get<int>();
			std::string fullPath = audioFilePath + file;
			m_refAssetManager.LoadSound(idxInt, fullPath);
		}
	}

	template<typename T>
	static Vector2D<T> ExtractVector2DFromJSON(const json& j, const std::string& key, Vector2D<T> ioVec)
	{
		if (!j.contains(key)) throw std::runtime_error("Invalid Rules File. Field Not Found: " + key);

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
		if (!j.contains(key)) throw std::runtime_error("Invalid Rules File. Field Not Found: " + key);

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

	static void addTransformComponent(ECS& refECS, Entity entity, const json& transformTemplates, std::string templateKey, int x, int y, size_t numUnitsPerTile)
	{
		const json* entityTransformTemplate = getJson(transformTemplates, templateKey);
		size_t zIndex = ExtractSizeTFromJSON(*entityTransformTemplate, "ZIndex", 0);
		const Vector2D<float> sizeInUnits = 
			ExtractVector2DFromJSON<float>(*entityTransformTemplate, "SizeInUnits", Vector2D<float>(1.0f, 1.0f));

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
			Vector2D<float>(x * (float)numUnitsPerTile, y * (float)numUnitsPerTile),
			Vector2D<float>(sizeInUnits.X * (float)numUnitsPerTile, sizeInUnits.Y * (float)numUnitsPerTile),
			1.0f, 1.0f, 1, zIndex, drawDebug, debugColorEnum
		);
	}

	static void addCameraComponent(ECS& refECS, Entity entity, const json& cameraTemplate, const std::string& cameraTemplateKey, const size_t numLayers)
	{
		const json* entityCameraTemplate = getJson(cameraTemplate, cameraTemplateKey);

		Vector2D<float> size = { 0.0f, 0.0f }; // size determined by engine 
		size_t pixelsPerUnit = ExtractSizeTFromJSON(*entityCameraTemplate, "PixelsPerUnit", 0);

		bool clampingOn = entityCameraTemplate->contains("ClampingOn") ? getJson(*entityCameraTemplate, "ClampingOn")->get<bool>() : false;

		Vector2D<float> screenRatio = { 1.0f, 1.0f };
		Vector2D<float> position = { 0.0f, 0.0f };

		if (const json* windowJson = getJson(*entityCameraTemplate, "Window"))
		{
			position.X = windowJson->value("X", 0.0f);
			position.Y = windowJson->value("Y", 0.0f);
			screenRatio.X = windowJson->value("Width", 1.0f);
			screenRatio.Y = windowJson->value("Height", 1.0f);
		}

		refECS.AddComponent<Camera>(entity, size, screenRatio, position, pixelsPerUnit, clampingOn, numLayers);
	}

	static void addPhysicsComponent(ECS& refECS, TileMap& refTileMap, Entity tileEntity, const char tileChar, const json& physicsComponentTemplate,
		const std::string& physicsTemplateKey, const json& transformsComponentTemplate, const std::string& transformTemplateKey,
		int x, int y, size_t numUnitsPerTile, std::unordered_set<char>& isSolid)
	{
		// Add Physics components.
		BodyType bodyType = STATIC;
		Filter category = ALL;
		Filter mask = ALL;
		Vector2D<float> size = Vector2D<float>(static_cast<float>(numUnitsPerTile), static_cast<float>(numUnitsPerTile));
		bool gravityOn = false;
		bool checkSimpleContacts = false;
		bool useChains = false;
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

				if (bodyTypeStr != "SENSOR") isSolid.emplace(tileChar);

				if (bodyTypeStr == "STATIC") bodyType = STATIC;
				else if (bodyTypeStr == "DYNAMIC") bodyType = DYNAMIC;
				else if (bodyTypeStr == "KINEMATIC") bodyType = KINEMATIC;
				else if (bodyTypeStr == "SENSOR") bodyType = SENSOR;
			}

			if (const json* fillRectJson = getJson(characterPhysicsRulesJson, "FillRect"))
			{
				fillRect = fillRectJson->get<bool>();
			}

			if (bodyType != SENSOR)
			{
				// If body is not a sensor, it is solid and collides with other bodies.
				// This information is needed for the chain system to know which tiles to link to.
				// Not the best design but will work for now.
				// isSolid.emplace(tileChar);
				
				if (const json* useChainsJson = getJson(characterPhysicsRulesJson, "UseChains"))
				{
					const bool useChainsBool = useChainsJson->get<bool>();
					if (useChainsBool == true)
					{
						useChains = true;
					}
				}
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
				bool checkSimpleContactsStr = checkSimpleContactsJson;
				if (checkSimpleContactsStr == true)
				{
					checkSimpleContacts = true;
				}

			}

			json characterTransformRulesJson = transformsComponentTemplate[transformTemplateKey];
			if (const json* transformSizeJson = getJson(characterTransformRulesJson, "SizeInUnits"))
			{
				if (transformSizeJson->is_array() && transformSizeJson->size() == 2)
				{

					if ((*transformSizeJson)[0].is_number() && (*transformSizeJson)[1].is_number())
					{
						size.X = (*transformSizeJson)[0].get<float>();
						size.Y = (*transformSizeJson)[1].get<float>();
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

			if (useChains) // Chains are used on tiles, typically map tiles, to avoid ghost collisions on adjacent tiles.
			{
				auto isTileSolid = [&](int x, int y) -> bool
					{
						const auto* tile = refTileMap.GetTile(x, y);
						if (!tile) return false;

						return isSolid.find(tile->second) != isSolid.end();
					};


				//adjacent tiles must include if a tile size is larger than 1x1 units.
				bool hasTileAbove = y > 0 && isTileSolid(x, y - 1);
				bool hasTileBelow = y < refTileMap.GetHeight() - 1 && isTileSolid(x, y + 1);
				bool hasTileLeft = x > 0 && refTileMap.GetTile(x - 1, y) && isTileSolid(x - 1, y);
				bool hasTileRight = x < refTileMap.GetWidth() - 1 && isTileSolid(x + 1, y);

				bool hasTileDiagonalLeftAbove = x > 0 && y > 0 && isTileSolid(x - 1, y - 1);
				bool hasTileDiagonalLeftBelow = x > 0 && y < refTileMap.GetHeight() - 1 && isTileSolid(x - 1, y + 1);
				bool hasTileDiagonalRightAbove = x < refTileMap.GetWidth() - 1 && y > 0 && isTileSolid(x + 1, y - 1);
				bool hasTileDiagonalRightBelow = x < refTileMap.GetWidth() - 1 && y < refTileMap.GetHeight() - 1 && isTileSolid(x + 1, y + 1);

				if (!hasTileAbove)
				{
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

					refECS.AddComponent<ChainColliderTop>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
						Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3), drawDebug, debugColor);
				}

				if (!hasTileLeft)
				{
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

					refECS.AddComponent<ChainColliderLeft>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
						Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3), drawDebug, debugColor);
				}

				if (!hasTileBelow)
				{
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

					refECS.AddComponent<ChainColliderBottom>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
						Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3), drawDebug, debugColor);
				}

				if (!hasTileRight)
				{
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

					refECS.AddComponent<ChainColliderRight>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
						Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3), drawDebug, debugColor);
				}
			}
			else // Create a regular physics body for the tiles that don't use chains.
			{
				refECS.AddComponent<PhysicsBody>(tileEntity, enabled, bodyType, category, mask,
					Vector2D<float>(size.X, size.Y),
					Vector2D<float>(static_cast<float>(x) * static_cast<float>(numUnitsPerTile), static_cast<float>(y) * static_cast<float>(numUnitsPerTile)),
					0.0f, gravityOn, checkSimpleContacts, drawDebug, fillRect, debugColor);
			}
		}
	}

	static void addSpriteComponent(ECS& refECS, AssetManager& refAssetManager, Entity entity, 
		const char tileChar, const json& spriteSheetsTemplate, const std::string& spriteSheetKey, const json& spriteSettings)
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

		Vector2D<float> sizeInUnits = ExtractVector2DFromJSON<float>(*entitySpriteSheetJson, "SizeInUnits", { 1.0f, 1.0f });
		Vector2D<int> pixelsPerFrame = ExtractVector2DFromJSON<int>(*entitySpriteSheetJson, "PixelsPerFrame", { 32, 32 });
		Vector2D<float> offsetFromTransform = ExtractVector2DFromJSON<float>(*entitySpriteSheetJson, "OffsetFromTransform", { 0.0f, 0.0f });
		Vector2D<size_t> dimensions = { entitySpriteSheetJson->value("Width", (size_t)1), entitySpriteSheetJson->value("Height", (size_t)1) };
 
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

	static void addAnimationsComponent(ECS& refECS, AssetManager& refAssetManager, Entity entity, const char tileChar, const json& animationsTemplate,
		const std::string& animationTemplateKey)
	{
		const json* entityAnimationsTemplate = getJson(animationsTemplate, animationTemplateKey);

		std::unordered_map<std::string, Animation> animations;
		json j = json::parse(entityAnimationsTemplate->dump());

		for (auto& [name, value] : j.items())
		{
			animations.emplace(name, Animation(name, value.at("Frames").get<std::vector<int>>(), value.at("FrameTime").get<int>(), value.at("Loop").get<bool>()));
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
				throw std::runtime_error("Invalid or missing sprite sheet directory: " + spriteDir);

			if (spriteRules->contains("Textures"))
			{
				const auto& textures = getJson(*spriteRules, "Textures");
				for (const auto& [textureName, textureFile] : textures->items())
				{
					std::string fullPath = spriteDir + textureFile.get<std::string>();
					if (!fs::exists(fullPath))
						throw std::runtime_error("Missing texture file: " + fullPath);
				}
			}
			else
			{
				throw std::runtime_error("Missing 'Textures' section in 'Sprites'.");
			}
		}

		// Verify Audio
		if (assetsRules.contains("Audio"))
		{
			const auto& audioRules = getJson(assetsRules, "Audio");
			std::string audioDir = audioRules->value("PathToAudioFiles", "");

			if (audioDir.empty() || !fs::exists(audioDir) || !fs::is_directory(audioDir))
				throw std::runtime_error("Invalid or missing audio directory: " + audioDir);

			if (audioRules->contains("Sounds"))
			{
				const auto& sounds = getJson(*audioRules, "Sounds");
				for (const auto& [soundFile, _] : sounds->items())
				{
					std::string fullPath = audioDir + soundFile;
					if (!fs::exists(fullPath))
						throw std::runtime_error("Missing sound file: " + fullPath);
				}
			}
			else
			{
				throw std::runtime_error("Missing 'Sounds' section in 'Audio'.");
			}
		}
	}

	static std::unordered_set<char> determineSolidTiles(const json& characterRules, const json& componentTemplates)
	{
		// Predetermine the solid tiles based on their physics body type to assist with chain creation.
		// Otherwise, when processing each tile we need to get the physics body type of all adjacent tiles.
		std::unordered_set<char> isSolid;

		for (auto& [key, value] : characterRules.items())
		{
			if (const json* characterPhysicsJson = getJson(value, "Physics"))
			{
				std::string physicsTemplateKey = characterPhysicsJson->get<std::string>();
				if (const json* physicsTemplates = getJson(componentTemplates, "Physics"))
				{
					if (const json* entityPhysicsTemplate = getJson(*physicsTemplates, physicsTemplateKey))
					{
						if (const json* bodyTypeJson = getJson(*entityPhysicsTemplate, "BodyType"))
						{
							std::string bodyTypeStr = bodyTypeJson->get<std::string>();
							if (bodyTypeStr != "SENSOR")
							{
								isSolid.emplace(key[0]);
							}
						}
					}
				}
			}
		}

		return isSolid;
	}
	

	void Scene::loadSceneEntitiesFromTileMap()
	{
		const std::string& sceneName = rulesJson.begin().key();
		ENGINE_INFO_D("Loading scene entities for scene: {}", sceneName);

		const json* sceneRules = getJson(rulesJson, sceneName);
		if (!sceneRules) throw std::runtime_error("No scene rules found for scene: " + sceneName);

		// Load the physics rules.
		size_t numUnitsPerTile = 1;
		size_t numLayers = 5;
		if (const json* worldRules = getJson(*sceneRules, "World"))
		{
			numLayers = ExtractSizeTFromJSON(*worldRules, "NumLayers", 5);
			const json* physicsRules = getJson(*worldRules, "Physics");
			if (!physicsRules) throw std::runtime_error("No physics rules found for world in scene: " + sceneName);

			SetGravity(ExtractVector2DFromJSON<float>(*physicsRules, "Gravity", { 0.0f, 0.0f }));
			numUnitsPerTile = ExtractSizeTFromJSON(*physicsRules, "NumUnitsPerTile", 1);
		}

		// Load Assets.
		const json* assetsRules = getJson(*sceneRules, "Assets");

		if (assetsRules) 
		{
			verifyAssetPaths(*assetsRules);
		}
		else
		{
			ENGINE_CRITICAL("No assets rules found for scene. Continuing with debug ojects only: {}", sceneName);
		}

		const json* characterRules = getJson(*sceneRules, "CharacterRules");
		if (!characterRules) throw std::runtime_error("No character rules found for scene: " + sceneName);

		const json* componentTemplates = getJson(*sceneRules, "ComponentTemplates");
		if (!componentTemplates) throw std::runtime_error("No component templates found for scene: " + sceneName);

		std::unordered_set<char> isSolid = determineSolidTiles(*characterRules, *componentTemplates);

		for (auto& [coords, info] : m_tileMap.GetMap())
		{
			const char tileChar = info.second;
			const int x = coords.first;
			const int y = coords.second;
			Entity tileEntity = info.first;

			std::string tileKey(1, tileChar);
			if (!characterRules->contains(tileKey))
			{
				ENGINE_INFO_D("No such tile exists: {}", tileKey);
				continue;
			}

			const json* characterComponents = getJson(*characterRules, tileKey);
			if (!characterComponents) continue; // If character does not exist in json, skip.

			bool activeOnStart = characterComponents->value("ActiveOnStart", true);

			const json* characterTransformJson = nullptr;
			if (characterTransformJson = getJson(*characterComponents, "Transform"))
			{
				std::string transformTemplateKey = characterTransformJson->get<std::string>();
				const json* transformTemplates = getJson(*componentTemplates, "Transforms");
				if (transformTemplates)
					addTransformComponent(m_refECS, tileEntity, *transformTemplates, transformTemplateKey, x, y, numUnitsPerTile);
			}
			else throw std::runtime_error("Transform component is required for all entities. Missing for tile: " + tileKey);

			if (const json* characterCameraJson = getJson(*characterComponents, "Camera"))
			{
				std::string cameraTemplateKey = characterCameraJson->get<std::string>();
				const json* cameraTemplates = getJson(*componentTemplates, "Camera");
				if (cameraTemplates)
					addCameraComponent(m_refECS, tileEntity, *cameraTemplates, cameraTemplateKey, numLayers);
			}
			if (const json* characterPhysicsJson = getJson(*characterComponents, "Physics"))
			{
				std::string physicsTemplateKey = characterPhysicsJson->get<std::string>();
				const json* physicsTemplates = getJson(*componentTemplates, "Physics");
				std::string transformTemplateKey = characterTransformJson->get<std::string>();
				const json* transformTemplates = getJson(*componentTemplates, "Transforms");
				if (physicsTemplates)
					addPhysicsComponent(m_refECS, m_tileMap, tileEntity, tileChar, *physicsTemplates, physicsTemplateKey, *transformTemplates, transformTemplateKey, x, y, numUnitsPerTile, isSolid);
			}
			if (const json* characterSpriteSheetJson = getJson(*characterComponents, "SpriteSheet"))
			{
				std::string spriteSheetTemplateKey = characterSpriteSheetJson->get<std::string>();
				const json* spriteSheetTemplates = getJson(*componentTemplates, "SpriteSheets");

				if (spriteSheetTemplates && assetsRules)
					addSpriteComponent(m_refECS, m_refAssetManager, tileEntity, tileChar, *spriteSheetTemplates, spriteSheetTemplateKey, *getJson(*assetsRules, "Sprites"));
			}
			if (const json* characterAnimationsJson = getJson(*characterComponents, "Animations"))
			{
				std::string animationsTemplateKey = characterAnimationsJson->get<std::string>();
				const json* animationsTemplate = getJson(*componentTemplates, "Animations");
				if (animationsTemplate)
					addAnimationsComponent(m_refECS, m_refAssetManager, tileEntity, tileChar, *animationsTemplate, animationsTemplateKey);
			}
			// ACTIVATE IN ECS AFTER ALL COMPONENTS CREATED.
			if (activeOnStart)
				m_refECS.Activate(tileEntity);
		}
	}
}