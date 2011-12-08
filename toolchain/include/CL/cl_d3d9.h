// Copyright (c) 2006-2007 Intel Corporation
// All rights reserved.
//
// WARRANTY DISCLAIMER
//
// THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
// MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Intel Corporation is the author of the Materials, and requests that all
// problem reports or change requests be submitted to it directly

#pragma once

#if defined (_WIN32)
#include <wtypes.h>
#include <d3d9types.h>
#include <d3d9.h>
#endif
#include "cl_platform.h"
#include "cl.h"

enum cl_d3d9_device_source_intel
{
    CL_D3D9_DEVICE_INTEL       = 0x4022,
    CL_D3D9_ADAPTER_NAME_INTEL = 0x4023
};

enum cl_d3d9_device_set_intel
{
    CL_PREFERRED_DEVICES_FOR_D3D9_INTEL = 0x4024,
    CL_ALL_DEVICES_FOR_D3D9_INTEL       = 0x4025
};

/* Additional cl_context_properties  */
#define CL_CONTEXT_D3D9_DEVICE_INTEL             0x4026

/* Paramters of clGetMemObjectInfo */

#define CL_MEM_D3D9_RESOURCE_INTEL               0x4027

/* Paramters of clGetImageInfo */

#define CL_IMAGE_D3D9_FACE_INTEL                 0x4028
#define CL_IMAGE_D3D9_LEVEL_INTEL                0x4029

/* Command types */

#define CL_COMMAND_ACQUIRE_D3D9_OBJECTS_INTEL    0x402A
#define CL_COMMAND_RELEASE_D3D9_OBJECTS_INTEL    0x402B

/* Additional error codes */

#define CL_INVALID_D3D9_DEVICE_INTEL             -1010
#define CL_INVALID_D3D9_RESOURCE_INTEL           -1011
#define CL_D3D9_RESOURCE_ALREADY_ACQUIRED_INTEL  -1012
#define CL_D3D9_RESOURCE_NOT_ACQUIRED_INTEL      -1013

/* Object types */

#define CL_D3D9_OBJECT_VERTEX_BUFFER          0x3000
#define CL_D3D9_OBJECT_INDEX_BUFFER           0x3001
#define CL_D3D9_OBJECT_SURFACE                0x3002
#define CL_D3D9_OBJECT_TEXTURE                0x3003
#define CL_D3D9_OBJECT_CUBE_TEXTURE           0x3004
#define CL_D3D9_OBJECT_VOLUME_TEXTURE         0x3005

#if defined (_WIN32)
/* API functions */

typedef CL_API_ENTRY cl_mem (CL_API_CALL *clCreateFromD3D9VertexBufferIntel_fn)(
    cl_context /*context*/,
    cl_mem_flags /*flags*/,
    IDirect3DVertexBuffer9* /*resource*/,
    cl_int* /*errcode_ret*/);

typedef CL_API_ENTRY cl_mem (CL_API_CALL *clCreateFromD3D9IndexBufferIntel_fn)(
    cl_context /*context*/,
    cl_mem_flags /*flags*/,
    IDirect3DIndexBuffer9* /*resource*/,
    cl_int* /*errcode_ret*/);

typedef CL_API_ENTRY cl_mem (CL_API_CALL *clCreateFromD3D9SurfaceIntel_fn)(
    cl_context /*context*/,
    cl_mem_flags /*flags*/,
    IDirect3DSurface9* /*resource*/,
    cl_int* /*errcode_ret*/);

typedef CL_API_ENTRY cl_mem (CL_API_CALL *clCreateFromD3D9TextureIntel_fn)(
    cl_context /*context*/,
    cl_mem_flags /*flags*/,
    IDirect3DTexture9* /*resource*/,
    UINT /*miplevel*/,
    cl_int* /*errcode_ret*/);

typedef CL_API_ENTRY cl_mem (CL_API_CALL *clCreateFromD3D9CubeTextureIntel_fn)(
    cl_context /*context*/,
    cl_mem_flags /*flags*/,
    IDirect3DCubeTexture9* /*resource*/,
    D3DCUBEMAP_FACES /*facetype*/,
    UINT /*miplevel*/,
    cl_int* /*errcode_ret*/);

typedef CL_API_ENTRY cl_mem (CL_API_CALL *clCreateFromD3D9VolumeTextureIntel_fn)(
    cl_context /*context*/,
    cl_mem_flags /*flags*/,
    IDirect3DVolumeTexture9* /*resource*/,
    UINT /*miplevel*/,
    cl_int* /*errcode_ret*/);

typedef CL_API_ENTRY cl_int (CL_API_CALL *clEnqueueAcquireD3D9ObjectsIntel_fn)(
    cl_command_queue /*command_queue*/,
    cl_uint /*num_objects*/,
    const cl_mem * /*mem_objects*/,
    cl_uint /*num_events_in_wait_list*/,
    const cl_event * /*event_wait_list*/,
    cl_event * /*event*/);

typedef CL_API_ENTRY cl_int (CL_API_CALL *clEnqueueReleaseD3D9ObjectsIntel_fn)(
    cl_command_queue /*command_queue*/,
    cl_uint /*num_objects*/,
    cl_mem * /*mem_objects*/,
    cl_uint /*num_events_in_wait_list*/,
    const cl_event * /*event_wait_list*/,
    cl_event * /*event*/);

typedef CL_API_ENTRY cl_int (CL_API_CALL* clGetDeviceIDsFromD3D9Intel_fn)(
    cl_platform_id /*platform*/,
    enum cl_d3d9_device_source_intel /*d3d_device_source*/,
    void* /*d3d_object*/,
    enum cl_d3d9_device_set_intel /*d3d_device_set*/,
    cl_uint /*num_entries*/, 
    cl_device_id* /*devices*/, 
    cl_uint* /*num_devices*/);
#endif
