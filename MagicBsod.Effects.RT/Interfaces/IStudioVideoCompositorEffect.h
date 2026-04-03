#pragma once
#include "IStudioGenericEffect.h"

using namespace Windows::Graphics::DirectX::Direct3D11;
using namespace Windows::Media::MediaProperties;

namespace MagicBsod_Effects_RT::Interfaces
{
	public interface class IStudioVideoCompositorEffect : IStudioGenericEffect
	{
		virtual property bool TimeIndependent
		{
			virtual bool get() = 0;
		}

		virtual void SetEncodingProperties(VideoEncodingProperties^ encodingProperties, IDirect3DDevice^ device) = 0;
		virtual void CompositeFrame(CompositeVideoFrameContext^ context) = 0;
	};

	public delegate IStudioVideoCompositorEffect^ VideoCompositorEffectConstructor();
}