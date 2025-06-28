# kvm_cpu_patch

This KVM kernel module patch helps evade CPU-based virtual machine detection and enhances CPU performance in virtual machines. **Currently supports Intel processors only**.

## Features

- **Authentic Hardware Monitoring**  
  Displays actual core temperature, frequency, voltage, etc., creating a more realistic virtual environment.

- **Host Topology Exposure**  
  Reveals detailed host topology (e.g., P-Cores/E-Cores, Hyper-Threading), correct OS scheduling. Significantly improves multi-core performance and avoids VM detection based on thread/core mismatches.

- **Intel V2 Topology Passthrough**  
  Directly passes Intel-specific V2 Extended Topology information, fixing Hyper-V nested virtualization failures on Intel CPUs.

- **Performance Control MSR Passthrough**  
  Grants VMs access to host performance control MSRs. If supported by the host, VMs can overclock CPUs (e.g., via `throttlestop`).

## Compilation

1. Download Linux kernel source code (this patch is based on `6.15.4`).  
2. Replace KVM module files in the kernel source tree with files from this project.  
3. Compile the KVM module.


## Usage

### Step 1: Install the compiled KVM module

### Step 2: Configure Libvirt XML
**Mandatory: 1:1 vCPU-to-host-logical-CPU binding**  
Example for Intel i9-13900K (24 cores, 32 threads):

    <vcpupin vcpu="0" cpuset="0"/>
    <vcpupin vcpu="1" cpuset="1"/>
    <vcpupin vcpu="2" cpuset="2"/>
    <vcpupin vcpu="3" cpuset="3"/>
    <vcpupin vcpu="4" cpuset="4"/>
    <vcpupin vcpu="5" cpuset="5"/>
    <vcpupin vcpu="6" cpuset="6"/>
    <vcpupin vcpu="7" cpuset="7"/>
    <vcpupin vcpu="8" cpuset="8"/>
    <vcpupin vcpu="9" cpuset="9"/>
    <vcpupin vcpu="10" cpuset="10"/>
    <vcpupin vcpu="11" cpuset="11"/>
    <vcpupin vcpu="12" cpuset="12"/>
    <vcpupin vcpu="13" cpuset="13"/>
    <vcpupin vcpu="14" cpuset="14"/>
    <vcpupin vcpu="15" cpuset="15"/>
    <vcpupin vcpu="16" cpuset="16"/>
    <vcpupin vcpu="17" cpuset="17"/>
    <vcpupin vcpu="18" cpuset="18"/>
    <vcpupin vcpu="19" cpuset="19"/>
    <vcpupin vcpu="20" cpuset="20"/>
    <vcpupin vcpu="21" cpuset="21"/>
    <vcpupin vcpu="22" cpuset="22"/>
    <vcpupin vcpu="23" cpuset="23"/>
    <vcpupin vcpu="24" cpuset="24"/>
    <vcpupin vcpu="25" cpuset="25"/>
    <vcpupin vcpu="26" cpuset="26"/>
    <vcpupin vcpu="27" cpuset="27"/>
    <vcpupin vcpu="28" cpuset="28"/>
    <vcpupin vcpu="29" cpuset="29"/>
    <vcpupin vcpu="30" cpuset="30"/>
    <vcpupin vcpu="31" cpuset="31"/>

**Note**: The patch ignores QEMU topology settings and enforces host topology alignment. All vCPUs should be configured as logical cores.

    <topology sockets="1" dies="1" cores="1" threads="32"/>

