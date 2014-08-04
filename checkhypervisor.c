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

void get_cpuid(unsigned cpuid, unsigned* eax, unsigned* ebx, unsigned* ecx, unsigned* edx) 
{
    unsigned a = *eax;
    unsigned b = *ebx;
    unsigned c = *ecx;
    unsigned d = *edx;

    __asm__ __volatile__ ("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (cpuid));

    *eax = a;
    *ebx = b;
    *ecx = c;
    *edx = d;
}

/*
Hyper-V specs: http://www.microsoft.com/en-us/download/confirmation.aspx?id=39289
VMWare specs: http://kb.vmware.com/selfservice/microsites/search.do?language=en_US&cmd=displayKC&externalId=1009458
*/
int get_hypervisor_id(char* id)
{
    unsigned eax = 0;
    unsigned ebx = 0;
    unsigned ecx = 0;
    unsigned edx = 0;
    
    id[0] = 0;

    get_cpuid(1, &eax, &ebx, &ecx, &edx);
    if(ecx & (1 << 31))
    {
        eax = 0;
        ebx = 0;
        ecx = 0;
        edx = 0;

        get_cpuid(0x40000000, &eax, &ebx, &ecx, &edx);
        *((unsigned*)id) = ebx;
        *((unsigned*)id + 1) = ecx;
        *((unsigned*)id + 2) = edx;
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

