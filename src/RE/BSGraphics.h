#pragma once
#include "DirectXMath.h"
#include <d3d11.h>

#define AutoPtr(Type, Name, OffsetSE, OffsetAE) static Type& Name = (*(Type*)RELOCATION_ID(OffsetSE, OffsetAE).address())
namespace BSGraphics
{
	struct alignas(16) ViewData
	{
		DirectX::XMVECTOR m_ViewUp;
		DirectX::XMVECTOR m_ViewRight;
		DirectX::XMVECTOR m_ViewDir;
		DirectX::XMMATRIX m_ViewMat;
		DirectX::XMMATRIX m_ProjMat;
		DirectX::XMMATRIX m_ViewProjMat;
		DirectX::XMMATRIX m_UnknownMat1;
		DirectX::XMMATRIX m_ViewProjMatrixUnjittered;
		DirectX::XMMATRIX m_PreviousViewProjMatrixUnjittered;
		DirectX::XMMATRIX m_ProjMatrixUnjittered;
		DirectX::XMMATRIX m_UnknownMat2;
		float             m_ViewPort[4];  // NiRect<float> { left = 0, right = 1, top = 1, bottom = 0 }
		RE::NiPoint2      m_ViewDepthRange;
		char              _pad0[0x8];
	};
	static_assert(sizeof(ViewData) == 0x250);

	struct CameraStateData
	{
		RE::NiCamera* pReferenceCamera;
		ViewData      CamViewData;
		RE::NiPoint3  PosAdjust;
		RE::NiPoint3  CurrentPosAdjust;
		RE::NiPoint3  PreviousPosAdjust;
		bool          UseJitter;
		char          _pad0[0x8];
	};
	static_assert(sizeof(CameraStateData) == 0x290);

	//struct State
	//{
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjNoiseMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjDiffuseMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjNormalMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjNormalDetailMap;
	//	char                               _pad0[0x1C];
	//	float                              unknown[2];
	//	float                              jitter[2];
	//	uint32_t                           uiFrameCount;
	//	bool                               bInsideFrame;
	//	bool                               bLetterbox;
	//	bool                               bUnknown1;
	//	bool                               bCompiledShaderThisFrame;
	//	bool                               bUseEarlyZ;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureBlack;  // "BSShader_DefHeightMap"
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureWhite;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureGrey;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultHeightMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultReflectionCubeMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultFaceDetailMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTexEffectMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureNormalMap;
	//	RE::NiPointer<RE::NiSourceTexture> pDefaultTextureDitherNoiseMap;
	//	RE::BSTArray<CameraStateData>      kCameraDataCacheA;
	//	float                              _pad2;                  // unknown dword
	//	float                              fHaltonSequence[2][8];  // (2, 3) Halton Sequence points
	//	float                              fDynamicResolutionCurrentWidthScale;
	//	float                              fDynamicResolutionCurrentHeightScale;
	//	float                              fDynamicResolutionPreviousWidthScale;
	//	float                              fDynamicResolutionPreviousHeightScale;
	//	float                              fDynamicResolutionWidthRatio;
	//	float                              fDynamicResolutionHeightRatio;
	//	uint16_t                           usDynamicResolutionCounter;
	//};
	//static_assert(sizeof(State) == 0x118);

	struct State
	{
		RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjNoiseMap;
		RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjDiffuseMap;
		RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjNormalMap;
		RE::NiPointer<RE::NiSourceTexture> pDefaultTextureProjNormalDetailMap;
		char                               _pad0[0x1C];
		float                              unknown[2];
		float                              jitter[2];

		//unsure whether the following haven't moved in AE, couldn't find any place where they're used
		uint32_t                           uiFrameCount;
		bool                               bInsideFrame;
		bool                               bLetterbox;
		bool                               bUnknown1;
		bool                               bCompiledShaderThisFrame;
		bool                               bUseEarlyZ;
		// ...

		// somewhere in the middle of this struct, there are bytes in AE that are not present in SE
		// therefore use GetRuntimeData to get the rest
		
		struct RUNTIME_DATA
		{
			RE::NiPointer<RE::NiSourceTexture> pDefaultTextureBlack;  // "BSShader_DefHeightMap"
			RE::NiPointer<RE::NiSourceTexture> pDefaultTextureWhite;
			RE::NiPointer<RE::NiSourceTexture> pDefaultTextureGrey;
			RE::NiPointer<RE::NiSourceTexture> pDefaultHeightMap;
			RE::NiPointer<RE::NiSourceTexture> pDefaultReflectionCubeMap;
			RE::NiPointer<RE::NiSourceTexture> pDefaultFaceDetailMap;
			RE::NiPointer<RE::NiSourceTexture> pDefaultTexEffectMap;
			RE::NiPointer<RE::NiSourceTexture> pDefaultTextureNormalMap;
			RE::NiPointer<RE::NiSourceTexture> pDefaultTextureDitherNoiseMap;
			RE::BSTArray<CameraStateData>      kCameraDataCacheA;
			float                              _pad2;                  // unknown dword
			float                              fHaltonSequence[2][8];  // (2, 3) Halton Sequence points
			float                              fDynamicResolutionCurrentWidthScale;
			float                              fDynamicResolutionCurrentHeightScale;
			float                              fDynamicResolutionPreviousWidthScale;
			float                              fDynamicResolutionPreviousHeightScale;
			float                              fDynamicResolutionWidthRatio;
			float                              fDynamicResolutionHeightRatio;
			uint16_t                           usDynamicResolutionCounter;
		};
		static_assert(offsetof(RUNTIME_DATA, fDynamicResolutionCurrentWidthScale) == 0xA4);

		[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RelocateMemberIfNewer<RUNTIME_DATA>(SKSE::RUNTIME_SSE_1_6_317, this, 0x58, 0x60);
		}

		[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RelocateMemberIfNewer<RUNTIME_DATA>(SKSE::RUNTIME_SSE_1_6_317, this, 0x58, 0x60);
		}
	};
	
	struct RenderTargetData
	{
		ID3D11Texture2D*           Texture;
		ID3D11Texture2D*           TextureCopy;
		ID3D11RenderTargetView*    RTV;      // For "Texture"
		ID3D11ShaderResourceView*  SRV;      // For "Texture"
		ID3D11ShaderResourceView*  SRVCopy;  // For "TextureCopy"
		ID3D11UnorderedAccessView* UAV;      // For "Texture"
	};
	static_assert(sizeof(RenderTargetData) == 0x30);

}

enum RenderTargetType : uint32_t
{
	RENDER_TARGET_NONE = 0xFFFFFFFF,
	RENDER_TARGET_FRAMEBUFFER = 0,
	RENDER_TARGET_MAIN,
	RENDER_TARGET_MAIN_COPY,
	RENDER_TARGET_MAIN_ONLY_ALPHA,
	RENDER_TARGET_NORMAL_TAAMASK_SSRMASK,
	RENDER_TARGET_NORMAL_TAAMASK_SSRMASK_SWAP,
	RENDER_TARGET_NORMAL_TAAMASK_SSRMASK_DOWNSAMPLED,
	RENDER_TARGET_MOTION_VECTOR,
	RENDER_TARGET_WATER_DISPLACEMENT,
	RENDER_TARGET_WATER_DISPLACEMENT_SWAP,
	RENDER_TARGET_WATER_REFLECTIONS,
	RENDER_TARGET_WATER_FLOW,
	RENDER_TARGET_UNDERWATER_MASK,
	RENDER_TARGET_REFRACTION_NORMALS,
	RENDER_TARGET_MENUBG,
	RENDER_TARGET_PLAYER_FACEGEN_TINT,
	RENDER_TARGET_LOCAL_MAP,
	RENDER_TARGET_LOCAL_MAP_SWAP,
	RENDER_TARGET_SHADOW_MASK,
	RENDER_TARGET_GETHIT_BUFFER,
	RENDER_TARGET_GETHIT_BLURSWAP,
	RENDER_TARGET_BLURFULL_BUFFER,
	RENDER_TARGET_HDR_BLURSWAP,
	RENDER_TARGET_LDR_BLURSWAP,
	RENDER_TARGET_HDR_BLOOM,
	RENDER_TARGET_LDR_DOWNSAMPLE0,
	RENDER_TARGET_HDR_DOWNSAMPLE0,
	RENDER_TARGET_HDR_DOWNSAMPLE1,
	RENDER_TARGET_HDR_DOWNSAMPLE2,
	RENDER_TARGET_HDR_DOWNSAMPLE3,
	RENDER_TARGET_HDR_DOWNSAMPLE4,
	RENDER_TARGET_HDR_DOWNSAMPLE5,
	RENDER_TARGET_HDR_DOWNSAMPLE6,
	RENDER_TARGET_HDR_DOWNSAMPLE7,
	RENDER_TARGET_HDR_DOWNSAMPLE8,
	RENDER_TARGET_HDR_DOWNSAMPLE9,
	RENDER_TARGET_HDR_DOWNSAMPLE10,
	RENDER_TARGET_HDR_DOWNSAMPLE11,
	RENDER_TARGET_HDR_DOWNSAMPLE12,
	RENDER_TARGET_HDR_DOWNSAMPLE13,
	RENDER_TARGET_LENSFLAREVIS,
	RENDER_TARGET_IMAGESPACE_TEMP_COPY,
	RENDER_TARGET_IMAGESPACE_TEMP_COPY2,
	RENDER_TARGET_IMAGESPACE_VOLUMETRIC_LIGHTING,
	RENDER_TARGET_IMAGESPACE_VOLUMETRIC_LIGHTING_PREVIOUS,
	RENDER_TARGET_IMAGESPACE_VOLUMETRIC_LIGHTING_COPY,
	RENDER_TARGET_SAO,
	RENDER_TARGET_SAO_DOWNSCALED,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_0_ESRAM,
	RENDER_TARGET_SAO_CAMERAZ,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_0,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_1,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_2,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_3,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_4,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_5,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_6,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_7,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_8,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_9,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_10,
	RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_11,
	RENDER_TARGET_SAO_RAWAO,
	RENDER_TARGET_SAO_RAWAO_DOWNSCALED,
	RENDER_TARGET_SAO_RAWAO_PREVIOUS,
	RENDER_TARGET_SAO_RAWAO_PREVIOUS_DOWNSCALED,
	RENDER_TARGET_SAO_TEMP_BLUR,
	RENDER_TARGET_SAO_TEMP_BLUR_DOWNSCALED,
	RENDER_TARGET_INDIRECT,
	RENDER_TARGET_INDIRECT_DOWNSCALED,
	RENDER_TARGET_RAWINDIRECT,
	RENDER_TARGET_RAWINDIRECT_DOWNSCALED,
	RENDER_TARGET_RAWINDIRECT_PREVIOUS,
	RENDER_TARGET_RAWINDIRECT_PREVIOUS_DOWNSCALED,
	RENDER_TARGET_RAWINDIRECT_SWAP,
	RENDER_TARGET_SAVE_GAME_SCREENSHOT,
	RENDER_TARGET_SCREENSHOT,
	RENDER_TARGET_VOLUMETRIC_LIGHTING_HALF_RES,
	RENDER_TARGET_VOLUMETRIC_LIGHTING_BLUR_HALF_RES,
	RENDER_TARGET_VOLUMETRIC_LIGHTING_QUARTER_RES,
	RENDER_TARGET_VOLUMETRIC_LIGHTING_BLUR_QUARTER_RES,
	RENDER_TARGET_TEMPORAL_AA_ACCUMULATION_1,
	RENDER_TARGET_TEMPORAL_AA_ACCUMULATION_2,
	RENDER_TARGET_TEMPORAL_AA_UI_ACCUMULATION_1,
	RENDER_TARGET_TEMPORAL_AA_UI_ACCUMULATION_2,
	RENDER_TARGET_TEMPORAL_AA_MASK,
	RENDER_TARGET_TEMPORAL_AA_WATER_1,
	RENDER_TARGET_TEMPORAL_AA_WATER_2,
	RENDER_TARGET_RAW_WATER,
	RENDER_TARGET_WATER_1,
	RENDER_TARGET_WATER_2,
	RENDER_TARGET_IBLENSFLARES_LIGHTS_FILTER,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_4X_4X_PING,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_4X_4X_PONG,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_PING,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_PONG,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_BLUR,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_BLUR_SWAP,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_PING,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_PONG,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_BLUR,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_BLUR_SWAP,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_16X_PING,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_16X_PONG,
	RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_16X_SWAP,
	RENDER_TARGET_BOOK_TEXT_0,
	RENDER_TARGET_BOOK_TEXT_1,
	RENDER_TARGET_BOOK_TEXT_2,
	RENDER_TARGET_BOOK_TEXT_3,
	RENDER_TARGET_SSR,
	RENDER_TARGET_SSR_RAW,
	RENDER_TARGET_SSR_BLURRED0,
	RENDER_TARGET_SNOW_SPECALPHA,
	RENDER_TARGET_SNOW_SWAP,

	RENDER_TARGET_COUNT,
	RENDER_TARGET_FRAMEBUFFER_COUNT = 1,
};
