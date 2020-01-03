#include <cassert>

#include "NavJumpConstraint.h"

namespace urchin
{
    NavJumpConstraint::NavJumpConstraint(float sourceEdgeJumpStartRange, float sourceEdgeJumpEndRange, float targetEdgeIndex) :
            sourceEdgeJumpStartRange(sourceEdgeJumpStartRange),
            sourceEdgeJumpEndRange(sourceEdgeJumpEndRange),
            targetEdgeIndex(targetEdgeIndex)
    {
        assert(targetEdgeIndex <= 2);
        assert(sourceEdgeJumpStartRange >= sourceEdgeJumpEndRange);
    }

    float NavJumpConstraint::getSourceEdgeJumpStartRange() const
    {
        return sourceEdgeJumpStartRange;
    }

    float NavJumpConstraint::getSourceEdgeJumpEndRange() const
    {
        return sourceEdgeJumpEndRange;
    }

    LineSegment3D<float> NavJumpConstraint::computeSourceJumpEdge(const LineSegment3D<float> &sourceEdge) const
    {
        return LineSegment3D<float>(
                sourceEdgeJumpStartRange * sourceEdge.getA() + (1.0f - sourceEdgeJumpStartRange) * sourceEdge.getB(),
                sourceEdgeJumpEndRange * sourceEdge.getA() + (1.0f - sourceEdgeJumpEndRange) * sourceEdge.getB());
    }

    float NavJumpConstraint::getTargetEdgeIndex() const
    {
        return targetEdgeIndex;
    }
}