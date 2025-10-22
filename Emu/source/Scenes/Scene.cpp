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

#include <nlohmann/json.hpp>
using json = nlohmann::json;

static json rulesJson; // Only one rules file per game for now so this will work.

namespace Engine
{
	Scene::Scene(ECS& refECS, AssetManager& refAssetManager)
		: m_refECS(refECS), m_levelDimensionsInUnits(32, 32), HasTileMap(false), m_tileMap(m_refECS), 
		m_physicsSimulation(refECS, m_tileMap), m_refAssetManager(refAssetManager),
		m_cameraSystem(refECS)
	{
		m_entities.reserve(10000);
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
		m_physicsSimulation.CreateWorld(Vector2D<float>(0.0f, 100.0f)); //TODO: Client sets gravity.

		// 2. Load audio files for the tile map.
		loadAudioFiles();

		// 3. Load the map. Adds components defined in the rules file and adds them to the ECS.
		loadSceneEntitiesFromTileMap();

		// 4. Activate the entities in the ECS. This will activate all components in the ECS.
		// m_refECS.ActivateEntities(m_entities);

		// 5. Frame the cameras
		m_cameraSystem.Frame(Vector2D<int>(GetLevelWidth(), GetLevelHeight()));
		

		// 6. Physics bodies need to be added to the world after they are activated and pooled.
		m_physicsSimulation.AddPhysicsBodiesToWorld(m_entities);
		m_physicsSimulation.AddLineCollidersToWorld(m_entities);

		// 7. Contact callbacks need to be activated.
		m_physicsSimulation.ActivateContactCallbacks();

		// 8. Deactivate all components that should not be active at the start of the scene?

		ENGINE_CRITICAL_D("Num transforms: " + std::to_string(m_refECS.GetHotComponents<Transform>().size()) + ", Num bodies: "
			+ std::to_string(m_refECS.GetHotComponents<PhysicsBody>().size()) + ", Num line colliders: "
			+ std::to_string(m_refECS.GetHotComponents<ChainCollider>().size()) + ", Num cameras: "
			+ std::to_string(m_refECS.GetHotComponents<Camera>().size()) + ", Num tile map entities: "
			+ std::to_string(m_tileMap.GetMap().size()));

		// process items client wants to do.
		if (m_clientOnScenePlay) m_clientOnScenePlay();

		AppState::IN_SCENE = true;
	}

	void Scene::OnSceneEnd()
	{
		// Could be problematic if this is called mid frame.
		AppState::IN_SCENE = false;

		m_physicsSimulation.Cleanup();

		// Deactivate all entities and destroy all components.
		m_refECS.DeactivateEntities();
		m_refECS.DestroyComponents(m_entities);
	}

	void Scene::AddTileMap(std::string mapFileName, std::string rulesFileName)
	{
		m_mapFileName = mapFileName;
		m_rulesFileName = rulesFileName;

		// Get a temp vector or tile IDs from the tile map. Both the transforms and the physics bodies.
		m_tileMap.CreateMap(mapFileName, rulesFileName);

		// ENGINE_INFO_D("Tile map text file size: " + std::to_string(m_tileMap.m_map.size()));

		m_levelDimensionsInUnits = Vector2D<int>(m_tileMap.GetWidth(), m_tileMap.GetHeight());

		ENGINE_CRITICAL_D("Map width: " + std::to_string(m_levelDimensionsInUnits.X) + ", Map height: " 
			+ std::to_string(m_levelDimensionsInUnits.Y));

		HasTileMap = true;

		for (auto& [coords, info] : m_tileMap.GetMap())
		{	
			Add(info.first);
		}
	}

	void Scene::Add(Entity entity)
	{
		m_entities.push_back(entity);
	}

	void Scene::Activate(Entity entity)
	{
		ActivatePhysics(entity);
	}

	void Scene::ActivatePhysics(Entity entity)
	{
		m_physicsSimulation.ActivateBody(entity);
		m_physicsSimulation.ActivateChains(entity);
	}

	void Scene::Deactivate(Entity entity)
	{
		DeactivatePhysics(entity);
	}

	void Scene::DeactivatePhysics(Entity entity)
	{
		m_physicsSimulation.DeactivateBody(entity);
		m_physicsSimulation.DeactivateChains(entity);
	}

	void Scene::Remove(Entity entity)
	{
		// Remove entity from the scene. Do not remove the entity from the ECS, just deactivate it.
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
		m_refECS.Deactivate(entity);
	}

	void Scene::SetLevelDimensions(const Vector2D<int> levelDimensions)
	{
		if (HasTileMap)
		{
			ENGINE_INFO_D("Scene already has a map. Overriding map width!");
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

	// Is this function necessary?
	void Scene::SetPhysicsSimulation(const Vector2D<float> gravity)
	{
		// What happens if this is called multiple times for one scene? Make sure nothing bad.

		ENGINE_INFO_D("Setting gravity: " + std::to_string(gravity.X) + ", " + std::to_string(gravity.Y));

		// m_gravity = gravity;

		m_physicsSimulation.UpdateGravity(gravity);

		// Need a reset function for the world which resets all objects in the world.

		if (!HasTileMap)
		{
			ENGINE_INFO_D("No map in the level. Add map or set level dimensions manually.");
		}
		else
		{
			ENGINE_INFO_D("Map exists in the level. Setting level width and height to map width and height.");
		}
	}

	Entity Scene::GetTileMapEntity(char tileChar) const
	{
		return m_tileMap.GetEntity(tileChar);
	}

	std::vector<Entity> Scene::GetTileMapEntities(const char tileChar) const
	{
		return m_tileMap.GetEntities(tileChar);
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

		if (!rulesJson.contains("Sounds"))
		{
			ENGINE_INFO_D("No audio files found in rules file.");
			return;
		}

		const auto& audioFilePathJson = rulesJson["PathToAudioFiles"];
		std::string audioFilePath = audioFilePathJson;

		json j = json::parse(rulesJson["Sounds"].dump());

		ENGINE_CRITICAL_D("Sounds: " + j.dump(4));

		m_refAssetManager.PrepareSoundStorage(j.size());

		for (auto& [file, idx] : j.items())
		{
			int idxInt = idx.get<int>();
			std::string fullPath = audioFilePath + file;
			ENGINE_INFO_D("Loading sound at index: " + std::to_string(idxInt) + " from file: " + fullPath);
			m_refAssetManager.LoadSound(idxInt, fullPath);
		}
	}

	void Scene::loadSceneEntitiesFromTileMap()
	{
		std::unordered_set<char> isSolid;
		size_t numUnitsPerTile = 0;

		// Read the NumMetersPerTile and store it in your member variable
		if (rulesJson.contains("NumMetersPerTile") && rulesJson["NumMetersPerTile"].is_number())
		{
			numUnitsPerTile = rulesJson["NumMetersPerTile"];
		}
		else
		{
			throw std::runtime_error("NumMetersPerTile not found or invalid in rules JSON");
		}

		auto& tileRules = rulesJson["Tile Rules"];

		// Check if the rules file contains a "SpriteSheetsPath" key
		if (!rulesJson.contains("PathToSpriteSheets"))
		{
			ENGINE_CRITICAL("No sprite sheets path found in rules file. Add path to folder with sprite pngs.");
			throw std::runtime_error("No sprite sheets path found in rules file. Add path to folder with sprite pngs.");
			return;
		}

		// Get the sprite sheets path.
		const auto& spriteSheetsPathJson = rulesJson["PathToSpriteSheets"];
		std::string spriteSheetsPath = spriteSheetsPathJson;
		ENGINE_INFO_D("Sprite sheets path: " + spriteSheetsPath);

		// Check if app has sprite folder.
		if (!std::filesystem::exists(spriteSheetsPath))
		{
			ENGINE_CRITICAL("Sprite sheets path does not exist: " + spriteSheetsPath);
			return;
		}

		// Create entity, character tiles.
		for (auto& [coords, info] : m_tileMap.GetMap())
		{
			const char tileChar = info.second;
			const int x = coords.first;
			const int y = coords.second;

			Entity tileEntity = info.first;

			// Convert tile char to string key ("P", "S", etc.)
			std::string tileKey(1, tileChar);

			if (!tileRules.contains(tileKey))
			{
				ENGINE_INFO_D("No such tile exists: " + tileKey);
				continue;
			}

			json characterRulesJson = tileRules[tileKey];

			bool activeOnStart = true;
			if (characterRulesJson.contains("ActiveOnStart"))
			{
				const auto& activeOnStartJson = characterRulesJson["ActiveOnStart"];
				bool activeOnStartBool = activeOnStartJson;
				if (activeOnStartBool == false)
				{
					activeOnStart = false;
				}
			}

			const bool hasTransform = characterRulesJson.contains("Transform");
			if (hasTransform)
			{
				int zIndex = 0; // Default depth.
				if (characterRulesJson["Transform"].contains("ZIndex"))
				{
					const auto& zIndexJson = characterRulesJson["Transform"]["ZIndex"];
					if (zIndexJson.is_number())
					{
						zIndex = zIndexJson.get<int>();
					}
					else if (zIndexJson.is_string())
					{
						// Handle string case if needed
						ENGINE_INFO_D("ZIndex is a string");
					}
				}

				bool drawDebug = false;
				std::string debugColor = "red";
				if (characterRulesJson["Transform"].contains("DrawDebug"))
				{
					drawDebug = true;

					const auto& drawDebugJson = characterRulesJson["Transform"]["DrawDebug"];
					if (drawDebugJson.is_string())
					{
						debugColor = drawDebugJson;
					}
				}

				m_refECS.AddComponent<Transform>(
					tileEntity,
					Vector2D<float>(x * static_cast<float>(numUnitsPerTile),
						y * static_cast<float>(numUnitsPerTile)),
					Vector2D<float>(static_cast<float>(numUnitsPerTile),
						static_cast<float>(numUnitsPerTile)),
					1.0f, 1.0f, 1, zIndex, drawDebug, debugColor
				);
			}

			// check for camera.
			const bool hasCamera = characterRulesJson.contains("Camera");
			if (hasCamera)
			{
				Vector2D<float> size = Vector2D<float>(0.0f, 0.0f);
				Vector2D<float> screenRatio = Vector2D<float>(1.0f, 1.0f);
				Vector2D<float> positionInFractionOfScreenSize = Vector2D<float>(0.0f, 0.0f);
				int pixelsPerUnit = 0;
				bool clampingOn = false;

				if (characterRulesJson["Camera"].contains("Size"))
				{
					const auto& sizeJson = characterRulesJson["Camera"]["Size"];
					if (sizeJson.is_array() && sizeJson.size() == 2)
					{
						if (sizeJson[0].is_number() && sizeJson[1].is_number())
						{
							size.X = sizeJson[0].get<float>();
							size.Y = sizeJson[1].get<float>();
						}
					}
					else
					{
						ENGINE_ERROR_D("Camera size is not an array of two numbers");
					}
				}

				if (characterRulesJson["Camera"].contains("PixelsPerUnit"))
				{
					const auto& pixelsPerUnitJson = characterRulesJson["Camera"]["PixelsPerUnit"];
					if (pixelsPerUnitJson.is_number())
					{
						pixelsPerUnit = pixelsPerUnitJson.get<int>();
					}
					else
					{
						ENGINE_ERROR_D("PixelsPerUnit is not a number");
					}
				}

				if (characterRulesJson["Camera"].contains("ClampingOn"))
				{
					const auto& clampingOnJson = characterRulesJson["Camera"]["ClampingOn"];
					if (clampingOnJson.is_boolean())
					{
						clampingOn = clampingOnJson.get<bool>();
					}
					else
					{
						ENGINE_ERROR_D("ClampingOn is not a boolean");
					}
				}
				if (characterRulesJson["Camera"].contains("Window"))
				{
					const auto& windowJson = characterRulesJson["Camera"]["Window"];
					if (windowJson.contains("X") && windowJson.contains("Y"))
					{
						if (windowJson["X"].is_number() && windowJson["Y"].is_number())
						{
							positionInFractionOfScreenSize.X = windowJson["X"].get<float>();
							positionInFractionOfScreenSize.Y = windowJson["Y"].get<float>();
						}
					}
					else
					{
						ENGINE_ERROR_D("Window is missing X or Y");
					}

					if (windowJson.contains("Width") && windowJson.contains("Height"))
					{
						if (windowJson["Width"].is_number() && windowJson["Height"].is_number())
						{
							screenRatio.X = windowJson["Width"].get<float>();
							screenRatio.Y = windowJson["Height"].get<float>();
						}

					}
				}

				m_refECS.AddComponent<Camera>(tileEntity, size, screenRatio, positionInFractionOfScreenSize, pixelsPerUnit, clampingOn);
			}

			// Add Physics components.
			BodyType bodyType = STATIC;
			Filter category = ALL;
			Filter mask = ALL;
			Vector2D<float> size = Vector2D<float>(static_cast<float>(numUnitsPerTile), static_cast<float>(numUnitsPerTile));
			bool gravityOn = false;
			bool checkSimpleContacts = false;
			bool useChains = false;

			if (characterRulesJson.contains("Physics"))
			{
				json characterPhysicsRulesJson = characterRulesJson["Physics"];

				// Physics library needs to know if the body is enabled or not.
				const bool enabled = activeOnStart;

				if (characterPhysicsRulesJson.contains("Category"))
				{
					const auto& categoryJson = characterPhysicsRulesJson["Category"];
					std::string categoryStr = categoryJson;
					if (categoryStr == "NONE") category = NONE;
					else if (categoryStr == "PLAYER") category = PLAYER;
					else if (categoryStr == "MAP") category = MAP;
					else if (categoryStr == "ALL") category = ALL;
				}
				if (characterPhysicsRulesJson.contains("Mask"))
				{
					const auto& maskJson = characterPhysicsRulesJson["Mask"];
					std::string maskStr = maskJson;
					if (maskStr == "NONE") mask = NONE;
					else if (maskStr == "PLAYER") mask = PLAYER;
					else if (maskStr == "MAP") mask = MAP;
					else if (maskStr == "ALL") mask = ALL;
				}
				if (characterPhysicsRulesJson.contains("BodyType"))
				{
					const auto& bodyTypeJson = characterPhysicsRulesJson["BodyType"];
					std::string bodyTypeStr = bodyTypeJson;
					if (bodyTypeStr == "STATIC") bodyType = STATIC;
					else if (bodyTypeStr == "DYNAMIC") bodyType = DYNAMIC;
					else if (bodyTypeStr == "KINEMATIC") bodyType = KINEMATIC;
					else if (bodyTypeStr == "SENSOR") bodyType = SENSOR;
				}

				if (bodyType != SENSOR)
				{
					// If body is not a sensor, it is solid and collides with other bodies.
					// This information is needed for the chain system to know which tiles to link to.
					// Not the best design but will work for now.
					isSolid.emplace(tileChar);

					if (characterPhysicsRulesJson.contains("UseChains")) // Chains are not used for sensors.
					{
						const auto& useChainsJson = characterPhysicsRulesJson["UseChains"];
						bool useChainsStr = useChainsJson;
						if (useChainsStr == true)
						{
							useChains = true;
						}
					}
				}

				if (characterPhysicsRulesJson.contains("GravityOn"))
				{
					const auto& gravityOnJson = characterPhysicsRulesJson["GravityOn"];
					bool gravityOnStr = gravityOnJson;
					if (gravityOnStr == true)
					{
						gravityOn = true;
					}
				}
				if (characterPhysicsRulesJson.contains("CheckSimpleContacts"))
				{
					const auto& checkSimpleContactsJson = characterPhysicsRulesJson["CheckSimpleContacts"];
					bool checkSimpleContactsStr = checkSimpleContactsJson;
					if (checkSimpleContactsStr == true)
					{
						checkSimpleContacts = true;
					}

				}
				if (characterPhysicsRulesJson.contains("DefaultPhysicsBodySize"))
				{
					const auto& sizeJson = characterPhysicsRulesJson["DefaultPhysicsBodySize"];
					if (sizeJson.is_array() && sizeJson.size() == 2)
					{
						if (sizeJson[0].is_number() && sizeJson[1].is_number())
						{
							size.X = sizeJson[0].get<float>();
							size.Y = sizeJson[1].get<float>();
						}
					}
				}

				if (useChains) // Chains are used on tiles, typically map tiles, to avoid ghose collisions on adjacent tiles.
				{
					auto isTileSolid = [&](int x, int y) -> bool
						{
							const auto* tile = m_tileMap.GetTile(x, y);
							return tile && isSolid.find(tile->second) != isSolid.end();
						};

					bool hasTileAbove = y > 0 && isTileSolid(x, y - 1);
					bool hasTileBelow = y < m_tileMap.GetHeight() - 1 && isTileSolid(x, y + 1);
					bool hasTileLeft = x > 0 && m_tileMap.GetTile(x - 1, y) && isTileSolid(x - 1, y);
					bool hasTileRight = x < m_tileMap.GetWidth() - 1 && isTileSolid(x + 1, y);

					bool hasTileDiagonalLeftAbove = x > 0 && y > 0 && isTileSolid(x - 1, y - 1);
					bool hasTileDiagonalLeftBelow = x > 0 && y < m_tileMap.GetHeight() - 1 && isTileSolid(x - 1, y + 1);
					bool hasTileDiagonalRightAbove = x < m_tileMap.GetWidth() - 1 && y > 0 && isTileSolid(x + 1, y - 1);
					bool hasTileDiagonalRightBelow = x < m_tileMap.GetWidth() - 1 && y < m_tileMap.GetHeight() - 1 && isTileSolid(x + 1, y + 1);

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

						m_refECS.AddComponent<ChainColliderTop>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
							Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
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

						m_refECS.AddComponent<ChainColliderLeft>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
							Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
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

						m_refECS.AddComponent<ChainColliderBottom>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
							Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
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

						m_refECS.AddComponent<ChainColliderRight>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
							Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
					}
				}
				else // Create a regular physics body for the tiles that don't use chains.
				{
					m_refECS.AddComponent<PhysicsBody>(tileEntity, enabled, bodyType, category, mask,
						Vector2D<float>(size.X, size.Y),
						Vector2D<float>(static_cast<float>(x) * static_cast<float>(numUnitsPerTile), static_cast<float>(y) * static_cast<float>(numUnitsPerTile)),
						0.0f, gravityOn, checkSimpleContacts);
				}
			}

			// Add sprite component.
			if (characterRulesJson.contains("SpriteSheet") && characterRulesJson.contains("Animations"))
			{
				const auto& spriteSheetJson = characterRulesJson["SpriteSheet"];
				const auto& animationsJson = characterRulesJson["Animations"];

				std::string spriteSheetPath;
				if (!spriteSheetJson.contains("Path"))
				{
					ENGINE_CRITICAL("No path for sprite sheet found.");
					spriteSheetPath = "default.png"; // Default sprite sheet path.
					continue;
				}

				spriteSheetPath = spriteSheetJson["Path"];

				// ENGINE_INFO_D("Loading texture from: " + spriteSheetPath);
				// The following must only be called once per scene.
				// Currently, it is called for every character that has a animation or sprite from
				// the texture. That takes way too long and wastes resources!!!
				m_refAssetManager.LoadTexture(tileEntity, spriteSheetsPath + spriteSheetPath); // Should this be called on scene play?



				Vector2D<float> frameSize = { 1.0f, 1.0f }; // size of sprite in units.
				if (!spriteSheetJson.contains("SizeInUnits"))
				{
					ENGINE_CRITICAL("No size for sprite sheet found. Defaulting to 1, 1");
				}
				else
				{
					const auto& sizeJson = spriteSheetJson["SizeInUnits"];
					if (sizeJson.is_array() && sizeJson.size() == 2)
					{
						// ENGINE_INFO_D("Found size for sprite sheet");
						if (sizeJson[0].is_number() && sizeJson[1].is_number())
						{
							frameSize.X = sizeJson[0].get<float>();
							frameSize.Y = sizeJson[1].get<float>();
						}
					}
				}

				Vector2D<int> pixelsPerFrame = { 32, 32 }; // size of sprite frame in pixels.
				if (!spriteSheetJson.contains("PixelsPerFrame"))
				{
					ENGINE_CRITICAL("No size for sprite sheet found. Defaulting to 32, 32");
				}
				else
				{
					const auto& pixelsPerFrameJson = spriteSheetJson["PixelsPerFrame"];
					if (pixelsPerFrameJson.is_array() && pixelsPerFrameJson.size() == 2)
					{
						if (pixelsPerFrameJson[0].is_number() && pixelsPerFrameJson[1].is_number())
						{
							pixelsPerFrame.X = pixelsPerFrameJson[0].get<int>();
							pixelsPerFrame.Y = pixelsPerFrameJson[1].get<int>();
						}
					}
				}

				Vector2D<float> offsetFromTransform = { 0.0f, 0.0f };
				if (!spriteSheetJson.contains("OffsetFromTransform"))
				{
					ENGINE_INFO_D("No offset for sprite sheet found. Defaulting to 0, 0");
				}
				else
				{
					const auto& offsetJson = spriteSheetJson["OffsetFromTransform"];
					if (offsetJson.is_array() && offsetJson.size() == 2)
					{
						if (offsetJson[0].is_number() && offsetJson[1].is_number())
						{
							offsetFromTransform.X = offsetJson[0].get<float>();
							offsetFromTransform.Y = offsetJson[1].get<float>();
						}
					}
				}

				Vector2D<size_t> dimensions = { 1, 1 }; // Default dimensions.
				if (spriteSheetJson.contains("Width") && spriteSheetJson.contains("Height"))
				{
					const auto& widthJson = spriteSheetJson["Width"];
					const auto& heightJson = spriteSheetJson["Height"];
					if (widthJson.is_number() && heightJson.is_number())
					{
						dimensions.X = widthJson.get<size_t>();
						dimensions.Y = heightJson.get<size_t>();
					}
				}

				bool drawDebug = false;
				std::string debugColor = "red";
				if (characterRulesJson.contains("SpriteSheet") && characterRulesJson["SpriteSheet"].contains("DrawDebug"))
				{
					drawDebug = true;

					const auto& drawDebugJson = characterRulesJson["SpriteSheet"]["DrawDebug"];
					if (drawDebugJson.is_string())
					{
						debugColor = drawDebugJson;

					}
				}

				// TODO: Get animation data from the JSON file.
				std::unordered_map<std::string, Animation> animations;
				json j = json::parse(characterRulesJson["Animations"].dump());

				auto makeAnimation = [](const std::string& name, const json& jAnim, Vector2D<int> pixelsPerFrame,
					const Vector2D<float> offsetFromTransform, const Vector2D<size_t> dimensions,
					const Vector2D<float> frameSize, bool drawDebug, const std::string& debugColor)
					{
						Animation a;
						a.m_name = name;
						a.m_frames = jAnim.at("Frames").get<std::vector<int>>();
						a.m_numFrames = a.m_frames.size();
						a.m_frameDuration = jAnim.at("FrameTime").get<int>();
						a.m_pixelsPerFrame = pixelsPerFrame;
						a.m_dimensions = dimensions;
						a.m_size = frameSize;
						a.m_offsetFromTransform = offsetFromTransform;
						a.m_loop = jAnim.at("Loop").get<bool>();
						a.m_drawDebug = drawDebug;
						a.m_debugColor = debugColor;

						return a;
					};


				for (auto& [name, value] : j.items())
				{
					animations.emplace(name, makeAnimation(name, value, pixelsPerFrame, offsetFromTransform,
						dimensions, frameSize, drawDebug, debugColor));
				}


				m_refECS.AddComponent<Animations>(tileEntity, animations);
			}

			if (characterRulesJson.contains("AudioSource"))
			{
				const auto& audioJson = characterRulesJson["Audio"];
				if (audioJson.contains("Path"))
				{
					std::string audioPath = audioJson["Path"];
					m_refECS.AddComponent<AudioSource>(tileEntity, audioPath);
				}
			}

			// If the character is "ActiveOnStart", activate it in the ECS.
			// MUST CALL AFTER ALL COMPONENTS CREATED.
			if (activeOnStart)
			{
				m_refECS.Activate(tileEntity);
			}
		}
	}
}