require 'orocos'
require 'vizkit'
include Orocos

Orocos.initialize

if ARGV.size < 1
    puts "usage: test.rb <device_name>"
    exit 0
end

Orocos.run "imu_inemo::Task" => "imu_inemo" do
    imu = TaskContext.get "imu_inemo"

    imu.io_port = "serial://#{ARGV[0]}:19200"
    imu.io_read_timeout = Time.at( 1 ) 
#    imu.io_read_timeout = Types::Base::Time.new( 1.0 )
    imu.configure

    imu.start
    Vizkit.display imu.orientation
    Vizkit.exec
end

