/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using namespace imu_inemo;

Task::Task(std::string const& name)
    : TaskBase(name), mDriver(NULL)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine), mDriver(NULL)
{
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    mDriver = new Driver();
    if( !_io_port.get().empty() )
        mDriver->openURI( _io_port.get() );
    setDriver( mDriver );

    if (! TaskBase::configureHook())
        return false;

    mDriver->connect();

    imu_inemo::output_mode mode;
    mode.setMAG();
    mode.setACC();
    mode.setGYRO();
    mode.setAHRS();
    mode.setFrequency( imu_inemo::output_mode::HZ_100 );
    mDriver->setOutputMode( mode );

    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;

    mDriver->startAcquisition();
    return true;
}
void Task::updateHook()
{
    TaskBase::updateHook();
}
void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    mDriver->stopAcquisition();
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    mDriver->disconnect();

    TaskBase::cleanupHook();
    delete mDriver;
}
void Task::processIO()
{
    imu_inemo::sensor_data data;
    mDriver->getSensorData( data );

    // write the orientation from the AHRS
    base::samples::RigidBodyState rbs;
    rbs.time = base::Time::now();
    rbs.orientation = data.getOrientation();

    _orientation.write( rbs );

    // write the raw sensor data
    base::samples::IMUSensors sensors;
    sensors.time = base::Time::now();
    sensors.acc = data.getAccelerometer();
    sensors.gyro = data.getGyro();
    sensors.mag = data.getMagnetometer();

    _sensors.write( sensors );
}
