#include "artery/inet/InetMobility.h"
#include "artery/traci/VehicleController.h"
#include <inet/common/ModuleAccess.h>
#include <inet/common/geometry/common/CanvasProjection.h>
#include <inet/features.h>
#include <cmath>

#ifdef WITH_VISUALIZERS
#   include <inet/visualizer/mobility/MobilityCanvasVisualizer.h>
#else
#   include <cstdio>
#endif

Define_Module(InetMobility)


int InetMobility::numInitStages() const
{
    return inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2 + 1;
}

void InetMobility::initialize(int stage)
{
    if (stage == inet::INITSTAGE_LOCAL) {
        mVisualRepresentation = inet::getModuleFromPar<cModule>(par("visualRepresentation"), this, false);
        mAntennaHeight = par("antennaHeight");
        WATCH(mVehicleId);
        WATCH(mPosition);
        WATCH(mSpeed);
        WATCH(mOrientation);
    } else if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2) {
        if (mVisualRepresentation) {
            auto visualizationTarget = mVisualRepresentation->getParentModule();
            mCanvasProjection = inet::CanvasProjection::getCanvasProjection(visualizationTarget->getCanvas());
        }
        emit(MobilityBase::stateChangedSignal, this);
    }
}

double InetMobility::getMaxSpeed() const
{
    return NaN;
}

inet::Coord InetMobility::getCurrentPosition()
{
    return mPosition;
}

inet::Coord InetMobility::getCurrentVelocity()
{
    return mSpeed;
}

inet::Coord InetMobility::getCurrentAcceleration()
{
    return inet::Coord::NIL;
}

inet::EulerAngles InetMobility::getCurrentAngularPosition()
{
    return mOrientation;
}

inet::EulerAngles InetMobility::getCurrentAngularVelocity()
{
    return inet::EulerAngles::NIL;
}

inet::EulerAngles InetMobility::getCurrentAngularAcceleration()
{
    return inet::EulerAngles::NIL;
}

inet::Coord InetMobility::getConstraintAreaMax() const
{
    return inet::Coord { mNetBoundary.xMax, mNetBoundary.yMax, mNetBoundary.zMax };
}

inet::Coord InetMobility::getConstraintAreaMin() const
{
    return inet::Coord { mNetBoundary.xMin, mNetBoundary.yMin, mNetBoundary.zMin };
}

void InetMobility::initialize(const Position& pos, Angle heading, double speed)
{
    using boost::units::si::meter;
    const double heading_rad = heading.radian();
    const inet::Coord direction { cos(heading_rad), -sin(heading_rad) };
    mPosition = inet::Coord { pos.x / meter, pos.y / meter, mAntennaHeight };
    mSpeed = direction * speed;
    mOrientation.alpha = inet::units::values::rad(-heading_rad);
}

void InetMobility::update(const Position& pos, Angle heading, double speed)
{
    initialize(pos, heading, speed);
    ASSERT(inet::IMobility::mobilityStateChangedSignal == MobilityBase::stateChangedSignal);
    emit(MobilityBase::stateChangedSignal, this);
}

void InetMobility::refreshDisplay() const
{
    // following code is taken from INET's MobilityBase::refreshDisplay
    if (mVisualRepresentation) {
        auto position = mCanvasProjection->computeCanvasPoint(mPosition);
        char buf[32];
        snprintf(buf, sizeof(buf), "%lf", position.x);
        buf[sizeof(buf) - 1] = 0;
        mVisualRepresentation->getDisplayString().setTagArg("p", 0, buf);
        snprintf(buf, sizeof(buf), "%lf", position.y);
        buf[sizeof(buf) - 1] = 0;
        mVisualRepresentation->getDisplayString().setTagArg("p", 1, buf);
    }
}
