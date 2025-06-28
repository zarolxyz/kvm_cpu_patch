# kvm_cpu_patch

这是一个KVM内核模块补丁，主要用于避基于CPU的虚拟机检测，和提升虚拟机CPU性能。目前仅支持Intel处理器。

# 特性

- 使得虚拟机内部正常显示核心温度，频率，电压等，提供更加真实的环境。
- 暴露主机拓展拓扑信息，虚拟机可正确识别核心间的差异，如大小核心，超线程等，操作系统可正确调度，大大提升了虚拟机内的CPU多核性能，同时避免了基于线程匹配的虚拟机检测。
- 直通Intel特有的V2拓展拓扑信息，修复了Intel处理器无法开启Hyper-V嵌套虚拟化的问题。
- 提供虚拟机对宿主机处理器的性能控制MSR，如果宿主机支持，虚拟机可正常超频处理器(使用throttlestop)。

# 编译

下载linux内核源码(以6.15.4为基础修改)，用本项目的文件替换linux源码中的kvm模块，然后编译kvm源码即可。

# 使用

安装编译好的kvm模块。
请注意libvirt启动参数：
必须在xml中 1:1 绑定VCPU与主机逻辑CPU，否则虚拟机不可能启动。
以i9 13900k为例，它有24个核心，32个线程，无需关心核心数量，也无需关注大小核心差异，xml中应添加如下内容：

<cputune>
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
  </cputune>

  kvm补丁将忽略qemu设置的拓扑信息而强制与宿主保持一致，xml中可以这样设置拓扑：

  <topology sockets="1" cores="1" threads="32"/>

  即全部核心为逻辑核心。

  若要开启Hyper V嵌套虚拟化，无需额外添加-level=30来降级cpuid，此参数会被忽略，补丁会直通HyperV嵌套所需的cpuid信息。
