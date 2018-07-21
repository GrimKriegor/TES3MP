#ifndef OPENMW_COMPONENTS_DETOURNAVIGATOR_NAVIGATOR_H
#define OPENMW_COMPONENTS_DETOURNAVIGATOR_NAVIGATOR_H

#include "findsmoothpath.hpp"
#include "flags.hpp"
#include "navmeshmanager.hpp"
#include "settings.hpp"
#include "settingsutils.hpp"

namespace DetourNavigator
{
    struct ObjectShapes
    {
        const btCollisionShape& mShape;
        const btCollisionShape* mAvoid;

        ObjectShapes(const btCollisionShape& shape, const btCollisionShape* avoid)
            : mShape(shape), mAvoid(avoid)
        {}
    };

    class Navigator
    {
    public:
        Navigator(const Settings& settings);

        void addAgent(const osg::Vec3f& agentHalfExtents);

        void removeAgent(const osg::Vec3f& agentHalfExtents);

        bool addObject(std::size_t id, const btCollisionShape& shape, const btTransform& transform);

        bool addObject(std::size_t id, const ObjectShapes& shapes, const btTransform& transform);

        bool updateObject(std::size_t id, const btCollisionShape& shape, const btTransform& transform);

        bool updateObject(std::size_t id, const ObjectShapes& shapes, const btTransform& transform);

        bool removeObject(std::size_t id);

        bool addWater(const osg::Vec2i& cellPosition, const int cellSize, const btScalar level,
            const btTransform& transform);

        bool removeWater(const osg::Vec2i& cellPosition);

        void update(const osg::Vec3f& playerPosition);

        void wait();

        template <class OutputIterator>
        OutputIterator findPath(const osg::Vec3f& agentHalfExtents, const osg::Vec3f& start,
            const osg::Vec3f& end, const Flags includeFlags, OutputIterator out) const
        {
            const auto navMesh = mNavMeshManager.getNavMesh(agentHalfExtents);
            return findSmoothPath(*navMesh.lock(), toNavMeshCoordinates(mSettings, agentHalfExtents),
                toNavMeshCoordinates(mSettings, start), toNavMeshCoordinates(mSettings, end), includeFlags,
                mSettings, out);
        }

        std::map<osg::Vec3f, std::shared_ptr<NavMeshCacheItem>> getNavMeshes() const;

        const Settings& getSettings() const;

    private:
        Settings mSettings;
        NavMeshManager mNavMeshManager;
        std::map<osg::Vec3f, std::size_t> mAgents;
        std::unordered_map<std::size_t, std::size_t> mAvoidIds;
        std::unordered_map<std::size_t, std::size_t> mWaterIds;

        void updateAvoidShapeId(const std::size_t id, const std::size_t avoidId);
        void updateWaterShapeId(const std::size_t id, const std::size_t waterId);
        void updateId(const std::size_t id, const std::size_t waterId, std::unordered_map<std::size_t, std::size_t>& ids);
    };
}

#endif
