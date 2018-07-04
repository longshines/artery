#ifndef INETMOBILITY_H_SKZPGILS
#define INETMOBILITY_H_SKZPGILS

#include "artery/traci/MobilityBase.h"
#include <inet/mobility/contract/IMobility.h>
#include <omnetpp/csimplemodule.h>

namespace inet { class CanvasProjection; }


class InetMobility : public inet::IMobility, public MobilityBase, public omnetpp::cSimpleModule
{
public:
    // inet::IMobility interface
    double getMaxSpeed() const override;
    inet::Coord getCurrentPosition() override;
    inet::Coord getCurrentVelocity() override;
    inet::Coord getCurrentAcceleration() override;
    inet::EulerAngles getCurrentAngularPosition() override;
    inet::EulerAngles getCurrentAngularVelocity() override;
    inet::EulerAngles getCurrentAngularAcceleration() override;
    inet::Coord getConstraintAreaMax() const override;
    inet::Coord getConstraintAreaMin() const override;

    // omnetpp::cSimpleModule
    void initialize(int stage) override;
    int numInitStages() const override;

protected:
    void refreshDisplay() const override;

private:
    void initialize(const Position& pos, Angle heading, double speed) override;
    void update(const Position& pos, Angle heading, double speed) override;

    inet::Coord mPosition;
    inet::Coord mSpeed;
    inet::EulerAngles mOrientation;
    double mAntennaHeight;
    omnetpp::cModule* mVisualRepresentation = nullptr;
    const inet::CanvasProjection* mCanvasProjection = nullptr;
};

#endif /* INETMOBILITY_H_SKZPGILS */
