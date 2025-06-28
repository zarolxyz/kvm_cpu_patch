// arch/x86/kvm/cpuid.c

u32 passthrough_cpuid_funcs[] = { 0x1f,	      0x02,	  0x04,
				  0x05,	      0x06,	  0x07,
				  0x14,	      0x80000000, 0x80000002,
				  0x80000003, 0x80000004, 0x80000005,
				  0x80000006, 0x80000007, 0x80000021 };

int is_cpuid_passthrough(u32 function);

int is_cpuid_passthrough(u32 function)
{
	int i;

	for (i = 0; i < sizeof(passthrough_cpuid_funcs) /
				sizeof(passthrough_cpuid_funcs[0]);
	     i++)
		if (function == passthrough_cpuid_funcs[i])
			return 1;

	return 0;
}

int kvm_emulate_cpuid(struct kvm_vcpu *vcpu)
{
	u32 eax, ebx, ecx, edx;

	if (cpuid_fault_enabled(vcpu) && !kvm_require_cpl(vcpu, 0))
		return 1;

	eax = kvm_rax_read(vcpu);
	ecx = kvm_rcx_read(vcpu);
	u32 eax_bak = eax;
	u32 ecx_bak = ecx;
	kvm_cpuid(vcpu, &eax, &ebx, &ecx, &edx, false);
	if (is_cpuid_passthrough(eax)) {
		asm volatile("cpuid\n"
			     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
			     : "a"(eax_bak), "c"(ecx_bak)
			     : "memory");
	}
	kvm_rax_write(vcpu, eax);
	kvm_rbx_write(vcpu, ebx);
	kvm_rcx_write(vcpu, ecx);
	kvm_rdx_write(vcpu, edx);
	return kvm_skip_emulated_instruction(vcpu);
}
