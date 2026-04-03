#include "pch.h"
#include "StudioVideoCompositorEffectTemplate.h"
#include <Interfaces/IStudioVideoCompositorEffect.h>

MagicBsod_Effects_RT::Interfaces::VideoCompositorEffectConstructor^ MagicBsod_Effects_RT::VideoCompositor::StudioVideoCompositorEffectTemplate::videoCompositorEffectConstructorReference = nullptr;

MagicBsod_Effects_RT::VideoCompositor::StudioVideoCompositorEffectTemplate::StudioVideoCompositorEffectTemplate()
{
	effectInstance = videoCompositorEffectConstructorReference();
}

void MagicBsod_Effects_RT::VideoCompositor::StudioVideoCompositorEffectTemplate::SetProperties(IPropertySet^ configuration)
{
	effectInstance->SetProperties(configuration);
}

void MagicBsod_Effects_RT::VideoCompositor::StudioVideoCompositorEffectTemplate::SetEncodingProperties(VideoEncodingProperties^ backgroundProperties, IDirect3DDevice^ device)
{
	effectInstance->SetEncodingProperties(backgroundProperties, device);
}

void MagicBsod_Effects_RT::VideoCompositor::StudioVideoCompositorEffectTemplate::CompositeFrame(CompositeVideoFrameContext^ context)
{
	effectInstance->CompositeFrame(context);
}

void MagicBsod_Effects_RT::VideoCompositor::StudioVideoCompositorEffectTemplate::Close(MediaEffectClosedReason reason)
{
	effectInstance->Close(reason);
}

void MagicBsod_Effects_RT::VideoCompositor::StudioVideoCompositorEffectTemplate::DiscardQueuedFrames()
{
	effectInstance->DiscardQueuedFrames();
}
