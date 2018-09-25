#include <stdio.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>



int main(){
    
    MonoDomain *domain;
    domain=mono_jit_init(domain_name);
    MonoAssembly *assembly;
    assembly=mono_domain_assembly_open();
    if(!assembly)
        error();
    retval =mono_jit_exec(domain,assembly,argc-1,argv+1);
    
    return 0;
}
