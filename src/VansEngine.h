#pragma once

#include "EngineBasic.h"
#include "ModelLoader.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class VansEngine : public EngineBasic
{
public:
    VansEngine(UINT width, UINT height, std::wstring name);

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();

private:
    static const UINT FrameCount = 2;

	//记录模型数据（暂时，还需要进一步统一格式）
	Model* m_model;


	//顶点数据
    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

	//常量缓冲区对象（矩阵）
	struct ObjectConstants
	{
		XMFLOAT4X4 WorldViewProj = XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	};

	//坐标系变换矩阵
	XMFLOAT4X4 mWorld = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4X4 mView = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4X4 mProj = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//MS quality
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ms_QualityLevels;
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	//用于进行虚拟内存映射
	ObjectConstants objConstants;

    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    UINT m_rtvDescriptorSize;

    // App resources.
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];

	ComPtr<ID3D12Resource> m_constantBuffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC m_constantBufferView;
	UINT8* pConstantDataBegin;	//constant data 起始地址

	//buffer view
	ComPtr<ID3D12DescriptorHeap>m_CbvHeap;	//常量缓冲视图堆（描述符堆）

    // Synchronization objects.
    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;

    void LoadPipeline();
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();


	void CreateDevice(ComPtr<IDXGIFactory4> factory);
	void CreateCommandQueue();
	void CreateSwapChain(ComPtr<IDXGIFactory4> factory);
	void CreateResourceDescriptor();
	void CreateRootSignature();
	void CreatePipeLineState();
	void CreateVerexBuffer();
	void CreateConstantBuffer();
	void CreateFence();
};
