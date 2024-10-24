import m5
from m5.objects import *

# Add the common scripts to our path
m5.util.addToPath("../../")
from common.FileSystemConfig import config_filesystem

# import the caches which we made
from learning_gem5.part1.caches import *

# Define number of cores
NUM_CORES = 2

# Create system
system = System()

# Create clock domain
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '3GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set RAM size to 8GB
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('8GB')]

# Create memory bus
system.membus = SystemXBar()

# Create x86 timing cores
system.cpu = [X86TimingSimpleCPU() for _ in range(NUM_CORES)]

# Create L2 buses
system.l2bus = [L2XBar() for _ in range(NUM_CORES)]

# Create L2 Caches
system.l2cache = [L2Cache() for _ in range(NUM_CORES)]

# Repeat for each core
for i in range(NUM_CORES):

    # Create L1 cache
    system.cpu[i].icache = L1ICache()
    system.cpu[i].dcache = L1DCache()

    # Connect core to L1 cache
    system.cpu[i].icache.connectCPU(system.cpu[i])
    system.cpu[i].dcache.connectCPU(system.cpu[i])

    # Connect L2 bus to L1 cache
    system.cpu[i].icache.connectBus(system.l2bus[i])
    system.cpu[i].dcache.connectBus(system.l2bus[i])

    # Connect L2 cache to memory bus
    system.l2cache[i].connectCPUSideBus(system.l2bus[i])
    system.l2cache[i].connectMemSideBus(system.membus)

    # Create interrupt controller
    system.cpu[i].createInterruptController()

    # Connect the parellel I/O ports
    system.cpu[i].interrupts[0].pio = system.membus.mem_side_ports
    system.cpu[i].interrupts[0].int_requestor = system.membus.cpu_side_ports
    system.cpu[i].interrupts[0].int_responder = system.membus.mem_side_ports

# Connect system port to memory bus to allow for reading and writing to memory
system.system_port = system.membus.cpu_side_ports

# Create memory controller and DRAM configuration
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR4_2400_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

# Set path to binary file
binary = 'tests/test-progs/sum-3/bin/x86/linux/sum-3'

# for gem5 V21 and beyond
system.workload = SEWorkload.init_compatible(binary)

# Create processes with specified binary path
process = Process()
process.cmd = [binary]
for cpu in system.cpu:
    cpu.workload = process
    cpu.createThreads()

# Set up the pseudo file system for the threads function above
config_filesystem(system)

# Instantiate root system
root = Root(full_system = False, system = system)
m5.instantiate()

# Begin simulation
print("Beginning simulation!")
exit_event = m5.simulate()

# Inspect system state
print('Exiting @ tick {} because {}'
      .format(m5.curTick(), exit_event.getCause()))
