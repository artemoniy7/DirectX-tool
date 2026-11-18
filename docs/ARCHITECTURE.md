# DXTool architecture

## Design goals

DXTool is a hybrid Direct3D 12 framework, not a full game engine.

- Keep common tasks simple through a high-level C++ API.
- Preserve access to native D3D12 and DXGI interfaces.
- Use RAII for ownership and deterministic resource lifetime.
- Keep platform-specific implementation details out of public headers where practical.

## Modules

- Core: logging, errors, assertions, and shared types.
- Platform: Windows-specific implementation details.
- Window: window creation and event processing.
- Graphics: Direct3D 12 device, queues, swap chain, descriptors, and resource management.
- Renderer: frame lifecycle and rendering operations built on Graphics.
- Resources: textures, buffers, shaders, and meshes.
- Input: keyboard and mouse state.
- Math: vectors, matrices, and helper functions.
- Utils: small reusable utilities.

## Dependency direction

The intended dependency direction is:

Platform -> Window -> Graphics -> Renderer -> higher-level application systems

Core and Math should remain broadly reusable and avoid depending on rendering systems.

## Native DirectX access

The public API will expose carefully chosen native handles or accessor methods for advanced use. DXTool will not force users to abandon Direct3D 12 concepts such as command lists, descriptor heaps, or resource barriers.
