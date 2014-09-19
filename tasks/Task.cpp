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

    base::samples::RigidBodyState rbs;
    rbs.orientation = 
        base::Quaterniond( 
                data.quat[0], data.quat[1], data.quat[2], data.quat[3] );

    _orientation.write( rbs );
}
