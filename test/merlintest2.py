import PyTango
import numpy
import time
import random

dev=PyTango.DeviceProxy('merlin/tango/1')
print "Software  version     :", dev.read_attribute("softwareversion").value

nframes = 10
exp_time = 0.5

lima=PyTango.DeviceProxy('limaccd/tango/3')
# do not change the order of the saving attributes!
lima.write_attribute("saving_directory","/home/grm84/data")
lima.write_attribute("saving_format","HDF5")
lima.write_attribute("saving_overwrite_policy","Overwrite")
lima.write_attribute("saving_suffix", ".hdf")
lima.write_attribute("saving_prefix","merlin_")
lima.write_attribute("saving_mode","AUTO_FRAME")
lima.write_attribute("saving_frames_per_file", nframes)

# do acquisition
lima.write_attribute("acq_nb_frames",nframes)
lima.write_attribute("latency_time",0.01)
lima.write_attribute("acq_expo_time",exp_time)
lima.write_attribute("acq_trigger_mode", "INTERNAL_TRIGGER")

lima.command_inout("prepareAcq")
start = time.time()
lima.command_inout("startAcq")
print "Running "
while lima.read_attribute("acq_status").value == "Running" :
    time.sleep(.2)

print "Completed run in ", time.time()-start, "secs"

for i in range(1000000):
    lima.command_inout("prepareAcq")
    start = time.time()
    lima.command_inout("startAcq")
    print "running"
    time.sleep(random.uniform(0.1, nframes*exp_time-0.1))
    lima.command_inout("stopAcq")       
    print "Stopped in run ", i, " after ", time.time()-start, "secs"

    while lima.read_attribute("acq_status").value == "Running"  :
        print "still waiting to stop", dev.read_attribute("acqRunning").value
        time.sleep(.1)

    print "Completely stopped run ", i, " after ", time.time()-start, "secs"

print "Done"
