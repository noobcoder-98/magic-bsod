#pragma once
#include <Interfaces/IStudioVideoCompositorEffect.h>

using namespace Windows::Media::Effects;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::DirectX::Direct3D11;
using namespace MagicBsod_Effects_RT::Interfaces;

namespace MagicBsod_Effects_RT::VideoCompositor {
	public ref class StudioVideoCompositorEffectTemplate sealed : IVideoCompositor
	{
	private:
		static VideoCompositorEffectConstructor^ videoCompositorEffectConstructorReference;
		IStudioVideoCompositorEffect^ effectInstance;
	public:

		static property VideoCompositorEffectConstructor^ ActivatableClassConstructor
		{
			VideoCompositorEffectConstructor^ get() {
				return videoCompositorEffectConstructorReference;
			}
			void set(VideoCompositorEffectConstructor^ value) {
				videoCompositorEffectConstructorReference = value;
			}
		}

		StudioVideoCompositorEffectTemplate();

		virtual property bool TimeIndependent
		{
			virtual bool get() {
				return effectInstance->TimeIndependent;
			}
		}
		virtual void SetProperties(IPropertySet^ configuration);
		virtual void SetEncodingProperties(VideoEncodingProperties^ backgroundProperties, IDirect3DDevice^ device);
		virtual void CompositeFrame(CompositeVideoFrameContext^ context);
		virtual void Close(MediaEffectClosedReason reason);
		virtual void DiscardQueuedFrames();
	};

}

