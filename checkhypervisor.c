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

#include <cpuid.h>
#include <stdio.h>
#include <stdlib.h>

int get_hypervisor_id(char* id)
{
    unsigned int eax = 0;
    unsigned int ebx = 0;
    unsigned int ecx = 0;
    unsigned int edx = 0;
    
    id[0] = 0;

    if(__get_cpuid(1, &eax, &ebx, &ecx, &edx) && (ecx & (1 << 31)))
    {
        if(__get_cpuid(0x40000000, &eax, &ebx, &ecx, &edx))
        {
            *((unsigned int*)id) = ebx;
            *((unsigned int*)id + 1) = ecx;
            *((unsigned int*)id + 2) = edx;
            id[12] = 0;

            return 1;
        }
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

