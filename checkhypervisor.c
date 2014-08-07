/*  Copyright (c) 2014 Cloudbase Solutions Srl.
*   All Rights Reserved.
*
*    Licensed under the Apache License, Version 2.0 (the "License"); you may
*    not use this file except in compliance with the License. You may obtain
*    a copy of the License at
*
*         http://www.apache.org/licenses/LICENSE-2.0
*
*    Unless required by applicable law or agreed to in writing, software
*    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
*    License for the specific language governing permissions and limitations
*    under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <intrin.h>
#endif

void get_cpuid(unsigned leaf, unsigned* regs)
{
#ifdef _WIN32
    __cpuid(regs, leaf);
#else
    __asm__ __volatile__ ("cpuid": "=a" (regs[0]), "=b" (regs[1]),
                          "=c" (regs[2]), "=d" (regs[3]) : "a" (leaf));
#endif
}

/*
Hyper-V specs: http://www.microsoft.com/en-us/download/confirmation.aspx?id=39289
VMWare specs: http://kb.vmware.com/selfservice/microsites/search.do?language=en_US&cmd=displayKC&externalId=1009458
*/
int get_hypervisor_id(char* id)
{
    unsigned regs[] = {0, 0, 0, 0};
    id[0] = 0;

    get_cpuid(1, regs);
    if(regs[2] & (1 << 31))
    {
        memset(regs, 0, sizeof(regs));
        get_cpuid(0x40000000, regs);
        memcpy(id, &regs[1], 12);
        id[12] = 0;

        return 1;
    }

    return 0;
}

void main()
{
    char id[13] = {0};
    if(get_hypervisor_id(id))
    {
        printf("%s\n", id);
        exit(0);
    }
    else
    {
        printf("No hypervisor detected\n");
        exit(1);
    }
}
