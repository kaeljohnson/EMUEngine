#pragma once

#include <string>
#include <random>
#include <sstream>
#include <unordered_set>

#include "Core.h"

namespace Engine
{
    class IDGenerator 
    {
    public:
        static void initialize(size_t maxID) 
        {
            IDGenerator::maxID = maxID;
        }

        EMU_API static size_t CreateUUID() 
        {
            if (usedIDs.size() >= maxID + 1) 
            {
                throw std::runtime_error("Error: All IDs are taken.");
            }

            for (size_t id = 0; id <= maxID; ++id) 
            {
                if (usedIDs.find(id) == usedIDs.end()) 
                {
                    usedIDs.insert(id);
                    return id;
                }
            }

            // This point should never be reached due to the earlier check
            throw std::runtime_error("Error: Unable to generate a new ID.");
        }

        static void releaseID(size_t id) 
        {
            usedIDs.erase(id);
        }

    private:
        static size_t maxID;
        static std::unordered_set<size_t> usedIDs;
    };
}