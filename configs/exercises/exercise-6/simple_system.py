import m5
from m5.objects import *

# Add the common scripts to our path
m5.util.addToPath("../../")

# import the caches which we made
from learning_gem5.part1.caches import *

# Create system
system = System()

# Create clock domain
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '3GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set RAM size to 8GB
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('8GB')]

# Create a simple x86 timing CPU core
system.cpu = X86TimingSimpleCPU()

# Create memory bus
system.membus = SystemXBar()

# Create L1 cache
system.cpu.icache = L1ICache()
system.cpu.dcache = L1DCache()

# Connect CPU to L1 cache
system.cpu.icache.connectCPU(system.cpu)
system.cpu.dcache.connectCPU(system.cpu)

# Connect L1 cache to memory bus
system.cpu.icache.connectBus(system.membus)
system.cpu.dcache.connectBus(system.membus)

# Create interrupt controller
system.cpu.createInterruptController()

# Connect the parellel I/O ports
system.cpu.interrupts[0].pio = system.membus.mem_side_ports
system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

# Connect system port to memory bus to allow for reading and writing to memory
system.system_port = system.membus.cpu_side_ports

# Create memory controller and DRAM configuration
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR4_2400_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

# Set path to binary file
binary = 'tests/test-progs/floating-point/bin/x86/linux/floating-point'

# for gem5 V21 and beyond
system.workload = SEWorkload.init_compatible(binary)

# Create process with specified binary path
process = Process()
process.cmd = [binary]
system.cpu.workload = process
system.cpu.createThreads()

# Instantiate root system
root = Root(full_system = False, system = system)
exit_event = m5.instantiate()

# Begin simulation
print("Beginning simulation!")

exit_event = m5.simulate()

# Inspect system state
print('Exiting @ tick {} because {}'
      .format(m5.curTick(), exit_event.getCause()))